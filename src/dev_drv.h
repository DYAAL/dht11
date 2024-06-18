#ifndef __DEV_DRV_H__
#define __DEV_DRV_H__
#include <linux/wait.h>

typedef struct {
    const char* dev_name;
    int minor;
    int irq_num;
    struct device* dev;
    struct gpio_desc* gpio_desc_p;
    wait_queue_head_t wait_queue;
} Device_Infor, *Device_Infor_P;

typedef struct {
    bool whether_data;
} Device_Data, *Device_Data_P;

typedef struct {
    Device_Infor device_infor;
    Device_Data device_data;
} Device_Register_Struct, *Device_Register_Struct_P;


#endif // __DEV_DRV_H__