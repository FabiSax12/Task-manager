#include "utils.h"
#include <iostream> // Para std::cerr
#include <string>   // Para std::string
#include <limits>   // Para std::numeric_limits

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
COORD getCursorPosition(HANDLE hConsole) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return csbi.dwCursorPosition;
    }
    // Retorna (0,0) si falla
    return COORD{0, 0};
}

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
void moveCursor(int x, int y,HANDLE hConsole) {
    // Mover el cursor a (x, y)
    COORD pos;
    pos.X = x;
    pos.Y = y;
    if (!SetConsoleCursorPosition(hConsole, pos)) {
        std::cerr << "Error al mover el cursor a la posición (" << x << ", " << y << ")." << std::endl;
        return;
    }
    // Mover el cursor
    if (!SetConsoleCursorPosition(hConsole, pos)) {
        std::cerr << "Error al mover el cursor de vuelta a la posición (" << x << ", " << y << ")." << std::endl;
        return;
    }
}

void deleteLine(HANDLE hConsole){
    // Obtener el ancho de la consola para saber cuántos espacios usar
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            std::cerr << "Error al obtener la información del buffer de la consola." << std::endl;
            return;
        }
        int anchoConsola = csbi.dwSize.X;

        // Crear una cadena de espacios para sobrescribir la línea
        std::string espacios(anchoConsola, ' ');

        // Imprimir los espacios para "borrar" la línea
        std::cout << espacios;
}
/**
 * @brief Mueve el cursor a una posición específica y elimina la línea en esa posición.
 *
 * Esta función mueve el cursor de la consola a las coordenadas especificadas (x, y) y luego
 * elimina la línea en esa posición utilizando el manejador de consola proporcionado.
 *
 * @param x Coordenada horizontal (columna) a la que se moverá el cursor.
 * @param y Coordenada vertical (fila) a la que se moverá el cursor.
 * @param hConsole Manejador de la consola en la que se realizará la operación.
 * @author Joseph
 */
void moveCursorAndDeleteLine(int x, int y, HANDLE hConsole){
    moveCursor(x, y, hConsole);
    deleteLine(hConsole);
}
/**
 * @brief Espera a que el usuario presione Enter para continuar.
 *
 * Esta función limpia el buffer de entrada previo y luego espera a que el usuario presione la tecla Enter.
 * Después de que se presiona Enter, limpia cualquier entrada residual en el buffer de entrada.
 * @author Joseph
 */

void waitEnter() {
    // Limpiar el buffer de entrada previo
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Esperar a que el usuario presione Enter
    std::string dummy;
    std::getline(std::cin, dummy);

    // Limpiar cualquier entrada residual (opcional)
    std::cin.clear();
}   

/**
 * @brief Limpia el buffer de entrada estándar (std::cin).
 *
 * Esta función restablece el estado de `std::cin` y descarta cualquier entrada residual en el buffer,
 * hasta encontrar un carácter de nueva línea (`'\n'`) o alcanzar el número máximo de caracteres.
 * @author Joseph
 */
void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/**
 * @brief Verifica si la entrada de usuario es válida.
 *
 * Esta función verifica si la entrada de usuario es válida, es decir, si no hay errores en la entrada.
 * Si se detecta un error, se restablece el estado de `std::cin` y se limpia el buffer de entrada.
 * @author Joseph
 */
void verifyInputType(){
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

/**
 * @brief Valida una fecha proporcionada en forma de cadena y asigna los valores de día, mes y año si es válida.
 *
 * Esta función toma una cadena de fecha en formato "dd-mm-yyyy", la analiza y valida los valores de día, mes y año.
 * Verifica que el formato sea correcto, que el mes esté entre 1 y 12, y que el día sea válido para el mes y año dados
 * (incluyendo la consideración de años bisiestos para febrero). Si alguna validación falla, muestra un mensaje de error
 * y termina la función.
 *
 * @param[out] day    Referencia al entero donde se almacenará el día validado.
 * @param[out] month  Referencia al entero donde se almacenará el mes validado.
 * @param[out] year   Referencia al entero donde se almacenará el año validado.
 * @param[in]  dateStr Cadena que contiene la fecha a validar en formato "dd-mm-yyyy".

 * @author Joseph
 */
bool validateDates(int &day,int &month,int &year,string dateStr){
    if (sscanf(dateStr.c_str(), "%d-%d-%d", &day, &month, &year) != 3) {
        std::cout << "Formato de fecha invalido." << endl;
        return false;
    }
    if (month < 1 || month > 12) {      /*Validar que el mes esté entre 1 y 12*/
        std::cout << "Mes invalido. Debe ser un numero entre 1 y 12." << endl;
        return false;
    }
    int maxDaysInMonth;                 /*Determinar el número máximo de días en el mes dado*/
    switch (month) {
        case 4: case 6: case 9: case 11:
            maxDaysInMonth = 30;
            break;
        case 2:
            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)){maxDaysInMonth = 29;}/*Determinar el número máximo de días en el mes dado*/
            else{maxDaysInMonth = 28;}   
            break;
        default:
            maxDaysInMonth = 31;
            break;
    }
    if (day < 1 || day > maxDaysInMonth) {                              /*Validar que el día esté dentro del rango válido para el mes*/
        std::cout << "Dia invalido para el mes especificado." << endl;
        return false;
    }
    return true;
}
