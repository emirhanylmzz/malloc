/* mymalloc.c & hw3
 * Emirhan YILMAZ 18120205037
 * 07.03.2020
*/
#include "mymalloc.h"
#include <stdlib.h>
#include <unistd.h>

void *mymalloc(size_t size){
    static int firstcall = 1;
    if (firstcall){
        heap_start = sbrk(sizeof(Block) * 1024);
        if (heap_start < 0){
            perror("sbrk failed!\n");
        }
        heap_start->info.size = 1024;
        heap_start->info.isfree = 0;
        heap_start->prev = NULL;
        end = (void*)heap_start + sizeof(Block) + heap_start->info.size;
        end->info.size = 0;
        end->info.isfree = 0;
        end->next = NULL;
        end->prev = (void*)heap_start;
        heap_start->next = (void*)end;
        free_list = heap_start;
        firstcall = 0;  
    }
    /*changed "16" to "24" because of block size 
    i used prev this increased block size 8 byte, 16 to 24*/
    if (size % 24 != 0)
    {
        size = size + (24 - (size % 24));
    }
    Block *p = free_list;
    if (strategy == bestFit)
    {
        //finding right block
        int min = p->info.size;
        while (p < end)
        {
            if (p->info.size > size && p->info.size <= min)
            {
                if ((void*)p->next != end)
                {
                    min = p->info.size;
                    p = (Block*)p->next;
                }
                else
                    break;
            }
        }
        if(p == end)
        {
            fprintf(stderr,"no available space");
            return NULL;
        }
        else if(size < p->info.size)
            split(p, size);
        else
            p->info.isfree = 1;
        return (void*)p;    
    }
    else if(strategy == worstFit)
    {
        //finding right block
        size_t max = 0;
        while (p < end)
        {
            if (p->info.size > size && max < p->info.size)
            {  
                if ((void*)p->next != end)
                {
                    max = p->info.size;
                    p = (Block*)p->next;
                }
                else
                    break;
            }
        }
        if (p == end || p == NULL)
        {
            fprintf(stderr,"no available space");
            return NULL;   
        }
        else if(size < p->info.size)
            split(p, size);
        else
            p->info.isfree = 1;
        return (void*)p;
    }
    else if(strategy == firstFit)
    {
        //finding right block
        while(size > p->info.size && p < end){
            p = (Block*)p->next;
        }
        if (p == end)
        {
            fprintf(stderr,"no available space\n");
        }
        if(size < p->info.size)
            split(p, size);
        else
            p->info.isfree = 1;
        return (void*)p;    
    }
}

/* bu alan iki bloğa split edilerek, birisi free liste eklenmeli diğerinin
(​size​ kadar alanın) başlangıç adresi return edilmelidir.*/
Block *split(Block *b, size_t size){
    int old_size = b->info.size;
    b->info.size = size;
    b->info.isfree = 1;
    Block *q = (void*)b + b->info.size + sizeof(Block);
    if (freelist_size == 1 && b == free_list)
    {
        free_list = q;
        q->next = (void*)end;
    }
    else if(q != free_list)
    {   
        //finding last free blok in free list 
        Block *temp = free_list;
        for (int i = 0; i < freelist_size; i++)
        {
            if ((void*)temp->next != end)
            {
                temp = (void*)temp->next;
            }
        }
        temp->next = (void*)q;
        q->next = (void*)end;
        freelist_size++;
    }
    q->info.size = old_size - size - sizeof(Block);
    q->info.isfree = 0;
    q->prev = (void*)b;
    return b;
}
/*p ile verilen alan free edilerek free liste eklenmelidir. Eğer
coalescing(birleştirme) yapılabiliyorsa (bir önceki ve bir sonraki blokla) yapılmalıdır.*/
void *myfree(void *p){
    Block *b = (Block*)p; /*block header*/
    b->info.isfree = 0;
    Block *prev = (Block*)b->prev;
    Block *next = (void*)b + b->info.size + sizeof(Block);
    
    //if previus block is free
    if (prev->info.isfree == 0)
    {
        prev->info.size += b->info.size + sizeof(Block);
        b = prev;
        next->prev = (void*)b;
    }
    //if next blok is free
    else if(next->info.isfree == 0)
    {
        b->info.size += next->info.size + sizeof(Block);
        next = b;
        prev->next = (void*)next;
    }
    //finding last free blok in free list 
    Block *temp = free_list;
    while (temp < end)
    {
        if ((void*)temp->next != end)
        {
            temp = (void*)temp->next;
        }
        else
            break;
    }
    temp->next = (void*)b;
    b->next = (void*)end;
    freelist_size++;
    return (void *)b;
}

int test(int fitStrategy){
    strategy = fitStrategy;
    int *m = (int*)mymalloc(sizeof(int));
    int *ptr = (int*)mymalloc(23);
    int *ptr2 = (int*)mymalloc(71);
    int *ptr3 = (int*)mymalloc(47);
    myfree(ptr);
    myfree(ptr2);
    myfree(ptr3);
    //printing blocks in freelist
    Block *b = (void*)free_list;
    for (int i = 1; i <= freelist_size; i++)
    {
        if (b == end || b->next == NULL)
        {
            break;
        }
        printf("Block %d Size: %d\n", i, b->info.size);
        b = (void*)b->next;
    }
    //NOT: Hocam inanın çok fazla uğraştım freelist ten remove yapamadığım için bazı hatalar çıkıyor :( 
    return 0;
}
int main(){
    return test(2);
}