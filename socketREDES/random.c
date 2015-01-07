#include<stdio.h>
#include<stdlib.h>

	void main(void){
				int puerto;				
				int puertos[1022];
				int i;
				for(i=0; i< 1023;i++){
					puertos[i]=i;
					}
					srand(getpid());
					puerto = rand()% (sizeof(puertos)/sizeof(puertos[0]));
					printf("%d\n\n",puerto);
			}
