#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h> 
using namespace std;
#define PLANTARSE 1
#define CONTINUAR 0
#define ABANDONAR 2
int main(int argc, char *argv[]) {                              //14 cartas maximo para aun jugador en una ronda
	cout << "ingrese la cantidad de jugadores " << endl;        
	int njugador;
	int numeros[] = {10,20,30,40,50,60,70,5};
	srand(time(NULL));
	cin >> njugador;
	pid_t* pid = new pid_t[njugador];
	auto serjug = new int[njugador][2];
	auto jugser = new int[njugador][2];
	for(int i = 0;i<njugador;i++){
		pipe(serjug[i]);
		pipe(jugser[i]);
		pid[i] = fork();
		if(pid[i] == 0){     //jugador
			int accion = CONTINUAR; 
			int cartas[14] = {0}, puntos = 0, buf = 0;
			for(int j=0;j<14;j++){
				close(serjug[i][1]);   //El jugador no escribe en el pipe servidor-->jugdos
				close(jugser[i][0]);  //El jugador no lee del pipe jugador-->servidor
				read(serjug[i][0],&buf,sizeof(int));
				cartas[j] = buf;
				puntos += buf;
				cout << "J" << i << "- ha leido la carta " << (double)cartas[j]/10 << endl;
				if(puntos > 50 && puntos <= 75){
					//accion = PLANTARSE;
					int puntosPLantarse[2] = {PLANTARSE, puntos};
					write(jugser[i][1],puntosPLantarse,2*sizeof(int));					
					cout << "J" << i << "PP- se planta y tiene puntos: " << (double)puntos/10 << endl;
					exit(0);
				}else{
					if(puntos > 75){
						accion = ABANDONAR;
						write(jugser[i][1],&accion,sizeof(int));
						cout << "J" << i << "XX- abandona " << "con " << (double)puntos/10 << " puntos" << endl;
						exit(0);
					}else{
						accion = CONTINUAR;
						write(jugser[i][1],&accion,sizeof(int));
					}
				}
			}
		}else{
			if(pid[i] == -1){
				cout << "no se pudo crear el proceso numero " << i << endl;
			}
			
		}
	}
	int estado = 1;
	int* respuestas = new int[njugador];
	int* resultJug = new int[njugador];
	for(int i=0;i<njugador;i++){                //cierro los pipes que no usa el server
		close(serjug[i][0]);   //El server no lee del pipe servidor-->jugdos
		close(jugser[i][1]);  //El jugador no escribe en el pipe jugador-->servidor
	}
	for(int i =0;i<njugador;i++){
		respuestas[i] = CONTINUAR;
		resultJug[i] = 0;
		int temp = rand()%8;
		write(serjug[i][1],&numeros[temp],sizeof(int)); 
		cout << "Repartidor envia un " << (double)numeros[temp]/10 << " a J" << i << endl;
	}
	while(estado){
		estado = 0;
		for(int i =0;i<njugador;i++){
			if(respuestas[i] == CONTINUAR){
				read(jugser[i][0],&respuestas[i],sizeof(int)); 
				if(respuestas[i] == CONTINUAR){ 
					estado = 1;
					int temp = rand()%8;
					write(serjug[i][1],&numeros[temp],sizeof(int)); 
					cout << "Repartidor envia un " << (double)numeros[temp]/10 << " a J" << i << endl;
				}else{
					if(respuestas[i] == PLANTARSE){
						read(jugser[i][0], &resultJug[i], sizeof(int));
					}
				}
			}	
		}
		
		
	}
	for(int i=0;i<njugador;i++){
		waitpid(pid[i],0,0);
	}
	cout << "-------------------------------------------------------------"<< endl;
	for(int i=0;i<njugador;i++){
		switch (respuestas[i]){
			case ABANDONAR: cout << "Jugador " << i << "--Estado: Abandonó" << endl;
			break;
			case PLANTARSE: cout << "Jugador " << i << "--Estado: Se PLantó con " << (double)resultJug[i]/10 << " puntos" << endl;
			break;  
		default: cout << "default"<<endl;
		}
	}
	return 0;
}
