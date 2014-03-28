#ifndef _LINKER_H_
#define _lINKER_H_

// link the loaded classes
void link();

// link a concrete class(alway is the lasted loaded class)
void link_class(struct Class* pclass);
//void link_class(struct class_entry* pclass);

#endif