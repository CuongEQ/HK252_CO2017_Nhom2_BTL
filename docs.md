# HCMC UNIVERSITY OF TECHNOLOGY

## FACULTY OF COMPUTER SCIENCE & ENGINEERING

**Course: Operating Systems**
**Assignment - Simple Operating System**
*March 17, 2026*

**Goals:** The objective of this assignment is to simulate major components of a simple operating system, such as the scheduler, synchronization mechanisms, and physical and virtual memory operations.

**Contents:** In detail, students will practice with three major modules: the scheduler, synchronization, and the mechanism for memory allocation from virtual to physical memory:

* scheduler
* synchronization
* memory-allocation operations from virtual to physical memory

In addition, students will practice designing and implementing a simple operating system programming interface through system calls.

**Results:** After this assignment, students will partially understand the principles of a simple OS. They will be able to explain and illustrate the roles of key OS modules.

---

### CONTENTS

* **1 Introduction**
  * 1.1 An overview
  * 1.2 Source Code
  * 1.3 Processes
  * 1.4 How to Create a Process?
  * 1.5 How to Run the Simulation
  * 1.6 How to Write the Kernel Interface
    * 1.6.1 The kernel structure
    * 1.6.2 The system call
    * 1.6.3 Adding a system call to Simple Operating System
* **2 Implementation**
  * 2.1 Scheduler
  * 2.2 Memory Management
    * 2.2.1 The process memory layout
    * 2.2.2 The virtual memory mapping in each process
    * 2.2.3 The system's physical memory
    * 2.2.4 Paging-based address translation scheme
    * 2.2.5 Wrapping-up all paging-oriented implementations
  * 2.3 Put It All Together
* **3 Submission**
  * 3.1 Source code
  * 3.2 Requirements
  * 3.3 Report
  * 3.4 Grading
  * 3.5 Code of ethics

---

# 1 Introduction

## 1.1 An overview

This assignment simulates a simple operating system to help students understand the fundamental concepts of scheduling, synchronization, and memory management. In general, the OS manages two virtual resources, CPU(s)/co-processor(s) and RAM, by using two core components:

* **Scheduler (and Dispatcher):** determines which process is allowed to run on which CPU.
* **Virtual memory:** isolates the memory space of each process from others. Physical RAM is shared by multiple processes, but each process is unaware of the others. This is done by giving each process its own virtual memory space, and the virtual memory engine maps and translates the virtual addresses provided by processes to the corresponding physical addresses.

The design supports dual-mode operation, which ensures system protection and controlled access to hardware resources. The OS operates in two modes: user mode and kernel mode, distinguished by a hardware-supported mode bit that indicates whether the CPU is executing user code or privileged kernel code. This mechanism guarantees that user programs cannot execute privileged instructions to preserve system stability and security. Through these modules, the OS allows multiple user-created processes to share and use virtual computing resources. Therefore, in this assignment, we focus on implementing the scheduler/dispatcher and the virtual memory engine.

## 1.2 Source Code

After downloading the source code of the assignment and extracting it, you will see the source code organized as follows:

* **Header files**
  * `timer.h`: defines the timer for the whole system.
  * `cpu.h`: defines functions used to implement the virtual CPU.
  * `queue.h`: defines functions used to implement the queue that holds process PCBs.
  * `sched.h`: defines functions used by the scheduler.
  * `mem.h`: (obsolete) defines functions used by the Virtual Memory Engine.
  * `loader.h`: (obsolete) defines functions used by the loader, which loads the program from disk into memory.
  * `common.h`: defines structs and functions used throughout the OS.
  * `bitopts.h`: defines operations on bit data.
  * `os-mm.h`, `mm.h`, `mm64.h`: define the structures and basic data for paging-based memory management.
  * `syscall.h`: defines system call headers.
  * `libxxx.h`: (Optional) defines the standard library headers.
  * `os-cfg.h`: (Optional) defines the constants used to switch software configurations.

* **Source files**
  * `timer.c`: implements the timer.
  * `cpu.c`: implements the virtual CPU.
  * `queue.c`: implements operations on (priority) queues.
  * `paging.c`: (obsolete) is used to check the functionality of the Virtual Memory Engine.
  * `os.c`: contains the main function that starts the whole OS.
  * `loader.c`: implements the loader.
  * `sched.c`: implements the scheduler.
  * `mem.c`: (obsolete) implements the previous RAM and virtual memory design.
  * `mm.c`, `mm64.c`, `mm-vm.c`, `mm-memphy.c`: implement paging-based memory management.
  * `libmem.c`, `libstd.c`: implement the standard library.
  * `syscall.c`, `syscall.tbl`, `syscalltbl.sh`, `sys_xxx.c`: implement system calls.

