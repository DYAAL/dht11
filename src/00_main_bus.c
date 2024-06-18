#include "linux/string.h"
#include <linux/device.h>
#include <linux/export.h>
#include <linux/init.h>
#include <linux/module.h>

int my_match(struct device* dev, struct device_driver* drv) //匹配成功是1，匹配失败是0
{
    if (!strcmp(drv->name, dev->kobj.name)) { // dev->kobj.name 是这个奇怪的名字
        printk(KERN_INFO "%s matched with %s\n", drv->name, dev->kobj.name);
        return 1;
    } else {
        printk(KERN_INFO "%s not matched with %s\n", drv->name, dev->kobj.name);
        return 0;
    }
    return 0;
}

struct bus_type my_bus_type = {
    .name = "my_bus",
    .match = my_match,
};

static int __init my_bus_init(void)
{
    printk("---------------%s---------------\n", __FUNCTION__);
    int ret = bus_register(&my_bus_type);
    if (IS_ERR_VALUE(ret)) {
        printk(KERN_ERR "Failed to register my_bus: %d\n", ret);
        return ret;
    }
    return 0;
}

static void __exit my_bus_exit(void)
{
    printk("---------------%s---------------\n", __FUNCTION__);
    bus_unregister(&my_bus_type);
}

EXPORT_SYMBOL(my_bus_type);
module_init(my_bus_init);
module_exit(my_bus_exit);

MODULE_LICENSE("GPL");