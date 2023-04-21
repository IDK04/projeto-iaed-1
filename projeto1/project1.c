/* iaed-23 - ist1106876 - project1 */
/*
 * Ficheiro:  project1.c
 * Autor:     Duarte Laia
 * Descrição: Primeiro projeto de IAED - Gestão de carreiras de transporte público
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1                        /*       abstração de verdadeiro        */
#define FALSE 0                       /*          abstração de falso          */
#define NAO_EXISTE -1                 /*   abstração de índice não existente  */

#define MAXIMO_CARREIRA 21            /*    dimensão do nome de carreiras     */
#define MAXIMO_NUMERO_CARREIRAS 200   /*    dimensão do vetor de carreiras    */
#define MAXIMO_INVERSO 8              /* dimensão máxima do parâmetro inverso */

#define MAXIMO_PARAGEM 51             /*     dimensão do nome de paragens     */
#define MAX_DOUBLE 30                 /*   dimensão da leitura de um double   */
#define MAXIMO_NUMERO_PARAGENS 10000  /*    dimensão do vetor de paragens     */

#define MAXIMO_NUMERO_LIGACOES 30000  /*    dimensão do vetor de ligações     */

/* Estruturas */
typedef struct 
{ 
    char nome[MAXIMO_PARAGEM];
    double latitude;
    double longitude;

} Paragem;

typedef struct 
{
    char nome[MAXIMO_CARREIRA];
    char origem[MAXIMO_PARAGEM];
    char destino[MAXIMO_PARAGEM];

} Carreira;

typedef struct 
{
    char carreira[MAXIMO_CARREIRA];
    char origem[MAXIMO_PARAGEM];
    char destino[MAXIMO_PARAGEM];
    double custo;
    double duracao;

} Ligacao;
/* ---------- */

/* Variaveis globais */
Carreira _carreiras[MAXIMO_NUMERO_CARREIRAS] = {0};  /*   Vetor de carreiras  */
int _numCarreiras = 0;                               /* Contador de carreiras */

Paragem _paragens[MAXIMO_NUMERO_PARAGENS] = {0};     /*   Vetor de paragens   */
int _numParagens = 0;                                /*  Contador de paragens */

Ligacao _ligacoes[MAXIMO_NUMERO_LIGACOES] = {0};     /*   Vetor de ligações   */
int _numLigacoes = 0;                                /*  Contador de ligações */
/* ----------------- */

/* Auxiliares */
/* Apaga uma string */
void apagarStr(char str[]) { strcpy(str, ""); }

/* Verifica se uma string está vazia */
int strVazia(char str[]) { return (strcmp(str, "") == 0); }
/* ---------- */

/* Input */
/* Limpa espaços do stdin*/
char limparComando()
{
    char c;
    while ((c = getchar()) == ' ');
    return c;
}

/* Lê uma sequencia de caracteres completa do stdin */
int lerArgumento(char argumento[])
{
    char c;
    int i = 0;

    c = limparComando();

    if (c == '\"')
    {
        while ((c = getchar()) != '\"')
        {
            argumento[i++] = c;
        }
        argumento[i] = '\0';
        return (c = getchar()) != '\n';
    }
    else
    {
        /* funcionamento diferente do anterior para ler o caracter apos os espacos*/
        while ((c != ' ') && (c != '\n'))
        {
            argumento[i++] = c;
            c = getchar();
        }
        argumento[i] = '\0';

        return c != '\n';
    }
}
/* ----- */

/* BubbleSort */
void troca(int* n1, int* n2){
    int temp;
    temp = *n1;
    *n1 = *n2;
    *n2 = temp;
}

void bubbleSort(int vetor[], int tamanho, int (*comparacao)(int,int))
{
    int i, j, ordenado = TRUE;

    for (i = 0; i < tamanho-1; i++)
    {
        ordenado = TRUE;
        for (j = tamanho-1; j > i; j--)
            if ((*comparacao)(vetor[j-1],vetor[j]))
            {
                troca(&vetor[j], &vetor[j-1]);
                ordenado = FALSE;
            }
        if (ordenado)
            break;
    }
}
/* -------- */

