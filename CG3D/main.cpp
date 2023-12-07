#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/ext.hpp>
#include <iostream>
#include <cmath>
#include "Quadrado.h"
#include <glm/glm.hpp>
#define STB_IMAGE_IMPLEMENTATION
#define FPS 30
#include "stb_image.h"

GLuint texcubo;
using namespace std;
float Tam_Janela_init = 500;
glm::vec4 camPos(-2.5, 2.5, 5, 1);    //posição inicial da câmera
glm::mat4 camRotacao = glm::rotate(glm::mat4(1), glm::radians(1.0f), glm::vec3(0, 1, 0)); //matriz de rotação para girar a câmera
bool camMove = true;                                                               //liga/desliga movimentação da câmera

//teste de iluminação
GLfloat luzAmbiente[] =  { 0.2, 0.2, 0.2, 1.0 };
GLfloat luzDifusa[] =    { 1.0, 1.0, 1.0, 1.0 }; // intensidade da luz em RGB -> 1,1,1,1 = branca 
GLfloat luzEspecular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat posicaoLuz[] =   { 0.0, 4.0, 4.0}; //posição da luz do plano 

GLfloat materialAmbiente[] = { 0.2, 0.2, 0.2, 1.0};
GLfloat materialDifuso[] =   { 0.8, 0.0, 0.0, 1.0};
GLfloat materialEspecular[] ={ 1.0, 1.0, 1.0, 1.0};
GLfloat materialBrilho = 0.0;

void teclado(unsigned char tecla, int x, int y) {
    if (tecla == ' ') camMove = !camMove; //tecla espaço alterna entre deixar ou não a câmera girar em torno do objeto
}
void timer(int v) {
    glutTimerFunc(1000 / FPS, timer, 0); //executada a quantidade FPS de vezes por segundo

    //se a opção camMove estiver verdadeira, gire a câmera
    if (camMove)
        camPos = camRotacao * camPos; //rotacionando a posição da câmera a matriz de rotação camRotacao
    //redesenhando a tela
    glutPostRedisplay();
}
//encerra teste de movimentação da camera 
GLuint carregarTextura(const char* caminhoDaTextura) {
    stbi_set_flip_vertically_on_load(true); //Inverte verticalmente a imagem ao carregar
    GLuint texturaID;
    glEnable(GL_TEXTURE_2D);             //habilitando o uso de texturas
    glGenTextures(1, &texturaID); 
    glBindTexture(GL_TEXTURE_2D, texturaID); // associa o identificador gerado a textura atual

    int largura, altura, nrCanais;
    unsigned char* dados = stbi_load(caminhoDaTextura, &largura, &altura, &nrCanais, 0);
    if (caminhoDaTextura == NULL) //verificação se o carregamento da imagem ocorreu com sucesso
        cout << "Erro, Imagem não carregada" << endl;
    else
        cout << "Imagem carregada com sucesso" << endl;


    if (dados) {

        GLenum formatoTextura = (nrCanais == 3) ? GL_RGB : GL_RGBA;


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //repetição espelhada no eixo S
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //repetição espelhada no eixo T

        //parâmetros da textura para filtragem quando tamanho de pixel e texel sejam diferentes (interpolação linear em ambos os casos)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //magnificação - quando um texel cobre vários pixels
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //minificação - quando muitos texels cabem em um único pixel

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

        glTexImage2D(GL_TEXTURE_2D, 0, formatoTextura, largura, altura, 0, formatoTextura, GL_UNSIGNED_BYTE, dados);
        //glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0); // desabilitando a textura
        stbi_image_free(dados);
        return texturaID;
    }
    else {
        // Lidar com erro ao carregar textura
        std::cerr << "Erro ao carregar a textura." << std::endl;
    }

    return texturaID;
}
void inicio() {
    
    glClearColor(0.0, 0.0, 0.0, 1.0); //indica qual cor será usada para limpar o frame buffer (normalmente usa uma cor de background)
    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_LIGHTING);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

    // Configurar material
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiente);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifuso);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialEspecular);
    glMaterialf(GL_FRONT, GL_SHININESS, materialBrilho);

    texcubo = carregarTextura("Gumball.jpg");
}


bool mousePress = false;


//Coordenadas de inicialização do cubo3D 1
float Xv1 = -1.0;
float Yv1 = -1.0;
float Zv1 = 1.0;

Quadrado PecaQuadrado(Xv1, Yv1, Zv1); // instancia um objeto do tipo cubo! 


