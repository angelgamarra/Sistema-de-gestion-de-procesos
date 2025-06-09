// sistema de gestion de procesos en c++ usando estructuras dinamicas lineales
// se utiliza lista enlazada, cola de prioridad y pila
// incluye persistencia de datos para los procesos

#include <iostream> // para entrada y salida por consola
#include <fstream> // para manejo de archivos (persistencia)
#include <string> // para evitar usar std:: en cada instruccion

using namespace std; // para evitar usar std:: en cada instruccion

// ============================
// Lista enlazada - Vera Chavez Fernando Vera
// ============================

// estructura que representa un proceso
struct Proceso {
    int id;             // identificador unico del proceso
    string nombre;      // nombre del proceso
    int prioridad;      // nivel de prioridad del proceso (mayor valor = mayor prioridad)
    Proceso* sig;       // puntero al siguiente proceso en la lista
};

// clase para gestionar la lista enlazada de procesos
class ListaProcesos {
private:
    Proceso* cabeza; // puntero al inicio de la lista enlazada

public:
    // constructor: inicializa la lista como vacia
    ListaProcesos() : cabeza(nullptr) {}

    // destructor: limpia la memoria al destruir el objeto
    ~ListaProcesos() { limpiar(); }

    // inserta un nuevo proceso al inicio de la lista
    void insertar(int id, const string& nombre, int prioridad) {
        Proceso* nuevo = new Proceso{id, nombre, prioridad, cabeza}; // crea nuevo nodo
        cabeza = nuevo; // actualiza la cabeza de la lista
    }

    // elimina un proceso de la lista segun su id
    void eliminar(int id) {
        Proceso* actual = cabeza; // nodo actual para recorrer
        Proceso* anterior = nullptr; // nodo anterior para actualizar enlaces

        // busca el nodo a eliminar
        while (actual && actual->id != id) {
            anterior = actual;
            actual = actual->sig;
        }

        // si no se encuentra, se sale
        if (!actual) return;

        // si es el primero, se actualiza la cabeza
        if (!anterior)
            cabeza = actual->sig;
        else
            anterior->sig = actual->sig; // enlaza el anterior con el siguiente

        delete actual; // libera la memoria
    }

    // busca un proceso por su id y lo retorna
    Proceso* buscar(int id) {
        Proceso* actual = cabeza; // recorre desde el inicio
        while (actual) {
            if (actual->id == id) return actual; // si coincide, se retorna
            actual = actual->sig;
        }
        return nullptr; // si no se encuentra
    }

    // modifica la prioridad de un proceso dado su id
    void modificar_prioridad(int id, int nueva_prioridad) {
        Proceso* p = buscar(id); // busca el proceso
        if (p) p->prioridad = nueva_prioridad; // si existe, modifica
    }

    // muestra todos los procesos registrados
    void mostrar() {
        Proceso* actual = cabeza;
        while (actual) {
            cout << "id: " << actual->id << ", nombre: " << actual->nombre
                 << ", prioridad: " << actual->prioridad << endl;
            actual = actual->sig;
        }
    }

    // guarda la lista de procesos en un archivo de texto
    void guardar_en_archivo(const string& nombreArchivo) {
        ofstream archivo(nombreArchivo); // abre archivo en modo escritura
        Proceso* actual = cabeza;
        while (actual) {
            archivo << actual->id << ";" << actual->nombre << ";" << actual->prioridad << endl; // escribe datos separados por ;
            actual = actual->sig;
        }
        archivo.close(); // cierra el archivo
    }

    // carga los procesos desde un archivo de texto
    void cargar_desde_archivo(const string& nombreArchivo) {
        ifstream archivo(nombreArchivo); // abre archivo en modo lectura
        if (!archivo.is_open()) return; // si no se puede abrir, salir

        limpiar(); // limpia la lista actual
        int id, prioridad;
        string nombre, linea;

        // lee linea por linea
        while (getline(archivo, linea)) {
            size_t pos1 = linea.find(";"); // encuentra primera separacion
            size_t pos2 = linea.find(";", pos1 + 1); // encuentra segunda
            if (pos1 != string::npos && pos2 != string::npos) {
                id = stoi(linea.substr(0, pos1)); // convierte id
                nombre = linea.substr(pos1 + 1, pos2 - pos1 - 1); // extrae nombre
                prioridad = stoi(linea.substr(pos2 + 1)); // convierte prioridad
                insertar(id, nombre, prioridad); // inserta proceso
            }
        }
        archivo.close(); // cierra archivo
    }

