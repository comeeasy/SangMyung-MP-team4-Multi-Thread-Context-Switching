// ======================================================================

#include "frdm_k64f.h"          // include for FRDM-K64F board
#include "threads.h"
#include "handlers.h"

// ======================================================================

unsigned int tick = 0;

// ======================================================================
// PendSV_Handler performs thread context switching using two
// variables tcb_current and tcb_next. The two variables should be
// well prepared before the handler entry. The variable tcbcurrent
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
  asm volatile ("str    r2, [r0]");_

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
  tick = ...

  // Update tid_current and thread state.
  tcb_array[tid_current].state = ...
  tid_current = ...
  tcb_array[tid_current].state = ...

  // Update tcb_next for PendSV handler.
  tcb_next = ...

  // Make PendSV exception pending.
  *(volatile unsigned int *) SCB_ICSR = ...
}

// ======================================================================