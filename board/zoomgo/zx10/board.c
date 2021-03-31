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

int write_ulong_to_memory(int offset,unsigned long p4_sector)
{
	ulong	addr;
	ulong bytes = 512;
	addr = 0x81000000;
	unsigned char *buf = map_sysmem(addr, bytes);
	buf = buf + offset;

	*buf++ = p4_sector & 0xff;
	*buf++ = (p4_sector >> 8) & 0xff;
	*buf++ = (p4_sector >> 16) & 0xff;
	*buf = (p4_sector >> 24) & 0xff;

	unmap_sysmem(buf);
	return 0;
}

int write_uchar_to_memory(int offset,unsigned char dat)
{
	ulong	addr;
	ulong bytes = 512;
	addr = 0x81000000;
	unsigned char *buf = map_sysmem(addr, bytes);
	buf = buf + offset;

	*buf = dat;

	unmap_sysmem(buf);
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

#ifdef FIX_MBR

#define ARRAY_SIZE(x)		((sizeof(x))/(sizeof(x[0])))

#define DBR_HEAD_1		0xEB
#define DBR_HEAD_3		0x90

#define DBR_TAIL_1		0x55
#define DBR_TAIL_2		0xAA

#define P1_FS			0x1C2
#define P1_OFFSET		0x1C6
#define P1_SIZE			0x1Ca

#define P2_FS			0x1D2
#define P2_OFFSET		0x1D6
#define P2_SIZE			0x1Da

#define P3_FS			0x1E2
#define P3_OFFSET		0x1E6
#define P3_SIZE			0x1Ea

#define P4_FS			0x1F2
#define P4_OFFSET		0x1F6
#define P4_SIZE			0x1Fa


struct mbr_tab_item {
	unsigned int	offset;
	unsigned int	size;
	unsigned char	type;
};

struct mbr_head {
	unsigned char head1;
	unsigned char head2;
	unsigned char head3;
};

struct mbr_tail {
	unsigned char tail1;
	unsigned char tail2;
};

int read_mbr_from_memory(struct mbr_tab_item *p_tab_item_mbr, struct mbr_head *p_head_mbr, struct mbr_tail *p_tail_mbr)
{
	int i = 0;
	ulong	addr;
	ulong bytes = 512;
	addr = 0x81000000;
	unsigned char *buf = map_sysmem(addr, bytes);

	if(NULL != p_tab_item_mbr){
		memcpy(&(p_tab_item_mbr)[0].offset,buf+P1_OFFSET,sizeof(unsigned int));
		memcpy(&(p_tab_item_mbr)[1].offset,buf+P2_OFFSET,sizeof(unsigned int));
		memcpy(&(p_tab_item_mbr)[2].offset,buf+P3_OFFSET,sizeof(unsigned int));
		memcpy(&(p_tab_item_mbr)[3].offset,buf+P4_OFFSET,sizeof(unsigned int));
		
		memcpy(&(p_tab_item_mbr)[0].size,buf+P1_SIZE,sizeof(unsigned int));
		memcpy(&(p_tab_item_mbr)[1].size,buf+P2_SIZE,sizeof(unsigned int));
		memcpy(&(p_tab_item_mbr)[2].size,buf+P3_SIZE,sizeof(unsigned int));
		memcpy(&(p_tab_item_mbr)[3].size,buf+P4_SIZE,sizeof(unsigned int));

		memcpy(&(p_tab_item_mbr)[0].type,buf+P1_FS,sizeof(unsigned char));
		memcpy(&(p_tab_item_mbr)[1].type,buf+P2_FS,sizeof(unsigned char));
		memcpy(&(p_tab_item_mbr)[2].type,buf+P3_FS,sizeof(unsigned char));
		memcpy(&(p_tab_item_mbr)[3].type,buf+P4_FS,sizeof(unsigned char));
	#if 0
		printf("partition\ttype\toffset\t\tsize\n");
		for(i=0;i<4;i++){
			printf("%d\t\t%02x\t%08x(%d MB)\t%08x(%d MB)\n",i+1,(p_tab_item_mbr)[i].type,(p_tab_item_mbr)[i].offset,
					((p_tab_item_mbr)[i].offset)/(1024*2),(p_tab_item_mbr)[i].size,((p_tab_item_mbr)[i].size)/(1024*2));

		}
		printf("\n");
	#endif
	}

	if(NULL != p_head_mbr){
		memcpy(&(*p_head_mbr).head1,buf+0,sizeof(unsigned char));
		memcpy(&(*p_head_mbr).head2,buf+1,sizeof(unsigned char));
		memcpy(&(*p_head_mbr).head3,buf+2,sizeof(unsigned char));
	#if 1
		printf("dbr head : ");
			printf("%02x %02x %02x\n", (*p_head_mbr).head1, (*p_head_mbr).head2, (*p_head_mbr).head3);
	#endif
	}

	if(NULL != p_tail_mbr){
		memcpy(&(*p_tail_mbr).tail1,buf+0x1FE,sizeof(unsigned char));
		memcpy(&(*p_tail_mbr).tail2,buf+0x1FF,sizeof(unsigned char));
	#if 1
		printf("dbr tail : ");
			printf("%02x %02x\n", (*p_tail_mbr).tail1, (*p_tail_mbr).tail2);
	#endif
	}

	unmap_sysmem(buf);
	return 0;
}

int fix_mbr(struct mmc *mmc)
{
	int ret = 0;
	struct mbr_tab_item	tab_item_mbr1[4];
	struct mbr_tab_item	tab_item_mbr2[4];
	struct mbr_head dm_head_mbr2;
	struct mbr_tail dm_tail_mbr2;
	struct mbr_head dm_head_dbr;
	struct mbr_tail dm_tail_dbr;
	memset(tab_item_mbr1, 0, ARRAY_SIZE(tab_item_mbr1));
	memset(tab_item_mbr2, 0, ARRAY_SIZE(tab_item_mbr2));
	memset(&dm_head_mbr2, 0, sizeof(struct mbr_head));
	memset(&dm_tail_mbr2, 0, sizeof(struct mbr_tail));
	memset(&dm_head_dbr, 0, sizeof(struct mbr_head));
	memset(&dm_tail_dbr, 0, sizeof(struct mbr_tail));

	//读取MBR1信息
	ret = get_data_from_mmc(mmc,0,1);
	if(ret != 0){
		printf("get data form mmc error\n");
		return -1;
	}
	ret = read_mbr_from_memory(tab_item_mbr1, NULL, NULL);
	if(ret != 0){
		printf("read data form memory error\n");
		return -1;
	}

	//读取第四个分区信息
	ret = get_data_from_mmc(mmc,tab_item_mbr1[3].offset,1);
	if(ret != 0){
		printf("get data form mmc error\n");
		return -1;
	}
	ret = read_mbr_from_memory(tab_item_mbr2, &dm_head_mbr2, &dm_tail_mbr2);
	if(ret != 0){
		printf("read data form memory error\n");
		return -1;
	}

	//判断第四个分区首512字节是否是MBR
	if((dm_head_mbr2.head1 == DBR_HEAD_1) && (dm_head_mbr2.head3 == DBR_HEAD_3) && 
		(dm_tail_mbr2.tail1 == DBR_TAIL_1) && (dm_tail_mbr2.tail2 == DBR_TAIL_2)){
		printf("no seconf mbr\n");
		return 1;
	}
	else if((dm_tail_mbr2.tail1 != DBR_TAIL_1) && (dm_tail_mbr2.tail2 != DBR_TAIL_2)){
		printf("no seconf mbr or dbr\n");
		return 1;
	}else{
		printf("has second mbr\n");
	}

	//如果为MBR，判断分区偏移地址是否为DBR
	unsigned int dbr_offset = tab_item_mbr1[3].offset + tab_item_mbr2[0].offset;
	ret = get_data_from_mmc(mmc,dbr_offset,1);
	if(ret != 0){
		printf("get data form mmc error\n");
		return -1;
	}
	ret = read_mbr_from_memory(NULL, &dm_head_dbr, &dm_tail_dbr);
	if(ret != 0){
		printf("read data form memory error\n");
		return -1;
	}

	//判断是否为DBR
	if((dm_head_dbr.head1 == DBR_HEAD_1) && (dm_head_dbr.head3 == DBR_HEAD_3) && 
		(dm_tail_dbr.tail1 == DBR_TAIL_1) && (dm_tail_dbr.tail2 == DBR_TAIL_2)){
		printf("has dbr\n");
	}
	else{
		printf("no dbr\n");
		return 1;
	}

	//更新MBR信息
	ret = get_data_from_mmc(mmc,0,1);
	if(ret != 0){
		printf("get data form mmc error\n");
		return -1;
	}
	write_uchar_to_memory(P4_FS, tab_item_mbr2[0].type);
	write_ulong_to_memory(P4_OFFSET, dbr_offset);
	write_ulong_to_memory(P4_SIZE, tab_item_mbr2[0].size);
	write_data_to_mmc(mmc,0,1);

	return 0;
}

#endif
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
#ifdef FIX_MBR
	int ret = 0;
	ret = fix_mbr(mmc);

	if(ret == 1)
	{
		printf("no need fix mbr!\n");
	}
	else if(ret == 0){
		printf("fix mbr success!\n");
	}
	else{
		printf("fix mbr fail!\n");
	}
#endif

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
