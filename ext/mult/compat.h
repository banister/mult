/* contains basic macros to facilitate ruby 1.8 and ruby 1.9 compatibility */

#ifndef GUARD_COMPAT_H
#define GUARD_COMPAT_H

#include <ruby.h>

/* this is the test we use to identify ruby 1.9.1 */

/* commenting out this test because it doesn't seem to work in 1.8.7 */
/*
#ifdef RCLASS_M_TBL
# define RUBY_19
#endif
*/

/* macros for backwards compatibility with 1.8 */
#ifndef RCLASS_M_TBL
# define RCLASS_M_TBL(c) (RCLASS(c)->m_tbl)
# define RCLASS_SUPER(c) (RCLASS(c)->super)
# define RCLASS_IV_TBL(c) (RCLASS(c)->iv_tbl)
#endif

/* a useful macro. cannot use ordinary CLASS_OF as it does not return an lvalue */
#define KLASS_OF(c) (RBASIC(c)->klass)

#endif
