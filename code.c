#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include <malloc.h>



typedef struct{ 
  unsigned int size; //Tamanho da lista
  int *list; //Lista
}set;



void printVetor(set *array){
  unsigned int i=0;
  printf("\nVetor:\n");
  while(i<array->size){
    printf("%d\n",array->list[i]);
    i++;
  }
} //Printa o vetor inteiro


int add(int value, set *array, int range){ //Testa se está cheio e adiciona na lista
  if(array->size<range){
    array->list[array->size]=value;
    array->size=array->size+1;
    return 1;
  }
  else{
    return 0;
  }
}

int insertionsort(set *array){  //Insertion Sort
  unsigned int tamanho=array->size;
  unsigned int trocas=0;
  unsigned int aux=0, i=0, j=0;
  for(i=0;i<tamanho;i++){
    for(j=0;j<tamanho-1;j++){
      if(array->list[j]>array->list[j+1]){
        aux=array->list[j];
        array->list[j]=array->list[j+1];
        array->list[j+1]=aux;
        trocas=trocas+1;
      }
    }
  }
  return(trocas); //Total de trocas
}

int selectionsort(set *array){ //Selection Sort
	unsigned int tamanho=array->size;
	unsigned int trocas=0;
	unsigned int aux=0,aux2=0, i=0, j=0;
	for(i=0;i<tamanho-1;i++){
		aux=i;
		for (j=i+1;j<tamanho;j++){
			if(array->list[j]<array->list[aux]){
				aux=j;
			}
		}
		if(aux!=i){
			aux2=array->list[aux];
			array->list[aux]=array->list[i];
			array->list[i]=aux2;
			trocas=trocas+1;
		}
	}
	return(trocas);
	}
int intercala(set *array, int p, int q, int r){ //Intercala do Merge sort
	set *array2;
	unsigned int i=0,j=0,k=0;
	unsigned int trocas=0;
	if ((array2 = (set *)malloc(1 * sizeof(set))) == NULL) {
	     perror("malloc");
	     exit(1);
	 }
	 if ((array2->list=(int *)calloc(256, sizeof(int))) == NULL) { //Aloca o vetor
	    perror("calloc");
	    exit(1);
	 }

	 for(i=p;i<=q;i++){
		array2->list[i]=array->list[i];
	   	array2->size=array2->size+1;
	}
	for(j=q+1;j<=r;j++){
		array2->list[r+q+1-j]=array->list[j];
	    	array2->size=array2->size+1;
	}
	i=p;
	j=r;
	
	for (k=p;k<=r;k++){
		trocas=trocas+1;//aki?
		if(array2->list[i]<=array2->list[j]){
			array->list[k]=array2->list[i];
			i=i+1;
		}
		else{
			array->list[k]=array2->list[j];
			j=j-1;
		}
	}
	  free(array2->list);
	  free(array2);
	  return(trocas);
}

int mergesort(set *array,int p, int r){ //Merge sort
	unsigned int q;
	unsigned int trocas=0;
	if (p<r){
		q=(p+r)/2;
		trocas=trocas+mergesort(array,p,q);
		trocas=trocas+mergesort(array,q+1,r);
		trocas=trocas+intercala(array,p,q,r);
	}
	return(trocas);
}

int delete(int value, set *array){ //Remove valor
  unsigned int tamanho=array->size;
  unsigned int aux=0, i=0;
  int aux2;
  while(aux<tamanho){
    if(value==array->list[aux]){
      array->list[aux]=0;
      for(i=aux;i<tamanho;i++){
        aux2=array->list[i];
        array->list[i]=array->list[i+1];
        array->list[i+1]=aux2;
        i++;
      }
      break;
    }
    aux++;
  }
  array->size=tamanho--;
}


int main(int argc, char* argv[]){
  set *array;
  int range=0;
  if ((array = (set *)malloc(1 * sizeof(set))) == NULL) {
      perror("malloc");
      exit(1);
  }
  srand((int)time(NULL));
  #ifndef DEBUG
  if(argc<2){
			printf("\t\tNo arguments\n \t\tHOW TO USE:\n ./program <Size array>\nSetting size 256\n");
			sleep(2);
			range = 256;
	}
  else{
    range = atoi(argv[1]);
  }
  #else
  range = 10;
  #endif
  int seed=42;
  srandom(seed);   // Initialization, should only be called once.
  if ((array->list=(int *)calloc(range, sizeof(int))) == NULL) { //Aloca o vetor
    perror("calloc");
    exit(1);
  }
  printf("range: %d\n",range);
  int i=0;
  while(i<(range/2)){
      if (add(random()%range, array, range)) { //Preenche a lista
          i++;
      }
  }
  #ifdef DEBUG
  printf("i: %d\n",i);
  printf("vetor0: %d\n",array->list[0]);
  printf("vetor1: %d\n",array->list[1]);
  printVetor(array);
  printf("Insertion Sort:\n");
  insertionsort(array);
  printVetor(array);
  #endif
  printf("tamanho vetor: %d\n",array->size);
  printf("Trocas: %d\n",insertionsort(array));
  free(array->list);
  free(array);
  exit(0);

}
