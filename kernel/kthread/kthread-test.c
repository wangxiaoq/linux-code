/* Copyright (c) 2016 by Wang Xiaoqiang<wang_xiaoq@126.com>.

   kthread-test is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; version
   2.

   kthread-test is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should find a copy of v2 of the GNU General Public License somewhere
   on your Linux system; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>

struct task_struct *kthread;

/* Just print some thing, not use tha parameter data */
int kthreadfn(void *data)
{
	int count = 0;

	while (!kthread_should_stop()) {
		pr_info("kthread-test: %d\n", count++);
		ssleep(5);
	}

	return 0;
}

int init_kthread(void)
{
	
	kthread = kthread_create(kthreadfn, NULL, "kthread-test %d", smp_processor_id());
	if (IS_ERR_OR_NULL(kthread)) {
		pr_err("kthread-test: create kernel thread error\n");
		return -ENOMEM;
	}

	/* bind the thread to current cpu */
	kthread_bind(kthread, smp_processor_id());

	/* wake up */
	wake_up_process(kthread);

	return 0;
}

void exit_kthread(void)
{
	kthread_stop(kthread);

	return ;
}

module_init(init_kthread);
module_exit(exit_kthread);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Wang Xiaoqiang <wang_xiaoq@126.com>");
