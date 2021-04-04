/*
 * Ingenic mensa configuration
 *
 * Copyright (c) 2013 Ingenic Semiconductor Co.,Ltd
 * Author: Zoro <ykli@ingenic.cn>
 * Based on: include/configs/urboard.h
 *           Written by Paul Burton <paul.burton@imgtec.com>
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

#ifndef __CONFIG_ZX10_H__
#define __CONFIG_ZX10_H__

/**
 * Basic configuration(SOC, Cache, UART, DDR).
 */
#define CONFIG_MIPS32		/* MIPS32 CPU core */
#define CONFIG_CPU_XBURST
#define CONFIG_SYS_LITTLE_ENDIAN
#define CONFIG_X1000

#define CONFIG_SYS_APLL_FREQ		1008000000	/*If APLL not use mast be set 0*/
#define CONFIG_SYS_MPLL_FREQ		600000000	/*If MPLL not use mast be set 0*/
#define CONFIG_CPU_SEL_PLL		APLL
#define CONFIG_DDR_SEL_PLL		MPLL
#define CONFIG_SYS_CPU_FREQ		1008000000
#define CONFIG_SYS_MEM_FREQ		200000000

#define CONFIG_SYS_EXTAL		24000000	/* EXTAL freq: 24 MHz */
#define CONFIG_SYS_HZ			1000 /* incrementer freq */


#define CONFIG_SYS_DCACHE_SIZE		16384
#define CONFIG_SYS_ICACHE_SIZE		16384
#define CONFIG_SYS_CACHELINE_SIZE	32

#define CONFIG_SYS_UART_INDEX		2
#define CONFIG_BAUDRATE			115200

#define CONFIG_UART2_PC

#define CONFIG_DDR_PARAMS_CREATOR
#define CONFIG_DDR_HOST_CC
#define CONFIG_DDR_TYPE_LPDDR
#define CONFIG_DDR_CS0			1	/* 1-connected, 0-disconnected */
#define CONFIG_DDR_CS1			0	/* 1-connected, 0-disconnected */
#define CONFIG_DDR_DW32			0	/* 1-32bit-width, 0-16bit-width */
#define CONFIG_MDDR_ECM220ACBCN_50

#define CONFIG_AUDIO_CAL_DIV
#define CONFIG_AUDIO_APLL CONFIG_SYS_APLL_FREQ
#define CONFIG_AUDIO_MPLL CONFIG_SYS_MPLL_FREQ

#if defined(CONFIG_SPL_SFC_NOR)
#define CONFIG_SPL_SFC_SUPPORT
#endif

#if defined(CONFIG_SPL_JZMMC_SUPPORT)
#define FIX_MBR
#endif

#define CONFIG_LZMA
/**
 * Boot arguments definitions.
 */
#define BOOTARGS_COMMON "console=ttyS2,115200n8 consoleblank=0 mem=256M@0x0 "

#if defined(CONFIG_SPL_SFC_SUPPORT)
	#if defined(CONFIG_SPL_SFC_NOR)
		#define CONFIG_BOOTARGS BOOTARGS_COMMON "ip=off rootfstype=squashfs,jffs2 root=/dev/mtdblock4 rw init=/sbin/init"
		#define CONFIG_BOOTCOMMAND "sfcnor read 0x40000 0x200000 0x80800000 ;bootm 0x80800000"
	#endif
#elif defined(CONFIG_SPL_JZMMC_SUPPORT)
	#define CONFIG_BOOTARGS BOOTARGS_COMMON " root=/dev/mmcblk0p1 rootfstype=ext4 rootwait init=/sbin/init"
	#define CONFIG_BOOTCOMMAND "mmc dev 0;mmc read 0x80600000 0x1800 0x3000; bootm 0x80600000"
#endif

/**
 * Boot command definitions.
 */
#define CONFIG_BOOTDELAY 1


/* GPIO */
#define CONFIG_JZ_GPIO

/**
 * Command configuration.
 */
#define CONFIG_CMD_BOOTD	/* bootd			*/
#define CONFIG_CMD_CONSOLE	/* coninfo			*/
#define CONFIG_CMD_ECHO		/* echo arguments		*/
#define CONFIG_CMD_EXT4 	/* ext4 support			*/
#define CONFIG_CMD_FAT		/* FAT support			*/
#define CONFIG_CMD_MEMORY	/* md mm nm mw cp cmp crc base loop mtest */
#define CONFIG_CMD_MISC		/* Misc functions like sleep etc*/
#define CONFIG_CMD_RUN		/* run command in env variable	*/
#define CONFIG_CMD_SAVEENV	/* saveenv			*/
#define CONFIG_CMD_SOURCE	/* "source" command support	*/
#define CONFIG_CMD_GETTIME
#define CONFIG_CMD_UNZIP        /* unzip from memory to memory  */

