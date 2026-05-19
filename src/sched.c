/*
 * Copyright (C) 2026 pdnguyen of HCMC University of Technology VNU-HCM
 */

/* LamiaAtrium release
 * Source Code License Grant: The authors hereby grant to Licensee
 * personal permission to use and modify the Licensed Source Code
 * for the sole purpose of studying while attending the course CO2018.
 */

#include "queue.h"
#include "sched.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

static struct queue_t ready_queue;
static struct queue_t run_queue;
pthread_mutex_t queue_lock;

static struct queue_t running_list;
#ifdef MLQ_SCHED
static struct queue_t mlq_ready_queue[MAX_PRIO];
static int slot[MAX_PRIO];
#endif

int queue_empty(void) {
  int is_empty = 1;
  pthread_mutex_lock(&queue_lock);
  
#ifdef MLQ_SCHED
  for (unsigned long prio = 0; prio < MAX_PRIO; prio++) {
    if (!empty(&mlq_ready_queue[prio])) {
      is_empty = 0; // FIX: Trả về 0 (không rỗng) thay vì -1
      break;
    }
  }
#else
  is_empty = (empty(&ready_queue) && empty(&run_queue));
#endif

  pthread_mutex_unlock(&queue_lock);
  return is_empty;
}

void init_scheduler(void) {
#ifdef MLQ_SCHED
  int i;
  for (i = 0; i < MAX_PRIO; i++) {
    mlq_ready_queue[i].size = 0;
    slot[i] = MAX_PRIO - i; 
  }
#endif
  ready_queue.size = 0;
  run_queue.size = 0;
  running_list.size = 0;
  pthread_mutex_init(&queue_lock, NULL);
}

#ifdef MLQ_SCHED
/* * Stateful design for routine calling
 * based on the priority and our MLQ policy
 * We implement stateful here using transition technique
 * State representation   prio = 0 .. MAX_PRIO, curr_slot = 0..(MAX_PRIO - prio)
 */
struct pcb_t *get_mlq_proc(void) {
  struct pcb_t *proc = NULL;

  pthread_mutex_lock(&queue_lock);
  /* Find queue - slot > 0 and !null */
  for (int i = 0; i < MAX_PRIO; i++) {
    if (!empty(&mlq_ready_queue[i]) && slot[i] > 0) {
      proc = dequeue(&mlq_ready_queue[i]);
      slot[i]--;
      goto found;
    }
  }
  
  /* !found -> reset slot and loop again */
  for (int i = 0; i < MAX_PRIO; i++)
    slot[i] = MAX_PRIO - i;

  for (int i = 0; i < MAX_PRIO; i++) {
    if (!empty(&mlq_ready_queue[i]) && slot[i] > 0) {
      proc = dequeue(&mlq_ready_queue[i]);
      slot[i]--;
      goto found;
    }
  }
  
found:
  if (proc != NULL)
    enqueue(&running_list, proc);

  pthread_mutex_unlock(&queue_lock);
  return proc;
}

void put_mlq_proc(struct pcb_t *proc) {
  pthread_mutex_lock(&queue_lock);
  purgequeue(&running_list, proc);
  int safe_prio = (proc->prio >= MAX_PRIO) ? (MAX_PRIO - 1) : proc->prio;
  enqueue(&mlq_ready_queue[safe_prio], proc);
  pthread_mutex_unlock(&queue_lock);
}

void add_mlq_proc(struct pcb_t *proc) {
  pthread_mutex_lock(&queue_lock);
  int safe_prio = (proc->prio >= MAX_PRIO) ? (MAX_PRIO - 1) : proc->prio;
  enqueue(&mlq_ready_queue[safe_prio], proc);
  pthread_mutex_unlock(&queue_lock);
}

struct pcb_t *get_proc(void) {
  return get_mlq_proc();
}

void put_proc(struct pcb_t *proc) {
  put_mlq_proc(proc);
}

void add_proc(struct pcb_t * proc) {
  add_mlq_proc(proc);
}

#else

struct pcb_t *get_proc(void) {
  struct pcb_t *proc = NULL;

  pthread_mutex_lock(&queue_lock);

  if (empty(&ready_queue) && !empty(&run_queue)) {
    while (!empty(&run_queue)) {
      enqueue(&ready_queue, dequeue(&run_queue));
    }
  }
  if (!empty(&ready_queue)) {
    proc = dequeue(&ready_queue);
    if (proc != NULL) 
      enqueue(&running_list, proc);
  }

  pthread_mutex_unlock(&queue_lock);
  return proc;
}

void put_proc(struct pcb_t *proc) {
  pthread_mutex_lock(&queue_lock);
  purgequeue(&running_list, proc);
  enqueue(&run_queue, proc);
  pthread_mutex_unlock(&queue_lock);
}

void add_proc(struct pcb_t *proc) {
  pthread_mutex_lock(&queue_lock);
  enqueue(&ready_queue, proc);
  pthread_mutex_unlock(&queue_lock);
}
#endif

/* Get prod by PID, use kernel -> looking up process */
struct pcb_t *get_proc_by_pid(uint32_t pid) {
  struct pcb_t *proc = NULL;
  pthread_mutex_lock(&queue_lock);

  /* Tìm trong running_list trước */
  for (int i = 0; i < running_list.size; i++) {
    if (running_list.proc[i] != NULL &&
        running_list.proc[i]->pid == pid) {
      proc = running_list.proc[i];
      goto done;
    }
  }

  /* Tìm trong ready_queue (non-MLQ) */
  for (int i = 0; i < ready_queue.size; i++) {
    if (ready_queue.proc[i] != NULL &&
        ready_queue.proc[i]->pid == pid) {
      proc = ready_queue.proc[i];
      goto done;
    }
  }

#ifdef MLQ_SCHED
  /* Tìm trong tất cả mlq_ready_queue */
  for (int prio = 0; prio < MAX_PRIO; prio++) {
    for (int i = 0; i < mlq_ready_queue[prio].size; i++) {
      if (mlq_ready_queue[prio].proc[i] != NULL &&
          mlq_ready_queue[prio].proc[i]->pid == pid) {
        proc = mlq_ready_queue[prio].proc[i];
        goto done;
      }
    }
  }
#else
  /* Tìm trong run_queue (non-MLQ) */
  for (int i = 0; i < run_queue.size; i++) {
    if (run_queue.proc[i] != NULL &&
        run_queue.proc[i]->pid == pid) {
      proc = run_queue.proc[i];
      goto done;
    }
  }
#endif

done:
  pthread_mutex_unlock(&queue_lock);
  return proc;
}