/*
 * Jailhouse, a Linux-based partitioning hypervisor
 *
 * Configuration for Linux inmate on AMD Seattle:
 * 2 CPUs, 512MB RAM, 1 serial port
 *
 * Copyright (C) 2015 Huawei Technologies Duesseldorf GmbH
 *
 * Authors:
 *  Antonios Motakis <antonios.motakis@huawei.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 */

#include <jailhouse/types.h>
#include <jailhouse/cell-config.h>

struct {
	struct jailhouse_cell_desc cell;
	struct jailhouse_cpu cpus[2];
	struct jailhouse_memory mem_regions[12];
	struct jailhouse_irqchip irqchips[2];
	struct jailhouse_pci_device pci_devices[1];
} __attribute__((packed)) config = {
	.cell = {
		.signature = JAILHOUSE_CELL_DESC_SIGNATURE,
		.revision = JAILHOUSE_CONFIG_REVISION,
		.architecture = JAILHOUSE_ARM64,
		.name = "linux-inmate-demo",
		.flags = JAILHOUSE_CELL_PASSIVE_COMMREG,

		.num_cpus = ARRAY_SIZE(config.cpus),
		.num_memory_regions = ARRAY_SIZE(config.mem_regions),
		.num_irqchips = ARRAY_SIZE(config.irqchips),
		.num_pci_devices = ARRAY_SIZE(config.pci_devices),

		.console = {
			.address = 0xe1010000,
			.type = JAILHOUSE_CON_TYPE_PL011,
			.flags = JAILHOUSE_CON_ACCESS_MMIO |
				 JAILHOUSE_CON_REGDIST_4,
		},
	},

	.cpus = {
		{
			.phys_id = 0x0300,
		},
		{
			.phys_id = 0x0301,
		},
	},

	.mem_regions = {
		/* IVSHMEM shared memory region */
		JAILHOUSE_SHMEM_NET_REGIONS(0x83e4000000, 1),
		/* UART */ {
			.phys_start = 0xe1010000,
			.virt_start = 0xe1010000,
			.size = 0x10000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO | JAILHOUSE_MEM_ROOTSHARED,
		},
		/* xgmac */ {
			.phys_start = 0xe0900000,
			.virt_start = 0xe0900000,
			.size =         0x100000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO,
		},
		/* phy */ {
			.phys_start = 0xe1240000,
			.virt_start = 0xe1240000,
			.size =           0x1000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO,
		},
		/* phy */ {
			.phys_start = 0xe1250000,
			.virt_start = 0xe1250000,
			.size =           0x1000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO,
		},
		/* RAM */ {
			.phys_start = 0x83dfff0000,
			.virt_start = 0x0,
			.size = 0x10000,	/* 64 KB */
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_EXECUTE | JAILHOUSE_MEM_DMA |
				JAILHOUSE_MEM_LOADABLE,
		},
		/* RAM */ {
			.phys_start = 0x83c0000000,
			.virt_start = 0x83c0000000,
			.size =         0x1fff0000,	/* 512 MB - 64 KB */
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_EXECUTE | JAILHOUSE_MEM_DMA |
				JAILHOUSE_MEM_LOADABLE,
		},
		/* v2m */ {
			.phys_start = 0xe1180000,
			.virt_start = 0xe1180000,
			.size =           0x1000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_IO |
				JAILHOUSE_MEM_ROOTSHARED,
		},
		/* communication region */ {
			.virt_start = 0x80000000,
			.size = 0x00001000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_COMM_REGION,
		},
	},

	.irqchips = {
		/* GIC */ {
			.address = 0xe1110000,
			.pin_base = 32,
			.pin_bitmap = {
				0, 0,
				(1 << (100 - 96)) /* MSI */
			},
		},
		/* GIC */ {
			.address = 0xe1110000,
			.pin_base = 352,
			.pin_bitmap = {
				(1 << (354 - 352)) | /* xgmac1 */
				(1 << (356 - 352)) | /* xgmac1 */
				(1 << (360 - 352)) | /* uart */
				(1 << (373 - 352)) | /* xgmac1 */
				(1 << (374 - 352)) | /* xgmac1 */
				(1 << (375 - 352)) | /* xgmac1 */
				(1 << (376 - 352))   /* xgmac1 */
			},
		},
	},

	.pci_devices = {
		/* 00:0f.0 */ {
			.type = JAILHOUSE_PCI_TYPE_IVSHMEM,
			.bdf = 0x0078,
			.bar_mask = JAILHOUSE_IVSHMEM_BAR_MASK_MSIX,
			.num_msix_vectors = 2,
			.shmem_regions_start = 0,
			.shmem_dev_id = 1,
			.shmem_peers = 2,
			.shmem_protocol = JAILHOUSE_SHMEM_PROTO_VETH,
		},
	},
};
