#include <iostream>
#include <conio.h>    // Para _kbhit() y _getch() (entrada de teclado)
#include <windows.h>  // Para Sleep() y funciones de consola
#include <ctime>      // Para time() y srand()

using namespace std;

// CONSTANTES DEL JUEGO
const int W = 10;      // Ancho del tablero (10 bloques)
const int H = 20;      // Alto del tablero (20 bloques)
const int GAMEOVER_LINE = H - 5; // Línea de game over (a 5 bloques del fondo)

// Matriz que representa el tablero (0 = vacío, >0 = bloque fijo)
int board[H][W] = {0};

// CARACTERES VISUALES
const char BLOCK_MOVING = 219;    // Carácter ASCII para bloque en movimiento (█)
const char BLOCK_FIXED = 176;     // Carácter ASCII para bloque fijo (░)
const char EMPTY_SPACE = ' ';     // Espacio vacío
const char GAMEOVER_MARKER = '.'; // Marcador visual para línea de game over

/**
 * FORMAS DE LAS PIEZAS:
 * - 4 piezas diferentes (I, O, T, L)
 * - Cada pieza tiene 4 rotaciones posibles
 * - Cada rotación es una matriz 4x4 donde 1 = bloque y 0 = espacio vacío
 */
int shapes[4][4][4][4] = {
    // Pieza I (forma de palo)
    {
        {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}}, // Horizontal
        {{0,0,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,1,0}}, // Vertical
        {{0,0,0,0}, {0,0,0,0}, {1,1,1,1}, {0,0,0,0}}, // Horizontal (alternativa)
        {{0,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,0,0}}  // Vertical (alternativa)
    },
    // Pieza O (cuadrado)
    {
        {{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}}, // Todas las rotaciones iguales
        {{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}},
        {{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}},
        {{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}}
    },
    // Pieza T
    {
        {{0,0,0,0}, {0,1,0,0}, {1,1,1,0}, {0,0,0,0}}, // T normal
        {{0,0,0,0}, {0,1,0,0}, {0,1,1,0}, {0,1,0,0}}, // Rotada 90°
        {{0,0,0,0}, {0,0,0,0}, {1,1,1,0}, {0,1,0,0}}, // Rotada 180°
        {{0,0,0,0}, {0,1,0,0}, {1,1,0,0}, {0,1,0,0}}  // Rotada 270°
    },
    // Pieza L
    {
        {{0,0,0,0}, {1,0,0,0}, {1,1,1,0}, {0,0,0,0}}, // L normal
        {{0,0,0,0}, {0,1,1,0}, {0,1,0,0}, {0,1,0,0}}, // Rotada 90°
        {{0,0,0,0}, {0,0,0,0}, {1,1,1,0}, {0,0,1,0}}, // Rotada 180°
        {{0,0,0,0}, {0,1,0,0}, {0,1,0,0}, {1,1,0,0}}  // Rotada 270°
    }
};

/**
 * ESTRUCTURA DE PIEZA:
 * - x, y: Posición en el tablero
 * - t: Tipo de pieza (0-3)
 * - r: Rotación actual (0-3)
 * - Constructor para inicializar fácilmente
 */
struct Piece { 
    int x, y, t, r; 
    Piece(int _x, int _y, int _t, int _r) : x(_x), y(_y), t(_t), r(_r) {}
};

/**
 * FUNCIÓN: checkCollision
 * OBJETIVO: Verificar si la pieza colisiona con bordes u otros bloques
 * PARÁMETROS:
 * - piece: La pieza a verificar
 * RETORNO:
 * - true si hay colisión, false si no
 * FUNCIONAMIENTO:
 * 1. Recorre la matriz 4x4 de la pieza
 * 2. Para cada bloque de la pieza:
 *    a. Calcula su posición en el tablero
 *    b. Verifica si está fuera de los límites
 *    c. Verifica si choca con un bloque ya existente
 */
