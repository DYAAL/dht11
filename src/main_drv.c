#include "linux/device.h"
#include "linux/err.h"
#include "linux/export.h"
#include "linux/fs.h"
#include "linux/ioport.h"
#include "linux/kdev_t.h"
#include "linux/platform_device.h"
#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>

#define MAX_MINOR 10 // 假设最大的minor值为10（从0到9）

typedef struct {
    bool used[MAX_MINOR];
    int next_minor;
} Minor_Struct, *Minor_Struct_P;

typedef struct {
    int major;
    struct class* cls;
    char* cls_name;
    char* chr_dev_name;
    Minor_Struct minor_struct;
} Onece_Regist, *Onece_Regist_P;
Onece_Regist_P onece_regist_p;

typedef struct {
    const char* dev_name;
    struct resource* res;
    int minor;
} Device_Infor, *Device_Infor_P;

void minor_struct_init(void)
{
    int i;
    onece_regist_p->minor_struct.next_minor = 0;
    for (i = 0; i < MAX_MINOR; ++i) {
        onece_regist_p->minor_struct.used[i] = false;
    }
}

int allocate_minor(void)
{
    int i;
    for (i = onece_regist_p->minor_struct.next_minor; i < MAX_MINOR; ++i) {
        if (!onece_regist_p->minor_struct.used[i]) {
            onece_regist_p->minor_struct.used[i] = true;
            return i;
        }
    }
    // 如果循环回到开始还没有找到未使用的minor，则继续搜索
    for (i = 0; i < onece_regist_p->minor_struct.next_minor; ++i) {
        if (!onece_regist_p->minor_struct.used[i]) {
            onece_regist_p->minor_struct.used[i] = true;
            onece_regist_p->minor_struct.next_minor = i + 1; // 更新下一个开始搜索的位置
            return i;
        }
    }
    return -1; // 如果没有可用的minor，则返回-1
}

void release_minor(int minor)
{
    if (minor >= 0 && minor < MAX_MINOR && onece_regist_p->minor_struct.used[minor]) {
        onece_regist_p->minor_struct.used[minor] = false;
    }
}

int my_drv_probe(struct platform_device* pdev)
{
    printk(KERN_INFO "file: %s, function: %s, line: %d\n", __FILE__, __func__, __LINE__);

    Device_Infor* device_infor = kmalloc(sizeof(Device_Infor), GFP_KERNEL);
    if (!device_infor) {
        printk(KERN_ERR "Failed to allocate memory for Device_Infor\n");
        return -ENOMEM;
    }

    device_infor->minor = allocate_minor();

    // 获取设备名称，如果没有则使用默认名称
    if (pdev->dev.kobj.name) {
        device_infor->dev_name = pdev->dev.kobj.name;
    } else {
        device_infor->dev_name = "unknown_device";
    }

    // 创建设备节点
    struct device* dev = device_create(onece_regist_p->cls, NULL, MKDEV(onece_regist_p->major, device_infor->minor), NULL, "%s%d", device_infor->dev_name, device_infor->minor);
    if (IS_ERR(dev)) {
        printk(KERN_ERR "Failed to create device node\n");
        release_minor(device_infor->minor);
        kfree(device_infor);
        return PTR_ERR(dev);
    }
    // 将设备信息保存到平台设备的私有数据中
    platform_set_drvdata(pdev, device_infor);

    return 0;
}

int my_drv_remove(struct platform_device* pdev)
{
    printk(KERN_INFO "file: %s, function: %s, line: %d\n", __FILE__, __func__, __LINE__);

    // 获取之前存储的设备信息
    Device_Infor* device_infor = platform_get_drvdata(pdev);
    if (device_infor) {
        // 销毁设备节点
        release_minor(device_infor->minor);
        device_destroy(onece_regist_p->cls, MKDEV(onece_regist_p->major, device_infor->minor));
        // 释放设备信息内存
        kfree(device_infor);
    }
    return 0;
}
const struct platform_device_id my_drv_id_table[] = {
    { "my_dev1", 0 },
    { "my_dev2", 0 },
    {},
};

const struct of_device_id my_drv_of_match[] = {};
static struct platform_driver my_drv
    = {
          .probe = my_drv_probe,
          .remove = my_drv_remove,
          .id_table = my_drv_id_table,
          .driver = {
              .of_match_table = my_drv_of_match,
              .name = "my_drv",
          }
      };

static struct file_operations my_drv_fops = {
    .open = NULL,
};

static int __init
my_drv_init(void)
{
    printk(KERN_INFO "file: %s, function: %s, line: %d\n", __FILE__, __func__, __LINE__);
    onece_regist_p = kmalloc(sizeof(Onece_Regist), GFP_KERNEL);
    if (!onece_regist_p) {
        printk(KERN_ERR "Failed to allocate memory for Onece_Regist\n");
        return -ENOMEM;
    }
    onece_regist_p->major = 0;
    onece_regist_p->cls_name = "my_class_name";
    onece_regist_p->chr_dev_name = "myZ_chr_dev_name";
    minor_struct_init();
    onece_regist_p->major
        = register_chrdev(onece_regist_p->major, onece_regist_p->chr_dev_name, &my_drv_fops);
    onece_regist_p->cls = class_create(THIS_MODULE, onece_regist_p->cls_name);
    if (IS_ERR(onece_regist_p->cls)) {
        printk(KERN_ERR "file: %s, function: %s, line: %d\n", __FILE__, __func__, __LINE__);
        unregister_chrdev(onece_regist_p->major, onece_regist_p->chr_dev_name);
        return PTR_ERR(onece_regist_p->cls);
    }
    int ret = platform_driver_register(&my_drv);
    if (IS_ERR_VALUE(ret)) {
        printk(KERN_ERR "file: %s, function: %s, line: %d\n", __FILE__, __func__, __LINE__);
        class_destroy(onece_regist_p->cls);
        unregister_chrdev(onece_regist_p->major, onece_regist_p->chr_dev_name);
        return ret;
    }
    return 0;
}

static void __exit my_drv_exit(void)
{
    printk(KERN_INFO "file: %s, function: %s, line: %d\n", __FILE__, __func__, __LINE__);
    platform_driver_unregister(&my_drv);
    class_destroy(onece_regist_p->cls);
    unregister_chrdev(onece_regist_p->major, onece_regist_p->chr_dev_name);
    kfree(onece_regist_p);
}

module_init(my_drv_init);
module_exit(my_drv_exit);
MODULE_LICENSE("GPL");