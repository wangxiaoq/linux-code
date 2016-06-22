all: module

ifndef KERNEL_DIR
KERNEL_DIR = /lib/modules/`uname -r`/build
endif

obj-m := migrate-page.o

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
	sudo insmod migrate-page.ko

.PHONY: uninstall
uninstall:
	sudo rmmod migrate-page.ko

.PHONY: reload
reload:
	sudo rmmod migrate-page.ko
	sudo insmod migrate-page.ko
