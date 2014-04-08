#ifndef _METHOD_H_
#define _METHOD_H_
#include "def.h"
#include "class.h"

//TODO move all method related definition to this header file

struct code_attribute_info* get_code_attribute_info(struct method_info* pmethod_info);
u2 get_parameters_size(struct method_info* pmethod_info);
//u1* get_code_attribute_address(struct method_info* pmethod);


#endif