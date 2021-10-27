#include <iostream>
#include <pthread.h>
#include <unistd.h>                //se observa que a diferencia del ej1 con threads
#include <stdlib.h>                //aquí las variables globales no se comparten
#include <sys/wait.h>               
using namespace std;           
int cont = 0;
int main(int argc, char *argv[]) {
	int n, m;
	srand(time(NULL));
	cout << "Ingrese el numero de procesos tipo 1" << endl;
	cin >> n;
	pid_t *pid1 = new pid_t[n];
	cout << "Ingrese el numero de procesos tipo 2" << endl;
	cin >> m;
	pid_t *pid2 = new pid_t[m];
	for(int i = 0; i<n; i++){
		pid1[i] = fork();
		if(pid1[i]){//codigo padre
			if(pid1[i] == -1) cout << "no se pudo crear el proceso de tipo 1 instancia " << i << endl;  
		}else{//codigo proceso hijo
			cout << "Proceso tipo 1 instancia "<< i << endl;
			usleep(rand()%1000000);
			cont++;
			exit(0);
		}	
	}
	for(int i = 0; i<m; i++){
		pid2[i] = fork();
		if(pid2[i]){ //codigo padre
			if(pid2[i] == -1) cout << "no se pudo crear el proceso de tipo 2 instancia " << i << endl;
		}else{//codigo proceso hijo
			cout << "Proceso tipo 2 instancia "<< i << endl;
			usleep(rand()%1000000);
			cout << "variable compartida " << cont << endl;
			exit(0);
		}	
	}
	for(int i = 0; i<n; i++){
		waitpid(pid1[i], 0,0);
	}
	for(int i = 0; i<m; i++){
		waitpid(pid2[i], 0,0);
	}
	cout << "programa finalizado." << endl;
	delete []pid1;
	delete []pid2;
	return 0;
}

