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

  /* register에 extern으로 선언된 변수를 write할 떈 해당 변수의 주소가 저장됨 */
  asm volatile ("ldr    r0, =tcb_next");
  asm volatile ("ldr    r2, [r0]");
  asm volatile ("ldr    r0, =tcb_current");
  asm volatile ("ldr    r1, [r0]");

  // Store r4-r11 into the stack of the current thread and update
  // tcb_current->sp.
  asm volatile ("mrs    r3, psp");

  /* stmfd: */
  /* Operands: Rm{!}, reglist */
  /* Store Multiple registers, increment after */
  /* ! : r3에 psp가 저장됐으므로 하나씩 저장하면서 값이 작아지는데 이를 저장 */
  asm volatile ("stmfd  r3!, {r4-r11}");

  /* 모두 저장된 psp를 tcb_current->sp에 저장 */
  /* tcb_current->sp = psp */
  asm volatile ("str    r3, [r1]");

  // The variable tcb_current is now safely updated with tcb_next.

  /* tcb_current = &tcb_next */
  asm volatile ("str    r2, [r0]");

  // Load r4-r11 from the stack of the next thread stack and update psp.

  /* r3 = tcb_next->sp */
  asm volatile ("ldr    r3, [r2]");
  /* ldmfd: */
  /* Operands: Rn{!}, reglist */
  /* Load Multiple registers, increment after */
  /* ! : r3에 psp가 저장됐으므로 하나씩 저장하면서 값이 작아지는데 이를 저장 */

  /* tcb_next->sp으로 부터 값을 load하고 그 값을 저장 */
  asm volatile ("ldmfd  r3!, {r4-r11}");

  /* psp = tcb_next->sp */
  asm volatile ("msr    psp, r3");
}

// ======================================================================

void SysTick_Handler(void)
{
  // Increment tick.
  tick = tick + 1;

  // Update tid_current and thread state.

  /* 현재 thread는 잠깐 대기하고 다음 thread가 current thread가 되게 함 */
  tcb_array[tid_current].state = STATE_READY;

  /* 0, 1, 2, 3, 4, 5 가 반복되도록 작성 */
  tid_current = (tid_current + 1) % 6;

  /* 다음 tcb의 state를 run 상태로 만듬 */
  tcb_array[tid_current].state = STATE_RUN;

  // Update tcb_next for PendSV handler.
  tcb_next = &tcb_array[tid_current];

  // Make PendSV exception pending.
  /* ICSR의 bit[28] 을 set하면 된다.*/
  *(volatile unsigned int *) SCB_ICSR |= 0x10000000;
}

// ======================================================================