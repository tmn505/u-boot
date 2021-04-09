/*
 * Ingenic mensa setup code
 *
 * Copyright (c) 2013 Ingenic Semiconductor Co.,Ltd
 * Author: Zoro <ykli@ingenic.cn>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/gpio.h>
#include <asm/arch/cpm.h>
#include <asm/arch/clk.h>
#include <asm/arch/mmc.h>
#include <mmc.h>


#define LED_GREEN GPIO_PC(26)

struct cgu_clk_src cgu_clk_src[] = {
	{OTG, EXCLK},
	{LCD, MPLL},
	{MSC, MPLL},
	{SFC, MPLL},
	{CIM, MPLL},
	{PCM, MPLL},
	{I2S, EXCLK},
	{SRC_EOF,SRC_EOF}
};

int board_early_init_f(void)
{
	return 0;
}

int led_init(void)
{
	gpio_request(LED_GREEN, "led-status");
	gpio_direction_output(LED_GREEN, 0);
	gpio_set_value(LED_GREEN, 0);
	udelay(200000);
	gpio_set_value(LED_GREEN, 1);
	udelay(200000);
	gpio_set_value(LED_GREEN, 0);
	udelay(200000);
	gpio_set_value(LED_GREEN, 1);
	udelay(200000);
	gpio_set_value(LED_GREEN, 0);

	return 0;
}

int misc_init_r(void)
{
	led_init();

	return 0;
}

#ifdef CONFIG_MMC
int board_mmc_init(bd_t *bd)
{
	jz_mmc_init();
	return 0;
}
#endif

#ifdef CONFIG_SPL_NOR_SUPPORT
int spl_start_uboot(void)
{
	return 1;
}
#endif

/* U-Boot common routines */
int checkboard(void)
{
	puts("Board: ZoomGo ZX10 (Damai DM6291A SoC)\n");
	printf("DDR Freq: %d\n",CONFIG_SYS_MEM_FREQ);
	return 0;
}

#ifdef CONFIG_SPL_BUILD
void spl_board_init(void)
{
}
#endif /* CONFIG_SPL_BUILD */
