/*
  This is my C99 implementation of Dijkstra's shortest path algorithm 
  written in the year 2007.

  Copyright (C) 2007 Thomas Trapp; mailto: git@thomastrapp.com

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, 
  Boston, MA  02110-1301, USA.

  See LICENSE for details.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_NEIGHBORS_PER_NODE 5
#define LEN_IDENTIFIER 4
#define MAX_NODES 30
#define MAX_NEIGHBORS MAX_NEIGHBORS_PER_NODE * MAX_NODES

#define MARK_PERM 0
#define MARK_TEMP 1

#define INFINITY 99999

struct NEIGH
{
  struct KNOT * pos;
  int distance;
} neighbors[MAX_NEIGHBORS];

struct KNOT
{
  char name[LEN_IDENTIFIER];
  int neigh_count;
  int index;
  struct NEIGH * neighbors[MAX_NEIGHBORS_PER_NODE];
} graph[MAX_NODES];

/*** debug helpers ***/

void print_node(struct KNOT * node)
{
  int i;

  printf("Name: %s\n", node->name);
  printf("Index: %d\n", node->index);
  printf("Neighbors: %d\n", node->neigh_count);
  printf("   ");
  for(i = 0; i < node->neigh_count; i++)
    printf("%s (%d), ", node->neighbors[i]->pos->name, node->neighbors[i]->distance);
  printf("\n");
}

void print_graph(struct KNOT graph[], int node_count)
{
  int i;

  for(i = 0; i < node_count; i++)
  {
    printf("============================================\n");
    printf("NODE NUMBER %d\n", i);
    printf("============================================\n\n");
    print_node(&graph[i]);
    printf("\n");
  }
}

void print_int_array(int array[], int sizeof_array)
{
  int i;
  for(i = 0; i < sizeof_array; i++)
  {
    printf("%d: %d\n", i, array[i]);
  }
}

/*** nodes ***/

struct KNOT * get_node_by_name(struct KNOT graph[], int node_count, char name[])
{
  int i;
  if( strlen(name) == LEN_IDENTIFIER-1 )
  {
    for(i = 0; i < node_count; i++)
    {
      if( strcmp(graph[i].name, name) == 0 )
      {
        return &(graph[i]);
      }
    }
  }
  return (struct KNOT *) 0;
}

struct NEIGH * get_neighbor_from_node_by_name(struct KNOT node, char name[])
{
  int i;
  for(i = 0; i < node.neigh_count; i++)
  {
    if( strcmp(node.neighbors[i]->pos->name, name) == 0 )
    {
      return node.neighbors[i];
    }
  }

  return (struct NEIGH *) 0;
}

int add_node(struct KNOT graph[], int * node_count, char name[])
{
  if( *node_count < MAX_NODES )
  {
    if( strlen(name) == LEN_IDENTIFIER-1 )
    {
      if( !get_node_by_name(graph, *node_count, name) )
      {
        strcpy(graph[*node_count].name, name);
        graph[*node_count].neigh_count = 0;
        graph[*node_count].index = *node_count;
        (*node_count)++;
        
        return 1;
      }
    }
  }

  return 0;
}

int set_distance(struct KNOT graph[], struct NEIGH neighbors[], int node_count, int * neigh_count, char name1[], char name2[], int distance)
{
  struct KNOT * node1 = NULL;
  struct KNOT * node2 = NULL;
  struct NEIGH * neigh1 = NULL;
  struct NEIGH * neigh2 = NULL;

  if( strlen(name1) == LEN_IDENTIFIER-1 && strlen(name2) == LEN_IDENTIFIER-1 )
  {
    if( name1 != name2 && distance > 0 )
    {
      // Does node exist?
      if( (node1 = get_node_by_name(graph, node_count, name1)) != (struct KNOT *) 0 )
      {
        if( (node2 = get_node_by_name(graph, node_count, name2)) != (struct KNOT *) 0 )
        {
          // Can we add a neighbor?
          if( node1->neigh_count < MAX_NEIGHBORS_PER_NODE && node2->neigh_count < MAX_NEIGHBORS_PER_NODE )
          {
            // NODE1
            neigh1 = get_neighbor_from_node_by_name(*node1, name2);
            if( neigh1 == (struct NEIGH *) 0 )
            {
              neighbors[*neigh_count].pos = node2;
              neighbors[*neigh_count].distance = distance;
              // Link neighbor to node
              node1->neighbors[node1->neigh_count] = &neighbors[*neigh_count];
              node1->neigh_count += 1;
              *neigh_count += 1;
            }
            // if neighbor does already exist update distance
            else 
            {
              neigh1->distance = distance;
            }

            // NODE2
            neigh2 = get_neighbor_from_node_by_name(*node2, name1);
            if( neigh2 == (struct NEIGH *) 0 )
            {
              neighbors[*neigh_count].pos = node1;
              neighbors[*neigh_count].distance = distance;
              // Link neighbor to node
              node2->neighbors[node2->neigh_count] = &neighbors[*neigh_count];
              node2->neigh_count += 1;
              *neigh_count += 1;
            }
            else
            {
              neigh2->distance = distance;
            }
            
            return 1;
          }
        }
      }
    }
  }
  
  return 0;
}

/*** shortest path ***/

