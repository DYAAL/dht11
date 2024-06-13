#include "linux/err.h"
#include <linux/device.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

struct resource my_resources1[] = {
    [0] = {
        .flags = IORESOURCE_MEM,
        .name = "my_mem_resource1",
    },
    [1] = {
        .name = "my_irq_resource1",
        .flags = IORESOURCE_IRQ,
    },
};

struct resource my_resources2[] = {
    [0] = {
        .flags = IORESOURCE_MEM,
        .name = "my_mem_resource1",
    },
    [1] = {
        .name = "my_irq_resource1",
        .flags = IORESOURCE_IRQ,
    },
};

void my_release_func(struct device* dev)
{
    printk(KERN_INFO "-------------------------%s----------------------------\n", __FUNCTION__);
}

static struct platform_device pdev1 = {
    .name = "platform_device_v1",
    .id = 0,
    .dev = {
        .release = my_release_func,
        .init_name = "platform_device_v1",
        // .kobj = {
        //     .name = "platform_v1",        kobj 千万不能赋值 否则会段错误，只能让系统赋值
        // },
    },
    .num_resources = ARRAY_SIZE(my_resources1),
    .resource = my_resources1,
};

// static struct platform_device pdev2 = {
//     .name = "platform_device_v2",
//     .id = 1,
//     .dev = {
//         .release = my_release_func,
//         .init_name = "platform_device_v2",
//         .kobj = {
//             .name = "platform_v1",
//         },
//     },
    
//     .num_resources = ARRAY_SIZE(my_resources2),
//     .resource = my_resources2,
// };

// static struct platform_device pdev2 = {
//     .name = "platform_device_v2",
//     .id = -1,
//     .num_resources = ARRAY_SIZE(my_resources),
//     .resource = my_resources,
// };

static int __init my_dev_init(void)
{
    int ret = platform_device_register(&pdev1);
    if (IS_ERR_VALUE(ret)) {
        printk(KERN_INFO "-------------------------%s----------------------------\n", __FUNCTION__);

        return ret;
    }
    // ret = platform_device_register(&pdev2);
    // if (IS_ERR_VALUE(ret)) {
    //     printk(KERN_ERR "platform_device_register failed with %d\n", ret);
    //     return ret;
    // }
    return 0;
}

static void __exit my_dev_exit(void)
{
    // platform_device_unregister(&pdev2);
    platform_device_unregister(&pdev1);
}

module_init(my_dev_init);
module_exit(my_dev_exit);
MODULE_LICENSE("GPL");
