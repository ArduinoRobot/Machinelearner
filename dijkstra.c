#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NIL -1
#define INFINITY 0xffff
/* This code was made by Yang Jin-woo */

struct edge
{
    int vertex;
    int weight;
};
typedef struct edge edge;

edge create_edge(int vertex, int weight)
{
    edge e;
    e.vertex = vertex;
    e.weight = weight;
    return e;
}

struct heap
{
    edge * arr;
    int size;
};
typedef struct heap heap;

heap * create_heap(int max_size)
{
    heap * h = (heap *) malloc(sizeof(heap));
    h->arr = (edge *) malloc(sizeof(edge) * max_size);
    h->size = 0;
    int i;
    for(i = 0;i<max_size;i++)
        h->arr[i].vertex = 0, h -> arr[i].weight = 0;
    return h;
}

bool empty_heap(heap * h)
{
    return h->size == 0;
}

void swap(edge * nb1,edge * nb2)
{
    edge tmp = * nb1;
    *nb1 = *nb2;
    *nb2 = tmp;
}

int max(int nb1,int nb2)
{
    return nb1 > nb2 ? nb1 : nb2;
}

int min(int nb1,int nb2)
{
    return nb1 < nb2 ? nb1 : nb2;
}

void insert_max_heap(heap *h, edge value)
{
    h->arr[++h->size] = value;
    int child_index = h->size;
    while(child_index > 1 && h->arr[child_index].weight > h->arr[child_index / 2].weight)
    {
        swap(&h->arr[child_index], &h->arr[child_index/2]);
    }
}

void insert_min_heap(heap * h, edge value)
{
    h->arr[++h->size] = value;
    int child_index = h->size;
    while(child_index > 1 && h->arr[child_index].weight < h->arr[child_index/2].weight)
    {
        swap(&h->arr[child_index], &h->arr[child_index/2]);
    }

}

edge extract_max_heap(heap *h)
{
    edge e= {0,0};
    if(empty_heap(h))
    {
        printf("Heap is full!\n");
        return e;
    }
    edge value = h->arr[1];
    h->arr[1] = h->arr[h->size];
    h->size--;
    int parent_index = 1;
    int left_child_index;
    int right_child_index;
    do
    {
        left_child_index = parent_index * 2;
        right_child_index = parent_index * 2 +1;
        if(h->arr[parent_index].weight > max(h->arr[left_child_index].weight, h->arr[right_child_index].weight))
            break;
        if(h->size == left_child_index || h->arr[left_child_index].weight > h->arr[right_child_index].weight)
        {
            swap(&h->arr[parent_index], &h->arr[left_child_index]);
            parent_index = left_child_index;
        }
        else
        {
            swap(&h->arr[parent_index] , &h->arr[right_child_index]);
            parent_index = right_child_index;
        }
    }while(parent_index < h->size);
    return value;
}

edge extract_min_heap(heap * h)
{
    edge e = {0,0};
    if(empty_heap(h))
    {
        printf("Heap is full!\n");
        return e;
    }
    edge value = h->arr[1];
    h->arr[1] = h->arr[h->size];
    h->size--;
    int parent_index = 1;
    int left_child_index;
    int right_child_index;
    do
    {
        left_child_index = parent_index * 2;
        right_child_index = parent_index * 2 +1;
        if(h->arr[parent_index].weight < min(h->arr[left_child_index].weight, h->arr[right_child_index].weight))
            break;
        if(h->size == left_child_index || h->arr[left_child_index].weight > h->arr[right_child_index].weight)
        {
            swap(&h->arr[parent_index], &h->arr[left_child_index]);
            parent_index = left_child_index;
        }
        else
        {
            swap(&h->arr[parent_index] , &h->arr[right_child_index]);
            parent_index = right_child_index;
        }
    }while(parent_index < h->size);
    return value;
}

//Graph Data Structure
struct edge_node
{
    int v;
    int weight;
    struct edge_node * next;
};

typedef struct edge_node edge_node;

struct graph
{
    edge_node * * edges;
    int nb_nodes;
    int nb_edges;
    bool is_directed;
};
typedef struct graph graph;

graph * create_graph(int nb_nodes, bool is_directed)
{
    graph * g = (graph *) malloc(sizeof(graph));
    g->nb_nodes = nb_nodes;
    g->nb_edges = 0;
    g->is_directed = is_directed;
    g->is_directed = is_directed;
    g->edges = (graph * *) malloc(sizeof(graph *) * nb_nodes);
    int u;
    for(u = 0;u < g->nb_nodes;u++)
        g->edges[u] = NULL;
    return g;
}

void print_graph(graph * g)
{
    int u;

    for(u = 0; u< g->nb_nodes;u++)
    {
        printf("%d --> ", u);
        //print all the nodes connected to u
        edge_node * e = g->edges[u];
        while(e != NULL)
        {
            printf("%d", e->v);
        edge_node * e = g->edges[u];
        while(e != NULL)
        {
            printf("%d", e->v);
        }
        }
    }
}

void graph_add_edge(graph * g, int u, int v, int weight)
{
    edge_node * e = (edge_node *)malloc(sizeof(edge_node));
    e->v = v;
    e->weight = weight;
    e->next = g->edges[u];
    g->edges[u] = e;
    if(!g->is_directed)
    {
        e = (edge_node *) malloc(sizeof(edge_node));
        e->v = u;
        e->weight = weight;
        e->next = g->edges[v];
        g->edges[v] = e;
    }
    g->nb_edges++;
}

void dijkstra(graph * g, int source, int destination)
{
    heap * h = create_heap(g->nb_edges);
    int parents[g->nb_nodes];
    int distances[g->nb_nodes];
    int node;
    for(node = 0;node < g->nb_nodes;node++)
    {
        parents[node] = NIL;
        distances[node] = INFINITY;
    }
    edge u = create_edge(source, 0);
    insert_min_heap(h,u);
    distances[source] = 0;
    parents[source] = NIL;
    while(!empty_heap(h))
    {
        u = extract_min_heap(h);
        edge_node * e = g->edges[u.vertex];
        while(e != NULL)
        {
            if(distances[u.vertex] + e->weight < distances[e->v])
            {
                distances[e->v] = distances[u.vertex] + e->weight;
                parents[e->v] = u.vertex;

                insert_min_heap(h,create_edge(e->v,distances[e->v]));
            }
            e = e->next;
        }
    }

    //copy path
    int path[g->nb_nodes];
    int path_size = 0;
    int dest = destination;
    do
    {
        path[path_size++] = dest;
        dest = parents[dest];
    }while(dest != NIL);
    printf("The distance from %d to %d is : ", source, destination, distances[destination]);
    int i;
    for(i = path_size-1;i>=0;i--)
    {
        printf("%d\t", path[i]);
    }
}

int main()
{
    int nb_nodes, nb_edges, source, destination;
    scanf("%d %d %d %d", &nb_nodes, &nb_edges, &source, &destination);
    graph * g = create_graph(nb_nodes+1, false);
    while(nb_edges--)
    {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        graph_add_edge(g, u, v, w);
    }
    dijkstra(g, source, destination);
    return 0;
}

/*
12 17 1 12
1 2 12
1 3 10
2 4 11
2 5 13
3 5 12
3 7 13
4 6 9
5 6 7
6 7 15
6 10 13
6 11 11
7 8 7
8 9 12
8 10 10
9 10 10
10 11 9
11 12 10
*/

