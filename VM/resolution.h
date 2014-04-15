#ifndef _RESOLUTION_H_
#define _RESOLUTION_H_

#include "def.h"
#include "class.h"

void resolution(struct Class* pclass, u2 index_in_constant_pool);
u1 resolution_method(struct Class* pclass, struct constant_methodref_info* pconstant_methodref_info);
u1 resolution_field(struct Class* pclass,  struct constant_fieldref_info* pconstant_fieldref_info);

#endif