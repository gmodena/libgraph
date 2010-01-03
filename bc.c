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

#include <unistd.h>
#include <stdio.h>

#include "lib/graph.h"
#include "lib/stack.h"
#include "lib/centrality.h"
#include "lib/graphio.h"

void help(void) 
{
  printf("\nUsage: \n");
  printf("-b : compute betwenness centrality index and print results on screen \n");
  printf("-f : load a graph from a CSV file \n");
  printf("-o : oriented graph \n");
  printf("-d : dump the graph output in DOT format \n\n");
  printf("Example: \n");
  printf("cb -o -d -f net.csv\n ");
    

}

int main(int argc, char **argv)
{
  t_vertex *v1, *v2, *v3, *v4, *v5, *v6, *v7, *v8, *v9, *v10;
  t_vertex *w;
  t_edge *e;
  t_graph *graph;
  int V = 0;
  t_stack stack;
  t_queue queue;
  int i=0;
  int dot = 0;
  int bc = 0;
  int is_directed = FALSE;
  char *file = NULL;
  char *num_vertices = NULL;
  int index;
  int f;
  int v;
  int c;
  int o;
     
  
  graph = createGraph(TRUE);
  
  while ((f = getopt (argc, argv, "odhbf:v:")) != -1)
    switch (f)
      {
      case 'd':
	dot = 1;
	break;
      case 'b':
	bc = 1;
	break;
      case 'o':
        is_directed = TRUE;
        break;
      case 'f':
	file = optarg;
	break;
      case 'h':
        help();
        break;
      case 'v':
        num_vertices = optarg;
        break;
      case '?':
	if (isprint (optopt)) {
	  fprintf (stderr, "Unknown option: `-%c'.\n", optopt);
          help();
        }
	else {
	  fprintf (stderr,
		   "Invalid option `\\x%x'.\n",
		   optopt);
          help();
        }
	return 1;
      default:
	 abort();
      }
     
  for (index = optind; index < argc; index++)
    printf ("Non-option argument %s\n", argv[index]);    
 
  
  graph = createGraph(is_directed);
  
  if (num_vertices != NULL){
    V = atoi(num_vertices) + 1;
    for (i = 1; i < V; i++)
      addVertex(graph, i);
      
  }
  
  if (file != NULL)
    readFromCSV(graph, file);

  if (bc == 1) {
    betweennessCentrality(graph);
    printGraphCB(graph);
  }

  if (dot == 1) 
    dumpDotGraph(graph);

  destroyCBData(graph);
  destroyGraph(graph);    
    
  return 0;
}
