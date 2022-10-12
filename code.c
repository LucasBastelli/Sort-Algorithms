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

#ifdef INSERTIONSORT
unsigned long int sort(set *array){  //Insertion Sort
  unsigned int tamanho=array->size;
  unsigned long int trocas=0, posicoes=0;
  unsigned int aux=0, i=0, j=0;
  for(i=0;i<tamanho;i++){
    for(j=0;j<tamanho-1;j++){
      if(array->list[j]>array->list[j+1]){
        aux=array->list[j];
        array->list[j]=array->list[j+1];
        array->list[j+1]=aux;
        trocas=trocas+1;
      }
      posicoes=posicoes+1;
    }
  }
  return(trocas); //Total de trocas
}
#endif

#ifdef SELECTIONSORT
unsigned long int sort(set *array){ //Selection Sort
	unsigned int tamanho=array->size;
	unsigned long int trocas=0, posicoes=0;
	unsigned int aux=0,aux2=0, i=0, j=0;
	for(i=0;i<tamanho-1;i++){
		aux=i;
		for (j=i+1;j<tamanho;j++){
			if(array->list[j]<array->list[aux]){
				aux=j;
        posicoes=posicoes+1;
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
#endif

#ifdef MERGESORT
unsigned long int intercala(set *array, int p, int q, int r){ //Intercala do Merge sort
	int i, j, k;
  int n1 = q - p + 1;
  int n2 = r - q;
  unsigned long int trocas = 0;
  /* create temp arrays */
  int left[n1], right[n2];

  /* Copy data to temp arrays L[] and R[] */
  for (i = 0; i < n1; i++)
      left[i] = array->list[p + i];
  for (j = 0; j < n2; j++)
      right[j] = array->list[q + 1 + j];

  i = 0;
  j = 0;
  k = p;
  while (i < n1 && j < n2) {
      if (left[i] <= right[j]) {
          array->list[k] = left[i];
          i++;
      }
      else {
          array->list[k] = right[j];
          j++;
      }
      k++;
  }
  while (i < n1) {
      array->list[k] = left[i];
      trocas=trocas+1;
      i++;
      k++;
  }
  while (j < n2) {
      array->list[k] = right[j];
      trocas=trocas+1;
      j++;
      k++;
  }
  return (trocas);
}

unsigned long int sort(set *array,int p, int r){ //Merge sort
	unsigned int q;
	unsigned long int trocas=0;
	if (p<r){
		q=p+(r-p)/2;
		trocas=trocas+sort(array,p,q);
		trocas=trocas+sort(array,q+1,r);
		trocas=trocas+intercala(array,p,q,r);
	}
	return(trocas);
}
#endif

#ifdef QUICKSORT
unsigned long int partition(int Esq, int Dir, int *i, int *j, set *array)
{ 
  int x, w;
  *i = Esq;
  *j = Dir;
  unsigned long int troca=0,posicoes=0;

  //pivo
  x = array->list[(*i+*j)/2];

  do
  { 
    while(x > array->list[*i]){
         posicoes=posicoes+1;
         (*i)++;
         }
    while(x < array->list[*j]){
        posicoes=posicoes+1;
        (*j)--;
        }
    if(*i <=*j)
    { 
      w = array->list[*i];
      array->list[*i] = array->list[*j];
      array->list[*j] = w;
      (*i)++;
      (*j)--;
      troca=troca+1;
    }
  } while (*i<=*j);
  return troca;
}


unsigned long int quicksort(int Esq, int Dir, set *v)
{ 
  int i,j;
  unsigned long int troca=0;
  troca=troca+partition(Esq, Dir, &i, &j, v);
  if (Esq < j)
     troca=troca+quicksort(Esq, j, v);
  if (i < Dir)
     troca=troca+quicksort(i, Dir, v);
  return troca;
}

unsigned long int sort(set *array)
{ 
  unsigned long int troca=0;
  troca = quicksort(0, array->size-1, array);
  return troca; 
}
#endif

#ifdef HEAPSORT
void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

unsigned long int heapify(set *array, int n, int i) {
  int largest = i;
  int left = 2 * i + 1;
  int right = 2 * i + 2;
  unsigned long int trocas=0;

  if (left < n && array->list[left] > array->list[largest])
    largest = left;

  if (right < n && array->list[right] > array->list[largest])
    largest = right;

  if (largest != i) {
    swap(&array->list[i], &array->list[largest]);
    trocas=trocas+1;
    trocas=trocas+heapify(array, n, largest);
  }
  return trocas;
}

unsigned long int sort(set *array) {
  unsigned long int trocas=0;
  for (int i = array->size / 2 - 1; i >= 0; i--)
    heapify(array, array->size, i);

  for (int i = array->size - 1; i >= 0; i--) {
    swap(&array->list[0], &array->list[i]);
    trocas=trocas+1;
    trocas=trocas+heapify(array, i, 0);
  }
  return trocas;
}
#endif

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
			range = 2*256;
	}
  else{
    range = 2*(atoi(argv[1]));
  }
  #else
  range = 10;
  #endif
  int seed=time(NULL);
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
  printf("Trocas: %lu\n",sort(array,0,array->size-1));
  printVetor(array);
  #endif
  printf("tamanho vetor: %d\n",array->size);
  #ifdef MERGESORT
  printf("Trocas: %lu\n",sort(array,0,array->size-1));
  #else
  printf("Trocas: %lu\n",sort(array));
  #endif
  free(array->list);
  free(array);
  exit(0);

}
