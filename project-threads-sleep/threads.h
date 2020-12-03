// ======================================================================

#ifndef _THREADS_H_
#define _THREADS_H_

// ======================================================================

#define NO_OF_THREADS           7
#define SIZE_OF_STACK           128

#define STATE_RUN               0
#define STATE_READY             1

/* thread 함수가 call할 수 있는 이 함수는 현재 thread를 인수로 주어진 ticks만큼 */
/* processor를 수행하지 않고 기다리게함 (WAIT 상태 추가 필요)                  */
#define WAIT                    2

// ======================================================================
// The thread control block.

typedef struct tcb_struct {
  unsigned int sp;              // thread stack pointer
  unsigned int function;        // thread function
  unsigned int state;           // thread state (RUN, READY, ...)

  /* TCB에 sleep tick 저장 변수 추가 */
  unsigned int sleep_tick;
} TCB;

// ======================================================================

extern TCB tcb_array[];         // the tcb array
extern int tid_current;         // current thread's id
extern TCB *tcb_current;        // current thread's tcb
extern TCB *tcb_next;           // next thread's tcb

// ======================================================================

extern void create_thread(TCB * tcb, void (*function) (void), unsigned int *sp);
extern void SysTick_init(int hz);

/* 함수void sleep_thread(unsigned int ticks) 추가 */
extern void sleep_thread(unsigned int ticks);

// ======================================================================
// Thread stacks and functions.

extern unsigned int thread0_stack[];
extern unsigned int thread1_stack[];
extern unsigned int thread2_stack[];
extern unsigned int thread3_stack[];
extern unsigned int thread4_stack[];
extern unsigned int thread5_stack[];
extern unsigned int thread6_stack[];

extern void thread0_function(void);
extern void thread1_function(void);
extern void thread2_function(void);
extern void thread3_function(void);
extern void thread4_function(void);
extern void thread5_function(void);
extern void thread6_function(void);


#endif

// ======================================================================