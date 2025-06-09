// sistema de gestion de procesos en c++ usando estructuras dinamicas lineales
// se utiliza lista enlazada, cola de prioridad y pila
// incluye persistencia de datos para los procesos


#include <iostream>
#include <fstream>
#include <string>

using namespace std; 

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