/* Carreira */

/* 
 * Função principal do comando "c" :
 * c [ <nome-da-carreira> [ inverso ] ]
 * 
 * Funcionalidades:
 * -> Cria uma nova carreira
 * -> Lista as carreiras existentes com informação adicional
 * -> Lista as paragens associadas a uma determinada carreira
 * 
 * Erros:
 * -> incorrect sort option: Argumento "inverso" incorreto
*/

/* Retorna o indice da carreira */
int encontraCarreira(char nomeCarreira[])
{
    int i;
    for (i = 0; i < _numCarreiras; i++)
        if (strcmp(nomeCarreira, _carreiras[i].nome) == 0)
            return i;

    return NAO_EXISTE;
}

/* Cria uma nova carreira */
void criarCarreira(char nomeCarreira[])
{
    Carreira novaCarreira;
    strcpy(novaCarreira.nome, nomeCarreira);
    strcpy(novaCarreira.destino, "");
    strcpy(novaCarreira.origem, "");

    _carreiras[_numCarreiras] = novaCarreira;
    _numCarreiras++;
}

/* Calcula os calores totais associados a uma carreira */
void calculaValoresCarreira(int i, double* custoTotal, double* duracaoTotal,
                            int* numParagens){

    int temLigacoes = FALSE,  indiceLigacao;

    for (indiceLigacao = 0; indiceLigacao < _numLigacoes; indiceLigacao++)
    {
        if (strcmp(_ligacoes[indiceLigacao].carreira, _carreiras[i].nome) == 0)
        {
            temLigacoes = TRUE;
            *custoTotal += _ligacoes[indiceLigacao].custo;
            *duracaoTotal += _ligacoes[indiceLigacao].duracao;
            *numParagens += 1;
        }
    }

    /* Condição de controlo (o número de paragens é inicializado a 1) */
    if (!temLigacoes)
        (*numParagens)--;

}

/* Lista as carreiras existentes */
void listarCarreiras()
{
    int i, numParagens = 1;
    double custoTotal = 0.0, duracaoTotal = 0.0;

    for (i = 0; i < _numCarreiras; i++)
    {
        custoTotal = 0.0;
        duracaoTotal = 0.0;
        numParagens = 1;

        calculaValoresCarreira(i, &custoTotal, &duracaoTotal, &numParagens);
        
        printf("%s ", _carreiras[i].nome);
        if (strcmp(_carreiras[i].origem, "") != 0)
        {
            printf("%s %s ", _carreiras[i].origem, _carreiras[i].destino);
        }
        printf("%d %.2f %.2f\n", numParagens, custoTotal, duracaoTotal);
    }
}

/* Retorna uma array com os indices das ligações referentes a uma carreira*/
int listaIndiceLigacoes(int indiceCarreira, int listaLigacoes[])
{
    int i, numListaLigacoes = 0;

    for (i = 0; i < _numLigacoes; i++)
    {
        if (strcmp(_carreiras[indiceCarreira].nome, _ligacoes[i].carreira) == 0)
        {
            listaLigacoes[numListaLigacoes] = i;
            numListaLigacoes++;
        }
    }
    return numListaLigacoes;
}

int comparacaoParagensCarreira(char paragem[], int indiceCarreira, int inverso){
    if (!inverso)
        return strcmp(paragem, _carreiras[indiceCarreira].destino) != 0;
    else
        return strcmp(paragem, _carreiras[indiceCarreira].origem) != 0;
}

