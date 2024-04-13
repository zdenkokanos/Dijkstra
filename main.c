#include <stdio.h>
#include <stdlib.h>

#define true 't'
#define false 'f'
#define MAX_WEIGHT 999999
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

int delete(VERTEX **graph, int vertex1, int vertex2, bool first_one)
{
    NEIGHBOURS *current = graph[vertex1]->neighbours;
    NEIGHBOURS *previous = NULL;
    while (current != NULL)
    {
        if (current->index == vertex2)
        {
            break;
        }
        previous = current;
        current = current->next;
    }
    if (current == NULL)
    {
        return 1;
    }
    if (current == graph[vertex1]->neighbours)
    {
        graph[vertex1]->neighbours = current->next;
        free(current);
    }
    else
    {
        previous->next = current->next;
        free(current);
    }
    if (first_one == true)
    {
        delete(graph, vertex2, vertex1, false);
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


int dijkstra(VERTEX **graph, int starting_vertex, int end_point, int N)
{
    // Initialization
    for (int i = 0; i < N; i++)
    {
        graph[i]->current_weight = MAX_WEIGHT;
        graph[i]->visited = false;
        graph[i]->best_index = -1;
    }

    graph[starting_vertex]->current_weight = 0;

    int visitedCount = 0;

    while (visitedCount != N)
    {
        int smallest_weight = MAX_WEIGHT;
        int smallest_index = -1;
        for (int i = 0; i < N; i++)
        {
            if (graph[i]->current_weight < smallest_weight && !graph[i]->visited)
            {
                smallest_weight = graph[i]->current_weight;
                smallest_index = i;
            }
        }
        if (smallest_index == -1)
        {
            return 1;
        }
        graph[smallest_index]->visited = true;
        visitedCount++;

        NEIGHBOURS *current = graph[smallest_index]->neighbours;
        while (current != NULL)
        {
            if (!graph[current->index]->visited)
            {
                int new_weight = graph[smallest_index]->current_weight + current->weight;
                if (new_weight < graph[current->index]->current_weight)
                {
                    graph[current->index]->current_weight = new_weight;
                    graph[current->index]->best_index = smallest_index;
                }
            }
            current = current->next;
        }
    }

    VERTEX *starting = graph[end_point];
    int route[N];
    for (int i = 0; i < N; i++)
    {
        route[i] = -1;
    }
    int i = 0;
    int index = end_point;
    int totalWeight = 0;
    while (starting != graph[starting_vertex])
    {
        route[i] = graph[index]->best_index;
        totalWeight += graph[index]->current_weight;
        starting = graph[graph[index]->best_index];
        i++;
    }
    printf("\n%d: [", totalWeight);
    for (int j = i - 1; j >= 0; j--)
    {
        printf("%d", route[j]);
        if (j > 0) printf(", ");
    }
    printf("]");

    return 0;
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
                if (dijkstra(graph, vertex1, vertex2, N) == 1)
                {
                    if (printed == false)
                    {
                        printf("search %d %d failed", vertex1, vertex2);
                        printed = true;
                    }
                    else
                    {
                        printf("\nsearch %d %d failed", vertex1, vertex2);
                    }
                }
                break;
            case 'd':
                scanf(" %d %d", &vertex1, &vertex2);
                if (delete(graph, vertex1, vertex2, true) == 1)
                {
                    if (printed == false)
                    {
                        printf("delete %d %d failed", vertex1, vertex2);
                        printed = true;
                    }
                    else
                    {
                        printf("\ndelete %d %d failed", vertex1, vertex2);
                    }
                }
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
