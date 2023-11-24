#include <GL/glut.h>
#include <SOIL/SOIL.h>  // Certifique-se de ter a biblioteca SOIL instalada
#include <iostream>
#include <cmath>
#include "Quadrado.h"
using namespace std;

GLuint texture;

 //É uma boa prática criar uma função para agrupar configurações iniciais do OpenGL para o desenho a ser feito
void inicio() {
     glClearColor(1.0, 1.0, 1.0, 1.0);  // Defina a cor de limpeza para branco ou a cor desejada
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);  // Não esqueça de habilitar a textura 2D
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

void carregarTextura() {
    texture = SOIL_load_OGL_texture(
        "textura.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );

    if (texture == 0) {
        cerr << "Erro ao carregar a textura." << endl;
        cerr << "Erro SOIL: " << SOIL_last_result() << endl;
        exit(EXIT_FAILURE);
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
}
void desenhaFundo() {
    // Adicione mensagens de log
    cout << "Desenhando fundo..." << endl;

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-3.0, -3.0, -5.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(5.0, -3.0, -5.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(5.0, 5.0, -5.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-3.0, 5.0, -5.0);
    glEnd();
}


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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(-2.0, 2.5, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    desenhaFundo();  // Desenha o fundo antes dos quadrados

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
    glutSwapBuffers();  // Troca os buffers (double-buffering)
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                         
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  // Adicione GLUT_DEPTH para habilitar o teste de profundidade
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(Tam_Janela_init, Tam_Janela_init);
    glutCreateWindow("Jogo Puzzle version 1.0");

    inicio();
    carregarTextura();

    glutDisplayFunc(desenha);
    glutMouseFunc(mouseClick);
    glutMotionFunc(movimentoMouse);

    glutMainLoop();

    return 0;
}
