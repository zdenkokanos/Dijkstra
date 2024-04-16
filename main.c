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
    NEIGHBOURS *neighbours;
} VERTEX;

typedef struct pq_e
{
    int index;
    int weight;
} PQ_E;

typedef struct pq
{
    PQ_E *heap;
    int size;
} PQ;

PQ *create_priorityQueue(int capacity)
{
    PQ *priorityQueue = (PQ *) malloc(sizeof(PQ));
    priorityQueue->heap = (PQ_E *) malloc(2 * capacity * sizeof(PQ_E));
    priorityQueue->size = 0;
    return priorityQueue;
}

void swap(PQ_E *a, PQ_E *b)
{
    PQ_E temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(PQ *priorityQueue, int index)
{
    if (index <= 0)
    {
        return;
    }

    int parent = (index - 1) / 2;
    if (priorityQueue->heap[parent].weight > priorityQueue->heap[index].weight)
    {
        swap(&priorityQueue->heap[parent], &priorityQueue->heap[index]);
        heapify_up(priorityQueue, parent);
    }
}

void heapify_down(PQ *priorityQueue, int index)
{
    int left_child = 2 * index + 1;
    int right_child = 2 * index + 2;
    int smallest = index;

    if (left_child < priorityQueue->size &&
        priorityQueue->heap[left_child].weight < priorityQueue->heap[smallest].weight)
    {
        smallest = left_child;
    }

    if (right_child < priorityQueue->size &&
        priorityQueue->heap[right_child].weight < priorityQueue->heap[smallest].weight)
    {
        smallest = right_child;
    }

    if (smallest != index)
    {
        swap(&priorityQueue->heap[index], &priorityQueue->heap[smallest]);
        heapify_down(priorityQueue, smallest);
    }
}

void insert(PQ *priorityQueue, int index, int weight)
{
    priorityQueue->heap[priorityQueue->size].index = index;
    priorityQueue->heap[priorityQueue->size].weight = weight;
    priorityQueue->size++;
    heapify_up(priorityQueue, priorityQueue->size - 1);
}

PQ_E extract_min(PQ *priorityQueue)
{
    PQ_E min = priorityQueue->heap[0];
    priorityQueue->heap[0] = priorityQueue->heap[priorityQueue->size - 1];
    priorityQueue->size--;
    heapify_down(priorityQueue, 0);
    return min;
}

int is_empty(PQ *priorityQueue)
{
    return priorityQueue->size == 0;
}

void destroy_priorityQueue(PQ *priorityQueue)
{
    free(priorityQueue->heap);
    free(priorityQueue);
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
        current = graph[vertex1]->neighbours;
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


int dijkstra(VERTEX **graph, int starting_vertex, int end_point, int N, bool *printed)
{
    int distances[N];
    int predecessors[N];
    bool visited[N];
    PQ *priorityQueue = create_priorityQueue(N);

    for (int i = 0; i < N; i++)
    {
        distances[i] = MAX_WEIGHT;
        predecessors[i] = -1;
        visited[i] = false;
    }

    distances[starting_vertex] = 0;
    insert(priorityQueue, starting_vertex, 0);

    while (!is_empty(priorityQueue))
    {
        PQ_E min = extract_min(priorityQueue);
        int min_index = min.index;
        visited[min_index] = true;

        NEIGHBOURS *neighbour = graph[min_index]->neighbours;
        while (neighbour != NULL)
        {
            int neighbour_index = neighbour->index;
            int weight = neighbour->weight;

            if (visited[neighbour_index] == false && distances[min_index] != MAX_WEIGHT &&
                distances[min_index] + weight < distances[neighbour_index])
            {
                distances[neighbour_index] = distances[min_index] + weight;
                predecessors[neighbour_index] = min_index;
                insert(priorityQueue, neighbour_index, distances[neighbour_index]);
            }

            neighbour = neighbour->next;
        }
    }
    destroy_priorityQueue(priorityQueue);
    int path[N];
    int path_length = 0;
    int current_vertex = end_point;

    while (current_vertex != starting_vertex)
    {
        path[path_length++] = current_vertex;
        current_vertex = predecessors[current_vertex];
        if (current_vertex == -1)
        {
            return 1; // No path found
        }
    }
    path[path_length++] = starting_vertex;

    if (*printed == false)
    {
        printf("%d: [", distances[end_point]);
        *printed = true;
    }
    else
    {
        printf("\n%d: [", distances[end_point]);
    }
    for (int i = path_length - 1; i >= 0; i--)
    {
        printf("%d", path[i]);
        if (i > 0)
        {
            printf(", ");
        }
    }
    printf("]");
    return 0;
}


int main()
{
    int N, M;
    int vertex1, vertex2, weight;
    char input;
    bool printed = false;
    scanf("%d %d", &N, &M);
    VERTEX **graph = (VERTEX **) malloc(N * sizeof(VERTEX *));
    for (int i = 0; i < N; i++)
    {
        VERTEX *newVertex = (VERTEX *) malloc(sizeof(VERTEX));
        newVertex->neighbours = NULL;
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
                if (dijkstra(graph, vertex1, vertex2, N, &printed) == 1)
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
                scanf(" %d %d %d", &vertex1, &vertex2, &weight);
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
                scanf(" %d %d %d", &vertex1, &vertex2, &weight);
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
            default:
                break;
        }
    }

    return 0;
}