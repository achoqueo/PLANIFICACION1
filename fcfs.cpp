#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>

using namespace std;

// Estructura requerida por la guía
struct Proceso {
    string id;
    int llegada;
    int servicio;
    int comienzo;
    int finalizacion;
    int espera;
    int estancia;
    double estanciaNormalizada;
    int ordenOriginal; // Para desempatar de acuerdo al orden de ingreso
};

int main() {
    int numProcesos;

    // 1. Solicitar el número de procesos (entre 3 y 5)
    do {
        cout << "Ingrese el numero de procesos (3 a 5): ";
        cin >> numProcesos;
    } while (numProcesos < 3 || numProcesos > 5);

    vector<Proceso> procesos(numProcesos);

    // 2. Permitir el ingreso de datos
    cout << "\n--- INGRESO DE PROCESOS ---\n";
    for (int i = 0; i < numProcesos; ++i) {
        procesos[i].id = "P" + to_string(i + 1);
        cout << "Proceso " << i + 1 << ":\n";
        cout << "  Identificador (ej. P" << i + 1 << " o A, B...): ";
        cin >> procesos[i].id;
        
        do {
            cout << "  Tiempo de llegada (>= 0): ";
            cin >> procesos[i].llegada;
        } while (procesos[i].llegada < 0);

        do {
            cout << "  Tiempo de servicio (> 0): ";
            cin >> procesos[i].servicio;
        } while (procesos[i].servicio <= 0);

        procesos[i].ordenOriginal = i;
    }

    // 4. Ordenar los procesos por tiempo de llegada.
    // En caso de empate, se respeta el orden de ingreso.
    sort(procesos.begin(), procesos.end(), [](const Proceso &a, const Proceso &b) {
        if (a.llegada != b.llegada) {
            return a.llegada < b.llegada;
        }
        return a.ordenOriginal < b.ordenOriginal;
    });

    // 5 y 8. Calcular tiempos e identificar intervalos ociosos
    int tiempoActual = 0;
    double sumaEspera = 0;
    double sumaEstancia = 0;

    cout << "\n--- CRONOGRAMA DE EJECUCION ---\n";
    for (int i = 0; i < numProcesos; ++i) {
        // Considerar los intervalos en que el procesador permanece ocioso
        if (tiempoActual < procesos[i].llegada) {
            cout << "[Ocioso] El procesador permanece ocioso desde t = " 
                 << tiempoActual << " hasta t = " << procesos[i].llegada << "\n";
            tiempoActual = procesos[i].llegada;
        }

        procesos[i].comienzo = tiempoActual;
        procesos[i].finalizacion = procesos[i].comienzo + procesos[i].servicio;
        procesos[i].estancia = procesos[i].finalizacion - procesos[i].llegada;
        procesos[i].espera = procesos[i].comienzo - procesos[i].llegada;
        procesos[i].estanciaNormalizada = (double)procesos[i].estancia / procesos[i].servicio;

        tiempoActual = procesos[i].finalizacion;

        sumaEspera += procesos[i].espera;
        sumaEstancia += procesos[i].estancia;
    }

    // 7. Mostrar la tabla con los resultados requeridos
    cout << "\n" << string(65, '-') << "\n";
    cout << left << setw(10) << "Proceso"
         << setw(6) << "TL"
         << setw(6) << "Ts"
         << setw(6) << "TC"
         << setw(6) << "TF"
         << setw(6) << "TE"
         << setw(6) << "Tr"
         << setw(10) << "Tr/Ts" << "\n";
    cout << string(65, '-') << "\n";

    for (const auto &p : procesos) {
        cout << left << setw(10) << p.id
             << setw(6) << p.llegada
             << setw(6) << p.servicio
             << setw(6) << p.comienzo
             << setw(6) << p.finalizacion
             << setw(6) << p.espera
             << setw(6) << p.estancia
             << fixed << setprecision(2) << setw(10) << p.estanciaNormalizada << "\n";
    }
    cout << string(65, '-') << "\n";

    // Mostrar promedios de espera y estancia
    cout << fixed << setprecision(2);
    cout << "Tiempo de espera promedio (TE promedio): " << (sumaEspera / numProcesos) << "\n";
    cout << "Tiempo de estancia promedio (Tr promedio): " << (sumaEstancia / numProcesos) << "\n";

    return 0;
}
