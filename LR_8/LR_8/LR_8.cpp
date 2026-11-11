#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <queue>

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

void addEdgeToList(Node** adjList, int u, int v) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) return;
    newNode->vertex = v;
    newNode->next = adjList[u];
    adjList[u] = newNode;
}

Node** matrixToAdjList(int** matrix, int n) {
    Node** adjList = (Node**)malloc(n * sizeof(Node*));
    if (!adjList) return NULL;
    for (int i = 0; i < n; i++) adjList[i] = NULL;

    for (int i = 0; i < n; i++) {
        for (int j = n - 1; j >= 0; j--) {
            if (matrix[i][j] == 1) {
                addEdgeToList(adjList, i, j);
            }
        }
    }
    return adjList;
}

void printAdjList(Node** adjList, int n) {
    printf("\n--- Списки смежности ---\n");
    for (int i = 0; i < n; i++) {
        printf("Вершина %d: ", i + 1);
        Node* curr = adjList[i];
        if (!curr) printf("[нет связей]");
        while (curr != NULL) {
            printf("-> %d ", curr->vertex + 1);
            curr = curr->next;
        }
        printf("\n");
    }
    printf("------------------------\n\n");
}

void freeAdjList(Node** adjList, int n) {
    if (!adjList) return;
    for (int i = 0; i < n; i++) {
        Node* curr = adjList[i];
        while (curr != NULL) {
            Node* tmp = curr;
            curr = curr->next;
            free(tmp);
        }
    }
    free(adjList);
}

typedef struct QueueNode {
    int data;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* back;
} MyQueue;

void initQueue(MyQueue* q) {
    q->front = NULL;
    q->back = NULL;
}

int isEmpty(MyQueue* q) {
    return q->front == NULL;
}

void pushQueue(MyQueue* q, int val) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (!newNode) return;
    newNode->data = val;
    newNode->next = NULL;

    if (q->front == NULL) {
        q->front = q->back = newNode;
    }
    else {
        q->back->next = newNode;
        q->back = newNode;
    }
}

int popQueue(MyQueue* q) {
    if (isEmpty(q)) return -1;

    QueueNode* temp = q->front;
    int val = temp->data;
    q->front = q->front->next;

    if (q->front == NULL) {
        q->back = NULL;
    }

    free(temp);
    return val;
}

void clearQueue(MyQueue* q) {
    while (!isEmpty(q)) {
        popQueue(q);
    }
}

int** create_matrix(int n) {
    int** matrix = (int**)malloc(n * sizeof(int*));
    if (!matrix) return NULL;
    for (int i = 0; i < n; i++) {
        matrix[i] = (int*)malloc(n * sizeof(int));
    }
    return matrix;
}

void free_matrix(int** matrix, int n) {
    if (matrix != NULL) {
        for (int i = 0; i < n; i++) free(matrix[i]);
        free(matrix);
    }
}

void generate_graph(int** matrix, int n) {
    for (int i = 0; i < n; i++) {
        matrix[i][i] = 0;
        for (int j = i + 1; j < n; j++) {
            int val = rand() % 2;
            matrix[i][j] = val;
            matrix[j][i] = val;
        }
    }
}

void print_matrix(int** matrix, int n) {
    printf("\nМатрица смежности G:\n    ");
    for (int j = 0; j < n; j++) printf("%3d", j + 1);
    printf("\n----");
    for (int j = 0; j < n; j++) printf("---");
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("%3d|", i + 1);
        for (int j = 0; j < n; j++) {
            printf("%3d", matrix[i][j]);
        }

        printf("\n");
    }
    printf("\n");
}

void show_graph_representations(int** matrix, int n) {
    print_matrix(matrix, n);
    Node** adjList = matrixToAdjList(matrix, n);
    printAdjList(adjList, n);
    freeAdjList(adjList, n);
}

void reset_num(int* NUM, int n) {
    for (int i = 0; i < n; i++) NUM[i] = 0;
}

void BFS_matrix_std(int start_v, int n, int** matrix, int* NUM, int print_output) {
    std::queue<int> q;
    q.push(start_v);
    NUM[start_v] = 1;

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        if (print_output) printf("%d ", v + 1);


        for (int i = 0; i < n; i++) {
            if (matrix[v][i] == 1 && NUM[i] == 0) {
                q.push(i);
                NUM[i] = 1;
            }
        }
    }
}