* **Makefile**
  * `input`: the folder contains a set of inputs used for verification.
  * `output`: sample outputs of the system.

## 1.3 Processes

We are going to build a multitasking OS that lets multiple processes run concurrently. The OS manages processes through their PCB, described as follows:

```c
// From include/common.h
struct pcb_t {
    uint32_t pid;
    uint32_t priority;
    char path[100];
    uint32_t code_seg_t * code;
    addr_t regs[10];
    uint32_t pc;
#ifdef MLQ_SCHED
    uint32_t prio;
#endif
    struct page_table_t page_table;
    uint32_t bp;
};
```

The meaning of fields in the struct:

* **PID:** the process ID.
* **priority:** process priority; the lower the value, the higher the priority. This legacy priority depends on the process's properties and remains fixed throughout execution.
* **code:** Text segment of the process.
* **regs:** Registers, each process could use up to 10 registers numbered from 0 to 9.
* **pc:** the current value of the program counter.
* **page_table:** the translation from virtual addresses to physical addresses (obsolete, do not use).
* **bp:** the break pointer, used to manage the heap segment.
* **prio:** Priority on execution (if supported), and this value overwrites the default priority.

Similar to a real process, each process in this simulation is just a list of instructions executed by the CPU one by one from beginning to end. There are five instructions that a process can perform:

* **CALC:** performs some calculation using the CPU. This instruction does not take any arguments.
* **ALLOC:** Allocate some chunk of bytes on the main memory (RAM). Instruction's syntax: `alloc [size] [reg]`
* **KMALLOC:** Allocate some chunk of bytes on the kernel memory space. Instruction's syntax: `kmalloc [size] [reg]`
* **KMEM_CACHE_CREATE:** Allocate some chunk of bytes in the kernel memory space to create a cache pool. Instruction's syntax: `kmem_cache_create [size] [align] [cache_pool_id]`
* **KMEM_CACHE_ALLOC:** Allocate some chunk of bytes in the given cache pool in kernel memory space. Instruction's syntax: `kmem_cache_alloc [reg] [cache_pool_id]`
* **FREE:** Free allocated memory. Instruction's syntax: `free [reg]`
* **READ (userspace only):** Read a byte from memory and can only access userspace address. Instruction's syntax: `read [source] [offset] [destination]`
* **WRITE (userspace only):** Write a value register to memory and can only access userspace address. Instruction's syntax: `write [data] [destination] [offset]`
* **COPY_FROM_USER:** do a direct read from the userspace address and write to the kernelspace address. Instruction's syntax: `copy_from_user [source] [destination] [offset] [size]`
* **COPY_TO_USER:** do a direct read from the kernelspace address and write to the userspace address. Instruction's syntax: `copy_to_user [source] [destination] [offset] [size]`

## 1.4 How to Create a Process?

To create a process, we must first generate the program which describes its content. A program is defined by a single file with the following format:

```text
[priority]
[N = number of instructions]
instruction 0
instruction 1
...
instruction N-1
```

The system employs a dual priority mechanism. The higher priority (with the smaller value) the process has, the process has higher chance to be picked up by the CPU from the queue.

## 1.5 How to Run the Simulation

To start the simulation process, we must create a description file in the input directory. The description file is defined in the following format:

```text
[time slice] [N = Number of CPU] [M = Number of Processes to be run]
[time 0] [path 0] [priority 0]
[time 1] [path 1] [priority 1]
...
[time M-1] [path M-1] [priority M-1]
```

To start the simulation, compile using `make all`. After that, run the command:
`./os [configure_file]`

## 1.6 How to Write the Kernel Interface

### 1.6.1 The kernel structure

The struct `krnl_t` encapsulates key components of the operating system kernel for managing process scheduling and memory subsystem.

```c
// From include/common.h
struct krnl_t {
    struct queue_t *ready_queue;
    struct queue_t running_list;
#ifdef MLQ_SCHED
    struct queue_t mlq_ready_queue;
#endif
#ifdef MM_PAGING
    struct mm_struct *mm;
    struct memphy_struct *mram;
    struct memphy_struct **mswp;
    struct memphy_struct *active_mswp;
    uint32_t active_mswp_id;
#endif
};
```

### 1.6.2 The system call

