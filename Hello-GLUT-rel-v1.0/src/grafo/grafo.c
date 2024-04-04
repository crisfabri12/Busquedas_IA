#include "Grafo.h"
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <limits.h> 

#define INF INT_MAX
#define MAX_ITERATIONS 10000 

#define VISITED 1

#define WINDOW_WIDTH 210
#define WINDOW_HEIGHT 210

#define OFFSET_X (WINDOW_WIDTH - GRAFO_SIZE) / 2
#define OFFSET_Y (WINDOW_HEIGHT - GRAFO_SIZE) / 2

void initGraph(Grafo *g) {
    int i, j;
    srand(time(NULL));
    for (i = 0; i < GRAFO_SIZE; i += STEP) {
        for (j = 0; j < GRAFO_SIZE; j += STEP) {
            g->puntos[i / STEP][j / STEP].x = i + OFFSET_X;
            g->puntos[i / STEP][j / STEP].y = j + OFFSET_Y;
            g->puntos[i / STEP][j / STEP].deleted = (rand() / (float)RAND_MAX) < PERCENT_DELETED;
        }
    }
}

void drawLine(int x1, int y1, int x2, int y2, int visited) {
    if (visited) {
        glColor3f(1.0, 0.0, 0.0);
    } else {
        glColor3f(0.0, 0.0, 1.0);
    }
    glBegin(GL_LINES);
    glVertex2i(x1, y1);
    glVertex2i(x2, y2);
    glEnd();
}

void drawGraph(Grafo *g) {
    int i, j;
    for (i = 0; i < GRAFO_SIZE / STEP; i++) {
        for (j = 0; j < GRAFO_SIZE / STEP; j++) {
            if (!g->puntos[i][j].deleted) {
                int x = g->puntos[i][j].x;
                int y = g->puntos[i][j].y;
                if (i + 1 < GRAFO_SIZE / STEP && !g->puntos[i + 1][j].deleted) {
                    drawLine(x, y, g->puntos[i + 1][j].x, g->puntos[i + 1][j].y, g->visitedEdges[i][j][0]);
                }
                if (j + 1 < GRAFO_SIZE / STEP && !g->puntos[i][j + 1].deleted) {
                    drawLine(x, y, g->puntos[i][j + 1].x, g->puntos[i][j + 1].y, g->visitedEdges[i][j][2]);
                }
                if (i + 1 < GRAFO_SIZE / STEP && j + 1 < GRAFO_SIZE / STEP && !g->puntos[i + 1][j + 1].deleted) {
                    drawLine(x, y, g->puntos[i + 1][j + 1].x, g->puntos[i + 1][j + 1].y, g->visitedEdges[i][j][4]);
                }
                if (i + 1 < GRAFO_SIZE / STEP && j - 1 >= 0 && !g->puntos[i + 1][j - 1].deleted) {
                    drawLine(x, y, g->puntos[i + 1][j - 1].x, g->puntos[i + 1][j - 1].y, g->visitedEdges[i][j][6]);
                }
                glPointSize(5.0);
                glBegin(GL_POINTS);
                glVertex2i(g->puntos[i][j].x, g->puntos[i][j].y);
                glEnd();
            }
            
        }
    }
}

void drawPath(Punto p1, Punto p2) {
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex2i(p1.x, p1.y);
    glVertex2i(p2.x, p2.y);
    glEnd();
}

void depthFirstSearch(Grafo* g, int startX, int startY, int targetX, int targetY) {
    int dx[] = {1, -1, 0, 0, 1, 1, -1, -1};
    int dy[] = {0, 0, 1, -1, 1, -1, 1, -1};

    int stackSize = (GRAFO_SIZE / STEP) * (GRAFO_SIZE / STEP);
    int stack[stackSize][2];
    int visited[GRAFO_SIZE/STEP][GRAFO_SIZE/STEP];
    int top = 0;
    int x, y, i;

    for (i = 0; i < GRAFO_SIZE/STEP; i++) {
        for (int j = 0; j < GRAFO_SIZE/STEP; j++) {
            visited[i][j] = 0;
        }
    }

    stack[top][0] = startX;
    stack[top][1] = startY;
    top++;

    while (top > 0) {
        top--;
        x = stack[top][0];
        y = stack[top][1];

        if (x == targetX && y == targetY) {
            printf("El camino fue encontrado\n");
            return;
        }

        if (x >= 0 && y >= 0 && x < GRAFO_SIZE / STEP && y < GRAFO_SIZE / STEP && !g->puntos[x][y].deleted && visited[x][y] == 0) {
            visited[x][y] = 1;

            for (i = 0; i < 8; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];
                if (nx >= 0 && nx < GRAFO_SIZE / STEP && ny >= 0 && ny < GRAFO_SIZE / STEP && !g->puntos[nx][ny].deleted && top < stackSize) {
                    stack[top][0] = nx;
                    stack[top][1] = ny;
                    top++;
                }
            }

            // Marcar las aristas visitadas desde el punto actual
            for (i = 0; i < 8; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];
                if (nx >= 0 && nx < GRAFO_SIZE / STEP && ny >= 0 && ny < GRAFO_SIZE / STEP && !g->puntos[nx][ny].deleted) {
                    g->visitedEdges[x][y][i] = VISITED;
                }
            }
        }
    }

    printf("La búsqueda por profundidad ha finalizado\n");
}

