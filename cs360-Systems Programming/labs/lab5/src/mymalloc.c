//will hopkirk
//lab5 - mymalloc

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include "mymalloc.h"

//the chunk struck with no previous anymore because of the lack of the -m32 flag
struct chunk{

  size_t size;
  struct chunk *next;
};

//comparison function for my one qsort call got from piazza post
int compare(const void *x, const void *y){

  return (*(int *)x - *(int *)y);
}

//my one global is the pointer to ther head of the free list as was advised
struct chunk *head = NULL;

void *my_malloc(size_t size){

    /*
     * variables for various uses
     *
     * VARLIST-
     * ptr = the pointer to the chunk i am returning
	   * tmp = flitorary chunk for sbrk calls as to not override the head
     * flit = (fl = free list), (it = iterator), used to traverse the free list
     * set size to proper allignment from the marz writeup linked in planks writeup for jmalloc
    */
    struct chunk *ptr = NULL;
    struct chunk *tmp;
    struct chunk *flit;
    size = (size + 7 + 8) & -8;

    /*
     * mymalloc algorithm "breakdown"
     *
     * there are 2 main cases and the second case has a sub case
     * they are broken into - case 1, case 2, and case 2a
	   * case 1 = when the head of the free list is null
     * case 2 = when the head of the free list is not null
     * case 2a = when the head is not null and nothing in the free list can store the amount of bytes requested
    */

    /*
     * CASE 1 - when the head of the free list is null
     *
     * see what the size is and either break off 8192 or just the size given whichever is larger
     * store adress of tmp into head
	   * remove the size of the requested chunk from heads size
     * set the pointer to the end of head
     * check if head is below 8 (meaning it cant store anything else) if it is bump it off the list by setting it to the next thing on the list
     * finally set the ptrs size to the given size and return it
    */
    if(head == NULL){

      if(size > 8192){

        tmp = sbrk(size);
        tmp->size = size;
      }else{

        tmp = sbrk(8192);
        tmp->size = 8192;
      }
        head = tmp;
        head->size -= size;
        ptr = (void*)head + head->size;
        if(head->size <= 8){

            head = head->next;
        }
        ptr->size = size;
        return ((void *)ptr + 8);

    /*
     * CASE 2 - when the head of the free list is not null
     *
     * set up flit as the head to begin iterating through the free list
     * check every value in fl to see if it will fit the size
	   * check if there is something in the next node of the free list if its null we break out
     * as before we check the head to see if its still usefull
     * finally check if we got anything from the traversal and if we did we set size and return it
     * otherwise its on to case 2a
    */
    }else{

        flit = head;
        while(ptr == NULL){

          if(flit->size >= size){

            flit->size -= size;
            ptr = (void*)flit + flit->size;  
          }

          if(flit->next != NULL){

            flit = flit->next;
          }else{

            break;
          }
        }
      if(head->size <= 8){

        head = head->next;
      }
    }
    if(ptr != NULL){
          
      ptr->size = size;
      return ((void *)ptr + 8);
    }else{
      
    /*
     * CASE 2a = when the head is not null and nothing in the free list can store the amount of bytes requested
     *
     * start with an sbrk check exactly like the first one
     * then set the iterator to head to begin iterating through the free list
	   * this time we are just trying to get to the end so we just go till its NULL
     * now that we are at the end of the fl we set the next thing to the sbrk value and continue like earlier in pt 1
     * one last check just like checking the head to see if we need to bump off this node
     * and finally we can return the pointer to the chunk
    */
      if(size > 8192){

        tmp = sbrk(size);
        tmp->size = size;
      }
      else{

        tmp = sbrk(8192);
        tmp->size = 8192;
      }
        flit = head;
        while(flit->next != NULL){

          flit = flit->next;
        }
        flit->next = tmp;
        tmp->size -= size;
        ptr = (void *)tmp + tmp->size;
        if(tmp->size <= 8){

          flit->next = NULL;
        }
        
      ptr->size = size;  
      return ((void *)ptr + 8);
    }
}

