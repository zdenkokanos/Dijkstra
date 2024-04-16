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

typedef struct heap
{
    int index;
    int weight;
} HEAP;

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

int right(int index)
{
    return (2 * index + 2);
}

int left(int index)
{
    return (2 * index + 1);
}


void heapify(HEAP heap[], int size, int index)
{
    if (size == 1)
    {
        return;
    }
    else
    {
        int smallest = index;
        int left_child = left(index);
        int right_child = right(index);
        if (left_child < size && heap[left_child].weight < heap[smallest].weight)
        {
            smallest = left_child;
        }
        if (right_child < size && heap[right_child].weight < heap[smallest].weight)
        {
            smallest = right_child;
        }
        if (smallest != index)
        {
            HEAP temp = heap[index];
            heap[index] = heap[smallest];
            heap[smallest] = temp;
            heapify(heap, size, smallest);
        }
    }
}

void add_to_heap(HEAP heap[], int newIndex, int weight, int *size)
{
    if (size == 0)
    {
        heap[0].index = newIndex;
        heap[0].weight = weight;
        (*size)++;
    }
    else
    {
        heap[*size].index = newIndex;
        heap[*size].weight = weight;
        (*size)++;
        for (int i = (*size) / 2 - 1; i >= 0; i--)
        {
            heapify(heap, *size, i);
        }
    }
}


void remove_from_heap(HEAP heap[], int *size)
{
    HEAP temp =  heap[0];
    heap[0] = heap[(*size) -1];
    heap[(*size)-1] = temp;
    (*size)--;
    for (int i = (*size) / 2 - 1; i >= 0; i--)
    {
        heapify(heap, *size, i);
    }
}

void find_and_update(HEAP heap[], int index, int weight, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (heap[i].index == index)
        {
            heap[i].weight = weight;
            return;
        }
    }
    for (int i = size / 2 - 1; i >= 0; i--)
    {
        heapify(heap, size, i);
    }
}

int dijkstra(VERTEX **graph, int starting_vertex, int end_point, int N, bool *printed)
{
    int distances[N];
    int predecessors[N];
    bool visited[N];
    HEAP heap[N];
    bool isInHeap[N];

    for (int i = 0; i < N; i++)
    {
        distances[i] = MAX_WEIGHT;
        predecessors[i] = -1;
        visited[i] = false;
        heap[i].index = -1;
        heap[i].weight = MAX_WEIGHT;
        isInHeap[i] = false;
    }

    distances[starting_vertex] = 0;
    heap[0].index = starting_vertex;
    heap[0].weight = 0;
    int size = 1;

    while (1)
    {
        int min_index = -1;
        int h_index = heap[0].index;
        if (visited[h_index] == false && distances[h_index] < distances[end_point])
        {
            min_index = h_index;
            remove_from_heap(heap, &size);
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
                if (isInHeap[neighbour_index] == false)
                {
                    add_to_heap(heap, neighbour_index, distances[neighbour_index], &size);
                    isInHeap[neighbour_index] = true;
                }
                else
                {
                    find_and_update(heap, neighbour_index, weight, size);
                }
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