void BFS_matrix_myqueue(int start_v, int n, int** matrix, int* NUM, int print_output) {
    MyQueue q;
    initQueue(&q);

    pushQueue(&q, start_v);
    NUM[start_v] = 1;

    while (!isEmpty(&q)) {
        int v = popQueue(&q);
        if (print_output) printf("%d ", v + 1);

        for (int i = 0; i < n; i++) {
            if (matrix[v][i] == 1 && NUM[i] == 0) {
                pushQueue(&q, i);
                NUM[i] = 1;
            }
        }
    }
}

void BFS_list_std(int start_v, Node** adjList, int* NUM, int print_output) {
    std::queue<int> q;
    q.push(start_v);
    NUM[start_v] = 1;

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        if (print_output) printf("%d ", v + 1);

        Node* curr = adjList[v];
        while (curr != NULL) {
            int neighbor = curr->vertex;
            if (NUM[neighbor] == 0) {
                q.push(neighbor);
                NUM[neighbor] = 1;
            }
            curr = curr->next;
        }
    }
}

void BFS_list_myqueue(int start_v, Node** adjList, int* NUM, int print_output) {
    MyQueue q;
    initQueue(&q);

    pushQueue(&q, start_v);
    NUM[start_v] = 1;

    while (!isEmpty(&q)) {
        int v = popQueue(&q);
        if (print_output) printf("%d ", v + 1);

        Node* curr = adjList[v];
        while (curr != NULL) {
            int neighbor = curr->vertex;
            if (NUM[neighbor] == 0) {
                pushQueue(&q, neighbor);
                NUM[neighbor] = 1;
            }
            curr = curr->next;
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand((unsigned int)time(NULL));

    int n = 6;
    int start_node = 0;

    printf("========================================================\n");
    printf("   Лабораторная работа №8: Обход графа в ширину (BFS)\n");
    printf("========================================================\n");
    printf("Введите количество вершин графа (N): ");
    if (scanf("%d", &n) != 1 || n < 1) n = 6;

    int** matrix = create_matrix(n);
    int* NUM = (int*)malloc(n * sizeof(int));
    generate_graph(matrix, n);
    show_graph_representations(matrix, n);

    int choice = -1;
    while (choice != 0) {
        printf("----------------- МЕНЮ -----------------\n");
        printf("Текущая стартовая вершина: %d\n", start_node + 1);
        printf("1. BFS по матрице (std::queue)\n");
        printf("2. BFS по спискам смежности (std::queue, Задание 3*)\n");
        printf("3. BFS по матрице на СВОЕЙ очереди (MyQueue, Задание 2*)\n");
        printf("4. Сравнение времени std::queue vs MyQueue (Задание 2* п.2)\n");
        printf("5. Выбрать стартовую вершину для обхода\n");

        printf("6. Сгенерировать новый граф\n");
        printf("7. Показать граф (матрицу и списки)\n");
        printf("8. BFS по спискам смежности на СВОЕЙ очереди\n");
        printf("0. Выход\n");
        printf("Выберите пункт: ");

        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
        case 1: {
            reset_num(NUM, n);
            printf("\n[BFS матрица (std::queue)]: ");
            clock_t t_start = clock();

            BFS_matrix_std(start_node, n, matrix, NUM, 1);

            for (int i = 0; i < n; i++) {
                if (NUM[i] == 0) BFS_matrix_std(i, n, matrix, NUM, 1);
            }
            clock_t t_end = clock();
            double time_ms = (double)(t_end - t_start) * 1000.0 / CLOCKS_PER_SEC;
            printf("\n[Время работы]: %.4f мс\n\n", time_ms);
            break;
        }

        case 2: {
            reset_num(NUM, n);
            Node** adjList = matrixToAdjList(matrix, n);
            printf("\n[BFS списки смежности (std::queue)]: ");
            clock_t t_start = clock();

            BFS_list_std(start_node, adjList, NUM, 1);
            for (int i = 0; i < n; i++) {
                if (NUM[i] == 0) BFS_list_std(i, adjList, NUM, 1);
            }


            clock_t t_end = clock();
            double time_ms = (double)(t_end - t_start) * 1000.0 / CLOCKS_PER_SEC;
            printf("\n[Время работы]: %.4f мс\n\n", time_ms);

            freeAdjList(adjList, n);
            break;
        }

        case 3: {
            reset_num(NUM, n);
            printf("\n[BFS матрица (MyQueue)]: ");
            clock_t t_start = clock();

            BFS_matrix_myqueue(start_node, n, matrix, NUM, 1);
            for (int i = 0; i < n; i++) {
                if (NUM[i] == 0) BFS_matrix_myqueue(i, n, matrix, NUM, 1);
            }
            clock_t t_end = clock();
            double time_ms = (double)(t_end - t_start) * 1000.0 / CLOCKS_PER_SEC;
            printf("\n[Время работы]: %.4f мс\n\n", time_ms);
            break;
        }

        case 4: {
            printf("\n=== Сравнение времени работы (std::queue vs MyQueue) ===\n");
            int test_n = 0;
            printf("Введите размер графа для теста производительности (рекомендуется 500-1000): ");
            if (scanf("%d", &test_n) != 1 || test_n < 10) test_n = 500;

            printf("Генерация большого тестового графа (%dx%d)...\n", test_n, test_n);
            int** test_mat = create_matrix(test_n);
            int* test_num = (int*)malloc(test_n * sizeof(int));
            generate_graph(test_mat, test_n);

            Node** test_adjList = matrixToAdjList(test_mat, test_n);

            reset_num(test_num, test_n);
            clock_t s1 = clock();
            for (int i = 0; i < test_n; i++) {
                if (test_num[i] == 0) BFS_matrix_std(i, test_n, test_mat, test_num, 0);
            }
            clock_t e1 = clock();
            double t1 = (double)(e1 - s1) * 1000.0 / CLOCKS_PER_SEC;

            reset_num(test_num, test_n);
            clock_t s2 = clock();
            for (int i = 0; i < test_n; i++) {
                if (test_num[i] == 0) BFS_matrix_myqueue(i, test_n, test_mat, test_num, 0);
            }
            clock_t e2 = clock();
            double t2 = (double)(e2 - s2) * 1000.0 / CLOCKS_PER_SEC;

            reset_num(test_num, test_n);
            clock_t s3 = clock();
            for (int i = 0; i < test_n; i++) if (test_num[i] == 0) BFS_list_std(i, test_adjList, test_num, 0);
            clock_t e3 = clock();
            double t3 = (double)(e3 - s3) * 1000.0 / CLOCKS_PER_SEC;

            reset_num(test_num, test_n);
            clock_t s4 = clock();
            for (int i = 0; i < test_n; i++) if (test_num[i] == 0) BFS_list_myqueue(i, test_adjList, test_num, 0);
            clock_t e4 = clock();
            double t4 = (double)(e4 - s4) * 1000.0 / CLOCKS_PER_SEC;

            printf("Результаты на графе N = %d:\n", test_n);
            printf("1. Матрица Стандартный std::queue : %.3f мс\n", t1);
            printf("2. Матрица Собственная MyQueue    : %.3f мс\n", t2);
            printf("3. Списки Стандартный std::queue : %.3f мс\n", t3);
            printf("4. Списки Собственная MyQueue    : %.3f мс\n", t4);
            printf("========================================================\n\n");

            free_matrix(test_mat, test_n);
            freeAdjList(test_adjList, test_n);
            free(test_num);
            break;
        }

        case 5: {
            int input_v;
            printf("Введите номер вершины для старта (от 1 до %d): ", n);
            if (scanf("%d", &input_v) == 1 && input_v >= 1 && input_v <= n) {
                start_node = input_v - 1;
                printf("Стартовая вершина установлена на: %d\n\n", start_node + 1);
            }
            else {

                printf("Некорректный номер! Оставлена прежняя вершина.\n\n");
            }
            break;
        }

        case 6:
            generate_graph(matrix, n);
            printf("\nГраф успешно перегенерирован!\n");
            show_graph_representations(matrix, n);
            break;

        case 7:
            show_graph_representations(matrix, n);
            break;

        case 0:
            printf("\nВыход из программы. До свидания!\n");
            break;


        default:
            printf("\nНеверный пункт меню!\n\n");
            break;

        case 8: {
            reset_num(NUM, n);
            Node** adjList = matrixToAdjList(matrix, n);
            printf("\n[BFS списки смежности на своей очереди]: ");
            clock_t t_start = clock();

            BFS_matrix_myqueue(start_node, n, matrix, NUM, 1);
            for (int i = 0; i < n; i++) {
                if (NUM[i] == 0) BFS_matrix_myqueue(i, n, matrix, NUM, 1);
            }
            clock_t t_end = clock();
            double time_ms = (double)(t_end - t_start) * 1000.0 / CLOCKS_PER_SEC;
            printf("\n[Время работы]: %.4f мс\n\n", time_ms);

            freeAdjList(adjList, n);
            break;
        }
        }
    }

    free_matrix(matrix, n);
    free(NUM);
    return 0;
}