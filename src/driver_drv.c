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

void print_kernel_infor(Infor_Level infor_level)
{
    switch (infor_level) {
    case kern_emerg:
        printk(KERN_EMERG "file: %s, function: %s, line: %d\n", __FILE__, __func__, __LINE__);
        break;
    case kern_alert:
        printk(KERN_ALERT "file: %s, function: %s, line: %d\n", __FILE__, __func__, __LINE__);
        break;
    case kern_crit:
        printk(KERN_CRIT "file: %s, function: %s, line: %d\n", __FILE__, __func__, __LINE__);
        break;
    case kern_err:
        printk(KERN_ERR "file: %s, function: %s, line: %d\n", __FILE__, __func__, __LINE__);
        break;
    case kern_warning:
        printk(KERN_WARNING "file: %s, function: %s, line: %d\n", __FILE__, __func__, __LINE__);
        break;
    case kern_notice:
        printk(KERN_NOTICE "file: %s, function: %s, line: %d\n", __FILE__, __func__, __LINE__);
        break;
    case kern_info:
        printk(KERN_INFO "file: %s, function: %s, line: %d\n", __FILE__, __func__, __LINE__);
        break;
    case kern_debug:
        printk(KERN_DEBUG "file: %s, function: %s, line: %d\n", __FILE__, __func__, __LINE__);
        break;
    default:
        printk(KERN_DEFAULT "file: %s, function: %s, line: %d\n", __FILE__, __func__, __LINE__);
        break;
    }
}

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
