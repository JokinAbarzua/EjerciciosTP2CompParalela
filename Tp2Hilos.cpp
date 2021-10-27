#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

void* thread1(void*);
void* thread2(void*);
int cont = 0;
int main(int argc, char *argv[]) {
	
	int n, m;
	cout << "Ingrese el numero de threads tipo 1" << endl;
	cin >> n;
	pthread_t *id1 = new pthread_t[n];
	cout << "Ingrese el numero de threads tipo 2" << endl;
	cin >> m;
	pthread_t *id2 = new pthread_t[m];
	
	for(int i =0;i<n;i++){
		if(pthread_create(&id1[i],NULL, thread1, (void*)i )){  //i es de tipo int y lo caste o tipo puntero a void
			cout << "no se pudo crear el thread tipo 1 numero " << i << endl;
		}
	}
	for(int i =0;i<m;i++){
		if(pthread_create(&id2[i],NULL, thread2, (void*)i )){  //i es de tipo int y lo caste o tipo puntero a void
			cout << "no se pudo crear el thread tipo 2 numero " << i << endl;
		}
	}
	for(int i =0;i<n;i++){
		pthread_join(id1[i],NULL);
	}
	for(int i =0;i<m;i++){
		pthread_join(id2[i],NULL);
	}
	cout << "programa finalizado." << endl;
	delete []id1;
	delete []id2;
	return 0;
}

void* thread1(void* args){
	srand(time(NULL));
	long nro = (long)args;
	cout << "Thread1 instancia " << nro << endl;
	usleep(rand()%1000000);
	cont++;
	pthread_exit(NULL);
}
	
void* thread2(void* args){
	srand(time(NULL));
	long nro = (long)args;
	cout << "Thread2 instancia " << nro << endl;
	usleep(rand()%1000000);
	cout << "variable compartida: " << cont << endl;
	pthread_exit(NULL);
}
