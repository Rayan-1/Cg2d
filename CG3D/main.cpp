#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GL/glu.h>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" //incluindo o arquivo responsável por realizar leitura de arquivos de imagem

#include <iostream>
using namespace std;

#define FPS 30
#define TEX_CHAO_FILE "textura.jpg"


//variáveis globais relacionadas a câmera
glm::vec4 camPos(6,6,6,1);                                                              //posição inicial da câmera
glm::mat4 camRotacao = glm::rotate(glm::mat4(1), glm::radians(1.0f), glm::vec3(0,1,0)); //matriz de rotação para girar a câmera
bool      camMove = true;                                                               //liga/desliga movimentação da câmera
bool      showEixos = true;
unsigned int texChaoId; //identificador da textura do chão
unsigned int texCuboId; //identificador da textura dos cubos

//variáveis globais relacionadas a dimensões da janela
int larguraJanela = 500, alturaJanela = 500;
float aspectRatio;

unsigned int carregaTextura(char fileName[]){
    unsigned int texId;
    int largura, altura, nCanais;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *imagem = stbi_load(fileName, &largura, &altura, &nCanais, 0); //qtd de pixels na largura e altura preenchidas automaticamente
    if(imagem == NULL) //verificação se o carregamento da imagem ocorreu com sucesso
        cout << "Erro, Imagem não carregada" << endl;
    else
        cout << "Imagem carregada com sucesso" << endl;

    glEnable(GL_TEXTURE_2D);             //habilitando o uso de texturas
    glGenTextures(1, &texId);            //gerando um objeto textura na máquina OpenGL
    glBindTexture(GL_TEXTURE_2D, texId); //ativando o objeto textura recém criado

    //parâmetros da textura caso coordenadas além do intervalo [0,1] seja utilizado
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); //repetição espelhada no eixo S
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); //repetição espelhada no eixo T

    //parâmetros da textura para filtragem quando tamanho de pixel e texel sejam diferentes (interpolação linear em ambos os casos)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //magnificação - quando um texel cobre vários pixels
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //minificação - quando muitos texels cabem em um único pixel

    //parâmetros da textura para indicar se a textura deve-se combinar ou substituir a atual cor do objeto
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); //GL_REPLACE = substitui, GL_COMBINE = combina (ou multiplica)

    //envio dos dados da textura para o OpenGL
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 largura,          //largura da textura
                 altura,           //altura da textura (carregadas automaticamente pela função da biblioteca)
                 0,
                 GL_RGB,           //formato da cor da textura
                 GL_UNSIGNED_BYTE, //tipos de dados de cada texel da textura
                 imagem);          //ponteiro para os dados da textura

    glBindTexture(GL_TEXTURE_2D, 0); //desativando o objeto textura criado (ativa apenas quando for usar em algum objeto)

    stbi_image_free(imagem); //memória RAM pode ser liberada pois a imagem foi enviada ao OpenGL (memória gráfica)

    return texId; //retornando o identificador da textura (sem ele não há como acessar a textura carregada)
}


//É uma boa prática criar uma função para agrupar configurações iniciais do OpenGL para o desenho a ser feito
void inicio(){
    glClearColor(0.0, 0.0, 0.0, 1.0); //cor de fundo (preto)
    glEnable(GL_DEPTH_TEST);          //habilitando a remoção de faces que estejam atrás de outras (ocultas)
    glEnable(GL_MULTISAMPLE);         //habilita um tipo de antialiasing (melhora serrilhado)

    //Essas instruções são para descobrir onde está seu arquivo executável. Adicione uma pasta texturas e suas texturas nesse mesmo lugar.
    char buffer[FILENAME_MAX];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        std::cout << "Diretório corrente: " << buffer << std::endl;
    } else {
        std::cerr << "Erro ao obter o diretório corrente." << std::endl;
    }

    //carregando as texturas que serão utilizadas
    texChaoId = carregaTextura(TEX_CHAO_FILE);
    
}


//função chamada sempre que a janela for modificada de tamanho
void alteraJanela(int largura, int altura){
    larguraJanela = largura;
    alturaJanela  = altura;
    aspectRatio   = (float)larguraJanela/alturaJanela;
    glutPostRedisplay();
}

