// sistema de gestion de procesos en c++ usando estructuras dinamicas lineales
// se utiliza lista enlazada, cola de prioridad y pila
// incluye persistencia de datos para los procesos


#include <iostream>
#include <fstream>
#include <string>

using namespace std; // para evitar usar std:: en cada instruccion
























// estructura para nodo de pila de memoria
struct NodoMemoria {
    int bloque_id; // identificador del bloque
    NodoMemoria* sig; // siguiente nodo (bloque siguiente)
};

// clase pila para manejar los bloques de memoria asignados
class PilaMemoria {
private:
    NodoMemoria* cima; // tope de la pila
    int contador = 0; // contador para asignar id a cada bloque

public:
    // constructor: pila vacia
    PilaMemoria() : cima(nullptr) {}

    // asigna un nuevo bloque de memoria (push)
    void push() {
        NodoMemoria* nuevo = new NodoMemoria{++contador, cima}; // nuevo nodo
        cima = nuevo; // actualiza cima
    }

    // libera el ultimo bloque asignado (pop)
    void pop() {
        if (!cima) return; // si esta vacia
        NodoMemoria* temp = cima;
        cima = cima->sig; // avanza la cima
        delete temp; // libera memoria
    }

    // muestra los bloques actuales en la pila
    void mostrar() {
        NodoMemoria* actual = cima;
        while (actual) {
            cout << "bloque de memoria id: " << actual->bloque_id << endl;
            actual = actual->sig;
        }
    }
};



