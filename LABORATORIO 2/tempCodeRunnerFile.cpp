// Estructura UNION-FIND
// Crear un programa en C++ para las estructuras Union-Find.

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class UnionFind
{
private:
    unordered_map<char, char> parent; // Map que almacena el padre de cada elemento
    unordered_map<char, int> size;    // Map que almacena el tamaño de cada conjunto

public:
    // Constructor para inicializar los conjuntos disjuntos
    UnionFind(vector<char> elements)
    {
        for (char element : elements)
        {
            parent[element] = element; // Cada elemento es su propio padre al inicio
            size[element] = 1;         // El tamaño de cada conjunto es 1 al inicio
        }
    }

    // Función FIND
    char find(char x)
    {
        if (parent[x] != x)
        {                                // Si el padre del elemento no es el mismo
            parent[x] = find(parent[x]); // Llamada recursiva hasta encontrar el padre
        }
        return parent[x]; // Devuelve el padre del elemento
    }

    // Función UNION
    void unionSet(char x, char y)
    {
        char rootX = find(x);
        char rootY = find(y);

        if (rootX == rootY)
        { // Si las raíces son iguales, no se pueden unir
            return;
        }

        // Unión por tamaño: el conjunto más pequeño se une al más grande
        if (size[rootX] < size[rootY])
        {
            parent[rootX] = rootY;
            size[rootY] += size[rootX];
        }
        else
        {
            parent[rootY] = rootX;
            size[rootX] += size[rootY];
        }

        cout << "Union: " << x << " y " << y << endl;
    }

    // Verifica si dos elementos son equivalentes
    bool areEquivalent(char x, char y)
    {
        return find(x) == find(y);
    }

    // Función para verificar el tipo de equivalencia
    void checkEquivalence(char x, char y)
    {
        cout << endl;
        if (areEquivalent(x, y))
        {
            cout << x << " y " << y << " son equivalentes." << endl;

            if (x == y)
            {
                cout << "Es una relacion reflexiva." << endl;
            }
            else
            {
                cout << "Es una relacion simetrica." << endl;

                // Transitividad implícita en Union-Find, imprimir solo si es necesario
                cout << "Es una relacion transitiva." << endl;
            }
        }
        else
        {
            cout << x << " y " << y << " no son equivalentes." << endl;
        }
    }
};

int main()
{
    vector<char> elements = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'};
    UnionFind uf(elements);

    // A = { a, b, c, d }
    uf.unionSet('b', 'a');
    uf.unionSet('c', 'a');
    uf.unionSet('d', 'c');

    // B = { e, f, g, h }
    uf.unionSet('e', 'f');
    uf.unionSet('g', 'f');
    uf.unionSet('h', 'g');

    // C = { i, j, k, l }
    uf.unionSet('i', 'j');
    uf.unionSet('k', 'j');
    uf.unionSet('l', 'k');

    // D = { m, n, o, p }
    uf.unionSet('m', 'n');
    uf.unionSet('o', 'n');
    uf.unionSet('p', 'o');

    // Uniones adicionales para crear conexiones entre los conjuntos
    uf.unionSet('f', 'a');
    uf.unionSet('j', 'a');
    uf.unionSet('n', 'a');

    // Verificación de equivalencias
    uf.checkEquivalence('a', 'b');
    uf.checkEquivalence('e', 'm');
    uf.checkEquivalence('g', 'o');
    uf.checkEquivalence('d', 'c');
    uf.checkEquivalence('h', 'g');
    uf.checkEquivalence('p', 'n');

    return 0;
}
