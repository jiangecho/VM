#ifndef _CLASSLOADER_H_
#define _CLASSLOADER_H_


#define PRIINT_CLASS_STRUCTURE 1

//parse the class file
struct Class* load_class(char* pclass_path);
void load_pending_classes();
void init_class_loader();

#endif