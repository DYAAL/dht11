#include "linux/err.h"
#include "linux/platform_device.h"
#include <linux/init.h>
#include <linux/module.h>

static struct resource my_devs_source1[] = {
    {
        .flags = IORESOURCE_IRQ,
        .start = 1,
        .end = 1,
    },
    {
        .flags = IORESOURCE_MEM,
    }
};
void my_dev_release(struct device* dev) {

};
static struct platform_device my_dev1
    = {
          .name = "my_dev1",
          .dev = {
              .init_name = "my_dev1",
              .release = my_dev_release,
          },
          .id = -1,
          .num_resources = 0,
          .resource = my_devs_source1,
      };

static struct platform_device my_dev2
    = {
          .name = "my_dev2",
          .dev = {
              .init_name = "my_dev2",
              .release = my_dev_release,
          },
          .id = -1,
          .num_resources = 0,
          .resource = my_devs_source1,
      };

static int __init my_dev_init(void)
{
    int err;

    printk(KERN_INFO "file: %s, function: %s, line: %d\n", __FILE__, __func__, __LINE__);
    err = platform_device_register(&my_dev1);
    if (IS_ERR_VALUE(err)) {
        printk(KERN_INFO "file: %s, function: %s, line: %d\n", __FILE__, __func__, __LINE__);
        return err;
    }
    
    err = platform_device_register(&my_dev2);
    if (IS_ERR_VALUE(err)) {
        printk(KERN_INFO "file: %s, function: %s, line: %d\n", __FILE__, __func__, __LINE__);
        return err;
    }
    return 0;
}

static void __exit my_dev_exit(void)
{
    printk(KERN_INFO "file: %s, function: %s, line: %d\n", __FILE__, __func__, __LINE__);
    platform_device_unregister(&my_dev1);
    platform_device_unregister(&my_dev2);
}

module_init(my_dev_init);
module_exit(my_dev_exit);
MODULE_LICENSE("GPL");