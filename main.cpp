#include<iostream>
#include<string>
#include<Windows.h>
#include<list>
#include<array>
using namespace std;

void gotoXY(int x, int y) {
    COORD coord;
    coord.X = 3 * x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void cargarMapa(string mapa[20][20]) {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (i == 0 || i == 19) {
                mapa[i][j] = "==="; // Paredes
            } else if (j == 0) {
                mapa[i][j] = "|| "; // Paredes
            } else if (j == 19) {
                mapa[i][j] = " ||"; // Paredes
            } else {
                mapa[i][j] = "   "; // Espacio vacío
            }
        }
    }
}

void cargarMapaLogico(string mapaLogico[20][20]) {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (i == 0 || i == 19) {
                mapaLogico[i][j] = "2"; // Paredes
            } else if (j == 0 || j == 19) {
                mapaLogico[i][j] = "2"; // Paredes
            } else {
                mapaLogico[i][j] = "0"; // Espacio vacío
            }
        }
    }
}

void dibujarMapa(string mapa[20][20]) {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            cout << mapa[i][j];
        }
        cout << endl;
    }
}

void borrarSerpiente(list<array<int, 2>>& serpiente) {
    for (const auto& segmento : serpiente) {
        gotoXY(segmento[0], segmento[1]);
        cout << "   "; // Borrar el segmento de la serpiente
    }
}

int moverSerpiente(list<array<int, 2>>& serpiente, char direccion, string mapaLogico[20][20]) {
    array<int, 2> nuevaPosicion = *serpiente.begin();
    array<int, 2> viejaPosicion = *serpiente.begin();

    switch (direccion) {
        case 'w': // Arriba
            if (nuevaPosicion[1] > 1) nuevaPosicion[1]--;
            else return 0; // Terminar el programa si se intenta mover fuera del mapa
            if (mapaLogico[nuevaPosicion[0]][nuevaPosicion[1]] == "2") {
                // Si la serpiente choca con una pared, terminar el juego
                gotoXY(0, 21);
                cout << "Game Over! Chocaste con una pared." << endl;
                return 0; // Terminar el programa
            }
            if (mapaLogico[nuevaPosicion[0]][nuevaPosicion[1]] == "1") {
                // Si la serpiente come una manzana, no eliminar el último segmento
                serpiente.push_back(viejaPosicion);
                mapaLogico[nuevaPosicion[1]][nuevaPosicion[0]] = "0"; // Eliminar la manzana
                gotoXY(0,21);
                cout << "Comiste una manzana!" << endl;
            }
            for (auto it = serpiente.begin(); it != serpiente.end(); ++it) {
                if (it != serpiente.begin() && *it == nuevaPosicion) {
                    // Si la serpiente choca consigo misma, terminar el juego
                    gotoXY(0, 21);
                    cout << "Game Over! Chocaste contigo mismo." << endl;
                    return 0; // Terminar el programa
                }
            }
            break;
        case 's': // Abajo
            if (nuevaPosicion[1] < 18) nuevaPosicion[1]++;
            if (mapaLogico[nuevaPosicion[0]][nuevaPosicion[1]] == "1") {
                // Si la serpiente come una manzana, no eliminar el último segmento
                serpiente.push_front(nuevaPosicion);
                mapaLogico[nuevaPosicion[1] - 1][nuevaPosicion[0]] = "0"; // Eliminar la manzana
            }
            break;
        case 'a': // Izquierda
            if (nuevaPosicion[0] > 1) nuevaPosicion[0]--;
            if (mapaLogico[nuevaPosicion[0]][nuevaPosicion[1]] == "1") {
                // Si la serpiente come una manzana, no eliminar el último segmento
                serpiente.push_front(nuevaPosicion);
                mapaLogico[nuevaPosicion[1] - 1][nuevaPosicion[0]] = "0"; // Eliminar la manzana
            }
            break;
        case 'd': // Derecha
            if (nuevaPosicion[0] < 18) nuevaPosicion[0]++;
            if (mapaLogico[nuevaPosicion[0]][nuevaPosicion[1]] == "1") {
                // Si la serpiente come una manzana, no eliminar el último segmento
                serpiente.push_front(nuevaPosicion);
                mapaLogico[nuevaPosicion[1] - 1][nuevaPosicion[0]] = "0"; // Eliminar la manzana
            }
            break;
    }

    serpiente.push_front(nuevaPosicion); // Añadir la nueva posición al frente
    serpiente.pop_back(); // Eliminar la última posición de la serpiente

    return 1; // Continuar el juego
}

void dibujarSerpiente(const list<array<int, 2>>& serpiente) {
    for (const auto& segmento : serpiente) {
        gotoXY(segmento[0], segmento[1]);
        cout << "^"; // Representación de la serpiente
    }
}

int main() {
    string mapaLogico[20][20], mapa[20][20];
    list<array<int, 2>> serpiente;
    char input; // Inicialmente hacia arriba
    int a = 1;

    serpiente.push_back({10, 10}); // Posición inicial de la serpiente

    mapaLogico[10][9] = "1"; // Manzana en la posición (10, 9)
    mapaLogico[10][10] = "2"; // Espacio ocupado por la serpiente

    cargarMapa(mapa);
    dibujarMapa(mapa);

    gotoXY(10, 9);
    cout << "A"; // Representación de la manzana

    gotoXY((*serpiente.begin())[0], (*serpiente.begin())[1]); 
    cout << "^"; // Representación de la serpiente

    while (a) {
        if (GetAsyncKeyState(VK_ESCAPE)) {
            break; // Salir del bucle si se presiona ESC
        }
        
        if (GetAsyncKeyState(VK_UP)) {
            input = 'w'; // Arriba
        } else if (GetAsyncKeyState(VK_DOWN)) {
            input = 's'; // Abajo
        } else if (GetAsyncKeyState(VK_LEFT)) {
            input = 'a'; // Izquierda
        } else if (GetAsyncKeyState(VK_RIGHT)) {
            input = 'd'; // Derecha
        }
        
        a = moverSerpiente(serpiente, input, mapaLogico);
        dibujarSerpiente(serpiente);
        Sleep(100); // Espera para evitar un bucle demasiado rápido
        borrarSerpiente(serpiente);
        gotoXY(0, 22); // Volver al inicio de la consola
        cout << a;
    }

    Sleep(10000); // Espera antes de cerrar

    gotoXY(0, 21);
    system("pause");
    return 0;
}