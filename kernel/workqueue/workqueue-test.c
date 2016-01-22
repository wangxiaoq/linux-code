/* Copyright (c) 2016 by Wang Xiaoqiang<wang_xiaoq@126.com>.

   workqueue-test is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; version
   2.

   workqueue-test is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should find a copy of v2 of the GNU General Public License somewhere
   on your Linux system; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include <linux/workqueue.h>
#include <linux/kernel.h>
#include <linux/module.h>

struct workqueue_struct *wkqueue;

/* Just a simulate work of my study */
struct mywork {
	int kernel_programming;
	int user_programming;
	struct work_struct work;
};

struct mywork mywork;

static void work_handler(struct work_struct *work)
{
	struct mywork *mywork = container_of(work, struct mywork, work);
	mywork->kernel_programming = 10;
	mywork->user_programming = 20;
	
	pr_info("workqueue-test: kernel programming study progress: %d%%\n",
		mywork->kernel_programming);
	pr_info("workqueue-test: user programming study progress: %d%%\n",
		mywork->user_programming);
}

int __init workqueue_init(void)
{
	INIT_WORK(&(mywork.work), &work_handler);

	wkqueue = create_workqueue("mywkqueue");
	if (!wkqueue) {
		pr_err("workqueue-test: create workqueue failed\n");
		return -ENOMEM;
	}

	queue_work(wkqueue, &(mywork.work));

	return 0;
}

void __exit workqueue_exit(void)
{
	destroy_workqueue(wkqueue);

	return ;
}

module_init(workqueue_init);
module_exit(workqueue_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wang Xiaoqiang <wang_xiaoq@126.com>");
