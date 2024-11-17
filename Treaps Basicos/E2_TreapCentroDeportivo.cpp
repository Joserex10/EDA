#include <iostream>
#include <cstdlib> // Para la funcion rand()
#include <iomanip> // Para formatear la salida
#include <limits>

using namespace std;

// Estructura del nodo del Treap
struct Nodo
{
    short inicio;    // Inicio del intervalo (en minutos)
    short fin;       // Fin del intervalo (en minutos)
    short prioridad; // Valor de la prioridad (aleatorio)
    Nodo *izquierdo; // Puntero al hijo izquierdo
    Nodo *derecho;   // Puntero al hijo derecho
};

// Función para leer un número entero con validación
int LeerNumero()
{
    int numero;
    while (true)
    {
        cin >> numero;
        if (cin.fail())
        {
            cin.clear();                                         // Limpia el flag de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignora hasta el fin de línea
            cout << "Entrada inválida. Por favor, ingrese un número entero.\n";
        }
        else
        {
            return numero;
        }
    }
}

// Función para leer horas y minutos con validación
bool LeerHoraMinuto(int &hora, int &minuto)
{
    do
    {
        cout << "Ingrese la hora y el minuto (HH MM): ";
        hora = LeerNumero();
        minuto = LeerNumero();

        // Validar hora y minuto
        if (hora < 0 || hora > 23 || minuto < 0 || minuto > 59)
        {
            cout << "Hora o minuto fuera de rango. Intente nuevamente.\n";
        }
        else
        {
            return true; // Entrada válida
        }
    } while (true); // Continuar pidiendo la entrada hasta que sea válida
}

// Funcion para convertir la hora y minuto en minutos
int HoraAMinutos(int hora, int minuto)
{
    return hora * 60 + minuto;
}

// Funcion para convertir minutos a formato "HH:MM"
void MinutosAHora(int minutosTotales, int &hora, int &minuto)
{
    hora = minutosTotales / 60;
    minuto = minutosTotales % 60;
}

// Crear un nuevo nodo
Nodo *CrearNodo(short inicio, short fin, short prioridad)
{
    Nodo *nuevo_nodo = new Nodo;
    nuevo_nodo->inicio = inicio;
    nuevo_nodo->fin = fin;
    nuevo_nodo->prioridad = prioridad;
    nuevo_nodo->izquierdo = nullptr;
    nuevo_nodo->derecho = nullptr;
    return nuevo_nodo;
}

// Funcion para rotar hacia la derecha
Nodo *RotarDerecha(Nodo *nodo)
{
    Nodo *nuevo_nodo = nodo->izquierdo;
    nodo->izquierdo = nuevo_nodo->derecho;
    nuevo_nodo->derecho = nodo;
    return nuevo_nodo;
}

// Funcion para rotar hacia la izquierda
Nodo *RotarIzquierda(Nodo *nodo)
{
    Nodo *nuevo_nodo = nodo->derecho;
    nodo->derecho = nuevo_nodo->izquierdo;
    nuevo_nodo->izquierdo = nodo;
    return nuevo_nodo;
}

// Insertar un nodo en el Treap
Nodo *Insertar(Nodo *nodo, int inicio, int fin, short prioridad)
{
    if (nodo == nullptr)
    {
        return CrearNodo(inicio, fin, prioridad);
    }

    // Comprobar si hay solapamiento de intervalos
    if (inicio < nodo->fin && fin > nodo->inicio)
    {
        cout << "El horario ingresado se cruza con una reserva existente.\n";
        return nodo;
    }

    // Insertar el nodo en el lugar adecuado basado en el tiempo
    if (inicio < nodo->inicio)
    {
        nodo->izquierdo = Insertar(nodo->izquierdo, inicio, fin, prioridad);
        if (nodo->izquierdo->prioridad > nodo->prioridad)
        {
            nodo = RotarDerecha(nodo);
        }
    }
    else
    {
        nodo->derecho = Insertar(nodo->derecho, inicio, fin, prioridad);
        if (nodo->derecho->prioridad > nodo->prioridad)
        {
            nodo = RotarIzquierda(nodo);
        }
    }

    return nodo;
}

