/* Copyright (c) 2016 by Wang Xiaoqiang.

   perf-test is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; version
   2.

   perf-test is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should find a copy of v2 of the GNU General Public License somewhere
   on your Linux system; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include <linux/perf_event.h>
#include <linux/kernel.h>
#include <linux/module.h>

#define SAMPLE_PERIOD	1000

/* TODO: command line parameter */
static  __u64 cache_misses = SAMPLE_PERIOD;

struct cpu_work {
	struct irq_work work;
	struct perf_event *event;
};

static struct cpu_work __percpu *cpu_work;

struct perf_event_attr cache_misses_attr = {
	.type = PERF_TYPE_HARDWARE,
	.config = PERF_COUNT_HW_CACHE_MISSES,
	.size = sizeof(struct perf_event_attr),
	.disabled = 1,
	.pinned = 1,
	.exclude_kernel = 1,
};

static void cache_misses_each_cpu_handler(struct irq_work *work)
{
	struct cpu_work *cwork = per_cpu_ptr(cpu_work, smp_processor_id());

	pr_info("perf-test: cpu %d cache misses expired\n", smp_processor_id());
#if 0
	pr_info("perf-test: !PERF_HES_STOPPED: %s\n", (!(cwork->event->hw.state & PERF_HES_STOPPED))?"yes":"no");
	pr_info("perf-test: %d\n", cwork->event->hw.idx);
#endif

	cwork->event->pmu->start(cwork->event, PERF_EF_RELOAD);
}

void cache_misses_overflow_handler(struct perf_event *event, 
	struct perf_sample_data *data, struct pt_regs *regs)
{
	struct cpu_work *cwork = per_cpu_ptr(cpu_work, smp_processor_id());
	irq_work_queue(&(cwork->work));
}

static int create_counter_on_each_cpu(struct perf_event_attr *attr)
{
	int cpu;
	struct cpu_work *cwork;
	attr->sample_period = cache_misses;

	for_each_online_cpu(cpu) {
		cwork = per_cpu_ptr(cpu_work, cpu);
		init_irq_work(&(cwork->work), cache_misses_each_cpu_handler);
		cwork->event = perf_event_create_kernel_counter(&cache_misses_attr,
			cpu, NULL, cache_misses_overflow_handler, NULL);
		if (IS_ERR_OR_NULL(cwork->event)) {
			pr_err("perf-test: create counter fail on cpu %d\n", cpu);
			return -EOPNOTSUPP;
		}
	}

	return 0;
}

static void __start_counter(void *arg)
{
	struct cpu_work *cwork = this_cpu_ptr(cpu_work);
	cwork->event->pmu->add(cwork->event, PERF_EF_START);
}

static void start_counters(void)
{
	on_each_cpu(__start_counter, NULL, 0);
}

static void disable_counter(void *arg)
{
	struct cpu_work *cwork = this_cpu_ptr(cpu_work);

	cwork->event->pmu->stop(cwork->event, PERF_EF_UPDATE);
	cwork->event->pmu->del(cwork->event, 0);
}

static void destroy_counter_on_each_cpu(void)
{
	on_each_cpu(disable_counter, NULL, 0);
}

static int __init perf_init(void)
{
	int err;
	
	cpu_work = alloc_percpu(struct cpu_work);
	smp_mb();
	err = create_counter_on_each_cpu(&cache_misses_attr);
	if (err) {
		pr_err("perf-test: create couter failed on some cpus\n");
		return -EOPNOTSUPP;
	}

	start_counters();

	return 0;
}

static void __exit perf_exit(void)
{	
	int cpu;
	struct cpu_work *cwork;

	destroy_counter_on_each_cpu();
	for_each_online_cpu(cpu) {
		cwork = per_cpu_ptr(cpu_work, cpu);
		if (cwork->event)
			perf_event_release_kernel(cwork->event);
	}
	smp_mb();
	free_percpu(cpu_work);

	return ;
}

module_init(perf_init);
module_exit(perf_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wang Xiaoqiang <wang_xiaoq@126.com>");
