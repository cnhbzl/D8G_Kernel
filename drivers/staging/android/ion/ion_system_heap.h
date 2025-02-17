/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2017-2019, The Linux Foundation. All rights reserved.
 * Copyright (C) 2021 XiaoMi, Inc.
 */
#include <soc/qcom/secure_buffer.h>
#include "ion.h"

#ifndef _ION_SYSTEM_HEAP_H
#define _ION_SYSTEM_HEAP_H

#ifndef CONFIG_ALLOC_BUFFERS_IN_4K_CHUNKS
#if defined(CONFIG_IOMMU_IO_PGTABLE_ARMV7S)
static const unsigned int orders[] = {8, 4, 0};
#else
static const unsigned int orders[] = {4, 0};
#endif
#else
static const unsigned int orders[] = {0};
#endif

#define NUM_ORDERS ARRAY_SIZE(orders)

struct ion_system_heap {
	struct ion_heap heap;
	struct ion_page_pool **uncached_pools;
	struct ion_page_pool **cached_pools;
	struct ion_page_pool **secure_pools[VMID_LAST];
	/* Prevents unnecessary page splitting */
	struct mutex split_page_mutex;
#ifdef CONFIG_OPLUS_ION_BOOSTPOOL
	struct ion_boost_pool *uncached_boost_pool, *cam_pool;
#endif /* CONFIG_OPLUS_ION_BOOSTPOOL */
};

struct page_info {
	struct page *page;
	bool from_pool;
	unsigned int order;
	struct list_head list;
};

int order_to_index(unsigned int order);

void free_buffer_page(struct ion_system_heap *heap,
			     struct ion_buffer *buffer, struct page *page,
			     unsigned int order);
int ion_system_heap_create_pools(struct device *dev,
					struct ion_page_pool **pools, bool graphic_buffer_flag);

void ion_system_heap_destroy_pools(struct ion_page_pool **pools);

#endif