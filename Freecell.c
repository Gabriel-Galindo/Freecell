#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

typedef struct no{
    short int valor;
    char naipe;
    struct no *prox;
} tCarta;

tCarta *primMesa[8];
tCarta *primNaipe[4];
tCarta *temp[4];
tCarta *primMonte=NULL;
char Naipe[4]={3,4,5,6};//Utilizado para fazer verificacao no movimento para o naipe

void gerarBaralho(void)//Cria um monte com todas as cartas em sequencia
{
    int i,j;
    tCarta *novo;
    for(i=3;i<7;i++){
        for(j=1;j<14;j++){
           novo=(tCarta*)malloc(sizeof(tCarta));
           novo->valor=j;
           novo->naipe=i;
           novo->prox=primMonte;
           primMonte=novo;
        }
    }
    return;
}

void embaralhaBaralho(void)
{
    tCarta *atual,*ant;
    int i,j;
    srand(time(NULL));
    for(i=0;i<156;i++){//Repete  varias vezes para garantir a aleatoriedade
            atual=primMonte;
            ant=primMonte;
        for(j=0;j<((rand()%52)+41);j++){//Usa um rand para percorrer o monte, comeca mais pro final pois ao testar percebi que o rand normalmente gera um numero baixo
             ant=atual;
             atual=atual->prox;
             if(atual==NULL){//Quando chega no final volta para o inicio
                atual=primMonte;
             }
        }
        if(ant->prox!=NULL){
            ant->prox=atual->prox;//Retira a carta atual e bota ela no inicio
            atual->prox=primMonte;
            primMonte=atual;
        }
        else{//Se a carta atual eh a inicial bota a inicial como final
            ant->prox=primMonte;
            primMonte=primMonte->prox;
            ant->prox->prox=NULL;
        }
    }
    return;
}

void distribuirMesa(void)//Como as cartas ja estao embaralhadas apenas coloca elas na mesa em "ordem"
{
    int i,j;
    tCarta *atual;
    for(i=0;i<8;i++){
        if(i<4){
            for(j=0;j<7;j++){
                atual=primMonte;
                primMonte=atual->prox;
                atual->prox=primMesa[i];
                primMesa[i]=atual;
            }
        }
        else{
            for(j=0;j<6;j++){
                atual=primMonte;
                primMonte=atual->prox;
                atual->prox=primMesa[i];
                primMesa[i]=atual;
            }
        }
    }
}

void imprime(void)
{
    tCarta *atual;
    int i;
    printf("\n[TEMP] = ");
    for(i=0;i<4;i++){
        atual=temp[i];
        if(atual!=NULL){
            printf("%d-[%2d|%c] ",i,atual->valor,atual->naipe);
        }else{
            printf("%d-[   ]",i);
        }
    }
    for(i=0;i<4;i++){
        printf("\n[NAIPE %c %d]= ",Naipe[i],i);
        atual=primNaipe[i];
        while(atual!=NULL){
            printf("[%2d|%c] ",atual->valor,atual->naipe);
            atual=atual->prox;
        }
    }
    for(i=0;i<8;i++){
        printf("\n[MESA %d]= ",i);
        atual=primMesa[i];
        while(atual!=NULL){
            printf("[%2d|%c] ",atual->valor,atual->naipe);
            atual=atual->prox;
        }
    }
    printf("\n");
    return;
}

void  moveMesaTemp(void)
{
    int mesa,tem;
    printf("\nDigite a mesa");
    scanf("%d",&mesa);
    printf("\nDigite a posicao no temporario");
    scanf("%d",&tem);
    if(((mesa>=0)&&(mesa<8))&&((tem>=0)&&(tem<4))&&(primMesa[mesa]!=NULL)){//Verifica se o movimento eh possivel pelos parametros
        if(temp[tem]==NULL){//Verifica se realmente eh possivel
            temp[tem]=primMesa[mesa];
            primMesa[mesa]=primMesa[mesa]->prox;
            temp[tem]->prox=NULL;
        }
        else{
            printf("\nMovimento invalido");
        }
    }
    else{
        printf("\nMovimento invalido");
    }
    return;
}


