/*
 * Copyright 2006, 2007, 2008, 2009, 2010  Gabriele Modena <gm@nowave.it>. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, this list of
 *     conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice, this list
 *     of conditions and the following disclaimer in the documentation and/or other materials
 *     provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY Gabriele Modena ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those of the
 * authors and should not be interpreted as representing official policies, either expressed
 * or implied, of Gabriele Modena.
 */



#include <stdlib.h>
#include <stdio.h>

#include "graph.h"
#include "stack.h"


void createStack(t_stack *s) 
{
  s->top = NULL;
}

/* Inserisce un elemento nello stack */
int stackPush(t_stack *stack, s_type item)
{
  t_item *new;

  new = malloc(sizeof(t_item));
  if (new == NULL) {
    fprintf(stderr, "Errore: malloc()\n");
    exit(2);
        
  }

  new->item = item;
  new->next = stack->top;
  stack->top = new;
  
  return 0;
}

s_type stackPop(t_stack *stack)
{
  s_type item;
  t_item *tmp;


  if (!isEmptyStack(stack)) {
    tmp = stack->top;
    item = stack->top->item;
    stack->top = stack->top->next;
    free(tmp);
    return item;
  }
  else {
    fprintf(stderr, "ERROR: stack is empty");
    exit(1);
  }
}

int isEmptyStack(t_stack *stack)
{
  return (stack->top == NULL);
}

void finalizeStack(t_stack *stack)
{
  while(!isEmptyStack(stack)) 
    stackPop(stack);

  stack->top = NULL;
}

void createQueue(t_queue *q)
{
  q->top = q->bottom = NULL;
}

/* Inserisce un elemento al fondo della coda */
int queuePush(t_queue *queue, q_type item)
{
  t_qitem *new;
  
  new = malloc(sizeof(t_qitem));
  if (new == NULL) {
    fprintf(stderr, "Errore: malloc()\n");
    exit(2);
  }
  
  new->item = item;
  new->next = NULL;

  if (queue->top == NULL)
  {
   queue->bottom = queue->top = new;
  }
  else {
  queue->bottom->next = new;
  queue->bottom = queue->bottom->next;
  }
  return 0;
}

/* Estrae un elemento dalla cima della coda */
q_type queuePop(t_queue *queue)
{
  t_qitem *new;
  q_type item;

  if (!isEmptyQueue(queue)) {
	  new = queue->top;
	  queue->top = queue->top->next;
	  item = new->item; 
	  free(new);
	  return item;
  }
  
}

int isEmptyQueue(t_queue *queue)
{
  return ( queue->top ==  NULL);
}

void finalizeQueue(t_queue *queue)
{
  if (!isEmptyQueue(queue))
    queuePop(queue);

  queue->top = NULL;
  queue->bottom = NULL;
}
