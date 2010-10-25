
#include <ruby.h>
#include "compat.h"

static VALUE cRubyTable;

typedef struct {
  st_table* table;
} RubyTable;

/* return the actual class of an object */
static VALUE
global_get_class(VALUE self, VALUE klass) {    
  return CLASS_OF(klass);
}

static VALUE
global_set_class(VALUE self, VALUE klass, VALUE new_klass) {    
  KLASS_OF(klass) = new_klass;
  return klass;
}

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
global_get_super(VALUE self, VALUE klass)
{
  return RCLASS_SUPER(klass);
}

static VALUE
global_set_super(VALUE self, VALUE klass, VALUE sup)
{
  return RCLASS_SUPER(klass) = sup;
}

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
global_get_ivar(VALUE self, VALUE klass, VALUE sym)
{
  return rb_ivar_get(klass, rb_to_id(sym));
}

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

static VALUE
global_set_ivar(VALUE self, VALUE klass, VALUE sym, VALUE val)
{
  return rb_ivar_set(klass, rb_to_id(sym), val);
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

/* generate table get methods */
#define CREATE_GET_TBL_FUNC(func_name, table_type) \
  static VALUE func_name(VALUE self) {             \
    RubyTable * tbl_struct = ALLOC(RubyTable);    \
    if (!table_type(self))              \
      table_type(self) = st_init_numtable();      \
    tbl_struct->table = table_type(self);            \
    return Data_Wrap_Struct(cRubyTable, 0, free, tbl_struct);   \
  }                                                                     

/* generate table set methods */
#define CREATE_SET_TBL_FUNC(func_name, table_type) \
  static VALUE                                     \
  func_name(VALUE self, VALUE rb_tbl) { \
    Check_Type(rb_tbl, T_DATA); \
    RubyTable * tbl_struct; \
    Data_Get_Struct(rb_tbl, RubyTable, tbl_struct); \
    table_type(self) = tbl_struct->table; \
    rb_clear_cache(); \
    return self; \
  }
  
/* get the method table */
CREATE_GET_TBL_FUNC(get_m_tbl, RCLASS_M_TBL)
/* set the method table */
CREATE_SET_TBL_FUNC(set_m_tbl, RCLASS_M_TBL)

/* get the iv table */
CREATE_GET_TBL_FUNC(get_iv_tbl, RCLASS_IV_TBL)
/* set the iv table */
CREATE_SET_TBL_FUNC(set_iv_tbl, RCLASS_IV_TBL)

static VALUE
set_flags(VALUE self, VALUE flag)
{
  Check_Type(flag, T_FIXNUM);
  FL_SET(self, FIX2INT(flag));
  return self;
}

static VALUE
get_flags(VALUE self)
{
  return INT2FIX(RBASIC(self)->flags);
}

static VALUE
has_flag_p(VALUE self, VALUE flag)
{
  Check_Type(flag, T_FIXNUM);
  return FL_TEST(self, FIX2INT(flag)) ? Qtrue : Qfalse;
}

void 
Init_mult() {

  cRubyTable = rb_define_class("RubyTable", rb_cObject);
  

  rb_define_method(rb_cObject, "RCLASS_GET", global_get_class, 1);
  rb_define_method(rb_cObject, "RCLASS_SET", global_set_class, 2);
    
  rb_define_method(rb_cObject, "actual_class", get_class, 0);
  rb_define_method(rb_cObject, "actual_class=", set_class, 1);

  rb_define_method(rb_cObject, "IVAR_GET", global_get_ivar, 2);
  rb_define_method(rb_cObject, "IVAR_SET", global_set_ivar, 3);

  rb_define_method(rb_cObject, "ivar_get", get_ivar, 1);
  rb_define_method(rb_cObject, "ivar_set", set_ivar, 2);

  rb_define_method(rb_cModule, "m_tbl", get_m_tbl, 0);
  rb_define_method(rb_cModule, "m_tbl=", set_m_tbl, 1);
  rb_define_method(rb_cModule, "iv_tbl", get_iv_tbl, 0);
  rb_define_method(rb_cModule, "iv_tbl=", set_iv_tbl, 1);


  rb_define_method(rb_cObject, "RCLASS_SUPER_GET", global_get_super, 1);
  rb_define_method(rb_cObject, "RCLASS_SUPER_SET", get_super, 1);

  rb_define_method(rb_cModule, "actual_super", get_super, 0);
  rb_define_method(rb_cModule, "actual_super=", set_super, 1);

  rb_define_method(rb_cClass, "is_singleton?", is_singleton, 0);
    
  rb_define_method(rb_cClass, "attached", get_attached, 0);
  rb_define_method(rb_cClass, "attached=", set_attached, 1);

  rb_define_method(rb_cObject, "has_flag?", has_flag_p, 1);
  rb_define_method(rb_cObject, "flags=", set_flags, 1);
  rb_define_method(rb_cObject, "flags", get_flags, 0);

  // constants
  rb_define_const(rb_cObject, "T_CLASS", INT2FIX(T_CLASS));
  rb_define_const(rb_cObject, "T_MODULE", INT2FIX(T_MODULE));
  rb_define_const(rb_cObject, "T_OBJECT", INT2FIX(T_OBJECT));
  rb_define_const(rb_cObject, "T_DATA", INT2FIX(T_DATA));
  rb_define_const(rb_cObject, "T_ICLASS", INT2FIX(T_ICLASS));
  rb_define_const(rb_cObject, "FL_FREEZE", INT2FIX(FL_FREEZE));
  rb_define_const(rb_cObject, "FL_SINGLETON", INT2FIX(FL_SINGLETON));
}