void atualizaParagemAtual(char paragem[], int indiceLigacaoAtual, int inverso){
    if (!inverso)
    {
        if (strcmp(paragem, _ligacoes[indiceLigacaoAtual].origem) == 0)
        {
            printf("%s, ", paragem);
            strcpy(paragem, _ligacoes[indiceLigacaoAtual].destino);
        }
    }
    else
    {
        if (strcmp(paragem, _ligacoes[indiceLigacaoAtual].destino) == 0)
        {
            printf("%s, ", paragem);
            strcpy(paragem, _ligacoes[indiceLigacaoAtual].origem);
        }
    }
}

int atualizaContadorParagens(int indiceListaLigacoesCarreira, int numLigacoes,
                              int inverso)
{
    /* Como as ligações podem não estar por ordem, sempre que se chegar ao
    indice final, voltar ao inicio */
    if (!inverso)
    {
        (indiceListaLigacoesCarreira)++;
        if ((indiceListaLigacoesCarreira) >= numLigacoes)
            indiceListaLigacoesCarreira = 0;
    }
    else
    {
        (indiceListaLigacoesCarreira)--;
        if ((indiceListaLigacoesCarreira) < 0)
            (indiceListaLigacoesCarreira) = numLigacoes - 1;
    }
    return indiceListaLigacoesCarreira;
    
}

void listarParagensCarreira(int indiceCarreira, int inverso)
{
    int numLigacoes, listaIndiceLigacoesCarreira[MAXIMO_NUMERO_LIGACOES],
        indiceListaLigacoesCarreira = 0, indiceLigacaoAtual, primeiraLigacao = TRUE;

    char paragem[MAXIMO_PARAGEM];

    numLigacoes = listaIndiceLigacoes(indiceCarreira, listaIndiceLigacoesCarreira);
    if (numLigacoes == 0)
        return;

    /* As ligacoes são guardadas "por referência" (pelo indice correspondente)
       no vetor de ligações */
    indiceLigacaoAtual = listaIndiceLigacoesCarreira[indiceListaLigacoesCarreira];

    if (!inverso)
        strcpy(paragem, _carreiras[indiceCarreira].origem);
    else
        strcpy(paragem, _carreiras[indiceCarreira].destino);

    /* O loop só termina quando a paragem atual for a correspondente ao destino
       da carreira */
    while (comparacaoParagensCarreira(paragem, indiceCarreira, inverso) ||
            primeiraLigacao)
    {

        atualizaParagemAtual(paragem, indiceLigacaoAtual, inverso);

        primeiraLigacao = FALSE;
        indiceListaLigacoesCarreira =
        atualizaContadorParagens(indiceListaLigacoesCarreira, numLigacoes, inverso);
        indiceLigacaoAtual = listaIndiceLigacoesCarreira[indiceListaLigacoesCarreira];
    }
    printf("%s\n", paragem);
}

/* Lê os argumentos */
void leArgumentoCarreira(int* numArgumentos, char nomeCarreira[],
                         char argumentoOpcional[])
{
    int lerArg = TRUE;

    lerArg = lerArgumento(nomeCarreira);
    *numArgumentos += !strVazia(nomeCarreira);

    if (lerArg)
    {
        lerArg = lerArgumento(argumentoOpcional);
        *numArgumentos += !strVazia(argumentoOpcional);
    }

    if (lerArg)
        limparComando();

}

/* Verifica os argumentos*/
int verificaArgumentosCarreira(char argumentoOpcional[]){
    char inverterComp[] = "inverso";

    if (!((strlen(argumentoOpcional) >= 3) &&
                !(strncmp(argumentoOpcional, inverterComp, strlen(argumentoOpcional)))))
    {
        printf("incorrect sort option.\n");
        return FALSE;
    }
    return TRUE;
}

