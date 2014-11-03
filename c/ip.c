/* Inclusión de archivos */
#include <stdio.h>
#include <stdlib.h>
 
/* Función principal */
int main ()
{
   /* Impresión por pantalla y salida del programa*/
   printf("Dame dos numeros\n");
   int numero1, numero2;
   scanf("%d",&numero1); 
   scanf("%d",&numero2);
   printf("and de los numeros %d\nor de los numeros %d\n",red(numero1, numero2), bro(numero1, numero2));


   system ("echo h");
   return 0;
}


int red(int a, int b){
	return a&b;
}
int or(int a, int b){
	return a|b;
}
int bro(int a, int b){
	return (a&b)|(~a);
}

/*
   128   64 32 16 8  4  2  1
   1     1  1  1  1  0  0  0  248
   1     1  0  0  1  0  1  1  203
   -------------------------
   1     1  0  0  1  0  0  0  200

*/