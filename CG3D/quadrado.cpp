#include "Quadrado.h"
#include <GL/glut.h>

Quadrado::Quadrado(float x, float y, float z) : Xv1(x), Yv1(y), Zv1(z), angle(0.0) {}//contrutor serve para inicializar as variaveis com os valores passados nos parametros! 

void Quadrado::desenha() const { // metodo desenho

  GLfloat vertices[][3] = {
    {Xv1, Yv1,  Zv1},          // Vértice 0 (canto inferior esquerdo frontal)
    {Xv1+2.0, Yv1,  Zv1},      // Vértice 1 (canto inferior direito frontal)
    {Xv1+2.0, Yv1+2.0,  Zv1},  // Vértice 2 (canto superior direito frontal)
    {Xv1, Yv1+2.0,  Zv1},      // Vértice 3 (canto superior esquerdo frontal)
    {Xv1, Yv1, Zv1-2.0},       // Vértice 4 (canto inferior esquerdo traseiro)
    {Xv1+2.0, Yv1, Zv1-2.0},   // Vértice 5 (canto inferior direito traseiro)
    {Xv1+2.0, Yv1+2.0, Zv1-2.0},// Vértice 6 (canto superior direito traseiro)
    {Xv1, Yv1+2.0, Zv1-2.0}    // Vértice 7 (canto superior esquerdo traseiro)
};
     //a matriz indica quais sao os vertices necessarios para desenha o cubo, para desenha a face frontal
    int faces[][4] = { //Cada face é definida por quatro índices de vértices
        {0, 1, 2, 3},  // Face frontal
        {4, 5, 6, 7},  // Face traseira
        {0, 3, 7, 4},  // Lado esquerdo
        {1, 2, 6, 5},  // Lado direito
        {0, 1, 5, 4},  // Base
        {2, 3, 7, 6}   // Topo
    };
    GLfloat coordenadasTextura[][2] = {
        {0.0, 0.0}, //Canto Inferior Esquerdo 
        {1.0, 0.0}, //Canto Inferior Direito
        {1.0, 1.0}, //Canto Superior Direito 
        {0.0, 1.0} //Canto Superior Esquerdo
    };

    glBegin(GL_QUADS);
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            glTexCoord2fv(coordenadasTextura[j]);
            glVertex3fv(vertices[faces[i][j]]);
        }
    }
    glEnd();

}

void Quadrado::mover(float novoX, float novoY) { //ele recebe 2 valores novos e atribui ao vertice do quadrado são relacionado ao ponto do mouse 
    Xv1 = novoX;
    Yv1 = novoY;
}

void Quadrado::girar(float angulo) { // função que quando eu clico com o botao direito do mouse, ele usa a matriz de rotação 
    glRotatef(angle, 0.0, 0.0, 1.0);// gira em torno do eixo Z 
}
