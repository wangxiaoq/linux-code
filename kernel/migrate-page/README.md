# MigratePage

MigratePage is a Linux tool which aims to migrate pages of applications from one physical location to annother location
in order to do something with the memory at the old location.

It makes use of the migration functionality implemented in mm/migrate.c.

## Introduction
MigratePage use a function called "dump_phy_page_address" to dump
the physical page frame addresses the process used, so we can see the
different between before migration and after migration.

## Usage

* Get the Linux source code of version 4.4 from kernel.org.
* Patch the 4.4 source code with the patch - migrate-page.patch.4.4.
* Compile the patched kernel and install it into the system.
* Reboot into the 4.4 kernel and enter the migrate-page directory.
* Type `make' to compile the module.
* Use `sudo insmod migrate-page.ko task_name="a.out" ' to migrate the
pages of process "a.out". Here we use a command parameter "task_name" to specify which process to migrate.
* Use command `make uninstall' to unload the module.