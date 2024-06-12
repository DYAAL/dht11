# 设置默认内核路径，可以在调用 make 时覆盖
KERN_DIR ?= /home/book/100ask_imx6ull-sdk/Linux-4.9.88

# 获取当前路径
PWD := $(shell pwd)
APP_DIR := $(PWD)/../app
BUS_DIR := $(PWD)
DEVICE_DIR := $(PWD)
DRIVER_DIR := $(PWD)

# 模块名和测试名
app_name := app_test
driver_name := driver
driver_reserved_line := 57
bus_name := bus
bus_reserved_line := 58
device_name := device
device_reserved_line := 59

# 挂载目录路径
mnt_directory := /home/book/remote/mnt_imx6ull





# 环境变量设置
ARCH ?= arm
CROSS_COMPILE ?= arm-buildroot-linux-gnueabihf-

# 获取所有以 _drv.c 结尾的文件
driver_module_srcs := $(filter %_drv.c, $(wildcard *.c))
# 将这些源文件转成对应的 .o 目标文件
driver_module_objs := $(patsubst %.c, %.o, $(driver_module_srcs))

# 设置编译内核模块的目标
$(driver_name)-y := $(driver_module_objs)
obj-m += $(driver_name).o

bus_module_srcs := $(filter %_bus.c, $(wildcard *.c))
bus_module_objs := $(patsubst %.c, %.o, $(bus_module_srcs))
$(bus_name)-y := $(bus_module_objs)
obj-m += $(bus_name).o

device_module_srcs := $(filter %_dev.c, $(wildcard *.c))
device_module_objs := $(patsubst %.c, %.o, $(device_module_srcs))
$(device_name)-y := $(device_module_objs)
obj-m += $(device_name).o

app_c_srcs := $(filter %.c, $(wildcard $(APP_DIR)/*.c))
app_cpp_srcs := $(filter %.cpp, $(wildcard $(APP_DIR)/*.cpp))
app_srcs := $(app_c_srcs) $(app_cpp_srcs)




driver-y :=  main_drv.o
bus-y :=  main_bus.o
device-y :=  main_dev.o

# 默认目标
.PHONY: all modules app clean cp cp_app cp_drv

# 顶层构建调试信息
ifneq ($(KERNELRELEASE),)
    # 内核构建系统中执行
    # 内核模块构建规则已定义
else
# 顶层构建系统中执行
$(info Current directory: $(PWD))
$(info Driver sources: $(driver_module_srcs))
$(info Bus sources: $(bus_module_srcs))
$(info Device sources: $(device_module_srcs))
$(info Application sources: $(app_srcs))


# 构建所有目标
all: modules app 

modules:
	sed -i '$(driver_reserved_line)s|.*|$(driver_name)-y := $(driver_module_objs)|' $(PWD)/Makefile
	sed -i '$(bus_reserved_line)s|.*|$(bus_name)-y := $(bus_module_objs)|' $(PWD)/Makefile
	sed -i '$(device_reserved_line)s|.*|$(device_name)-y := $(device_module_objs)|' $(PWD)/Makefile
	$(MAKE) -C $(KERN_DIR) M=$(PWD) modules

app:
	$(CROSS_COMPILE)g++ -o $(app_name) -I$(APP_DIR) $(app_srcs)

# 清理生成的文件
clean:
	$(MAKE) -C $(KERN_DIR) M=$(PWD) clean
	rm -rf modules.order Module.symvers $(driver_name).mod.c
	rm -f $(driver_name).ko $(app_name)
	rm -f $(driver_module_objs) $(bus_module_objs) $(device_module_objs)
	rm *.json

# 拷贝模块到指定目录
cp: cp_app cp_drv clean

cp_app:
	cp $(app_name) $(mnt_directory)

cp_drv:
	cp $(driver_name).ko $(mnt_directory)
	cp $(bus_name).ko $(mnt_directory)
	cp $(device_name).ko $(mnt_directory)
endif