void moveMesaNape(void)
{
    int mesa,nape;
    tCarta *atual;
    printf("\nDigite a mesa");
    scanf("%d",&mesa);
    printf("\nDigite a posicao no naipe");
    scanf("%d",&nape);
    if(((mesa>=0)&&(mesa<8))&&((nape>=0)&&(nape<4))&&(primMesa[mesa]!=NULL)){//Verifica os parametros passados
        if(primMesa[mesa]->naipe==Naipe[nape]){//Verifica o naipe
            if((primNaipe[nape]==NULL)&&(primMesa[mesa]->valor==1)){//Se o Naipe tiver vazio verifica se a carta eh 1
                primNaipe[nape]=primMesa[mesa];
                primMesa[mesa]=primMesa[mesa]->prox;
                primNaipe[nape]->prox=NULL;
                return;
            }
            if(primNaipe[nape]!=NULL){//Se nao estiver vazio verifica se eh o valor presente no naipe + 1
                atual=primNaipe[nape];
                while(atual->prox!=NULL){
                    atual=atual->prox;
                }
               if(atual->valor+1==primMesa[mesa]->valor){
                atual->prox=primMesa[mesa];
                primMesa[mesa]=primMesa[mesa]->prox;
                atual->prox->prox=NULL;
               }
               else{
                printf("\nMovimento invalido");
               }
                return;
            }
            printf("\nMovimento invalido");
        }
        else{
            printf("\nMovimento invalido");
        }
    }
    else{
        printf("\nMovimento invalido");
    }
    return;
}

int corDiferente(char n,char na)//Funcao para verificar a cor do naipe
{
    if(n==na){
        return 0;
    }
    if((n==3&&na==4)||(n==4&&na==3)||(n==5&&na==6)||(n==6&&na==5)){
        return 0;
    }
    else{
        return 1;
    }
}

void moveTempMesa(void)
{
    int tem,mesa;
    printf("\nDigite a posicao no temporario");
    scanf("%d",&tem);
    printf("\nDigite a mesa");
    scanf("%d",&mesa);
    if(((mesa>=0)&&(mesa<8))&&((tem>=0)&&(tem<4))&&(temp[tem]!=NULL)){//Verifica os parametros passados
        if(primMesa[mesa]==NULL){
            primMesa[mesa]=temp[tem];
            temp[tem]=NULL;
            return;
        }
        else{
            if(corDiferente(primMesa[mesa]->naipe,temp[tem]->naipe)&&(primMesa[mesa]->valor>temp[tem]->valor)){//Verifica a cor e o valor das cartas
                temp[tem]->prox=primMesa[mesa];
                primMesa[mesa]=temp[tem];
                temp[tem]=NULL;
                return;
            }
            else{
                printf("\nMovimento invalido");
                return;
            }
        }
    }
    else{
        printf("\nMovimento invalido");
    }
    return;
}

void moveNapeTemp(void)
{
    int nape,tem;
    printf("\nDigite a posicao no temporario");
    scanf("%d",&tem);
    printf("\nDigite a posicao no naipe");
    scanf("%d",&nape);
    if(((nape>=0)&&(nape<4))&&((tem>=0)&&(tem<4))&&(primNaipe[nape]!=NULL)){//Verifica os parametros passados
        if(temp[tem]==NULL){
            temp[tem]=primNaipe[nape];
            primNaipe[nape]=primNaipe[nape]->prox;
            temp[tem]->prox=NULL;
        }
        else{
            printf("\nMovimento invalido");
        }
    }
    else{
        printf("\nMovimento invalido");
    }
    return;
}

void moveTempNape(void)
{
    int tem,nape;
    printf("\nDigite a posicao no temporario");
    scanf("%d",&tem);
    printf("\nDigite o naipe");
    scanf("%d",&nape);
    if(((nape>=0)&&(nape<4))&&((tem>=0)&&(tem<4))&&(temp[tem]!=NULL)){//Verifica os parametros passados
        if((temp[tem]->naipe==Naipe[nape])&&(primNaipe[nape]==NULL)){//Verifica o naipe
            primNaipe[nape]=temp[tem];
            temp[tem]=NULL;
            return;
        }
        else{
            if((temp[tem]->naipe==Naipe[nape])&&(primNaipe[nape]->valor<temp[tem]->valor)){//Se nao estiver vazio verifica o valor da carta
                temp[tem]->prox=primNaipe[nape];
                primNaipe[nape]=temp[tem];
                temp[tem]=NULL;
                return;
            }
            else{
                printf("\nMovimento invalido");
                return;
            }
        }
    }
    else{
        printf("\nMovimento invalido");
    }
    return;
}

void moveNapeMesa(void)
{
    tCarta *aux;
    int nape,mesa;
    printf("\nDigite o Naipe");
    scanf("%d",&nape);
    printf("\nDigite a mesa");
    scanf("%d",&mesa);
    if(((mesa>=0)&&(mesa<8))&&((nape>=0)&&(nape<4))&&(primNaipe[nape]!=NULL)){//Verifica os parametros passados
        if(primMesa[mesa]==NULL){
            primMesa[mesa]=primNaipe[nape];
            primNaipe[nape]=primNaipe[nape]->prox;
            primMesa[mesa]->prox=NULL;
            return;
        }
        else{
            if(corDiferente(primMesa[mesa]->naipe,primNaipe[nape]->naipe)&&(primMesa[mesa]->valor>primNaipe[nape]->valor)){//Verifica a cor do naipe e o valor
                aux=primNaipe[nape];
                primNaipe[nape]=primNaipe[nape]->prox;
                aux->prox=primMesa[mesa];
                primMesa[mesa]=aux;
                return;
            }
            else{
                printf("\nMovimento invalido");
                return;
            }
        }
    }
    else{
        printf("\nMovimento invalido");
    }
    return;
}

