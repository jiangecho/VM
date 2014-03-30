#ifndef _LINKER_H_
#define _lINKER_H_

// link the loaded classes
void link();
void link_class(struct Class* pclass);

// link a concrete class(alway is the lasted loaded class)
//void link_constant_pool(struct Class* pclass);

#endif