/* Função principal da carreira */
void chamarCarreira()
{
    char nomeCarreira[MAXIMO_CARREIRA], argumentoOpcional[MAXIMO_INVERSO],
         indiceCarreira;
    int numArgumentos = 0;

    leArgumentoCarreira(&numArgumentos, nomeCarreira, argumentoOpcional);

    if (numArgumentos == 2)
        if (!verificaArgumentosCarreira(argumentoOpcional))
            return;

    indiceCarreira = encontraCarreira(nomeCarreira);

    switch (numArgumentos)
    {
    case 0:
        listarCarreiras();
        break;

    case 1:
        if (indiceCarreira == NAO_EXISTE)
            criarCarreira(nomeCarreira);
        else
            listarParagensCarreira(indiceCarreira, FALSE);
        break;

    case 2:
        listarParagensCarreira(indiceCarreira, TRUE);
    }
}

/* Paragem */

/* 
 * Função principal do comando "p" :
 * p [ <nome-de-paragem> [ <latitude> <longitude> ]
 * 
 * Funcionalidades:
 * -> Cria uma nova paragem
 * -> Lista as paragens existentes com informação adicional
 * -> Mostra a latitude e longitude de uma paragem
 * 
 * Erros:
 * -> stop already exists: Nome de paragem já existente
 * -> no such stop: Tentativa de listar paragem não existente
*/

/* Retorna o indice de uma paragem */
int encontrarParagem(char nomeParagem[])
{
    int i;
    for (i = 0; i < _numParagens; i++)
    {
        if (strcmp(nomeParagem, _paragens[i].nome) == 0)
            return i;
    }
    return NAO_EXISTE;
}

/* Mostra os detalhes (latitude e longitude de uma paragem)*/
void detalharParagem(int indiceParagem, int linhaUnica)
{
    printf("%16.12f %16.12f", _paragens[indiceParagem].latitude,
           _paragens[indiceParagem].longitude);
    /* Bool de controlo para diferentes comportamentos na chamada à função */
    if (!linhaUnica)
        putchar('\n');
    else
        putchar(' ');
}

/* Cria uma nova paragem */
void criarParagem(char nomeParagem[], char latitudeStr[], char longitudeStr[])
{
    Paragem novaParagem;
    double latitude, longitude;

    latitude = strtod(latitudeStr, NULL);
    longitude = strtod(longitudeStr, NULL);

    strcpy(novaParagem.nome, nomeParagem);

    novaParagem.latitude = latitude;
    novaParagem.longitude = longitude;

    _paragens[_numParagens] = novaParagem;
    _numParagens++;
}

/* Verifica se uma carreira já foi vista */
int carreiraVista(int indiceCarreira, int carreirasVistas[], int numCarreirasVistas)
{
    int i;
    for (i = 0; i < numCarreirasVistas; i++)
        if (indiceCarreira == carreirasVistas[i])
            return TRUE;
    return FALSE;
}

/* Retorna o número de carreiras que param na paragem*/
int numCarreirasComParagem(int indiceParagem)
{
    int i, total = 0, indiceCarreira;
    int carreirasVistas[MAXIMO_NUMERO_CARREIRAS], numCarreirasVistas = 0;

    for (i = 0; i < _numLigacoes; i++)
    {
        indiceCarreira = encontraCarreira(_ligacoes[i].carreira);
        if (((strcmp(_ligacoes[i].destino, _paragens[indiceParagem].nome) == 0) ||
             (strcmp(_ligacoes[i].origem, _paragens[indiceParagem].nome) == 0)) &&
            !carreiraVista(indiceCarreira, carreirasVistas, numCarreirasVistas))
        {
            total++;
            carreirasVistas[numCarreirasVistas] = indiceCarreira;
            numCarreirasVistas++;
        }
    }

    return total;
}

/* Lista todas as paragens */
void listarParagens()
{
    int i, total;

    for (i = 0; i < _numParagens; i++)
    {
        total = numCarreirasComParagem(i);
        printf("%s: ", _paragens[i].nome);
        detalharParagem(i, TRUE);
        printf("%d\n", total);
    }
}

