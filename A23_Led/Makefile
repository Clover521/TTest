KERNEL_DIR = /home/clover/Clover/lichee/linux-3.4
CROSS_GCC = /home/clover/Clover/lichee/out/android/common/buildroot/external-toolchain/bin/arm-linux-gnueabi-

CUR_DIR = ${shell pwd}


modules:

		#表示进入内核源码目录，并且告诉编译器将当前目录下的源码编译成驱动模块
		make -C $(KERNEL_DIR) M=$(CUR_DIR) ARCH=arm CROSS_COMPILE=$(CROSS_GCC) modules

clean:

		make -C $(KERNEL_DIR) M=$(CUR_DIR) clean

install:
		cp -a *.ko ~/1120


obj-m = led_Driver.o




























