#include <stdio.h>
#include <stdlib.h>

#define true 't'
#define false 'f'
#define MAX_WEIGHT 999999;
typedef char bool;

typedef struct neighbour
{
    int index;
    int weight;
    struct neighbour *next;
} NEIGHBOURS;

typedef struct vertex
{
    bool visited;
    int current_weight;
    NEIGHBOURS *neighbours;
} VERTEX;

void print(VERTEX **graph, int vertex)
{
    printf("Vertex %d: [", vertex);
    NEIGHBOURS *current = graph[vertex]->neighbours;
    while (current != NULL)
    {
        printf("%d,", current->index);
        current = current->next;
    }
    printf("]\n");
}

void add_edge(VERTEX **graph, int vertex1, int vertex2, int weight, bool first_one)
{
    NEIGHBOURS *neighbour = (NEIGHBOURS *) malloc(sizeof(NEIGHBOURS));
    neighbour->index = vertex2;
    neighbour->weight = weight;
    neighbour->next = NULL;
    if (graph[vertex1]->neighbours == NULL)
    {
        graph[vertex1]->neighbours = neighbour;
    }
    else
    {
        NEIGHBOURS *current = graph[vertex1]->neighbours;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = neighbour;
    }
    if (first_one == true)
    {
        add_edge(graph, vertex2, vertex1, weight, false);
    }
}


void dijkstra(VERTEX **graph, int starting_vertex, int end_point, int N)
{
    int total_weight = 0;
    int min_index = -1;
    for (int i = 0; i < N; i++)
    {
        graph[i]->current_weight = MAX_WEIGHT;
        graph[i]->visited = false;
    }
    graph[starting_vertex]->current_weight = 0;

    NEIGHBOURS *current = graph[starting_vertex]->neighbours;
    int index = starting_vertex;
    if (graph[current->index]->visited == false)
    {
        if (current->weight + graph[index]->current_weight < graph[current->index]->current_weight)
        {
            graph[current->index]->current_weight = current->weight + graph[index]->current_weight;
            min_index = current->index;
        }
    }


}

int main()
{
    int N, M;
    int vertex1, vertex2, weight;
    char input;
    scanf("%d %d", &N, &M);
    VERTEX **graph = (VERTEX **) malloc(N * sizeof(VERTEX *));
    for (int i = 0; i < N; i++)
    {
        VERTEX *newVertex = (VERTEX *) malloc(sizeof(VERTEX));
        newVertex->visited = false;
        newVertex->neighbours = NULL;
        newVertex->current_weight = MAX_WEIGHT;
        graph[i] = newVertex;
    }
    for (int i = 0; i < M; i++)
    {
        scanf(" (%d, %d, %d)", &vertex1, &vertex2, &weight);
        add_edge(graph, vertex1, vertex2, weight, true);
    }
    while (scanf("%c", &input) == 1)
    {
        switch (input)
        {
            case 's':
                scanf(" %d %d", &vertex1, &vertex2);
                dijkstra(graph, vertex1, vertex2, N);
                break;
            case 'd':

                break;
            case 'i':

                break;
            case 'u':

                break;
            case 'p':
                scanf("%d", &vertex1);
                print(graph, vertex1);
                break;
            default:
                break;
        }
    }

    return 0;
}
