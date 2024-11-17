#include <iostream>
#include <cstdlib> // Para la funcion rand()

using namespace std;

// Estructura del nodo del Treap
struct Nodo{
    short clave;        // Valor de la clave
    short prioridad;    // Valor de la prioridad (aleatorio)
    Nodo* izquierdo;    // Puntero al hijo izquierdo
    Nodo* derecho;      // Puntero al hijo derecho
};

// Crear un nuevo nodo
Nodo* CrearNodo(short clave, short prioridad){
    Nodo* nuevo_nodo = new Nodo;
    nuevo_nodo->clave = clave;
    nuevo_nodo->prioridad = prioridad;
    nuevo_nodo->izquierdo = nullptr;
    nuevo_nodo->derecho = nullptr;
    return nuevo_nodo;
}

// Funcion para rotar hacia la derecha
Nodo* RotarDerecha(Nodo* nodo){
    Nodo* nuevo_nodo = nodo->izquierdo;
    nodo->izquierdo = nuevo_nodo->derecho;
    nuevo_nodo->derecho = nodo;
    return nuevo_nodo;
}

// Funcion para rotar hacia la izquierda
Nodo* RotarIzquierda(Nodo* nodo){
    Nodo* nuevo_nodo = nodo->derecho;
    nodo->derecho = nuevo_nodo->izquierdo;
    nuevo_nodo->izquierdo = nodo;
    return nuevo_nodo;
}

Nodo* Insertar(Nodo* nodo, short clave, short prioridad);
void InsertarVarios(Nodo*& raiz);
Nodo* Eliminar(Nodo* nodo, short clave);
Nodo* Buscar(Nodo* nodo, short clave);

// Funcion para insertar un nuevo nodo en el Treap
Nodo* Insertar(Nodo* nodo, short clave, short prioridad){
    // Crear un nuevo nodo si es NULL
    if (nodo == nullptr){ 
        return CrearNodo(clave, prioridad);
    }

    // Verificar si la clave ya existe utilizando la funcion Buscar
    if (Buscar(nodo, clave) != nullptr) {
        // Si la clave ya existe, no insertamos el nodo
        cout << "La clave " << clave << " ya existe en el Treap. No se puede insertar.\n";
        return nodo;
    }
    
    // Insercion segun la clave (BST)
    if (clave < nodo->clave){
        nodo->izquierdo = Insertar(nodo->izquierdo, clave, prioridad);
        // Verifica la prioridad para rotar
        if (nodo->izquierdo->prioridad > nodo->prioridad){
            nodo = RotarDerecha(nodo);
        }
    } else{
        nodo->derecho = Insertar(nodo->derecho, clave, prioridad);
        // Verifica la prioridad para rotar
        if (nodo->derecho->prioridad > nodo->prioridad){
            nodo = RotarIzquierda(nodo);
        }
    }
    
    return nodo; // Retorna la nueva raiz
}

// Funcion para insertar varios nodos en el Treap
void InsertarVarios(Nodo*& raiz){
    short cantidad, clave;
    cout << "Cuantos nodos quieres insertar?: "; cin >> cantidad;
    for(short i = 0; i < cantidad; i++) {
        cout << "Ingrese la clave del nodo #" << (i + 1) << ": "; cin >> clave;
        // Verifica si la clave ya existe antes de intentar insertarla
        if (Buscar(raiz, clave) != nullptr){
            cout << "La clave " << clave << " ya existe en el Treap. No se puede insertar.\n";
        } else {
            short prioridad = rand() % 100; // Genera una prioridad aleatoria
            raiz = Insertar(raiz, clave, prioridad);
            cout << "\t--- NODO ---\nClave: " << clave << "\t| Prioridad: " << prioridad << "\n";
        }
    }
}

