#include <iostream>
using namespace std;

// Estructura de un nodo en el árbol Rojo y Negro
struct Nodo
{
    int valor;                         // Valor almacenado en el nodo
    bool color;                        // Color del nodo: true para Rojo, false para Negro
    Nodo *izquierda, *derecha, *padre; // Punteros a los hijos izquierdo, derecho y al padre
};

// Función para crear un nuevo nodo con un valor dado
Nodo *CrearNodo(int valor)
{
    Nodo *nodo = new Nodo();                                 // Reserva memoria para un nuevo nodo
    nodo->valor = valor;                                     // Asigna el valor al nodo
    nodo->color = true;                                      // Los nuevos nodos se insertan como rojos por defecto
    nodo->izquierda = nodo->derecha = nodo->padre = nullptr; // Inicialmente, no tiene hijos ni padre
    return nodo;                                             // Devuelve el nuevo nodo creado
}

// Función para obtener el color de un nodo (retorna false si el nodo es nullptr)
bool ColorDeNodo(Nodo *nodo)
{
    return nodo != nullptr && nodo->color;
}

// Función para cambiar el color de un nodo
void CambiarColor(Nodo *nodo, bool color)
{
    nodo->color = color; // Asume que el nodo no es nullptr
}

// Función para realizar una rotación hacia la izquierda
void RotarIzquierda(Nodo *&raiz, Nodo *nodo)
{
    Nodo *derechaHijo = nodo->derecha;      // Toma el hijo derecho del nodo
    nodo->derecha = derechaHijo->izquierda; // El hijo izquierdo del hijo derecho se convierte en el nuevo hijo derecho del nodo

    if (derechaHijo->izquierda != nullptr)    // Si el hijo izquierdo del hijo derecho no es nullptr
        derechaHijo->izquierda->padre = nodo; // Actualiza el padre del hijo izquierdo del hijo derecho

    derechaHijo->padre = nodo->padre; // El padre del hijo derecho se convierte en el padre del nodo

    if (nodo->padre == nullptr)               // Si el nodo es la raíz del árbol
        raiz = derechaHijo;                   // El hijo derecho se convierte en la nueva raíz
    else if (nodo == nodo->padre->izquierda)  // Si el nodo es hijo izquierdo
        nodo->padre->izquierda = derechaHijo; // Actualiza el padre del hijo derecho
    else
        nodo->padre->derecha = derechaHijo; // Si el nodo es hijo derecho

    derechaHijo->izquierda = nodo; // El nodo se convierte en el hijo izquierdo del hijo derecho
    nodo->padre = derechaHijo;     // Actualiza el padre del nodo
}

// Función para realizar una rotación hacia la derecha
void RotarDerecha(Nodo *&raiz, Nodo *nodo)
{
    Nodo *izquierdaHijo = nodo->izquierda;    // Toma el hijo izquierdo del nodo
    nodo->izquierda = izquierdaHijo->derecha; // El hijo derecho del hijo izquierdo se convierte en el nuevo hijo izquierdo del nodo

    if (izquierdaHijo->derecha != nullptr)    // Si el hijo derecho del hijo izquierdo no es nullptr
        izquierdaHijo->derecha->padre = nodo; // Actualiza el padre del hijo derecho del hijo izquierdo

    izquierdaHijo->padre = nodo->padre; // El padre del hijo izquierdo se convierte en el padre del nodo

    if (nodo->padre == nullptr)               // Si el nodo es la raíz del árbol
        raiz = izquierdaHijo;                 // El hijo izquierdo se convierte en la nueva raíz
    else if (nodo == nodo->padre->derecha)    // Si el nodo es hijo derecho
        nodo->padre->derecha = izquierdaHijo; // Actualiza el padre del hijo izquierdo
    else
        nodo->padre->izquierda = izquierdaHijo; // Si el nodo es hijo izquierdo

    izquierdaHijo->derecha = nodo; // El nodo se convierte en el hijo derecho del hijo izquierdo
    nodo->padre = izquierdaHijo;   // Actualiza el padre del nodo
}

