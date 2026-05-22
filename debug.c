#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mm.h"
#include "mm64.h"
#include "libmem.h"
#include "cpu.h"
#include "syscall.h"
#include "queue.h"
#include "sched.h"

void show_memory_status_direct(struct pcb_t *proc) {
    if (!proc || !proc->krnl || !proc->krnl->mm) return;
    
    struct mm_struct *mm = proc->krnl->mm;
    struct vm_area_struct *vma = mm->mmap;
    
    printf("\n=== DIRECT MEMORY STATUS (Kernel Space) ===\n");
    printf("PID: %d\n", proc->pid);
    printf("Heap: start=0x%08lx, sbrk=0x%08lx, end=0x%08lx\n", 
           vma->vm_start, vma->sbrk, vma->vm_end);
    printf("Used heap: %lu bytes\n", vma->sbrk - vma->vm_start);
    printf("Free heap: %lu bytes\n", vma->vm_end - vma->sbrk);
    
    printf("\nAllocated regions:\n");
    for (int i = 0; i < PAGING_MAX_SYMTBL_SZ; i++) {
        // ✅ Sửa: kiểm tra rg_end thay vì rg_start
        if (mm->symrgtbl[i].rg_end != 0) {
            printf("  reg[%d]: 0x%08lx -> 0x%08lx (size=%lu)\n", 
                   i, mm->symrgtbl[i].rg_start, mm->symrgtbl[i].rg_end,
                   mm->symrgtbl[i].rg_end - mm->symrgtbl[i].rg_start);
        }
    }
    printf("============================================\n");
}

int main() {
    printf("=== DEBUG MEMORY STATUS ===\n\n");
    
    // Init RAM
    struct memphy_struct mram;
    init_memphy(&mram, 4 * 1024 * 1024, 1);
    
    // Init SWAP
    struct memphy_struct mswp[PAGING_MAX_MMSWP];
    for (int i = 0; i < PAGING_MAX_MMSWP; i++) {
        init_memphy(&mswp[i], 2 * 1024 * 1024, 0);
    }
    
    // Init kernel
    struct krnl_t os = {0};
    os.mram = &mram;
    os.active_mswp = &mswp[0];
    
    // Init queues
    struct queue_t ready_q = {0};
    struct queue_t running_q = {0};
    os.ready_queue = &ready_q;
    os.running_list = &running_q;
    
    // Init process
    struct pcb_t proc = {0};
    proc.pid = 1;
    proc.krnl = &os;
    proc.pc = 0;
    
    // Init code segment
    proc.code = malloc(sizeof(struct code_seg_t));
    proc.code->size = 10;
    proc.code->text = calloc(10, sizeof(struct inst_t));
    
    // Init MM
    os.mm = malloc(sizeof(struct mm_struct));
    init_mm(os.mm, &proc);
    
    // Add to queue
    os.ready_queue->proc[0] = &proc;
    os.ready_queue->size = 1;
    os.running_list->proc[0] = &proc;
    os.running_list->size = 1;
    
    // Show initial status
    show_memory_status_direct(&proc);
    
    // Perform ALLOC
    struct inst_t ins;
    ins.opcode = ALLOC;
    ins.arg_0 = 100;
    ins.arg_1 = 0;
    proc.code->text[0] = ins;
    proc.pc = 0;
    run(&proc);
    printf("\nALLOC 100 -> reg[0] = 0x%08lx\n", proc.regs[0]);
    
    ins.arg_0 = 200;
    ins.arg_1 = 1;
    proc.code->text[1] = ins;
    proc.pc = 1;
    run(&proc);
    printf("ALLOC 200 -> reg[1] = 0x%08lx\n", proc.regs[1]);
    
    // Show status after allocations
    show_memory_status_direct(&proc);
    
    // Cleanup
    free(proc.code->text);
    free(proc.code);
    free(os.mm);
    
    return 0;
}