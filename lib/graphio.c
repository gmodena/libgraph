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


#include "graphio.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "graph.h"

int readFromCSV(t_graph *graph, char *filename)
{
  FILE *fp;
  char *line, *src, *dst, *weight;
  int w;
  t_vertex *v1, *v2;
  line = malloc(sizeof(char)*100);
  if (line == NULL) {
    fprintf(stderr, "Errore: malloc()\n");
    exit(2);
  }
  if (line)
  if ( (fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Errore di I/O nell'apertura del file %s\n", filename);
    return -1;
  }
  
  while (fgets(line, 100, fp)) {
    src = strtok(line, " ");
    dst = strtok(NULL, " ");
    weight = strtok(NULL, " " );
    
    if (weight == NULL)
      w = 1;
    else 
      w = atoi(weight);
    
    
    if (isInGraph(graph, atoi(src)) == -1)
      v1 = addVertex(graph, atoi(src));
   else 
      v1 = getVertex(graph, atoi(src));
  
    if (isInGraph(graph, atoi(dst)) == -1)
      v2 = addVertex(graph, atoi(dst));
    else
      v2 = getVertex(graph, atoi(dst));
    
    addEdge(graph, v1, v2, w);
  }
  
  free(line);
  fclose(fp);

}


void dumpDotGraph(t_graph *graph)
{
  t_graph *dump;
  t_vertex *v, *dv, *dw;
  t_edge *e, *etmp;
  int i = graph->num_vertices;
  int j = 0;
  
  dump = malloc(sizeof(t_graph));
  if (dump == NULL) {
    fprintf(stderr, "Errore: malloc()\n");
    exit(2);
  }
  dump->is_directed = graph->is_directed; 
  
  if (graph->is_directed == FALSE)     
    puts(DOT_GRAPH_HEADER);
  else
    puts(DOT_DGRAPH_HEADER);
  
  v = graph->vertices;
  
  /* Grafo non orientato */
  if (graph->is_directed == FALSE)
  {
    /* Inserisco i nodi di graph nel grafo temporaneo se NON gia`  presenti! */ 
    while(v != NULL) {
    if (isInGraph(dump, v->id) == -1 )
      dv = addVertex(dump, v->id);
    else dv = getVertex(dump, v->id);
       
    e = v->edges;
      while (e != NULL) {
        if (isInGraph(dump, e->links->id) == -1)
          dw = addVertex(dump, e->links->id);
        else
          dw = getVertex(dump, e->links->id);
        
        /* Copio il lato originale a meno che non ne esista gia` uno uguale ma diretto in senso inverso. 
         * Quindi lo stampo. */
        
        if (isConnectedTo(dw, dv) == NULL){
          addEdge(dump, dv, dw, (int) e->weight);
          printf("\"%d\"", dv->id);
          printf (" -- ");
          printf("\"%d\"\n", dw->id);
        }
        e = (e)->next;
      }
      v = (v)->next;
    }
      /* Grafo orientato */ 
  }
  else {
    while (v != NULL) {
      e = v->edges;
      while (e != NULL) {
        printf("\"%d\"", v->id);
        printf (" -> ");
        printf("\"%d\"\n", e->links->id);
        e = (e)->next;
      }
      v = (v)->next;
    }
  }
  puts(DOT_GRAPH_FOOTER);
}   

void dumpGraph(t_graph *graph)
{
  t_vertex *v;
  t_edge *e;
  v = graph->vertices;
  while (v != NULL)
  {
    printf("%d ", v->id);
    e = v->edges;
    while (e != NULL) {
      printf ("-> ");
      printf("%d ", e->links->id);
      e = (e)->next;
    }
    printf("\n");
    v = (v)->next;
  }
      
}
