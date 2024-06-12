
#include "linux/printk.h"
#include <linux/device.h>
#include <linux/init.h>
#include <linux/module.h>

extern struct bus_type my_bus_type;

int my_probe(struct device* dev)
{
    printk(KERN_INFO "my_probe: %s\n", dev->kobj.name);
    return 0;
}
int my_remove(struct device* dev)
{
    return 0;
}
struct device_driver my_driver_type
    = {
          .name = "this_bus_name",
          .bus = &my_bus_type,
          .probe = my_probe,
          .remove = my_remove,
      };

static int __init
my_dev_init(void)
{
    printk("---------------%s---------------\n", __FUNCTION__);
    int ret = driver_register(&my_driver_type);
    if (IS_ERR_VALUE(ret)) {
        printk(KERN_ERR "Failed to register my_drv: %d\n", ret);
        return ret;
    }
    return 0;
}

static void __exit my_dev_exit(void)
{
    driver_unregister(&my_driver_type);
    printk("---------------%s---------------\n", __FUNCTION__);
}

module_init(my_dev_init);
module_exit(my_dev_exit);

MODULE_LICENSE("GPL");