//função que trata ações feitas com o teclado
void teclado(unsigned char tecla, int x, int y){
    if(tecla == ' ') camMove = !camMove; //tecla espaço alterna entre deixar ou não a câmera girar em torno do objeto
    if(tecla == 'e' || tecla == 'E') showEixos = !showEixos; //tecla 'E' alterna entre mostrar ou ocultar os eixos do sistema de coordenadas
}

//função que é chamada em intervalos regulares de tempo
void timer(int v){
    glutTimerFunc(1000/FPS, timer, 0); //executada a quantidade FPS de vezes por segundo

    //se a opção camMove estiver verdadeira, gire a câmera
    if(camMove)
        camPos = camRotacao * camPos; //rotacionando a posição da câmera a matriz de rotação camRotacao

    //redesenhando a tela
    glutPostRedisplay();
}

//função para desenhar os eixos cartesianos do sistema de coordenadas global (ou de mundo, ou de cenário)
void eixos(){
    glLineWidth(3); //altera a largura das linhas para 3 pixels
    glBegin(GL_LINES);
        //eixo x em vermelho
        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(100,0,0);
        //eixo y em verde
        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,100,0);
        //eixo z em azul
        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,100);
    glEnd();
}

void cubo(){
    glm::vec3 verticePos[8];
    glm::vec2 texturePos[12];

    //todas as posições dos vértices do cubo
    verticePos[0] = glm::vec3(-1,-1,-1);
    verticePos[1] = glm::vec3(-1,-1, 1);
    verticePos[2] = glm::vec3(-1, 1,-1);
    verticePos[3] = glm::vec3(-1, 1, 1);
    verticePos[4] = glm::vec3( 1,-1,-1);
    verticePos[5] = glm::vec3( 1,-1, 1);
    verticePos[6] = glm::vec3( 1, 1,-1);
    verticePos[7] = glm::vec3( 1, 1, 1);

    //todas as posições da textura que serão utilizadas para mapear nos vértices do cubo
    texturePos[ 0] = glm::vec2(0.00, 0.0);
    texturePos[ 1] = glm::vec2(0.33, 0.0);
    texturePos[ 2] = glm::vec2(0.67, 0.0);
    texturePos[ 3] = glm::vec2(1.00, 0.0);
    texturePos[ 4] = glm::vec2(0.00, 0.5);
    texturePos[ 5] = glm::vec2(0.33, 0.5);
    texturePos[ 6] = glm::vec2(0.67, 0.5);
    texturePos[ 7] = glm::vec2(1.00, 0.5);
    texturePos[ 8] = glm::vec2(0.00, 1.0);
    texturePos[ 9] = glm::vec2(0.33, 1.0);
    texturePos[10] = glm::vec2(0.67, 1.0);
    texturePos[11] = glm::vec2(1.00, 1.0);

    int facesVert[6][4];
    int facesTex[6][4];

    //Cada face conecta 4 vértices. Anotando os índices do vetor verticePos que cada face conecta.
    facesVert[0][0] = 0; facesVert[0][1] = 1; facesVert[0][2] = 3; facesVert[0][3] = 2;
    facesVert[1][0] = 0; facesVert[1][1] = 4; facesVert[1][2] = 5; facesVert[1][3] = 1;
    facesVert[2][0] = 4; facesVert[2][1] = 6; facesVert[2][2] = 7; facesVert[2][3] = 5;
    facesVert[3][0] = 6; facesVert[3][1] = 2; facesVert[3][2] = 3; facesVert[3][3] = 7;
    facesVert[4][0] = 1; facesVert[4][1] = 5; facesVert[4][2] = 7; facesVert[4][3] = 3;
    facesVert[5][0] = 0; facesVert[5][1] = 2; facesVert[5][2] = 6; facesVert[5][3] = 4;

    //Cada face precisa de 4 coordenadas de textura. Anotando os índices do vetor texturaPos que cada face irá precisar.
    facesTex[0][0] = 0; facesTex[0][1] = 1; facesTex[0][2] = 5;  facesTex[0][3] = 4;
    facesTex[1][0] = 1; facesTex[1][1] = 2; facesTex[1][2] = 6;  facesTex[1][3] = 5;
    facesTex[2][0] = 2; facesTex[2][1] = 3; facesTex[2][2] = 7;  facesTex[2][3] = 6;
    facesTex[3][0] = 4; facesTex[3][1] = 5; facesTex[3][2] = 9;  facesTex[3][3] = 8;
    facesTex[4][0] = 5; facesTex[4][1] = 6; facesTex[4][2] = 10; facesTex[4][3] = 9;
    facesTex[5][0] = 6; facesTex[5][1] = 7; facesTex[5][2] = 11; facesTex[5][3] = 10;

    //desenhando o cubo com a textura
    glBindTexture(GL_TEXTURE_2D, texCuboId); //ativando a textura
    glBegin(GL_QUADS);                       //todas as faces são quadrados
    for(int i = 0 ; i < 6 ; i++){            //desenhe um total de 6 faces
        for(int v = 0 ; v < 4 ; v++){        //passando por cada um dos 4 vértices de cada face
            glTexCoord2f(texturePos[facesTex[i][v]].s, texturePos[facesTex[i][v]].t); //coordenada de textura localizada no índice armazenado naquela face
            glVertex3f(verticePos[facesVert[i][v]].x, verticePos[facesVert[i][v]].y, verticePos[facesVert[i][v]].z); //coordenada do vértice localizada no índice armazenado naquela face
        }
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0); //após terminar o desenho do objeto, desativa a textura
}

