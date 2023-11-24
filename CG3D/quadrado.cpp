#include "Quadrado.h"
#include <GL/glut.h>

Quadrado::Quadrado(float x, float y) : Xv1(x), Yv1(y), angle(0.0) {}//contrutor serve para inicializar as variaveis com os valores passados nos parametros! 

void Quadrado::desenha() const { // metodo desenho
    
    glColor3f(1.0, 0.0, 1.0); //coloca as cores dos vertices no padrão RGB para os proximos pontos
    glBegin(GL_QUADS); // utilizando a primitiva do quadrado
    glVertex2f(Xv1, Yv1);  //instrução que envia as coordenadas dos vértices do triângulo
    glVertex2f(Xv1, Yv1 + 1.0f);
    glColor3f(1.0, 1.0, 0.0); // muda os pontos para outra cor 
    glVertex2f(Xv1 + 1.0f, Yv1 + 1.0f);
    glVertex2f(Xv1 + 1.0f, Yv1);
    glEnd(); // finaliza o desenho 

}

void Quadrado::mover(float novoX, float novoY) { //ele recebe 2 valores novos e atribui ao vertice do quadrado são relacionado ao ponto do mouse 
    Xv1 = novoX;
    Yv1 = novoY;
}

void Quadrado::girar(float angulo) { // função que quando eu clico com o botao direito do mouse, ele usa a matriz de rotação 
    glRotatef(angle, 0.0, 0.0, 1.0);// gira em torno do eixo Z 
}