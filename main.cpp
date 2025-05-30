#include <iostream>
#include <string>
#include <Windows.h>
#include <list>
#include <array>
#include <cstdlib>  
#include <ctime>    
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
                mapa[i][j] = "===";
            } else if (j == 0) {
                mapa[i][j] = "|| ";
            } else if (j == 19) {
                mapa[i][j] = " ||";
            } else {
                mapa[i][j] = "   ";
            }
        }
    }
}

void cargarMapaLogico(string mapaLogico[20][20]) {
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 20; x++) {
            if (y == 0 || y == 19 || x == 0 || x == 19) {
                mapaLogico[y][x] = "2"; // Pared
            } else {
                mapaLogico[y][x] = "0"; // Libre
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


void generarManzana(string mapaLogico[20][20]) {
    int x, y;
    do {
        x = rand() % 18 + 1; // entre 1 y 18
        y = rand() % 18 + 1;
    } while (mapaLogico[y][x] != "0"); // Repetir hasta encontrar una celda libre

    mapaLogico[y][x] = "1";
    gotoXY(x, y);
    cout << "A";
}

void borrarSerpiente(list<array<int, 2>>& serpiente) {
    for (const auto& segmento : serpiente) {
        gotoXY(segmento[0], segmento[1]);
        cout << "   ";
    }
}

int moverSerpiente(list<array<int, 2>>& serpiente, char direccion, string mapaLogico[20][20]) {
    array<int, 2> cabeza = serpiente.front();
    array<int, 2> nuevaPos = cabeza;

    if (direccion == 'w') nuevaPos[1]--;
    else if (direccion == 's') nuevaPos[1]++;
    else if (direccion == 'a') nuevaPos[0]--;
    else if (direccion == 'd') nuevaPos[0]++;

    int x = nuevaPos[0], y = nuevaPos[1];

    if (mapaLogico[y][x] == "2") {
        gotoXY(0, 21);
        cout << "Game Over! Chocaste.";
        return 0;
    }

    for (const auto& segmento : serpiente) {
        if (segmento == nuevaPos) {
            gotoXY(0, 21);
            cout << "Game Over! Chocaste contigo mismo.";
            return 0;
        }
    }

    bool comio = mapaLogico[y][x] == "1";

    serpiente.push_front(nuevaPos);
    mapaLogico[y][x] = "2"; // Marcar como ocupada por la serpiente

    if (!comio) {
        array<int, 2> cola = serpiente.back();
        mapaLogico[cola[1]][cola[0]] = "0"; // Liberar casilla
        serpiente.pop_back();
    } else {
        gotoXY(0, 21);
        cout << "Comiste una manzana!";
        generarManzana(mapaLogico);
    }

    return 1;
}

void dibujarSerpiente(const list<array<int, 2>>& serpiente) {
    for (const auto& segmento : serpiente) {
        gotoXY(segmento[0], segmento[1]);
        cout << "^";
    }
}

int main() {
    string mapa[20][20], mapaLogico[20][20];
    list<array<int, 2>> serpiente;
    char input = 'w'; // Dirección inicial

    cargarMapa(mapa);
    cargarMapaLogico(mapaLogico);

    // Posición inicial
    serpiente.push_back({10, 10});
    mapaLogico[10][10] = "2";

    // Manzana
    srand(time(0)); // Inicializar la semilla aleatoria
    
    dibujarMapa(mapa);
    generarManzana(mapaLogico);
    dibujarSerpiente(serpiente);

    int continuar = 1;

    while (continuar) {
        if (GetAsyncKeyState(VK_ESCAPE)) break;

        if (GetAsyncKeyState(VK_UP)) input = 'w';
        else if (GetAsyncKeyState(VK_DOWN)) input = 's';
        else if (GetAsyncKeyState(VK_LEFT)) input = 'a';
        else if (GetAsyncKeyState(VK_RIGHT)) input = 'd';

        borrarSerpiente(serpiente);
        continuar = moverSerpiente(serpiente, input, mapaLogico);
        dibujarSerpiente(serpiente);

        Sleep(150);
    }

    gotoXY(0, 23);
    system("pause");
    return 0;
}
