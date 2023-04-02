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

// typedef struct
// {
//     bitmap_t *bmap;
// }block_t;

// struct block_store
// {      
//     block_t *blocks;
// };

struct block_store
{      
    bitmap_t *Bmaps;
};
// block_t *create_single_block()
// {
//   //create origins bitmap
//   bitmap_t *bmOrigin = bitmap_create(BITMAP_SIZE_BYTES * 8);
  
//   //Allocate space for one full block
//   block_t *block_single = malloc(8 * sizeof(bmOrigin));
  
//   //create generic block
//   int i;
//   for(i = 0; i < 8; i++)
//   {
//     block_single[i].bmap = bitmap_create(BITMAP_SIZE_BYTES * 8);
//   }
  
//   free(bmOrigin);
  
//   return block_single;
// }

// block_t *free_block_map()
// {
//   //create origins bitmap
//   bitmap_t *bmOrigin = bitmap_create(BITMAP_SIZE_BYTES * 8);
  
//   //Allocate space for free_block_map
//   block_t *free_block_map = malloc(sizeof(bmOrigin));
  
//   //create generic block
  
//   free_block_map[0].bmap = bitmap_create(BITMAP_SIZE_BYTES * 8);
  
//   free(bmOrigin);

//   return free_block_map;
// }

// block_store_t *block_store_create()
// {
//     //create origins block
//     block_t *blockOrigin = create_single_block();
        
//     //Allocate space for blockStore
//     block_store_t *bs = malloc(BLOCK_STORE_NUM_BLOCKS * sizeof(block_t));
    
//     //create blockStore
//     int i;
    
//     for(i = 0; i < BLOCK_STORE_NUM_BLOCKS; i++)
//     {
//         if(i == 127)
//         {
//             bs[i].blocks = free_block_map();
//         }
//         else
//         {
//             bs[i].blocks = create_single_block();
//         }      
//     }   
    
//     free(blockOrigin);

//     return bs;
// }

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
    
    printf("Done Creating");
    return bs;
}


void block_store_destroy(block_store_t *const bs)
{
    printf("Begins destruction");
    if(bs == NULL){
        return;
    }
    if(bs)
    {
        // for(int i = 0; i < BLOCK_STORE_NUM_BLOCKS; i++)
        // {
            
        //     bitmap_destroy(&bs->Bmaps[i]);
            
        // }
        bitmap_destroy(bs->Bmaps);
    }
    //free(bs);
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

    //bitmap_export(&bs->Bmaps[block_id]);
    // memcpy(buffer, bs[block_id].blocks[0].bmap, BITMAP_SIZE_BYTES);
    // memcpy(buffer, bs[block_id].blocks[1].bmap, BITMAP_SIZE_BYTES);
    // memcpy(buffer, bs[block_id].blocks[2].bmap, BITMAP_SIZE_BYTES);
    // memcpy(buffer, bs[block_id].blocks[3].bmap, BITMAP_SIZE_BYTES);
    // memcpy(buffer, bs[block_id].blocks[4].bmap, BITMAP_SIZE_BYTES);
    // memcpy(buffer, bs[block_id].blocks[5].bmap, BITMAP_SIZE_BYTES);
    // memcpy(buffer, bs[block_id].blocks[6].bmap, BITMAP_SIZE_BYTES);

    // int i;
    // for(i = 0; i < 8; i++)
    // {
    //     memcpy(buffer+(BITMAP_SIZE_BYTES*i), bs[block_id].blocks[0].bmap, BITMAP_SIZE_BYTES);
    // }

    return BLOCK_SIZE_BYTES;
}

size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer)
{
    //Error Checking
    if(bs == NULL || block_id > BLOCK_STORE_NUM_BLOCKS || buffer == NULL){
        return 0;
    }
    
    bs->Bmaps[block_id] = *bitmap_import(BLOCK_SIZE_BYTES, buffer);
    // int i;
    // for(i = 0; i < 8; i++)
    // {
    //     bs[block_id].blocks[0].bmap = bitmap_import(BITMAP_SIZE_BYTES, buffer+(BITMAP_SIZE_BYTES*i));
    // }

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
    
    // int j;
    // for(j = 0; j < BLOCK_STORE_NUM_BLOCKS; j++)
    // {
    //     read(fd,bs[i].data,sizeof(bs[i].data));
    // }

    // for(int i = 0; i < BLOCK_STORE_NUM_BLOCKS; i++)
    // {            
    //     if(i == 127)
    //     {
    //         if(bs[i].blocks[0].bmap)
    //         {
    //             read(fd,bs[i].blocks[0].bmap,sizeof(bs[i].blocks[0].bmap));
    //         }
    //         else
    //         { 
    //             return NULL;
    //         }
    //     }
    //     else
    //     {
    //         for(int j = 0; j < 8; j++)
    //         {
    //             if(bs[i].blocks[j].bmap)
    //             {
    //                 read(fd,bs[i].blocks[j].bmap,sizeof(bs[i].blocks[j].bmap));
    //             }
    //             else
    //             { 
    //                 return NULL;
    //             }
    //         }
    //     }            
    // }
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
            printf("BAD FILENAME\n");
            return 0;
        }
        k++;
    }

    //Open(or create) the file for writing
    int fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0777);
    if(fd == -1){
        printf("FAILED TO OPEN\n");
        return 0;
    }
    
    // int j;
    // for(j = 0; j < BLOCK_STORE_NUM_BLOCKS; j++)
    // {
    //     write(fd,bs[i].data,sizeof(bs[i].data));
    // }
    //write(fd,bs,BLOCK_STORE_NUM_BYTES);

    // for(int i = 0; i < BLOCK_STORE_NUM_BLOCKS; i++)
    // {            
    //     if(i == 127)
    //     {
    //         if(bs[i].blocks[0].bmap)
    //         {
    //             write(fd,bs[i].blocks[0].bmap,sizeof(bs[i].blocks[0].bmap));
    //         }
    //         else
    //         { 
    //             return 0;
    //         }
    //     }
    //     else
    //     {
    //         for(int j = 0; j < 8; j++)
    //         {
    //             if(bs[i].blocks[j].bmap)
    //             {
    //                 write(fd,bs[i].blocks[j].bmap,sizeof(bs[i].blocks[j].bmap));
    //             }
    //             else
    //             { 
    //                 return 0;
    //             }
    //         }
    //     }            
    // }

    for(int i = 0; i < BLOCK_STORE_NUM_BLOCKS; i++)
    {            
       write(fd, &bs->Bmaps[i], BLOCK_SIZE_BYTES);        
    }

    close(fd);

    return BLOCK_STORE_NUM_BYTES;
}