#include <stdio.h>
#include <stdint.h>
#include "bitmap.h"
#include "bitmap.c"
#include "block_store.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

// include more if you need

// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

struct block_store{
    bitmap_t *data;
};


block_store_t *block_store_create()
{
    bitmap_t *test = bitmap_create(BITMAP_SIZE_BYTES * 8);
    
    block_store_t *bs = malloc(BLOCK_STORE_NUM_BLOCKS * sizeof(test));
    
    for(int j = 0; j < BLOCK_STORE_NUM_BLOCKS; j++){
        bs[j].data = bitmap_create(BITMAP_SIZE_BYTES * 8);
    }
    
    free(test);
    return bs;
}


void block_store_destroy(block_store_t *const bs)
{
    int c = 0;
    if(bs == NULL){
        return;
    }
    if(bs)
    {
        for(int i = 0; i < BLOCK_STORE_NUM_BLOCKS; i++)
        {            
            //printf(" Block num: %d\n", c);
            c++;
            if(bs[i].data)
            {
                bitmap_destroy(bs[i].data);
            }            
        }
        //bitmap_destroy(bs->data);
    }
    free(bs);
}

size_t block_store_allocate(block_store_t *const bs)
{
    if(bs == NULL){
        return SIZE_MAX;
    }
    size_t bit = bitmap_ffz(bs[127].data);
    if(bit >= BLOCK_STORE_AVAIL_BLOCKS){
        return SIZE_MAX;
    }
    if(bit == SIZE_MAX){
        return SIZE_MAX;
    }
    bitmap_set(bs[127].data, bit);
    return bit;
}

bool block_store_request(block_store_t *const bs, const size_t block_id)
{
   if(bs == NULL || block_id > BLOCK_STORE_NUM_BLOCKS){
        return false;
    }
    if(bitmap_test(bs[127].data, block_id)){
		return false;
	}
    bitmap_set(bs[127].data, block_id);
    return true;
}

void block_store_release(block_store_t *const bs, const size_t block_id)
{
    if(bs == NULL || block_id > BLOCK_STORE_NUM_BLOCKS){
        return;
    }
    bitmap_reset(bs[127].data,block_id);
}

size_t block_store_get_used_blocks(const block_store_t *const bs)
{
    if(bs == NULL){
        return SIZE_MAX;
    }
    return bitmap_total_set(bs[127].data);
}

size_t block_store_get_free_blocks(const block_store_t *const bs)
{
    if(bs == NULL){
        return SIZE_MAX;
    }
    return BLOCK_STORE_AVAIL_BLOCKS - bitmap_total_set(bs[127].data);
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
        
    memcpy(buffer, bs[block_id].data->data, BLOCK_SIZE_BYTES/8);

    return BLOCK_SIZE_BYTES;
}

size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer)
{
    //Error Checking
    if(bs == NULL || block_id > BLOCK_STORE_NUM_BLOCKS || buffer == NULL){
        return 0;
    }

    bs[block_id].data = bitmap_import(BLOCK_SIZE_BYTES, buffer);
    
    return BLOCK_SIZE_BYTES;
}

block_store_t *block_store_deserialize(const char *const filename)
{
    //Error Checking
    if(filename == NULL){
        return NULL;
    }

    int i = 0;
    while(((char*)filename)[i] != '\0'){
        if(((char*)filename)[i] == '\n'){
            return NULL;
        }
        i++;
    }

    //Open the file to read
    int fd = open(filename, O_RDONLY, 0777);
    if(fd == -1){
        return NULL;
    }

    // void * buffer = malloc(BLOCK_STORE_NUM_BYTES);

    block_store_t *bs = malloc(BLOCK_STORE_NUM_BYTES);
    bs = block_store_create();
    // if(read(fd, bs, BLOCK_SIZE_BYTES)  == -1){
    //     close(fd);
    //     return NULL;
    // }
    for(i = 0; i < BLOCK_STORE_NUM_BLOCKS; i++){
        read(fd, bs[i].data, BITMAP_SIZE_BYTES);
    }
    

    //Read in the file using the read function
    
    // bs = block_store_create();
    // bs->data = buffer;

    close(fd);
    return bs;
}

size_t block_store_serialize(const block_store_t *const bs, const char *const filename)
{
    //Error Checking
    if(filename == NULL || bs == NULL){
        return 0;
    }

    int i = 0;
    while(((char*)filename)[i] != '\0'){
        if(((char*)filename)[i] == '\n'){
            printf("BAD FILENAME\n");
            return 0;
        }
        i++;
    }
    int errno;
    //Open(or create) the file for writing
    int fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0777);
    if(fd == -1){
        printf("FAILED TO OPEN\n");
        printf("ERROR: %d\n", errno);
        return 0;
    }

    
    // void * buffer = malloc(BLOCK_STORE_NUM_BYTES);
    
    write(fd,bs,BLOCK_STORE_NUM_BYTES);

    close(fd);

    return BLOCK_STORE_NUM_BYTES;
}
