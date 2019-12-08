#include "ext.h"							// standard Max include, always required
#include "ext_obex.h"						// required for new style Max object
#include "ext_buffer.h"
#include "buffer.h"

typedef struct _{
    t_object s_obj;
    t_buffer_ref *l_buffer_reference;
    t_buffer_obj *buffer;
    void *outlet1;
    void *outlet2;
    void *outlet3;
    void *outlet4;
}t_getPeak;
static t_class *s_getPeak_class; // global pointer to our class definition that is setup in ext_main()

void *getPeak_new();
void getPeak_list(t_getPeak *x, t_symbol *s, long argc, t_atom *argv);
t_symbol w_name;
t_atom_long buffer_len;
float *header;
void getPeak_bang(t_getPeak *x);
void getPeak_set(t_getPeak *x, t_symbol *s, long argc, t_atom *argv);


void ext_main(void *r)
{
    t_class *c;
    c = class_new("getPeak", (method)getPeak_new, (method)NULL, sizeof(t_getPeak), 0L, 0);
    class_addmethod(c, (method)getPeak_bang, "bang", 0); //bang
    class_addmethod(c, (method)getPeak_set, "set", A_GIMME, 0);

    class_register(CLASS_BOX, c);
    s_getPeak_class = c;
}

void *getPeak_new()
{
    t_getPeak *x = (t_getPeak *)object_alloc(s_getPeak_class);
    x->outlet4 = floatout((t_object *)x);
    x->outlet3 = intout((t_object *)x);
    x->outlet2 = floatout((t_object *)x);
    x->outlet1 = intout((t_object *)x);
    return x;
}

void getPeak_set(t_getPeak *x, t_symbol *s, long argc, t_atom *argv)
{
    //post("%s", argv->a_w.w_sym->s_name);
    w_name.s_name = argv->a_w.w_sym->s_name;
    x->l_buffer_reference = buffer_ref_new((t_object *)x, &w_name);
    x->buffer = buffer_ref_getobject(x->l_buffer_reference);
    buffer_len = buffer_getframecount(x->buffer);
    
    post("Buffer: %s is %d samples", w_name.s_name, buffer_len);
}

void getPeak_bang(t_getPeak *x){
    header = buffer_locksamples(x->buffer);
    int idx_arr[buffer_len];
    for(int i = 0; i < buffer_len; i++){
        idx_arr[i] = i;
    }
    int tmp_i;
    float tmp;
    for(int p = 0; p < buffer_len; p++){
        for(int q = p + 1; q < buffer_len; q++){
            if(header[p] < header[q]){
                ////
                tmp = header[p];
                tmp_i = idx_arr[p];
                ///
                header[p] = header[q];
                idx_arr[p] = idx_arr[q];
                ///
                header[q] = tmp;
                idx_arr[q] = tmp_i;
                ///
            }
        }
    }

    //check in log
//    for (int i=0; i<buffer_len; i++){
//        post("%d => %f", idx_arr[i], header[i]);
//    }
    if(idx_arr[0] < idx_arr[1]){
        outlet_int(x->outlet1, idx_arr[0]);
        outlet_float(x->outlet2, header[0]);
        outlet_int(x->outlet3, idx_arr[1]);
        outlet_float(x->outlet4, header[1]);
    }else{
        outlet_int(x->outlet1, idx_arr[1]);
        outlet_float(x->outlet2, header[1]);
        outlet_int(x->outlet3, idx_arr[0]);
        outlet_float(x->outlet4, header[0]);
    }

    
}
