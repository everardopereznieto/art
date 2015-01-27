/* 
----------Cliente UDP------------
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


#define MYPORT 4950 /* el puerto donde se enviaran los datos */ 

#define MAXBUFLEN 100 /* Max. cantidad de bytes que podra recibir en una llamada a recvfrom() */ 

//estructura para el manejo de paquetes
typedef struct{
  char nombre[50];
  int existe;
  int tamanio;
  char contenido[1500];
  int nPaquete;
  int ACK;

}Paquete;


int main(int argc, char *argv[]) 
{ 




  FILE * ficherorecv;

  Paquete datos;
  // //////inicializo estructura
  // datos.nPaquete=0;
  // strcpy(datos.contenido," ");
  // datos.ACK=0;
  // datos.existe=0;
  // strcpy(datos.nombre," ");
  // datos.tamanio=0;
  int sockfd; 
  int port=MYPORT;
  char msg[MAXBUFLEN];
  char * ip;
  int cuentafichero, sizecontenido, sizefichero;
  
  struct sockaddr_in their_addr; /* Almacenara la direccion IP y numero de puerto del servidor */ 
  int addr_len, numbytes; 
  char buf[MAXBUFLEN];

  if (argc >= 2)
  {
    ip = argv[1];
    
    if (argc == 3) 
    {
      port = atoi(argv[2]);
    }
  }
  else
  { 
    fprintf(stderr,"Uso: cteUDP <ip> [puerto]\n");
    exit(1);
  } 

  /* Creamos el socket */ 
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
  {
    perror("Error al crear socket"); 
    exit(1);
  } 


  their_addr.sin_family = AF_INET; /* host byte order - LE*/ 

  their_addr.sin_port = htons(MYPORT); /* network byte order - BE*/ 

  their_addr.sin_addr.s_addr=inet_addr(ip);
 
  bzero(&(their_addr.sin_zero), 8); 
  
  addr_len = sizeof(struct sockaddr);

  do{
  	/* Solicitamos mensaje */ 
  	printf("Mensaje a enviar: ");
  	gets(msg);   
  
  	/* enviamos el mensaje, esta linea contiene una barra invertida al final, indicando que sigue abajo*/ 
  	   if ((numbytes=sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) 
  	   {
        	perror("Error al enviar mensaje con: sendto"); 
    	   exit(1);
  	   } 

  	printf("Cliente envia %d bytes a Servidor  %s\n",numbytes,inet_ntoa(their_addr.sin_addr));
	
    //si enviamos ls

      if(strcasecmp(msg,"ls")==0)
      {

        printf("reciviendo ls.txt\n");
        while(strcasecmp(buf,"TERMINADO")!=0)
        {
          if ((numbytes=recvfrom(sockfd, buf, MAXBUFLEN, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) 
          {
            perror("error en recvfrom"); 
            exit(1);
          } 
    ///si termina de enviar envia msg '\0'
          // if(strcasecmp(buf,"TERMINADO")==0)
          //   {
          //   strcpy(buf, "TERMINADO");
          //   }
    /* Se visualiza lo recibido */ 
    // printf("paquete proveniente de : %s\n",inet_ntoa(their_addr.sin_addr));
 
     // printf("longitud del paquete en bytes : %d\n",numbytes);
  
        buf[numbytes] = '\0'; 

        printf("%s",buf);


        }
//////////////////////////////7 enviando nombre de archivo//////////////////////////////////////////////////77
        while(1)
        {
          printf("\nDame el nombre del archivo:\n");
          gets(datos.nombre);
          printf("%s\n",datos.nombre );
          if ((numbytes=sendto(sockfd,&datos,sizeof(datos), 0, (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) 
              {
                perror("Error al enviar mensaje con: sendto"); 
                exit(1);
              } 
              if ((numbytes=recvfrom(sockfd,&datos,sizeof(datos), 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) 
                {
                  perror("error en recvfrom"); 
                  exit(1);
                } 
                ////intento tratar si no esta el archivo
              if(datos.existe==0)
                {
                  printf("mande mal el nombre[%d]\n",datos.existe);
                }
             





              if(datos.existe==1)//////////////////////recibo tamaño de archivo////////////////////////////////////
              {
                printf("El archivo si existe [datos.existe]%d\n",datos.existe );
                printf("reciviendo el archivo...........[%s]\n",datos.nombre);
                ficherorecv=fopen(datos.nombre,"wb+");
                printf("%d\n",datos.tamanio );
                sizecontenido = sizeof(datos.contenido);
                cuentafichero=sizeof(datos.contenido);
                sizefichero=datos.tamanio;
                int contadorPaquetes; 
                contadorPaquetes =1;
                int posicionfseek;
                int errnak1,errnak2,errnak3,errnak4;
                errnak1=1;errnak2=1;errnak3=1;errnak4=1;
                ///////////////recibo archivo///////////////////
                fcntl (sockfd, F_SETFL, O_NONBLOCK);/*se determina que el socket no es bloqueante*/
                while(1){



                  while(1){

                           while(1){

                                  if ((numbytes=recvfrom(sockfd,&datos,sizeof(datos), 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) 
                                    {;}
                                     else{
                                      break;
                                     }
                                  }
                                if (cuentafichero < sizefichero && contadorPaquetes == datos.nPaquete){
                                  //printf("%s\n",datos.contenido );
                                  posicionfseek=(datos.nPaquete * sizeof(datos.contenido))-sizeof(datos.contenido);
                                    fseek(ficherorecv,posicionfseek,SEEK_SET);
                                  fwrite(datos.contenido,sizeof(datos.contenido),1,ficherorecv);
                                  //memset( datos.contenido , 0 , sizeof( datos.contenido ) );
                                  printf("Copiando...[%d] pack %d-->[%d]\n",cuentafichero,datos.nPaquete,datos.tamanio);
                                  
                                  datos.ACK=1;
                                  while(errnak1){if(datos.nPaquete==3){datos.ACK=0;errnak1=0;printf("Envio NAK:%d --> Paquete[%d] \n",datos.ACK,datos.nPaquete);}else {break;}}
                                  while(errnak2){if(datos.nPaquete==6){datos.ACK=0;errnak2=0;printf("Envio NAK:%d --> Paquete[%d] \n",datos.ACK,datos.nPaquete);}else {break;}}
                                  while(errnak3){if(datos.nPaquete==10){sleep(5);errnak3=0;printf("Envio NAK:%d --> Paquete[%d] \n",datos.ACK,datos.nPaquete);}else {break;}}
                                  while(errnak4){if(datos.nPaquete==15){datos.ACK=0;errnak4=0;printf("Envio NAK:%d --> Paquete[%d] \n",datos.ACK,datos.nPaquete);}else {break;}}



                    if ((numbytes=sendto(sockfd,&datos,sizeof(datos), 0, (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) 
                          {
                            perror("Error al enviar mensaje con: sendto"); 
                            exit(1);
                           }
                  }
                    if(datos.ACK==1){
                    contadorPaquetes +=1;
                    break;
                    }

                }
                  if(cuentafichero > datos.tamanio){
                    cuentafichero = cuentafichero -sizecontenido;
                    sizefichero = sizefichero - cuentafichero;
                    fwrite(datos.contenido,sizefichero,1,ficherorecv);
                    printf("Copiando...[%d] pack %d-->[%d]\n",sizefichero,datos.nPaquete,datos.tamanio);
                    fclose(ficherorecv);
                    printf("termine de copiar\n");
                    break;
                  }
                  cuentafichero +=sizecontenido;




                }///////termina recibo archivo
                
                break;
              }  












        }

      }
    ///////////fin si enviamos ls

//recivimos datos
printf("recibo\n");

while(1){

	if ((numbytes=recvfrom(sockfd, buf, MAXBUFLEN, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) 
  	{;
  	} 
    else{
      break;
    }
  }
  	/* Se visualiza lo recibido */ 
  	printf("proveniente de Servidor: %s longitud %d bytes\n",inet_ntoa(their_addr.sin_addr), numbytes);
 
  	// printf("longitud del paquete en bytes : %d\n",numbytes);
 	
  	buf[numbytes] = '\0'; 

  	printf("el paquete contiene : %s\n",buf);


  


  }while(strcmp("CLOSE",msg));
  close(sockfd); 

  return 0; 

} 