/* Copyright (c) 2016 by Wang Xiaoqiang.

   hrtimer-test is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; version
   2.

   hrtimer-test is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should find a copy of v2 of the GNU General Public License somewhere
   on your Linux system; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include <linux/ktime.h>
#include <linux/hrtimer.h>
#include <linux/module.h>
#include <linux/kernel.h>

#define TIME	1000000000	/* 1 second */

struct hrtimer timer;

/* only used in timer_handler, no need to use lock to protect */
static int count;

static enum hrtimer_restart timer_handler(struct hrtimer *timer)
{
	ktime_t now = timer->base->get_time();

	count ++;
	pr_info("timer expired %d times\n", count);

	hrtimer_forward(timer, now, ktime_set(0, TIME));

	return HRTIMER_RESTART;
}

int __init init_hrtimer(void)
{
	hrtimer_init(&timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL_PINNED);
	timer.function = &timer_handler;
	hrtimer_start(&timer, ktime_set(0, TIME), HRTIMER_MODE_REL_PINNED);

	return 0;
}

void __exit exit_hrtimer(void)
{
	hrtimer_cancel(&timer);

	return;
}

module_init(init_hrtimer);
module_exit(exit_hrtimer);

MODULE_AUTHOR("Wang Xiaoqiang <wang_xiaoq@126.com>");
MODULE_LICENSE("GPL");
