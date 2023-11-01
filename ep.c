#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//LÊ A QUANTIDADE DE LINHAS DE UM ARQUIVO
int contarLinhas(char* nomeArquivo) {
    int totalDeLinhas = 0;
    char linha[50];
    FILE* file = fopen(nomeArquivo, "r");

    if (file != NULL) {
        while (fgets(linha, sizeof(linha), file) != NULL) {
            totalDeLinhas++;
        }   
        fclose(file);
        return totalDeLinhas;
    } else {
        printf("Erro ao tentar abrir arquivo para contar as linhas\n");
        return -1;
    }  
}

//LÊ A QUANTIDADE DE COLUNAS DE UM ARQUIVO (PARA O NOSSO CONTEXTO)
int contarColunas(char* nomeArquivo){
    int totalDeColunas = 0;
    char linha[50];
    FILE* file = fopen(nomeArquivo, "r");

    if (file != NULL) {
        if (fgets(linha, sizeof(linha), file) != NULL) {
            char* token = strtok(linha, ",");
            while (token != NULL) {
                totalDeColunas++;
                token = strtok(NULL, ",");
            }
        }

        fclose(file);
        return totalDeColunas;
    } else {
        printf("Erro ao tentar abrir arquivo para contar as colunas\n");
        return -1;
    } 
}

// Lê o conteúdo do arquivo (caracter por caracter) e retorno um array de float com os dados
float* leArquivo (char* nomeArquivo, int quantidadeLinhas, int quantidadeColunas) { 
    float* array = (float*) malloc(sizeof(float)*quantidadeLinhas*quantidadeColunas);

    float numero = 0;
    float divisor = 10.0;
    int decimal = 0;
    int index = 0;

    FILE* file = fopen(nomeArquivo, "r");
    
    for (char caracter = getc(file); caracter != EOF; caracter = getc(file)){
        if (caracter == '\r') continue;

        if (caracter == ',' || caracter == '\n') {
            array[index] = numero;
            numero = 0;
            divisor = 10.0;
            decimal = 0;
            index++;
            
            continue;
        }

        
        if (caracter == '.') {
            decimal = 1;
        } else {
            int digito = caracter - '0';
            if (decimal) {
                numero += digito / divisor;
                divisor *= 10;
            } else {
                numero = numero * 10 + digito;
            }
        }
    }    

    fclose(file);
    return array;
}

float* knn (int k, float *xtrain, float *ytrain, float *xtest) {
    float* distancias = (float*) malloc(sizeof(float)*576);
    float* ytest = (float*) malloc(sizeof(float)*k);
    int inicio = 0;

    for(int index = 0; index < k; index++) {
        for (int i = 0; i < 576; i++) {
            float distancia = 0;
            for (int j = inicio; j < inicio+8; j++) {
                float operacao = xtest[j] - xtrain[(j - inicio) + i * 8];
                distancia += (operacao * operacao);
                //distancia += pow(xtest[j] - xtrain[(j - inicio) + i * 8], 2);
            }
            distancias[i] = distancia;
        }
        
        float menorDistancia = distancias[0];
        int posicaoMenor = 0;
    
        for (int i = 1; i < 576; i++) {
            if (distancias[i] < menorDistancia) {
                menorDistancia = distancias[i];
                posicaoMenor = i;
            }
        }

        ytest[index] = ytrain[posicaoMenor];
        inicio +=8 ;
    }

    return ytest;
}

void salvarVetorEmArquivo(char *nomeArquivo, float* vetor, int tam) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    for (int i = 0; i < tam; i++) {
        fprintf(arquivo, "%.1f\n", vetor[i]);
    }

    fclose(arquivo);
}

int main (int argc, char** argv) {

    float* xtrain;
    float* ytrain;
    float* xtest;

    int linhas;
    int colunas;

    for (int i = 1; i < 4; i++) {
        linhas = contarLinhas(argv[i]);
        colunas = contarColunas(argv[i]);

        if (strcmp(argv[i], "xtrain.txt") == 0)
            xtrain = leArquivo(argv[i], linhas, colunas);

        if (strcmp(argv[i], "ytrain.txt") == 0)
            ytrain = leArquivo(argv[i], linhas, colunas);

        if (strcmp(argv[i], "xtest.txt") == 0)
            xtest = leArquivo(argv[i], linhas, colunas);
    }

    float* ytest = knn(contarLinhas("xtest.txt"), xtrain, ytrain, xtest);
    salvarVetorEmArquivo("ytest.txt", ytest, 192); 

    return 0;
}
