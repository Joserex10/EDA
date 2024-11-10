#include <iostream>
#include <vector>
#include <limits>

using namespace std;

// Estructura del Nodo
struct Nodo
{
    short valor;
    short indice; // posicion del array
};

// Inicializacion del Min Heap
struct MinHeap
{
    vector<Nodo> array; // lista de nodos
    short tamaño = 0;   // numero actual de elementos en el heap
};

// Funcion para crear un nodo
Nodo CrearNodo(short valor, short indice)
{
    Nodo nuevoNodo;
    nuevoNodo.valor = valor;
    nuevoNodo.indice = indice;
    return nuevoNodo;
}

// Funcion para crear un Min Heap
MinHeap CrearMinHeap(short tamaño)
{
    MinHeap heap;
    heap.array.resize(tamaño);
    return heap;
}

// Funciones para las operaciones del MinHeap
void insertar(MinHeap &heap, short valor);
short extraerMin(MinHeap &heap);
void disminuirClave(MinHeap &heap, short indice, short nuevaClave);
void siftUp(MinHeap &heap, short indice);
void siftDown(MinHeap &heap, short indice);
void insertarVarios(MinHeap &heap);
void imprimirHeap(const MinHeap &heap);

// Funcion para insertar un elemento
void insertar(MinHeap &heap, short valor)
{
    if (heap.tamaño == static_cast<int>(heap.array.size()))
    { // Comprobar si el heap esta lleno
        cout << "\nHeap esta lleno" << endl;
        return; // Salir si el heap esta lleno
    }
    // Crear el nodo y agregarlo en la posición correcta
    Nodo nodo = CrearNodo(valor, heap.tamaño); // Asignar el valor y el indice
    heap.array[heap.tamaño] = nodo;            // Insertar en el array
    heap.tamaño++;                             // Incrementar el tamaño del heap
    siftUp(heap, heap.tamaño - 1);             // Llamar a siftUp con el indice correcto
}

// Funcion para Sift Up (Reestructuracion)
void siftUp(MinHeap &heap, short indice)
{
    short padre = (indice - 1) / 2; // Calcular el indice del padre

    // Mientras el nodo no este en la raiz y su valor sea menor que el del padre
    while (indice > 0 && heap.array[indice].valor < heap.array[padre].valor)
    {
        swap(heap.array[indice], heap.array[padre]); // Intercambiar los valores
        indice = padre;                              // Actualizar el indice para el siguiente ciclo
        padre = (indice - 1) / 2;                    // Recalcular el indice del nuevo padre
    }
}

// Funcion para extrar el minimo
short extraerMin(MinHeap &heap)
{
    if (heap.tamaño == 0)
    { // Verificar si el heap esta vacio
        cout << "\nHeap esta vacio" << endl;
        return -1; // Valor para indicar que no hay elementos
    }
    short minValor = heap.array[0].valor; // Guardar el valor minimo

    // Reemplazar el primer elemento con el ultimo elemento
    heap.array[0] = heap.array[heap.tamaño - 1];
    heap.tamaño--;     // Reducir el tamaño del heap
    siftDown(heap, 0); // Reestructurar el heap con siftDown
    return minValor;   // Retornar el valor minimo extraido
}

// Funcion para SiftDown (Reestructuracion)
void siftDown(MinHeap &heap, short indice)
{
    short menor = indice;
    short izquierda = 2 * indice + 1;
    short derecha = 2 * indice + 2;

    if (izquierda < heap.tamaño && heap.array[izquierda].valor < heap.array[menor].valor)
        menor = izquierda;
    if (derecha < heap.tamaño && heap.array[derecha].valor < heap.array[menor].valor)
        menor = derecha;
    if (menor != indice)
    {
        swap(heap.array[indice], heap.array[menor]);
        siftDown(heap, menor);
    }
}

// Funcion para Disminuir clave
void disminuirClave(MinHeap &heap, short indice, short nuevaClave)
{
    // Comprobar si la nueva clave es mayor que la clave actual, lo cual no esta permitido
    if (nuevaClave > heap.array[indice].valor)
    {
        cout << "\nLa nueva clave es mayor que la actual" << endl;
        return; // Si es mayor retorna
    }
    heap.array[indice].valor = nuevaClave; // Asignar la nueva clave al nodo
    siftUp(heap, indice);                  // Usamos siftUp para mover el nodo hacia la raíz si es necesario
    cout << "\nDisminucion realizada correctamente." << endl;
}

// Función para insertar varios elementos en el MinHeap
void insertarVarios(MinHeap &heap)
{
    short cantidad;
    cout << "\nCuantos elementos deseas ingresar? ";
    cin >> cantidad;

    for (short i = 0; i < cantidad; i++)
    {
        short valor;
        cout << "   Ingrese el valor para el elemento " << i + 1 << ": ";
        cin >> valor;
        insertar(heap, valor);
    }
}

// Función para imprimir el contenido del MinHeap
void imprimirHeap(const MinHeap &heap)
{
    if (heap.tamaño == 0)
    {
        cout << "\nHeap esta vacio";
    }
    else
    {
        cout << "\nContenido del MinHeap:" << endl;
        for (short i = 0; i < heap.tamaño; i++)
        {
            cout << "   Valor: " << heap.array[i].valor << ", Indice: " << heap.array[i].indice << endl;
        }
    }
}

// Menú principal
void menuMinHeap()
{
    MinHeap heap = CrearMinHeap(100); // Tamaño máximo del heap
    short opcion, valor, indice, nuevaClave;

    do
    {
        cout << "\n------------ MENU MIN HEAP ------------\n\n";
        cout << "\t1. Insertar elemento\n";
        cout << "\t2. Insertar varios elementos\n";
        cout << "\t3. Extraer el minimo\n";
        cout << "\t4. Disminuir clave\n";
        cout << "\t5. Mostrar heap\n";
        cout << "\t6. Salir\n\n";
        cout << "\tOpcion: ";
        cin >> opcion;

        switch (opcion)
        {
        case 1:
            cout << "\nIngrese el valor a insertar: ";
            cin >> valor;
            insertar(heap, valor);
            break;
        case 2:
            insertarVarios(heap);
            break;
        case 3:
            valor = extraerMin(heap);
            if (valor != -1)
            { // Verificar que el heap no está vacío
                cout << "\nElemento minimo extraido: " << valor << endl;
            }
            else
            {
                cout << "No se pudo extraer el minimo, dado que no hay elementos." << endl;
            }
            break;
        case 4:
            cout << "\nIngrese el indice y la nueva clave: " << endl;
            cout << "\tIndice: ";
            cin >> indice;
            cout << "\tNueva clave: ";
            cin >> nuevaClave;
            disminuirClave(heap, indice, nuevaClave);
            break;
        case 5:
            imprimirHeap(heap);
            break;
        case 6:
            cout << "\nSaliendo del programa...\n";
            break;
        default:
            cout << "\nOpcion invalida. intentelo de nuevo. ";
        }
        cout << "\n";
        system("pause");
        system("cls");
    } while (opcion != 6);
}

int main()
{
    menuMinHeap();
    return 0;
}