#include <iostream>
using namespace std;

int main() {
    int queue[5];
    int adelante = 0, atras = 0, opcion, valor;

    while (true) {
        cout << "1. Ingresar\n2. Borrar\n3. Mostrar\n4. Salir\n";
        cin >> opcion;

        if (opcion == 1) {  // Ingresar
            if ((atras + 1) % 5 == adelante) {
                cout << "Cola llena\n";
            } else {
                cout << "Ingresa valor: ";
                cin >> valor;
                queue[atras] = valor;
                atras = (atras + 1) % 5;
            }
        } else if (opcion == 2) {  // Borrar
            if (adelante == atras) {
                cout << "Cola vacía\n";
            } else {
                cout << "Valor eliminado: " << queue[adelante] << "\n";
                adelante = (adelante + 1) % 5;
            }
        } else if (opcion == 3) {  // Mostrar cola
            cout << "Cola: ";
            for (int i = adelante; i != atras; i = (i + 1) % 5) {
                cout << queue[i] << " ";
            }
            cout << "\n";
        } else if (opcion == 4) {  // Salir
            break;
        }
    }
    return 0;
}
