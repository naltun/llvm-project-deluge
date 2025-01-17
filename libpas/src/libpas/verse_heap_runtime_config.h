/*
 * Copyright (c) 2023 Epic Games, Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY EPIC GAMES, INC. ``AS IS AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL EPIC GAMES, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef VERSE_HEAP_RUNTIME_CONFIG_H
#define VERSE_HEAP_RUNTIME_CONFIG_H

#include "pas_heap_runtime_config.h"

#include "pas_heap_page_provider.h"
#include "pas_large_heap_physical_page_sharing_cache.h"
#include "pas_reserve_commit_cache_large_free_heap.h"
#include "verse_heap_object_set_set.h"

#if PAS_ENABLE_VERSE

PAS_BEGIN_EXTERN_C;

struct verse_heap_object_set;
struct verse_heap_runtime_config;
typedef struct verse_heap_object_set verse_heap_object_set;
typedef struct verse_heap_runtime_config verse_heap_runtime_config;

struct verse_heap_runtime_config {
    pas_heap_runtime_config base;

    /* Some heaps use the global page cache, while others use their own page cache. They use their
       own page cache if they are caged to a specific VA area.

       If they use the global page cache, all of this will be zero or NULL. Otherwise they will have
       nonzero values. */
    uintptr_t heap_base;
    size_t heap_size;
    size_t heap_alignment;
	
	pas_heap_page_provider page_provider;
	void* page_provider_arg;
    
    pas_large_heap_physical_page_sharing_cache large_cache;
    pas_reserve_commit_cache_large_free_heap small_cache;

    /* FIXME: Should this be here, or in the type? Could be either, I guess. Maybe that's true of all of the
       fields here. */
    verse_heap_object_set_set object_sets;
};

/* Allocate pages either from the config's own page cache (if it has one) or out of the global page cache
   (otherwise).

   Size must be a multiple of VERSE_HEAP_CHUNK_SIZE. */
PAS_API pas_allocation_result verse_heap_runtime_config_allocate_chunks(verse_heap_runtime_config* config,
                                                                        size_t size,
																		pas_physical_memory_transaction* transaction,
																		pas_primordial_page_state desired_state);

PAS_API pas_allocation_result verse_heap_runtime_config_chunks_provider(size_t size,
																		pas_alignment alignment,
																		const char* name,
																		pas_heap* heap,
																		pas_physical_memory_transaction* transaction,
																		pas_primordial_page_state desired_state,
																		void* arg);

PAS_END_EXTERN_C;

#endif /* PAS_ENABLE_VERSE */

#endif /* VERSE_HEAP_RUNTIME_CONFIG_H */

