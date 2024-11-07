#include <iostream>
using namespace std;

// Definición de la estructura del nodo
struct Node
{
    int data;     // Valor del nodo
    Node *parent; // Puntero al nodo padre
    Node *left;   // Puntero al hijo izquierdo
    Node *right;  // Puntero al hijo derecho
    int color;    // Color del nodo: 0 = NEGRO, 1 = ROJO
};

typedef Node *NodePtr; // Alias para puntero a nodo

// Definición de la clase del árbol rojo-negro
class RedBlackTree
{
private:
    NodePtr root;  // Puntero a la raíz del árbol
    NodePtr TNULL; // Nodo sentinel (nodo nulo) usado para simplificar el manejo de nodos nulos

    // Inicializa un nodo TNULL con valores predeterminados
    void initializeNULLNode(NodePtr node, NodePtr parent)
    {
        node->data = 0;        // Valor predeterminado para TNULL
        node->parent = parent; // Nodo padre
        node->left = nullptr;  // No tiene hijo izquierdo
        node->right = nullptr; // No tiene hijo derecho
        node->color = 0;       // Color negro para el nodo TNULL
    }

    // Realiza una rotación hacia la izquierda en el nodo x
    void leftRotate(NodePtr x)
    {
        NodePtr y = x->right; // El hijo derecho de x se convierte en el nuevo nodo
        x->right = y->left;   // El hijo izquierdo de y se convierte en el hijo derecho de x
        if (y->left != TNULL)
        {
            y->left->parent = x; // Ajusta el padre del antiguo hijo izquierdo de y
        }
        y->parent = x->parent; // El padre de y es el antiguo padre de x
        if (x->parent == nullptr)
        {
            this->root = y; // Si x era la raíz, actualiza la raíz
        }
        else if (x == x->parent->left)
        {
            x->parent->left = y; // Si x era el hijo izquierdo, ajusta el hijo izquierdo del padre
        }
        else
        {
            x->parent->right = y; // Si x era el hijo derecho, ajusta el hijo derecho del padre
        }
        y->left = x;   // x se convierte en el hijo izquierdo de y
        x->parent = y; // Ajusta el padre de x
    }

    // Realiza una rotación hacia la derecha en el nodo x
    void rightRotate(NodePtr x)
    {
        NodePtr y = x->left; // El hijo izquierdo de x se convierte en el nuevo nodo
        x->left = y->right;  // El hijo derecho de y se convierte en el hijo izquierdo de x
        if (y->right != TNULL)
        {
            y->right->parent = x; // Ajusta el padre del antiguo hijo derecho de y
        }
        y->parent = x->parent; // El padre de y es el antiguo padre de x
        if (x->parent == nullptr)
        {
            this->root = y; // Si x era la raíz, actualiza la raíz
        }
        else if (x == x->parent->right)
        {
            x->parent->right = y; // Si x era el hijo derecho, ajusta el hijo derecho del padre
        }
        else
        {
            x->parent->left = y; // Si x era el hijo izquierdo, ajusta el hijo izquierdo del padre
        }
        y->right = x;  // x se convierte en el hijo derecho de y
        x->parent = y; // Ajusta el padre de x
    }

    // Corrige el árbol después de la inserción para mantener las propiedades del árbol rojo-negro
    void insertFix(NodePtr k)
    {
        NodePtr u;                    // Tío del nodo k
        while (k->parent->color == 1) // Mientras el color del padre de k sea rojo
        {
            if (k->parent == k->parent->parent->right) // Si el padre de k es el hijo derecho del abuelo
            {
                u = k->parent->parent->left; // El tío de k es el hijo izquierdo del abuelo
                if (u->color == 1)           // Si el tío es rojo
                {
                    u->color = 0;                 // Cambia el color del tío a negro
                    k->parent->color = 0;         // Cambia el color del padre a negro
                    k->parent->parent->color = 1; // Cambia el color del abuelo a rojo
                    k = k->parent->parent;        // Subir al abuelo para continuar el ajuste
                }
                else
                {
                    if (k == k->parent->left) // Si k es el hijo izquierdo del padre
                    {
                        k = k->parent;  // Subir al padre
                        rightRotate(k); // Realiza rotación a la derecha
                    }
                    k->parent->color = 0;          // Cambia el color del padre a negro
                    k->parent->parent->color = 1;  // Cambia el color del abuelo a rojo
                    leftRotate(k->parent->parent); // Realiza rotación a la izquierda
                }
            }
            else
            {
                u = k->parent->parent->right; // El tío de k es el hijo derecho del abuelo
                if (u->color == 1)            // Si el tío es rojo
                {
                    u->color = 0;                 // Cambia el color del tío a negro
                    k->parent->color = 0;         // Cambia el color del padre a negro
                    k->parent->parent->color = 1; // Cambia el color del abuelo a rojo
                    k = k->parent->parent;        // Subir al abuelo para continuar el ajuste
                }
                else
                {
                    if (k == k->parent->right) // Si k es el hijo derecho del padre
                    {
                        k = k->parent; // Subir al padre
                        leftRotate(k); // Realiza rotación a la izquierda
                    }
                    k->parent->color = 0;           // Cambia el color del padre a negro
                    k->parent->parent->color = 1;   // Cambia el color del abuelo a rojo
                    rightRotate(k->parent->parent); // Realiza rotación a la derecha
                }
            }
            if (k == root) // Si k es la raíz, termina el ajuste
            {
                break;
            }
        }
        root->color = 0; // Asegura que la raíz sea negra
    }

