#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>
#include <linux/sched.h>

asmlinkage long sys_get_demand (int pid, struct d_params *d_arguments){

	struct task_struct *temp;
	struct d_params to_return;
	unsigned long result;	
	
	
	if(pid==-1){
		to_return.demand = current->demand;
	}
	else if (pid <=0){
		printk("Illegal pid given\n\n");
		return EINVAL;
	}
	else{
		temp = find_task_by_vpid(pid);
		if(temp == NULL){printk("Process doesn't exists\n"); return EINVAL;}
		if(current->pid == temp->parent->pid){
				to_return.demand = temp->demand;
		}
		else{
			printk("Not a child process\n\n");
			return EINVAL;
		}	
	}
	if(!d_arguments){
		return EINVAL;
	}

	result = copy_to_user(d_arguments, &to_return, sizeof(struct d_params));
	if(result == 0){
		return 0;
	}else{
		return EINVAL;
	}
	return 0;
}
