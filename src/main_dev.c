#include <linux/device.h>
#include <linux/init.h>
#include <linux/module.h>

extern struct bus_type my_bus_type;

void my_release(struct device* dev)
{
}

struct device my_device_type
    = {
          .init_name = "this_bus_name",
          .bus = &my_bus_type,
          .release = my_release,
      };

static int __init
my_dev_init(void)
{
    printk("---------------%s---------------\n", __FUNCTION__);
    int ret = device_register(&my_device_type);
    if (IS_ERR_VALUE(ret)) {
        printk(KERN_ERR "Failed to register my_dev: %d\n", ret);
        return ret;
    }
    return 0;
}

static void __exit my_dev_exit(void)
{
    device_unregister(&my_device_type);
    printk("---------------%s---------------\n", __FUNCTION__);
}

module_init(my_dev_init);
module_exit(my_dev_exit);

MODULE_LICENSE("GPL");