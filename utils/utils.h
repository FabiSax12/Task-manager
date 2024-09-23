#ifndef UTILS_H
#define UTILS_H

#include <windows.h>

/**
 * @brief Obtiene la posición actual del cursor en la consola.
 *
 * Esta función utiliza las funciones de la API de Windows para recuperar la posición
 * actual del cursor en la consola. Si no puede obtener la posición, retorna (0, 0).
 *
 * @param hConsole El handle de la consola obtenido con GetStdHandle.
 * @return COORD La posición actual del cursor en la consola.
 *
 * @author Joseph
 */
COORD getCursorPosition(HANDLE hConsole);

/**
 * @brief Mueve el cursor a una posición específica en la consola y borra la línea.
 *
 * Esta función mueve el cursor a las coordenadas proporcionadas (x, y), obtiene el ancho
 * de la consola, crea una cadena de espacios para sobrescribir la línea en esa posición,
 * imprime los espacios para borrar la línea y vuelve a mover el cursor al inicio de la línea borrada.
 *
 * @param x La coordenada X (columna) a la que se moverá el cursor.
 * @param y La coordenada Y (fila) a la que se moverá el cursor.
 * @param hConsole El handle de la consola obtenido con GetStdHandle.
 *
 * @author Joseph
 */
void moveCursor(int x, int y,HANDLE hConsole);

void deleteLine(HANDLE hConsole);

void moveCursorAndDeleteLine(int x, int y, HANDLE hConsole);

void waitEnter();

void clearInputBuffer();

void verifyInputType();

bool validateDates(int &day,int &month,int &year,string dateStr);

#include "utils.cpp"
#endif // UTILS_H