asmlinkage void schedule(void)
{
	struct schedule_data * sched_data;
	struct task_struct *prev, *next, *p;
	struct list_head *tmp;
	int this_cpu, c;
	int need_resched;/*控制外层循环*/
	int recalculate;/*控制内层循环*/
		
	if (!current->active_mm) BUG();
	/*外层循环*/
	do
	{
		need_resched = 0;
		prev = current;
		this_cpu = prev->processor;
	
		if (in_interrupt())
		{
			printk("Scheduling in interrupt\n");
			BUG();
			return;
		}
		
		release_kernel_lock(prev, this_cpu);
	
		/* Do "administrative" work here while we don't hold any locks */
		if (softirq_active(this_cpu) & softirq_mask(this_cpu))
		{
			do_softirq();
		}
		/*
		 * 'sched_data' is protected by the fact that we can run
		 * only one process per CPU.
		 */
		sched_data = & aligned_data[this_cpu].schedule_data;
	
		spin_lock_irq(&runqueue_lock);
	
		/* move an exhausted RR process to be last.. */
		if (prev->policy == SCHED_RR)
		{
			if (!prev->counter) {
				prev->counter = NICE_TO_TICKS(prev->nice);
				move_last_runqueue(prev);
			}
		}
		switch (prev->state) {
			case TASK_INTERRUPTIBLE:
				if (signal_pending(prev)) {
					prev->state = TASK_RUNNING;
					break;
				}
			default:
				del_from_runqueue(prev);
			case TASK_RUNNING:
		}
		prev->need_resched = 0;
	
		/*
		 * this is the scheduler proper:
		 */
		/*内层循环*/
		do
		{
			recalculate = 0;
			/*
			 * Default process to select..
			 */
			next = idle_task(this_cpu);
			c = -1000;
			if (prev->state == TASK_RUNNING)
			{
				c = goodness(prev, this_cpu, prev->active_mm);
				next = prev;
			}
			
			list_for_each(tmp, &runqueue_head) {
				p = list_entry(tmp, struct task_struct, run_list);
				if (can_schedule(p, this_cpu)) {
					int weight = goodness(p, this_cpu, prev->active_mm);
					if (weight > c)
						c = weight, next = p;
				}
			}
		
			/* Do we need to re-calculate counters? */
			if (!c)
			{
				struct task_struct *p;
				spin_unlock_irq(&runqueue_lock);
				read_lock(&tasklist_lock);
				for_each_task(p)
					p->counter = (p->counter >> 1) + NICE_TO_TICKS(p->nice);
				read_unlock(&tasklist_lock);
				spin_lock_irq(&runqueue_lock);
				recalculate = 1;
			}
			
		}while(recalculate);/*内层循环结束*/
		/*
		 * from this point on nothing can prevent us from
		 * switching to the next task, save this fact in
		 * sched_data.
		 */
		sched_data->curr = next;
	#ifdef CONFIG_SMP
	 	next->has_cpu = 1;
		next->processor = this_cpu;
	#endif
		spin_unlock_irq(&runqueue_lock);
	
		if (prev == next)
		{
			reacquire_kernel_lock(current);
			if (current->need_resched)
			{
				continue;
			}
			else
				break;
			
		}
		
	
	#ifdef CONFIG_SMP
	 	/*
	 	 * maintain the per-process 'last schedule' value.
	 	 * (this has to be recalculated even if we reschedule to
	 	 * the same process) Currently this is only used on SMP,
		 * and it's approximate, so we do not have to maintain
		 * it while holding the runqueue spinlock.
	 	 */
	 	sched_data->last_schedule = get_cycles();
	
		/*
		 * We drop the scheduler lock early (it's a global spinlock),
		 * thus we have to lock the previous process from getting
		 * rescheduled during switch_to().
		 */
	
	#endif /* CONFIG_SMP */
	
		kstat.context_swtch++;
		/*
		 * there are 3 processes which are affected by a context switch:
		 *
		 * prev == .... ==> (last => next)
		 *
		 * It's the 'much more previous' 'prev' that is on next's stack,
		 * but prev is set to (the just run) 'last' process by switch_to().
		 * This might sound slightly confusing but makes tons of sense.
		 */
		prepare_to_switch();
		{
			struct mm_struct *mm = next->mm;
			struct mm_struct *oldmm = prev->active_mm;
			if (!mm) {
				if (next->active_mm) BUG();
				next->active_mm = oldmm;
				atomic_inc(&oldmm->mm_count);
				enter_lazy_tlb(oldmm, next, this_cpu);
			} else {
				if (next->active_mm != mm) BUG();
				switch_mm(oldmm, mm, next, this_cpu);
			}
	
			if (!prev->mm) {
				prev->active_mm = NULL;
				mmdrop(oldmm);
			}
		}
	
		/*
		 * This just switches the register state and the
		 * stack.
		 */
		switch_to(prev, next, prev);
		__schedule_tail(prev);
	
		reacquire_kernel_lock(current);
		if (current->need_resched)
			need_resched = 1;
		
	}while(need_resched);/*外层循环结束*/
	
	return;
}


