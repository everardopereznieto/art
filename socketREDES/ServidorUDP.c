/* 
----------Servidor UDP------------
gcc srvUDP1.c -o srvUDP1
./srvUDP1
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MYPORT 4950 /* puerto donde
los cliente envian los paquetes */ 

#define MAXBUFLEN 100 /* Max. cantidad de bytes que podra recibir en una llamada a recvfrom() */ 

main(int argc, char *argv[]) 
{ 
  int sockfd; 
  int port;	/* El puerto a utilizar */	

  struct sockaddr_in my_addr;	/* direccion IP y numero de puerto local */ 

  struct sockaddr_in their_addr;	/* direccion IP y numero de puerto del cliente */ 

  /* addr_len contendra el tamanio de la estructura sockadd_in
    y numbytes el numero de bytes recibidos */ 

  int addr_len, numbytes; 

  char buf[MAXBUFLEN];	/* Buffer de recepcion */ 
  char msg[MAXBUFLEN];

  /* Tratamiento de la linea de comandos. */
  if (argc < 2)
  {
    /* No especificó el puerto a escuchar. */
    port=MYPORT;
  }
  else
  {
    /* Asigna a Port el Puerto especificado. */
    port=atoi(argv[1]);
  }
  
  /* se crea el socket */ 
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
  { 
    perror("error al crear el socket"); 
    exit(1);
  } 

  /* Se establece la estructura my_addr para luego llamar a bind() */ 

  my_addr.sin_family = AF_INET;  /* host byte order */ 

  my_addr.sin_port = htons(port); /* network byte order (BigEndian)*/ 

  my_addr.sin_addr.s_addr = INADDR_ANY; /* se asigna automaticamente la direccion IP local */ 

  bzero(&(my_addr.sin_zero), 8);  /* rellena con ceros el resto de la estructura */
 

  /* Se le da un nombre al socket */ 
  if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) 
  {
    perror("error en el bind"); 
    exit(1);
  } 

   /* Se reciben los datos */ 
  addr_len = sizeof(struct sockaddr); 
  char buffer[MAXBUFLEN];
  do{
  	if ((numbytes=recvfrom(sockfd, buf, MAXBUFLEN, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) 
  	{
    		perror("error en recvfrom"); 
    		exit(1);
  	} 

  	/* Se visualiza lo recibido */ 
  	printf("Paquete proveniente de : %s:%d\n",inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port));
   	printf("longitud del paquete en bytes : %d\n",numbytes);
  	buf[numbytes] = '\0'; 

  	printf("el paquete contiene : %s\n",buf);

	sprintf(buffer,"ip:%s puerto:%d msg:%s",inet_ntoa(their_addr.sin_addr),ntohs(their_addr.sin_port),buf);
	
	if ((numbytes=sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) 
  	{
    	perror("Error al enviar mensaje con: sendto"); 
    	exit(1);
  	}

	printf("Enviados: %d bytes:--%s--\n",numbytes, buffer);
  	
	
  }while(strcmp(buf,"CLOSE"));
  /* devolvemos recursos al sistema */ 
  close(sockfd); 

} 
