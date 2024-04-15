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
    int vertex;
    int distance;
} NODE;

typedef struct
{
    NODE **heap;
    int size;
    int capacity;
} PRIORITY_QUEUE;

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

void priorityQueueInit(PRIORITY_QUEUE *priorityQueue, int N)
{
    priorityQueue->size = 0;
    priorityQueue->capacity = N;
    priorityQueue->heap = (NODE **) malloc(N * sizeof(NODE *));
}

void remakeQueue(PRIORITY_QUEUE *priorityQueue, int N)
{
    for (int i = 0; i < N; i++)
    {
        if (priorityQueue->heap[i] != NULL)
        {
            //free(priorityQueue->heap[i]);
            priorityQueue->heap[i] = NULL;
        }
    }
    free(priorityQueue->heap);
    priorityQueue->heap = NULL;
    priorityQueue->size = 0;
    priorityQueue->heap = (NODE **) malloc(N * sizeof(NODE *));
}

void swap(NODE **a, NODE **b)
{
    NODE *temp = *a;
    *a = *b;
    *b = temp;
}

void addNode(PRIORITY_QUEUE *priorityQueue, int vertex, int distance)
{
    int i = priorityQueue->size++;
    priorityQueue->heap[i] = (NODE *) malloc(sizeof(NODE));
    priorityQueue->heap[i]->vertex = vertex;
    priorityQueue->heap[i]->distance = distance;
    while (i > 0 && priorityQueue->heap[i]->distance < priorityQueue->heap[(i - 1) / 2]->distance)
    {
        swap(&priorityQueue->heap[i], &priorityQueue->heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

NODE *getMin(PRIORITY_QUEUE *priorityQueue)
{
    if (priorityQueue->size == 0)
    {
        return NULL;
    }

    NODE *minNode = priorityQueue->heap[0];
    priorityQueue->heap[0] = priorityQueue->heap[--priorityQueue->size];

    int i = 0;
    while (1)
    {
        int leftChild = 2 * i + 1;
        int rightChild = 2 * i + 2;
        int smallest = i;

        if (leftChild < priorityQueue->size &&
            priorityQueue->heap[leftChild]->distance < priorityQueue->heap[smallest]->distance)
        {
            smallest = leftChild;
        }

        if (rightChild < priorityQueue->size &&
            priorityQueue->heap[rightChild]->distance < priorityQueue->heap[smallest]->distance)
        {
            smallest = rightChild;
        }

        if (smallest != i)
        {
            swap(&priorityQueue->heap[i], &priorityQueue->heap[smallest]);
            i = smallest;
        }
        else
        {
            break;
        }
    }

    return minNode;
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

int dijkstra(VERTEX **graph, int starting_vertex, int end_point, int N, bool *printed, PRIORITY_QUEUE *priorityQueue)
{
    int distances[N];
    int predecessors[N];
    bool visited[N];

    remakeQueue(priorityQueue, N);

    for (int i = 0; i < N; i++)
    {
        distances[i] = MAX_WEIGHT;
        predecessors[i] = -1;
        visited[i] = false;
    }

    distances[starting_vertex] = 0;
    addNode(priorityQueue, starting_vertex, 0);

    while (priorityQueue->size > 0)
    {
        NODE *minNode = getMin(priorityQueue);
        int min_index = minNode->vertex;
        free(minNode);

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
                addNode(priorityQueue, neighbour_index, distances[neighbour_index]);
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
    PRIORITY_QUEUE *priorityQueue = (PRIORITY_QUEUE *) malloc(sizeof(PRIORITY_QUEUE));
    priorityQueueInit(priorityQueue, N);
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
            case 'p':
                scanf(" %d", &vertex1);
                print(graph, vertex1);
                break;
            default:
                break;
        }
    }

    return 0;
}
