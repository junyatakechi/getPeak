#include "ext.h"							// standard Max include, always required
#include "ext_obex.h"						// required for new style Max object
#include "ext_buffer.h"
#include "buffer.h"

typedef struct _{
    t_object s_obj;
    t_buffer_ref *l_buffer_reference;
    t_buffer_obj *buffer;
}t_start;
static t_class *s_start_class; // global pointer to our class definition that is setup in ext_main()

void *start_new();
void start_list(t_start *x, t_symbol *s, long argc, t_atom *argv);
t_symbol w_name;
t_atom_long buffer_len;
float *w_header;
float *w_header_init;
t_atom_long w_header_count;
void start_bang(t_start *x);


void ext_main(void *r)
{
    t_class *c;
    c = class_new("start", (method)start_new, (method)NULL, sizeof(t_start), 0L, 0);
    class_addmethod(c, (method)start_list, "list", A_GIMME, 0); //list
    class_addmethod(c, (method)start_bang, "bang", 0); //bang

    class_register(CLASS_BOX, c);
    s_start_class = c;
}

void start_bang(t_start *x){
    w_name.s_name = "ring";
    x->l_buffer_reference = buffer_ref_new((t_object *)x, &w_name);
    x->buffer = buffer_ref_getobject(x->l_buffer_reference);
    buffer_len = buffer_getframecount(x->buffer);
    w_header = buffer_locksamples(x->buffer);
    w_header_init = buffer_locksamples(x->buffer);
    w_header_count = 0;
    post("Total ring sample: %d", buffer_len);
}

void *start_new()
{
    t_start *x = (t_start *)object_alloc(s_start_class);
    return x;
}

void start_list(t_start *x, t_symbol *s, long argc, t_atom *argv){
    t_atom *ap;
    float samp;
    ap = argv;
    t_atom_long list_len = argc;
    
    // loop buffer writing.
    if(w_header_count + list_len > buffer_len){
        post("Loop buffer");
        w_header = w_header_init;
        w_header_count = 0;
    }

    int i;
    for (i = 0; i < list_len; i++, ap++, w_header++){
        samp = ap->a_w.w_float;
        *w_header = samp;
    }
    buffer_unlocksamples(x->buffer);
    w_header_count += list_len;
    post("w_header_count %d / %d", w_header_count, buffer_len);
}
