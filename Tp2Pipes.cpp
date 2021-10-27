#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>                  
using namespace std;

int main(int argc, char *argv[]) {
	int pfd[2], n;
	pid_t pid;
	char buf[20];
	if(pipe(pfd)){//creo el pipe
		cout << "no se pudo crear el pipe" << endl;
	}else{
		cout<< "se pudo crear el pipe." << endl
			<< "   >Lectura: "<< pfd[0] << endl
			<< "   >Escritura: "<< pfd[1] << endl;
	}
	pid = fork();
	if(pid){//cod padre----productor
		if(pid == -1) cout << "no se pudo crear el proceso consumidor" << endl;
		for(int i=0;i<10;i++){
			cout << "Ingrese una frase para pasar el consumidor" << endl;
			const char* frase;
			scanf("%s", frase);
			strcpy(buf, frase);
			write(pfd[1],buf,20*sizeof(char));
			//cout << "productor produce" << endl;
		}
		close(pfd[1]);
		
	}else{ //cod hijo----consumidor
		for(int i=0;i<10;i++){
			read(pfd[0],buf,20*sizeof(char));
			cout << "consumidor consume " << buf << endl;
		}
		close(pfd[0]);
	}
	
	
	
	while(1){
		
	}
	return 0;
}

