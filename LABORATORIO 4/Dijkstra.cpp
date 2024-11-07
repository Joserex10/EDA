#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <chrono>  // Para medir tiempos
#include <cstdlib> // Para generar números aleatorios
#include <ctime>

using namespace std;

const int INF = numeric_limits<int>::max(); // Infinito, utilizado para representar distancias no alcanzables

// Función para agregar una arista al grafo
void agregar_arista(vector<vector<pair<int, int>>> &adyacente, int u, int v, int peso)
{
    adyacente[u].push_back({v, peso}); // Agrega una arista desde el nodo u al nodo v con el peso dado
}

// Función para generar nodos y pesos aleatorios
void generar_grafo_aleatorio(vector<vector<pair<int, int>>> &adyacente, int n, int num_aristas)
{
    srand(time(0)); // Inicializa el generador de números aleatorios usando el tiempo actual como semilla
    for (int i = 0; i < num_aristas; ++i)
    {
        int u = rand() % n; // Genera un nodo aleatorio 'u' entre 0 y n-1
        int v = rand() % n; // Genera un nodo aleatorio 'v' entre 0 y n-1
        if (u != v)
        {                                          // Verifica que 'u' y 'v' no sean iguales para evitar auto-bucles
            int peso = rand() % 100 + 1;           // Genera un peso aleatorio entre 1 y 100 para la arista
            agregar_arista(adyacente, u, v, peso); // Agrega la arista con el peso generado al grafo
        }
    }
}

// Función para implementar Dijkstra
void dijkstra(const vector<vector<pair<int, int>>> &adyacente, int origen, vector<int> &distancias, vector<int> &previo)
{
    int n = adyacente.size();        // Obtiene el número de nodos en el grafo
    distancias.assign(n, INF);       // Inicializa el vector de distancias con infinito para todos los nodos
    previo.assign(n, -1);            // Inicializa el vector previo (nodo anterior en el camino más corto) con -1
    vector<bool> visitado(n, false); // Inicializa un vector para marcar los nodos visitados como 'falso'

    // Cola de prioridad para elegir el nodo con la menor distancia (usamos un min-heap)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> cola_prioridad;

    distancias[origen] = 0;           // La distancia desde el origen a sí mismo es 0
    cola_prioridad.push({0, origen}); // Inserta el nodo origen en la cola de prioridad con distancia 0

    while (!cola_prioridad.empty())
    {
        int actual = cola_prioridad.top().second; // Extrae el nodo con la menor distancia en la cola de prioridad
        cola_prioridad.pop();                     // Elimina el nodo de la cola

        if (visitado[actual]) // Si el nodo ya fue visitado, lo ignoramos
            continue;
        visitado[actual] = true; // Marcamos el nodo actual como visitado

        // Recorre los vecinos del nodo actual
        for (auto &vecino : adyacente[actual])
        {
            int nodo_vecino = vecino.first; // El vecino del nodo actual
            int peso = vecino.second;       // El peso de la arista que conecta el nodo actual con el vecino

            if (!visitado[nodo_vecino]) // Si el nodo vecino no ha sido visitado
            {
                int nueva_distancia = distancias[actual] + peso; // Calcula la nueva distancia

                // Si la nueva distancia es menor que la distancia actual almacenada
                if (nueva_distancia < distancias[nodo_vecino])
                {
                    distancias[nodo_vecino] = nueva_distancia;           // Actualiza la distancia más corta
                    previo[nodo_vecino] = actual;                        // Almacena el nodo previo en el camino más corto
                    cola_prioridad.push({nueva_distancia, nodo_vecino}); // Inserta el nodo vecino en la cola de prioridad
                }
            }
        }
    }
}

int main()
{
    int n = 501;                                 // Número de nodos en el grafo
    int num_aristas = 2000;                      // Número de aristas en el grafo
    vector<vector<pair<int, int>>> adyacente(n); // Lista de adyacencia para representar el grafo

    // Generar un grafo aleatorio con 501 nodos y 2000 aristas
    generar_grafo_aleatorio(adyacente, n, num_aristas);

    int origen = 0;                 // Nodo origen desde donde se ejecutará el algoritmo de Dijkstra
    vector<int> distancias, previo; // Vectores para almacenar las distancias y los nodos previos en el camino más corto

    // Medir el tiempo de ejecución usando std::chrono
    auto inicio = chrono::high_resolution_clock::now(); // Obtiene el tiempo de inicio

    // Ejecutar el algoritmo de Dijkstra
    dijkstra(adyacente, origen, distancias, previo);

    auto fin = chrono::high_resolution_clock::now();  // Obtiene el tiempo de finalización
    chrono::duration<double> duracion = fin - inicio; // Calcula la duración del algoritmo

    // Mostrar las distancias más cortas desde el nodo origen
    cout << "Distancias desde el nodo " << origen << ":\n";
    for (int i = 0; i < n; ++i)
    {
        cout << "Nodo " << i << ": " << distancias[i] << "\n"; // Imprime la distancia de cada nodo desde el origen
    }

    // Muestra el tiempo de ejecución en segundos
    cout << "Tiempo de ejecución: " << duracion.count() << " segundos\n";

    return 0;
}