#ifndef CONFIG_SPL_BUILD
#define CONFIG_USE_ARCH_MEMSET
#define CONFIG_USE_ARCH_MEMCPY
#endif

/* MMC */
#define CONFIG_CMD_MMC

#ifdef CONFIG_CMD_MMC
#define CONFIG_GENERIC_MMC		1
#define CONFIG_MMC			1
#define CONFIG_JZ_MMC			1

#define CONFIG_JZ_MMC_MSC0_PA_4BIT 1
#define CONFIG_MSC_DATA_WIDTH_4BIT
#endif

/**
 * Serial download configuration
 */
#define CONFIG_LOADS_ECHO	1	/* echo on for serial download */

/**
 * Miscellaneous configurable options
 */
#define CONFIG_DOS_PARTITION

#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_SYS_NO_FLASH
#define CONFIG_SYS_FLASH_BASE	0 /* init flash_base as 0 */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_MISC_INIT_R 1

#define CONFIG_BOOTP_MASK	(CONFIG_BOOTP_DEFAUL)

#define CONFIG_SYS_MAXARGS 16
#define CONFIG_SYS_LONGHELP

#if defined(CONFIG_SPL_JZMMC_SUPPORT)
	#define CONFIG_SYS_PROMPT CONFIG_SYS_BOARD "-msc0# "
#elif defined(CONFIG_SPL_SFC_SUPPORT)
	#if defined(CONFIG_SPL_SFC_NOR)
		#define CONFIG_SYS_PROMPT CONFIG_SYS_BOARD "-sfcnor# "
	#endif
#endif

#define CONFIG_SYS_CBSIZE 1024 /* Console I/O Buffer Size */
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)

#if defined(CONFIG_SUPPORT_EMMC_BOOT)
#define CONFIG_SYS_MONITOR_LEN		(384 * 1024)
#else
#define CONFIG_SYS_MONITOR_LEN		(512 << 10)
#endif

#define CONFIG_SYS_MALLOC_LEN		(8 * 1024 * 1024)
#define CONFIG_SYS_BOOTPARAMS_LEN	(128 * 1024)

#define CONFIG_SYS_SDRAM_BASE		0x80000000 /* cached (KSEG0) address */
#define CONFIG_SYS_SDRAM_MAX_TOP	0x90000000 /* don't run into IO space */
#define CONFIG_SYS_INIT_SP_OFFSET	0x400000
#define CONFIG_SYS_LOAD_ADDR		0x88000000
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		0x88000000
#define CONFIG_SYS_TEXT_BASE		0x80100000
#define CONFIG_SYS_MONITOR_BASE		CONFIG_SYS_TEXT_BASE

/**
 * Environment
 */
#if defined(CONFIG_ENV_IS_IN_MMC)
#define CONFIG_SYS_MMC_ENV_DEV		0
#define CONFIG_ENV_SIZE			(32 << 10)
#define CONFIG_ENV_OFFSET		(CONFIG_SYS_MONITOR_LEN + CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR * 512)
#elif defined(CONFIG_ENV_IS_IN_SFC)
#define CONFIG_ENV_SIZE			(64 << 10)
#define CONFIG_ENV_OFFSET		0xff0000
#define CONFIG_CMD_SAVEENV
#else
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_SIZE			(32 << 10)
#endif

/**
 * SPL configuration
 */
