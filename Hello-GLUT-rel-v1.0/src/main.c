#include <GL/glut.h>
#include ".\grafo\grafo.h"
#include <stdio.h>
#include <math.h>
#define SIZE 201
Grafo grafo;
int startX, startY;
int targetX, targetY;


void drawBigPoint(int x, int y, float r, float g, float b) {
    glColor3f(r, g, b);
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void resetEdgeColors(Grafo* g) {
    int i, j, k;
    for (i = 0; i < GRAFO_SIZE/STEP; i++) {
        for (j = 0; j < GRAFO_SIZE/STEP; j++) {
            for (k = 0; k < 8; k++) {
                g->visitedEdges[i][j][k] = 0;
            }
        }
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'b':
            resetEdgeColors(&grafo);
            breadthFirstSearch(&grafo, startX, startY, targetX, targetY);
            break;
        case 'd':
            resetEdgeColors(&grafo);
            depthFirstSearch(&grafo, startX, startY, targetX, targetY);
            break;
        case 'h':
            resetEdgeColors(&grafo);
            hillClimbing(&grafo, startX, startY, targetX, targetY);
            break;
        case 'a':
            resetEdgeColors(&grafo);
            aStar(&grafo, startX, startY, targetX, targetY);
            break;
        default:
            break;
    }
}

void drawText(const char* text, int x, int y) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(0.1, 0.1, 0.1); // Escalar el texto para que sea visible

    // Renderizar cada carácter del texto
    for (const char* c = text; *c != '\0'; c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }

    glPopMatrix();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Dibujar el grafo
    drawGraph(&grafo);

    // Dibujar el punto de inicio en verde
    drawBigPoint(grafo.puntos[startX][startY].x, grafo.puntos[startX][startY].y, 0.0f, 1.0f, 0.0f);

    // Dibujar el punto de destino en rojo
    drawBigPoint(grafo.puntos[targetX][targetY].x, grafo.puntos[targetX][targetY].y, 1.0f, 0.0f, 0.0f);

    // Dibujar el texto de la ruta y si se encontró o no
    glColor3f(0.0f, 0.0f, 0.0f); // Color del texto (negro)
    glRasterPos2i(10, 10); // Posición del texto
    char ruta[50]; // Aquí podrías almacenar la ruta
    sprintf(ruta, "Ruta: %s", "ruta_aqui"); // Aquí reemplaza "ruta_aqui" por la ruta real
    drawText(ruta, 10, 10);

    glFlush();
}

int main(int argc, char** argv) {


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Grafo");
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, SIZE, 0.0, SIZE);
    glutDisplayFunc(display);
    initGraph(&grafo);

    do {
        startX = rand() % (GRAFO_SIZE / STEP);
        startY = rand() % (GRAFO_SIZE / STEP);
    } while (grafo.puntos[startX][startY].deleted);

    // Definir un punto de destino aleatorio que no esté borrado
    do {
        targetX = rand() % (GRAFO_SIZE / STEP);
        targetY = rand() % (GRAFO_SIZE / STEP);
    } while (grafo.puntos[targetX][targetY].deleted);

    // Realizar la búsqueda por profundidad
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return EXIT_SUCCESS;
}


