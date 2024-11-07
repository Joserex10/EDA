#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

// Definición de la clase BTreeNode (Nodo del arbol B)
class BTreeNode
{
public:
    int t;                    // Grado minimo del arbol B (t).
    int n;                    // numero actual de llaves en el nodo.
    bool hoja;                // Indica si el nodo es hoja (sin hijos).
    vector<int> claves;       // Vector que almacena las llaves en el nodo.
    vector<BTreeNode *> hijo; // Vector que almacena los punteros a los hijos del nodo.

    // Constructor de la clase BTreeNode
    BTreeNode(int _t, bool _hoja);

    // Funcion que recorre el arbol y muestra las llaves en orden ascendente
    void recorrido();

    // Funcion para insertar una nueva llave en un nodo que no esta lleno
    void insertarVacio(int k);

    // Funcion para dividir un hijo y en un nodo no lleno
    void dividirHijo(int i, BTreeNode *y);

    // Clase amiga para acceder a miembros privados/protegidos de BTreeNode desde BTree
    friend class BTree;
};

// Constructor de la clase BTreeNode
BTreeNode::BTreeNode(int t1, bool hoja1)
{
    t = t1;                   // Inicializa el grado minimo del arbol.
    hoja = hoja1;             // Inicializa si el nodo es hoja o no.
    n = 0;                    // Inicializa el numero de llaves en el nodo a 0.
    claves.resize(2 * t - 1); // Redimensiona el vector de llaves para que tenga 2 * t - 1 elementos.
    hijo.resize(2 * t);       // Redimensiona el vector de hijos para que tenga 2 * t elementos.
}

// Funcion para recorrer el arbol y mostrar las llaves
void BTreeNode::recorrido()
{
    int i;
    // Recorre cada llave en el nodo
    for (i = 0; i < n; i++)
    {
        // Si el nodo no es hoja, recorre el subarbol antes de imprimir la llave
        if (!hoja)
            hijo[i]->recorrido();
        cout << " " << claves[i]; // Imprime la llave actual
    }

    // Si el nodo no es hoja, recorre el subarbol del último hijo
    if (!hoja)
        hijo[i]->recorrido();
}

// Funcion para insertar una nueva llave en un nodo que no esta lleno
void BTreeNode::insertarVacio(int k)
{
    int i = n - 1; // Comienza con la ultima posición de llaves

    // Si el nodo es hoja, inserta la nueva llave en la posición correcta
    if (hoja)
    {
        while (i >= 0 && claves[i] > k) // Desplaza las llaves mayores que k una posición a la derecha
        {
            claves[i + 1] = claves[i];
            i--;
        }
        claves[i + 1] = k; // Inserta la nueva llave en la posición adecuada
        n++;               // Incrementa el numero de llaves en el nodo
    }
    else
    {
        // Encuentra el hijo adecuado donde insertar la nueva llave
        while (i >= 0 && claves[i] > k)
            i--;

        // Si el hijo esta lleno, divídelo
        if (hijo[i + 1]->n == 2 * t - 1)
        {
            dividirHijo(i + 1, hijo[i + 1]);

            // Decide cual de los dos hijos sera el destino para la nueva llave
            if (claves[i + 1] < k)
                i++;
        }
        hijo[i + 1]->insertarVacio(k); // Inserta la nueva llave en el hijo adecuado
    }
}

// Funcion para dividir un hijo en un nodo
void BTreeNode::dividirHijo(int i, BTreeNode *y)
{
    // Crea un nuevo nodo que tomara la mitad de las llaves de y
    BTreeNode *z = new BTreeNode(y->t, y->hoja);
    z->n = t - 1;

    // Copia las ultimas (t-1) llaves de y a z
    for (int j = 0; j < t - 1; j++)
        z->claves[j] = y->claves[j + t];

    // Copia los ultimos t hijos de y a z si y no es hoja
    if (!y->hoja)
    {
        for (int j = 0; j < t; j++)
            z->hijo[j] = y->hijo[j + t];
    }

    // Reduce el numero de llaves en y
    y->n = t - 1;

    // Crea espacio para un nuevo hijo en el nodo actual
    for (int j = n; j >= i + 1; j--)
        hijo[j + 1] = hijo[j];

    // Enlaza el nuevo hijo z al nodo actual
    hijo[i + 1] = z;

    // Mueve las llaves para hacer espacio para la nueva llave
    for (int j = n - 1; j >= i; j--)
        claves[j + 1] = claves[j];

    // Copia la llave media de y al nodo actual
    claves[i] = y->claves[t - 1];

    n++; // Incrementa el numero de llaves en el nodo actual
}

// Definición de la clase BTree (arbol B)
class BTree
{
public:
    BTreeNode *root; // Puntero a la raiz del arbol
    int t;           // Grado minimo del arbol

    // Constructor de la clase BTree
    BTree(int _t)
    {
        root = nullptr; // Inicializa la raiz como nula
        t = _t;         // Inicializa el grado minimo del arbol
    }

    // Funcion para recorrer el arbol
    void recorrido()
    {
        if (root != nullptr)
            root->recorrido(); // Llama a la Funcion de recorrido del nodo raiz
    }

    // Funcion para insertar una nueva llave en el arbol
    void insert(int k);
};

// Funcion para insertar una nueva llave en el arbol B
void BTree::insert(int k)
{
    if (root == nullptr)
    {
        // Si el arbol esta vacio, crea la raiz con la nueva llave
        root = new BTreeNode(t, true);
        root->claves[0] = k;
        root->n = 1;
    }
    else
    {
        // Si la raiz esta llena, crea un nuevo nodo raiz
        if (root->n == 2 * t - 1)
        {
            BTreeNode *s = new BTreeNode(t, false);
            s->hijo[0] = root;       // El antiguo nodo raiz se convierte en el primer hijo del nuevo nodo raiz
            s->dividirHijo(0, root); // Divide la antigua raiz

            // Decide en que hijo insertar la nueva llave
            int i = 0;
            if (s->claves[0] < k)
                i++;
            s->hijo[i]->insertarVacio(k);

            root = s; // La nueva raiz es el nuevo nodo creado
        }
        else
        {
            root->insertarVacio(k); // Inserta la nueva llave en la raiz actual
        }
    }
}

// Funcion principal
int main()
{
    BTree t(3);                                        // Crea un arbol B con grado minimo 3
    auto start = chrono::high_resolution_clock::now(); // Marca el tiempo de inicio

    // Inserta varias llaves en el arbol B (Osea inserta los datos)
    t.insert(70);
    t.insert(50);
    t.insert(30);
    t.insert(40);
    t.insert(20);
    t.insert(80);
    t.insert(25);
    t.insert(90);
    t.insert(75);
    t.insert(10);
    t.insert(15);

    // Recorre el arbol y muestra las llaves en orden ascendente
    cout << "Recorrido del arbol B:\n";
    t.recorrido();
    auto end = chrono::high_resolution_clock::now();                                   // Marca el tiempo de finalización
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);          // Calcula la duración en microsegundos
    cout << "\nTiempo de ejecucion: " << duration.count() << " microsegundos" << endl; // Imprime el tiempo de ejecución

    return 0; // Termina el programa
}
