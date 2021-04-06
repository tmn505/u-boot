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

#if defined(CONFIG_SPL_JZMMC_SUPPORT)
enum mmc_state {
	MMC_INVALID,
	MMC_READ,
	MMC_WRITE,
	MMC_ERASE,
};

struct mmc *sd_mmc_init()
{
	int curr_device = -1;
	if (curr_device < 0) {
		if (get_mmc_num() > 0)
			curr_device = 0;
		else {
			puts("No MMC device available\n");
			return 1;
		}
	}
	struct mmc *mmc = find_mmc_device(curr_device);
	mmc_init(mmc);
	return mmc ;
}

int get_data_from_mmc(struct mmc *mmc,u32 blk,u32 cnt)
{
	int curr_device = 0;

	enum mmc_state state;

	state = MMC_READ;
	if (state != MMC_INVALID) {
		
		int idx = 2;
		u32 n;
		void *addr;

		addr = 0x81000000;

		if (!mmc) {
			printf("no mmc device at slot %x\n", curr_device);
			return 1;
		}

		printf("\nMMC read: dev # %d, block # %d, count %d ... ",
				 curr_device, blk, cnt);

		if ((state == MMC_WRITE || state == MMC_ERASE)) {
			if (mmc_getwp(mmc) == 1) {
				printf("Error: card is write protected!\n");
				return 1;
			}
		}

		switch (state) {
		case MMC_READ:
			n = mmc->block_dev.block_read(curr_device, blk,
						      cnt, addr);
			/* flush cache after read */
			flush_cache((ulong)addr, cnt * 512); /* FIXME */
			break;
		case MMC_WRITE:
			n = mmc->block_dev.block_write(curr_device, blk,
						      cnt, addr);
			break;
		case MMC_ERASE:
			n = mmc->block_dev.block_erase(curr_device, blk, cnt);
			break;
		default:
			BUG();
		}

		printf("%d blocks read: %s\n",
				n, (n == cnt) ? "OK" : "ERROR");
		return (n == cnt) ? 0 : 1;
	}
	return 0;
}

int write_data_to_mmc(struct mmc *mmc,u32 blk,u32 cnt)
{
	int curr_device = 0;
	enum mmc_state state;
	
	state = MMC_WRITE;
	if (state != MMC_INVALID) {
		
		int idx = 2;
		u32 n;
		void *addr;

		addr = 0x81000000;

		if (!mmc) {
			printf("no mmc device at slot %x\n", curr_device);
			return 1;
		}

		printf("\nMMC write: dev # %d, block # %d, count %d ... ",
				 curr_device, blk, cnt);

		if ((state == MMC_WRITE || state == MMC_ERASE)) {
			if (mmc_getwp(mmc) == 1) {
				printf("Error: card is write protected!\n");
				return 1;
			}
		}

		switch (state) {
		case MMC_READ:
			n = mmc->block_dev.block_read(curr_device, blk,
						      cnt, addr);
			/* flush cache after read */
			flush_cache((ulong)addr, cnt * 512); /* FIXME */
			break;
		case MMC_WRITE:
			n = mmc->block_dev.block_write(curr_device, blk,
						      cnt, addr);
			break;
		case MMC_ERASE:
			n = mmc->block_dev.block_erase(curr_device, blk, cnt);
			break;
		default:
			BUG();
		}

		printf("%d blocks write: %s\n",
				n, (n == cnt) ? "OK" : "ERROR");
		return (n == cnt) ? 0 : 1;
	}
	return 0;
}

static void print_mmcinfo(struct mmc *mmc)
{
	printf("Device: %s\n", mmc->name);
	printf("Manufacturer ID: %x\n", mmc->cid[0] >> 24);
	printf("OEM: %x\n", (mmc->cid[0] >> 8) & 0xffff);
	printf("Name: %c%c%c%c%c \n", mmc->cid[0] & 0xff,
			(mmc->cid[1] >> 24), (mmc->cid[1] >> 16) & 0xff,
			(mmc->cid[1] >> 8) & 0xff, mmc->cid[1] & 0xff);

	printf("Tran Speed: %d\n", mmc->tran_speed);
	printf("Rd Block Len: %d\n", mmc->read_bl_len);

	printf("%s version %d.%d\n", IS_SD(mmc) ? "SD" : "MMC",
			(mmc->version >> 8) & 0xf, mmc->version & 0xff);

	printf("High Capacity: %s\n", mmc->high_capacity ? "Yes" : "No");
	puts("Capacity: ");
	print_size(mmc->capacity, "\n");

	printf("Bus Width: %d-bit\n", mmc->bus_width);
}


int get_disk_size(struct mmc *mmc,unsigned long *sector)
{
	int curr_device = 0;
	unsigned long all_sector = 0;

	if (mmc) {
		
		puts("MMC info :\n");

		print_mmcinfo(mmc);
		all_sector = (unsigned long)((mmc->capacity) >>9 ) ;
		*sector = all_sector;
		printf("all sectors : %lu\n", all_sector);
		
		return 0;
	} else {
		printf("no mmc device at slot %x\n", curr_device);
		return 1;
	}
}
#endif

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
