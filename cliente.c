/* 
----------Cliente UDP------------
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MYPORT 4950 /* el puerto donde se enviaran los datos */ 

#define MAXBUFLEN 100 /* Max. cantidad de bytes que podra recibir en una llamada a recvfrom() */ 

//estructura para el manejo de paquetes
typedef struct{
  int numeroPaquete;
  char contenido[20];
  int ACK;

}Paquete;




int main(int argc, char *argv[]) 
{ 






  Paquete datos;
  int sockfd; 
  int port=MYPORT;
  char msg[MAXBUFLEN];
  char * ip;
  
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
          if(strcasecmp(buf,"TERMINADO")==0)
            {
            strcpy(buf, "TERMINADO");
            }
    /* Se visualiza lo recibido */ 
    // printf("paquete proveniente de : %s\n",inet_ntoa(their_addr.sin_addr));
 
     // printf("longitud del paquete en bytes : %d\n",numbytes);
  
        buf[numbytes] = '\0'; 

        printf("%s",buf);


        }
// enviando nombre de archivo
        while(1)
        {
          printf("Dame el nombre del archivo:\n");
          gets(msg);
          if ((numbytes=sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) 
              {
                perror("Error al enviar mensaje con: sendto"); 
                exit(1);
              } 
              printf("lo que tiene BUF%s\n",buf );
              if ((numbytes=recvfrom(sockfd, buf, MAXBUFLEN, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) 
                {
                  perror("error en recvfrom"); 
                  exit(1);
                } 
                ////intento tratar si no esta el archivo
                //  printf("%s\n",buf);
              if(strcmp(buf,"No tengo ese archivo")==0)
                {
                  printf("mande mal el nombre\n");
                }
              else
              {
                if ((numbytes=recvfrom(sockfd,&datos,sizeof(datos), 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) 
                {
                  perror("error en recvfrom"); 
                  exit(1);
                }
                printf("%s\n",datos.contenido );

              }  
        }

      }
    ///////////fin si enviamos ls

//recivimos datos



	if ((numbytes=recvfrom(sockfd, buf, MAXBUFLEN, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) 
  	{
    		perror("error en recvfrom"); 
    		exit(1);
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