
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
    printk(KERN_INFO "file: %s, function: %s, line: %d\n", __FILE__, __func__, __LINE__);
    int minor = MINOR(filp->f_inode->i_rdev);
    Device_Register_Struct_P device_register_struct_p = device_register_struct_ps[minor];

    // ─── 发送10us高电平 ───────────────────────────────────────────────────────────────
    gpiod_set_value(device_register_struct_p->device_gpio_resource.gpio_desc_pp[0], 1);
    udelay(15);
    gpiod_set_value(device_register_struct_p->device_gpio_resource.gpio_desc_pp[0], 0);
    // ─── 查看gpio状态 ────────────────────────────────────────────────────────
    // wait_event_interruptible(device_register_struct_p->device_infor.wait_queue, device_register_struct_p->device_data.whether_data);
    // int ret = copy_to_user(usr_buf, &device_register_struct_p->device_data.data_us, sizeof(int));
    // reset_device_data(device_register_struct_p);
    // if (ret < 0) {
    //     return -EAGAIN;
    // }
    return sizeof(int);
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
    .open = NULL,
    .release = NULL,
    .read = sr04_read,
    .write = sr04_write,
};