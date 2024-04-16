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

typedef struct node
{
    int index;
    int weight;
} PQ_E;

typedef struct pq
{
    PQ_E *heap;
    int capacity;
    int size;
} PQ;

void swap(PQ_E *a, PQ_E *b)
{
    PQ_E temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(PQ *priority_queue, int index)
{
    if (index <= 0)
    { return; }

    int parent = (index - 1) / 2;
    if (priority_queue->heap[parent].weight > priority_queue->heap[index].weight)
    {
        swap(&priority_queue->heap[parent], &priority_queue->heap[index]);
        heapify_up(priority_queue, parent);
    }
}

void heapify_down(PQ *priority_queue, int index)
{
    int left_child = 2 * index + 1;
    int right_child = 2 * index + 2;
    int smallest = index;

    if (left_child < priority_queue->size &&
        priority_queue->heap[left_child].weight < priority_queue->heap[smallest].weight)
    {
        smallest = left_child;
    }

    if (right_child < priority_queue->size &&
        priority_queue->heap[right_child].weight < priority_queue->heap[smallest].weight)
    {
        smallest = right_child;
    }

    if (smallest != index)
    {
        swap(&priority_queue->heap[index], &priority_queue->heap[smallest]);
        heapify_down(priority_queue, smallest);
    }
}

void insert(PQ *priority_queue, int index, int weight)
{
    priority_queue->heap[priority_queue->size].index = index;
    priority_queue->heap[priority_queue->size].weight = weight;
    priority_queue->size++;
    heapify_up(priority_queue, priority_queue->size - 1);
}

PQ_E extract_min(PQ *priority_queue)
{
    if (priority_queue->size <= 0)
    {
        printf("Priority queue underflow\n");
        PQ_E empty;
        empty.index = -1;
        empty.weight = -1;
        return empty;
    }

    PQ_E min = priority_queue->heap[0];
    priority_queue->heap[0] = priority_queue->heap[priority_queue->size - 1];
    priority_queue->size--;
    heapify_down(priority_queue, 0);
    return min;
}

int is_empty(PQ *priority_queue)
{
    return priority_queue->size == 0;
}

void destroy_priority_queue(PQ *priority_queue)
{
    free(priority_queue->heap);
    free(priority_queue);
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


int dijkstra(VERTEX **graph, int starting_vertex, int end_point, int N, bool *printed, PQ *priorityQueue)
{
    int distances[N];
    int predecessors[N];
    bool visited[N];


    for (int i = 0; i < N; i++)
    {
        distances[i] = MAX_WEIGHT;
        predecessors[i] = -1;
        visited[i] = false;
    }

    distances[starting_vertex] = 0;

    while (1)
    {
        int min_distance = MAX_WEIGHT;
        int min_index = -1;

        for (int vertex = 0; vertex < N; vertex++)
        {
            if (visited[vertex] == false && distances[vertex] < min_distance &&
                distances[vertex] < distances[end_point])
            {
                min_distance = distances[vertex];
                min_index = vertex;
            }
        }

        if (min_index == -1)
        {
            break;
        }

        visited[min_index] = true;

        NEIGHBOURS *neighbour = graph[min_index]->neighbours;
        while (neighbour != NULL)
        {
            int neighbour_index = neighbour->index;
            int weight = neighbour->weight;

            if (distances[min_index] != MAX_WEIGHT && distances[min_index] + weight < distances[neighbour_index])
            {
                distances[neighbour_index] = distances[min_index] + weight;
                predecessors[neighbour_index] = min_index;
            }

            neighbour = neighbour->next;
        }
    }

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
    PQ *priorityQueue = (PQ *) malloc(sizeof(PQ));
    priorityQueue->capacity = N;
    priorityQueue->size = 0;
    priorityQueue->heap = (PQ_E *) malloc(N * sizeof(PQ_E));
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
                if (dijkstra(graph, vertex1, vertex2, N, &printed, priorityQueue) == 1)
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