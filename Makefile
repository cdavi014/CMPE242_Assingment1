obj-m += led.o

KDIR = /home/pi/Documents/cmpe_242_2018/kernel_sources/linux

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
	cp *.ko ../bin/

clean:
	rm -rf *o *.ko *.mod.* *.symvers *.order
	rm ../bin/*.ko