System calls are generally not invoked directly, but rather through wrapper functions in libstd.

* `listsyscall`: lists all system calls.
* `memmap`: maps memory. Supports various operations including `SYSMEM_MAP_OP`, `SYSMEM_INC_OP`, `SYSMEM_SWP_OP`, `SYSMEM_IO_READ`, `SYSMEM_IO_WRITE`.

---

# 2 Implementation

## 2.1 Scheduler

The OS uses multiple queues called ready queue to determine which process to be executed when a CPU becomes available. Each queue is associated with a fixed priority value. The scheduler is designed based on the "multilevel queue" algorithm used in the Linux kernel.
We implement the Multi-Level Queue (MLQ) policy. The system contains `MAX_PRIO` priority levels.

Your job in this part is to implement this algorithm by completing the following functions:

* `enqueue()` and `dequeue()` (in `queue.c`)
* `get_proc()` (in `sched.c`)

## 2.2 Memory Management

### 2.2.1 The process memory layout

The virtual memory space is organized as a memory mapping for each process PCB.

* **Memory region struct:** `struct vm_rg_struct`
* **Memory area struct:** `struct vm_area_struct`
* **Memory mapping struct:** `struct mm_struct`

**Table 1: The 64-bit address layout**

| Start addr (hex) | Offset | End addr (hex) | Size | VM area description |
|---|---|---|---|---|
| 0x0000000000000000 | 0 | 0x00ffffffffffffff | 64 PB | user-space virtual memory |
| 0xff11000000000000 | -59.75 PB | 0xff90ffffffffffff | 32 PB | direct mapping of all physical memory |
| 0xffa0000000000000 | -24 PB | 0xffd1ffffffffffff | 12.5 PB | vmalloc/ioremap space |
| ffffffff80000000 | -2 GB | ffffffff9fffffff | 512 MB | kernel text mapping |

### 2.2.3 The system's physical memory

There are two types of devices, RAM and SWAP. In our settings, we support hardware configured with one RAM device and up to 4 SWAP devices.

### 2.2.4 Paging-based address translation scheme

**Table 2: Various CPU address bus configurations**

| CPU bus | PAGE size | PAGE bit | No pg entry | PAGE Entry sz | PAGE TBL | OFFSET bit | PGT mem | MEMPHY | fram bit |
|---|---|---|---|---|---|---|---|---|---|
| 20 | 256B | 12 | ~4000 | 4byte | 16KB | 8 | 2MB | 1MB | 12 |
| 22 | 256B | 14 | ~16000 | 4byte | 64KB | 8 | 8MB | 1MB | 12 |
| 22 | 512B | 13 | ~8000 | 4byte | 32KB | 9 | 4MB | 1MB | 11 |

**Table 3: CPU 64-bit scheme**

| Bit | 63-57 | 56-48 | 47-39 | 38-30 | 29-21 | 20-12 | 11-0 |
|---|---|---|---|---|---|---|---|
| Paging level | | L5 | L4 | L3 | L2 | L1 | OFFSET |
| Page directories | | PGD | P4D | PUD | PMD | PT | OFFSET |

## 2.3 Put It All Together

Finally, we combine the scheduler and memory management to form a complete OS. Since the OS runs on multiple processors, shared resources may be accessed concurrently by more than one process at a time. Your job in this section is to identify shared resources and use locking mechanisms to protect them.

---

# 3 Submission

## 3.1 Source code

Requirement: You have to code the system calls following the coding style.

## 3.2 Requirements

* **Scheduler:** Implement the scheduler that employs the MLQ policy.
* **Memory Management:** Implement the paging subsystem with separable user/kernel space.
* **Multi-level paging:** Implement the long-address 64-bit scheme.
* **vmap_pgd_memset:** Emulate page-directory behavior and skip real memory allocation.

## 3.3 Report

Write a report that answers questions in the implementation section and interprets the results:

* Scheduling: draw a Gantt diagram.
* Memory management: show the status of memory allocation.
* Multi-level paging: show the multi-level paging address translation scheme.

## 3.4 Grading

The overall grade for your group is determined by the following components:

* Demonstration (7 points)
  * Scheduling: 3 points
  * MMU and user/kernel spaces: 2 points
  * Multi-level Paging: 2 points
* Report (3 points)

## 3.5 Code of ethics

Source Code License Grant: The author(s) hereby grant the Licensee personal permission to use and modify the Licensed Source Code for the sole purpose of studying while attending course CO2018 at HCMUT.
