#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

struct d_params {
	int demand;
};

long set_demand(int pid, int demand){
	return syscall(341, pid, demand);
}

long get_demand(int pid, struct d_params *d_arguments){
	return syscall(342, pid, d_arguments);
}


int main(int argc, char *argv[]){

	long sysNumber, sysNumber2;
	struct d_params *param;
	pid_t child_pid;
	
	printf("Trap to kernel level\n");
	switch(atoi(argv[1]))
{
		case (1):
			sysNumber = set_demand(-1, 10);
			if (sysNumber == 0){ printf("SystemCall set_demand : Success\n");}
			break;

		case (2):
			sysNumber = set_demand(-1, 120);
			if (sysNumber == 0){ printf("SystemCall set_demand : Success\n");}
			break;

		case (3):
			sysNumber = set_demand(-1, 15);
			if (sysNumber == 0){ printf("SystemCall set_demand : Success\n");}
			param = (struct d_params*)malloc(sizeof(struct d_params));
			sysNumber2 = get_demand(-1, param);
			if (sysNumber2 == 0){ printf("SystemCall get_demand : Success, demand = %d \n", param->demand);
			printf("get_demand returned %d\n", param->demand);}
			break;

		case (4):
			sysNumber = set_demand(-1, 60);
			if (sysNumber == 0){ printf("SystemCall set_demand : Success\n");}
			sysNumber = set_demand(-1, 60);
			if (sysNumber == 0) {printf("SystemCall set_demand : Success\n");}
			break;
		case (5):
			child_pid = fork();
			if(child_pid == 0){
				while(1) ;
			}
			sysNumber = set_demand(child_pid, 15);
			if(sysNumber == 0) {printf("ChildPid's demand set\n");}
			param = (struct d_params*)malloc(sizeof(struct d_params));
			sysNumber2 = get_demand(child_pid,param);
			if(sysNumber2 == 0) {printf("SystemCall get_demand : Success\n");
			printf("get_demand returned %d\n",param->demand);}
			break;
		case (6):
			sysNumber = set_demand(125235, 10);
			param = (struct d_params*)malloc(sizeof(struct d_params));
			sysNumber2 = get_demand(117251, param);
			break;
		default:
			break;
}
	printf("Back to user level\n");
	return 0;
}
