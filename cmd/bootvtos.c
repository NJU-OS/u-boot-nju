
/*
 * Boot support
 */
#include <common.h>
#include <bootm.h>
#include <bootvtos.h>
#include <command.h>
#include <environment.h>
#include <errno.h>
#include <image.h>
#include <lmb.h>
#include <malloc.h>
#include <mapmem.h>
#include <nand.h>
#include <asm/byteorder.h>
#include <linux/compiler.h>
#include <linux/ctype.h>
#include <linux/err.h>
#include <u-boot/zlib.h>


#ifdef CONFIG_CMD_PING
static int ping_server(const char *ip)
{
	net_ping_ip = string_to_ip(ip);
	if (net_ping_ip.s_addr == 0)
		return CMD_RET_USAGE;

	if (net_loop(PING) < 0) {
		printf("ping failed; tftp server %s is not alive\n", ip);
		return CMD_RET_FAILURE;
	}

	printf("tftp server %s is alive\n", ip);

	return CMD_RET_SUCCESS;
}
#else
static int ping_server(const char *ip)
{
	net_ping_ip = string_to_ip(ip);
	if (net_ping_ip.s_addr == 0)
		return CMD_RET_USAGE;

	printf("ping failed; can't ping tftp server %s\n", ip);
	return CMD_RET_FAILURE;
}
#endif

static int tftp_download(const char *addr, const char *bin_name)
{
	int size;
	char bin_size[20];
	load_addr = simple_strtoul(addr, NULL, 16);

	copy_filename(net_boot_file_name, bin_name,
			      sizeof(net_boot_file_name));

	size = net_loop(TFTPGET);

	if (size < 0) 
	{
		return CMD_RET_FAILURE;
	}
	sprintf(bin_size, "%d", size);
	setenv("bin_size", bin_size);

	/* done if no file was loaded (no errors though) */
	if (size == 0) 
	{
		return CMD_RET_SUCCESS;
	}

	/* flush cache */
	flush_cache(load_addr, size);

	return CMD_RET_SUCCESS;
}

int do_bootvtos(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	char *addrip = "192.168.1.112";
	char *serverip = "192.168.1.116";
	char *bin_name = "vtos.bin";
	char *entry_addr = "80080000";
	
	if(argc == 2)
	{
		entry_addr = argv[1];
	}
	char *ep = entry_addr;

	setenv("addrip", addrip);
	setenv("serverip", serverip);
	
	printf("###ping server %s###\n", serverip);
	if(ping_server(serverip) != CMD_RET_SUCCESS)
	{
		puts("###ping fail###\n");
		return CMD_RET_FAILURE;
	}

	printf("\n###tftp: load %s from %s to RAM 0x%s###\n", bin_name, serverip, entry_addr);
	if(tftp_download(entry_addr, bin_name) != CMD_RET_SUCCESS)
	{
		puts("###tftp fail###\n");
		return CMD_RET_FAILURE;
	}
		
	printf("\n###boot vtos at 0x%s###\n", ep);
	if(boot_vtos(ep) != CMD_RET_SUCCESS)
	{
		puts("###boot fail###\n");
		return CMD_RET_FAILURE;
	}
	return CMD_RET_SUCCESS;
}

#ifdef CONFIG_SYS_LONGHELP
static char bootvtos_help_text[] =
	"[addr]\n	- boot vtos stored in memory\n"
	"addr: load address in memory, 0x80080000 by default\n"
	"";
#endif

U_BOOT_CMD(
	bootvtos, CONFIG_SYS_MAXARGS, 1, do_bootvtos,
	"boot vtos.bin", bootvtos_help_text
);
