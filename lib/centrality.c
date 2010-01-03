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
#include "centrality.h"

void betweennessCentrality(t_graph *graph)
{
  t_vertex *s, *t, *w, *v;
  t_edge *e;
  t_parents *p, *tmp;


  w = graph->vertices;
  while (w != NULL)
  {
    w->data = malloc(sizeof(cb_data));
    if (w->data == NULL) {
      fprintf(stderr, "Errore: malloc()\n");
      exit(2);
    }
    w->data->rank = 0.0;
    w->data->parents = NULL;
    w = (w)->next;
  }
          

  /* Eseguo la scansione dei vertici di G */
  s = graph->vertices;
  while ( s != NULL) {
    t_stack S; createStack(&S); // Inizializzo lo stack
    t_queue Q; createQueue(&Q); // Inizializzo la coda

    /* Inizializzo P[w] */
    w = graph->vertices;
    while (w != NULL)
    {
      if (w->data->parents != NULL) {
        freeParent(w);
      }
      w->data->parents = NULL;    
      w->data->dependency = 0.0;
      w->data->sp_num = 0.0;
      w->data->distance = -1.0;
      w = (w)->next;
    }
    
    s->data->sp_num = 1.0;
    s->data->distance = 0.0;

    queuePush(&Q, s);
    while(!isEmptyQueue(&Q)) {
      v = queuePop(&Q);
      stackPush(&S, v);

      /* Analizzo i vicini di v */
      e = v->edges;
      while ( e != NULL) {
        w = e->links;

        /* Sto visitando w per la prima volta */
        if (w->data->distance < 0.0) {
          queuePush(&Q, w);
          w->data->distance = v->data->distance + 1.0; // grafo non pesato!
        }
        
        /* Percorso minimo verso w passando per v ? */
        if (w->data->distance == v->data->distance + 1.0) {
                  
          w->data->sp_num = w->data->sp_num + v->data->sp_num; 
                      
          /* Aggiorno la lista dei padri */
          appendParent(w, v);
        }
        
        e = (e)->next;
      }
                  
      
    }
   v = graph->vertices;
   while ( v != NULL) {
     v->data->dependency = 0.0;
     v = (v)->next;
   }


    while (!isEmptyStack(&S)) {
      t_parents *par;
      w = stackPop(&S);

      par = w->data->parents;
      while (par != NULL) {
        double partial_dep = 0.0;
        v = par->vertex;
        partial_dep = v->data->sp_num/w->data->sp_num;
        v->data->dependency += partial_dep * (1.0 +  w->data->dependency);
        par = (par)->next;
      }
      if (w != s) {
        w->data->rank +=  w->data->dependency; 
      }
    }

    /* destroy stack and queue */

    finalizeStack(&S);
    finalizeQueue(&Q);
    s = (s)->next; // Passo al vertice successivo
  }

  if (graph->is_directed == FALSE) {
    v = graph->vertices;
    while ( v != NULL) {
      v->data->rank = v->data->rank / 2.0 ;
      v = (v)->next;
    }
  }  
}

void printGraphCB(t_graph *graph)
{
  t_vertex *v;
  v = graph->vertices;
    while ( v != NULL) {
       printf("%d\t\t\t%.16f\t\t\t%.16f\t\t\t%.16f\n", 
           v->id, 
           v->data->rank, 
           v->data->rank/((double)(graph->num_vertices-1)*(double)(graph->num_vertices-2))/2.0,
           v->data->rank/((double)(graph->num_vertices-1)*(double)(graph->num_vertices-2))
           
           );


       v = (v)->next;
    }
}

void destroyCBData(t_graph *graph)
{
  t_vertex *w;
  t_parents *p, *tmp;
  w = graph->vertices;
  while (w != NULL)
  {
    if (w->data != NULL) {
      p = w->data->parents;
      while (p != NULL) {
        p->vertex = NULL;
        tmp = (p)->next;
        (p)->next = NULL;
        free(p);
        p = tmp;
      }
    }
    if ((w)->data != NULL)
      free(w->data);
    w = (w)->next;
  }
}

void appendParent(t_vertex *w, t_vertex *v )
{
  t_parents *P, *cur;
  P = malloc(sizeof(t_parents));
  if (P == NULL) {
    fprintf(stderr, "Errore: malloc()\n");
    exit(2);
  }
  P->vertex = v;
  P->next = NULL;
  if (w->data != NULL) {
    if (w->data->parents == NULL) {
      w->data->parents = P;
    }
    else {
      cur = w->data->parents;
      while (cur->next != NULL) {
        cur = (cur)->next;
      }
      (cur)->next = P;
    }
  }
}

void freeParent(t_vertex *w)
{
  t_parents *p, *tmp;

  p = w->data->parents;
  
  while (p != NULL) {
    p->vertex = NULL;
    tmp = (p)->next;
    (p)->next = NULL;
    free(p);
    p = tmp;
  }
}
