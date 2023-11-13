#include "Quadrado.h"
#include <GL/glut.h>

Quadrado::Quadrado(float x, float y) : Xv1(x), Yv1(y), angle(0.0) {}

void Quadrado::desenha() const {
    glColor3f(1.0, 0.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(Xv1, Yv1);  //instru��o que envia as coordenadas dos v�rtices do tri�ngulo
    glVertex2f(Xv1, Yv1 + 1.0f);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(Xv1 + 1.0f, Yv1 + 1.0f);
    glVertex2f(Xv1 + 1.0f, Yv1);
    glEnd();
    //glPopMatrix(); // Restaura a matriz salva
}

void Quadrado::mover(float novoX, float novoY) {
    Xv1 = novoX;
    Yv1 = novoY;
}

void Quadrado::girar(float angulo) {
    angle = angulo;
    glRotatef(angle, 0.0, 0.0, 1.0);
}