    // Imprime el árbol en un recorrido en orden
    void printHelper(NodePtr root)
    {
        if (root != TNULL) // Si el nodo no es TNULL
        {
            string sColor = root->color ? "ROJO" : "NEGRO";      // Determina el color del nodo
            cout << root->data << " (" << sColor << ")" << endl; // Imprime el valor y el color del nodo
            printHelper(root->left);                             // Imprime el subárbol izquierdo
            printHelper(root->right);                            // Imprime el subárbol derecho
        }
    }

public:
    // Constructor de la clase RedBlackTree
    RedBlackTree()
    {
        TNULL = new Node;       // Crea un nuevo nodo TNULL
        TNULL->color = 0;       // El color de TNULL es negro
        TNULL->left = nullptr;  // No tiene hijo izquierdo
        TNULL->right = nullptr; // No tiene hijo derecho
        root = TNULL;           // La raíz inicial es TNULL
    }

    // Inserta un nuevo valor en el árbol
    void insert(int key)
    {
        NodePtr node = new Node; // Crea un nuevo nodo
        node->parent = nullptr;  // Inicialmente, no tiene padre
        node->data = key;        // Asigna el valor al nodo
        node->left = TNULL;      // Inicialmente, no tiene hijo izquierdo
        node->right = TNULL;     // Inicialmente, no tiene hijo derecho
        node->color = 1;         // Inicialmente, el nodo es rojo

        NodePtr y = nullptr;    // Nodo que servirá como referencia al padre del nuevo nodo
        NodePtr x = this->root; // Comienza desde la raíz

        while (x != TNULL) // Mientras x no sea TNULL
        {
            y = x;                    // Actualiza y al nodo actual
            if (node->data < x->data) // Si el nuevo valor es menor que el valor del nodo actual
            {
                x = x->left; // Mueve a la izquierda
            }
            else
            {
                x = x->right; // Mueve a la derecha
            }
        }

        node->parent = y; // Asigna el padre del nuevo nodo
        if (y == nullptr) // Si no había ningún nodo en el árbol
        {
            root = node; // La raíz es el nuevo nodo
        }
        else if (node->data < y->data) // Si el nuevo nodo es menor que el valor del nodo padre
        {
            y->left = node; // El nuevo nodo se convierte en el hijo izquierdo
        }
        else
        {
            y->right = node; // El nuevo nodo se convierte en el hijo derecho
        }

        if (node->parent == nullptr) // Si el nuevo nodo es la raíz
        {
            node->color = 0; // La raíz debe ser negra
            return;
        }

        if (node->parent->parent == nullptr) // Si el nuevo nodo tiene padre pero no abuelo
        {
            return; // No es necesario hacer ajustes
        }

        insertFix(node); // Ajusta el árbol para mantener las propiedades del árbol rojo-negro
    }

    // Imprime el árbol en un recorrido en orden
    void printTree()
    {
        if (root)
        {
            printHelper(this->root); // Llama a la función auxiliar para imprimir el árbol
        }
    }
};

// Función principal
int main()
{
    RedBlackTree bst; // Crea una instancia del árbol rojo-negro
    bst.insert(20);   // Inserta valores en el árbol
    bst.insert(10);
    bst.insert(5);
    bst.insert(15);
    bst.insert(50);
    bst.insert(35);
    bst.insert(30);
    bst.insert(40);
    bst.insert(60);
    bst.insert(55);
    bst.insert(70);
    bst.insert(65);
    bst.insert(85);
    bst.insert(80);
    bst.insert(90);

    bst.printTree(); // Imprime el árbol

    return 0; // Termina el programa
}
