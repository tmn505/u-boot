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

#define CONFIG_LZMA

/**
 * Boot arguments & command definitions.
 */
#define BOOTARGS_COMMON "mem=256M@0x0 "

#define CONFIG_BOOTARGS BOOTARGS_COMMON
#define CONFIG_EXTRA_ENV_SETTINGS \
	"kernel_addr_r=" __stringify(CONFIG_SYS_LOAD_ADDR) "\0" \
	"boot_prefixes=/ /boot/\0" \
	"boot_targets=msc sfcnor\0" \
	"bootcmd_msc=" \
		"setenv devtype mmc; " \
		"setenv devnum 0; " \
		"setenv rootpart 1; " \
		"for prefix in ${boot_prefixes}; " \
			"do if load ${devtype} ${devnum}:${rootpart} ${kernel_addr_r} ${prefix}boot.scr; then " \
				"source ${kernel_addr_r}; " \
			"fi; " \
		"done;\0" \
	"bootcmd_sfcnor=sfcnor read 0x40000 0x800000 ${kernel_addr_r}; bootm ${kernel_addr_r};\0"
#define CONFIG_BOOTCOMMAND "for target in ${boot_targets}; do run bootcmd_${target}; done"
#define CONFIG_BOOTDELAY 1

/* GPIO */
#define CONFIG_JZ_GPIO

/**
 * Command configuration.
 */
#define CONFIG_CMD_BOOTD	/* bootd			*/
#define CONFIG_CMD_CONSOLE	/* coninfo			*/
#define CONFIG_CMD_ECHO		/* echo arguments		*/
#define CONFIG_CMD_FS_GENERIC
#define CONFIG_CMD_PART
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
#define CONFIG_GENERIC_MMC		1
#define CONFIG_MMC			1
#define CONFIG_JZ_MMC			1
#define CONFIG_JZ_MMC_MSC0_PA_4BIT 1
#define CONFIG_MSC_DATA_WIDTH_4BIT

/**
 * Serial download configuration
 */
#define CONFIG_LOADS_ECHO	1	/* echo on for serial download */

/**
 * Miscellaneous configurable options
 */
#define CONFIG_DOS_PARTITION
#define CONFIG_EFI_PARTITION
#define CONFIG_PARTITION_UUIDS
#define CONFIG_FS_EXT4
#define CONFIG_FS_FAT

#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_SYS_NO_FLASH
#define CONFIG_SYS_FLASH_BASE	0 /* init flash_base as 0 */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_ENV_VARS_UBOOT_CONFIG
#define CONFIG_MISC_INIT_R 1

#define CONFIG_BOOTP_MASK	(CONFIG_BOOTP_DEFAUL)

#define CONFIG_SYS_MAXARGS 16
#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT CONFIG_SYS_BOARD "# "

#define CONFIG_SYS_CBSIZE 1024 /* Console I/O Buffer Size */
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)

#define CONFIG_SYS_MONITOR_LEN		(512 << 10)

#define CONFIG_SYS_MALLOC_LEN		(8 * 1024 * 1024)
#define CONFIG_SYS_BOOTPARAMS_LEN	(128 * 1024)
#define CONFIG_SYS_BOOTM_LEN		(32 * 1024 * 1024)

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
#if defined(CONFIG_ENV_IS_IN_SFC)
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
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPL_GPIO_SUPPORT
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_UBOOT_OFFSET             (4<<12)
#define CONFIG_JZ_SFC_PA_6BIT
#define CONFIG_SPI_SPL_CHECK
#define CONFIG_SPL_TEXT_BASE		0xf4001000
#define CONFIG_SPL_MAX_SIZE		((16 * 1024) - 0x800)
#define CONFIG_SPL_PAD_TO		16384

#define CONFIG_CMD_SFC_NOR
#define CONFIG_JZ_SFC
#define CONFIG_JZ_SFC_NOR
#define CONFIG_SPI_QUAD
#define CONFIG_SPIFLASH_PART_OFFSET     0x3c00
#define CONFIG_SPI_NORFLASH_PART_OFFSET     0x3c74
#define CONFIG_NOR_MAJOR_VERSION_NUMBER     1
#define CONFIG_NOR_MINOR_VERSION_NUMBER     0
#define CONFIG_NOR_REVERSION_NUMBER     0
#define CONFIG_NOR_VERSION     (CONFIG_NOR_MAJOR_VERSION_NUMBER | (CONFIG_NOR_MINOR_VERSION_NUMBER << 8) | (CONFIG_NOR_REVERSION_NUMBER <<16))

#endif /* __CONFIG_ZX10_H__ */