#define CONFIG_SPL
#define CONFIG_SPL_FRAMEWORK
#define CONFIG_SPL_NO_CPU_SUPPORT_CODE
#define CONFIG_SPL_START_S_PATH		"$(CPUDIR)/$(SOC)"
#ifdef CONFIG_SPL_NOR_SUPPORT
#define CONFIG_SPL_LDSCRIPT             "$(CPUDIR)/$(SOC)/u-boot-nor-spl.lds"
#else
#define CONFIG_SPL_LDSCRIPT		"$(CPUDIR)/$(SOC)/u-boot-spl.lds"
#endif
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR	0x3A /* 12KB+17K offset */
#define CONFIG_SYS_U_BOOT_MAX_SIZE_SECTORS	0x200 /* 256 KB */
#ifndef CONFIG_SMALLER_SPL
#define CONFIG_SPL_SERIAL_SUPPORT
#endif
#define CONFIG_SPL_GPIO_SUPPORT
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#if defined(CONFIG_SPL_JZMMC_SUPPORT)
#define CONFIG_SPL_PAD_TO		12288  /* spl size */
#define CONFIG_SPL_TEXT_BASE		0xf4001000
#define CONFIG_SPL_MAX_SIZE		(12 * 1024)
#elif defined(CONFIG_SPL_SFC_SUPPORT)
#define CONFIG_UBOOT_OFFSET             (4<<12)
#define CONFIG_JZ_SFC_PA_6BIT
#define CONFIG_SPI_SPL_CHECK
#define CONFIG_SPL_TEXT_BASE		0xf4001000
#define CONFIG_SPL_MAX_SIZE		((16 * 1024) - 0x800)
#define CONFIG_SPL_PAD_TO		16384
#define CONFIG_CMD_SFC_NOR
#endif

#ifdef CONFIG_CMD_SFC_NOR
#define CONFIG_JZ_SFC
#define CONFIG_JZ_SFC_NOR
#define CONFIG_SPI_QUAD
#endif

#ifdef CONFIG_JZ_SFC_NOR
#define CONFIG_SPIFLASH_PART_OFFSET     0x3c00
#define CONFIG_SPI_NORFLASH_PART_OFFSET     0x3c74
#define CONFIG_NOR_MAJOR_VERSION_NUMBER     1
#define CONFIG_NOR_MINOR_VERSION_NUMBER     0
#define CONFIG_NOR_REVERSION_NUMBER     0
#define CONFIG_NOR_VERSION     (CONFIG_NOR_MAJOR_VERSION_NUMBER | (CONFIG_NOR_MINOR_VERSION_NUMBER << 8) | (CONFIG_NOR_REVERSION_NUMBER <<16))
#endif


/**
 * MBR configuration
 */

#define SD_SIZE_4G

#ifdef CONFIG_MBR_CREATOR
#if 0
#define CONFIG_MBR_P0_OFF	64mb
#define CONFIG_MBR_P0_END	556mb
#define CONFIG_MBR_P0_TYPE 	linux

#define CONFIG_MBR_P1_OFF	580mb
#define CONFIG_MBR_P1_END 	1604mb
#define CONFIG_MBR_P1_TYPE 	linux

#define CONFIG_MBR_P2_OFF	28mb
#define CONFIG_MBR_P2_END	58mb
#define CONFIG_MBR_P2_TYPE 	linux

#define CONFIG_MBR_P3_OFF	1609mb
#define CONFIG_MBR_P3_END	7800mb
#define CONFIG_MBR_P3_TYPE 	fat
#endif


#define CONFIG_MBR_P0_OFF	8mb
#define CONFIG_MBR_P0_END	60mb    /* 48 */
#define CONFIG_MBR_P0_TYPE 	linux

#define CONFIG_MBR_P1_OFF	61mb
#define CONFIG_MBR_P1_END 	62mb
#define CONFIG_MBR_P1_TYPE 	linux

#define CONFIG_MBR_P2_OFF	63mb
#define CONFIG_MBR_P2_END	64mb
#define CONFIG_MBR_P2_TYPE 	linux

#define CONFIG_MBR_P3_OFF	70mb
#define CONFIG_MBR_P3_OFF_INT 70

#if defined(SD_SIZE_4G)
#define CONFIG_MBR_P3_END	3688mb /*3488mb*/
#elif defined(SD_SIZE_8G)
#define CONFIG_MBR_P3_END	7800mb
#elif defined(SD_SIZE_16G)
#define CONFIG_MBR_P3_END	14900mb
#elif defined(SD_SIZE_32G)
#define CONFIG_MBR_P3_END	30420mb
#elif defined(SD_SIZE_64G)
#define CONFIG_MBR_P3_END	61100mb
#elif defined(SD_SIZE_128G)
#define CONFIG_MBR_P3_END	122480mb
#endif

#define CONFIG_MBR_P3_TYPE 	fat


#else
#define CONFIG_GPT_TABLE_PATH	"$(TOPDIR)/board/$(BOARDDIR)"
#endif

#endif /* __CONFIG_ZX10_H__ */
