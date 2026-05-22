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

void show_memory_status(struct pcb_t *proc) {
    if (!proc || !proc->krnl || !proc->krnl->mm) return;
    
    struct mm_struct *mm = proc->krnl->mm;
    struct vm_area_struct *vma = mm->mmap;
    
    // Tính actively used
    size_t used = 0;
    for (int i = 0; i < PAGING_MAX_SYMTBL_SZ; i++) {
        if (mm->symrgtbl[i].rg_end != 0) {
            used += mm->symrgtbl[i].rg_end - mm->symrgtbl[i].rg_start;
        }
    }
    
    printf("\n=== MEMORY STATUS ===\n");
    printf("sbrk: %lu bytes, Actively used: %lu bytes\n", 
           vma->sbrk - vma->vm_start, used);
    
    printf("Allocated:\n");
    for (int i = 0; i < PAGING_MAX_SYMTBL_SZ; i++) {
        if (mm->symrgtbl[i].rg_end != 0) {
            printf("  reg[%d]: 0x%08lx -> 0x%08lx (%lu bytes)\n", 
                   i, mm->symrgtbl[i].rg_start, mm->symrgtbl[i].rg_end,
                   mm->symrgtbl[i].rg_end - mm->symrgtbl[i].rg_start);
        }
    }
    
    printf("Free list:\n");
    struct vm_rg_struct *free_rg = vma->vm_freerg_list;
    while (free_rg) {
        if (free_rg->rg_start < free_rg->rg_end) {
            printf("  0x%08lx -> 0x%08lx (%lu bytes)\n", 
                   free_rg->rg_start, free_rg->rg_end,
                   free_rg->rg_end - free_rg->rg_start);
        }
        free_rg = free_rg->rg_next;
    }
    printf("====================\n");
}

int main() {
    printf("=== DEBUG: FREE & REALLOCATE ===\n");
    
    // Init
    struct memphy_struct mram;
    init_memphy(&mram, 4 * 1024 * 1024, 1);
    
    struct memphy_struct mswp[PAGING_MAX_MMSWP];
    for (int i = 0; i < PAGING_MAX_MMSWP; i++) {
        init_memphy(&mswp[i], 2 * 1024 * 1024, 0);
    }
    
    struct krnl_t os = {0};
    os.mram = &mram;
    os.active_mswp = &mswp[0];
    
    struct queue_t ready_q = {0}, running_q = {0};
    os.ready_queue = &ready_q;
    os.running_list = &running_q;
    
    struct pcb_t proc = {0};
    proc.pid = 1;
    proc.krnl = &os;
    
    proc.code = malloc(sizeof(struct code_seg_t));
    proc.code->size = 20;
    proc.code->text = calloc(20, sizeof(struct inst_t));
    
    os.mm = malloc(sizeof(struct mm_struct));
    init_mm(os.mm, &proc);
    
    os.ready_queue->proc[0] = &proc;
    os.ready_queue->size = 1;
    os.running_list->proc[0] = &proc;
    os.running_list->size = 1;
    
    struct inst_t ins;
    
    // 1. Cấp phát 3 vùng
    printf("\n--- 1. Allocate 100, 200, 50 bytes ---\n");
    ins.opcode = ALLOC; ins.arg_0 = 100; ins.arg_1 = 0;
    proc.code->text[0] = ins; proc.pc = 0; run(&proc);
    printf("reg[0]=0x%08lx\n", proc.regs[0]);
    
    ins.arg_0 = 200; ins.arg_1 = 1;
    proc.code->text[0] = ins; proc.pc = 0; run(&proc);
    printf("reg[1]=0x%08lx\n", proc.regs[1]);
    
    ins.arg_0 = 50; ins.arg_1 = 2;
    proc.code->text[0] = ins; proc.pc = 0; run(&proc);
    printf("reg[2]=0x%08lx\n", proc.regs[2]);
    
    show_memory_status(&proc);
    
    // 2. Free vùng giữa (reg[1])
    printf("\n--- 2. Free reg[1] (200 bytes) ---\n");
    ins.opcode = FREE; ins.arg_0 = 1;
    proc.code->text[0] = ins; proc.pc = 0; run(&proc);
    show_memory_status(&proc);
    
    // 3. Cấp phát mới - phải tái sử dụng vùng vừa free
    printf("\n--- 3. Allocate 150 bytes (should reuse freed space) ---\n");
    ins.opcode = ALLOC; ins.arg_0 = 150; ins.arg_1 = 3;
    proc.code->text[0] = ins; proc.pc = 0; run(&proc);
    printf("reg[3]=0x%08lx\n", proc.regs[3]);
    show_memory_status(&proc);
    
    // 4. Cấp phát thêm - phải mở rộng heap
    printf("\n--- 4. Allocate 500 bytes (need to expand heap) ---\n");
    ins.opcode = ALLOC; ins.arg_0 = 500; ins.arg_1 = 4;
    proc.code->text[0] = ins; proc.pc = 0; run(&proc);
    printf("reg[4]=0x%08lx\n", proc.regs[4]);
    show_memory_status(&proc);
    
    // Cleanup
    free(proc.code->text);
    free(proc.code);
    free(os.mm);
    
    printf("\n=== DONE ===\n");
    return 0;
}