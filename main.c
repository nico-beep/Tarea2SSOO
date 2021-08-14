/**
 *Tarea 2 - ELO321 
 *13/08/2021
 * 
 * Integrantes:
 * Francisco Rodriguez H - 201520154-3
 * Natalia Baeza Q - 201621038-5
 * Lorens Paez P - 201630012-0
 * Nicolas Miranda V - 201721013-3
 * 
 * 
 * MAKEFILE
 * make run : para compilar y ejecutar 
 * make clean
 * 
**/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIZE 51 //50 + 1, para tener espacio para el '\0' y 50 caracteres

int checksum(char b[],int k);


int main(int argc,char* argv[]){
	
	
    /**
        *Se introducen 3 strings por consola
        *Si se ingresa mas de 50 caracteres se se trunca a solo los primeros
    **/
    char str1[SIZE];
	do{
		printf( "First string: ");
		fgets(str1, sizeof(str1), stdin);
		if (strchr(str1, '\n') == NULL) {	//buscar salto de linea en el buffer
			int ch;
			//printf("No newline found\n");
			while ((ch = fgetc(stdin)) != '\n' && ch != EOF) { 			//avanzar en los caracteres del stdin hasta '\n' o EOF
			}
		}	
	}while (strlen(str1) == 0);   
	printf("Len: %ld, %s\n", strlen(str1), str1);

	char str2[SIZE];
  	do{
		printf( "Second string: ");
		fgets(str2, sizeof(str2), stdin);
		if (strchr(str2, '\n') == NULL) {	//buscar salto de linea en el buffer
			int ch;
			//printf("No newline found\n");
			while ((ch = fgetc(stdin)) != '\n' && ch != EOF) { 			// avanzar en los caracteres del stdin hasta '\n' o EOF
			}
		}	
	}while (strlen(str2) == 0);
	printf("Len: %ld, %s\n", strlen(str2), str2);
	


	char str3[SIZE];
	do{
		printf( "Third string: ");
		fgets(str3, sizeof(str3), stdin);
		if (strchr(str3, '\n') == NULL) {	//buscar salto de linea en el buffer
			int ch;
			//printf("No newline found\n");
			while ((ch = fgetc(stdin)) != '\n' && ch != EOF) { 			// avanzar en los caracteres del stdin hasta '\n' o EOF
			}
		}	
	}while (strlen(str3) == 0);
	printf("Len: %ld, %s\n", strlen(str3), str3);

	//ints pid para resultados de los fork()
    int pid1;
    int pid2;
    int pid3;

    /**
     *Se crean pipes(1-2-3) ida(padre->hijo) y vuelta(hijo->padre)
    **/
    int pipe1_ida[2];
    int pipe1_vuelta[2];

    int pipe2_ida[2];
    int pipe2_vuelta[2];

    int pipe3_ida[2];
    int pipe3_vuelta[2];

    pipe(pipe1_ida);    
    pipe(pipe2_ida);
    pipe(pipe3_ida);

    pipe(pipe1_vuelta);
    pipe(pipe2_vuelta);
    pipe(pipe3_vuelta);


    /**
    *Creacion de solo 3 procesos hijos y sus funcionamientos
    **/
    pid1 = fork();	//primer fork()
    if(pid1 == 0){  //entra el hijo 1
        //CODIGO DE HIJO 1
        printf("Proceso hijo 1 creado, PID: %d, PPID: %d\n", getpid(), getppid());
        close(pipe1_ida[1]);        //cerrar escritura
        close(pipe1_vuelta[0]);    //cerrar lectura
        char buffer[SIZE];
        int check;
		
		
        read( pipe1_ida[0], buffer, SIZE );	//leer pipe_ida y guardar en buffer

        if(strchr(buffer, '\0')==NULL){		//revisa si el mensaje resivido tiene un final de string, buscar errores
			printf("NO '0' IN PROCESS 1\n");
		}
				
        check = checksum(buffer, strlen(buffer));			//calcular el checksum con el mensaje
        printf("Hijo1: Mensaje recibido, checksum = %d\n", check);
        write(pipe1_vuelta[1], &check, (sizeof(check)));	//enviar por pipe el int resultado del checksum

        close(pipe1_ida[0]);        //cerrar pipe1_ida
        close(pipe1_vuelta[1]);        //cerrar pipe1_vuelta
        printf("Hijo1: Exit\n");
        exit(0);


    }else {
        pid2 = fork();	//segundo fork()
        if(pid2 == 0){  //entra el hijo 2
            //CODIGO DE HIJO 2
            printf("Proceso hijo 2 creado, PID: %d, PPID: %d\n", getpid(), getppid());
            close(pipe2_ida[1]);        //cerrar escritura
            close(pipe2_vuelta[0]);    //cerrar lectura
            char buffer[SIZE];
            int check;

            read( pipe2_ida[0], buffer, SIZE );  //leer pipe_ida y guardar en buffer

            if(strchr(buffer, '\0')==NULL){		//revisa si el mensaje resivido tiene un final de string, buscar errores
				printf("NO '0' IN PROCESS 2\n");
			}
            
            check = checksum(buffer, strlen(buffer));				//calcular el checksum con el mensaje
            printf("Hijo2: Mensaje recibido, checksum = %d\n",  check);
			write(pipe2_vuelta[1], &check, (sizeof(check)));		//enviar por pipe el int resultado del checksum

            close(pipe2_ida[0]);        //cerrar pipe2_ida
            close(pipe2_vuelta[1]);        //cerrar pipe2_vuelta
            printf("Hijo2: Exit\n");
            exit(0);



        }else{
            pid3 = fork();  //tercer fork()
            if (pid3 == 0){ //entra el hijo 3
                //CODIGO DE HIJO 3
                printf("Proceso hijo 3 creado, PID: %d, PPID: %d\n", getpid(), getppid());
                close(pipe3_ida[1]);       //cerrar escritura
                close(pipe2_vuelta[0]);    //cerrar lectura
                char buffer[SIZE];
                int check;

                read( pipe3_ida[0], buffer, SIZE );  //leer pipe_ida y guardar en buffer

                if(strchr(buffer, '\0')==NULL){		//revisa si el mensaje recibido tiene un final de string, buscar errores
					printf("NO '0' IN PROCESS 3\n");
				}
                
                
                check = checksum(buffer, strlen(buffer));			//calcular el checksum con el mensaje
                printf("Hijo3: Mensaje recibido, checksum = %d\n", check);
                write(pipe3_vuelta[1], &check, (sizeof(check)));	//enviar por pipe el int resultado del checksum

            
                close(pipe3_ida[0]);            //cerrar pipe3_ida
                close(pipe3_vuelta[1]);        //cerrar pipe3_vuelta
                printf("Hijo3: Exit \n");
                exit(0);



            }else{          //el padre termino de hacer los hijos
                //CODIGO DEL PADRE
                
                close(pipe1_ida[0]);        //cerrar lectura pipe_ida
                close(pipe2_ida[0]);
                close(pipe3_ida[0]);

                close(pipe1_vuelta[1]);    //cerrar escrituras pipe_vuelta
                close(pipe2_vuelta[1]);
                close(pipe3_vuelta[1]);


				//mandar los strings por pipes
				printf("Padre: Escribiendo mensajes a procesos hijos\n");
                write(pipe1_ida[1], str1, (strlen(str1)));
                write(pipe2_ida[1], str2, (strlen(str2)));
                write(pipe3_ida[1], str3, (strlen(str3)));


                //recibir los checksum
                int buffer1, buffer2, buffer3;
				
				printf("Padre: Esperando mensajes de procesos hijos\n");	//lee los mensajes de los hijos en sus buffers individuales
                read( pipe1_vuelta[0], &buffer1, sizeof(buffer1) ) ;    	    //se reciben datos int
                read( pipe2_vuelta[0], &buffer2, sizeof(buffer2) ) ;
                read( pipe3_vuelta[0], &buffer3, sizeof(buffer3) ) ;

                printf("Padre: Checksum proceso hijo 1: %d\n", buffer1);        //print resultados
                printf("Padre: Checksum proceso hijo 2: %d\n", buffer2);
                printf("Padre: Checksum proceso hijo 3: %d\n", buffer3);
                

                close(pipe1_ida[1]);  	  //cerrar los 3 pipe_ida 
                close(pipe2_ida[1]);
                close(pipe3_ida[1]);

                close(pipe1_vuelta[0]);    //cerrar los 3 pipes_vuelta
                close(pipe2_vuelta[0]);
                close(pipe3_vuelta[0]);
                
                printf("Padre: Esperando proceso hijos para terminar...\n");
                wait(NULL);		//espera que se cierren procesos hijos
                exit(0);
            }
        }
    }

}


int checksum(char b[],int k) {
    int checksum,sum=0,i;
    for(i=0;i<k;i++){
        sum+=(int)b[i];
        checksum=~sum;
    }
    return checksum;
}