void leArgumentosParagem(int* numArgumentos, char nomeParagem[],
                         char latitudeStr[], char longitudeStr[]){
    int lerArg = TRUE;

    lerArg = lerArgumento(nomeParagem);
    *numArgumentos += !strVazia(nomeParagem);

    if (lerArg)
    {
        lerArg = lerArgumento(latitudeStr);
        *numArgumentos += !strVazia(latitudeStr);

        lerArg = lerArgumento(longitudeStr);
        *numArgumentos += !strVazia(longitudeStr);
    }
    if (lerArg)
        limparComando();
}

int verificaArgumentosParagem(int numArgumentos, int indiceParagem,
                              char nomeParagem[])
{

    if ((indiceParagem == NAO_EXISTE) && (numArgumentos==1))
    {
        printf("%s: no such stop.\n", nomeParagem);
        return FALSE;
    }
    if ((indiceParagem != NAO_EXISTE) && (numArgumentos==3))
    {
        printf("%s: stop already exists.\n", nomeParagem);
        return FALSE;
    }

    return TRUE;
}

/* Função principal da paragem */
void chamarParagem()
{
    int numArgumentos = 0, indiceParagem;

    char nomeParagem[MAXIMO_PARAGEM], longitudeStr[MAX_DOUBLE],
        latitudeStr[MAX_DOUBLE];

    leArgumentosParagem(&numArgumentos, nomeParagem, latitudeStr, longitudeStr);    

    indiceParagem = encontrarParagem(nomeParagem);

    if (!verificaArgumentosParagem(numArgumentos, indiceParagem, nomeParagem))
        return;

    switch (numArgumentos)
    {
    case 0: listarParagens();
        break;

    case 1: detalharParagem(indiceParagem, FALSE);
        break;

    case 3: criarParagem(nomeParagem, latitudeStr, longitudeStr);
        break;
    }
}

/* Ligacao */

/* 
 * Função principal do comando "l" :
 * l <nome-de-carreira> <paragem-origem> <paragem-destino> <custo> <duração>
 * 
 * Funcionalidades:
 * -> Cria uma nova ligação
 * 
 * Erros:
 * -> no such line: Nome de carreira não existente
 * -> no such stop: Nome de paragem não existente
 * -> link cannot be associated with bus line: Impossibilidade de adicionar
 *                                             ligação a um extremo da carreira
 * -> negative cost or duration: Custo ou duração com valor negativo
*/

/* Altera o destino de uma carreira */
void alteraDestinoCarreira(int indiceCarreira, int indiceParagem)
{
    strcpy(_carreiras[indiceCarreira].destino, _paragens[indiceParagem].nome);
}

/* Altera a origem de uma carreira */
void alteraOrigemCarreira(int indiceCarreira, int indiceParagem)
{
    strcpy(_carreiras[indiceCarreira].origem, _paragens[indiceParagem].nome);
}

/* Adiciona uma nova ligação */
void adicionaLigacao(int indiceCarreira, int indiceParagemOrigem,
                     int indiceParagemDestino, double custo, double duracao)
{
    Ligacao novaLigacao;
    strcpy(novaLigacao.carreira, _carreiras[indiceCarreira].nome);
    strcpy(novaLigacao.origem, _paragens[indiceParagemOrigem].nome);
    strcpy(novaLigacao.destino, _paragens[indiceParagemDestino].nome);
    novaLigacao.custo = custo;
    novaLigacao.duracao = duracao;
    _ligacoes[_numLigacoes] = novaLigacao;
    _numLigacoes++;
}

/* Compara a origem de uma carreira com a paragem de destino de uma ligação */
int comparaOrigemCarreira(int indiceCarreira, int indiceParagem)
{
    return strcmp(_carreiras[indiceCarreira].origem,
                  _paragens[indiceParagem].nome) == 0;
}
/* Compara o destino de uma carreira com a paragem de origem de uma ligação */
int comparaDestinoCarreira(int indiceCarreira, int indiceParagem)
{
    return strcmp(_carreiras[indiceCarreira].destino,
                  _paragens[indiceParagem].nome) == 0;
}

