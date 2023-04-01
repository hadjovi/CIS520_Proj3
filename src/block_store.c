#include <stdio.h>
#include <stdint.h>
#include "bitmap.h"
#include "bitmap.c"
#include "block_store.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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
    size_t bit = bitmap_ffz(bs[126].data);
    if(bit >= BLOCK_STORE_AVAIL_BLOCKS){
        return SIZE_MAX;
    }
    if(bit == SIZE_MAX){
        return SIZE_MAX;
    }
    bitmap_set(bs[126].data, bit);
    return bit;
}

bool block_store_request(block_store_t *const bs, const size_t block_id)
{
   if(bs == NULL || block_id > BLOCK_STORE_NUM_BLOCKS){
        return false;
    }
    if(bitmap_test(bs[126].data, block_id)){
		return false;
	}
    bitmap_set(bs[126].data,block_id);
    return true;
}

void block_store_release(block_store_t *const bs, const size_t block_id)
{
    if(bs == NULL || block_id > BLOCK_STORE_NUM_BLOCKS){
        return;
    }
    bitmap_reset(bs[126].data,block_id);
}

size_t block_store_get_used_blocks(const block_store_t *const bs)
{
    if(bs == NULL){
        return SIZE_MAX;
    }
    return bitmap_total_set(bs[126].data);
}

size_t block_store_get_free_blocks(const block_store_t *const bs)
{
    if(bs == NULL){
        return SIZE_MAX;
    }
    return BLOCK_STORE_AVAIL_BLOCKS - bitmap_total_set(bs[126].data);
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
    
    //memcpy(buffer, bs[block_id].data, BLOCK_SIZE_BYTES);

    const uint8_t *temp = bitmap_export(bs[block_id].data);
    //temp = bitmap_export(bs[block_id].data);

    //buffer = (void *) bitmap_export(bs[block_id].data);

    memcpy(buffer, temp, BLOCK_SIZE_BYTES);

    return BLOCK_SIZE_BYTES;
}

size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer)
{
    //Error Checking
    if(bs == NULL || block_id > BLOCK_STORE_NUM_BLOCKS || buffer == NULL){
        return 0;
    }
    
    // int c = 0;

    // while(buffer != '\0')
    // {
    //     c++;
    //     buffer++;
    // }
    //memcpy(bs[block_id].data, buffer, BLOCK_SIZE_BYTES);
    //const bitmap_t *temp = import(BLOCK_SIZE_BYTES, buffer);
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
            return false;
        }
        i++;
    }

    //Open the file to read
    int fd = open(filename, O_RDONLY | S_IROTH);
    if(fd == -1){
        printf("FAILED TO OPEN DESER");
        return NULL;
    }

    void * buffer = malloc(BLOCK_STORE_NUM_BYTES);

    if(read(fd, buffer, BLOCK_SIZE_BYTES)  == -1){
        close(fd);
        printf("FAILED TO READ DESER");
        return false;
    }

    //Read in the file using the read function

    block_store_t *bs = malloc(BLOCK_STORE_NUM_BYTES);
    memcpy(bs, buffer, BLOCK_STORE_NUM_BYTES);
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
            return 0;
        }
        i++;
    }
    printf("FN Good\n");

    //Open(or create) the file for writing
    int fd = open(filename, O_WRONLY | O_CREAT | S_IROTH | S_IWOTH);
    if(fd == -1){
        printf("FAILED TO OPEN DESER");
        return 0;
    }

    printf("FD OPEN\n");
    void * buffer = malloc(BLOCK_STORE_NUM_BYTES);
    memcpy(buffer, bs, BLOCK_STORE_NUM_BYTES);

    close(fd);

    return BLOCK_STORE_NUM_BYTES;
}