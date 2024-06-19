#include "dev_drv.h"
#include "driver_drv.h"
#include "file_ops_drv.h"
#include "linux/err.h"
#include "linux/kern_levels.h"
#include "marcos_drv.h"
#include <linux/device.h>
#include <linux/gpio/consumer.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/wait.h>
Driver_Register_Struct_P driver_register_struct_p;
Device_Register_Struct_P device_register_struct_ps[MAX_MINOR];

static irqreturn_t sr04_irq_handler(int irq, void* data)
{

    return IRQ_HANDLED;
}

int sr04_drv_probe(struct platform_device* pdev)
{
    int minor;
    print_kernel_infor(kern_info);
    if (pdev->dev.of_node) {
        printk(KERN_INFO "sr04_drv_probe: device tree node is valid\n");
        driver_register_struct_p->minor_struct.current_minor = allocate_minor(driver_register_struct_p);
    }
    minor = driver_register_struct_p->minor_struct.current_minor;
    device_register_struct_ps[minor] = kmalloc(sizeof(Device_Register_Struct), GFP_KERNEL);
    if (device_register_struct_ps[minor]) {
        print_kernel_infor(kern_err);
        printk(KERN_ERR "Failed to allocate memory for Onece_Regist\n");
        return -ENOMEM;
    }
    // /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */ 申请内存
    device_struct_init(device_register_struct_ps[minor], minor);
    // /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */ 初始化
    int ret = of_property_read_string(pdev->dev.of_node, "device_name", &(device_register_struct_ps[minor]->device_infor.dev_name));
    if (IS_ERR_VALUE(ret)) {
        printk(KERN_ERR "the ret value is (ret = %d)\n", ret);
        printk(KERN_ERR "Failed to read device_name from device tree\n");
        device_register_struct_ps[minor]->device_infor.dev_name = "unknown_device";
    }
    // /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */ get device name
    device_register_struct_ps[minor]->device_gpio_resource.gpio_num = 2;
    device_register_struct_ps[minor]->device_gpio_resource.gpio_desc_pp = kmalloc(device_register_struct_ps[minor]->device_gpio_resource.gpio_num * sizeof(struct gpio_desc*), GFP_KERNEL);
    if (device_register_struct_ps[minor]->device_gpio_resource.gpio_desc_pp == NULL) {
        print_kernel_infor(kern_err);
        printk(KERN_ERR "Failed to allocate memory for gpio_desc_pp\n");
        goto free_device_register_struct;
    }
    device_register_struct_ps[minor]->device_gpio_resource.gpio_desc_pp[0] = devm_gpiod_get_index(&pdev->dev, "trig", 0, GPIOD_OUT_LOW);
    if (IS_ERR(device_register_struct_ps[minor]->device_gpio_resource.gpio_desc_pp[0])) {
        print_kernel_infor(kern_err);
        printk(KERN_ERR "Failed to get gpio_desc_pp[0]\n");
        goto free_device_gpio_resource;
    }
    device_register_struct_ps[minor]->device_gpio_resource.gpio_desc_pp[1] = devm_gpiod_get_index(&pdev->dev, "echo", 0, GPIOD_IN);
    if (IS_ERR(device_register_struct_ps[minor]->device_gpio_resource.gpio_desc_pp[1])) {
        print_kernel_infor(kern_err);
        printk(KERN_ERR "Failed to get gpio_desc_pp[1]\n");
        goto devm_gpiod_put0;
    }
    // ─── 获得gpio资源 ────────────────────────────────────────────────────────────────
    device_register_struct_ps[minor]->device_gpio_resource.irq_num = 1;
    device_register_struct_ps[minor]->device_gpio_resource.irq_p = kmalloc(device_register_struct_ps[minor]->device_gpio_resource.irq_num * sizeof(int), GFP_KERNEL);
    device_register_struct_ps[minor]->device_gpio_resource.irq_p[0] = gpiod_to_irq(device_register_struct_ps[minor]->device_gpio_resource.gpio_desc_pp[1]);
    if (IS_ERR_VALUE(device_register_struct_ps[minor]->device_gpio_resource.irq_p[0])) {
        printk(KERN_ERR "Failed to get irq_p[0]\n");
        print_kernel_infor(kern_err);
        goto devm_gpiod_put1;
    }
    ret = request_irq(device_register_struct_ps[minor]->device_gpio_resource.irq_p[0], sr04_irq_handler, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "sr04_irq_handler", NULL);
    if (IS_ERR_VALUE(ret)) {
        print_kernel_infor(kern_err);
        printk(KERN_ERR "request_irq_failed\n");
        goto devm_gpiod_put1;
    }
    // ─── 获得中断资源 ──────────────────────────────────────────────────────────────────
    device_register_struct_ps[minor]->device_infor.dev = device_create(driver_register_struct_p->cls, NULL, MKDEV(driver_register_struct_p->major, minor), NULL, device_register_struct_ps[minor]->device_infor.dev_name);
    if (IS_ERR(device_register_struct_ps[minor]->device_infor.dev)) {
        print_kernel_infor(kern_err);
        printk(KERN_ERR "Failed to create device\n");
        goto irq_return;
    }
    // ─── 创建设备节点 ──────────────────────────────────────────────────────────
    platform_set_drvdata(pdev, device_register_struct_ps[minor]);
    return 0;
    // ─── 设置私有属性 ──────────────────────────────────────────────────────────

irq_return:
    free_irq(device_register_struct_ps[minor]->device_gpio_resource.irq_p[0], NULL);
devm_gpiod_put1:
    devm_gpiod_put(&pdev->dev, device_register_struct_ps[minor]->device_gpio_resource.gpio_desc_pp[1]);
devm_gpiod_put0:
    devm_gpiod_put(&pdev->dev, device_register_struct_ps[minor]->device_gpio_resource.gpio_desc_pp[0]);
free_device_gpio_resource:
    kfree(device_register_struct_ps[minor]->device_gpio_resource.gpio_desc_pp);
free_device_register_struct:
    kfree(device_register_struct_ps[minor]);
    return -EAGAIN;
}

