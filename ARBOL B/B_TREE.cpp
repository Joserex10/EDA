#include <iostream>
#include <cstdlib> // Para la función malloc
#include <limits>
using namespace std;

// Estructura para un nodo del árbol B
struct BTreeNode
{
    int *claves;   // Arreglo de claves     (Datos del nodo)
    int t;         // Grado mínimo          (Grado minimo de datos en un nodo)
    BTreeNode **C; // Arreglo de punteros a los hijos
    int n;         // Número actual de claves
    bool hoja;     // Verdadero si es un nodo hoja      (Verifica si es una hoja o no)
};

// Función para crear un nuevo nodo BTreeNode
BTreeNode *createNode(int t, bool hoja)
{
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));
    node->t = t;
    node->hoja = hoja;
    node->claves = (int *)malloc(sizeof(int) * (2 * t - 1));       // Asignar espacio en memoria para las claves (Se usa malloc por que no sabemos cuanta memoria va a necesitar)
    node->C = (BTreeNode **)malloc(sizeof(BTreeNode *) * (2 * t)); // Asignar espacio en memoria para los hijos  (Se usa malloc por que no sabemos cuanta memoria va a necesitar)
    node->n = 0;                                                   // Inicialmente, el número de claves es 0     (El numero de claves inicia en 0 por defecto)
    return node;
}

// Función para recorrer el árbol B y mostrar las claves en orden
void recorrer(BTreeNode *node)
{
    if (node == NULL)
        return; // Si el nodo es NULL, salir de la función
    int i;
    for (i = 0; i < node->n; i++)
    {
        if (!node->hoja) // Si el nodo no es hoja, recorrer el subárbol hijo antes de mostrar la clave
            recorrer(node->C[i]);
        cout << " " << node->claves[i]; // Mostrar la clave osea el dato
    }
    if (!node->hoja) // Recorrer el último subárbol hijo
        recorrer(node->C[i]);
}

// Función para buscar una clave en el árbol B
BTreeNode *buscar(BTreeNode *node, int k)
{
    int i = 0;
    while (i < node->n && k > node->claves[i]) // Encontrar la posición de la clave o la posición donde debería estar
        i++;
    if (node->claves[i] == k) // Si la clave está en el nodo actual, devolver el nodo (Encontro el dato)
        return node;
    if (node->hoja) // Si es un nodo hoja, la clave no está en el árbol (No encontro el dato)
        return NULL;
    return buscar(node->C[i], k); // Buscar en el subárbol hijo adecuado
}

// Función para dividir un nodo hijo
void dividirHijo(BTreeNode *x, int i, BTreeNode *y)
{
    int t = y->t;
    BTreeNode *z = createNode(t, y->hoja); // Crear un nuevo nodo z que contendrá t-1 claves de y
    z->n = t - 1;                          // z tendrá t-1 claves

    for (int j = 0; j < t - 1; j++) // Copiar las últimas t-1 claves de y a z
        z->claves[j] = y->claves[j + t];

    if (!y->hoja)
    { // Si y no es un nodo hoja, copiar los últimos t hijos de y a z
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j + t];
    }

    y->n = t - 1; // Reducir el número de claves en y

    for (int j = x->n; j >= i + 1; j--) // Mover los hijos de x para hacer espacio para z
        x->C[j + 1] = x->C[j];
    x->C[i + 1] = z; // Enlazar z con x

    for (int j = x->n - 1; j >= i; j--) // Mover las claves de x para hacer espacio para la clave media de y
        x->claves[j + 1] = x->claves[j];
    x->claves[i] = y->claves[t - 1]; // Copiar la clave media de y a x

    x->n = x->n + 1; // Incrementar el número de claves en x
}

