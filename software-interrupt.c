/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <metal/cpu.h>
#include <metal/timer.h>
#include <metal/interrupt.h>

#ifndef METAL_WAIT_CYCLE
#define METAL_WAIT_CYCLE 5000
#endif

int sw_status = 99;

void metal_riscv_cpu_intc_msip_handler() {
    sw_status = 0;

    struct metal_cpu cpu = metal_cpu_get(metal_cpu_get_current_hartid());
    metal_cpu_clear_ipi(cpu);
}

int main (void)
{
    struct metal_cpu cpu = metal_cpu_get(metal_cpu_get_current_hartid());

    metal_cpu_enable_ipi();
    metal_cpu_enable_interrupts();

    metal_cpu_set_ipi(cpu);

    unsigned long long start;
    unsigned long long count;
    metal_timer_get_cyclecount(metal_cpu_get_current_hartid(), &start);
    do {
        metal_timer_get_cyclecount(metal_cpu_get_current_hartid(), &count);
    } while (count < (start + METAL_WAIT_CYCLE));

    return sw_status;
}
