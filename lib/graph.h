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


#ifndef _graph_h
#define _graph_h

#define TRUE        0
#define FALSE       1

typedef unsigned int v_tag;

typedef struct tvertex 
{
  v_tag id; // id del vertice

  struct tedge *edges; // informazioni sui lati correlati al grafo
  struct tvertex *next; // vertice successivo nella lista linkata
  int visited;
  struct cbData *data;
  
} t_vertex;

typedef struct tedge 
{
  int weight; // Peso del lato, 1 per grafi non pesati
  struct tvertex *links; // punta al vertice collegato
  struct tedge *next;
} t_edge;

typedef struct tgraph
{
  int num_vertices;
  int is_directed;
  struct tvertex *vertices;
} t_graph;


/* Controlla se v e w sono collegati da un lato. In caso positivo ritorna
 * una puntatore ad esso */
t_edge *isConnectedTo(t_vertex *v, t_vertex *w);

/* Aggiunge un nuovo vertice con nome 'id' al grafo graph */
t_vertex *addVertex(t_graph *graph, v_tag id);

/* Crea un lato da srcVertex a dstVertex */
int addEdge(t_graph *graph, t_vertex *srcVertex, t_vertex *dstVertex, int weight);

/* Preleva dal grafo 'graph' il vertice chiamato 'id'. Se tale vertice non
 * esiste la funzione ritorna NULL */
t_vertex *getVertex(t_graph *graph, v_tag id);

/* Controlla se il vertice 'id' fa parte del grafo. In caso affermativo,
 * ritorna 0, altrimenti ritorna -1*/
int isInGraph(t_graph *graph, v_tag id); 

/* Dealloca la memoria dedicata al grafo */
void destroyGraph(t_graph *graph);

/* Rimuove un nodo e tutti gli archi a lui associati */
void removeVertex(t_graph *graph, t_vertex *v);

/* Rimuove un lato e aggiorna la lista dei puntatori del vertice sorgente */
void removeEdge(t_graph *graph, t_vertex *src, t_vertex *dst);

/* Alloca memoria per il grafo */
t_graph *createGraph(int is_directed);



#endif
