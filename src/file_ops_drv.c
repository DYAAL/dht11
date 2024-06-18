
#include "linux/device.h"
#include "linux/err.h"
#include "linux/gpio/consumer.h"
#include "linux/wait.h"
#include <asm/uaccess.h>
#include <linux/slab.h>




ssize_t my_read(struct file* filp, char __user* usr_buf, size_t size, loff_t* offset)
{

};

ssize_t my_write(struct file* filp, const char __user* usr_buf, size_t size, loff_t* offset)
{

    return 0;
};
int my_open(struct inode* inode_p, struct file* filp)
{

    return 0;
};

int my_release(struct inode* inode_p, struct file* filp)
{
    return 0;
};
struct file_operations sr04_drv_fops = {
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write,
};