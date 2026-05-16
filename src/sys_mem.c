/*
 * Copyright (C) 2026 pdnguyen of HCMC University of Technology VNU-HCM
 */

/* Caitoa release
 * Source Code License Grant: The authors hereby grant to Licensee
 * personal permission to use and modify the Licensed Source Code
 * for the sole purpose of studying while attending the course CO2018.
 */

#include "libmem.h"
#include "os-mm.h"
#include "queue.h"
#include "syscall.h"

#ifdef MM64
#include "mm64.h"
#else
#include "mm.h"
#endif

// Find the process in the given queue by PID
static struct pcb_t *
find_proc_in_queue (struct queue_t *q, uint32_t pid)
{
  // Input validation
  if (q == NULL)
    return NULL;

  for (int i = 0; i < q->size; i++)
    {
      if (q->proc[i] != NULL && q->proc[i]->pid == pid)
        return q->proc[i];
    }

  return NULL;
}

// Find the process by PID in `running_list`, `ready_queue` and
// `mlq_ready_queue` (if supported)
static struct pcb_t *
find_proc_by_pid (struct krnl_t *krnl, uint32_t pid)
{
  struct pcb_t *proc = NULL;

  if (krnl == NULL)
    return NULL;

  proc = find_proc_in_queue (krnl->running_list,
                             pid); // Check in running_list first
  if (proc != NULL)
    return proc;

  proc = find_proc_in_queue (krnl->ready_queue,
                             pid); // Check in ready_queue next
  if (proc != NULL)
    return proc;

#ifdef MLQ_SCHED // If MLQ_SCHED is supported, check in mlq_ready_queue last
  if (krnl->mlq_ready_queue != NULL)
    {
      for (int prio = 0; prio < MAX_PRIO; prio++)
        {
          proc = find_proc_in_queue (&krnl->mlq_ready_queue[prio], pid);
          if (proc != NULL)
            return proc;
        }
    }
#endif

  return NULL;
}

int
__sys_memmap (struct krnl_t *krnl, uint32_t pid, struct sc_regs *regs)
{
  BYTE value; // Temporary variable to hold the value read from memory for
              // SYSMEM_IO_READ

  // Input validation
  if (krnl == NULL || regs == NULL)
    return -1;

  int memop = (int)regs->a1; // Memory operation code
  struct pcb_t *caller
      = find_proc_by_pid (krnl, pid); // Find the caller process by PID

  /*
   * @bksysnet: Please note in the dual spacing design
   *            syscall implementations are in kernel space.
   */

  switch (memop)
    {
    case SYSMEM_MAP_OP:
      /* Reserved process case */
      if (caller == NULL)
        return -1;
      vmap_pgd_memset (caller, (addr_t)regs->a2, (int)regs->a3);
      break;
    case SYSMEM_INC_OP:
      if (caller == NULL)
        return -1;
      inc_vma_limit (caller, (int)regs->a2, (addr_t)regs->a3);
      break;
    case SYSMEM_SWP_OP:
      if (caller == NULL)
        return -1;
      __mm_swap_page (caller, (addr_t)regs->a2, (addr_t)regs->a3);
      break;
    case SYSMEM_IO_READ:
      if (krnl->mram == NULL)
        return -1;
      MEMPHY_read (krnl->mram, (addr_t)regs->a2, &value);
      regs->a3 = value;
      break;
    case SYSMEM_IO_WRITE:
      if (krnl->mram == NULL)
        return -1;
      MEMPHY_write (krnl->mram, (addr_t)regs->a2, (BYTE)regs->a3);
      break;
    default: // Invalid memory operation code
      printf ("Memop code: %d\n", memop);
      return -1;
    }

  return 0;
}
