/* Copyright (c) 2016 by Wang Xiaoqiang <wang_xiaoq@126.com>.
 *
 * MigratPage is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; version 3.
 *
 * MigratePage is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * You should find a copy of v3 of the GNU General Public License somewhere
 * on your Linux system; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/sched.h> /* task_struct */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <asm/pgtable.h>
#include <asm-generic/memory_model.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <asm/pgtable_64.h>
#include <linux/migrate.h>
#include <linux/hugetlb.h>

MODULE_LICENSE("GPL v3");
MODULE_AUTHOR("Wang Xiaoqiang <wang_xiaoq@126.com>");

static char* task_name = "NULL";
module_param(task_name, charp, S_IRUGO);

static unsigned long vaddr = 0;
module_param(vaddr, ulong, S_IRUGO);

static struct task_struct *name_task_struct(char *name)
{
	struct task_struct *task;

	for_each_process(task)
		if (!strcmp(task->comm, name))
			return task;

	return NULL;
}

struct page *virt2page(struct mm_struct *mm, unsigned long addr)
{
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
	struct page *page = NULL;;

	pgd = pgd_offset(mm, addr);
	if (pgd_none(*pgd) || pgd_bad(*pgd))
		return NULL;
	
	pud = pud_offset(pgd, addr);
	if (pud_none(*pud) || pud_bad(*pud))
		return NULL;

	pmd = pmd_offset(pud, addr);
	if (pmd_none(*pmd) || pmd_bad(*pmd))
		return NULL;

	if (!(pte = pte_offset_map(pmd, addr)))
		return NULL;

	if (pte_none(*pte))
		goto out;

	if (!pte_present(*pte))
		goto out;

	if (!(page = pte_page(*pte)))
		goto out;

out:
	pte_unmap(pte);

	return page;
}

static void dump_phy_page_address(struct task_struct *task)
{
	struct vm_area_struct *vma = NULL;
	unsigned long vpage;
	unsigned long pfn;

	if (task->mm && task->mm->mmap) {
		for (vma = task->mm->mmap; vma; vma = vma->vm_next)
			for (vpage = vma->vm_start; vpage < vma->vm_end; vpage += PAGE_SIZE) {
				struct page *p = virt2page(task->mm, vpage);
				if (p) {
					pfn = page_to_pfn(p);
					pr_info("%lu\n", pfn);
				} else
					pr_info("0\n");
			}
	}
}

static void collect_pages(struct task_struct *task, struct list_head *l)
{
	struct vm_area_struct *vma = NULL;
	unsigned long vpage;

	if (task->mm && task->mm->mmap) {
		for (vma = task->mm->mmap; vma; vma = vma->vm_next)
			for (vpage = vma->vm_start; vpage < vma->vm_end; vpage += PAGE_SIZE) {
				struct page *p = virt2page(task->mm, vpage);
				if (p != NULL && pfn_valid(page_to_pfn(p)) && !PageReserved(p)) {
					isolate_lru_page(p);
					list_add(&p->lru, l);
				}
			}
	}
}

static struct page *new_page(struct page *page, unsigned long private, int **reason)
{
	struct page *p = alloc_page(GFP_USER | __GFP_ZERO);
	return p;
}

static void free_pg(struct page *page, unsigned long private)
{
	__free_page(page);
}

static void migrate_memory(char *name)
{
	struct task_struct *t = name_task_struct(name);
	LIST_HEAD(from);
	int ret = 0;
	
	if (!t)
		return;
	
	pr_info("before migration\n");
	dump_phy_page_address(t);

	pr_info("begin migration\n");
	migrate_prep();
	collect_pages(t, &from);
	/* TODO: the last parameter of migrate_pages stands for the reason of migration, ignore it now */
	ret = migrate_pages(&from, new_page, free_pg, (unsigned long)0, MIGRATE_SYNC, 0);
	
	if (ret)
		pr_err("%d page%s not migrate\n", ret, ret > 1 ? "s":"");

	pr_info("after migration\n");
	dump_phy_page_address(t);
}

static int __init change_pgtab_init(void)
{
	pr_info("module insert\n");
	migrate_memory(task_name);

	return 0;
}

static void __exit change_pgtab_exit(void)
{
	pr_info("module exit\n");
	return ;
}

module_init(change_pgtab_init);
module_exit(change_pgtab_exit);
