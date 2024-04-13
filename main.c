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
        printf("%d: [weight: %d],", current->index, current->weight);
        current = current->next;
    }
    printf("]\n");
}

int update(VERTEX **graph, int vertex1, int vertex2, int weight, bool first_one, int N)
{
    if (vertex1 > N - 1 || vertex2 > N - 1)
    {
        return 1;
    }
    NEIGHBOURS *current = graph[vertex1]->neighbours;
    while (current != NULL)
    {
        if (current->index == vertex2)
        {
            break;
        }
        current = current->next;
    }
    if (current == NULL)
    {
        return 1;
    }
    int newWeight = current->weight + weight;
    if (newWeight < 0)
    {
        return 1;
    }
    else
    {
        current->weight = newWeight;
    }

    if (first_one == true)
    {
        update(graph, vertex2, vertex1, weight, false, N);
    }
    return 0;
}

int add_edge(VERTEX **graph, int vertex1, int vertex2, int weight, bool first_one, int N)
{
    if (vertex1 > N - 1 || vertex2 > N - 1)
    {
        return 1;
    }
    NEIGHBOURS *current = graph[vertex1]->neighbours;
    while (current != NULL)
    {
        if (current->index == vertex2)
        {
            return 1;
        }
        current = current->next;
    }
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
        add_edge(graph, vertex2, vertex1, weight, false, N);
    }
    return 0;
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
    }
}


int main()
{
    int N, M;
    int vertex1, vertex2, weight;
    char input;
    char printed = false;
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
        if (add_edge(graph, vertex1, vertex2, weight, true, N) == 1)
        {
            if (printed == false)
            {
                printf("insert %d %d failed", vertex1, vertex2);
                printed = true;
            }
            else
            {
                printf("\ninsert %d %d failed", vertex1, vertex2);
            }
        }
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
                scanf(" %d %d", &vertex1, &vertex2);

                break;
            case 'i':
                scanf("%d %d %d", &vertex1, &vertex2, &weight);
                if (add_edge(graph, vertex1, vertex2, weight, true, N) == 1)
                {
                    if (printed == false)
                    {
                        printf("insert %d %d failed", vertex1, vertex2);
                        printed = true;
                    }
                    else
                    {
                        printf("\ninsert %d %d failed", vertex1, vertex2);
                    }
                }
                break;
            case 'u':
                scanf("%d %d %d", &vertex1, &vertex2, &weight);
                if (update(graph, vertex1, vertex2, weight, true, N) == 1)
                {
                    if (printed == false)
                    {
                        printf("update %d %d failed", vertex1, vertex2);
                        printed = true;
                    }
                    else
                    {
                        printf("\nupdate %d %d failed", vertex1, vertex2);
                    }
                }
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