// Funcion para eliminar un nodo en el Treap
Nodo* Eliminar(Nodo* nodo, short clave){
    // Si el nodo es NULL, no hay nada que eliminar
    if (nodo == nullptr){
        cout << "El nodo que buscas no existe en el Treap." << endl;
        return nullptr;
    }

    // Buscar el nodo a eliminar segun la clave (BST)
    if (clave < nodo->clave){
        nodo->izquierdo = Eliminar(nodo->izquierdo, clave);
    } else if (clave > nodo->clave){
        nodo->derecho = Eliminar(nodo->derecho, clave);
    } else{
        // Nodo encontrado
        cout << "Nodo con clave " << clave << " eliminado." << endl; // Imprimir mensaje cuando se elimina el nodo
        if (nodo->izquierdo == nullptr){
            // Caso donde solo hay hijo derecho
            Nodo* temp = nodo->derecho;
            delete nodo;
            return temp;
        } else if (nodo->derecho == nullptr){
            // Caso donde solo hay hijo izquierdo
            Nodo* temp = nodo->izquierdo;
            delete nodo;
            return temp;
        } else {
            // Nodo con dos hijos, decidir rotacion por prioridad
            if (nodo->izquierdo->prioridad > nodo->derecho->prioridad){
                nodo = RotarDerecha(nodo);
                nodo->derecho = Eliminar(nodo->derecho, clave);
            } else {
                nodo = RotarIzquierda(nodo);
                nodo->izquierdo = Eliminar(nodo->izquierdo, clave);
            }
        }
    }
    return nodo; // Retorna la nueva raiz
}

// Funcion para buscar un nodo en el Treap
Nodo* Buscar(Nodo* nodo, short clave){
    if (nodo == nullptr || nodo->clave == clave){
        return nodo;
    }

    // Buscar recursivamente segun la clave
    if (clave < nodo->clave){
        return Buscar(nodo->izquierdo, clave);
    } else {
        return Buscar(nodo->derecho, clave);
    }
}

// Funcion auxiliar para imprimir espacios
void ImprimirEspacios(short nivel){
    for (short i = 0; i < nivel; i++) {
        cout << "\t";  // Espacios para la indentacion
    }
}

// Funcion para mostrar el Treap de manera grafica
void MostrarGrafico(Nodo* nodo, short nivel = 0){
    if (nodo == nullptr){
        return;
    }
    // Primero, mostrar el subarbol derecho (con mayor profundidad)
    MostrarGrafico(nodo->derecho, nivel + 1);
    // Imprimir el nodo actual
    ImprimirEspacios(nivel);
    cout << "|--" << nodo->clave << " (p: " << nodo->prioridad << ")\n";
    // Luego, mostrar el subarbol izquierdo
    MostrarGrafico(nodo->izquierdo, nivel + 1);
}

void Menu(){
    Nodo* raiz = nullptr;
    Nodo* nodoEncontrado = nullptr;
    short opcion, clave;
    
    do{
        cout << "\n------------ MENU TREAP ------------\n";
        cout << "\n\t1. Insertar nodo";
        cout << "\n\t2. Insertar varios nodos";
        cout << "\n\t3. Buscar nodo";
        cout << "\n\t4. Eliminar nodo";
        cout << "\n\t5. Mostrar Treap";
        cout << "\n\t6. Salir";
        cout << "\n\n\tOpcion: "; cin >> opcion;
        cout << "\n";

        switch (opcion){
            case 1:
                cout << "\nIngrese la clave del nodo a insertar: "; cin >> clave;
                raiz = Insertar(raiz, clave, rand() % 100); // Inserta con prioridad aleatoria
                break;
            case 2:
                InsertarVarios(raiz);
                break;
            case 3:
                cout << "\nIngrese la clave del nodo a buscar: "; cin >> clave;
                nodoEncontrado = Buscar(raiz, clave); // Llama a la funcion de busqueda
                if (nodoEncontrado != nullptr){
                    // Si el nodo se encuentra, muestra el nodo
                    cout << "Nodo encontrado: " << nodoEncontrado->clave << " (prioridad: " << nodoEncontrado->prioridad << ")" << endl;
                } else{
                    cout << "El nodo con clave " << clave << " no se encuentra en el Treap." << endl;
                }
                break;
            case 4:
                cout << "\nIngrese la clave del nodo a eliminar: "; cin >> clave;
                raiz = Eliminar(raiz, clave);
                break;
            case 5:
                cout << "\n------------ TREAP ------------\n";
                MostrarGrafico(raiz);
                break;
            case 6:
                cout << "\nSaliendo del programa...\n";
                break;
            default:
                cout << "\nOpcion invalida. Intentelo de nuevo.\n";
        }
        system("pause");
        system("cls");
    } while(opcion != 6);
}

// Funcion principal
int main() {
    Menu();
    return 0;
}