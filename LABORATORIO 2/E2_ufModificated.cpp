// Estructura UNION-FIND
// Crear un programa en C++ para las estructuras Union-Find.

#include <iostream>
#include <vector>

using namespace std;

class UnionFind
{
private:
    vector<int> parent; // parent -> padre
    vector<int> size;   // size -> dimension

public:
    // Function Constructor
    UnionFind(int n)
    {                      // Initialize the constructor to have 'n' elements.
        parent.resize(n);  // Resize the vector 'parent' to have 'n' elements.
        size.resize(n, 1); // Resize the vector 'size' to have 'n' elements initializing them at 1.

        /*The vector 'size' sets its own elements automatically because it has an initialized at 1.
            So all the elements of 'parent' are a set of their own, creating like a nood of each element, as:
            ➊  ➋  ➌  ➍  ➎  ➏  ➐  ➑  ➒  ➓
        */

        for (int i = 0; i < n; i++)
        {
            parent[i] = i; // This "fori" is to set the numbers(elements) into the vector 'parent'
        }
    }

    // Function FIND
    int find(int x)
    { // U add the number that u want to find.
        // parent[x]: This search for the parent of ur number.

        if (parent[x] != x)
        {                                // So if the parent of the number is not the same number
            parent[x] = find(parent[x]); // It will repeat recursivity until find that are the same.
            // That's when u have a tree whith a lot of parents and sons.
        }
        return parent[x]; // When u find it, now u can return which is the parent of ur number.
    }

    // Function UNION
    void unionSet(int x, int y)
    { // U want to add X to Y.    x -> Y
        // root: the parent of parents / the first parent

        // Create variables to save the root of X and Y, using function FIND.
        int rootX = find(x);
        int rootY = find(y);

        if (rootX == rootY)
        { // if the roots are the same u can't union them, cause them are in the same set.
            return;
        }

        // UNION FOR SIZE: The goal is to join/unite the smallest set in the biggest set.
        // Minimizes the trees height, which helps operations such as FIND to be faster.

        if (size[rootX] < size[rootY])
        {                               // if the set of 'x' is smallest than the set of 'y':   Y in X   |   X <- Y
            parent[rootX] = rootY;      // X <- Y
            size[rootY] += size[rootX]; // The size of the set of 'y' is updated by:  setOfY = 'setOfY + setOfX'
        }
        else
        {                          // if it's not,  done in reverse:   X in Y   |   Y <- X
            parent[rootY] = rootX; // Y <- X
            size[rootX] += size[rootY];
        }

        cout << "Union: " << x << " y " << y << endl;
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

    cout << "El elemento 1 y el elemento 3 son equivalentes?" << endl;
    if (uf.find(1) == uf.find(3))
    { // uf.fin(1): 0     uf.find(3): 0       0 == 0
        cout << "Si" << endl;
    }
    else
    {
        cout << "No" << endl;
    }

    cout << "El elemento 4 y el elemento 7 son equivalentes?" << endl;
    if (uf.find(4) == uf.find(7))
    { // uf.fin(4): 4     uf.find(7): 4       4 == 4
        cout << "Si" << endl;
    }
    else
    {
        cout << "No" << endl;
    }

    return 0;
}