void moveMesaMesa(void)
{
    int reti,rece,quanti,aux=1;
    tCarta *auxiliar,*maior;
    printf("\nDigite a mesa cujas cartas serao retiradas");
    scanf("%d",&reti);
    printf("\nDigite a mesa que recebera cartas");
    scanf("%d",&rece);
    printf("\nDigite a quantidade de cartas a ser movimentada");
    scanf("%d",&quanti);
    if(((reti>=0)&&(reti<8))&&((rece>=0)&&(rece<8))&&(primMesa[reti]!=NULL)&&(quanti>=1)){//Verifica os parametros passados
        if(primMesa[rece]==NULL){
            auxiliar=primMesa[reti];
            while((aux<quanti)&&(auxiliar->prox!=NULL)){//Percorre o monte para saber o que vai sobrar da mesa antiga
                auxiliar=auxiliar->prox;
                aux++;
            }
            primMesa[rece]=primMesa[reti];//Como estava nulo simplesmente passa toda a mesa para a nova
            primMesa[reti]=auxiliar->prox;//Atualiza a mesa antiga 
            auxiliar=primMesa[rece];
            aux=1;
            while((aux<quanti)&&(auxiliar->prox!=NULL)){//Percorre o monte para saber o que deveria ter ficado na mesa antiga
                auxiliar=auxiliar->prox;
                aux++;
            }
            auxiliar->prox=NULL;//Apaga as cartas da mesa antiga
            return;
        }
        else{
            auxiliar=primMesa[reti];
            maior=primMesa[reti];
            while((aux<quanti)&&(auxiliar->prox!=NULL)){//Percorre o bloco a ser retirado para descobrir a carta mais alta
                auxiliar=auxiliar->prox;
                aux++;
                if(auxiliar->valor>maior->valor){
                    maior=auxiliar;
                }
            }
            if(corDiferente(maior->naipe,primMesa[rece]->naipe)&&(primMesa[rece]->valor>maior->valor)){//Verifica o naipe e valor da carta mais alta
                maior=primMesa[rece];//Guarda o estado inicial da mesa nova
                auxiliar=primMesa[reti];
                aux=1;
                while((aux<quanti)&&(auxiliar->prox!=NULL)){//Percorre a mesa antiga para saber o que guardar
                    auxiliar=auxiliar->prox;
                    aux++;
                }
                primMesa[rece]=primMesa[reti];//"Apaga" a mesa nova e bota o valor da mesa antiga
                primMesa[reti]=auxiliar->prox;//Atualiza a mesa antiga
                auxiliar=primMesa[rece];
                aux=1;
                while((aux<quanti)&&(auxiliar->prox!=NULL)){//Percorre a mesa nova para saber o que realmente foi movimentado
                    auxiliar=auxiliar->prox;
                    aux++;
                }
                auxiliar->prox=maior;//Adiciona o estado inicial da mesa nova ao final do adicionado
                return;
            }
            else{
                printf("\nMovimento invalido");
                return;
            }
        }
    }
    else{
        printf("\nMovimento invalido");
    }
    return;
}

int main(int argc, char **argv){
    int op=0;
    srand(time(NULL));
    gerarBaralho();
    embaralhaBaralho();
    distribuirMesa();
    while(op!=8){
        imprime();
        printf("(1)Move Mesa-Temp\n");
        printf("(2)Move Mesa-Nape\n");
        printf("(3)Move Temp-Mesa\n");
        printf("(4)Move Nape-Temp\n");
        printf("(5)Move Temp-Nape\n");
        printf("(6)Move Nape-Mesa\n");
        printf("(7)Move Mesa-Mesa\n");
        printf("(8)Sair\n");
        printf("Opcao: ");
        scanf("%d", &op);
        getchar();
        switch(op){
            case 1:
                moveMesaTemp();
                break;
            case 2:
                moveMesaNape();
                break;
            case 3:
                moveTempMesa();
                break;
            case 4:
                moveNapeTemp();
                break;
            case 5:
                moveTempNape();
                break;
            case 6:
                moveNapeMesa();
                break;
            case 7:
                moveMesaMesa();
                break;    
            default:
                printf("Opcao Invalida.\n");
                break;
        }
    }
    return 0;
}
