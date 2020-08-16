/* Copyright 2018 Canaan Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <sysctl.h>
#include "bsp.h"
#include "gpio.h"
#include "fpioa.h"
#include "timer.h"
#include "sysctl.h"

int core1_function(void *ctx)
{
    while(1)
	{
		
	}
}

static int timer_callback(void *ctx)
{
	static uint32_t flag = 0U;
	
	if(flag == 0U)
	{
		gpio_set_pin(5, GPIO_PV_LOW);
		flag = 1U;
	}
	else{
		gpio_set_pin(5, GPIO_PV_HIGH);
		flag = 0U;
	}
	
	return 0;

}

static void setup_timer(uint32_t milliseconds, int repeat)
{
	sysctl_clock_enable(SYSCTL_CLOCK_TIMER0 + TIMER_DEVICE_0);
	
    timer_set_interval(TIMER_DEVICE_0, TIMER_CHANNEL_0, milliseconds * 1000000);
    timer_irq_register(TIMER_DEVICE_0, TIMER_CHANNEL_0, !repeat, 4, timer_callback, NULL);
    timer_set_enable(TIMER_DEVICE_0, TIMER_CHANNEL_0, 1);
    sysctl_enable_irq();
}


int main(void)
{
    sysctl_pll_set_freq(SYSCTL_PLL0, 800000000);
    uint64_t core = current_coreid();
    
    gpio_init();
    fpioa_init();
	setup_timer(500U, 1U);

    fpioa_set_function(13, FUNC_GPIO5);
    gpio_set_drive_mode(5, GPIO_DM_OUTPUT);
    

    printf("Core %ld BlinkAI\n", core);
    register_core1(core1_function, NULL);

    while(1);
	
	
    return 0;
}
