#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include "Quadrado.h"
using namespace std;

 //� uma boa pr�tica criar uma fun��o para agrupar configura��es iniciais do OpenGL para o desenho a ser feito
void inicio() {
    glClearColor(1.0, 0.0, 0.0, 0.0); //indica qual cor ser� usada para limpar o frame buffer (normalmente usa uma cor de background)
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
}
bool mousePress = false;
float Tam_Janela_init = 500;

//Coordenadas de inicializa��o do quadrado 1
float Xv1 = -0.5;
float Yv1 = -0.5;
//Coordenadas de inicializa��o do quadrado 2
float Xv2 = -1.0;
float Yv2 = -1.0;
Quadrado PecaQuadrado(Xv1, Yv1); // instancia um objeto do tipo quadrado! 
Quadrado PecaQuadrado2(Xv2, Yv2);

float result_trans_x = 0;
float result_trans_y = 0;



void mouseClick(int button, int state, int x, int clique_mouse_y) {
    clique_mouse_y = Tam_Janela_init - clique_mouse_y; //inverter os valores da coordenada y do mouse, ja que ela pega os pixel contado de cima para baixo.
    result_trans_y = -1 + (clique_mouse_y * (6)) / Tam_Janela_init;//transformar coordenadas de pixel em coordendas cartesianas por regra de 3 
    result_trans_x = -1 + (x * (6)) / Tam_Janela_init;
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            // Quando o bot�o esquerdo do mouse � pressionado, inicia o movimento
            cout << "Estou entrando akiii " << endl;
            mousePress = true;
        }
        else if (state == GLUT_UP) {
            // Quando o bot�o esquerdo do mouse � liberado, para o movimento
            cout << "saindo quando paro de clicar" << endl;
            mousePress = false;
        }
    }
    if (result_trans_x >= Xv1 && result_trans_x <= (Xv1 + 1.0f) && result_trans_y >= Yv1 && result_trans_y <= (Yv1 + 1.0f)) {// verifica se o clique est� dentro do quadrado, so entao ele realiza a movimenta��o 
        if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON) { // Trata apenas do evento de clique do mouse, pois sem essa instru��o de condi��o, ela trataria dois eventos...

            PecaQuadrado.girar(90.0);

            glutPostRedisplay();

            cout << "x= " << result_trans_x << " y= " << result_trans_y << endl;
        }

    }
    
}
void movimentoMouse(int x, int y) {
    if (mousePress) {
        y = Tam_Janela_init - y; //inverter os valores da coordenada y do mouse, ja que ela pega os pixel contado de cima para baixo.
        result_trans_y = -1 + (y * (6)) / Tam_Janela_init;//transformar coordenadas de pixel em coordendas cartesianas por regra de 3 
        result_trans_x = -1 + (x * (6)) / Tam_Janela_init;

        if (result_trans_x >= Xv1 && result_trans_x <= (Xv1 + 1.0f) && result_trans_y >= Yv1 && result_trans_y <= (Yv1 + 1.0f)) {// verifica se o clique est� dentro do quadrado, so entao ele realiza a movimenta��o 
            Xv1 = result_trans_x - 0.5f; // aqui � uma gambiarra para pegar o meio do quadrado 
            Yv1 = result_trans_y - 0.5f; // aqui � uma gambiarra para pegar o meio do quadrado 

            PecaQuadrado.mover(Xv1, Yv1);
           
            glutPostRedisplay();

        }
        if (result_trans_x >= Xv2 && result_trans_x <= (Xv2 + 1.0f) && result_trans_y >= Yv2 && result_trans_y <= (Yv2 + 1.0f)) {
            Xv2 = result_trans_x - 0.5f; // aqui � uma gambiarra para pegar o meio do quadrado 
            Yv2 = result_trans_y - 0.5f; // aqui � uma gambiarra para pegar o meio do quadrado 

            PecaQuadrado2.mover(Xv2, Yv2);


            glutPostRedisplay();
        }
        
    }
}

//Fun��o que ser� usada para desenhar o conte�do no Frame Buffer
void desenha() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //sempre antes de desenhar qualquer coisa, deve-se limpar o Frame Buffer
    glLoadIdentity();
    glOrtho(-3, 5, -3, 5, -5, 7);// tamanho de visualiza��o da tela � de 6 vai de (-1 at� 5) 
    gluLookAt(-2.0, 2.5, 5.0,  // Posi��o da c�mera
              0.0, 0.0, 0.0,  // Ponto de destino
              0.0, 1.0, 0.0);  // Vetor de orienta��o para cima
    PecaQuadrado.desenha();
    PecaQuadrado2.desenha();
    glBegin(GL_QUADS);
    // Face frontal
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);

    // Face traseira
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glColor3f(1.0, 0.0, 1.0);
    // Face superior
    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(-1.0, 1.0, -1.0);

    // Face inferior
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glColor3f(0.0, 0.0, 1.0);
    // Face lateral esquerda
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f(-1.0, -1.0, -1.0);

    // Face lateral direita
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);

    glEnd();

    


    glFlush();  //Todas as instru��es anteriores apenas indicaram o que deve ser feito. Essa � a ordem pra GPU redesenhar com as informa��es enviadas
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                         //inicializar a biblioteca GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  //configura��es do frame buffer (um frame buffer com tr�s canais de cor: RGB)
    glutInitWindowPosition(200, 200);              //posi��o do canto superior esquerdo da janela com rela��o a tela
    glutInitWindowSize(Tam_Janela_init, Tam_Janela_init);                  //resolu��o da janela (framebuffer)
    glutCreateWindow("Jogo Pluzze version 1.0");             //cria a janela (a string aparece na barra de t�tulo da janela)

    inicio();

    glutDisplayFunc(desenha);   //indica pra GLUT que a fun��o 'desenha' ser� chamada para atualizar o frame buffer
    glutMouseFunc(mouseClick);
    glutMotionFunc(movimentoMouse);
    

    glutMainLoop();             //mant�m um la�o interno usando threads para que a janela permane�a aberta


    return 0;
}