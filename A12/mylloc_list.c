/*----------------------------------------------
* Author: Emily Lu
* Date: 12/5/2024
* Description: Implemented malloc and free based on the reading and extend the header to keep track of both the requested size and the amount of memory currently used in the chunk.
---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
  int size;
  int used;
  struct chunk *next;
};
struct chunk *flist = NULL;

 void *malloc (size_t size) {
   // TODO: Implement malloc with a free list (See reading for details)
   if(size <= 0) {
    return NULL;
   }
   
   struct chunk *ptr = flist;
   struct chunk *prev = NULL;

   while(ptr != NULL) {
     if(ptr->size >= size) {
       if(prev != NULL) {
         prev->next = ptr->next;
       } else {
         flist = ptr->next;
       }
       ptr -> used = size;
       ptr -> next = NULL;
       return (void*)(ptr + 1);
     } else {
       prev = ptr;
       ptr = ptr->next;
     }
   }

    struct chunk *memory = sbrk(size + sizeof(struct chunk));
    if(memory == (void *) - 1) {
      return NULL; //sbrk failed
    } else {
      memory->size = size;
      memory->used = size;
      memory->next = NULL;
    }
    return (void*)(memory + 1);
 }

void free(void *memory) {
  // TODO: Implement malloc with a free list (See reading for details)
  if(memory != NULL) {
    struct chunk *cnk = (struct chunk*)((struct chunk*)memory - 1);
    cnk->next = flist;
    flist = cnk;
  }
  return;
}
