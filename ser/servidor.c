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
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>


#define MYPORT 4950 /* puerto donde
los cliente envian los paquetes */ 

#define MAXBUFLEN 100 /* Max. cantidad de bytes que podra recibir en una llamada a recvfrom() */ 

int I=0;
int termino,id,timeout;
double secs,compara=0.99;
clock_t comienzo, final;
struct tm *tiempoComienzoPtr, *tiempoFinalPtr;




/*funcion del hilo*/
void *codigo_del_hilo (void *id)
{   
   while(1){
    final = clock();
      if(timeout==1){
        printf("termino hilo\n");
        pthread_exit(id);
        break;
      }
      secs = (double)(final - comienzo) / CLOCKS_PER_SEC;
      if(secs>compara){
        printf("Termino TIEMPO\n");
      timeout=1;
      pthread_exit(id);
          break;   
      }
   }
}
/**/




//estructura para el manejo de paquetes
typedef struct{
  char nombre[50];
  int existe;
  int tamanio;
  char contenido[1500];
  int nPaquete;
  int ACK;

}Paquete;

main(int argc, char *argv[]) 
{ 
  FILE *ficherols, *ficherosend;
  int sockfd; 
  int port; /* El puerto a utilizar */  
  int cuentafichero, sizecontenido, sizefichero;
  Paquete datos;/*estrucuta donde se enviaran los datos*/
// ///////////////7inicializo la estructura
//   datos.nPaquete=0;
//   strcpy(datos.contenido," ");
//   datos.ACK=0;
//   datos.existe=0;
//   strcpy(datos.nombre,"o");
//   datos.tamanio=0;
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
	while(1){
    if ((numbytes=recvfrom(sockfd, buf, MAXBUFLEN, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) 
    {; }
else{break;}
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
        while(!feof(ficherols))
        {

        fgets(buffer, MAXBUFLEN, ficherols);

        if ((numbytes=sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) 
          {
          perror("Error al enviar mensaje con: sendto"); 
          exit(1);
          }
          printf("%s",buffer );
          memset( buffer , 0 , sizeof(buffer) );
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
		while(1){
          if ((numbytes=recvfrom(sockfd,&datos,sizeof(datos), 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) 
            {; }
		else{break;}	
		}










///////// si existe el archivo
           if( ficherosend=fopen(datos.nombre,"rb+")){
            printf("enviando...........%s\n",datos.nombre);
              memset(&datos.existe, 0, sizeof(datos.existe));
              datos.existe=1;
              fseek(ficherosend,0,SEEK_END);
              datos.tamanio= (int) ftell(ficherosend);
              sizefichero=datos.tamanio;
              printf("%d\n",datos.tamanio);
              fseek(ficherosend,0,SEEK_SET);
              sizecontenido=sizeof(datos.contenido);
              cuentafichero=sizecontenido;
///////////////envio existe el ficherotamaño/////
              if ((numbytes=sendto(sockfd,&datos,sizeof(datos), 0, (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) 
                  {
                    perror("Error al enviar mensaje con: sendto"); 
                    exit(1);
                  }
                  printf("[%d] Bytes enviados: %d\n",numbytes,datos.tamanio );

///////////////envio archivo
                      int posicionfseek;
                      datos.nPaquete = 1;
                      fcntl (sockfd, F_SETFL, O_NONBLOCK);/*se determina que el socket no es bloqueante*/
                 


                  while(1){
                           while(1){
                                    if (cuentafichero < sizefichero){
                                                posicionfseek=(datos.nPaquete * sizeof(datos.contenido))-sizeof(datos.contenido);
                                                //printf("{%d}\n",posicionfseek );
                                                
                                                fseek(ficherosend,posicionfseek,SEEK_SET);
                                                // sleep(1);
                                                  id=1;
                                                  int error;
                                                  int *salida;
                                                  timeout=0;
                                                  pthread_t h;
                                                  datos.ACK=0;
                                                  //sleep(1);
                                                  comienzo = clock();
                                                  error=pthread_create (&h, NULL, codigo_del_hilo,&id);
                                                fread(datos.contenido,sizeof(datos.contenido),1,ficherosend);
                                                //memset( datos.contenido , 0 , sizeof(datos.contenido ) );
                                                printf("Enviando...[%d] pack %d-->[%d]",cuentafichero,datos.nPaquete,datos.tamanio);
                                                if ((numbytes=sendto(sockfd,&datos,sizeof(datos), 0, (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) 
                                                  {
                                                    perror("Error al enviar mensaje con: sendto"); 
                                                    exit(1);
                                                   }      
                                                   sleep(1);                                      

                                                  while(1){


                                                          if ((numbytes=recvfrom(sockfd,&datos,sizeof(datos), 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) 
                                                          {; }
                                                        if (timeout == 1)
                                                        {
                                                          printf("reenviando %d",datos.nPaquete);
                                                          error=pthread_join (h, (void **) &salida);
                                                           break;
                                                        }
                                                          else{
                                                            timeout=1;
                                                            error=pthread_join (h, (void **) &salida);
                                                          break;
                                                              }
                                                          }
                                    }
                                      if (datos.ACK == 1){
                                            datos.nPaquete +=1;
                                            cuentafichero +=sizecontenido;             
                                            break;
                                            }
                                            //else {break;}






                       }
                        if(cuentafichero > datos.tamanio){
                          cuentafichero = cuentafichero -sizecontenido;
                          sizefichero = sizefichero - cuentafichero;
                          fread(datos.contenido,sizefichero,1,ficherosend);
                          printf("Enviando...[%d] pack %d-->[%d]\n",sizefichero,datos.nPaquete,datos.tamanio);
                          printf("termine de Enviar \n");
                          if ((numbytes=sendto(sockfd,&datos,sizeof(datos), 0, (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) 
                        {
                          perror("Error al enviar mensaje con: sendto"); 
                          exit(1);
                        }
                          fclose(ficherosend);
                          break;
                        }  
                  }


            break;
           }











           //manejo si no tengo el archivo 
           else {
                printf("No tengo archivo\n");
                datos.existe=0;
                if ((numbytes=sendto(sockfd,&datos,sizeof(datos), 0, (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) 
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
