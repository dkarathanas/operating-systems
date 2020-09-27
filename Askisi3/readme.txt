1) defined the 2 system call numbers (341,342) at: linux-2.6.38.1/arch/x86/include/asm/unistd_32.h

2) defined the funct pointers at: linux-2.6.38.1/arch/x86/kernel/syscall_table_32.S

3) defined the functions set and get demand at: linux-2.6.38.1/include/asm-generic/syscalls.h

4) created the 2 .c files here :  linux-2.6.38.1/kernel/   in order to implement the system calls

5) also, i added the necessary .o files to Makefile and modified kernel's version with my login name

6) created the file d_params.h here: linux-2.6.38.1/include/linux/d_params.h in order to implement 
the struct d_params and keep info about the demand of the process.

7) set total_demand in file set_demand.c in order to check everytime if we can give to the process
the amount of time it demands

8)initialized demand = 0 at include/linx/init_task.h

TESTFILE WITH ARGV[]

	./a.out 1	set current process demand 10

	./a.out 2	set current process demand 120 	ERROR

	./a.out 3	set current process demand 15
			get current process demand

	./a.out 4	set current process demand 60 2 times, VALID

	./a.out 5	fork and set childs process demand 15
			get child process demand, 	       VALID

	./a.out 6	set random process demand 10	ERROR
