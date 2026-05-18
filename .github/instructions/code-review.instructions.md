---
applyTo: "**/*.c, **/*.h"
excludeAgent: "cloud-agent"
---

# PR Code Review Guidelines (feature/* -> development)
Your role is to strictly evaluate Pull Requests and catch bugs before the code is merged into `development`. Enforce the following criteria and specifications:

## 1. Coding Standards & Memory Safety
- Flag C code that violates the GNU C Coding Style (applies only to syscall-related files).
- Analyze memory leaks: Ensure every `malloc` has a corresponding `free`. The CI system uses Valgrind, so proactively catch unmapped memory.
- Detect unused variables, null pointer dereferences, or buffer overflows.

## 2. Module Specifications (Feature Specs)

### Scheduler (`sched.c`, `queue.c`)
- MLQ Algorithm: Processes must be enqueued into the correct priority queue.
- CPU Slot Allocation: Verify the formula `slot = MAX_PRIO - prio` (where `MAX_PRIO = 140`).
- Reject any code utilizing `run_queue` logic (marked as obsolete in documentation).

### Memory & Paging (`mm.c`, `mm64.c`, `mm-vm.c`, `libmem.c`)
- 64-bit Paging: Extract bits correctly for 5-level paging: PGD (56-48), P4D (47-39), PUD (38-30), PMD (29-21), PT (20-12).
- Page Fault Handling: When a page is missing (Present bit = 0), the code MUST call `find_victim_page()` and `__swap_cp_page()`. Do not rewrite inline replacement logic.
- Memory Protection: No direct pointer dereferencing from user mode bypassing `krnl_t`.

### Synchronization (`sched.c`, `queue.c`, `mm-vm.c`)
- Race Conditions: All Read/Write operations on shared resources (`mlq_ready_queue`, `memphy_struct`) must be strictly enclosed within `pthread_mutex_lock()` and `pthread_mutex_unlock()`.
- Verify the absence of Deadlocks (circular wait).
- Page Replacement: `find_victim_page()` must only fetch the target page (e.g., via `fifo_pgn` list). It is strictly forbidden to perform page swapping logic within this function.

### Syscall (`syscall.c`, `sys_*.c`, `syscalltbl.sh`)
- Parameters must be packed via `struct sc_regs`. Reject bare parameters.
- Memmap Routing: Syscall 17 must correctly route `regs->a1` to the following handlers:
  - `SYSMEM_MAP_OP` -> `vmap_pgd_memset()`
  - `SYSMEM_INC_OP` -> `inc_vma_limit()`
  - `SYSMEM_SWP_OP` -> `__mm_swap_page()`
  - `SYSMEM_IO_READ` -> `MEMPHY_read()`
  - `SYSMEM_IO_WRITE` -> `MEMPHY_write()`

## 3. Commenting Rules
- Quote the exact faulty code snippet and immediately provide the corrected code.
- State the reason briefly based strictly on the system specs. Avoid wordy explanations.
