#include <ruby.h>
#include "compat.h"

/* return the actual class of an object */
static VALUE
get_class(VALUE self) {    
    return CLASS_OF(self);
}

/* set the class of an object */
static VALUE
set_class(VALUE self, VALUE klass) {
    KLASS_OF(self) = klass;
    return klass;
}


/* get the actual super class of an object */
static VALUE
get_super(VALUE self)
{
    return RCLASS_SUPER(self);
}

/* set the super class of an object */
static VALUE
set_super(VALUE self, VALUE klass) {    
    RCLASS_SUPER(self) = klass;
    return klass;
}

/* is the object a singleton class ? */
static VALUE
is_singleton(VALUE self, VALUE klass) {    
    return FL_TEST(self, FL_SINGLETON) ? Qtrue : Qfalse;
}

/* get the attached class if receiver is a singleton class */
static VALUE
get_attached(VALUE self)
{
    if(FL_TEST(self, FL_SINGLETON))
        return rb_iv_get(self, "__attached__");
    else
        return Qnil;
}


/* set the attached class if receiver is a singleton class */
static VALUE
set_attached(VALUE self, VALUE klass) { 
    if(FL_TEST(self, FL_SINGLETON)) { 
        rb_iv_set(self, "__attached__", klass);
        return self;
    }
    else
        return Qnil;
}

void 
Init_mult() {
    rb_define_method(rb_cObject, "actual_class", get_class, 0);
    rb_define_method(rb_cObject, "actual_class=", set_class, 1);

    rb_define_method(rb_cClass, "actual_super", get_super, 0);
    rb_define_method(rb_cClass, "actual_super=", set_super, 1);

    rb_define_method(rb_cClass, "is_singleton?", is_singleton, 0);
    
    rb_define_method(rb_cClass, "attached", get_attached, 0);
    rb_define_method(rb_cClass, "attached=", set_attached, 1);
}
