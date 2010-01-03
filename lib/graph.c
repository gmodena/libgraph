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

t_vertex *addVertex(t_graph *graph, v_tag id)  
{
  t_vertex *v, *cur, *prev;

  /* Alloco memoria per il nuvoo vertice */
  v = malloc(sizeof(t_vertex));
  if (v == NULL) {
    fprintf(stderr, "Errore: malloc()\n");
    exit(2);
  
  }
  v->id = id;
  v->visited = 0;
  v->edges = NULL;
  v->next = NULL;
  v->data = NULL;
  graph->num_vertices += 1;
  
  /* Inserisco il vertice nel grafo */
  if (graph->vertices == NULL)
    graph->vertices = v;
  else {
    cur =  graph->vertices;
    while (cur->next != NULL) {
      cur = (cur)->next;
    }
    (cur)->next = v;
  }
  
  return v;
}

int addEdge(t_graph *graph, t_vertex *srcVertex, t_vertex *dstVertex, int weight)
{
  t_edge *e;
  t_edge *cur;
  e = malloc(sizeof(t_edge));
  if (e == NULL) {
    fprintf(stderr, "Errore: malloc()\n");
    exit(2);    
  }
  e->weight = weight; // Standard unweighted graph 
  e->links = dstVertex;
 
  /* Inserisco un lato DIRETTO da srcVertex a dstVertex */
  if (srcVertex->edges == NULL)
    srcVertex->edges = e;
  else {
    cur = srcVertex->edges;
    while (cur->next != NULL) {
      cur = (cur)->next;
    }
    (cur)->next = e;
  }

  /* Nel caso in cui il grafo sia non orientato, eseguo un collegamento anche in
   * senso inverso */

  if (graph->is_directed == FALSE) {
    t_edge *b;
    b = malloc(sizeof(t_edge));
    if (b == NULL) {
      fprintf(stderr, "Errore: malloc()\n");
      exit(2);
            
    }
    b->links = srcVertex;
    if (dstVertex->edges == NULL)
      dstVertex->edges = b;
    else {
      cur = dstVertex->edges;
      while (cur->next != NULL) {
        cur = (cur)->next;
      }
      (cur)->next = b;
    }
    
  }

  return 0;
}

t_vertex *getVertex(t_graph *graph, v_tag id)
{
  t_vertex *v;
  v = graph->vertices;
  while (v->id != id) {
    v = v->next;
  }
  return v;
}

int isInGraph(t_graph *graph, v_tag id)
{
  t_vertex *v;
  if (graph->num_vertices == 0)
    return -1; // Il grafo è vuoto.
  else {
    v =  graph->vertices;
    while (v != NULL) {
      if (v->id == id) {// Vertice già presente nel grafo.
        return 0;}
      v = (v)->next;
    }
    
  }
  return -1;   
}

/* verifica l'esistenza di un arco diretto da v1 a v2. In caso affermativo
 * ritorna un puntatore, altrimenti ritorna NULL  */
t_edge *isConnectedTo(t_vertex *v1, t_vertex *v2)
{
  t_edge *e;
  
  e = (v1)->edges;
  while (e != NULL) {
    if (e->links->id == v2->id) {
        return e;
    }
    e = (e)->next;
  }
  return NULL;
}

void removeVertex(t_graph *graph, t_vertex *v)
{
  t_vertex *cur, *prev, *tmp;
  t_edge *e, *etmp;
  
  /* Elimino i lati entranti e il riferimento a v nella lista */
  tmp = graph->vertices;
  while (tmp != NULL) {
    if ((tmp->next != NULL) && (tmp->next->id == v->id))
      prev = tmp;
    e = tmp->edges;
    while (e != NULL){
     /* Rimuovo il capo della lista dei lati */
     if (e->links->id == v->id) {
       etmp = (e)->next;
       free(tmp->edges);
       tmp->edges = etmp;
     }
     
     if ((e->next != NULL) && (e->next->links->id == v->id)) {
        etmp = e->next->next;
        free(e->next);
       (e)->next = etmp;
     }
      e = (e)->next;
    }
    tmp = (tmp)->next;
  }
  
  e = v->edges;
  while (e != NULL) {
    etmp = e;
    e = etmp->next;
    free(etmp);
  }

  tmp = graph->vertices;
  if (prev == NULL)
    graph->vertices = tmp->next;
  else {
    prev->next = v->next;
  }

  v->next = NULL;
  v->data = NULL;

  free(v);
}

void removeEdge(t_graph *graph, t_vertex *src, t_vertex *dst)
{
  t_edge *e, *etmp;

  e = src->edges;
  while (e != NULL) {
    if (e->links->id == dst->id) {
      etmp = (e)->next;
      free(src->edges);
      src->edges = etmp;
    }
    if ((e->next != NULL) && (e->next->links->id == dst->id)) {
      etmp = e->next->next;
      free(e->next);
      e->next = etmp;
    }
    e = (e)->next;
  }
}

void destroyGraph(t_graph *graph)
{
  t_vertex *v, *prev;
  t_edge *e, *eprev;
  
  while (graph->vertices != NULL) { 
    v = graph->vertices;
    if (v->next == NULL) {
      while (v->next != NULL) {
        e = v->edges;
        if (e->next != NULL) {
          e->links = NULL;
          free(e);
          v->edges = NULL;
        }
        else {
          eprev = e;
          e = eprev->next;
          while(e->next != NULL) {
            eprev = e;
            e = eprev->next;
          }
          e->links = NULL;
          free(e);
          eprev->next = NULL;
        }
      }
      free(v);
      graph->vertices = NULL;
    }
    else {
      prev = v;
      v = prev->next;
      while (v->next != NULL) {
        prev = v;
        v = prev->next;
      }
      while (v->edges != NULL) {
        e = v->edges;
        if (e->next == NULL){
          e->links = NULL;
          free(e);
          v->edges = NULL;
        }
        else {
          eprev = e;
          e = eprev->next;
          while (e->next != NULL) {
            eprev = e;
            e = eprev->next;
          }
          e->links = NULL;
          free(e);
          eprev->next = NULL;
        }
      }
      free(v);
      graph->num_vertices--;
      prev->next = NULL; 
    }
  }
  graph->num_vertices--;
  free(graph);

}

t_graph *createGraph(int is_directed)
{
  t_graph *graph;
  if ((graph = malloc(sizeof(t_graph))) == NULL)
  {
    fprintf(stderr, "Errore: malloc()\n");
    exit(2);
  }
  graph->num_vertices = 0;
  graph->vertices = NULL;
  graph->is_directed = is_directed;

  return graph;
}
