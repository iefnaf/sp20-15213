/* 
 * Code for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Modified to store strings, 2018
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    queue_t *q =  (queue_t*)malloc(sizeof(queue_t));
    if (q == NULL)
    {
      return NULL;
    }
    /* What if malloc returned NULL? */
    q->head = NULL;
    q->tail = NULL;
    q->_size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (q == NULL)
    {
      return;
    }
    for (int i = 0; i < q->_size; i++)
    {
      list_ele_t *p = q->head;
      q->head = q->head->next;
      if (p->value != NULL)
      {
        free(p->value);
      }
      
      free(p);
    }

    free(q);
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    /* What should you do if the q is NULL? return false*/
    if (q == NULL)
    {
      return false;
    }

    list_ele_t *newh = (list_ele_t*)malloc(sizeof(list_ele_t));
    if (newh == NULL)
    {
      return false;
    }
    /* Don't forget to allocate space for the string and copy it */
    newh->value = (char*)malloc(strlen(s) + 1);
    /* What if either call to malloc returns NULL? */
    if (newh->value == NULL)
    {
      free(newh); //don't forget to free
      return false;
    }

    strcpy(newh->value, s);
    //当前队列为空
    if (q->_size == 0)
    {
      q->head = newh;
      q->tail = newh;
    }
    else
    {
      newh->next = q->head;
      q->head = newh;
    }
    q->_size++;
    return true;
}


/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    if (q == NULL)
    {
      return false;
    }

    list_ele_t *newt = (list_ele_t*)malloc(sizeof(list_ele_t));
    if (newt == NULL)
    {
      return false;
    }
    newt->value = (char*)malloc(strlen(s) + 1);
    if (newt->value == NULL)
    {
      free(newt);
      return false;
    }

    strcpy(newt->value, s);

    if (q->tail == NULL)//当前队列中元素为空
    {
      q->head = newt;
      q->tail = newt;
    }
    else
    {
      q->tail->next = newt;
      q->tail = newt;
    }
    newt->next = NULL;
    q->_size++;
    return true;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If sp is non-NULL and an element is removed, copy the removed string to *sp
  (up to a maximum of bufsize-1 characters, plus a null terminator.)
  The space used by the list element and the string should be freed.
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* You need to fix up this code. */
    if (q == NULL || q->_size == 0)
    {
      return false;
    }

    list_ele_t *oldh = q->head;
    q->head = q->head->next;
    oldh->next = NULL;

    if (sp != NULL)
    {
      strncpy(sp, oldh->value, bufsize - 1);
      sp[bufsize-1] = '\0'; 
    }
    
    if (oldh->value != NULL)
    {
      free(oldh->value);
    }
    
    free(oldh);
    q->_size--;
    return true;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    if (q == NULL)
    {
      return 0;
    }
    return q->_size;
}

/*
  Reverse elements in queue
  No effect if q is NULL or empty
  This function should not allocate or free any list elements
  (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
  It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q == NULL || q->_size == 0)
    {
      return;
    }

    list_ele_t *oldh = q->head;
    list_ele_t *oldt = q->tail;
    for (int i = 0; i < q->_size - 1; i++)
    {
      list_ele_t *h = q->head;
      q->head = q->head->next;
      h->next = q->tail->next;
      q->tail->next = h;
    }
    q->head = oldt;
    q->tail = oldh;
}

