target=hrtimer-test.o

all: module

ifndef KERNEL_DIR
KERNEL_DIR = /lib/modules/`uname -r`/build
endif

obj-m := $(target)

.PHONY: module
module:
	make -C $(KERNEL_DIR) M=$(PWD) modules

.PHONY: sparse
sparse:
	make C=2 -C $(KERNEL_DIR) M=$(PWD) modules

.PHONY: cocci
cocci:
	make coccicheck MODE=report -C $(KERNEL_DIR) M=$(PWD) 

.PHONY: clean
clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
	rm *~ > /dev/null 2>&1

.PHONY: install
install:
	sudo insmod timer.ko

.PHONY: uninstall
uninstall:
	sudo rmmod timer.ko

.PHONY: reload
reload:
	sudo rmmod timer.ko
	sudo insmod timer.ko
