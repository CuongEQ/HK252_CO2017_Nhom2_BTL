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
#include <stdlib.h>

#ifdef MM64
#include "mm64.h"
#else
#include "mm.h"
#endif

// Find the process in the given queue by PID



// Find the process by PID in `running_list`, `ready_queue` and
// `mlq_ready_queue` (if supported)

extern struct pcb_t *get_proc_by_pid(uint32_t pid);

static struct pcb_t *
find_proc_by_pid (struct krnl_t *krnl, uint32_t pid)
{
  return get_proc_by_pid(pid);
}


int
__sys_memmap (struct krnl_t *krnl, uint32_t pid, struct sc_regs *regs)
{
  BYTE value; // Temporary variable to hold the value read from memory for
              // SYSMEM_IO_READ
  // Input validation
  if (krnl == NULL || regs == NULL){
    return -1;

  }

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
      if (MEMPHY_read (krnl->mram, (addr_t)regs->a2, &value) != 0)
        return -1;
      regs->a3 = value;
      break;
    case SYSMEM_IO_WRITE:
      if (krnl->mram == NULL)
        return -1;
      if (MEMPHY_write (krnl->mram, (addr_t)regs->a2, (BYTE)regs->a3) != 0)
        return -1;
      break;
    default: // Invalid memory operation code
      printf ("Memop code: %d\n", memop);
      return -1;
    }

  return 0;
}