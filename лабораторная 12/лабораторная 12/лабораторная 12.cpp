#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <cstdlib>

using namespace std;
using namespace chrono;

// Бесконечность (очень большое число, обозначающее отсутствие ребра)
const int INF = 1e9;

// Часть 1: алгоритм Дейкстры (Без оптимизации, O(V^2))

void dijkstra(int start, const vector<vector<int>>& graph, vector<int>& dist) {
    int V = graph.size();

    // Изначально все расстояния - бесконечность
    dist.assign(V, INF);
    dist[start] = 0;

    // Массив для отметки посещенных вершин
    vector<bool> visited(V, false);

    for (int count = 0; count < V; count++) {
        // 1. Находим непосещенную вершину с минимальным расстоянием
        int u = -1;
        int minDist = INF;
        for (int i = 0; i < V; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                u = i;
            }
        }

        // Если все оставшиеся вершины недостижимы, прерываемся
        if (u == -1) break;

        // 2. Отмечаем ее как посещенную
        visited[u] = true;

        // 3. Обновляем расстояния до соседей
        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] != INF && graph[u][v] != 0) {
                if (dist[u] + graph[u][v] < dist[v]) {
                    dist[v] = dist[u] + graph[u][v];
                }
            }
        }
    }
}

// Часть 2: алгоритм Флойда-Уоршела
void floydWarshall(const vector<vector<int>>& graph, vector<vector<int>>& dist) {
    int V = graph.size();

    // Инициализируем матрицу расстояний на основе матрицы смежности
    dist = graph;

    // Перебираем все возможные промежуточные вершины (k)
    for (int k = 0; k < V; k++) {
        // Перебираем начальную вершину (i)
        for (int i = 0; i < V; i++) {
            // Перебираем конечную вершину (j)
            for (int j = 0; j < V; j++) {
                // Если путь через k короче, чем текущий прямой путь
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }
}


// Вспомогательные функции

// Генерация случайного графа (неориентированного, с весами)
vector<vector<int>> generateGraph(int V) {
    vector<vector<int>> graph(V, vector<int>(V, INF));

    // Главная диагональ = 0 (расстояние до самого себя)
    for (int i = 0; i < V; i++) graph[i][i] = 0;

    // Случайные ребра (примерно 30% связности)
    srand(42); // Фиксируем seed для воспроизводимости
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            if (rand() % 100 < 30) { // 30% шанс, что есть ребро
                int weight = rand() % 20 + 1; // Вес от 1 до 20
                graph[i][j] = weight;
                graph[j][i] = weight; // Граф неориентированный
            }
        }
    }
    return graph;
}

// Вывод матрицы на экран (для проверки)
void printMatrix(const vector<vector<int>>& matrix) {
    int V = matrix.size();
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (matrix[i][j] == INF) cout << setw(5) << "INF";
            else cout << setw(5) << matrix[i][j];
        }
        cout << endl;
    }
}

int main() {
    setlocale(LC_ALL, "rus");
    // Задание 1 и 2 из Части 3: Граф на 20 вершин 
    cout << " Часть 3 (п.1-4): Граф на 20 вершин " << endl;
    int V20 = 20;
    vector<vector<int>> graph20 = generateGraph(V20);

    // Дейкстра из случайной стартовой вершины (например, из 0)
    int startVertex = 0;
    vector<int> distDijkstra;

    auto start = high_resolution_clock::now();
    dijkstra(startVertex, graph20, distDijkstra);
    auto end = high_resolution_clock::now();
    cout << "Время Дейкстры (V=20): " << duration<double>(end - start).count() << " сек." << endl;

    // Флойд-Уоршелл
    vector<vector<int>> distFloyd;
    start = high_resolution_clock::now();
    floydWarshall(graph20, distFloyd);
    end = high_resolution_clock::now();
    cout << "Время Флойда-Уоршелла (V=20): " << duration<double>(end - start).count() << " сек." << endl;

    // Сравнение результатов (п.4)
    cout << "\nСравнение расстояний (Дейкстра vs Флойд) для вершины " << startVertex << ":" << endl;
    cout << "Вершина\tДейкстра\tФлойд" << endl;
    for (int i = 0; i < V20; i++) {
        if (i % 5 == 0) { // Выводим не все, чтобы не засорять экран
            cout << i << "\t" << (distDijkstra[i] == INF ? -1 : distDijkstra[i])
                << "\t\t" << (distFloyd[startVertex][i] == INF ? -1 : distFloyd[startVertex][i]) << endl;
        }
    }

    // Вывод матрицы (Доп. задание к Части 2)
    cout << "\nМатрица расстояний (Флойд-Уоршелл). Фрагмент 7x7:" << endl;
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if (distFloyd[i][j] == INF) cout << setw(5) << "INF";
            else cout << setw(5) << distFloyd[i][j];
        }
        cout << endl;
    }

    // Задание 5 и 6: Замер времени для разных V ---
    cout << "\n Часть 3 (п.5-6): Замер времени для V = 10, 20, 50, 100 ---" << endl;
    cout << "V\tДейкстра (сек)\tФлойд (сек)" << endl;
    cout << "----------------------------------------" << endl;

    vector<int> sizes = { 10, 20, 50, 100 };
    for (int V : sizes) {
        vector<vector<int>> g = generateGraph(V);
        vector<int> d;
        vector<vector<int>> f;

        // Замер Дейкстры
        auto t1 = high_resolution_clock::now();
        dijkstra(0, g, d);
        auto t2 = high_resolution_clock::now();
        double timeDijkstra = duration<double>(t2 - t1).count();

        // Замер Флойда
        t1 = high_resolution_clock::now();
        floydWarshall(g, f);
        t2 = high_resolution_clock::now();
        double timeFloyd = duration<double>(t2 - t1).count();

        cout << V << "\t" << timeDijkstra << "\t\t" << timeFloyd << endl;
    }

    return 0;
}