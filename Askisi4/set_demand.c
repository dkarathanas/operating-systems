#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/types.h>

int total_demand = 0;

asmlinkage long sys_set_demand (int pid, int demand){
	struct task_struct *temp;
	int tempDemand = 0;

	if(demand < 0){printk("demand<0"); return EINVAL;}
	if ((pid<-1) || (pid==0)){
		printk("pid < -1, error\n\n");
		return EINVAL;
	}
	else if(pid==-1){
		tempDemand = total_demand - current->demand + demand;
		if(tempDemand <= 100){
			total_demand =total_demand + demand - current->demand;
			current->demand = demand;
			printk("total_demand = %d, demand set\n\n", total_demand);
			return 0;
		}
		else{
			printk("ERROR,total_demand = %d,demand = %d,>=100\n\n",total_demand, demand);
			return EINVAL;
		}
	}
	else{
			temp = find_task_by_vpid(pid);
			if(temp == NULL){printk("Process doesn't exists\n");return EINVAL;}
			if(current->pid == temp->parent->pid){
				tempDemand = total_demand - temp->demand + demand;
				if(tempDemand <= 100){
					total_demand = total_demand + demand - temp->demand;
					temp->demand = demand;
					return 0;
				}
			}
			else{
				printk("Error, total demand >= 100\n\n");
				return EINVAL;
			}	
	}
	return EINVAL;
}
