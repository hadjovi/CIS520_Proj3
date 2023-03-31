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
    block_store_t *bs = malloc(BLOCK_STORE_NUM_BYTES);
    if(bs == NULL){
        return NULL;
    }

    bitmap_t * a[BLOCK_STORE_NUM_BLOCKS];

    for(int i = 0; i < BLOCK_STORE_NUM_BLOCKS; i++){
        a[i] = bitmap_create(BLOCK_SIZE_BYTES);
    }

    bs->data = *a;
    return bs;
}


void block_store_destroy(block_store_t *const bs)
{
    if(bs == NULL){
        return;
    }
    if(bs){
        bitmap_destroy(bs->data);
    }
    free(bs);
}

size_t block_store_allocate(block_store_t *const bs)
{
    if(bs == NULL){
        return SIZE_MAX;
    }
    size_t bit = bitmap_ffz(bs->data);
    if(bit >= BLOCK_STORE_AVAIL_BLOCKS){
        return SIZE_MAX;
    }
    if(bit == SIZE_MAX){
        return SIZE_MAX;
    }
    bitmap_set(bs->data, bit);
    return bit;
}

bool block_store_request(block_store_t *const bs, const size_t block_id)
{
   if(bs == NULL || block_id > BLOCK_STORE_NUM_BLOCKS){
        return false;
    }
    if(bitmap_test(bs->data, block_id)){
		return false;
	}
    bitmap_set(bs->data,block_id);
    return true;
}

void block_store_release(block_store_t *const bs, const size_t block_id)
{
    if(bs == NULL || block_id > BLOCK_STORE_NUM_BLOCKS){
        return;
    }
    bitmap_reset(bs->data,block_id);
}

size_t block_store_get_used_blocks(const block_store_t *const bs)
{
    if(bs == NULL){
        return SIZE_MAX;
    }
    return bitmap_total_set(bs->data);
}

size_t block_store_get_free_blocks(const block_store_t *const bs)
{
    if(bs == NULL){
        return SIZE_MAX;
    }
    return BLOCK_STORE_AVAIL_BLOCKS - bitmap_total_set(bs->data);
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
    
    memcpy(buffer, &bs->data[block_id], BLOCK_SIZE_BYTES);

    return BLOCK_SIZE_BYTES;
}

size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer)
{
    //Error Checking
    if(bs == NULL || block_id > BLOCK_STORE_NUM_BLOCKS || buffer == NULL){
        return 0;
    }

    memcpy(&bs->data[block_id], buffer, BLOCK_SIZE_BYTES);

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
    int fd = open(filename, O_RDONLY);
    if(fd == -1){
        printf("FAILED TO OPEN DESER");
        return NULL;
    }

    //Move to the approprate offset

    //Read in the file using the read function
    void * buffer = malloc(BLOCK_SIZE_BYTES);
    if(read(fd, buffer, BLOCK_SIZE_BYTES)  == -1){
        close(fd);
        printf("FAILED TO READ DESER");
        return false;
    }

    block_store_t *bs = malloc(BLOCK_SIZE_BYTES);
    memcpy(bs, buffer, BLOCK_SIZE_BYTES);
    
    //If the file closes successfully the bulk read has been successful
    if(close(fd) != 0){
        printf("FAILED TO CLOSE FILE DESER");
        return NULL;
    }
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

    // //Open(or create) the file for writing
    // int fd = open(filename, O_WRONLY | O_CREAT);
    // if(fd == -1){
    //     printf("FAILED TO OPEN SER\n");
    //     return 0;
    // }

    FILE *fpt;
    fpt=fopen(filename, "w+");

    void * buffer = malloc(BLOCK_STORE_NUM_BYTES);
    memcpy(buffer, bs, BLOCK_STORE_NUM_BYTES);

    // if( write(fd, buffer, BLOCK_STORE_NUM_BYTES) == -1){
    //     close(fd);
    //     printf("FAILED TO WRITE SER\n");
    //     return 0;
    // }
    
    fputs(buffer, fpt);
    
    fclose(fpt);
    return BLOCK_STORE_NUM_BYTES;
}