// Función para insertar una clave en un nodo no lleno
void insertarVacio(BTreeNode *node, int k)
{
    int i = node->n - 1; // Empezar desde la última clave

    if (node->hoja)
    { // Si es un nodo hoja
        while (i >= 0 && node->claves[i] > k)
        { // Desplazar las claves para hacer espacio para la nueva clave
            node->claves[i + 1] = node->claves[i];
            i--;
        }
        node->claves[i + 1] = k; // Insertar la nueva clave en su lugar
        node->n = node->n + 1;   // Incrementar el número de claves en el nodo
    }
    else
    {                                         // Si no es un nodo hoja
        while (i >= 0 && node->claves[i] > k) // Encontrar el hijo adecuado para insertar la clave
            i--;
        if (node->C[i + 1]->n == 2 * node->t - 1)
        {                                             // Si el nodo hijo está lleno
            dividirHijo(node, i + 1, node->C[i + 1]); // Dividir el hijo
            if (node->claves[i + 1] < k)              // Decidir en cuál de los dos hijos insertar la clave
                i++;
        }
        insertarVacio(node->C[i + 1], k); // Insertar la clave en el nodo hijo adecuado
    }
}

// Función para insertar una clave en el árbol B
void insertar(BTreeNode **raiz, int t, int k)
{
    if (*raiz == NULL) // Si el árbol está vacío
    {
        *raiz = createNode(t, true); // Crear un nuevo nodo raíz
        (*raiz)->claves[0] = k;      // Insertar la primera clave
        (*raiz)->n = 1;              // El nodo tiene una clave
    }
    else
    {
        if ((*raiz)->n == 2 * t - 1)
        {                                        // Si la raíz está llena
            BTreeNode *s = createNode(t, false); // Crear un nuevo nodo s
            s->C[0] = *raiz;                     // Hacer que la antigua raíz sea el primer hijo de s
            dividirHijo(s, 0, *raiz);            // Dividir la antigua raíz y mover una clave a s

            int i = 0;
            if (s->claves[0] < k) // Decidir cuál de los dos hijos de s necesita la nueva clave
                i++;
            insertarVacio(s->C[i], k); // Insertar la clave en el hijo adecuado
            *raiz = s;                 // Hacer que s sea la nueva raíz
        }
        else
        {
            insertarVacio(*raiz, k); // Insertar la clave en la raíz actual si no está llena
        }
    }
}

// Función para eliminar un nodo del árbol B
void eliminar(BTreeNode *&raiz)
{
    cout << "Esta funcion aun no esta implementada." << endl;
}

// Menú interactivo para que el usuario elija qué hacer
void menu()
{
    BTreeNode *raiz = NULL; // Inicialmente, el árbol está vacío
    int t = 3;              // Grado mínimo del árbol B
    int option, value;

    do
    {

        cout << "\nOpciones del Arbol B:\n";
        cout << "1. Insertar clave\n";
        cout << "2. Buscar clave\n";
        cout << "3. Recorrer arbol\n";
        cout << "4. Eliminar clave\n";
        cout << "5. Salir\n";
        cout << "Elige una opcion: ";
        cin >> option;

        while (cin.fail() || option != static_cast<int>(option) || option < 0 || option > 5) // Verificar si los valores ingresados son números enteros
        {
            if (option > 5)
            {
                cout << "\nDebe ingresar un numero del menu (1-5)\n";
                system("pause");
                system("cls");
                menu(); // Volver a pedir los datos
            }
            else
            {
                cin.clear();                                         // Limpiar el estado de error del flujo de entrada
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descartar la entrada inválida hasta la nueva línea
                cout << "\nDebe ingresar solo numeros enteros. Intentelo de nuevo: \n";
                system("pause");
                system("cls");
                menu(); // Volver a pedir los datos
            }
        }

        switch (option)
        {
        case 1:
            cout << "Introduce el valor a insertar: ";
            cin >> value;
            insertar(&raiz, t, value);
            cout << "Clave insertada.\n";
            system("pause");
            system("cls");
            break;
        case 2:
            cout << "Introduce el valor a buscar: ";
            cin >> value;
            if (buscar(raiz, value) != NULL)
            {
                cout << "Clave " << value << " encontrada.\n";
                system("pause");
                system("cls");
            }
            else
            {
                cout << "Clave " << value << " no encontrada.\n";
                system("pause");
                system("cls");
            }
            break;
        case 3:
            cout << "Recorrido del arbol B:\n";
            recorrer(raiz);
            cout << endl;
            system("pause");
            system("cls");
            break;
        case 4:
            eliminar(raiz); // Por ahora, solo mostramos un mensaje
            system("pause");
            system("cls");
            break;
        case 5:
            cout << "Saliendo del programa.\n";
            break;
        }
    } while (option != 5);
    exit(0);
}

// Función principal
int main()
{
    menu();
    return 0;
}
