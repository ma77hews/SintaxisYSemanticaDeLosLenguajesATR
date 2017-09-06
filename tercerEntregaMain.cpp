#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
int FinDeTexto=0;		//lo uso en la condicion del while del scanner para que no sea infinito
static int mat[15][13] = {{1,3,5,6,7,8,9,10,11,14,13,0,14}, 	//matriz del automata
							{1,1,2,2,2,2,2,2,2,2,2,2,2},
							{99,99,99,99,99,99,99,99,99,99,99,99,99},
							{4,3,4,4,4,4,4,4,4,4,4,4,4},
							{99,99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99,99},
							{14,14,14,14,14,14,14,14,14,12,14,14,14},
							{99,99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99,99},
							{99,99,99,99,99,99,99,99,99,99,99,99,99}};
							
typedef enum { 
INICIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE, 
PARENIZQUIERDO,PERENDERECHO, PUNTOYCOMA, COMA, ASIGNACION, SUMA,RESTA,FDT,PALABRARESERVADA, ERRORLEXICO
} TOKEN;

char buffer[100]; //buffer donde guardo lo que leo

void limpiarBuffer(){	//vacio el buffer cuando el escanner detecta un lexema
	for(int i=0;i<=100;i++){
		buffer[i]='\0';
	}
}

void agregarCaracter(char caracter){	//cargo caracter al buffer
int i;
	for(i= 0; buffer[i] != '\0' ; i++){
	}
	buffer[i]=caracter;
}

TOKEN esReservada(){	//Para diferenciar identificador de palabra reservada
	if (buffer[0] == 'i' && buffer[1]=='n' && buffer[2]=='i' && buffer[3]=='c' && buffer[4]=='i' 
		&& buffer[5]=='o' && buffer[6] == '\0'){
		return INICIO;
	} 
	if (buffer[0] == 'l' && buffer[1]=='e' && buffer[2]=='e' && buffer[3]=='r' && buffer[4]=='\0'){
	
		return LEER;
	}
	if(buffer[0] == 'e' && buffer[1]=='s' && buffer[2]=='c' && buffer[3]=='r' && buffer[4]=='i' 
		&& buffer[5]=='b' && buffer[6]=='i' && buffer[7]=='r' && buffer[8]=='\0'){
			return ESCRIBIR;
		}
	if(buffer[0] == 'f' && buffer[1]=='i' && buffer[2]=='n' && buffer[3]=='\0'){
		return FIN;
	}
	else{
		return ID;
	}
}

bool medirCadena(){		//los identificadores deben medir menos de 32 caracteres
	int contador=0;
	for(int i = 0;buffer[i] != '\0';i++){
		contador++;
		}
	if(contador <= 32){
		return true;
	}else{
		return false;
	}
}

int columna(char a, FILE* f)		//Funcion que me ubica en la columna de la matriz
{
    if(isalpha(a)){
    	return 0;
	}
	if(isdigit(a)){
		return 1;
	}
	if(a == '+'){
		return 2;
	}
	if(a == '-'){
		return 3;
	}
	if(a == '('){
		return 4;
	}
	if(a == ')'){
		return 5;
	}
	if(a == ','){
		return 6;
	}
	if(a == ';'){
		return 7;
	}
	if(a == ':'){
		return 8;
	}
	if(a == '='){
		return 9;
	}
	if(feof(f)){
		FinDeTexto=1;
		return 10;
	}
	if(isspace(a)){
		return 11;
	}
	else{
		return 12;
	}
}
TOKEN scanner (FILE * f){
    int estado=0;
    int contador=0;
    char caracter;
    int col;
    caracter = fgetc(f);
    while (FinDeTexto==0)
    {	
        col = columna(caracter,f);
        estado = mat[estado][col];
	    switch (estado){
		case 0:
			limpiarBuffer();
			caracter = fgetc(f);
    		break;
    	case 1:
    		agregarCaracter(caracter);
    		 caracter = fgetc(f);
    		break;
    	case 2:
    		ungetc(caracter,f);
    		if(esReservada()!= ID){
    			while(buffer[contador]!='\0'){
				printf("%c",buffer[contador]);
				contador++;
				}
				printf("    PALABRA RESERVADA \n");
				}else{
					if(medirCadena()){
				while(buffer[contador]!='\0'){
				printf("%c",buffer[contador]);
				contador++;
				}
				printf("    IDENTIFICADOR \n");
					}else{
						printf("ERROR LEXICO,IDENTIFICADOR SUPERA LIMITE DE CARACTERES \n");
					}
					
				}
				limpiarBuffer();
				caracter = fgetc(f);
				contador=0;
				 estado=0;				 
		break;
    	case 3:
    		agregarCaracter(caracter);
    		 caracter = fgetc(f);
    		break;
    	case 4:
    		ungetc(caracter,f);
    		while(buffer[contador]!='\0'){
				printf("%c",buffer[contador]);
				contador++;
				}
			printf("    DIGITO \n");
			 estado=0;
			 contador=0;
			 limpiarBuffer();
			 caracter = fgetc(f);
			break;
		case 5:
			printf("+");
			printf("    SUMA \n");
			caracter = fgetc(f);
			estado=0;
			break;
		case 6:
			printf("-");
			printf("    RESTA \n");
			caracter = fgetc(f);
			estado=0;
			break;
		case 7:
			printf("(");
			printf("    SIGNO DE PUNTUACION \n");
			caracter = fgetc(f);
			estado=0;
			break;
		case 8:
			printf(")");
			printf("    SIGNO DE PUNTUACION \n");
			caracter = fgetc(f);
			estado=0;
			break;
		case 9:
			printf(",");
			printf("    SIGNO DE PUNTUACION \n");
			caracter = fgetc(f);
			estado=0;
			break;
		case 10:
			printf(";");
			printf("    SIGNO DE PUNTUACION \n");
			caracter = fgetc(f);
			estado=0;
			break;
		case 11:
			printf(":");
			printf("    SIGNO DE PUNTUACION \n");
			caracter = fgetc(f);
			break;
		case 12:
			printf("=");
			printf("    SIGNO DE PUNTUACION \n");
			caracter = fgetc(f);
			estado=0;
			break;
		case 13:
			return FDT;
			break;
		case 14:
			printf("ERROR LEXICO \n");
			estado=0;
			caracter = fgetc(f);
			break;
}
}
}


int main(int argc, char** argv) {
	FILE* f=fopen("micro.txt","r+b");
	scanner(f);
	fclose(f);
	return 0;
}
