KERNELDIR = /lib/modules/$(shell uname -r)/build

obj-m := btn.o

PWD := $(shell pwd)

default :
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
