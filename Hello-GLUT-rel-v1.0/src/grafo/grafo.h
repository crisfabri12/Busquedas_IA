#ifndef GRAFO_H
#define GRAFO_H

#include "Punto.h"

#define GRAFO_SIZE 200
#define STEP 10
#define PERCENT_DELETED 0.2 // 20% de los puntos serán borrados

typedef struct {
    Punto puntos[GRAFO_SIZE/STEP][GRAFO_SIZE/STEP];
    int visitedEdges[GRAFO_SIZE/STEP][GRAFO_SIZE/STEP][8]; // Nueva matriz para las aristas visitadas
} Grafo;

typedef struct {
    int x;
    int y;
    double f; // Costo total estimado
    double g; // Costo real desde el nodo inicial al nodo actual
    double h; // Heurística desde el nodo actual al nodo objetivo
} Nodo;

void initGraph(Grafo* g);
void drawGraph(Grafo* g);
void drawPath(Punto p1, Punto p2);
void depthFirstSearch(Grafo* g, int startX, int startY, int targetX, int targetY);
void breadthFirstSearch(Grafo* g, int startX, int startY, int targetX, int targetY);
void hillClimbing(Grafo* g, int startX, int startY, int targetX, int targetY);
void aStar(Grafo* g, int startX, int startY, int targetX, int targetY);
#endif
