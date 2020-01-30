#include "ext.h"							// standard Max include, always required
#include "ext_obex.h"						// required for new style Max object
#include "ext_buffer.h"

typedef struct _{
    t_object s_obj;
    t_buffer_ref *l_buffer_reference;
    t_buffer_obj *buffer;
    t_symbol w_name;
    t_atom_long buffer_len;
    float *header;
    void *outlet1;
    void *outlet2;
    void *outlet3;
    void *outlet4;
}t_getPeak;

static t_class *s_getPeak_class; // global pointer to our class definition that is setup in ext_main()
void *getPeak_new();
void getPeak_list(t_getPeak *x, t_symbol *s, long argc, t_atom *argv);
void getPeak_bang(t_getPeak *x);
void getPeak_set(t_getPeak *x, t_symbol *s, long argc, t_atom *argv);


void ext_main(void *r)
{
    t_class *c;
    c = class_new("getPeak", (method)getPeak_new, (method)NULL, sizeof(t_getPeak), 0L, 0);
//    class_addmethod(c, (method)getPeak_bang, "bang", 0); //bang
//    class_addmethod(c, (method)getPeak_set, "set", A_GIMME, 0);
    class_addmethod(c, (method)getPeak_list, "list", A_GIMME, 0);

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

void getPeak_list(t_getPeak *x, t_symbol *s, long argc, t_atom *argv){
    unsigned long range = 0; // needs three points
    float samps[3];
    float idx[3];
    unsigned int flag_peak1 = 0;
    unsigned int flag_peak2 = 0;

    t_atom *ap = argv;
    
    for(unsigned long i = 0; i < argc; i++, ap++){
        samps[range] = ap->a_w.w_float;
        idx[range] = i;
        
        range += 1;
        if(range > 2){
            if(samps[1] > samps[0] && samps[1] > samps[2]){
                if(flag_peak1 == 0){
                    outlet_int(x->outlet1, idx[1]);
                    outlet_float(x->outlet2, samps[1]);
                    flag_peak1 = 1;
                }else if(flag_peak2 == 0){
                    outlet_int(x->outlet3, idx[1]);
                    outlet_float(x->outlet4, samps[1]);
                    flag_peak2 = 1;
                }else{
                    break;
                }

            }
            samps[0] = samps[2];
            idx[0] = idx[2];
            range = 1;
        }

    }

}




//void getPeak_set(t_getPeak *x, t_symbol *s, long argc, t_atom *argv)
//{
//    //post("%s", argv->a_w.w_sym->s_name);
//    x->w_name.s_name = argv->a_w.w_sym->s_name;
//    x->l_buffer_reference = buffer_ref_new((t_object *)x, &x->w_name);
//    x->buffer = buffer_ref_getobject(x->l_buffer_reference);
//    x->buffer_len = buffer_getframecount(x->buffer);
//
//    post("Buffer: %s is %d samples", x->w_name.s_name, x->buffer_len);
//}
//
//void getPeak_bang(t_getPeak *x){
//    x->header = buffer_locksamples(x->buffer);
//    unsigned long range = 0; // needs three points
//    float n0 = 0;
//    float n1 = 0;
//    float n2 = 0;
//    unsigned long bin1 = 0;
//    unsigned int flag_peak1 = 0;
//    unsigned int flag_peak2 = 0;
//
//    for(unsigned long i = 0; i < x->buffer_len / 2; i++, x->header++){
//        if(range == 0){
//            n0 = *x->header;
//        }else if(range == 1){
//            n1 = *x->header;
//            bin1 = i;
//        }else{
//            n2 = *x->header;
//        }
//
//        if(n1 > n0 && n1 > n2){ // output conditions
//            if(flag_peak1 == 0){
//                outlet_int(x->outlet1, bin1);
//                outlet_float(x->outlet2, n1);
//                flag_peak1 = 1;
//            }else if(flag_peak2 == 0){
//                outlet_int(x->outlet3, bin1);
//                outlet_float(x->outlet4, n1);
//                flag_peak2 = 1;
//            }else{
//                break;
//            }
//        }
//
//        range += 1;
//        if(range > 2){
//            range = 0;
//        }
//
//
//    }
//    buffer_unlocksamples(x->buffer);
//
//}

