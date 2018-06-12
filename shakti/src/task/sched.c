#include <klib.h>
#include <sched.h>

process_t processes[256];
thread_t process_threads[256][256];
size_t nprocesses = 0;

static int find_free_process()
{
	for (size_t i = 0; i < 256; i++) {
		if (processes[i].attached == 0) {
			processes[i].attached = 1;
			return i;
		}
	}
	return -1;
}

static int find_free_thread(process_t *process)
{
	for (size_t i = 0; i < 256; i++) {
		if (process->threads[i].attached == 0) {
			process->threads[i].attached = 1;
			return i;
		}
	}
	return -1;
}

int sched_create(char *name, uint32_t ds, uint32_t cr3, uint32_t eip, uint32_t esp, uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx)
{
	if (kstrlen(name) > 20) {
		return -1;
	}
	int index = find_free_process();
	if (index == -1) {
		return -1;
	}
	kmemcpy(processes[index].name, name, kstrlen(name) + 1);
	processes[index].cr3 = cr3;
	processes[index].ds = ds;
    
    sched_create_thread(index, eip, esp, eax, ebx, ecx, edx);
    
    nprocesses++;
	return 0;
}

int sched_create_thread(int pid, uint32_t eip, uint32_t esp, uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx)
{
	int index = find_free_thread(&processes[pid]);
	if (index == -1) {
		return -1;
	}
	processes[pid].threads[index].eip = eip;
	processes[pid].threads[index].esp = esp;
	processes[pid].threads[index].eax = eax;
	processes[pid].threads[index].ebx = ebx;
	processes[pid].threads[index].ecx = ecx;
	processes[pid].threads[index].edx = edx;
    processes[pid].nthreads++;
	return index;
}

int sched_init()
{
	for (size_t i = 0; i < 256; i++) {
		processes[i].attached = 0;
		processes[i].threads = process_threads[i];
		for (size_t j = 0; j < 256; j++) {
			processes[i].threads[j].attached = 0;
		}
	}
	return 0;
}

void schedule()
{

}
