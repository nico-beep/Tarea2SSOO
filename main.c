#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


int checksum(char b[],int k) ;

int main(int argc,char* argv[]){

    /*
        *Se introducen 3 strings por consola
    */
    char str1[50];
	do{
		printf( "First string: ");
		fgets(str1, sizeof(str1), stdin);
	}while (strlen(str1) == 0);   

	char str2[50];
  	do{
		printf( "Second string: ");
		fgets(str2, sizeof(str2), stdin);
	}while (strlen(str2) == 0);
	


	char str3[50];
	do{
		printf( "Third string: ");
		fgets(str3, sizeof(str3), stdin);
	}while (strlen(str3) == 0);


    int pid1;
    int pid2;
    int pid3;

    /*
        *pipes ida(padre->hijo) y vuelta(hijo->padre)
    */
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

    /*
        *Creacion de solo 3 procesos hijos y sus funcionamientos
    */
    pid1 = fork();
    if(pid1 == 0){  //entra el hijo 1
        //codigo de hijo 1
        printf("Proceso hijo 1\n");
        close(pipe1_ida[1]);        //cerrar escritura
        close(pipe1_vuelta[0]);    //cerrar lectura
        char buffer[50];
        int readbytes;
        int check;

        while( (readbytes=read( pipe1_ida[0], buffer, 50 ) ) > 0);
        
        check = checksum(buffer, strlen(buffer));

        close(pipe1_ida[0]);        //cerrar pipe1_ida
        close(pipe1_vuelta[1]);        //cerrar pipe1_vuelta
        printf("Exit proceso hijo 1\n");
        exit(0);


    }else {
        pid2 = fork();
        if(pid2 == 0){  //entra el hijo 2
            //codigo de hijo 2
            printf("Proceso hijo 2\n");
            close(pipe2_ida[1]);        //cerrar escritura
            close(pipe2_vuelta[0]);    //cerrar lectura
            char buffer[50];
            int readbytes;
            int check;

            while( (readbytes=read( pipe1_ida[0], buffer, 50 ) ) > 0);
            
            check = checksum(buffer, strlen(buffer));

            close(pipe2_ida[0]);        //cerrar pipe2_ida
            close(pipe2_vuelta[1]);        //cerrar pipe2_vuelta
            printf("Exit proceso hijo 2\n");
            exit(0);



        }else{
            pid3 = fork();
            if (pid3 == 0){ //entra el hijo 3
                //codigo de hijo 3
                printf("Proceso hijo 3\n");
                close(pipe3_ida[1]);       //cerrar escritura
                close(pipe2_vuelta[0]);    //cerrar lectura
                char buffer[50];
                int readbytes;
                int check;

                while( (readbytes=read( pipe1_ida[0], buffer, 50 ) ) > 0);
                check = checksum(buffer, strlen(buffer));
                write(pipe3_vuelta[1], check, (sizeof(check)));

                check = checksum(buffer, strlen(buffer));
            
                close(pipe3_ida[0]);        //cerrar pipe3_ida
                close(pipe3_vuelta[1]);        //cerrar pipe3_vuelta
                printf("Exit proceso hijo 3\n");
                exit(0);



            }else{          //el padre termino de hacer los hijos
                //codigo del padre
                //mandar los strings
                close(pipe1_ida[0]);    //cerrar lectura
                close(pipe2_ida[0]);
                close(pipe3_ida[0]);

                close(pipe1_vuelta[1]);    //cerrar escrituras
                close(pipe2_vuelta[1]);
                close(pipe3_vuelta[1]);


                write(pipe1_ida[1], str1, (strlen(str1)));
                write(pipe2_ida[1], str2, (strlen(str2)));
                write(pipe3_ida[1], str3, (strlen(str3)));

                //recibir los checksum
                int buffer1, buffer2, buffer3;
                int readbytes1, readbytes2, readbytes3;

                while( (readbytes1=read( pipe1_vuelta[0], buffer1, sizeof(buffer1) ) ) > 0);
                while( (readbytes2=read( pipe2_vuelta[0], buffer2, sizeof(buffer2) ) ) > 0);
                while( (readbytes3=read( pipe3_vuelta[0], buffer3, sizeof(buffer3) ) ) > 0);

                printf("Checksum proceso 1: %d\n", buffer1);
                printf("Checksum proceso 2: %d\n", buffer2);
                printf("Checksum proceso 3: %d\n", buffer3);
                

                close(pipe1_ida[1]);    //cerrar lectura
                close(pipe2_ida[1]);
                close(pipe3_ida[1]);

                close(pipe1_vuelta[0]);    //cerrar escrituras
                close(pipe2_vuelta[0]);
                close(pipe3_vuelta[0]);
                
                printf("Esperando proceso hijo 3\n");
                wait(pid1);
                
                printf("Esperando proceso hijo 3\n");
                wait(pid2);
                
                printf("Esperando proceso hijo 3\n");
                wait(pid3);
                exit(0);
            }
        }
    }

}


int checksum(char b[],int k) {
    int checksum,sum=0,i;
    for(i=0;i<k;i++)
        sum+=(int)b[i];
        printf("SUM IS: %d\n",sum);
         checksum=~sum;
        printf("CHECKSUM IS: %d\n",checksum);
    return checksum;
}