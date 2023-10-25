#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Lê o conteúdo do arquivo (caracter por caracter) e retorno uma matriz de float com os dados
float** leArquivo (char* nomeArquivo, int quantidadeLinhas, int quantidadeColunas) { 
    float** matriz = (float**) malloc(sizeof(float*)*quantidadeLinhas);

    for (int i = 0; i < quantidadeLinhas; i++){
        matriz[i] = (float*) malloc(sizeof(float)*quantidadeColunas);
    }

    float numero = 0;
    float divisor = 10.0;
    int decimal = 0;
    int linha = 0;
    int coluna = 0;

    FILE* file = fopen(nomeArquivo, "r");
    
    for (char caracter = getc(file); caracter != EOF; caracter = getc(file)){
        if (caracter == '\r') continue;

        if (caracter == ',' || caracter == '\n') {
            matriz[linha][coluna] = numero;
            numero = 0;
            divisor = 10.0;
            decimal = 0;

            if (caracter == '\n'){
                coluna = 0;
                linha++;
            }    

            if (caracter == ',')
                coluna++;
            
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
    return matriz;
}



int ordena(int k, float *xtrain, float *ytrain, float *xtest) {

}

int main (int argc, char** argv) {
    char* nomeArquivo = argv[1];
    printf("%d\n", contarLinhas(nomeArquivo));  //TESTE
    printf("%d\n", contarColunas(nomeArquivo)); //TESTE

    int linhas = contarLinhas(nomeArquivo);
    int colunas = contarColunas(nomeArquivo);
    float** matriz = leArquivo(nomeArquivo, linhas, colunas);

    //IMPRIME O CONTEUDO DO ARQUIVO LIDO DO ARQUIVO E ARMAZENADO EM matriz 
    for(int i = 0; i < linhas; i++) {     
        printf("[");
        for(int j = 0; j < colunas; j++) {
            printf("%f, ", matriz[i][j]);
        }
        printf("]\n");
    }    

    

    return 0;
}
