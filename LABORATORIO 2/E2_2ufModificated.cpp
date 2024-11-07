// Estructura UNION-FIND
// Crear un programa en C++ para las estructuras Union-Find.

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class UnionFind
{
private:
    unordered_map<char, char> parent; // Map that stores the parent of each element
    unordered_map<char, int> size;    // Map that stores the size of each element

public:
    // Function Constructor to initialize the disjoint-sets
    UnionFind(vector<char> elements)
    {
        for (char element : elements)
        {
            parent[element] = element; // Each element is its own parent at the beginning
            size[element] = 1;         // The size of each set is 1 at the beginning
        }
    }

    // Function FIND
    char find(char x)
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
    void unionSet(char x, char y)
    { // U want to add X to Y.    x -> Y
        // root: the parent of parents / the first parent

        // Create variables to save the root of X and Y, using function FIND.
        char rootX = find(x);
        char rootY = find(y);

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

    // Unions
    uf.unionSet('f', 'a');
    uf.unionSet('j', 'a');
    uf.unionSet('n', 'a');

    cout << "El elemento 'c' y el elemento 'm' son equivalentes?" << endl;
    if (uf.find('c') == uf.find('m'))
    { // uf.fin(c): a     uf.find(m): a       a == a
        cout << "Si" << endl;
    }
    else
    {
        cout << "No" << endl;
    }

    cout << "El elemento 'd' y el elemento 'j' son equivalentes?" << endl;
    if (uf.find('d') == uf.find('j'))
    { // uf.fin(d): a     uf.find(j): a       a == a
        cout << "Si" << endl;
    }
    else
    {
        cout << "No" << endl;
    }

    return 0;
}