void breadthFirstSearch(Grafo* g, int startX, int startY, int targetX, int targetY) {
    int dx[] = {1, -1, 0, 0, 1, 1, -1, -1};
    int dy[] = {0, 0, 1, -1, 1, -1, 1, -1};

    int queueSize = (GRAFO_SIZE / STEP) * (GRAFO_SIZE / STEP);
    int queue[queueSize][2];
    int visited[GRAFO_SIZE/STEP][GRAFO_SIZE/STEP];
    int front = 0;
    int rear = 0;
    int x, y, i;

    for (i = 0; i < GRAFO_SIZE/STEP; i++) {
        for (int j = 0; j < GRAFO_SIZE/STEP; j++) {
            visited[i][j] = 0;
        }
    }

    queue[rear][0] = startX;
    queue[rear][1] = startY;
    rear++;

    while (front != rear) {
        x = queue[front][0];
        y = queue[front][1];
        front++;

        if (x == targetX && y == targetY) {
            printf("El camino fue encontrado\n");
            return;
        }

        if (x >= 0 && y >= 0 && x < GRAFO_SIZE / STEP && y < GRAFO_SIZE / STEP && !g->puntos[x][y].deleted && visited[x][y] == 0) {
            visited[x][y] = 1;

            for (i = 0; i < 8; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];
                if (nx >= 0 && nx < GRAFO_SIZE / STEP && ny >= 0 && ny < GRAFO_SIZE / STEP && !g->puntos[nx][ny].deleted && rear < queueSize) {
                    queue[rear][0] = nx;
                    queue[rear][1] = ny;
                    rear++;
                }
            }

            // Marcar las aristas visitadas desde el punto actual
            for (i = 0; i < 8; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];
                if (nx >= 0 && nx < GRAFO_SIZE / STEP && ny >= 0 && ny < GRAFO_SIZE / STEP && !g->puntos[nx][ny].deleted) {
                    g->visitedEdges[x][y][i] = VISITED;
                }
            }
        }
    }

    printf("La búsqueda en amplitud ha finalizado\n");
}

