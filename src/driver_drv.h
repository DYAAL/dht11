#ifndef __DRIVER_DRV_H__
#define __DRIVER_DRV_H__
#include <linux/types.h>
#include "marcos_drv.h"
typedef struct {
    bool used[MAX_MINOR];
    int current_minor;
} Minor_Struct, *Minor_Struct_P;
typedef struct {
    int major;
    struct class* cls;
    char* cls_name;
    char* chr_dev_name;
    Minor_Struct minor_struct;
} Driver_Register_Struct, *Driver_Register_Struct_P;

void print_kernel_infor(Infor_Level infor_level);
int allocate_minor(Driver_Register_Struct* this);
void release_minor(int minor, Driver_Register_Struct* this);
void init_struct(Driver_Register_Struct_P this);

#endif // __DRIVER_DRV_H__