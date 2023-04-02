#include <stdio.h>
#include <stdint.h>
#include "bitmap.h"
#include "bitmap.c"
#include "block_store.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

// Creates a structure to hold the bitmaps
struct block_store
{      
    bitmap_t *Bmaps;
};


block_store_t *block_store_create()
{
    //Make sure enought space for the Block Store
    block_store_t *bs = malloc(sizeof(block_store_t));
        
    //Allocate space for the whole list of bitmaps
    bs->Bmaps = malloc(BLOCK_STORE_NUM_BLOCKS * sizeof(bitmap_t));
    
    int i;
    for(i = 0; i < BLOCK_STORE_NUM_BLOCKS; i++)
    {
        if(i == 127) //Creates a bitmap for the Free Block Map on block 127
        {
            bs->Bmaps[i] = *bitmap_create(BITMAP_SIZE_BYTES*8);
        }
        else //Creates a bitmap for everyother block
        {
            bs->Bmaps[i] = *bitmap_create(BLOCK_SIZE_BYTES*8);
        }      
    } 
    //Returns an empty, ready to go Block_Store
    return bs;
}


void block_store_destroy(block_store_t *const bs)
{
    //Parameter Checking
    if(bs == NULL){
        return;
    }
    //If it exists
    if(bs)
    {
        //Use the bitmap destroy function to handle destroying all the bitmaps
        bitmap_destroy(bs->Bmaps);
    }
    //Free the bs pointer
    free(bs);
}

size_t block_store_allocate(block_store_t *const bs)
{
    //Parameter Checking
    if(bs == NULL){
        return SIZE_MAX;
    }
    //Find the first available block in the FBM
    size_t bit = bitmap_ffz(&bs->Bmaps[127]);
    //Error Checking the bit value
    if(bit >= BLOCK_STORE_AVAIL_BLOCKS){
        return SIZE_MAX;
    }
    if(bit == SIZE_MAX){
        return SIZE_MAX;
    }

    //Set the FBM bit high to indicate the block is in use.
    bitmap_set(&bs->Bmaps[127], bit);
    return bit;
}

bool block_store_request(block_store_t *const bs, const size_t block_id)
{
    //Parameter Checking
    if(bs == NULL || block_id > BLOCK_STORE_NUM_BLOCKS){
        return false;
    }
    //Check the FBM to ensure the block is not in use
    if(bitmap_test(&bs->Bmaps[127], block_id)){
		return false;
	}
    //Set the FBM block to high
    bitmap_set(&bs->Bmaps[127], block_id);
    return true;
}

void block_store_release(block_store_t *const bs, const size_t block_id)
{
    //Parameter Checking
    if(bs == NULL || block_id > BLOCK_STORE_NUM_BLOCKS){
        return;
    }
    //Set the FBM block low indicating the block is free to use
    bitmap_reset(&bs->Bmaps[127], block_id);
}

size_t block_store_get_used_blocks(const block_store_t *const bs)
{
    //Parameter Checking
    if(bs == NULL){
        return SIZE_MAX;
    }
    //Return the total number of set bits, AKA used blocks
    return bitmap_total_set(&bs->Bmaps[127]);
}

size_t block_store_get_free_blocks(const block_store_t *const bs)
{
    //Parameter Checking
    if(bs == NULL){
        return SIZE_MAX;
    }
    //Return the total Number of unset blocks, AKA unused blocks
    return BLOCK_STORE_AVAIL_BLOCKS - bitmap_total_set(&bs->Bmaps[127]);
}

size_t block_store_get_total_blocks()
{
    //Returns the total number of blocks available for use within the block store.
    return BLOCK_STORE_AVAIL_BLOCKS;
}

size_t block_store_read(const block_store_t *const bs, const size_t block_id, void *buffer)
{
    //Parameter Checking
    if(bs == NULL || block_id > BLOCK_STORE_NUM_BLOCKS || buffer == NULL){
        return 0;
    }
    
    //Copies the data from a bitmap into a buffer
    memcpy(buffer, bitmap_export(&bs->Bmaps[block_id]), BLOCK_SIZE_BYTES);

    //Returns the number of bytes in a single block being read
    return BLOCK_SIZE_BYTES;
}

size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer)
{
    //Parameter Checking
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
    //Error Checking
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