struct KNOT * get_node_by_index(struct KNOT * graph, int node_count, int index)
{
  int i;
  if( index < node_count )
  {
    for(i = 0; i < node_count; i++)
    {
      if( graph[i].index == index )
      {
        return &(graph[i]);
      }
    }
  }

  return NULL;
}

int get_index_shortest_temp_dist(int node_distance[], int node_mark[], int node_count)
{
  int i;
  int index = INFINITY;

  for(i = 0; i < node_count; i++)
  {
    if( node_mark[i] == MARK_TEMP && node_distance[i] < index )
      index = i;
  }

  return index;
}

int perm_neigh_has_temp_distance(int node_mark[], struct KNOT graph[], int node_count)
{
  int i, h;
  struct KNOT * node = NULL;

  for(i = 0; i < node_count; i++)
  {
    if( node_mark[i] == MARK_PERM )
    {
      node = get_node_by_index(graph, node_count, i);
      for(h = 0; h < node->neigh_count; h++)
      {
        if( node_mark[node->neighbors[h]->pos->index] == MARK_TEMP )
          return 1;
      }
    }
  }
  
  return 0;
}

int dijkstras_algorithm(struct KNOT graph[], int node_count, char name_start[], char name_end[])
{
  int i;
  int node_distance[node_count];
  int node_mark[node_count];
  struct KNOT * node_start = NULL;
  struct KNOT * node_end = NULL;
  struct KNOT * node_temp_final = NULL;
  struct NEIGH * curr_neigh = NULL;
  
  node_start = get_node_by_name(graph, node_count, name_start);
  node_end = get_node_by_name(graph, node_count, name_end);

  for(i = 0; i < node_count; i++)
  {
    node_distance[i] = INFINITY;
  }
  node_distance[node_start->index] = 0;

  for(i = 0; i < node_count; i++)
  {
    node_mark[i] = MARK_TEMP;
  }
  node_mark[node_start->index] = MARK_PERM;

  node_temp_final = node_start;

  while( perm_neigh_has_temp_distance(node_mark, graph, node_count) )
  {
    for(i = 0; i < node_temp_final->neigh_count; i++)
    {
      curr_neigh = node_temp_final->neighbors[i];
      if( node_mark[curr_neigh->pos->index] == MARK_TEMP )
      {
        if( node_distance[node_temp_final->index] + curr_neigh->distance < node_distance[curr_neigh->pos->index] )
          node_distance[curr_neigh->pos->index] = node_distance[node_temp_final->index] + curr_neigh->distance;
      }
    }

    node_temp_final = get_node_by_index(graph, node_count, get_index_shortest_temp_dist(node_distance, node_mark, node_count));
    node_mark[node_temp_final->index] = MARK_PERM;
  }

  return node_distance[node_end->index];
}

void print_shortest_paths_possible(struct KNOT grap[], int node_count)
{
  int i, h;
  for(i = 0; i < node_count; i++)
  {
    for(h = 0; h < node_count; h++)
    {
      if( i != h )
      {
        printf("%s->%s:\t%d\n", graph[i].name, graph[h].name, dijkstras_algorithm(graph, node_count, graph[i].name, graph[h].name));
      }
    }
  }
}

int main()
{
  int node_count = 0;
  int neigh_count = 0;

  // Add nodes
  if( !add_node(graph, &node_count, "VLV") )
    printf("ERROR: Could not add node number %d\n", node_count);
  if( !add_node(graph, &node_count, "SPC") )
    printf("ERROR: Could not add node number %d\n", node_count);
  if( !add_node(graph, &node_count, "TFN") )
    printf("ERROR: Could not add node number %d\n", node_count);
  if( !add_node(graph, &node_count, "GOM") )
    printf("ERROR: Could not add node number %d\n", node_count);
  if( !add_node(graph, &node_count, "LPA") )
    printf("ERROR: Could not add node number %d\n", node_count);
  if( !add_node(graph, &node_count, "ACE") )
    printf("ERROR: Could not add node number %d\n", node_count);
  if( !add_node(graph, &node_count, "FUE") )
    printf("ERROR: Could not add node number %d\n", node_count);

  // Set distances
  if( !set_distance(graph, neighbors, node_count, &neigh_count, "VLV", "SPC", 95) )
    printf("ERROR: Could not set distance\n");
  if( !set_distance(graph, neighbors, node_count, &neigh_count, "SPC", "TFN", 147) )
    printf("ERROR: Could not set distance\n");
  if( !set_distance(graph, neighbors, node_count, &neigh_count, "TFN", "GOM", 98) )
    printf("ERROR: Could not set distance\n");
  if( !set_distance(graph, neighbors, node_count, &neigh_count, "TFN", "LPA", 95) )
    printf("ERROR: Could not set distance\n");
  if( !set_distance(graph, neighbors, node_count, &neigh_count, "TFN", "FUE", 253) )
    printf("ERROR: Could not set distance\n");
  if( !set_distance(graph, neighbors, node_count, &neigh_count, "GOM", "LPA", 198) )
    printf("ERROR: Could not set distance\n");
  if( !set_distance(graph, neighbors, node_count, &neigh_count, "LPA", "FUE", 155) )
    printf("ERROR: Could not set distance\n");
  if( !set_distance(graph, neighbors, node_count, &neigh_count, "FUE", "ACE", 65) )
    printf("ERROR: Could not set distance\n");

  print_shortest_paths_possible(graph, node_count);
  
  return EXIT_SUCCESS;
}