// Función para ajustar el árbol después de una inserción para mantener las propiedades del árbol Rojo y Negro
void Ajustar(Nodo *&raiz, Nodo *nodo)
{
    while (nodo != raiz && nodo->padre->color == true)
    {
        if (nodo->padre == nodo->padre->padre->izquierda)
        {
            Nodo *tio = nodo->padre->padre->derecha;

            if (ColorDeNodo(tio) == true)
            {
                CambiarColor(nodo->padre, false);
                CambiarColor(tio, false);
                CambiarColor(nodo->padre->padre, true);
                nodo = nodo->padre->padre;
            }
            else
            {
                if (nodo == nodo->padre->derecha)
                {
                    nodo = nodo->padre;
                    RotarIzquierda(raiz, nodo);
                }
                CambiarColor(nodo->padre, false);
                CambiarColor(nodo->padre->padre, true);
                RotarDerecha(raiz, nodo->padre->padre);
            }
        }
        else // Simétrico para el caso donde el padre es hijo derecho del abuelo
        {
            Nodo *tio = nodo->padre->padre->izquierda;

            if (ColorDeNodo(tio) == true)
            {
                CambiarColor(nodo->padre, false);
                CambiarColor(tio, false);
                CambiarColor(nodo->padre->padre, true);
                nodo = nodo->padre->padre;
            }
            else
            {
                if (nodo == nodo->padre->izquierda)
                {
                    nodo = nodo->padre;
                    RotarDerecha(raiz, nodo);
                }
                CambiarColor(nodo->padre, false);
                CambiarColor(nodo->padre->padre, true);
                RotarIzquierda(raiz, nodo->padre->padre);
            }
        }
    }
    CambiarColor(raiz, false); // Asegura que la raíz sea negra
}

// Función para insertar un nodo en el árbol
void Insertar(Nodo *&raiz, int valor)
{
    Nodo *nuevoNodo = CrearNodo(valor); // Crea un nuevo nodo con el valor dado
    Nodo *y = nullptr;                  // Nodo padre temporal
    Nodo *x = raiz;                     // Empieza desde la raíz

    while (x != nullptr)
    { // Encuentra la posición adecuada para insertar el nuevo nodo
        y = x;
        if (nuevoNodo->valor < x->valor)
            x = x->izquierda;
        else
            x = x->derecha;
    }

    nuevoNodo->padre = y; // Establece el padre del nuevo nodo

    if (y == nullptr)
    {                     // Si el árbol estaba vacío
        raiz = nuevoNodo; // El nuevo nodo es la raíz
    }
    else if (nuevoNodo->valor < y->valor)
    {                             // Si el valor es menor que el del nodo padre
        y->izquierda = nuevoNodo; // Inserta el nuevo nodo como hijo izquierdo
    }
    else
    {
        y->derecha = nuevoNodo; // Inserta el nuevo nodo como hijo derecho
    }

    Ajustar(raiz, nuevoNodo); // Ajusta el árbol para mantener las propiedades del árbol Rojo y Negro
}

// Función para imprimir el árbol en orden (recorrido in-order)
void ImprimirEnOrden(Nodo *raiz)
{
    if (raiz != nullptr)
    {                                                                                   // Si el nodo no es nullptr
        ImprimirEnOrden(raiz->izquierda);                                               // Recorre el subárbol izquierdo
        cout << raiz->valor << " (" << (raiz->color ? "Rojo" : "Negro") << ")" << endl; // Imprime el valor y el color del nodo
        ImprimirEnOrden(raiz->derecha);                                                 // Recorre el subárbol derecho
    }
}

// Función principal
int main()
{
    Nodo *raiz = nullptr; // Inicializa la raíz del árbol

    // Inserta una serie de valores en el árbol
    int valores[] = {50, 70, 40, 30, 65, 35, 85, 20, 90, 10, 80, 5, 15, 60, 55};
    for (int valor : valores)
    {
        Insertar(raiz, valor);
    }

    // Imprime el árbol en orden para verificar su estructura
    ImprimirEnOrden(raiz);

    return 0; // Fin del programa
}
