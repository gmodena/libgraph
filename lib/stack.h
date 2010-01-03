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

#ifndef _stack_h
#define _stack_h

/* Strutture dati per gestire lo STACK del grafo */


typedef t_vertex *s_type; // elemento di uno stack

typedef struct titem {
  s_type item;
  struct titem *next;
} t_item;

typedef struct tstack {
  struct titem *top;
} t_stack;

/* Strutture dati per la CODA */

typedef t_vertex *q_type;

typedef struct tqitem {
  q_type item;
  struct tqitem *next;
} t_qitem;

typedef struct tqueue {
    struct tqitem *top, *bottom;
} t_queue;


/* Funzioni per gestire lo STACK*/

void createStack(t_stack *stack);
int stackPush(t_stack *stack, s_type item);
s_type stackPop(t_stack *stack);
int isEmptyStack(t_stack *stack);
void finalizeStack(t_stack *stack);

/* Funzioni per gestire la CODA */

void createQueue(t_queue *queue);
int queuePush(t_queue *queue, q_type item);
q_type queuePop(t_queue *queue);
int isEmptyQueue(t_queue *queue);
void finalizeQueue(t_queue *queue);
  

#endif
