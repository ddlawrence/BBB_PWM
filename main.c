//
// BeagleboneBlack PWM Demo Program
//
// mixed C & assembly 
// demonstrates PWM operation in poll mode
// also GPIO under interrupt control (BOOT button & LED USR2)
// requires an RC-servo, get a sub-miniture one 
// do NOT power it from the BBB!  you can splice into the barrel jack
// built with GNU tools :) on platform Win32 :(
//
// BBB-PWM driver hacked from Rodrigo Fagundez, e2e.ti.com
//
// provide feedback at www.baremetal.tech
//
// pwm test application - 50Hz varying duty cycle for RC-servo control
//                      - RC-servo spec: a pulse every 20 msec, 
//                        pulse width between 1 and 2 msec
//
// use ehrpwm0A (pin P9_31),  ehrpwm0B (pin P9_29) 
//     ehrpwm1A (pin P9_14),  ehrpwm1B (pin P9_16) 
//     ehrpwm2A (pin P8_19),  ehrpwm2B (pin P8_13)
//
// TODO implement ECAP module for 2 more channels 
//
#include <stdio.h>

#include "main.h"

// PWM_PRESCALE of 224 = 14 * X  (X=1,2,4...128), see TBCTL Register, spruh73l 15.2.4.1
#define PWM_PRESCALE              224          // pwm clk divider (14*16) ~TBCLK
// TICKS_PER_MS * PWM_PERIOD_MS = period of pwm output in ticks = TBPRD (16 bits wide)
#define TICKS_PER_MS              446          // ticks per msec pwm output period
#define PWM_PERIOD_MS             20           // pwm output period in ms, which = 50Hz

void delay(volatile uint32_t count) {
  while(count--);
}

inline void servo_1(uint32_t tbprd) {
  pwm_write_A(SOC_EPWM_0_REGS, tbprd);
}
inline void servo_2(uint32_t tbprd) {
  pwm_write_B(SOC_EPWM_0_REGS, tbprd);
}
inline void servo_3(uint32_t tbprd) {
  pwm_write_A(SOC_EPWM_1_REGS, tbprd);
}
inline void servo_4(uint32_t tbprd) {
  pwm_write_B(SOC_EPWM_1_REGS, tbprd);
}
inline void servo_5(uint32_t tbprd) {
  pwm_write_A(SOC_EPWM_2_REGS, tbprd);
}
inline void servo_6(uint32_t tbprd) {
  pwm_write_B(SOC_EPWM_2_REGS, tbprd);
}

uint32_t main() {
  uint32_t i=0, pins;
  uint32_t delta, base, direction, tbprd;

  pins = 0xf << 21;  // enab all USR LEDs, pins 21-24
  gpio_init(SOC_GPIO_1_REGS, pins);

  pinmux(GPIO_0_22, 4);  // pin P8_19  ehrpwm2A
  pinmux(GPIO_0_23, 4);  // pin P8_13  ehrpwm2B
  pinmux(GPIO_1_18, 6);  // pin P9_14  ehrpwm1A
  pinmux(GPIO_1_19, 6);  // pin P9_16  ehrpwm1B
  pinmux(GPIO_3_14, 1);  // pin P9_31  ehrpwm0A
  pinmux(GPIO_3_15, 1);  // pin P9_29  ehrpwm0B

  pwm_clk_init(SOC_PWMSS0_REGS);
  pwm_clk_init(SOC_PWMSS1_REGS);
  pwm_clk_init(SOC_PWMSS2_REGS);

  pwm_init(SOC_PWMSS0_REGS);
  pwm_init(SOC_PWMSS1_REGS);
  pwm_init(SOC_PWMSS2_REGS);

  irq_init();

  delay(0x500000);  // kill 6 msec

  base = (TICKS_PER_MS * PWM_PERIOD_MS)/20; // 1 msec base pulse width - RC Servo
  delta = base/10;                          // .1 msec pulse width increment
  i = 4;
  direction = 1;
  while (1) {
    if(direction == 1) i++;
    if(direction == 0) i--;
    tbprd = base + (delta * i);
    servo_1(tbprd);
    servo_2(tbprd);
    servo_3(tbprd);
    servo_4(tbprd);
    servo_5(tbprd);
    servo_6(tbprd);
    if(i <= 0) direction = 1;
    if(i >= 10) direction = 0;
    gpio_on(SOC_GPIO_1_REGS, 0x200000);
    delay(0x20000);
    gpio_off(SOC_GPIO_1_REGS, 0x200000);
    delay(0x400000);
  }
}
