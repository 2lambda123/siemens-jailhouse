/*
 * Jailhouse, a Linux-based partitioning hypervisor
 *
 * Configuration for demo inmate on emCON-RZ/G1H:
 * 1 CPU, 64K RAM, serial ports SCIFA0, CCU
 *
 * Copyright (c) emtrion GmbH, 2017
 *
 * Authors:
 *  Jan von Wiarda <jan.vonwiarda@emtrion.de>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 */

#include <jailhouse/types.h>
#include <jailhouse/cell-config.h>

struct {
	struct jailhouse_cell_desc cell;
	struct jailhouse_cpu cpus[1];
	struct jailhouse_memory mem_regions[3];
} __attribute__((packed)) config = {
	.cell = {
		.signature = JAILHOUSE_CELL_DESC_SIGNATURE,
		.revision = JAILHOUSE_CONFIG_REVISION,
		.architecture = JAILHOUSE_ARM,
		.name = "emtrion-emconrzg1h-inmate-demo",
		.flags = JAILHOUSE_CELL_PASSIVE_COMMREG,

		.num_cpus = ARRAY_SIZE(config.cpus),
		.num_memory_regions = ARRAY_SIZE(config.mem_regions),

		.console = {
			.address = 0xe6c40000,
			.clock_reg = 0xe6150138,
			.gate_nr = 4,
			.divider = 0x1b,
			.type = JAILHOUSE_CON_TYPE_SCIFA,
			.flags = JAILHOUSE_CON_ACCESS_MMIO |
				 JAILHOUSE_CON_REGDIST_4,
		},
	},

	.cpus = {
		{
			.phys_id = 1,
		},
	},

	.mem_regions = {
		/* SCIFA0 */ {
			.phys_start = 0xe6c40000,
			.virt_start = 0xe6c40000,
			.size = 0x1000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO,
		},
		/* RAM */ {
			.phys_start = 0x7bff0000,
			.virt_start = 0,
			.size = 0x00010000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_EXECUTE | JAILHOUSE_MEM_LOADABLE,
		},
		/* communication region */ {
			.virt_start = 0x80000000,
			.size = 0x00001000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_COMM_REGION,
		},
	}
};
