#include "linux/err.h"
#include "linux/export.h"
#include "linux/fs.h"
#include "linux/kdev_t.h"
#include "linux/platform_device.h"
#include "linux/printk.h"

#include <linux/device.h>
#include <linux/init.h>
#include <linux/module.h>

typedef struct {
    unsigned int major;
    unsigned int minor;
    const struct file_operations* fops;
    struct device* device;
    struct class* class;
    const char* class_name;
    const char* device_name;
    const char* chr_dev_name;
} My_Device_Struct, *My_Device_Struct_P;

ssize_t my_read(struct file* filp, char __user* u_buffer, size_t size, loff_t* offset)
{
    printk(KERN_INFO "-------------------------%s----------------------------\n", __FUNCTION__);

    return 0;
}
ssize_t my_write(struct file* filp, const char __user* u_buffer, size_t size, loff_t* offset)
{
    printk(KERN_INFO "-------------------------%s----------------------------\n", __FUNCTION__);

    return 0;
}
int my_open(struct inode* nodp, struct file* filp)
{
    printk(KERN_INFO "-------------------------%s----------------------------\n", __FUNCTION__);

    return 0;
}
int my_release(struct inode* nodp, struct file* filp)
{
    printk("-------------------------%s----------------------------\n", __FUNCTION__);
    return 0;
}

const struct file_operations my_fops
    = {
          .open = my_open,
          .read = my_read,
          .write = my_write,
          .release = my_release,
      };

static const struct of_device_id my_of_match[]
    = {
          [0] = {
              .compatible = "myvendor,mydevice",
          },
          {},
      };

static const struct platform_device_id my_platform_ids[] = {
    { "platform_device_v1", 0 },
    { "platform_device_v1", 1 },
    { "platform_device_v1", 2 },
    { "platform_device_v2", 0 },
    { "platform_device_v2", 1 },
    {},
};

int my_platform_probe(struct platform_device* pdev)
{
    My_Device_Struct_P my_device_struct_p;
    printk(KERN_INFO "Probing my_platform_device,name=%s, id = %d\n", pdev->name, pdev->id);
    my_device_struct_p = devm_kzalloc(&pdev->dev, sizeof(My_Device_Struct), GFP_KERNEL);
    if (!my_device_struct_p) {
        printk(KERN_ERR "Failed to allocate memory for my_device_struct\n");
        return -ENOMEM;
    }
    my_device_struct_p->minor = pdev->id;
    printk(KERN_INFO "minor = %d\n", my_device_struct_p->minor);
    my_device_struct_p->major = 0;
    my_device_struct_p->fops = &my_fops;
    my_device_struct_p->class_name = "my_class_name";
    if (pdev && pdev->dev.kobj.name) {
        printk(KERN_INFO "pdev->dev.kobj.name = %s\n", pdev->dev.kobj.name);
        my_device_struct_p->device_name = pdev->dev.kobj.name;
    } else {
        // Handle error or take appropriate action
        printk(KERN_INFO "pdev->dev.kobj.name is NULL\n");
        my_device_struct_p->device_name = "my_device_name";
    }

    my_device_struct_p->chr_dev_name = pdev->dev.kobj.name;

    my_device_struct_p->major
        = register_chrdev(my_device_struct_p->major, my_device_struct_p->chr_dev_name, my_device_struct_p->fops);
    printk(KERN_INFO "major = %d\n", my_device_struct_p->major);

    my_device_struct_p->class = class_create(THIS_MODULE, my_device_struct_p->class_name);
    my_device_struct_p->device = device_create(my_device_struct_p->class, NULL, MKDEV(my_device_struct_p->major, my_device_struct_p->minor), NULL, "%s%d", my_device_struct_p->device_name, my_device_struct_p->minor);
    platform_set_drvdata(pdev, my_device_struct_p);
    return 0;
}
int my_platform_remove(struct platform_device* pdev)
{
    My_Device_Struct_P my_device_struct_p = platform_get_drvdata(pdev);
    if (my_device_struct_p->device) {
        device_destroy(my_device_struct_p->class, MKDEV(my_device_struct_p->major, my_device_struct_p->minor));
    }

    if (my_device_struct_p->class) {
        class_destroy(my_device_struct_p->class);
    }

    if (my_device_struct_p->major) {
        unregister_chrdev(my_device_struct_p->major, my_device_struct_p->chr_dev_name);
    }

    printk(KERN_INFO "Removing my_platform_device, name=%s, id=%d\n", pdev->name, pdev->id);

    return 0;
}

struct platform_driver pdrv = {
    .probe = my_platform_probe,
    .remove = my_platform_remove,
    .id_table = my_platform_ids,
    .driver = {
        .name = "my_platform_driver", // 也可以做匹配，优先级最低
        .of_match_table = my_of_match,
    },

};

static int __init my_driver_init(void)
{
    int ret = platform_driver_register(&pdrv);
    if (IS_ERR_VALUE(ret)) {
        printk(KERN_ERR "Failed to register platform driver\n");
        return ret;
    }
    return 0;
}

static void __exit my_driver_exit(void)
{
    platform_driver_unregister(&pdrv);
}

module_init(my_driver_init);
module_exit(my_driver_exit);
MODULE_LICENSE("GPL");