/*
 * Jailhouse, a Linux-based partitioning hypervisor
 *
 * Configuration for Xilinx ZynqMP ZCU102 eval board
 *
 * Copyright (c) Siemens AG, 2016-2022
 *
 * Authors:
 *  Jan Kiszka <jan.kiszka@siemens.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 *
 * Reservation via device tree: 0x800000000..0x83fffffff
 */

#include <jailhouse/types.h>
#include <jailhouse/cell-config.h>

struct {
	struct jailhouse_system header;
	struct jailhouse_cpu cpus[4];
	struct jailhouse_memory mem_regions[13];
	struct jailhouse_irqchip irqchips[1];
	struct jailhouse_pci_device pci_devices[2];
	union jailhouse_stream_id stream_ids[3];
} __attribute__((packed)) config = {
	.header = {
		.signature = JAILHOUSE_SYSTEM_SIGNATURE,
		.revision = JAILHOUSE_CONFIG_REVISION,
		.architecture = JAILHOUSE_ARM64,
		.flags = JAILHOUSE_SYS_VIRTUAL_DEBUG_CONSOLE,
		.hypervisor_memory = {
			.phys_start = 0x800000000,
			.size =       0x000400000,
		},
		.debug_console = {
			.address = 0xff000000,
			.size = 0x1000,
			.type = JAILHOUSE_CON_TYPE_XUARTPS,
			.flags = JAILHOUSE_CON_ACCESS_MMIO |
				 JAILHOUSE_CON_REGDIST_4,
		},
		.platform_info = {
			.pci_mmconfig_base = 0xfc000000,
			.pci_mmconfig_end_bus = 0,
			.pci_is_virtual = 1,
			.pci_domain = -1,
			.iommu_units = {
				{
					.type = JAILHOUSE_IOMMU_ARM_MMU500,
					.base = 0xfd800000,
					.size = 0x20000,
				},
			},
			.arm = {
				.gic_version = 2,
				.gicd_base = 0xf9010000,
				.gicc_base = 0xf902f000,
				.gich_base = 0xf9040000,
				.gicv_base = 0xf906f000,
				.maintenance_irq = 25,
			},
		},
		.root_cell = {
			.name = "ZynqMP-ZCU102",

			.num_cpus = ARRAY_SIZE(config.cpus),
			.num_memory_regions = ARRAY_SIZE(config.mem_regions),
			.num_irqchips = ARRAY_SIZE(config.irqchips),
			.num_pci_devices = ARRAY_SIZE(config.pci_devices),
			.num_stream_ids = ARRAY_SIZE(config.stream_ids),

			.vpci_irq_base = 136-32,
		},
	},

	.cpus = {
		{
			.phys_id = 0,
		},
		{
			.phys_id = 1,
		},
		{
			.phys_id = 2,
		},
		{
			.phys_id = 3,
		},
	},

	.mem_regions = {
		/* IVSHMEM shared memory region for 0001:00:00.0 */
		JAILHOUSE_SHMEM_NET_REGIONS(0x800400000, 0),
		/* IVSHMEM shared memory region for 0001:00:01.0 */
		JAILHOUSE_SHMEM_NET_REGIONS(0x800500000, 0),
		/* MMIO low (permissive) */ {
			.phys_start = 0xfd000000,
			.virt_start = 0xfd000000,
			.size =	      0x00800000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO,
		},
		/* MMIO high (permissive) */ {
			.phys_start = 0xfd820000,
			.virt_start = 0xfd820000,
			.size =	      0x027e0000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO,
		},
		/* RAM */ {
			.phys_start = 0x0,
			.virt_start = 0x0,
			.size = 0x80000000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_EXECUTE,
		},
		/* RAM */ {
			.phys_start = 0x800600000,
			.virt_start = 0x800600000,
			.size = 0x7fa00000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_EXECUTE,
		},
		/* PCI host bridge */ {
			.phys_start = 0x8000000000,
			.virt_start = 0x8000000000,
			.size = 0x1000000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO,
		},
	},

	.irqchips = {
		/* GIC */ {
			.address = 0xf9010000,
			.pin_base = 32,
			.pin_bitmap = {
				0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
			},
		},
	},

	.pci_devices = {
		/* 0001:00:01.0 */ {
			.type = JAILHOUSE_PCI_TYPE_IVSHMEM,
			.domain = 1,
			.bdf = 1 << 3,
			.bar_mask = JAILHOUSE_IVSHMEM_BAR_MASK_INTX,
			.shmem_regions_start = 0,
			.shmem_dev_id = 0,
			.shmem_peers = 2,
			.shmem_protocol = JAILHOUSE_SHMEM_PROTO_VETH,
		},
		/* 0001:00:02.0 */ {
			.type = JAILHOUSE_PCI_TYPE_IVSHMEM,
			.domain = 1,
			.bdf = 2 << 3,
			.bar_mask = JAILHOUSE_IVSHMEM_BAR_MASK_INTX,
			.shmem_regions_start = 4,
			.shmem_dev_id = 0,
			.shmem_peers = 2,
			.shmem_protocol = JAILHOUSE_SHMEM_PROTO_VETH,
		},
	},

	.stream_ids = {
		{
			.mmu500.id = 0x860,
			.mmu500.mask_out = 0x0,
		},
		{
			.mmu500.id = 0x861,
			.mmu500.mask_out = 0x0,
		},
		{
			.mmu500.id = 0x870,
			.mmu500.mask_out = 0xf,
		},
	},
};
