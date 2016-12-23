#include <stdio.h>

unsigned int* map;

int main(void)
{        
    printf("Hello World!\n");

    readMap("/home/agutierrez/workspace/Astar/mat.txt");

    return 0;
}


void readMap (const char* file_name)
{
  FILE* file = fopen (file_name, "r");
  unsigned int i, j, width, heigth;

  fscanf(file, "%u", &heigth);
  fscanf(file, "%u", &width);
  printf("Matrix: %u X %u \n", width, heigth);
  printf("-----------------------------------\n");

  map = calloc(width*heigth,sizeof(unsigned int));

  for (i = 0; i < heigth; ++i) {
      for (j = 0; j < width; ++j) {
          fscanf (file, "%u", &map[i*width + j]);
      }
  }


  for (i = 0; i < heigth; ++i) {
      for (j = 0; j < width; ++j) {
          printf("%u, ", map[i*width + j]);
      }
      printf("\n");
  }


  printf("-----------------------------------\n");
  fclose (file);
}
