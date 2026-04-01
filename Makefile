ifneq ($(KERNELRELEASE),)
obj-m += mfd_demo.o
mfd_demo-y := src/mfd_demo_parent.o src/mfd_demo_child_one.o src/mfd_demo_child_two.o
else
KDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

.PHONY: all clean

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
endif
