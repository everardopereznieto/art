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

//estructura para el manejo de paquetes
typedef struct{
  int numeroPaquete;
  char contenido[20];
  int ACK;

}Paquete;

main(int argc, char *argv[]) 
{ 
  FILE *ficherols, *ficherosend;
  int sockfd; 
  int port; /* El puerto a utilizar */  
///////////estructura de prueva
  Paquete datos;
  datos.numeroPaquete=1;
  strcpy(datos.contenido,"hola");
  datos.ACK=2;

  printf("%s\n",datos.contenido);
  ///////////
  struct sockaddr_in my_addr; /* direccion IP y numero de puerto local */ 

  struct sockaddr_in their_addr;  /* direccion IP y numero de puerto del cliente */ 

  /* addr_len contendra el tamanio de la estructura sockadd_in
    y numbytes el numero de bytes recibidos */ 

  int addr_len, numbytes; 

  char buf[MAXBUFLEN];  /* Buffer de recepcion */ 
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
    printf("Paquete proveniente de Cliente: %s puerto:%d longitud: %d bytes\n",inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port),numbytes);
    // printf("longitud del paquete en bytes : %d\n",numbytes);
    buf[numbytes] = '\0'; 

    printf(": %s\n",buf);

    sprintf(buffer,"ip:%s puerto:%d msg:%s",inet_ntoa(their_addr.sin_addr),ntohs(their_addr.sin_port),buf);
 
    ///si solicitan la lista de archivos
    if (strcasecmp(buf,"ls")==0)
    {
      system("ls -l> ls.txt");
      ficherols= fopen("ls.txt","r");
        while(feof(ficherols) == 0)
        {

        fgets(buffer, MAXBUFLEN, ficherols);

        if ((numbytes=sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) 
          {
          perror("Error al enviar mensaje con: sendto"); 
          exit(1);
          }
          printf("%s",buffer );
        }
      strcpy(buffer, "TERMINADO");
      if ((numbytes=sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) 
        {
          perror("Error al enviar mensaje con: sendto"); 
          exit(1);
        }
        printf("Envia Servidor: %d numbytes:  buffer--%s--\n",numbytes, buffer);
      fclose(ficherols);
//ciclo para manejar el envio de archivo 
      while(1)
      {


          if ((numbytes=recvfrom(sockfd, buf, MAXBUFLEN, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) 
            {
              perror("error en recvfrom"); 
              exit(1);
            }

           if( ficherosend=fopen(buf,"r")){
            printf("enviando...........\n");
              if ((numbytes=sendto(sockfd,&datos,sizeof(datos), 0, (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) 
                  {
                    perror("Error al enviar mensaje con: sendto"); 
                    exit(1);
                  }
            fclose(ficherosend);
            break;
           }
           //manejo si no tengo el archivo 
           else {
                strcpy(buffer, "No tengo ese archivo");
                printf("%s\n",buffer );
                if ((numbytes=sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) 
                  {
                    perror("Error al enviar mensaje con: sendto"); 
                    exit(1);
                  }
          }

      }//ciclo para manejar el envio de archivo 

    }
    //termina lista de archivos


//enviamos datos
  printf("Mensaje:\n");
  gets(buffer);
  
  if ((numbytes=sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) 
    {
      perror("Error al enviar mensaje con: sendto"); 
      exit(1);
    }

  printf("Enviados: %d bytes:\n",numbytes);
    
  //////////termina enviados
  }while(strcmp(buf,"CLOSE"));
  /* devolvemos recursos al sistema */ 
  close(sockfd); 

} 