int sr04_drv_remove(struct platform_device* pdev)
{
    print_kernel_infor(kern_info);
    Device_Register_Struct_P device_register_struct_p = platform_get_drvdata(pdev);
    if (IS_ERR(device_register_struct_p)) {
        printk(KERN_ERR "device_register_struct_p is error\n");
        print_kernel_infor(kern_info);
        return -EAGAIN;
    } else {
        free_irq(device_register_struct_p->device_gpio_resource.irq_p[0], NULL);
        devm_gpiod_put(&pdev->dev, device_register_struct_p->device_gpio_resource.gpio_desc_pp[1]);
        devm_gpiod_put(&pdev->dev, device_register_struct_p->device_gpio_resource.gpio_desc_pp[0]);
        kfree(device_register_struct_p->device_gpio_resource.gpio_desc_pp);
        kfree(device_register_struct_p);
        return 0;
    }
}

const struct platform_device_id sr04_id_table[] = {
    {},
};

const struct of_device_id sr04_of_match_table[] = {
    {
        .compatible = "sr04_compatible",
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
        return -ENOMEM;
    }

    // /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */ 申请内存
    init_struct(driver_register_struct_p);
    driver_register_struct_p->major = register_chrdev(driver_register_struct_p->major, driver_register_struct_p->chr_dev_name, &sr04_drv_fops);
    if (IS_ERR_VALUE(driver_register_struct_p->major)) {
        printk(KERN_ERR "register_chrdev failed\n");
        print_kernel_infor(kern_err);
        goto register_chrdev_err;
    }
    // /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */ 创建 chrdev
    driver_register_struct_p->cls
        = class_create(THIS_MODULE, driver_register_struct_p->cls_name);
    if (IS_ERR(driver_register_struct_p->cls)) {
        print_kernel_infor(kern_err);
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
    return -EPERM;
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