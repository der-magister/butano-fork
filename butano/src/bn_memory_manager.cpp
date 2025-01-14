/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_memory_manager.h"

#include "bn_optional.h"
#include "bn_config_memory.h"
#include "bn_best_fit_allocator.h"
#include "../hw/include/bn_hw_memory.h"

#include "bn_memory.cpp.h"
#include "bn_cstdlib.cpp.h"
#include "bn_cstring.cpp.h"

namespace bn::memory_manager
{

namespace
{
    static_assert(BN_CFG_MEMORY_MAX_EWRAM_ALLOC_ITEMS > 0);

    class static_data
    {

    public:
        optional<best_fit_allocator<BN_CFG_MEMORY_MAX_EWRAM_ALLOC_ITEMS>> allocator;
    };

    BN_DATA_EWRAM static_data data;
}

void init()
{
    char* start = hw::memory::ewram_heap_start();
    char* end = hw::memory::ewram_heap_end();
    data.allocator.emplace(static_cast<void*>(start), end - start);
}

void* ewram_alloc(int bytes)
{
    return data.allocator->alloc(bytes);
}

void* ewram_calloc(int num, int bytes)
{
    return data.allocator->calloc(num, bytes);
}

void* ewram_realloc(void* ptr, int new_bytes)
{
    return data.allocator->realloc(ptr, new_bytes);
}

void ewram_free(void* ptr)
{
    return data.allocator->free(ptr);
}

int used_alloc_ewram()
{
    return data.allocator->used_bytes();
}

int available_alloc_ewram()
{
    return data.allocator->available_bytes();
}

int used_items_ewram()
{
    return data.allocator->used_items();
}

int available_items_ewram()
{
    return data.allocator->available_items();
}

}
