#ifndef TEXTURA_H
#define TEXTURA_H

Class Textura{
    private:
        int largura, altura, nCanais;
        unsigned char *dados;
        unsigned int id;
    public:
        Textura();
        void     carregar(char *arquivo);
        unsigned int get_id();

};

#endif


