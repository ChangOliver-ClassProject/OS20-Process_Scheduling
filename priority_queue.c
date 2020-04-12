#include <stdio.h>

int tree_array_size = 20;
int heap_size = 0;
const int INF = 100000;

void swap(int *a, int *b){
  int tmp = *a;
  *a = *b;
  *b = tmp;
}