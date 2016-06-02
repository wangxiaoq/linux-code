/* Copyright (c) 2016 by Wang Xiaoqiang<wang_xiaoq@126.com>.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; version
   2.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should find a copy of v2 of the GNU General Public License somewhere
   on your Linux system; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/pgtable.h>
#include <linux/mm.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/string.h>

#define MODULE_NAME "vma-test"

static struct page *page;

static int vma_test_mmap(struct file *filp, struct vm_area_struct *vma)
{
	int ret;

	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
	ret = remap_pfn_range(vma, vma->vm_start, page_to_pfn(page), PAGE_SIZE, vma->vm_page_prot);
	if (ret)
		pr_warn("mmap error\n");

	return ret;
}

char buf[64] = "hello\n";

static ssize_t vma_test_read(struct file *filp, char __user *ubuf, size_t size, loff_t *pos)
{
	unsigned long vaddr = (unsigned long)pfn_to_kaddr(page_to_pfn(page));

	if (*pos >= PAGE_SIZE)
		return 0;

	if (size > PAGE_SIZE)
		size = PAGE_SIZE;

	if (copy_to_user(ubuf, (char*)vaddr, size))
		return -EFAULT;

	*pos += size;

	return size;
}

static struct file_operations vma_test_fops = {
	.read = vma_test_read,
	.mmap = vma_test_mmap,
};

struct miscdevice vma_test_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = MODULE_NAME,
	.fops = &vma_test_fops,
};

static int vma_test_init(void)
{
	page = alloc_page(GFP_USER);
	if (!page) {
		pr_err("alloc page error\n");
		return -ENOMEM;
	}

	misc_register(&vma_test_dev);

	return 0;
}

static void vma_test_exit(void)
{
	__free_page(page);

	misc_deregister(&vma_test_dev);	

	return ;
}

module_init(vma_test_init);
module_exit(vma_test_exit);

MODULE_LICENSE("GPL v3");
MODULE_AUTHOR("wang_xiaoq@126.com");
