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
    int best_index;
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
    // Initialize distances and visited array
    for (int i = 0; i < N; i++)
    {
        graph[i]->current_weight = MAX_WEIGHT;
        graph[i]->visited = false;
    }

    // Set the distance of the starting vertex to 0
    graph[starting_vertex]->current_weight = 0;

    // Array to store visited vertices
    int visited_vertices[N];
    int visited_count = 0;

    // Main loop for Dijkstra's algorithm
    for (int count = 0; count < N - 1; count++)
    {
        // Find the vertex with the minimum current_weight among unvisited vertices
        int min_index = -1;
        int min_weight = MAX_WEIGHT;
        for (int i = 0; i < N; i++)
        {
            if (graph[i]->visited == false && graph[i]->current_weight < min_weight)
            {
                min_index = i;
                min_weight = graph[i]->current_weight;
            }
        }

        // If no minimum weight vertex is found, break
        if (min_index == -1)
            break;

        // Mark the selected vertex as visited
        graph[min_index]->visited = true;

        // Update distances to its neighbors
        NEIGHBOURS *current = graph[min_index]->neighbours;
        while (current != NULL)
        {
            int neighbor_index = current->index;
            int neighbor_weight = current->weight;
            if (!graph[neighbor_index]->visited &&
                graph[min_index]->current_weight + neighbor_weight < graph[neighbor_index]->current_weight)
            {
                graph[neighbor_index]->current_weight = graph[min_index]->current_weight + neighbor_weight;
            }
            current = current->next;
        }

        // Store the visited vertex
        visited_vertices[visited_count++] = min_index;
    }

    // Print shortest distance to end point
    printf("Shortest distance from %d to %d is %d\n", starting_vertex, end_point, graph[end_point]->current_weight);

    // Print visited vertices
    printf("Visited vertices: ");
    for (int i = 0; i < visited_count; i++)
    {
        printf("%d ", visited_vertices[i]);
    }
    printf("\n");
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
