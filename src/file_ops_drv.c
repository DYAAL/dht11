
#include "dev_drv.h"
#include "driver_drv.h"
#include "linux/device.h"
#include "linux/err.h"
#include "linux/gpio/consumer.h"
#include "linux/irqflags.h"
#include "linux/wait.h"
#include "marcos_drv.h"
#include <asm/delay.h>
#include <asm/uaccess.h>

#include <linux/slab.h>

extern Driver_Register_Struct_P driver_register_struct_p;
extern Device_Register_Struct_P device_register_struct_ps[MAX_MINOR];

ssize_t sr04_read(struct file* filp, char __user* usr_buf, size_t size, loff_t* offset)
{
    int count_us = 0;
    int timeout = 1000000;
    int minor = MINOR(filp->f_inode->i_rdev);
    Device_Register_Struct_P device_register_struct_p = device_register_struct_ps[minor];
    unsigned long irq_flags = 0;
    // ─── 关闭本地中断 ──────────────────────────────────────────────────────────
    local_irq_save(irq_flags);
    // ─── 发送10us高电平 ───────────────────────────────────────────────────────────────
    gpiod_set_value(device_register_struct_p->device_gpio_resource.gpio_desc_pp[0], 1);
    udelay(15);
    gpiod_set_value(device_register_struct_p->device_gpio_resource.gpio_desc_pp[0], 0);
    // ─── 查看gpio状态 ────────────────────────────────────────────────────────
    while (gpiod_get_value(device_register_struct_p->device_gpio_resource.gpio_desc_pp[1]) == 0 && timeout--) {
        udelay(1);
    }
    if (timeout == 0) {
        printk("timeout\n");
        print_kernel_infor(kern_err);
        local_irq_restore(irq_flags);
        return -EAGAIN;
    }
    timeout = 1000000;
    while (gpiod_get_value(device_register_struct_p->device_gpio_resource.gpio_desc_pp[1]) == 1 && timeout--) {
        udelay(1);
        count_us++;
    }
    if (timeout == 0) {
        printk("timeout\n");
        print_kernel_infor(kern_err);
        local_irq_restore(irq_flags);
        return -EAGAIN;
    }
    // ─── 开中断 ─────────────────────────────────────────────────────────────────────
    else {
        local_irq_restore(irq_flags);
        int ret = copy_to_user(usr_buf, &count_us, sizeof(count_us));
        if (IS_ERR_VALUE(ret)) {
            printk("copy_to_user error\n");
            print_kernel_infor(kern_err);
            return -EAGAIN;
        }
        return 0;
    }
};

ssize_t sr04_write(struct file* filp, const char __user* usr_buf, size_t size, loff_t* offset)
{

    return 0;
};
int sr04_open(struct inode* inode_p, struct file* filp)
{

    return 0;
};

int sr04_release(struct inode* inode_p, struct file* filp)
{
    return 0;
};
struct file_operations sr04_drv_fops = {
    .open = sr04_open,
    .release = sr04_release,
    .read = sr04_read,
    .write = sr04_write,
};