#include "dev_drv.h"
#include "driver_drv.h"
#include "file_ops_drv.h"
#include "marcos_drv.h"
#include <linux/device.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/wait.h>

Driver_Register_Struct_P driver_register_struct_p;


int sr04_drv_probe(struct platform_device* pdev)
{
    print_kernel_infor(kern_info);
    if (pdev->dev.of_node) {
        printk(KERN_INFO "sr04_drv_probe: device tree node is valid\n");
        driver_register_struct_p->minor_struct.current_minor = allocate_minor(driver_register_struct_p);
    }
}

int sr04_drv_remove(struct platform_device* pdev)
{
}

const struct platform_device_id sr04_id_table[] = {
    {},
};

const struct of_device_id sr04_of_match_table[] = {
    {
        .compatible = "my_device,sr04_compatible",
    },
    {},
};

static struct platform_driver sr04_platform_drv = {
    .probe = sr04_drv_probe,
    .remove = sr04_drv_remove,
    .id_table = sr04_id_table,
    .driver = {
        .of_match_table = sr04_of_match_table,
        .name = "sr04_drviver",
    }
};

static int __init
sr04_drv_init(void)
{
    print_kernel_infor(kern_info);
    driver_register_struct_p = kmalloc(sizeof(Driver_Register_Struct), GFP_KERNEL);
    if (IS_ERR(driver_register_struct_p)) {
        printk(KERN_ERR "kmalloc driver_register_struct_p failed\n");
        // print_kernel_infor(kern_err);
        return PTR_ERR(driver_register_struct_p);
    }

    // /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */ 申请内存
    init_struct(driver_register_struct_p);
    driver_register_struct_p->major = register_chrdev(driver_register_struct_p->major, driver_register_struct_p->chr_dev_name, &sr04_drv_fops);
    if (IS_ERR_VALUE(driver_register_struct_p->major)) {
        printk(KERN_ERR "register_chrdev failed\n");
        // print_kernel_infor(kern_err);
        goto register_chrdev_err;
    }
    // /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */ 创建 chrdev
    driver_register_struct_p->cls
        = class_create(THIS_MODULE, driver_register_struct_p->cls_name);
    if (IS_ERR(driver_register_struct_p->cls)) {
        // print_kernel_infor(kern_err);
        goto class_create_err;
    }
    // /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */ 创建 class
    int ret
        = platform_driver_register(&sr04_platform_drv);
    if (IS_ERR_VALUE(ret)) {
        print_kernel_infor(kern_err);
        goto platform_driver_register_err;
    }

    // / /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */ 创建 platform_driver
    return 0;
platform_driver_register_err:
    class_destroy(driver_register_struct_p->cls);
class_create_err:
    unregister_chrdev(driver_register_struct_p->major, driver_register_struct_p->chr_dev_name);
register_chrdev_err:
    kfree(driver_register_struct_p);
    return -1;
}

static void __exit sr04_drv_exit(void)
{
    print_kernel_infor(kern_info);
    class_destroy(driver_register_struct_p->cls);
    unregister_chrdev(driver_register_struct_p->major, driver_register_struct_p->chr_dev_name);
    kfree(driver_register_struct_p);
}

module_init(sr04_drv_init);
module_exit(sr04_drv_exit);

MODULE_LICENSE("GPL");