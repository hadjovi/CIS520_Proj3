#include <stdio.h>
#include <stdint.h>
#include "bitmap.h"
#include "bitmap.c"
#include "block_store.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

struct block_store
{      
    bitmap_t *Bmaps;
};

block_store_t *block_store_create()
{
    //create one bs
    block_store_t *bs = malloc(sizeof(block_store_t));
        
    //Allocate space for bmap list
    bs->Bmaps = malloc(BLOCK_STORE_NUM_BLOCKS * sizeof(bitmap_t));
    
    //create blockStore
    int i;
    
    for(i = 0; i < BLOCK_STORE_NUM_BLOCKS; i++)
    {
        if(i == 127)
        {
            bs->Bmaps[i] = *bitmap_create(BITMAP_SIZE_BYTES*8);
        }
        else
        {
            bs->Bmaps[i] = *bitmap_create(BLOCK_SIZE_BYTES*8);
        }      
    } 
        return bs;
}


void block_store_destroy(block_store_t *const bs)
{
    if(bs == NULL){
        return;
    }
    if(bs)
    {
        bitmap_destroy(bs->Bmaps);
    }
}

size_t block_store_allocate(block_store_t *const bs)
{
    if(bs == NULL){
        return SIZE_MAX;
    }
    size_t bit = bitmap_ffz(&bs->Bmaps[127]);
    if(bit >= BLOCK_STORE_AVAIL_BLOCKS){
        return SIZE_MAX;
    }
    if(bit == SIZE_MAX){
        return SIZE_MAX;
    }
    bitmap_set(&bs->Bmaps[127], bit);
    return bit;
}

bool block_store_request(block_store_t *const bs, const size_t block_id)
{
   if(bs == NULL || block_id > BLOCK_STORE_NUM_BLOCKS){
        return false;
    }
    if(bitmap_test(&bs->Bmaps[127], block_id)){
		return false;
	}
    bitmap_set(&bs->Bmaps[127], block_id);
    return true;
}

void block_store_release(block_store_t *const bs, const size_t block_id)
{
    if(bs == NULL || block_id > BLOCK_STORE_NUM_BLOCKS){
        return;
    }
    bitmap_reset(&bs->Bmaps[127], block_id);
}

size_t block_store_get_used_blocks(const block_store_t *const bs)
{
    if(bs == NULL){
        return SIZE_MAX;
    }
    return bitmap_total_set(&bs->Bmaps[127]);
}

size_t block_store_get_free_blocks(const block_store_t *const bs)
{
    if(bs == NULL){
        return SIZE_MAX;
    }
    return BLOCK_STORE_AVAIL_BLOCKS - bitmap_total_set(&bs->Bmaps[127]);
}

size_t block_store_get_total_blocks()
{
    return BLOCK_STORE_AVAIL_BLOCKS;
}

size_t block_store_read(const block_store_t *const bs, const size_t block_id, void *buffer)
{
    //Error Checking
    if(bs == NULL || block_id > BLOCK_STORE_NUM_BLOCKS || buffer == NULL){
        return 0;
    }
        
    memcpy(buffer, bitmap_export(&bs->Bmaps[block_id]), BLOCK_SIZE_BYTES);

    return BLOCK_SIZE_BYTES;
}

size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer)
{
    //Error Checking
    if(bs == NULL || block_id > BLOCK_STORE_NUM_BLOCKS || buffer == NULL){
        return 0;
    }
    
    bs->Bmaps[block_id] = *bitmap_import(BLOCK_SIZE_BYTES*8, buffer);

    return BLOCK_SIZE_BYTES;
}

block_store_t *block_store_deserialize(const char *const filename)
{
    //Error Checking
    if(filename == NULL){
        return NULL;
    }

    int k = 0;
    while(((char*)filename)[k] != '\0'){
        if(((char*)filename)[k] == '\n'){
            return NULL;
        }
        k++;
    }

    //Open the file to read
    int fd = open(filename, O_RDONLY, 0777);
    if(fd == -1){
        return NULL;
    }

    block_store_t *bs = block_store_create();
    
    for(int i = 0; i < BLOCK_STORE_NUM_BLOCKS; i++)
    {            
       read(fd, &bs->Bmaps[i], BLOCK_SIZE_BYTES);        
    }

    close(fd);

    return bs;
}

size_t block_store_serialize(const block_store_t *const bs, const char *const filename)
{
    //Parameter validation for filename and block store 
    if(filename == NULL || bs == NULL){
        return 0;
    }

    int k = 0;
    while(((char*)filename)[k] != '\0'){
        if(((char*)filename)[k] == '\n'){
            return 0;
        }
        k++;
    }

    //Open(or create) the file for writing
    int fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0777);
    if(fd == -1){
        return 0;
    }

    for(int i = 0; i < BLOCK_STORE_NUM_BLOCKS; i++)
    {            
       write(fd, &bs->Bmaps[i], BLOCK_SIZE_BYTES);        
    }

    close(fd);

    return BLOCK_STORE_NUM_BYTES;
}
