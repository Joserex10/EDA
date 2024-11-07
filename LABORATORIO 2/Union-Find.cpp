#include <iostream>
#include <vector>
using namespace std;

class UnionFind
{
private:
    vector<int> parent;
    vector<int> size;

public:
    // Constructor
    UnionFind(int n)
    {
        parent.resize(n);
        size.resize(n, 1);
        for (int i = 0; i < n; ++i)
        {
            parent[i] = i;
        }
    }

    // Encuentra el representante (líder) del conjunto al que pertenece x
    int find(int x)
    {
        if (parent[x] != x)
        {
            parent[x] = find(parent[x]); // Compresión de caminos
        }
        return parent[x];
    }

    // Une los conjuntos que contienen x e y
    void unionSet(int x, int y)
    {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX != rootY)
        {
            // Union por tamaño
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
        }
    }
};

int main()
{
    UnionFind uf(12);

    uf.unionSet(1, 0);
    uf.unionSet(2, 0);
    uf.unionSet(3, 2);
    uf.unionSet(5, 4);
    uf.unionSet(6, 4);
    uf.unionSet(7, 6);
    uf.unionSet(9, 8);
    uf.unionSet(10, 8);
    uf.unionSet(11, 10);

    cout << "\nEl elemento 1 y el elemento 3 son equivalentes? \n";
    if (uf.find(1) == uf.find(3))
    {
        cout << "Si" << endl;
    }
    else
    {
        cout << "No" << endl;
    }

    cout << "El elemento 2 y el elemento 9 son equivalentes? \n";
    if (uf.find(2) == uf.find(9))
    {
        cout << "Si" << endl;
    }
    else
    {
        cout << "No" << endl;
    }

    cout << "El elemento 4 y el elemento 7 son equivalentes? \n";
    if (uf.find(4) == uf.find(7))
    {
        cout << "Si" << endl;
    }
    else
    {
        cout << "No" << endl;
    }

    return 0;
}
