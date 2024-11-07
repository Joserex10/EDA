#include <iostream>
using namespace std;

void menu();
int queue[5];
int adelante = 0, atras = 0, opcion, valor;

int main()
{

    menu();
    return 0;
}
void menu()
{
    do
    {
        cout << "\nOPCIONES:\n\n";
        cout << "1. Ingresar\n2. Borrar\n3. Mostrar\n4. Salir\n";
        cin >> opcion;

        switch (opcion)
        {
        case 1:
            if ((atras + 1) % 5 == adelante)
            {
                cout << "Cola llena\n\n";
            }
            else
            {
                cout << "\nIngresa valor: ";
                cin >> valor;
                queue[atras] = valor;
                atras = (atras + 1) % 5;
                cout << "\nValor " << valor << " ingresado correctamente" << endl;
            }
            system("pause");
            break;
        case 2:
            if (adelante == atras)
            {
                cout << "\nCola vacia\n";
            }
            else
            {
                cout << "\nValor eliminado: " << queue[adelante] << "\n\n";
                adelante = (adelante + 1) % 5;
            }
            system("pause");
            break;
        case 3:
            cout << "Mostrando la cola:" << endl;
            cout << "\n\n";
            cout << "Cola: ";
            for (int i = adelante; i != atras; i = (i + 1) % 5)
            {
                cout << queue[i] << " ";
            }
            cout << "\n\n";
            system("pause");
            break;
        case 4:
            break;
        }
    } while (opcion != 4);
}
