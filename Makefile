ifneq ($(KERNELRELEASE),)

obj-m += sysfs_attrs.o

else

KDIR = /home/max/Embedded/Repos/Linux_Kernel_stable

modules:
	$(MAKE) -C $(KDIR) M=$(PWD) C=1 modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) C=1 clean

endif