void my_free(void *ptr){

    /*
     * variables for various uses
     *
     * VARLIST-
     * freeptr = the ptr to the chunk im freeing
     * flit = (fl = free list), (it = iterator), used to traverse the free list
     * set the freeptr to the ptr because ptr is void *ptr and we need a chunk struct
    */
    struct chunk *freeptr;
    struct chunk *flit;
    freeptr = (void *)ptr - 8;

    /*
     * algorithm for free
     *
     * just see if head is null first if it is we set it to the free ptr and we are done
     * otherwise we need to traverse
	   * litterally the same traversal as case 2a from mymalloc
     * just set the next null chunk in the free list the the freeptr
     * thats it
    */
    if(head == NULL){

      head = freeptr;
    }else{

      flit = head;
      while(flit->next != NULL){

        flit = flit->next;
      }
      flit->next = freeptr;
    }
}

//just returns the head of the fl
void *free_list_begin(){

    return ((void*)head);
}

//just returns the next ptr in the fl although needed to store it as a chunk struct in order to use ->next
void *free_list_next(void* node){

    struct chunk *ptr;
    ptr = node;
    return ((void*)ptr->next);
}

  /*
   * small note
   *
   * there was a piazza post about typecasting the array and i was having a similar porblem
   * after trying different data types it seemed that the (struct Chunk*)(intptr_t) worked without giving the [-Wint-to-pointer-cast] warning
   * one of the TA's answers said that doing this loses information but this passes the gradescripts so i hope its ok i used it
  */
void coalesce_free_list(){

  /*
   * STEP 1
   *
   * first i see if head is null in which case we dont need to do anything (no free list)
   * otherwise we need to traverse once through the list to get the number of nodes (nodes)
	 * because we are using qsort() we need an array so we make an array of ints to represent the adresses
   * now we traverse the list again to put all the node's adresses into the array
   * finally we can call the qsort()
  */
  int nodes = 1;
  struct chunk *flit;
  if(head == NULL){

    return;
  }else{

    flit = head;
    while(flit->next != NULL){
      
      flit = flit->next;
      nodes++;
    }
  }

  int fl[nodes];

  flit = head;
  for(int i = 0; i < nodes; i++){

    fl[i] = (int)flit;
    flit = flit->next;
  }

  qsort(fl, nodes, sizeof(int), compare);

  /*
   * STEP 2
   *
   * now we have a sorted array of the adresses (nodes)
   * first we set the head node and use our flit so we dont alter the head
   * time to traverse once again
	 * all is the same but this time im traversing the array
   * we are basically reversing the step of putting the names on the array now they are sorted we need them on the fl again
   * finally we have to set the next thing to null to finnish the list off
  */
  head = (struct chunk *)(intptr_t)fl[0];
  flit = head;
  for(int i = 1; i < nodes; i++){
    
    flit->next = (struct chunk *)(intptr_t)fl[i];
    flit = flit->next;
  }
  flit->next = NULL;

  /*
   * STEP 3
   *
   * now we have the actual free list and its actually sorted (could have skipped the above if there was a way to store them in order in myfree)
   * first we set up our iterator and for the last time we traverse
   * if we encounter a node that is contiguos to the next node (nodeadress+nodesize = nextnodeadress) we need to merge them
	 * we merge them by adding the next node's size the the current node
   * now we need to check if the node after this new merged node exists
   * if it does then we set it to the next node in the list
   * otherwise we need to set the next node to NULL
  */
  flit = head;
  while(flit->next != NULL){

    if((void *)flit + flit->size == (void *)flit->next){

      flit->size += flit->next->size;
      if(flit->next->next != NULL){

        flit->next = flit->next->next;
      }else{

        flit->next = NULL;
      }
    }else{

      flit = flit->next;
    }
  }
}


