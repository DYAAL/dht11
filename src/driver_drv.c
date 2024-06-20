#include "driver_drv.h"
#include "dev_drv.h"
#include "marcos_drv.h"
#include <linux/device.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>

#include <asm/string.h>


static void minor_struct_init(Driver_Register_Struct* this)
{
    int i;
    for (i = 0; i < MAX_MINOR; ++i) {
        this->minor_struct.used[i] = false;
    }
    this->minor_struct.current_minor = -1; // Initialize current_minor to an invalid value
}

int allocate_minor(Driver_Register_Struct* this)
{
    int i;
    for (i = 0; i < MAX_MINOR; ++i) {
        if (!this->minor_struct.used[i]) {
            this->minor_struct.used[i] = true;
            this->minor_struct.current_minor = i;
            return 0;
        }
    }
    return -1; // 如果没有可用的minor，则返回-1
}

void release_minor(int minor, Driver_Register_Struct* this)
{
    if (minor >= 0 && minor < MAX_MINOR) {
        if (this->minor_struct.used[minor]) {
            this->minor_struct.used[minor] = false;
        } else {
            printk(KERN_ERR "release_minor: minor number %d not in use\n", minor);
        }
    } else {
        printk(KERN_ERR "release_minor: invalid minor number %d\n", minor);
    }
}

void init_struct(Driver_Register_Struct_P this)
{
    memset(this, 0, sizeof(Driver_Register_Struct));
    this->major = 0;
    this->cls = NULL;
    this->cls_name = "sr04_class_nam";
    this->chr_dev_name = "sr04_chr_dev_nam";
    minor_struct_init(this);
}
