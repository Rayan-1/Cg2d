#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include "Quadrado.h"
using namespace std;

 //É uma boa prática criar uma função para agrupar configurações iniciais do OpenGL para o desenho a ser feito
void inicio() {
    glClearColor(1.0, 0.0, 0.0, 0.0); //indica qual cor será usada para limpar o frame buffer (normalmente usa uma cor de background)
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
}
bool mousePress = false;
float Tam_Janela_init = 500;

//Coordenadas de inicialização do quadrado 1
float Xv1 = -0.5;
float Yv1 = -0.5;
//Coordenadas de inicialização do quadrado 2
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
            // Quando o botão esquerdo do mouse é pressionado, inicia o movimento
            cout << "Estou entrando akiii " << endl;
            mousePress = true;
        }
        else if (state == GLUT_UP) {
            // Quando o botão esquerdo do mouse é liberado, para o movimento
            cout << "saindo quando paro de clicar" << endl;
            mousePress = false;
        }
    }
    if (result_trans_x >= Xv1 && result_trans_x <= (Xv1 + 1.0f) && result_trans_y >= Yv1 && result_trans_y <= (Yv1 + 1.0f)) {// verifica se o clique está dentro do quadrado, so entao ele realiza a movimentação 
        if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON) { // Trata apenas do evento de clique do mouse, pois sem essa instrução de condição, ela trataria dois eventos...

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

        if (result_trans_x >= Xv1 && result_trans_x <= (Xv1 + 1.0f) && result_trans_y >= Yv1 && result_trans_y <= (Yv1 + 1.0f)) {// verifica se o clique está dentro do quadrado, so entao ele realiza a movimentação 
            Xv1 = result_trans_x - 0.5f; // aqui é uma gambiarra para pegar o meio do quadrado 
            Yv1 = result_trans_y - 0.5f; // aqui é uma gambiarra para pegar o meio do quadrado 

            PecaQuadrado.mover(Xv1, Yv1);
           
            glutPostRedisplay();

        }
        if (result_trans_x >= Xv2 && result_trans_x <= (Xv2 + 1.0f) && result_trans_y >= Yv2 && result_trans_y <= (Yv2 + 1.0f)) {
            Xv2 = result_trans_x - 0.5f; // aqui é uma gambiarra para pegar o meio do quadrado 
            Yv2 = result_trans_y - 0.5f; // aqui é uma gambiarra para pegar o meio do quadrado 

            PecaQuadrado2.mover(Xv2, Yv2);


            glutPostRedisplay();
        }
        
    }
}

//Função que será usada para desenhar o conteúdo no Frame Buffer
void desenha() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //sempre antes de desenhar qualquer coisa, deve-se limpar o Frame Buffer
    glLoadIdentity();
    glOrtho(-3, 5, -3, 5, -5, 7);// tamanho de visualização da tela é de 6 vai de (-1 até 5) 
    gluLookAt(-2.0, 2.5, 5.0,  // Posição da câmera
              0.0, 0.0, 0.0,  // Ponto de destino
              0.0, 1.0, 0.0);  // Vetor de orientação para cima
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

    


    glFlush();  //Todas as instruções anteriores apenas indicaram o que deve ser feito. Essa é a ordem pra GPU redesenhar com as informações enviadas
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                         //inicializar a biblioteca GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  //configurações do frame buffer (um frame buffer com três canais de cor: RGB)
    glutInitWindowPosition(200, 200);              //posição do canto superior esquerdo da janela com relação a tela
    glutInitWindowSize(Tam_Janela_init, Tam_Janela_init);                  //resolução da janela (framebuffer)
    glutCreateWindow("Jogo Pluzze version 1.0");             //cria a janela (a string aparece na barra de título da janela)

    inicio();

    glutDisplayFunc(desenha);   //indica pra GLUT que a função 'desenha' será chamada para atualizar o frame buffer
    glutMouseFunc(mouseClick);
    glutMotionFunc(movimentoMouse);
    

    glutMainLoop();             //mantém um laço interno usando threads para que a janela permaneça aberta


    return 0;
}