// Funcion para eliminar una reserva en el Treap
Nodo *Eliminar(Nodo *nodo, short inicio, short fin)
{
    // Si el nodo es NULL, no hay nada que eliminar
    if (nodo == nullptr)
    {
        cout << "El horario que desea eliminar no existe en las reservas.\n";
        return nullptr;
    }

    // Buscar el nodo a eliminar segun el inicio del intervalo (BST)
    if (inicio < nodo->inicio)
    {
        nodo->izquierdo = Eliminar(nodo->izquierdo, inicio, fin);
    }
    else if (inicio > nodo->inicio)
    {
        nodo->derecho = Eliminar(nodo->derecho, inicio, fin);
    }
    else
    {
        // Nodo encontrado
        if (nodo->inicio == inicio && nodo->fin == fin)
        {
            cout << "Reserva eliminada correctamente (Revisar la opcion 4. Mostrar Reservas).\n";
            if (nodo->izquierdo == nullptr)
            {
                Nodo *temp = nodo->derecho;
                delete nodo;
                return temp;
            }
            else if (nodo->derecho == nullptr)
            {
                Nodo *temp = nodo->izquierdo;
                delete nodo;
                return temp;
            }
            else
            {
                // Nodo con dos hijos, decidir rotacion por prioridad
                if (nodo->izquierdo->prioridad > nodo->derecho->prioridad)
                {
                    nodo = RotarDerecha(nodo);
                    nodo->derecho = Eliminar(nodo->derecho, inicio, fin);
                }
                else
                {
                    nodo = RotarIzquierda(nodo);
                    nodo->izquierdo = Eliminar(nodo->izquierdo, inicio, fin);
                }
            }
        }
        else
        {
            cout << "El horario solicitado no coincide exactamente con ninguna reserva.\n";
        }
    }
    return nodo; // Retorna la nueva raiz
}

// Funcion para buscar una reserva en el Treap
Nodo *Buscar(Nodo *nodo, short inicio, short fin)
{
    if (nodo == nullptr)
    {
        return nullptr;
    }

    // Verificar si hay solapamiento con el nodo actual
    if (inicio < nodo->fin && fin > nodo->inicio)
    {
        return nodo;
    }

    // Buscar recursivamente
    if (inicio < nodo->inicio)
    {
        return Buscar(nodo->izquierdo, inicio, fin);
    }
    else
    {
        return Buscar(nodo->derecho, inicio, fin);
    }
}

// Funcion para mostrar las reservas en orden
void MostrarReservas(Nodo *nodo)
{
    if (nodo == nullptr)
    {
        return;
    }

    // Primero, recorremos el subarbol izquierdo
    MostrarReservas(nodo->izquierdo);

    // Luego mostramos la reserva actual (ordenada por inicio)
    int inicioHora, inicioMinuto, finHora, finMinuto;
    MinutosAHora(nodo->inicio, inicioHora, inicioMinuto);
    MinutosAHora(nodo->fin, finHora, finMinuto);

    // Mostrar la reserva en formato HH:MM
    cout << setw(2) << setfill('0') << inicioHora << ":" << setw(2) << setfill('0') << inicioMinuto
         << " - " << setw(2) << setfill('0') << finHora << ":" << setw(2) << setfill('0') << finMinuto
         << " (p: " << nodo->prioridad << ")\n";

    // Finalmente, recorremos el subarbol derecho
    MostrarReservas(nodo->derecho);
}

// Funcion principal para gestionar el menu
void Menu()
{
    Nodo *raiz = nullptr;
    Nodo *reserva = nullptr;
    short opcion, inicio, fin, prioridad;
    int horaInicio, minutoInicio, horaFin, minutoFin;

    do
    {
        cout << "\n------------ MENU DE RESERVAS ------------\n";
        cout << "\n\t1. Insertar reserva";
        cout << "\n\t2. Eliminar reserva";
        cout << "\n\t3. Buscar disponibilidad";
        cout << "\n\t4. Mostrar Reservas";
        cout << "\n\t5. Salir";
        cout << "\n\n\tOpcion: ";
        opcion = LeerNumero();

        switch (opcion)
        {
        case 1:
            LeerHoraMinuto(horaInicio, minutoInicio);
            inicio = HoraAMinutos(horaInicio, minutoInicio);

            LeerHoraMinuto(horaFin, minutoFin);
            fin = HoraAMinutos(horaFin, minutoFin);

            prioridad = rand() % 100; // Genera una prioridad aleatoria
            raiz = Insertar(raiz, inicio, fin, prioridad);
            break;
        case 2:
            LeerHoraMinuto(horaInicio, minutoInicio);
            inicio = HoraAMinutos(horaInicio, minutoInicio);

            LeerHoraMinuto(horaFin, minutoFin);
            fin = HoraAMinutos(horaFin, minutoFin);

            raiz = Eliminar(raiz, inicio, fin);
            break;
        case 3:
            LeerHoraMinuto(horaInicio, minutoInicio);
            inicio = HoraAMinutos(horaInicio, minutoInicio);

            LeerHoraMinuto(horaFin, minutoFin);
            fin = HoraAMinutos(horaFin, minutoFin);

            reserva = Buscar(raiz, inicio, fin);
            if (reserva)
            {
                cout << "No hay disponibilidad en el horario buscado.\n";
            }
            else
            {
                cout << "Si hay disponibilidad en el horario buscado.\n";
            }
            break;
        case 4:
            cout << "\n------------ RESERVAS ------------\n";
            MostrarReservas(raiz);
            break;
        case 5:
            cout << "Saliendo...\n";
            break;
        default:
            cout << "Opcion no valida.\n";
            break;
        }
    } while (opcion != 5);
}

int main()
{
    srand(time(NULL)); // Inicializa el generador de numeros aleatorios
    Menu();
    return 0;
}