bool checkCollision(const Piece& piece) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (shapes[piece.t][piece.r][y][x]) {
                int boardX = piece.x + x;
                int boardY = piece.y + y;
                
                // Verificar límites del tablero
                if (boardX < 0 || boardX >= W || boardY >= H) 
                    return true;
                    
                // Verificar colisión con bloques existentes (solo si está dentro del tablero)
                if (boardY >= 0 && board[boardY][boardX])
                    return true;
            }
        }
    }
    return false;
}

/**
 * FUNCIÓN: checkGameOver
 * OBJETIVO: Verificar si el juego debe terminar
 * RETORNO:
 * - true si hay bloques en la línea de game over
 * FUNCIONAMIENTO:
 * 1. Recorre la línea de game over (GAMEOVER_LINE)
 * 2. Si encuentra cualquier bloque fijo, retorna true
 */
bool checkGameOver() {
    for (int x = 0; x < W; x++) {
        if (board[GAMEOVER_LINE][x] != 0) {
            return true;
        }
    }
    return false;
}

/**
 * FUNCIÓN: draw
 * OBJETIVO: Dibujar el tablero y la pieza actual
 * PARÁMETROS:
 * - p: La pieza actual en movimiento
 * FUNCIONAMIENTO:
 * 1. Limpia la pantalla
 * 2. Dibuja el borde superior
 * 3. Para cada fila del tablero:
 *    a. Dibuja el borde izquierdo
 *    b. Para cada columna:
 *       - Verifica si hay parte de la pieza actual
 *       - Dibuja bloque móvil, fijo o espacio vacío
 *       - Marca la línea de game over con puntos
 *    c. Dibuja el borde derecho
 * 4. Dibuja el borde inferior
 */
void draw(const Piece& p) {
    system("cls"); // Limpiar pantalla
    
    // Dibujar borde superior
    cout << "+";
    for (int x = 0; x < W; x++) cout << "-";
    cout << "+\n";
    
    // Dibujar cada fila del tablero
    for (int y = 0; y < H; y++) {
        cout << "|"; // Borde izquierdo
        
        for (int x = 0; x < W; x++) {
            bool isPiece = false;
            
            // Verificar si esta posición es parte de la pieza actual
            for (int py = 0; py < 4 && !isPiece; py++) {
                for (int px = 0; px < 4; px++) {
                    if (p.y + py == y && p.x + px == x && shapes[p.t][p.r][py][px]) {
                        isPiece = true;
                        break;
                    }
                }
            }
            
            // Dibujar el contenido de la celda
            if (board[y][x]) {
                // Bloque fijo (o marcador en línea de game over)
                cout << (y == GAMEOVER_LINE ? GAMEOVER_MARKER : BLOCK_FIXED);
            } else {
                // Espacio vacío o bloque móvil (con marcador en línea de game over)
                cout << (y == GAMEOVER_LINE ? GAMEOVER_MARKER : 
                       (isPiece ? BLOCK_MOVING : EMPTY_SPACE));
            }
        }
        cout << "|\n"; // Borde derecho
    }
    
    // Dibujar borde inferior
    cout << "+";
    for (int x = 0; x < W; x++) cout << "-";
    cout << "+\n";
}

/**
 * FUNCIÓN: clearLines
 * OBJETIVO: Eliminar líneas completas y hacer caer los bloques superiores
 * PARÁMETROS:
 * - p: Pieza actual (para redibujar)
 * FUNCIONAMIENTO:
 * 1. Recorre el tablero de abajo hacia arriba
 * 2. Para cada línea:
 *    a. Verifica si está completa (todos los bloques llenos)
 *    b. Si está completa:
 *       - Efecto visual de parpadeo
 *       - Mueve todas las líneas superiores hacia abajo
 *       - Limpia la línea superior
 *       - Ajusta el índice para revisar la nueva línea en esta posición
 */
