#include "dev_drv.h"
#include "asm/string.h"
#include "linux/stddef.h"
#include <linux/wait.h>
void device_struct_init(Device_Register_Struct_P this, int minor)
{
    memset(this, 0, sizeof(*this));
    this->device_data.whether_data = false;
    init_waitqueue_head(&this->device_infor.wait_queue);
    this->device_infor.minor = minor;
    this->device_data.start_time = 0;
    this->device_data.end_time = 0;
    this->device_data.data_us = 0;
}

void reset_device_data(Device_Register_Struct_P this)
{
    this->device_data.start_time = 0;
    this->device_data.end_time = 0;
    this->device_data.data_us = 0;
    this->device_data.whether_data = false;
}