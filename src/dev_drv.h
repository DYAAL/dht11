#ifndef __DEV_DRV_H__
#define __DEV_DRV_H__
#include <linux/wait.h>

typedef struct {
    const char* dev_name;
    int minor;
    struct device* dev;
    wait_queue_head_t wait_queue;
} Device_Infor, *Device_Infor_P;

typedef struct {
    int irq_num;
    int* irq_p;
    int gpio_num;
    struct gpio_desc** gpio_desc_pp;
} Device_Gpio_Resource, *Device_Gpio_Resource_P;

typedef struct {
    bool whether_data;
} Device_Data, *Device_Data_P;

typedef struct {
    Device_Infor device_infor;
    Device_Data device_data;
    Device_Gpio_Resource device_gpio_resource;
} Device_Register_Struct, *Device_Register_Struct_P;

void device_struct_init(Device_Register_Struct_P this, int minor);
#endif // __DEV_DRV_H__