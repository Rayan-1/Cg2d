#ifndef QUADRADO_H
#define QUADRADO_H

class Quadrado {
public:
    Quadrado(float x, float y);
    void desenha() const;
    void mover(float novoX, float novoY);
    void girar(float angulo);

private:
    float Xv1;
    float Yv1;
    float angle; // Novo membro para armazenar o ângulo de rotação
};

#endif

