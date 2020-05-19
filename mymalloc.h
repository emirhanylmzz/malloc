/* mymalloc.c & hw3
 * Emirhan YILMAZ 18120205037
 * 07.03.2020
*/
#include <stdio.h>

typedef enum {bestFit, worstFit, firstFit} Strategy;

Strategy strategy = firstFit;


typedef struct{
    unsigned int size;
    unsigned int isfree;
}Info;

/*NOT: Hocam 1 ay önce odanıza geldiğimde 
bir kaç soru için prev kullanabilirsiniz dediniz boundary tag yerine*/
typedef struct{
   Info info;           //number of  16 byte blocks
   struct block *next;  //next free block(not just next)
   struct block *prev;  //previous block  
   char data[0];        //start of the allocated memory
 }Block;
/*Block: |4byte(size)|4byte(isfree)|8-byte next|8-byte prev|0-byte(data)|*/

static Block *free_list = NULL;     /*start of the free list*/
static Block *heap_start = NULL;    /*head of allocated memory from sbrk */
static Block *end = NULL;
static int freelist_size = 1; //number of blocks in freelist

void *mymalloc(size_t size);
void *myfree(void *p);
Block *split(Block *b, size_t size);
int test(int fitStrategy);
