#ifndef _CLASSLOADER_H_
#define _CLASSLOADER_H_

#define CLASS_LOADING     0
#define CLASS_LOADED      1
#define CLASS_PREPARATION 2
#define CLASS_RESOLUTION  3
#define CLASS_INITIALIZATION 4

#define PRIINT_CLASS_STRUCTURE 1

//parse the class file
struct Class* load_class(char* pclass_path);
void load_pending_classes();
void init_class_loader();

#endif