void modificaCarreira(int alteraDestino, int alteraOrigem, int indiceCarreira,
                      int indiceParagemDestino, int indiceParagemOrigem,
                      int primeiraLigacao)
{

    if (alteraDestino && alteraOrigem)
    {
        /* Caso do loop */
        alteraDestinoCarreira(indiceCarreira, indiceParagemDestino);
    }
    else if (primeiraLigacao)
    {
        alteraOrigemCarreira(indiceCarreira, indiceParagemOrigem);
        alteraDestinoCarreira(indiceCarreira, indiceParagemDestino);
    }
    else if (alteraDestino)
    {
        alteraDestinoCarreira(indiceCarreira, indiceParagemDestino);
    }
    else if (alteraOrigem)
    {
        alteraOrigemCarreira(indiceCarreira, indiceParagemOrigem);
    }

}

void leArgumentoLigacoes(char nomeCarreira[], char nomeParagemOrigem[], 
                         char nomeParagemDestino[], char custoStr[], 
                         char duracaoStr[]){
    int lerArg = TRUE;
    lerArgumento(nomeCarreira);
    lerArgumento(nomeParagemOrigem);
    lerArgumento(nomeParagemDestino);
    lerArgumento(custoStr);
    lerArg = lerArgumento(duracaoStr);

    if (lerArg)
        limparComando();

}

int verificaArgumentosLigacoes1(int indiceCarreira, int indiceParagemDestino,
                               int indiceParagemOrigem, char nomeCarreira[],
                               char nomeParagemDestino[], char nomeParagemOrigem[],
                               char custoStr[], char duracaoStr[])
{
    if (indiceCarreira == NAO_EXISTE)
    {
        printf("%s: no such line.\n", nomeCarreira);
    }
    else if (indiceParagemDestino == NAO_EXISTE)
    {
        printf("%s: no such stop.\n", nomeParagemDestino);
    }
    else if (indiceParagemOrigem == NAO_EXISTE)
    {
        printf("%s: no such stop.\n", nomeParagemOrigem);
    }
    else if ((custoStr[0] == '-') || (duracaoStr[0] == '-'))
    {
        printf("negative cost or duration.\n");
    }
    return (indiceCarreira>=0) && (indiceParagemDestino>=0) &&
            (indiceParagemOrigem>=0) && (custoStr[0] != '-') && 
            (duracaoStr[0] != '-');
}

int verificaArgumentosLigacoes2(int alteraDestino, int alteraOrigem,
                                int primeiraLigacao){
    if (!primeiraLigacao && !alteraDestino && !alteraOrigem)
    {
        printf("link cannot be associated with bus line.\n");
        return FALSE;
    }
    return TRUE;
}

