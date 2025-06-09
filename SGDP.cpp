// sistema de gestion de procesos en c++ usando estructuras dinamicas lineales
// se utiliza lista enlazada, cola de prioridad y pila
// incluye persistencia de datos para los procesos


#include <iostream>
#include <fstream>
#include <string>

using namespace std; // para evitar usar std:: en cada instruccion


// ============================
// Cola de prioridad - Gerson Vidal Curasi Saire
// ============================

// estructura nodo para la cola de prioridad
struct NodoCola {
    Proceso* proceso; // puntero al proceso
    NodoCola* sig;    // siguiente nodo en la cola
};

// clase para gestionar la cola de prioridad (planificador de cpu)
class ColaPrioridad {
private:
    NodoCola* frente; // inicio de la cola

public:
    // constructor: cola vacia
    ColaPrioridad() : frente(nullptr) {}

    // encola un proceso segun su prioridad (mayor prioridad va primero)
    void encolar(Proceso* p) {
        NodoCola* nuevo = new NodoCola{p, nullptr}; // nuevo nodo con el proceso

        // si esta vacia o tiene mayor prioridad
        if (!frente || p->prioridad > frente->proceso->prioridad) {
            nuevo->sig = frente;
            frente = nuevo;
        } else {
            NodoCola* actual = frente;
            // busca la posicion donde insertar
            while (actual->sig && actual->sig->proceso->prioridad >= p->prioridad)
                actual = actual->sig;

            nuevo->sig = actual->sig;
            actual->sig = nuevo;
        }
    }

    // saca el proceso con mayor prioridad y lo retorna
    Proceso* desencolar() {
        if (!frente) return nullptr; // si esta vacia
        NodoCola* temp = frente;
        frente = frente->sig; // avanza el frente
        Proceso* p = temp->proceso; // obtiene proceso
        delete temp; // libera nodo
        return p;
    }

    // muestra la cola actual con sus prioridades
    void mostrar() {
        NodoCola* actual = frente;
        while (actual) {
            cout << "id: " << actual->proceso->id << " (prioridad: " << actual->proceso->prioridad << ")\n";
            actual = actual->sig;
        }
    }
};

















// Pila de memoria - Gamarra Romero Angel Rodrigo

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



