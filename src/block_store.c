#include <stdio.h>
#include <stdint.h>
#include "bitmap.h"
#include "bitmap.c"
#include "block_store.h"
// include more if you need

// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

struct block_store{
    bitmap_t *data;
};

block_store_t *block_store_create()
{
    block_store_t *bs = malloc(BLOCK_STORE_NUM_BYTES);
    if(bs == NULL){
        return NULL;
    }

    bitmap_t * a[BLOCK_STORE_NUM_BLOCKS];

    for(int i = 0; i < BLOCK_STORE_NUM_BLOCKS; i++){
        a[i] = malloc(BLOCK_SIZE_BYTES);
        a[i] = bitmap_create(BLOCK_SIZE_BYTES);
    }
    bs->data = *a;
    return bs;
}


void block_store_destroy(block_store_t *const bs)
{
    if(bs != NULL){
        if(bs->data != NULL){
            free(bs->data);
        }
        free(bs);
    }
}

size_t block_store_allocate(block_store_t *const bs)
{
    UNUSED(bs);
    // long unsigned bit = bitmap_ffz(bs);
    // if(bit != SIZE_MAX){
    //     bitmap_flip(bs, bit);
    // }
    return 0;
}

bool block_store_request(block_store_t *const bs, const size_t block_id)
{
    UNUSED(bs);
    UNUSED(block_id);
    return false;
}

void block_store_release(block_store_t *const bs, const size_t block_id)
{
    
    UNUSED(bs);
    UNUSED(block_id);
}

size_t block_store_get_used_blocks(const block_store_t *const bs)
{

    UNUSED(bs);
    return 0;
}

size_t block_store_get_free_blocks(const block_store_t *const bs)
{

    UNUSED(bs);
    return 0;
}

size_t block_store_get_total_blocks()
{
    return BLOCK_STORE_AVAIL_BLOCKS;
}

size_t block_store_read(const block_store_t *const bs, const size_t block_id, void *buffer)
{
    UNUSED(bs);
    UNUSED(block_id);
    UNUSED(buffer);
    return 0;
}

size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer)
{
    UNUSED(bs);
    UNUSED(block_id);
    UNUSED(buffer);
    return 0;
}

block_store_t *block_store_deserialize(const char *const filename)
{
    UNUSED(filename);
    return NULL;
}

size_t block_store_serialize(const block_store_t *const bs, const char *const filename)
{
    UNUSED(bs);
    UNUSED(filename);
    return 0;
}