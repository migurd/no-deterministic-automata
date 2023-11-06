#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <stdlib.h>

// made by Angel Qui @migurd at github

// Define the states of the NFA
enum Estado {
    INICIO,				            // Estado inicial
    INICIO_MAS,                     // q1
    ENTERO_POSITIVO,                // q2   F
    DECIMAL_POSITIVO,               // q3
    FLOTANTE_POSITIVO,              // q4   F
    DIVISION,                       // q5
    DIVISION_SIGNO,                 // q6
    FRACCION_INDEFINIDO,            // q7   F
    FRACCION,                       // q8   F
    INICIO_MENOS,                   // q9
    ENTERO_NEGATIVO,                // q10  F
    DECIMAL_NEGATIVO,               // q11  
    FLOTANTE_NEGATIVO,              // q12  F
    ERROR,                          // q13  F
};

// Funci�n para transicionar entre estados
enum Estado transicion(enum Estado estado, char entrada) {
    switch (estado) {
        case INICIO: // q0
            if(isdigit(entrada)) return ENTERO_POSITIVO;
            else if(entrada == '+') return INICIO_MAS;
            else if (entrada == '-') return INICIO_MENOS;
            else if (entrada == '.') return DECIMAL_POSITIVO;
            else return ERROR;
        case INICIO_MAS: // q1
            if(isdigit(entrada)) return ENTERO_POSITIVO;
            else if(entrada == '.') return DECIMAL_POSITIVO;
            else return ERROR;
        case ENTERO_POSITIVO: //q2 F
        	if(isdigit(entrada)) return ENTERO_POSITIVO;
            else if(entrada == '/') return DIVISION;
            else if(entrada == '.') return DECIMAL_POSITIVO;
            else return ERROR;
        case DECIMAL_POSITIVO: // q3
            if(isdigit(entrada)) return FLOTANTE_POSITIVO;
            else return ERROR;
        case FLOTANTE_POSITIVO: // q4 F
            if(isdigit(entrada)) return FLOTANTE_POSITIVO;
            else return ERROR;
        case DIVISION: // q5
            if(entrada == '+' || entrada == '-') return DIVISION_SIGNO;
            else if(entrada == '0') return FRACCION_INDEFINIDO;
            else if(isdigit(entrada)) return FRACCION;
            else return ERROR;
        case DIVISION_SIGNO: // q6
            if(entrada == '0') return FRACCION_INDEFINIDO;
            else if(isdigit(entrada)) return FRACCION;
            else return ERROR;
        case FRACCION_INDEFINIDO: // q7 F
            if(entrada == '0') return FRACCION_INDEFINIDO;
            else if(isdigit(entrada)) return FRACCION;
            else ERROR;
        case FRACCION: // q8 F
            if(isdigit(entrada)) return FRACCION;
            else ERROR;
        case INICIO_MENOS: // q9
            if(isdigit(entrada)) return ENTERO_NEGATIVO;
            else if(entrada == '.') return DECIMAL_NEGATIVO;
            else return ERROR;
        case ENTERO_NEGATIVO: // q10 F
        	if(isdigit(entrada)) return ENTERO_NEGATIVO;
            else if(entrada == '/') return DIVISION;
            else if(entrada == '.') return DECIMAL_NEGATIVO;
            else return ERROR;
        case DECIMAL_NEGATIVO: // q11
            if(isdigit(entrada)) return FLOTANTE_NEGATIVO;
            else return ERROR;
        case FLOTANTE_NEGATIVO: // q12 F
            if(isdigit(entrada)) return FLOTANTE_NEGATIVO;
            else return ERROR;
        case ERROR:
            return ERROR;
        default:
            return ERROR;
    }
}

void gotoxy(int x, int y) {
    printf("\033[%d;%df", y, x);
}

// FUNCÓN QUE TE DEVUELVE UN BOOLEANO DICIENDO SI EL NODO ES FINAL
bool esNodoFinal(enum Estado estado) {
    if(
        estado == ENTERO_POSITIVO || 
        estado == FLOTANTE_POSITIVO || 
        estado == FRACCION_INDEFINIDO || 
        estado == FRACCION || 
        estado == ENTERO_NEGATIVO || 
        estado == FLOTANTE_NEGATIVO
    )
        return true;
    else
        return false;
}

// Funci�n de entrada personalizada
void entradaPersonalizada(char *entrada) {
    // Limpiar la entrada anterior
    for (int j = 0; j < strlen(entrada); j++)
        entrada[j] = ' ';

    int i = 0;
    char c;
    while (1) {
        c = getch();
        if (c == '\r') {  // Comprobar si se presiona la tecla Enter
            printf("\n");
            break;
        } else if (c == '.' || isdigit(c) || c == '+' || c == '-' || c == '/') {
            printf("%c", c);
            entrada[i] = c;
            i++;
        } else if (c == '\b' && i > 0) {  // Manejar la tecla Retroceso
            printf("\b \b");  // Borrar el car�cter en la pantalla
            i--;
        }
    }
    // printf("%s", entrada);
    entrada[i] = '\0';  // Terminar la cadena con nulo
    // printf("%s", entrada);
}

void analizadorLexico(char* entrada, int longitud) {
	enum Estado estadoActual = INICIO;
	bool esValido;
	
	estadoActual = INICIO;

    // Pasamos la cadena a lo largo de los nodos para que se actualice
    for (int i = 0; i < longitud; i++) {
        estadoActual = transicion(estadoActual, entrada[i]);
    }

    // verificamos si es en nodo activo, de otra manera es un error
    // imprime el estado final correspondiente si es que lo hay
    
    if (!esNodoFinal(estadoActual))printf("ERROR");
    else
	{
        if (estadoActual == ENTERO_POSITIVO) printf("ENTERO POSITIVO");
        else if(estadoActual == FLOTANTE_POSITIVO) printf("FLOTANTE POSITIVO");
        else if(estadoActual == FRACCION_INDEFINIDO) printf("INDEFINIDO");
        else if(estadoActual == FRACCION) printf("FRACCION");
        else if(estadoActual == ENTERO_NEGATIVO) printf("ENTERO NEGATIVO");
        else if(estadoActual == FLOTANTE_NEGATIVO) printf("FLOTANTE NEGATIVO");
        else printf("ERROR");
    }
}

int main() {
    char entrada[100];
    int longitud;

    while (1) {  // Continuar hasta que se presione Enter sin entrada
    	system("cls");
    	gotoxy(8,5);
    	printf("**************************");
    	gotoxy(8,11);
        printf("**************************");
    	gotoxy(8,7);
        printf("Ingrese una entrada (Presione Enter para salir): ");
        entradaPersonalizada(entrada);
        longitud = strlen(entrada); // Eliminar el car�cter de nueva l�nea
        entrada[longitud] = '\0'; // Reemplazar el car�cter de nueva l�nea con nulo
        
        gotoxy(8,9);
        analizadorLexico(entrada, longitud);
        
        getch();
    }

    return 0;
}