//função que desenha o cenário inteiro
void cenario(){
    //desenhando o chão no plano XZ
    glPushMatrix();
    glColor3f(0.3,0.3,0.3);
    glScalef(3,1,3);
    glBindTexture(GL_TEXTURE_2D, texChaoId);     //habilitando a textura do chão
    glBegin(GL_QUADS);                           //desenhar um quadrado
        glTexCoord2f(0,0); glVertex3f(-1,0,-1);  //coordenadas de textura seguidas pelas coordenadas de vértice correspondente do quadrado
        glTexCoord2f(1,0); glVertex3f( 1,0,-1);
        glTexCoord2f(1,1); glVertex3f( 1,0, 1);
        glTexCoord2f(0,1); glVertex3f(-1,0, 1);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);             //desabilitando a textura após terminar de usá-la
    glPopMatrix();

    //desenhando cubo 1
    glPushMatrix();
    glTranslatef(1.0,0.5,2);
    glRotatef(30, 0,1,0);
    glScalef(0.5,0.5,0.5);
    cubo();
    glPopMatrix();

    //desenhando cubo 2
    glPushMatrix();
    glTranslatef(-2.0,0.5,0);
    glRotatef(45, 0,1,0);
    glRotatef(90, 1,0,0);      //rotação de 90º para colocar um número diferente na face superior do dado
    glScalef(0.5,0.5,0.5);
    cubo();
    glPopMatrix();
}

//Função que desenha conteúdo na tela
void desenha(){
    glViewport(0,0,larguraJanela,alturaJanela);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //definindo o tipo de projeção
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glm::mat4 matrizProjecao = glm::perspective(glm::radians(45.0f),               //ângulo de abertura do frustum
                                                (float)larguraJanela/alturaJanela, //aspect ratio do plano de projeção
                                                1.0f, 100.0f);                     //near e far (limites na profundidade do volume de visualização)
    glMultMatrixf(glm::value_ptr(matrizProjecao));

    //definindo o posicionamento da câmera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glm::mat4 matrizCamera = glm::lookAt(glm::vec3(camPos), //eye = posição da câmera
                                         glm::vec3(0,0,0),  //at  = para onde a câmera aponta
                                         glm::vec3(0,1,0)); //up  = para onde o topo da câmera aponta
    glMultMatrixf(glm::value_ptr(matrizCamera)); //criada a matriz usando GLM, deve-se enviá-la para OpenGL

    //desenhando cenário (chão e objeto) e eixos
    cenario();
    if(showEixos) eixos();

    glutSwapBuffers(); //atualizando o desenho na tela
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_MULTISAMPLE | GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(larguraJanela,alturaJanela);
    glutCreateWindow("Jogo Pluzze version 1.0");

    inicio();
   

    glutDisplayFunc(desenha);
    glutReshapeFunc(alteraJanela);
    glutKeyboardFunc(teclado);
    glutTimerFunc(1000.0/FPS, timer, 0);
    

    glutMainLoop();

 // g++ -o programa main.cpp quadrado.cpp -lGL -lGLU -lglut -lSOIL
 //    ./programa

    return 0;
}
