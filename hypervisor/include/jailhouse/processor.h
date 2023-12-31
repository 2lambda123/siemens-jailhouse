/*
 * Jailhouse, a Linux-based partitioning hypervisor
 *
 * Copyright (c) Siemens AG, 2013-2022
 *
 * Authors:
 *  Jan Kiszka <jan.kiszka@siemens.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 */
#ifndef _JAILHOUSE_PROCESSOR_H
#define _JAILHOUSE_PROCESSOR_H

#include <asm/processor.h>

unsigned long phys_processor_id(void);

unsigned int cpu_by_phys_processor_id(u64 phys_id);

#endif