float distanciaEuclidiana(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void encontrarMejorSucesor(Grafo *g, int x, int y, int targetX, int targetY, int *nextX, int *nextY) {
    int dx[] = {1, -1, 0, 0, 1, 1, -1, -1};
    int dy[] = {0, 0, 1, -1, 1, -1, 1, -1};
    float mejorDistancia = INF;

    for (int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (nx >= 0 && ny >= 0 && nx < GRAFO_SIZE / STEP && ny < GRAFO_SIZE / STEP &&
            !g->puntos[nx][ny].deleted) { // Verifica que el nodo no esté borrado
            float distancia = distanciaEuclidiana(nx, ny, targetX, targetY);
            if (distancia < mejorDistancia) {
                mejorDistancia = distancia;
                *nextX = nx;
                *nextY = ny;
            }
        }
    }
}

void hillClimbing(Grafo* g, int startX, int startY, int targetX, int targetY) {
    int currentX = startX;
    int currentY = startY;
    int nextX = currentX;
    int nextY = currentY;
    float currentDistance = distanciaEuclidiana(startX, startY, targetX, targetY); // Calcula la distancia inicial
    float nextDistance;
    int iterations = 0;
    int dx[] = {1, -1, 0, 0, 1, 1, -1, -1};
    int dy[] = {0, 0, 1, -1, 1, -1, 1, -1};

    while (currentDistance > 0 && iterations < MAX_ITERATIONS) {
        // Encuentra el mejor sucesor
        encontrarMejorSucesor(g, currentX, currentY, targetX, targetY, &nextX, &nextY);
        nextDistance = distanciaEuclidiana(nextX, nextY, targetX, targetY);

        if (nextDistance >= currentDistance) {
            printf("Local Optimum Found\n");
            break;
        }

        // Marcar todas las aristas hacia los nodos vecinos no borrados como visitadas
        for (int i = 0; i < 8; i++) {
            int neighborX = currentX + dx[i];
            int neighborY = currentY + dy[i];
            if (neighborX >= 0 && neighborX < GRAFO_SIZE / STEP && neighborY >= 0 && neighborY < GRAFO_SIZE / STEP &&
                !g->puntos[neighborX][neighborY].deleted) {
                g->visitedEdges[currentX][currentY][i] = VISITED;
            }
        }

        // Marcar todas las aristas desde los nodos vecinos no borrados hacia el nodo actual como visitadas
        for (int i = 0; i < 8; i++) {
            int neighborX = nextX + dx[i];
            int neighborY = nextY + dy[i];
            if (neighborX >= 0 && neighborX < GRAFO_SIZE / STEP && neighborY >= 0 && neighborY < GRAFO_SIZE / STEP &&
                !g->puntos[currentX][currentY].deleted) {
                g->visitedEdges[nextX][nextY][(i + 4) % 8] = VISITED;
            }
        }

        currentX = nextX;
        currentY = nextY;
        currentDistance = nextDistance;
        iterations++;
    }

    // Marcar todas las aristas desde el último nodo hacia el nodo objetivo no borrado como visitadas
    for (int i = 0; i < 8; i++) {
        int neighborX = currentX + dx[i];
        int neighborY = currentY + dy[i];
        if (neighborX >= 0 && neighborX < GRAFO_SIZE / STEP && neighborY >= 0 && neighborY < GRAFO_SIZE / STEP &&
            !g->puntos[currentX][currentY].deleted && neighborX == targetX && neighborY == targetY) {
            g->visitedEdges[currentX][currentY][i] = VISITED;
        }
    }
}

void encontrarMejorSucesorAStar(Grafo* g, int x, int y, int targetX, int targetY, Nodo* next) {
    int dx[] = {1, -1, 0, 0, 1, 1, -1, -1};
    int dy[] = {0, 0, 1, -1, 1, -1, 1, -1};

    float mejorCostoTotal = INF;

    for (int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (nx >= 0 && ny >= 0 && nx < GRAFO_SIZE / STEP && ny < GRAFO_SIZE / STEP &&
            !g->puntos[nx][ny].deleted) { // Verifica que el nodo no esté borrado
            float g = distanciaEuclidiana(x, y, nx, ny); // Costo acumulado desde el nodo inicial hasta el sucesor
            float h = distanciaEuclidiana(nx, ny, targetX, targetY); // Heurística estimada desde el sucesor hasta el nodo objetivo
            float f = g + h; // Costo total estimado

            if (f < mejorCostoTotal) {
                mejorCostoTotal = f;
                next->x = nx;
                next->y = ny;
                next->g = g;
                next->h = h;
                next->f = f;
            }
        }
    }
}

void aStar(Grafo* g, int startX, int startY, int targetX, int targetY) {
    Nodo start = {startX, startY, 0, distanciaEuclidiana(startX, startY, targetX, targetY), 0};
    Nodo current = start;
    Nodo next;

    int dx[] = {1, -1, 0, 0, 1, 1, -1, -1};
    int dy[] = {0, 0, 1, -1, 1, -1, 1, -1};

    while (current.x != targetX || current.y != targetY) {
        encontrarMejorSucesorAStar(g, current.x, current.y, targetX, targetY, &next);

        if (next.x == current.x && next.y == current.y) {
            printf("No se encontró camino\n");
            return;
        }

        // Verificar si el siguiente nodo está borrado
        if (g->puntos[next.x][next.y].deleted) {
            printf("El siguiente nodo está borrado, no se puede continuar la búsqueda\n");
            return;
        }

        // Marcar todas las aristas hacia los nodos vecinos no borrados como visitadas
        for (int i = 0; i < 8; i++) {
            int neighborX = current.x + dx[i];
            int neighborY = current.y + dy[i];
            if (neighborX >= 0 && neighborX < GRAFO_SIZE / STEP && neighborY >= 0 && neighborY < GRAFO_SIZE / STEP &&
                !g->puntos[current.x][current.y].deleted && !g->puntos[neighborX][neighborY].deleted) {
                g->visitedEdges[current.x][current.y][i] = VISITED;
            }
        }

        // Marcar todas las aristas desde los nodos vecinos no borrados hacia el nodo actual como visitadas
        for (int i = 0; i < 8; i++) {
            int neighborX = next.x + dx[i];
            int neighborY = next.y + dy[i];
            if (neighborX >= 0 && neighborX < GRAFO_SIZE / STEP && neighborY >= 0 && neighborY < GRAFO_SIZE / STEP &&
                !g->puntos[next.x][next.y].deleted && !g->puntos[current.x][current.y].deleted) {
                g->visitedEdges[next.x][next.y][(i + 4) % 8] = VISITED;
            }
        }

        current = next;
    }

    printf("El camino fue encontrado\n");
}

