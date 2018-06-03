KERNELDIR = /lib/modules/$(shell uname -r)/build

obj-m := ledtest_dev.o

PWD := $(shell pwd)

default: 
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

project : project.o
	gcc -o project project.o -lwiringPi -lwiringPiDev

project.o : project.c
	gcc -c project.c

clean : 
	rm  *.o project
