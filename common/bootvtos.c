/*
 * (C) Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef USE_HOSTCC
#include <common.h>
#include <bootstage.h>
#include <bzlib.h>
#include <errno.h>
#include <fdt_support.h>
#include <lmb.h>
#include <malloc.h>
#include <mapmem.h>
#include <asm/io.h>
#include <linux/lzo.h>
#include <lzma/LzmaTypes.h>
#include <lzma/LzmaDec.h>
#include <lzma/LzmaTools.h>
#if defined(CONFIG_CMD_USB)
#include <usb.h>
#endif
#else
#include "mkimage.h"
#endif

#include <command.h>
#include <bootm.h>
#include <bootvtos.h>
#include <image.h>

#ifdef CONFIG_ARM64
static void do_nonsec_virt_switch(void)
{
	smp_kick_all_cpus();
	dcache_disable();	/* flush cache before swtiching to EL2 */
	armv8_switch_to_el2();
#ifdef CONFIG_ARMV8_SWITCH_TO_EL1
	armv8_switch_to_el1();
#endif
}
#endif

/*
 * announce_and_cleanup() - Print message and prepare for kernel boot
 */
static void announce_and_cleanup(void)
{
	puts("\nStarting kernel ...\n\n");
#ifdef CONFIG_USB_DEVICE
	udc_disconnect();
#endif
	cleanup_before_vtos();
}

static void boot_jump_vtos(char *addr)
{
	void (*kernel_entry)(void);
	ulong ep = simple_strtoul(addr, NULL, 16);
	kernel_entry = (void (*)(void))ep;
	printf("## Transferring control to vtos (at address 0x%lx)...\n",
		(ulong) kernel_entry);
	announce_and_cleanup();
	do_nonsec_virt_switch();
	kernel_entry();
}

int boot_vtos(char *addr)
{
	ulong iflag = 0;
	iflag = bootm_disable_interrupts();
	/* Now run the OS! We hope this doesn't return */
	boot_jump_vtos(addr);

	puts("\n## Control returned to monitor - resetting...\n");

	/* Deal with any fallout */
	if (iflag)
	{
		enable_interrupts();
	}
	do_reset(NULL, 0, 0, NULL);
	return 1;
}
