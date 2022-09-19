#include <assert.h>
#include <limits.h>
#include <pthread.h>
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

typedef struct thread_data { //Informaçoes para quando for rodar benchmark
  unsigned long nb_add;
  unsigned long nb_remove;
  unsigned long nb_contains;
  unsigned long nb_found;
  unsigned short seed[3];
  int diff;
  int range;
  int update;
  int alternate;
} thread_data_t;

static volatile int stop;
static unsigned short main_seed[3];

static inline void rand_init(unsigned short *seed)
{
  seed[0] = (unsigned short)rand();
  seed[1] = (unsigned short)rand();
  seed[2] = (unsigned short)rand();
}

static inline int rand_range(int n, unsigned short *seed)
{
  /* Return a random number in range [0;n) */
  int v = (int)(erand48(seed) * n);
  assert (v >= 0 && v < n);
  return v;
} //Retorna valor aleatório

void printVetor(set *array){
  unsigned int i=0;
  printf("\nVetor:\n");
  while(i<array->size){
    printf("%d\n",array->list[i]);
    i++;
  }
} //Printa o vetor inteiro

/*
static void *test(void *data) //Função com benchmark, ainda não adaptada para o código
{
  int op, val, last = -1;
  thread_data_t *d = (thread_data_t *)data;
  FILE *arqHASH = fopen("Hash.bin", "r+b");
  printf("Range: %u\n",d->range);
  while (stop == 0) {
    op = rand_range(100, d->seed);
    if (op < d->update) {
      if (d->alternate) {
        // Alternate insertions and removals
        if (last < 0) {
          // Add random value
          val = rand_range(d->range, d->seed) + 1;
          if (add(val, arqHASH)) {
            d->diff++;
            last = val;
          }
          d->nb_add++;
        } else {
          // Remove last value
          if (delete(last, arqHASH))
            d->diff--;
          d->nb_remove++;
          last = -1;
        }
      } else {
        // Randomly perform insertions and removals
        val = rand_range(d->range, d->seed) + 1;
        if ((op & 0x01) == 0) {
          // Add random value
          if (add(val, arqHASH))
            d->diff++;
          d->nb_add++;
        } else {
          // Remove random value
          if (delete(val, arqHASH))
            d->diff--;
          d->nb_remove++;
        }
      }
    } else {
      // Look for random value
      val = rand_range(d->range, d->seed) + 1;
      if (search(val, arqHASH))
        d->nb_found++;
      d->nb_contains++;
    }
  }
  fclose(arqHASH);
  return NULL;
}
*/

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
  thread_data_t *data;
  pthread_t threads;
  rand_init(main_seed);
  if ((data = (thread_data_t *)malloc(1 * sizeof(thread_data_t))) == NULL) {
      perror("malloc");
      exit(1);
  }
  if ((array = (set *)malloc(1 * sizeof(set))) == NULL) {
      perror("malloc");
      exit(1);
  }
  srand((int)time(NULL));
  #ifndef DEBUG
  if(argc<3){
			printf("\t\tNo arguments\n \t\tHOW TO USE:\n ./program <Size array> <Num. Updates>\nSetting values 10 50\n");
			sleep(2);
			data[0].range = 10;
      data[0].update = 50;  //Para o benchmark
	}
  else{
    data[0].range = atoi(argv[1]);
    data[0].update = atoi(argv[2]); //Para o benchmark
  }
  #else
  data[0].range = 10;
  data[0].update = 50;
  #endif
  data[0].alternate = 1; //Para o benchmark
  int duration = 10; //secs
  rand_init(data[0].seed);
  if ((array->list=(int *)calloc(data[0].range, sizeof(int))) == NULL) { //Aloca o vetor
    perror("calloc");
    exit(1);
  }
  #ifdef DEBUG
  printf("range: %d\n",data[0].range);
  #endif
  int i=0;
  while(i<(data[0].range/2)){
      if (add(rand_range(data[0].range, main_seed), array, data->range)) { //Preenche a lista
          i++;
      }
  }
  #ifdef DEBUG
  printf("tamanho vetor: %d\n",array->size);
  printf("i: %d\n",i);
  printf("vetor0: %d\n",array->list[0]);
  printf("vetor1: %d\n",array->list[1]);
  printVetor(array);
  printf("Insertion Sort:\n");
  insertionsort(array);
  printVetor(array);
  #endif
  free(data);
  free(array->list);
  free(array);
  exit(0);

}
