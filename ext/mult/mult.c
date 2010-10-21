#include <ruby.h>
#include "compat.h"

static VALUE cRubyTable;

typedef struct {
  st_table* table;
} RubyTable;

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

/* get a raw instance var */
static VALUE
get_ivar(VALUE self, VALUE sym)
{
  return rb_ivar_get(self, rb_to_id(sym));
}

/* set a raw instance var */
static VALUE
set_ivar(VALUE self, VALUE sym, VALUE val)
{
  return rb_ivar_set(self, rb_to_id(sym), val);
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

/* get the method table */
static VALUE
get_m_tbl(VALUE self) {
  RubyTable * m_tbl_struct = ALLOC(RubyTable);

  if (!RCLASS_M_TBL(self))
    RCLASS_M_TBL(self) = st_init_numtable();

  m_tbl_struct->table = RCLASS_M_TBL(self);
  VALUE ret = Data_Wrap_Struct(cRubyTable, 0, free, m_tbl_struct);

  return ret;
}

/* set the method table */
static VALUE
set_m_tbl(VALUE self, VALUE rb_m) {
  Check_Type(rb_m, T_DATA);
  
  RubyTable * m_tbl_struct;
  Data_Get_Struct(rb_m, RubyTable, m_tbl_struct);
  
  RCLASS_M_TBL(self) = m_tbl_struct->table;

  rb_clear_cache();
  
  return self;
}

void 
Init_mult() {

  cRubyTable = rb_define_class("RubyTable", rb_cObject);
  
  rb_define_method(rb_cObject, "actual_class", get_class, 0);
  rb_define_method(rb_cObject, "actual_class=", set_class, 1);

  rb_define_method(rb_cObject, "ivar_get", get_ivar, 1);
  rb_define_method(rb_cObject, "ivar_set", set_ivar, 2);

  rb_define_method(rb_cModule, "m_tbl", get_m_tbl, 0);
  rb_define_method(rb_cModule, "m_tbl=", set_m_tbl, 1);

  rb_define_method(rb_cModule, "actual_super", get_super, 0);
  rb_define_method(rb_cModule, "actual_super=", set_super, 1);

  rb_define_method(rb_cClass, "is_singleton?", is_singleton, 0);
    
  rb_define_method(rb_cClass, "attached", get_attached, 0);
  rb_define_method(rb_cClass, "attached=", set_attached, 1);
}
