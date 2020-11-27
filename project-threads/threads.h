// ======================================================================

#ifndef _THREADS_H_
#define _THREADS_H_

// ======================================================================

#define NO_OF_THREADS           6
#define SIZE_OF_STACK           128

#define STATE_RUN               0
#define STATE_READY             1

// ======================================================================
// The thread control block.

typedef struct tcb_struct {
  unsigned int sp;              // thread stack pointer
  unsigned int function;        // thread function
  unsigned int state;           // thread state (RUN, READY, ...)
} TCB;

// ======================================================================

extern TCB tcb_array[];         // the tcb array

extern int tid_current;         // current thread's id

extern TCB *tcb_current;        // current thread's tcb
extern TCB *tcb_next;           // next thread's tcb

// ======================================================================

extern void create_thread(TCB * tcb, void (*function) (void), unsigned int *sp);
extern void SysTick_init(int hz);

// ======================================================================
// Thread stacks and functions.

extern unsigned int thread0_stack[];
extern unsigned int thread1_stack[];
extern unsigned int thread2_stack[];
extern unsigned int thread3_stack[];
extern unsigned int thread4_stack[];
extern unsigned int thread5_stack[];

extern void thread0_function(void);
extern void thread1_function(void);
extern void thread2_function(void);
extern void thread3_function(void);
extern void thread4_function(void);
extern void thread5_function(void);


#endif

// ======================================================================