    // libera toda la memoria de la lista
    void limpiar() {
        while (cabeza) {
            Proceso* temp = cabeza;
            cabeza = cabeza->sig;
            delete temp;
        }
    }
};

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



// ============================
// Pila de memoria - Gamarra Romero Angel Rodrigo
// ============================

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


// ============================
// Menú de usuario e interacción - Sotelo Gallegos Raúl -Loaiza Quispe Melany Fiorela
// ============================

void menu(ListaProcesos& lista) {
    ColaPrioridad cola; // instancia de la cola de prioridad
    PilaMemoria pila;   // instancia de la pila de memoria

    int opcion; // opcion elegida por el usuario
    do {
        // muestra el menu
        cout << "\n--- menu del sistema de gestion de procesos ---\n";
        cout << "1. agregar proceso\n2. eliminar proceso\n3. buscar proceso\n4. modificar prioridad\n5. ver lista de procesos\n";
        cout << "6. encolar proceso\n7. ejecutar proceso\n8. ver cola de cpu\n";
        cout << "9. asignar bloque de memoria\n10. liberar bloque\n11. ver estado de memoria\n";
        cout << "12. guardar datos\n13. cargar datos\n0. salir\nopcion: ";
        cin >> opcion; // lee la opcion

        int id, prioridad;
        string nombre;

                // realiza la accion segun la opcion
        switch (opcion) {
            case 1:
                cout << "id: "; cin >> id;
                cout << "nombre: "; cin >> nombre;
                cout << "prioridad: "; cin >> prioridad;
                lista.insertar(id, nombre, prioridad);
                break;
            case 2:
                cout << "id a eliminar: "; cin >> id;
                lista.eliminar(id);
                break;
            case 3:
                cout << "id a buscar: "; cin >> id;
                if (Proceso* p = lista.buscar(id))
                    cout << "encontrado: " << p->nombre << ", prioridad: " << p->prioridad << endl;
                else
                    cout << "no encontrado\n";
                break;
            case 4:
                cout << "id a modificar: "; cin >> id;
                cout << "nueva prioridad: "; cin >> prioridad;
                lista.modificar_prioridad(id, prioridad);
                break;
            case 5:
                lista.mostrar();
                break;
            case 6:
                cout << "id a encolar: "; cin >> id;
                if (Proceso* p = lista.buscar(id))
                    cola.encolar(p);
                else
                    cout << "proceso no encontrado\n";
                break;
            case 7:
                if (Proceso* p = cola.desencolar()) {
                    cout << "Ejecutando proceso id: " << p->id << ", nombre: " << p->nombre << endl;
                } else {
                    cout << "cola vacia\n";
                }
                break;
            case 8:
                cola.mostrar();
                break;
            case 9:
                pila.push();
                cout << "bloque asignado\n";
                break;
            case 10:
                pila.pop();
                cout << "bloque liberado\n";
                break;
            case 11:
                pila.mostrar();
                break;
            case 12:
                lista.guardar_en_archivo("procesos.txt");
                cout << "datos guardados\n";
                break;
            case 13:
                lista.cargar_desde_archivo("procesos.txt");
                cout << "datos cargados\n";
                break;
            case 0:
                cout << "saliendo...\n";
                break;
            default:
                cout << "opcion invalida\n";
        }
    } while (opcion != 0);
}

int main() {
    ListaProcesos lista; // instancia de la lista de procesos

    lista.cargar_desde_archivo("procesos.txt"); // carga procesos desde archivo al iniciar

    menu(lista); // muestra el menu y espera acciones del usuario

    lista.guardar_en_archivo("procesos.txt"); // guarda procesos al cerrar

    return 0;
}

