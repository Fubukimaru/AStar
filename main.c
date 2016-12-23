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


typedef struct {
    unsigned int* map;
    unsigned int width, heigth;
} Map;




FILE* outChannel;

//Map operators
void readMap (Map* map, const char* file_name)
{
  FILE* file = fopen (file_name, "r");
  unsigned int x,y;

  //Scan map size  
  fscanf(file, "%u", &map->heigth);
  fscanf(file, "%u", &map->width);
  printf("Matrix: %u X %u \n", map->width, map->heigth);

  map->map = calloc(map->width*map->heigth,sizeof(unsigned int));//Reserve memory of mapsize

  for (y = 0; y < map->heigth; ++y) { //Read map
      for (x = 0; x <map->width; ++x) {
          fscanf (file, "%u", &map->map[y*map->width + x]);
      }
  }

  fclose (file);
}

void printMap(const Map* map) {
    unsigned int x, y;
    for (y = 0; y < map->heigth; ++y) {
        for (x = 0; x < map->width; ++x) {            
            fprintf(outChannel,"%u, ",map->map[y*map->width + x]);
        }
         fprintf(outChannel,"\n");
    }
}

void deleteMap(Map* map) {
    free(map->map);
}


//Queue operators
void initQueue(Queue* q, const Map* map){    
    q->first=-1;
    q->last=-1;
    q->size = map->width*map->heigth;
    q->queue = calloc(q->size,sizeof(unsigned int)); //WxH at most. TODO: (Check upper bound)    
}

bool isQueueEmpty(const Queue *q){
    return (q->first < 0);
}

void addElement(Queue* q, const Cell c) {
    //TODO: Check if queue is full.
    printf("Adding (%u,%u) in position %u. Queue size: %u. \n", c.x, c.y, q->last % q->size, q->size);
    q->last = (q->last + 1) % q->size;
    if (!isQueueEmpty(q)) q->queue[q->last] = c; //Add next to the current last one. Wrap if we arrive to the end.
    else {
        q->first = q->last = 0;
        q->queue[0] = c;
    }
}

Cell getNext(Queue* q) {
    //Pre: Queue is not empty. We do not cover dumb programmers that don't check the queue!
    unsigned int elem = q->first;

    if(q->first == q->last) {
        q->first = -1;
        //q.last = -1; we don't care when adding elements. [Check isQueueEmpty function and addElement when empty]
    }
    else ++q->first % q->size; //Next and wrap


    return q->queue[elem];
}

void cleanQueue(Queue* q) {
    q->first = -1;
}

void printQueue(const Queue* q) {
    unsigned int i, j;
    for (i = q->first; i <= q->last; ++i) {
       fprintf(outChannel,"q[%u] = (%u, %u) <- ", i, q->queue[i].x, q->queue[i].y);
    }
    fprintf(outChannel,"end\n");
}

void deleteQueue(Queue* q) {
    free(q->queue);
}


//Search

void printBoolMap(bool* visitedMap, unsigned int heigth, unsigned int width) {
    unsigned int i, j;
    for (i = 0; i < heigth; ++i) {
        for (j = 0; j < width; ++j) {
            fprintf(outChannel,"%u, ", visitedMap[i*width + j]);
        }
        fprintf(outChannel,"\n");
    }
}




Cell BFS(const Map* map, Cell current, unsigned int target) {
    fprintf(outChannel, "Map dim: %u,%u", map->width,map->heigth);
    bool* visitedMap;
    visitedMap = calloc(map->width*map->heigth,sizeof(bool));//Reserve memory of mapsize



    Cell tmp;
    Queue q;
    initQueue(&q, map);

    //cleanQueue(q); //This can be substituted by a instantation of the queue.

    visitedMap[current.y*map->width + current.x] = true;

    fprintf(outChannel, "\n\n");
    printMap(map);
    fprintf(outChannel,"Starting BFS\n First: %u", map->map);
    fprintf(outChannel,"Starting BFS\n First: %u", map->map[current.y*map->width + current.x]);
    printMap(map);

    while (map->map[current.y*map->width + current.x] != target) {
        fprintf(outChannel,"Visiting (%u,%u) = %u\n", current.x, current.y, map->map[current.y*map->width + current.x]);
        printBoolMap(visitedMap,map->heigth, map->width);


        tmp.x = current.x-1;
        tmp.y = current.y;
        if (current.x > 0 && !visitedMap[tmp.y*map->width + tmp.x]) {
            fprintf(outChannel,"-Expanding (%u,%u)\n", tmp.x, tmp.y);
            addElement(&q, tmp);
            visitedMap[tmp.y*map->width + tmp.x] = true;
        }

        tmp.x = current.x+1;
        tmp.y = current.y;
        if (current.x+1 < map->width && !visitedMap[tmp.y*map->width + tmp.x]) {
            fprintf(outChannel,"-Expanding (%u,%u)\n", tmp.x, tmp.y);
            addElement(&q,tmp);
            visitedMap[tmp.y*map->width + tmp.x] = true;
        }

        tmp.x = current.x;
        tmp.y = current.y-1;
        if (current.y > 0 && !visitedMap[tmp.y*map->width + tmp.x]) {
            fprintf(outChannel,"-Expanding (%u,%u)\n", tmp.x, tmp.y);
            addElement(&q,tmp);
            visitedMap[tmp.y*map->width + tmp.x] = true;
        }

        tmp.x = current.x;
        tmp.y = current.y+1;
        if (current.y+1 < map->heigth && !visitedMap[tmp.y*map->width + tmp.x]) {
            fprintf(outChannel,"-Expanding (%u,%u)\n", tmp.x, tmp.y);
            addElement(&q,tmp);
            visitedMap[tmp.y*map->width + tmp.x] = true;
        }


        printQueue(&q);

        //Get new element
        current = getNext(&q);
        //fprintf(outChannel,"New current (%u,%u)\n", current.x,current.y);

        fprintf(outChannel,"\n\n");
    }
    //FOUND!

    free(visitedMap);
    //deleteQueue(&q); //If we do this, the code crashes... Double delete??
    return current;
}


//MAIN


void testMap(Map* map) {
    readMap(map, "/home/agutierrez/workspace/Astar/mat.txt");
    printMap(map);
}

void testQueue(const Map* map) {
    Cell tmp;
    Queue q;
    initQueue(&q, map);

    printf("Is queue empty? %u \n", isQueueEmpty(&q));
    tmp.x = 1;
    tmp.y = 1;
    addElement(&q,tmp);

    printf("Is queue empty? %u \n", isQueueEmpty(&q));
    tmp.x = 1;
    tmp.y = 2;
    addElement(&q,tmp);

    printf("Is queue empty? %u \n", isQueueEmpty(&q));
    tmp.x = 1;
    tmp.y = 3;
    addElement(&q,tmp);

    printQueue(&q);
    tmp = getNext(&q);
    printf("Got element (%u,%u) from queue.\n", tmp.x, tmp.y);


    printQueue(&q);
    deleteQueue(&q);

}


void testBFS(const Map* map) {
    Cell c;
    unsigned int target = 8;

    c.x = 0;
    c.y = 0;


    map->map[0];
    printMap(map);

    c = BFS(map, c, target);

    printf("Target %u is in (%u,%u)", target, c.x, c.y);
}

int main(void)
{        
    printf("Hello World!\n");

    outChannel = stdout; //Setting channel for prints

    //testMap();
    //testQueue();

    Map map;

    readMap(&map, "/home/agutierrez/workspace/Astar/mat.txt");
    printMap(&map);


    testBFS(&map);

    deleteMap(&map);

    return 0;
}