/* Função principal das ligações */
void chamarLigacoes()
{
    int indiceCarreira, indiceParagemOrigem, indiceParagemDestino, alteraOrigem,
        alteraDestino, primeiraLigacao;

    double custo, duracao;

    char nomeCarreira[MAXIMO_CARREIRA], nomeParagemOrigem[MAXIMO_NUMERO_PARAGENS],
        nomeParagemDestino[MAXIMO_NUMERO_PARAGENS], custoStr[MAX_DOUBLE],
        duracaoStr[MAX_DOUBLE];
    
    leArgumentoLigacoes(nomeCarreira, nomeParagemOrigem, nomeParagemDestino,
                        custoStr, duracaoStr);

    indiceCarreira = encontraCarreira(nomeCarreira);
    indiceParagemDestino = encontrarParagem(nomeParagemDestino);
    indiceParagemOrigem = encontrarParagem(nomeParagemOrigem);

    if (!verificaArgumentosLigacoes1(indiceCarreira, indiceParagemDestino,
                               indiceParagemOrigem, nomeCarreira,
                               nomeParagemDestino, nomeParagemOrigem,
                               custoStr, duracaoStr))
        return;

    custo = strtod(custoStr, NULL);    /*     Parse de string para double     */
    duracao = strtod(duracaoStr, NULL);/* NULL porque não é necessário o valor*/
                                       /* de retorno                          */
    alteraDestino = comparaDestinoCarreira(indiceCarreira, indiceParagemOrigem);
    alteraOrigem = comparaOrigemCarreira(indiceCarreira, indiceParagemDestino);
    primeiraLigacao = strVazia(_carreiras[indiceCarreira].origem);

    if (!verificaArgumentosLigacoes2(alteraDestino, alteraOrigem,primeiraLigacao))
        return;

    modificaCarreira(alteraDestino, alteraOrigem, indiceCarreira,
                    indiceParagemDestino, indiceParagemOrigem, primeiraLigacao);

    adicionaLigacao(indiceCarreira, indiceParagemOrigem, indiceParagemDestino,
                    custo, duracao);
}

/* Intersecoes */

/* 
 * Função principal do comando "i" :
 * i
 * 
 * Funcionalidades:
 * -> Lista as paragens por onde pára mais de uma linha, ordenadas alfabeticamente
*/

/* Verifica se a carreira2 é "alfabeticamente superior" à carreira um */
int comparacaoNomeCarreira(int indiceCarreira1, int indiceCarreira2)
{
    return strcmp(_carreiras[indiceCarreira1].nome,
                  _carreiras[indiceCarreira2].nome) > 0;
}

/* Encontra o indice das carreiras com interseções */
void indiceCarreirasComIntersecao(int indiceCarreiras[],
                                  int* numIndiceCarreiras, int iParagem){
    int iLigacao, indiceCarreira;
    for (iLigacao = 0; iLigacao < _numLigacoes; iLigacao++)
    {
        indiceCarreira = encontraCarreira(_ligacoes[iLigacao].carreira);
        if (!carreiraVista(indiceCarreira, indiceCarreiras, *numIndiceCarreiras) &&
            ((strcmp(_paragens[iParagem].nome, _ligacoes[iLigacao].origem) == 0) ||
                (strcmp(_paragens[iParagem].nome, _ligacoes[iLigacao].destino) == 0)))
        {
            indiceCarreiras[*numIndiceCarreiras] = indiceCarreira;
            (*numIndiceCarreiras)++;
        }
    }
}

/* Função principal das interseções */
void chamarIntersecoes()
{
    int iParagem, iCarreira, indiceCarreiras[MAXIMO_NUMERO_CARREIRAS],
        numIndiceCarreiras;

    for (iParagem = 0; iParagem < _numParagens; iParagem++)
    {
        numIndiceCarreiras = 0;

        indiceCarreirasComIntersecao(indiceCarreiras, &numIndiceCarreiras, iParagem);

        if (numIndiceCarreiras > 1)
        {
            bubbleSort(indiceCarreiras, numIndiceCarreiras, comparacaoNomeCarreira);
            printf("%s %d: ", _paragens[iParagem].nome, numIndiceCarreiras);
            for (iCarreira = 0; iCarreira < numIndiceCarreiras; iCarreira++)
            {
                if (iCarreira != 0)
                    putchar(' ');
                printf("%s", _carreiras[indiceCarreiras[iCarreira]].nome);
            }
            putchar('\n');
        }
    }
}

int main()
{
    char c;
    while ((c = getchar()) != EOF && (c != '\0'))
    {
        switch (c)
        {
        case 'q': return 0;
            break;

        case 'c': chamarCarreira();
            break;

        case 'p': chamarParagem();
            break;

        case 'l': chamarLigacoes();
            break;

        case 'i': chamarIntersecoes();
            break;
        }
    }
    return 0;
}