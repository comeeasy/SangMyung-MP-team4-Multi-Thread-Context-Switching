// ======================================================================

#include "frdm_k64f.h"          // include for FRDM-K64F board
#include "threads.h"
#include "handlers.h"

// ======================================================================

unsigned int tick = 0;

// ======================================================================
// PendSV_Handler performs thread context switching using two
// variables tcb_current and tcb_next. The two variables should be
// well prepared before the handler entry. The variable tcb_current
// is updated during the context switching.

void PendSV_Handler(void)
{
  // Set r0=&tcb_current, r1=tcb_current and r2=tcb_next.
  asm volatile ("ldr    r0, =tcb_next");
  asm volatile ("ldr    r2, [r0]");
  asm volatile ("ldr    r0, =tcb_current");
  asm volatile ("ldr    r1, [r0]");

  // Store r4-r11 into the stack of the current thread and update
  // tcb_current->sp.
  asm volatile ("mrs    r3, psp");
  asm volatile ("stmfd  r3!, {r4-r11}");
  asm volatile ("str    r3, [r1]");

  // The variable tcb_current is now safely updated with tcb_next.
  asm volatile ("str    r2, [r0]");

  // Load r4-r11 from the stack of the next thread stack and update
  // psp.
  asm volatile ("ldr    r3, [r2]");
  asm volatile ("ldmfd  r3!, {r4-r11}");
  asm volatile ("msr    psp, r3");
}

// ======================================================================

void SysTick_Handler(void)
{
  // Increment tick.
  tick += 1;

  /* sleep중인 thread의 sleep_tick을 1씩 감소 */
  if(tcb_array[1].sleep_tick) tcb_array[1].sleep_tick--;
  if(tcb_array[2].sleep_tick) tcb_array[2].sleep_tick--;
  if(tcb_array[3].sleep_tick) tcb_array[3].sleep_tick--;
  if(tcb_array[4].sleep_tick) tcb_array[4].sleep_tick--;
  if(tcb_array[5].sleep_tick) tcb_array[5].sleep_tick--;
  
  /* 현재 thread의 상태가 WAIT이었다면 그대로 WAIT */
  /* STATE_RUN이었다면 STATE_READY */
  if(tcb_current->state == STATE_RUN)
    tcb_current->state = STATE_READY;

  /* sleep중인 thread는 next thread 선정 시 제외 */
  while(tcb_array[(tid_current+1)%7].state != STATE_READY) {
    tid_current = (tid_current+1) % 7;
  }
  /* 다음 thread의 state가 READY이므로 한 칸 더 진행 */
  tid_current = (tid_current + 1) % 7;
  
  /* 다음 tcb의 state를 run 상태로 만듬 */
  tcb_array[tid_current].state = STATE_RUN;

  // Update tcb_next for PendSV handler.
  tcb_next = &tcb_array[tid_current];

  // Make PendSV exception pending.
  /* ICSR의 bit[28]을 set하면 된다. */
  *(volatile unsigned int *) SCB_ICSR |= 0x10000000;
}

/* origin */
void SysTick_Handler(void)
{
  // Increment tick.
  tick = tick + 1;

  /* 현재 thread는 잠깐 대기하고 다음 thread가 current thread가 되게 함 */
  tcb_array[tid_current].state = STATE_READY;

  /* 0, 1, 2, 3, 4, 5 가 반복되도록 작성 */
  tid_current = (tid_current + 1) % 7;

  /* 다음 tcb의 state를 run 상태로 만듬 */
  tcb_array[tid_current].state = STATE_RUN;

  // Update tcb_next for PendSV handler.
  tcb_next = &tcb_array[tid_current];

  // Make PendSV exception pending.
  /* ICSR의 bit[28] 을 set하면 된다.*/
  *(volatile unsigned int *) SCB_ICSR |= 0x10000000;
}

// ======================================================================