float result_trans_x = 0;
float result_trans_y = 0;



void mouseClick(int button, int state, int x, int clique_mouse_y) {
    clique_mouse_y = Tam_Janela_init - clique_mouse_y; //inverter os valores da coordenada y do mouse, ja que ela pega os pixel contado de cima para baixo.
    result_trans_y = -3 + (clique_mouse_y * (10)) / Tam_Janela_init;//transformar coordenadas de pixel em coordendas cartesianas por regra de 3 
    result_trans_x = -3 + (x * (10)) / Tam_Janela_init;
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            // Quando o botão esquerdo do mouse é pressionado, inicia o movimento
            //cout << "Estou entrando akiii " << endl;
            mousePress = true;
        }
        else if (state == GLUT_UP) {
            // Quando o botão esquerdo do mouse é liberado, para o movimento
            //cout << "saindo quando paro de clicar" << endl;
            mousePress = false;
        }
    }
}
void movimentoMouse(int x, int y) {
    if (mousePress) {
        y = Tam_Janela_init - y; //inverter os valores da coordenada y do mouse, ja que ela pega os pixel contado de cima para baixo.
        result_trans_y = -3 + (y * (10)) / Tam_Janela_init;//transformar coordenadas de pixel em coordendas cartesianas por regra de 3 
        result_trans_x = -3 + (x * (10)) / Tam_Janela_init;

        if (result_trans_x >= Xv1 && result_trans_x <= (Xv1 + 2.0f) && result_trans_y >= Yv1 && result_trans_y <= (Yv1 + 2.0f)) {// verifica se o clique está dentro do quadrado, so entao ele realiza a movimentação 
            Xv1 = result_trans_x - 1.0f; // aqui é uma gambiarra para pegar o meio do quadrado 
            Yv1 = result_trans_y - 1.0f; // aqui é uma gambiarra para pegar o meio do quadrado 

            PecaQuadrado.mover(Xv1, Yv1);

            glutPostRedisplay();

        }

    }
}

//Função que será usada para desenhar o conteúdo no Frame Buffer
void desenha() {
    //glEnable(GL_TEXTURE_2D);  // Ativação de Textura
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //sempre antes de desenhar qualquer coisa, deve-se limpar o Frame Buffer
    //glMatrixMode(GL_PROJECTION);
     
    glLoadIdentity();
    glOrtho(-3, 7, -3, 7, -5, 7);// tamanho de visualização da tela é de 6 vai de (-3 até 7)
    
    glm::mat4 matrizCamera = glm::lookAt(glm::vec3(camPos), //eye = posição da câmera
                             glm::vec3(0, 0, 0),  //at  = para onde a câmera aponta
                             glm::vec3(0, 1, 0)); //up  = para onde o topo da câmera aponta
    glMultMatrixf(glm::value_ptr(matrizCamera)); //criada a matriz usando GLM, deve-se enviá-la para OpenGL
    /*gluLookAt(-2.5, 2.5, 5.0,  // Posição da câmera
        0.0, 0.0, 0.0,  // Ponto de para onde olha
        0.0, 1.0, 0.0);  // Vetor de orientação para cima
      */
    glBindTexture(GL_TEXTURE_2D, texcubo);  
    PecaQuadrado.desenha();
    glBindTexture(GL_TEXTURE_2D, 0);
    //PecaQuadrado2.desenha();
    



    glFlush();  //Todas as instruções anteriores apenas indicaram o que deve ser feito. Essa é a ordem pra GPU redesenhar com as informações enviadas
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                         //inicializar a biblioteca GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  //configurações do frame buffer (um frame buffer com três canais de cor: RGB)
    glutInitWindowPosition(200, 200);              //posição do canto superior esquerdo da janela com relação a tela
    glutInitWindowSize(Tam_Janela_init, Tam_Janela_init);                  //resolução da janela (framebuffer)
    glutCreateWindow("Jogo Pluzze version 1.0");             //cria a janela (a string aparece na barra de título da janela)

    inicio();
    //GLuint texcubo = carregarTextura("/home/renato/cg/Gumball.jpg");
   
    glutDisplayFunc(desenha);   //indica pra GLUT que a função 'desenha' será chamada para atualizar o frame buffer
    glutMouseFunc(mouseClick);
    glutMotionFunc(movimentoMouse);

    glutKeyboardFunc(teclado);
    glutTimerFunc(1000.0 / FPS, timer, 0);
    glutMainLoop();             //mantém um laço interno usando threads para que a janela permaneça aberta


    return 0;
}
