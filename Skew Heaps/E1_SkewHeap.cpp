#include <iostream>
#include <limits>
#include <memory> // Para usar shared_ptr
#include <stdexcept> // Para manejar excepciones

using namespace std;

// Funcion par validar la entrada de un digito
template <typename T>
T validarDigito(T& n){
    while (cin.fail()) {
        cout << "\nError: El valor ingresado no es valido." << endl;
        cin.clear(); // Limpiar buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discartar entrada invalida
        cout << "Porfavor ingresa un numero valido: "; 
        cin >> n;
    }
    return n;
}

// Clase que representa un nodo en el Skew Heap
class Node {
public:
    int valor; // Valor del nodo
    shared_ptr<Node> izquierda; // Puntero al hijo izquierdo
    shared_ptr<Node> derecha;   // Puntero al hijo derecho

    // Constructor del nodo
    Node(int val) : valor(val), izquierda(nullptr), derecha(nullptr) {}
};

// Clase que representa el Skew Heap
class SkewHeap {
public:
    shared_ptr<Node> raiz; // Puntero a la raiz del heap

    // Constructor del Skew Heap, inicialmente vacio
    SkewHeap() : raiz(nullptr) {}

    // Funcion que combina dos heaps en uno
    shared_ptr<Node> combinar(shared_ptr<Node> heap1, shared_ptr<Node> heap2) {
        // Si uno de los heaps es nulo, se retorna el otro
        if (!heap1) return heap2;
        if (!heap2) return heap1;

        // Garantiza que el heap con el menor valor este en la raiz
        if (heap1->valor > heap2->valor) swap(heap1, heap2);

        // Combina recursivamente el subarbol derecho de heap1 con heap2
        heap1->derecha = combinar(heap1->derecha, heap2);

        // Intercambia los hijos izquierdo y derecho del heap1
        swap(heap1->izquierda, heap1->derecha);

        // Retorna el heap combinado
        return heap1;
    }

    // Inserta un nuevo valor en el heap
    void insertar(int valor) {
        // Crea un nuevo nodo para el valor
        auto nuevoNodo = make_shared<Node>(valor);

        // Combina el nuevo nodo con el heap existente
        raiz = combinar(raiz, nuevoNodo);
    }

    // Inserta varios valores en el heap
    void insertarVarios() {
        short cantidad;
        cout << "\nCuantos elementos quieres ingresar?: "; cin >> cantidad; validarDigito(cantidad);

        for (short i = 0; i < cantidad; i++) {
            int valor;
            cout << "   Ingrese el valor para el elemento #" << i + 1 << ": "; cin >> valor; validarDigito(valor);
            insertar(valor);
        }
    }

    // Funcion auxiliar para imprimir el heap (preorden)
    void imprimir(shared_ptr<Node> nodo) {
        if (!nodo) return; // Si el nodo es nulo, no hace nada
        cout << nodo->valor << " "; // Imprime el valor del nodo
        imprimir(nodo->izquierda);  // Recorre el subarbol izquierdo
        imprimir(nodo->derecha);    // Recorre el subarbol derecho
    }

    // Funcion publica para imprimir el heap desde la raiz
    void imprimir() {
        imprimir(raiz); // Llama a la funcion auxiliar con la raiz
        cout << endl;
    }
};

// Funcion que muestra el menu para interactuar con el Skew Heap
void menuSkewHeap() {
    SkewHeap heap;
    short opcion, valor;

    do{
        cout << "\n------------ MENU SKEW HEAP ------------\n\n";
        cout << "\t1. Insertar elemento\n";
        cout << "\t2. Insertar varios elementos\n";
        cout << "\t3. Mostrar heap (preorden)\n";
        cout << "\t4. Salir\n\n";
        cout << "\tOpcion: "; cin >> opcion; validarDigito(opcion);

        switch(opcion){
            case 1:
                cout << "\nIngrese el valor a insertar: "; cin >> valor; validarDigito(valor);
                heap.insertar(valor);
                break;
            case 2:
                heap.insertarVarios();
                break;
            case 3:
                cout << "\n------------ SKEW HEAP (PREORDEN) ------------\n\n";
                heap.imprimir();
                break;
            case 4:
                cout << "\nSaliendo del programa...\n";
                break;
            default:
                cout << "\nOpcion invalida. Intentelo de nuevo.";
        }
        cout << "\n";
        system("pause");
        system("cls");
    } while (opcion != 4);
}

int main() {
    menuSkewHeap();
    return 0;
}
