#include <iostream>  // Librería estándar para manejo de entrada y salida
#include <vector>    // Librería para usar vectores dinámicos en C++
#include <tuple>     // Librería para manejar tuplas (estructura de múltiples valores)
#include <algorithm> // Librería para usar funciones como sort() para ordenar
#include <chrono>    // Librería para medir el tiempo (benchmarking)
#include <random>    // Librería para generar números aleatorios

using namespace std;

// Estructuras globales para Union-Find
vector<int> parent; // Vector que almacena el padre (representante) de cada nodo
vector<int> rango;  // Vector que almacena el rango (profundidad) de cada nodo en su árbol

// Inicializa la estructura Union-Find
void init_union_find(int tamaño)
{
    parent.resize(tamaño);   // Redimensiona el vector 'parent' para que tenga 'tamaño' elementos
    rango.resize(tamaño, 0); // Redimensiona el vector 'rango' y lo inicializa con 0 en todos sus elementos
    for (int i = 0; i < tamaño; i++)
    {
        parent[i] = i; // Inicializa cada nodo como su propio padre (representante)
    }
}

// Función para encontrar el representante del conjunto que contiene al nodo u
int encontrar(int u)
{
    if (parent[u] != u)
    {                                     // Si el nodo u no es su propio representante
        parent[u] = encontrar(parent[u]); // Compresión de caminos: hace que u apunte directamente a su representante
    }
    return parent[u]; // Retorna el representante del nodo u
}

// Función para unir los conjuntos que contienen a u y v
void unir(int u, int v)
{
    int raiz_u = encontrar(u); // Encuentra el representante (raíz) del conjunto de u
    int raiz_v = encontrar(v); // Encuentra el representante (raíz) del conjunto de v

    if (raiz_u != raiz_v)
    { // Solo une si u y v están en conjuntos diferentes
        // Realiza la unión por rango para optimizar la estructura del árbol
        if (rango[raiz_u] > rango[raiz_v])
        {                            // Si el rango de la raíz de u es mayor
            parent[raiz_v] = raiz_u; // El representante de v se convierte en u
        }
        else if (rango[raiz_u] < rango[raiz_v])
        {                            // Si el rango de la raíz de v es mayor
            parent[raiz_u] = raiz_v; // El representante de u se convierte en v
        }
        else
        {                            // Si ambos tienen el mismo rango
            parent[raiz_v] = raiz_u; // Hacemos a u el representante de v
            rango[raiz_u]++;         // Incrementamos el rango de la raíz de u
        }
    }
}

// Función para agregar una arista (peso, u, v) al grafo
void agregar_arista(vector<tuple<int, int, int>> &aristas, int u, int v, int peso)
{
    // Inserta una tupla (peso, u, v) en el vector de aristas
    aristas.push_back(make_tuple(peso, u, v));
}

// Proceso de Kruskal para encontrar el Árbol de Expansión Mínima (MST)
vector<tuple<int, int, int>> Kruskal(int n, vector<tuple<int, int, int>> &aristas)
{
    vector<tuple<int, int, int>> MST; // Vector para almacenar el Árbol de Expansión Mínima
    init_union_find(n);               // Inicializa la estructura Union-Find con n nodos

    sort(aristas.begin(), aristas.end()); // Ordena las aristas por peso en orden ascendente

    // Recorre las aristas ordenadas
    for (auto [peso, u, v] : aristas)
    {
        if (encontrar(u) != encontrar(v))
        {                                          // Si u y v están en diferentes conjuntos
            unir(u, v);                            // Une los conjuntos que contienen a u y v
            MST.push_back(make_tuple(u, v, peso)); // Agrega la arista al Árbol de Expansión Mínima
        }
    }

    return MST; // Retorna el Árbol de Expansión Mínima
}

int main()
{
    int n = 500;                          // Número de nodos (en este caso, 500 nodos)
    vector<tuple<int, int, int>> aristas; // Vector para almacenar todas las aristas (peso, u, v)

    // Generación de pesos y aristas aleatorios para 500 nodos
    random_device rd;                         // Generador de números aleatorios
    mt19937 gen(rd());                        // Motor Mersenne Twister para generar números aleatorios
    uniform_int_distribution<> dist(1, 1000); // Distribución uniforme de enteros entre 1 y 1000 (pesos)

    // Bucle para generar aristas entre nodos
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            int peso = dist(gen);                // Genera un peso aleatorio entre 1 y 1000
            agregar_arista(aristas, i, j, peso); // Agrega una arista entre el nodo i y el nodo j con el peso generado
        }
    }

    // Medir el tiempo de ejecución del algoritmo de Kruskal
    auto start = chrono::high_resolution_clock::now();      // Captura el tiempo actual antes de ejecutar Kruskal
    vector<tuple<int, int, int>> MST = Kruskal(n, aristas); // Ejecuta el algoritmo de Kruskal
    auto end = chrono::high_resolution_clock::now();        // Captura el tiempo actual después de ejecutar Kruskal

    chrono::duration<double> tiempo_ejecucion = end - start; // Calcula la duración del algoritmo de Kruskal

    // Muestra el tiempo de ejecución en segundos
    cout << "Tiempo de ejecucion de Kruskal con " << n << " nodos: " << tiempo_ejecucion.count() << " segundos" << endl;

    // Mostrar las aristas del Árbol de Expansión Mínima (MST)
    cout << "Aristas del Árbol de Expansión Mínima:\n";
    for (const auto &[u, v, peso] : MST)
    {
        cout << "Nodo " << u << " - Nodo " << v << " con peso: " << peso << endl; // Imprime cada arista
    }

    return 0; // Fin del programa
}
