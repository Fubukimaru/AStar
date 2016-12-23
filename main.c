#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    unsigned int x, y;
} Cell;



typedef struct {
    Cell* queue;
    int first, last;
    unsigned int size;
} Queue;



//Vars
unsigned int* map;
unsigned int width, heigth;

Queue q; //We need a queue as A* is a Breadth first search algorithm.

//Map operators
void readMap (const char* file_name)
{
  FILE* file = fopen (file_name, "r");
  unsigned int i, j;

  //Scan map size  
  fscanf(file, "%u", &heigth);
  fscanf(file, "%u", &width);
  printf("Matrix: %u X %u \n", width, heigth);    

  map = calloc(width*heigth,sizeof(unsigned int));//Reserve memory of mapsize  

  for (i = 0; i < heigth; ++i) { //Read map
      for (j = 0; j < width; ++j) {
          fscanf (file, "%u", &map[i*width + j]);
      }
  }

  fclose (file);
}

void printMap() {
    unsigned int i, j;
    for (i = 0; i < heigth; ++i) {
        for (j = 0; j < width; ++j) {
            printf("%u, ", map[i*width + j]);
        }
        printf("\n");
    }
}


//Queue operators
void initQueue(){
    q.first=-1;
    q.last=-1;
    q.size = width*heigth;
    q.queue = calloc(q.size,sizeof(unsigned int)); //WxH at most. TODO: (Check upper bound)
}

bool isQueueEmpty(){
    return (q.first < 0);
}

void addElement(Cell c) {
    printf("Adding (%u,%u) in position %u. Queue size: %u. \n", c.x, c.y, q.last % q.size, q.size);
    q.last = (q.last + 1) % q.size;
    if (!isQueueEmpty()) q.queue[q.last] = c; //Add next to the current last one. Wrap if we arrive to the end.
    else {
        q.first = q.last = 0;
        q.queue[0] = c;
    }
}

Cell getNext() {
    //Pre: Queue is not empty. We do not cover dumb programmers that don't check the queue!
    unsigned int elem = q.first;

    if(q.first == q.last) {
        q.first = -1;
        //q.last = -1; we don't care when adding elements. [Check isQueueEmpty function and addElement when empty]
    }
    else ++q.first % q.size; //Next and wrap


    return q.queue[elem];
}

void printQueue() {
    unsigned int i, j;
    for (i = q.first; i <= q.last; ++i) {
       printf("(%u, %u) <- ", q.queue[i].x, q.queue[i].y);
    }
    printf("end\n");
}



//MAIN


void testMap() {
    readMap("/home/agutierrez/workspace/Astar/mat.txt");
    printMap();
}

void testQueue() {
    Cell tmp;
    initQueue();

    printf("Is queue empty? %u \n", isQueueEmpty());
    tmp.x = 1;
    tmp.y = 1;
    addElement(tmp);

    printf("Is queue empty? %u \n", isQueueEmpty());
    tmp.x = 1;
    tmp.y = 2;
    addElement(tmp);

    printf("Is queue empty? %u \n", isQueueEmpty());
    tmp.x = 1;
    tmp.y = 3;
    addElement(tmp);

    printQueue();
    tmp = getNext();
    printf("Got element (%u,%u) from queue.\n", tmp.x, tmp.y);


    printQueue();

}

int main(void)
{        
    printf("Hello World!\n");

    testMap();
    testQueue();

    return 0;
}


