ifneq ($(KERNELRELEASE),)
obj-m += mfd_demo.o
mfd_demo-y := src/soc/soc_uart_parent.o \
	      src/mcu/mcu_core.o \
	      src/sensor/sensor_a.o \
	      src/sensor/sensor_b.o
else
KDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

.PHONY: all clean

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
endif
