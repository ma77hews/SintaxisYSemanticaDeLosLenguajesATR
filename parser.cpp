#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <iostream>
#include <libreria1.h>
FILE* f = fopen("micro.txt","r+b");
TOKEN tokenActual;
void Expresion (void);
void ErrorSintactico(TOKEN a){
	return ;
}
bool Match(TOKEN a){
	if (tokenActual == a){
		return true;
	}else{
		printf("error semantico \n");
		return false;
	}	
}
TOKEN proximoToken(void){
	return scanner(f);
}
void ListaDeIdentificadores(){
	Match(ID);
	while(!Match(PARENDERECHO)){
		Match(COMA);
		Match(ID);
	}
}
void Primaria(){
	tokenActual = proximoToken();
	switch(tokenActual){
		case ID:
			Match(ID);
			break;
		case CONSTANTE:
			Match(CONSTANTE);
			break;
		case PARENIZQUIERDO:
			Expresion();
			Match(PARENDERECHO);
			break;
	}
}
void OperadorAditivo(void){
	TOKEN t= proximoToken();
	if(t==SUMA || t== RESTA){
		Match(t);
	}
	else{
		ErrorSintactico(t);
	}
}
void Expresion (void){
	Primaria();
	tokenActual = proximoToken();
	for(tokenActual;tokenActual==SUMA || tokenActual == RESTA;tokenActual = proximoToken()){
		OperadorAditivo(); Primaria();
	}
}
void ListaDeExpresiones(){
	TOKEN t;
	Expresion();
	t= proximoToken();
	while(t != PARENDERECHO){
		Match(t);
		Expresion();
		t = proximoToken();
	}
}

void Sentencia(void){
	tokenActual = proximoToken();
	switch(tokenActual){
		case ID:
			Match(ID);
			tokenActual = proximoToken();
			Match(ASIGNACION);
			Expresion();
			Match(PUNTOYCOMA);
			break;
		case LEER:
			Match(LEER);Match(PARENIZQUIERDO); ListaDeIdentificadores(); Match(PARENDERECHO); Match(PUNTOYCOMA);
			break;
		case ESCRIBIR:
			Match(ESCRIBIR);Match(PARENIZQUIERDO); ListaDeExpresiones(); Match(PARENDERECHO); Match(PUNTOYCOMA);
			break;
		default:
			ErrorSintactico(tokenActual); 
			break;
	}
}
void ListaSentencias(void){
	Sentencia();
	while(1){
			tokenActual = proximoToken();
		switch (tokenActual){
			case ID: case LEER: case ESCRIBIR:
				Sentencia();
				break;
			default:
				return;
		}
	}
}

void Programa(void){
	tokenActual = proximoToken();
	Match(INICIO);
	ListaSentencias();
	Match(FIN);
}

void Objetivo(void){
	Programa();
	tokenActual = proximoToken();
	Match(FDT);
}


int main(int argc, char *argv[]) {
	Objetivo();
	fclose(f);
	return 0;
}