void clearLines(Piece& p) {
    for (int y = H-1; y >= 0; y--) {
        bool lineComplete = true;
        
        // Verificar si la línea está completa
        for (int x = 0; x < W; x++) {
            if (board[y][x] == 0) {
                lineComplete = false;
                break;
            }
        }
        
        // Si la línea está completa
        if (lineComplete) {
            // Efecto visual: parpadeo (3 veces)
            for (int flash = 0; flash < 3; flash++) {
                // Cambiar a valor especial (8) para efecto
                for (int x = 0; x < W; x++) {
                    board[y][x] = 8;
                }
                draw(p);
                Sleep(100);
                
                // Volver al valor normal (1)
                for (int x = 0; x < W; x++) {
                    board[y][x] = 1;
                }
                draw(p);
                Sleep(100);
            }
            
            // Mover todas las líneas superiores hacia abajo
            for (int yy = y; yy > 0; yy--) {
                for (int x = 0; x < W; x++) {
                    board[yy][x] = board[yy-1][x];
                }
            }
            
            // Limpiar la línea superior
            for (int x = 0; x < W; x++) {
                board[0][x] = 0;
            }
            
            // Revisar la misma posición otra vez (ahora con nueva línea)
            y++;
        }
    }
}

/**
 * FUNCIÓN MAIN - Bucle principal del juego
 * FUNCIONAMIENTO:
 * 1. Inicializa semilla aleatoria y pieza inicial
 * 2. Bucle principal:
 *    a. Maneja entrada del teclado
 *    b. Mueve la pieza hacia abajo automáticamente
 *    c. Cuando la pieza se fija:
 *       - Verifica y limpia líneas completas
 *       - Verifica game over
 *       - Crea nueva pieza
 *    d. Dibuja el estado actual
 */
int main() {
    // Configurar consola para caracteres especiales
    SetConsoleOutputCP(65001);
    
    // Inicializar generador aleatorio
    srand(static_cast<unsigned>(time(nullptr)));
    
    // Crear primera pieza (centrada cerca del tope)
    Piece p(W/2-2, -2, rand()%4, 0);
    int speed = 20;    // Velocidad de caída (ciclos por movimiento)
    int counter = 0;   // Contador para movimiento automático

    // Bucle principal del juego
    while (true) {
        // Manejar entrada del teclado
        if (_kbhit()) {
            Piece tmp = p; // Copia temporal para probar movimiento
            
            switch (tolower(_getch())) {
                case 'a': tmp.x--; break; // Mover izquierda
                case 'd': tmp.x++; break; // Mover derecha
                case 's': tmp.y++; break; // Mover abajo
                case 'w': tmp.r = (tmp.r + 1) % 4; break; // Rotar
                case 'q': return 0; // Salir
            }
            
            // Aplicar movimiento si no hay colisión
            if (!checkCollision(tmp)) p = tmp;
        }

        // Movimiento automático hacia abajo
        if (++counter > speed) {
            p.y++; // Mover pieza hacia abajo
            
            // Verificar colisión después de mover
            if (checkCollision(p)) {
                p.y--; // Revertir movimiento si hay colisión
                
                // Fijar la pieza al tablero
                for (int y = 0; y < 4; y++) {
                    for (int x = 0; x < 4; x++) {
                        if (shapes[p.t][p.r][y][x] && p.y + y >= 0) {
                            board[p.y + y][p.x + x] = p.t + 1;
                        }
                    }
                }
                
                // Verificar y eliminar líneas completas
                clearLines(p);
                
                // Verificar condición de game over
                if (checkGameOver()) {
                    draw(p);
                    cout << "\nGAME OVER! Bloques alcanzaron la linea limite!\n";
                    system("pause");
                    return 0;
                }
                
                // Crear nueva pieza
                p = Piece(W/2-2, -2, rand()%4, 0);
                
                // Verificar si la nueva pieza colisiona (game over)
                if (checkCollision(p)) {
                    draw(p);
                    cout << "\nGAME OVER! No hay espacio para nueva pieza!\n";
                    system("pause");
                    return 0;
                }
            }
            counter = 0; // Reiniciar contador
        }

        // Dibujar estado actual del juego
        draw(p);
        Sleep(50); // Pequeña pausa para controlar velocidad
    }

    return 0;
}