#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/module.h>
#include <linux/platform_device.h>

// 定义第一个设备的资源
static struct resource my_device1_resources[] = {
    {
        .flags = IORESOURCE_MEM,
    },
    {

        .flags = IORESOURCE_IRQ,
    },
};

// 定义第一个设备的 platform_device 结构体
static struct platform_device my_device1 = {
    .name = "my_device1",
    .id = -1, // 设备标识符设为 -1，表示任意实例
    .num_resources = ARRAY_SIZE(my_device1_resources),
    .resource = my_device1_resources,
};

// 定义第二个设备的资源
static struct resource my_device2_resources[] = {
    {

        .flags = IORESOURCE_MEM,
    },
    {

        .flags = IORESOURCE_IRQ,
    },
};

// 定义第二个设备的 platform_device 结构体
static struct platform_device my_device2 = {
    .name = "my_device2",
    .id = -1, // 设备标识符设为 -1，表示任意实例
    .num_resources = ARRAY_SIZE(my_device2_resources),
    .resource = my_device2_resources,
};

// 模块初始化函数
static int __init my_driver_init(void)
{
    int ret;

    // 注册第一个设备
    ret = platform_device_register(&my_device1);
    if (ret)
        goto out;

    // 注册第二个设备
    ret = platform_device_register(&my_device2);
    if (ret)
        goto unregister_device1;

    // 成功注册所有设备
    return 0;

unregister_device1:
    platform_device_unregister(&my_device1);
out:
    return ret;
}

// 模块退出函数
static void __exit my_driver_exit(void)
{
    // 反向顺序取消注册设备
    platform_device_unregister(&my_device2);
    platform_device_unregister(&my_device1);
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");

