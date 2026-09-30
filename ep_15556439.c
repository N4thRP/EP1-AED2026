#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct celula{
    int linha;
    int coluna;
    int valor;
    struct celula *proxima_linha;
    struct celula *proxima_coluna;
} celula_t;

typedef struct fileira {
    int indice;
    celula_t* primeiro;
    struct fileira *proximo;
} fileira_t;

typedef struct {
    bool transposicao;
    int linha;
    int coluna;
    union {
        int valor_anterior;
        int tamanho;
    };
} operacao_t;

typedef struct elo_pilha {
    operacao_t op;
    struct elo_pilha *proximo;
} elo_pilha_t;

typedef struct {
    elo_pilha_t* topo;
} pilha_t;

typedef struct {
    fileira_t* primeira_linha;
    fileira_t* primeira_coluna;
    int total_celulas;
    pilha_t historico;
} planilha_t;

//FUNÇÃO AUXILIAR, LEITURA DE STRINGS DOS COMANDOS
int igual(char* a, char* b) {
    int i = 0;
    while (a[i] == b[i] && a[i] != '\0') i++;

    return (a[i] == b[i]);
}

//OPERAÇÕES COM FUNÇÕES!!!!!!!!!!!!!

void inicializar_planilha(planilha_t *p) {
    // TODO: inicialize primeiraLinha, primeiraColuna, total_celulas e historico.topo
    p->primeira_linha = (fileira_t *) malloc(sizeof(fileira_t)); //????? CONFERIR!!!
    p->primeira_coluna = (fileira_t *) malloc(sizeof(fileira_t));
    p->historico.topo = (elo_pilha_t *) malloc(sizeof(elo_pilha_t));

    p->primeira_linha = NULL; //eh um ponteiro
    p->primeira_coluna = NULL; //eh um ponteiro
    p->total_celulas = 0; //int
    p->historico.topo = NULL; //eh ponteiro
}

celula_t* buscar_celula(planilha_t *p, int lin, int col, celula_t** cel_ant_linha, celula_t** cel_ant_coluna, fileira_t** fil_ant_linha, fileira_t** fil_ant_coluna) {
    /* TODO: localize a celula (lin,col), preencha os quatro antecessores 
    por referencia, retorne NULL se a celula nao existir */

    //primeiro estabelecer um padrão
    fileira_t *fil_linha = p->primeira_linha;
    fileira_t *fil_coluna = p->primeira_coluna;
    celula_t *cel_lin;
	celula_t *cel_col;
	if(fil_coluna != NULL && fil_linha != NULL) {
		cel_lin = p->primeira_linha->primeiro;
		cel_col = p->primeira_coluna->primeiro;
	} else {
		cel_lin = NULL;
		cel_col = NULL;
	}
    //os antecessores são: a celula anterior da linha, a celula anterior da coluna, a fileira anterior da linha e a fileira anterior da coluna
    *cel_ant_linha = NULL;
    *cel_ant_coluna = NULL; 
    *fil_ant_linha = NULL;
    *fil_ant_coluna = NULL;
    
    //primeiro while para achar a linha
    while(fil_linha != NULL && fil_linha->indice < lin){ //&& ou ||
        //cel_lin->coluna = fil_coluna->indice; //parte SUPER IMPORTANTE, SE NÃO AS LINHAS E COLUNAS NÃO ESTARÃO DEMARCADAS NA PRÓPRIA CELULA!!!
        printf("entrou no while");
        
        *fil_ant_linha = fil_linha; //lembrando que antes era NULL
        //agora para continuar o while
        fil_linha = fil_linha->proximo;
        if(fil_linha != NULL){
            cel_lin = fil_linha->primeiro;
        }
    }
    //achou? agora vai pra coluna certa
    if(fil_linha != NULL && fil_linha->indice == lin){
        while(cel_lin != NULL && cel_lin->coluna < col){
            *cel_ant_linha = cel_lin; //lembrando que antes era NULL
            cel_lin = cel_lin->proxima_linha;
        }
    }
    //em tese, já temos a posição, mas precisamos atualizar a coluna
    while(fil_coluna != NULL && fil_coluna->indice < col){
        *fil_ant_coluna = fil_coluna;
        
        //agora para continuar o while
        fil_coluna = fil_coluna->proximo;
        if(fil_coluna != NULL){
            cel_col = fil_coluna->primeiro;
        }
    }
    //por fim, atualizamos os anteriores para celula coluna, que dá na mesma posição
    if(fil_coluna != NULL && fil_coluna->indice == col){
        while(cel_col != NULL && cel_col->linha < lin) {
            *cel_ant_coluna = cel_col;
            cel_col = cel_col->proxima_coluna;
        }
    }
    
    if(cel_lin != NULL && (cel_lin->linha == lin && cel_lin->coluna == col)){
        return cel_lin; 
        //poderia ser cel_lin ou cel_col...dá na mesma!
    }
    return NULL;
}

int obter_valor(planilha_t *p, int linha, int coluna) { //atualizar
    // TODO: use buscarCelula; retorne 0 se a celula nao existir
    celula_t * cel_ant_linha;
    celula_t* cel_ant_coluna;
    fileira_t* fil_ant_linha;
    fileira_t* fil_ant_coluna;

    celula_t *cel = buscar_celula(p, linha, coluna, &cel_ant_linha, &cel_ant_coluna, &fil_ant_linha, &fil_ant_coluna);
    
    if(cel != NULL){
        return cel->valor; //aqui, (ANTES DE DEFINIR A CELULA) ainda não temos um valor
    } 
    return 0;
}

int somar_intervalo(planilha_t* p, int linha_ini, int linha_fim, int coluna_ini, int coluna_fim) { //conferir função! não eh a mais otimizada...
    // TODO: some os valores das celulas nao nulas no intervalo dado

    int soma = 0;
    int cel_valor;
   
    for(int i = linha_ini; i <= linha_fim; i++){
        for(int j = coluna_ini; j <= coluna_fim; j++){
            cel_valor = obter_valor(p, i, j);
            if(cel_valor != 0){
                soma += cel_valor;
            }
        }
    }

    if(soma != 0){
        return soma;
    }

    //poderia ser só return soma
    return 0; //assinatura...
}

//concertar no final!!!!!!!!!
int contar_nao_nulas(planilha_t* p) { //não eh a maneira mais otimizada... 
    // TODO: retorne a quantidade de celulas nao nulas
    int count = 0;
    int valor;
    fileira_t *lin = p->primeira_linha;
    fileira_t *col = p->primeira_coluna;
   
    for(fileira_t *i = lin; i != NULL; i = lin->proximo){
        for(fileira_t *j = col; j != NULL; j = col->proximo){
            valor = obter_valor(p, i->indice, j->indice); //de fato.
            if(valor != 0){
                count++;
            }
        }
    }
  
    if(count != 0){
        return count;
    }

    //poderia ser só return count...
    return 0; //assinatura
}

bool definir_celula(planilha_t* p, int lin, int col, int valor) {
    /* TODO: implemente os 4 casos (atualizar/criar/remover/nulo),
       empilhando em p->historico quando houver alteracao efetiva.
    Retorna true se houve alteracao, false se foi operacao nula. */

    celula_t * cel_ant_linha;
    celula_t* cel_ant_coluna;
    fileira_t* fil_ant_linha;
    fileira_t* fil_ant_coluna;

    //é como se eu fizesse um push aqui
    elo_pilha_t *hist_novo = (elo_pilha_t*) malloc(sizeof(elo_pilha_t));
    if(hist_novo == NULL) return false; //se malloc der ERRADO

    celula_t *cel_atual = (celula_t *) malloc(sizeof(celula_t));
    if(cel_atual == NULL) return false; //se malloc der ERRADO

    cel_atual = buscar_celula(p, lin, col, &cel_ant_linha, &cel_ant_coluna, &fil_ant_linha, &fil_ant_coluna);

    //ANTES: determinar o estado atual da célula
    if(cel_atual != NULL){
        if(valor != cel_atual->valor){ //para ser uma operação efeitiva
            hist_novo->op.valor_anterior = cel_atual->valor; //guardou o valor anterior
            hist_novo->proximo = p->historico.topo;
            p->historico.topo = hist_novo;
        }
    } else {
        hist_novo->op.valor_anterior = 0; //não existia
        hist_novo->proximo = p->historico.topo;
        p->historico.topo = hist_novo;
    }
    
    //casos!!
    if(cel_atual != NULL){
        if(valor != 0){
            cel_atual->valor = valor; //atulizando para A PRÓPRIA CELULA
        } else {

            if(cel_ant_linha != NULL){
                cel_ant_linha->proxima_linha = cel_atual->proxima_linha;
                if(cel_ant_coluna != NULL){
                    cel_ant_coluna->proxima_coluna = cel_atual->proxima_coluna;
                } else if(cel_ant_coluna == NULL && cel_atual->proxima_coluna != NULL){
                    fil_ant_coluna->proximo->primeiro = cel_atual->proxima_coluna;
                    //fil_coluna_primeiro = cel_atual->proxima_coluna;
                } else{ //SE EH O UNICO DA COLUNA
                    fil_ant_coluna->proximo = fil_ant_coluna->proximo->proximo;
                    //fil_ant_coluna->proximo = fil_coluna->proximo;
                }
            } else{
                if(cel_atual->proxima_linha != NULL){
                    fil_ant_linha->proximo->primeiro = cel_atual->proxima_linha;
                    //fil_linha->primeiro = cel_atual->proxima_linha;
                    if(cel_ant_coluna != NULL){
                        cel_ant_coluna->proxima_coluna = cel_atual->proxima_coluna;
                    } else if(cel_ant_coluna == NULL && cel_atual->proxima_coluna != NULL){
                        cel_ant_coluna->proxima_coluna = cel_atual->proxima_coluna;
                        fil_ant_coluna->proximo->primeiro = cel_atual->proxima_coluna; //conferir isso daqui!!!!!
                        cel_ant_linha->proxima_linha = cel_atual->proxima_linha;
                    } else {
                        if(fil_ant_coluna != NULL){
                            fil_ant_coluna->proximo = fil_ant_coluna->proximo->proximo; //APAGOU A COLUNA
                        } else {
                            p->primeira_coluna = p->primeira_coluna->proximo;
                        }
                    }
                } else {
                    if(fil_ant_linha != NULL){
                        fil_ant_linha->proximo = fil_ant_linha->proximo->proximo;
                        //fil_ant_linha->proximo = fil_linha->proximo;
                    } else {
                        p->primeira_linha = p->primeira_linha->proximo;
                    }

                    if(cel_ant_coluna != NULL){
                        cel_ant_coluna->proxima_coluna = cel_atual->proxima_coluna;
                    } else if(cel_ant_coluna == NULL && cel_atual->proxima_coluna != NULL){
                        fil_ant_coluna->proximo->primeiro = cel_atual->proxima_coluna;
                        //fil_coluna->primeiro = cel_atual->proxima_coluna;
                    } else {
                        if(fil_ant_coluna != NULL){
                            fil_ant_coluna->proximo = fil_ant_coluna->proximo->proximo;
                            //fil_ant_coluna->proximo = fil_coluna->proximo; //APAGOU A COLUNA
                        } else {
                            p->primeira_coluna = p->primeira_coluna->proximo;
                        }
                    }
                }
            }
            //libera no final
            free(cel_atual); 
        }
    } else {
        if(valor == 0){
            //faz nada
        } else if(valor != 0){
            celula_t *novo = malloc(sizeof(celula_t));
            novo->coluna = col;
            novo->linha = lin;
            novo->valor = valor;

            if(cel_ant_linha != NULL){
                novo->proxima_linha = cel_ant_linha->proxima_linha;
                cel_ant_linha->proxima_linha = novo; //até aqui tydo certo

                if(cel_ant_coluna != NULL){
                    novo->proxima_coluna = cel_ant_coluna->proxima_coluna;
                    cel_ant_coluna->proxima_coluna = novo; //certo
                } else if(cel_ant_coluna == NULL && fil_ant_coluna->proximo != NULL){ //se cel_ant_linha != NULL não está na primerira coluna
                    //logo tem uma coluna anterior
                    if(fil_ant_coluna->proximo->primeiro != NULL){
                        novo->proxima_coluna = fil_ant_coluna->proximo->primeiro;
                        //novo->proxima_coluna = fil_coluna_primeiro
                        fil_ant_coluna->proximo->primeiro = novo;
                        //fil_coluna_primeiro = novo;
                    }
                } else if(fil_ant_coluna->proximo == NULL){ //SE NAO HÁ NA COLUNA
                    fileira_t *col_nova = (fileira_t*) malloc(sizeof(fileira_t));
                    col_nova->indice = col;

                    fil_ant_coluna->proximo = col_nova; //(COMO FAZ ISOSOSOSOSOOOOOO??????)
                    col_nova->primeiro = novo;
                    novo->proxima_coluna = NULL;
                    //faltou inicializar o proximo da coluna
                    col_nova->proximo = NULL;
                }
            } else{
                if(fil_ant_linha != NULL){
                    if(fil_ant_linha->proximo != NULL){ // se ela existe, tem primeiro, se não existe, não tem primeiro
                        novo->proxima_linha = fil_ant_linha->proximo->primeiro;
                        fil_ant_linha->proximo->primeiro = novo;
                        //novo->proximo_linha = fil_linha->primeiro;
                        //fil_linha->primeiro = novo;
                        if(cel_ant_coluna != NULL){
                            novo->proxima_coluna = cel_ant_coluna->proxima_coluna;
                            cel_ant_coluna->proxima_coluna = novo;
                        } else if(cel_ant_coluna == NULL && fil_ant_coluna->proximo != NULL){ //se cel_ant_linha != NULL não está na primerira coluna
                            //logo tem uma coluna anterior
                            if(fil_ant_coluna->proximo->primeiro != NULL){
                                novo->proxima_coluna = fil_ant_coluna->proximo->primeiro;
                                //novo->proxima_coluna = fil_coluna_primeiro
                                fil_ant_coluna->proximo->primeiro = novo;
                                //fil_coluna_primeiro = novo;
                            }
                        } else if(fil_ant_coluna->proximo == NULL){
                            fileira_t *col_nova = (fileira_t*) malloc(sizeof(fileira_t));
                            col_nova->indice = col;

                            fil_ant_coluna->proximo = col_nova; //(COMO FAZ ISOSOSOSOSOOOOOO??????)
                            col_nova->primeiro = novo;
                            novo->proxima_coluna = NULL;
                            //faltou inicializar o proximo da coluna
                            col_nova->proximo = NULL;
                        }
                    } else {
                        fileira_t *lin_nova = (fileira_t*) malloc(sizeof(fileira_t));
                        lin_nova->indice = lin;
                        fil_ant_linha->proximo = lin_nova; //(COMO FAZ ISOSOSOSOSOOOOOO??????)
                        lin_nova->primeiro = novo;
                        novo->proxima_linha = NULL;
                        //faltou inicializar o proximo da linha
                        lin_nova->proximo = NULL;

                        if(cel_ant_coluna != NULL){
                            novo->proxima_coluna = cel_ant_coluna->proxima_coluna;
                            cel_ant_coluna->proxima_coluna = novo;
                        } else if(cel_ant_coluna == NULL && fil_ant_coluna->proximo->primeiro != NULL){
                            novo->proxima_coluna = fil_ant_coluna->proximo->primeiro;
                            fil_ant_coluna->proximo->primeiro = novo;
                            //fil_coluna->primeiro = novo; //CONFERIR!!!!!!!!!!!!
                            //novo->proximo_coluna = cel_atual->proximo_coluna;
                        } else {
                            fileira_t *col_nova = (fileira_t*) malloc(sizeof(fileira_t));
                            col_nova->indice = col;
                            
                            fil_ant_coluna->proximo = col_nova; //(COMO FAZ ISOSOSOSOSOOOOOO??????)
                            col_nova->primeiro = novo;
                            novo->proxima_coluna = NULL;
                            //faltou inicializar o proximo da coluna
                            col_nova->proximo = NULL;
                        }
                    }
                } else if(p->primeira_linha == NULL){
                    fileira_t *lin_nova = (fileira_t*) malloc(sizeof(fileira_t));
                    fileira_t *col_nova = (fileira_t*) malloc(sizeof(fileira_t));

                    lin_nova->indice = lin;
                    col_nova->indice = col;
                    p->primeira_linha = lin_nova;
                    p->primeira_coluna = col_nova;

                    lin_nova->proximo = NULL;
                    col_nova->proximo = NULL;

                    lin_nova->primeiro = novo;
                    col_nova->primeiro = novo;
                    
                    novo->proxima_linha = NULL;
                    novo->proxima_coluna = NULL;
                }
            }
        }
    }

    if(valor != hist_novo->op.valor_anterior){
        return true;
    }
    return false;
}

bool remover_celula(planilha_t* p, int lin, int col) {
    // TODO: remova a celula (lin,col)

    //quero remover uma célula de uma certa linha e coluna
    //como eu a acho? buscando!
    celula_t * cel_ant_linha;
    celula_t* cel_ant_coluna;
    fileira_t* fil_ant_linha;
    fileira_t* fil_ant_coluna;

    //vou achar a exata celula
    celula_t *cel_atual = buscar_celula(p, lin, col, &cel_ant_linha, &cel_ant_coluna, &fil_ant_linha, &fil_ant_coluna);

    //vamos excluir agora!!
    if(cel_ant_linha != NULL){
        cel_ant_linha->proxima_linha = cel_atual->proxima_linha;
        if(cel_ant_coluna != NULL){
            cel_ant_coluna->proxima_coluna = cel_atual->proxima_coluna;
        } else if(cel_ant_coluna == NULL && cel_atual->proxima_coluna != NULL){
            fil_ant_coluna->proximo->primeiro = cel_atual->proxima_coluna;
            //fil_coluna_primeiro = cel_atual->proxima_coluna;
        } else{ //SE EH O UNICO DA COLUNA
            fil_ant_coluna->proximo = fil_ant_coluna->proximo->proximo;
            //fil_ant_coluna->proximo = fil_coluna->proximo;
        }
    } else{
        if(cel_atual->proxima_linha != NULL){
            fil_ant_linha->proximo->primeiro = cel_atual->proxima_linha;
            //fil_linha->primeiro = cel_atual->proxima_linha;
            if(cel_ant_coluna != NULL){
                cel_ant_coluna->proxima_coluna = cel_atual->proxima_coluna;
            } else if(cel_ant_coluna == NULL && cel_atual->proxima_coluna != NULL){
                cel_ant_coluna->proxima_coluna = cel_atual->proxima_coluna;
                fil_ant_coluna->proximo->primeiro = cel_atual->proxima_coluna; //conferir isso daqui!!!!!
                cel_ant_linha->proxima_linha = cel_atual->proxima_linha;
            } else {
                if(fil_ant_coluna != NULL){
                    fil_ant_coluna->proximo = fil_ant_coluna->proximo->proximo; //APAGOU A COLUNA
                } else {
                    p->primeira_coluna = p->primeira_coluna->proximo;
                }
            }
        } else {
            if(fil_ant_linha != NULL){
                fil_ant_linha->proximo = fil_ant_linha->proximo->proximo;
                //fil_ant_linha->proximo = fil_linha->proximo;
            } else {
                p->primeira_linha = p->primeira_linha->proximo;
            }

            if(cel_ant_coluna != NULL){
                cel_ant_coluna->proxima_coluna = cel_atual->proxima_coluna;
            } else if(cel_ant_coluna == NULL && cel_atual->proxima_coluna != NULL){
                fil_ant_coluna->proximo->primeiro = cel_atual->proxima_coluna;
                //fil_coluna->primeiro = cel_atual->proximo_coluna;
            } else {
                if(fil_ant_coluna != NULL){
                    fil_ant_coluna->proximo = fil_ant_coluna->proximo->proximo;
                    //fil_ant_coluna->proximo = fil_coluna->proximo; //APAGOU A COLUNA
                } else {
                    p->primeira_coluna = p->primeira_coluna->proximo;
                }
            }
        }
    }

    //libera no final, eu já a criei...não preciso de malloc de novo
    free(cel_atual);
    //calma q essa parte é só pro historico
    if(cel_atual != NULL){
        elo_pilha_t *hist = (elo_pilha_t *) malloc(sizeof(elo_pilha_t));
        hist->op.linha = lin;
        hist->op.coluna = col;
        hist->proximo = p->historico.topo;
        p->historico.topo = hist;
        return true;
    }
    return false;
}

bool transpor(planilha_t* p, int lin, int col, int tamanho) {
    /* TODO: transpoe uma matriz quadrada que está localizada entre
    as linhas [lin, lin + tamanho) e colunas [col, col + tamanho). */
    if(lin+tamanho >= 10000000000 || col+tamanho >= 10000000000){
        //função faz nada e retorna falso
        return false;
    }

    celula_t * cel_ant_linha;
    celula_t* cel_ant_coluna;
    fileira_t* fil_ant_linha;
    fileira_t* fil_ant_coluna;

    planilha_t *p_nova = p;

    //a celula que ele vai pegar vai ser a respectiva da lin e col indicada
    celula_t *atual = buscar_celula(p_nova, lin, col, &cel_ant_linha, &cel_ant_coluna, &fil_ant_linha, &fil_ant_coluna);
    celula_t *aux_linha = (celula_t *) malloc(sizeof(celula_t));
    celula_t *aux_coluna = (celula_t *) malloc(sizeof(celula_t));
    int cont = 0;

    fileira_t *j = fil_ant_coluna->proximo;
    fileira_t *i = fil_ant_linha->proximo;
    while((i->indice <= lin+tamanho && i != NULL) || (j->indice <= col+tamanho && j != NULL)){ //que não seja uma fileira que não exista
        aux_coluna = atual->proxima_coluna;
        aux_linha = atual->proxima_linha;
        //verificação
        if(aux_coluna == NULL || aux_coluna->coluna > col+tamanho || aux_linha == NULL || aux_linha->linha > lin+tamanho) break;

        atual->proxima_coluna = atual->proxima_linha;
        atual->proxima_linha = aux_coluna;
        cont++;
        aux_coluna = aux_coluna->proxima_coluna;
        aux_linha = aux_linha->proxima_linha;
        //verificação
        if(aux_coluna == NULL || aux_coluna->coluna > col+tamanho || aux_linha == NULL || aux_linha->linha > lin+tamanho) break;

        atual = atual->proxima_coluna;
        atual->proxima_coluna = aux_linha;
        aux_linha = aux_coluna;

        atual = atual->proxima_linha;

        //da a continuação para o while
        j = fil_ant_coluna->proximo->proximo;
        i = fil_ant_linha->proximo->proximo;
    } 

    if(cont >= 1){ //vamos adicionar na pilha
        elo_pilha_t *hist_novo = (elo_pilha_t*) malloc(sizeof(elo_pilha_t));
        if(hist_novo == NULL) return false; //se malloc der ERRADO
        hist_novo->op.transposicao = true;
        hist_novo->op.linha = lin;
        hist_novo->op.coluna = col;
        hist_novo->op.tamanho = tamanho;

        hist_novo->proximo = p->historico.topo;
        p->historico.topo = hist_novo;

        return true;
    }

    return false; //deixo ou não...
}

bool desfazer(planilha_t* p) {
    /* TODO: desempilhe de p->historico e restaure o valor anterior.
    Retorna false se o historico estiver vazio, true caso contrario. */

    elo_pilha_t *end = p->historico.topo;
    //celula_t *cel;
    //só para salvar
    //celula_t * cel_ant_linha;
    //celula_t* cel_ant_coluna;
    //fileira_t* fil_ant_linha;
    //fileira_t* fil_ant_coluna;

    int lin, col;
    int valor_ant;
    int tamanho;

    //em caso de celula
    lin = end->op.linha;
    col = end->op.coluna;
    if(end->op.transposicao == false){
        valor_ant = end->op.valor_anterior;

        //cel = buscar_celula(p, lin, col, &cel_ant_linha, &cel_ant_coluna, &fil_ant_linha, &fil_ant_coluna);
        //cel->valor = valor_ant; //o que eu faço quando é igual a 0?????
        definir_celula(p, lin, col, valor_ant);
        //tenho que a remover
        //uso remover_celula ou definir_celula desde o início???
    } else { //houve transposição
        tamanho = end->op.tamanho;
        transpor(p, lin, col, tamanho);
    }

    //não tenho que apagar as novas adições tbm????
    elo_pilha_t *apagar_novo = p->historico.topo;
    p->historico.topo = end;
    free(apagar_novo); //apaguei as edições novas agora!
    elo_pilha_t *apagar = end;
    end = end->proximo; //passa o topo para o outro
    free(apagar);

    if(p->historico.topo != NULL) return true;
    return false;
}

void exibir_planilha(planilha_t *p) {
    /* TODO: imprima "linha coluna valor" por linha, em ordem crescente
       de linha e, dentro de cada linha, de coluna. Se vazia, imprima
       "PLANILHA VAZIA" */
    if(p->primeira_linha == NULL || p->primeira_coluna == NULL) printf("PLANILHA VAZIA\n");
    
    //se não...
    int cel_valor;
    for(fileira_t *i = p->primeira_linha; i != NULL; i = p->primeira_linha->proximo){
        for(fileira_t *j = p->primeira_coluna; i != NULL; i = p->primeira_coluna->proximo){
            cel_valor = obter_valor(p, i->indice, j->indice);
            if(cel_valor != 0){
                printf("linha: %d, coluna: %d, valor: %d\n", i->indice, j->indice, cel_valor);
            }
        }
    }
    printf("\n");
}

void exibir_historico(planilha_t* p) {
    /* TODO: imprima "linha coluna valor_anterior" por linha, do topo
    para a base. Se vazio, imprima "HISTORICO VAZIO" */
    if(p->historico.topo == NULL) printf("HISTORICO VAZIO\n");

    //se não...
    elo_pilha_t *end = p->historico.topo; //eh ponteiro
    while(end != NULL){
        printf("linha: %d, coluna: %d, valor_anterior: %d\n", end->op.linha, end->op.coluna, end->op.valor_anterior);
        end = end->proximo;
    }
    printf("\n");
}

void liberar_tudo(planilha_t* p) {
    // TODO: libere toda a memória alocada por fileira, celula, e pilha.

    //primeiro vamos liberar o historico;
    elo_pilha_t *end = p->historico.topo;
    while(end != NULL){
        elo_pilha_t *apagar = p->historico.topo;
        end = end->proximo;
        free(apagar);
    }
    //vamos liberar agora as fileiras
    //depois liberar células

    fileira_t *lin = p->primeira_linha;
    fileira_t *col = p->primeira_coluna;
    celula_t *cel = p->primeira_linha->primeiro; //não precisa fazer um para colunas
    //se uma celula tem uma coluna ela também tem uma linha, logo, eventualmente chegarei nela!
    while(lin != NULL){
        fileira_t *apagar_col = col;
        celula_t *cel_prox_col = cel->proxima_coluna;
        celula_t *apagar_cel = cel;
        col = col->proximo;
        cel = cel->proxima_linha;
        free(apagar_cel);

        if(col == NULL && lin != NULL){
            col = p->primeira_coluna;
        } else if(col->primeiro == NULL && cel_prox_col == NULL){
            free(apagar_col);
        }

        if(cel == NULL){
            fileira_t *apagar_lin = lin;
            lin = lin->proximo;
            cel = lin->primeiro;
            free(apagar_lin);
        }
    }

    //vamos reiniciar os campos da planilha para representar vazia
    p->primeira_linha = NULL; //eh um ponteiro
    p->primeira_coluna = NULL; //eh um ponteiro
    p->total_celulas = 0; //int
    p->historico.topo = NULL; //eh ponteiro

}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso do comando eh: %s arquivo_entrada.txt arquivo_saida.txt\n", argv[0]);
        return 1;
    }
    
    FILE* entrada = fopen(argv[1], "r");
    FILE* saida = freopen(argv[2], "w", stdout);

    if (!entrada || !saida) {
        fprintf(stderr, "Erro ao tentar abrir os arquivos.\n");
        return 1;
    }

    planilha_t p;
    inicializar_planilha(&p);

    int n;
    fscanf(entrada, "%d", &n);

    char cmd[20];

    while (fscanf(entrada, "%s", cmd) != EOF) {
 
        if (igual(cmd, "DEF")) {
            int lin, col, valor;
            fscanf(entrada, "%d %d %d", &lin, &col, &valor);
            definir_celula(&p, lin, col, valor);
        } else if (igual(cmd, "REM")) {
            int lin, col;
            fscanf(entrada, "%d %d", &lin, &col);
            remover_celula(&p, lin, col);
        } else if (igual(cmd, "GET")) {
            int lin, col;
            fscanf(entrada, "%d %d", &lin, &col);
            fprintf(saida, "GET %d %d %d\n", lin, col, obter_valor(&p, lin, col));
        } else if (igual(cmd, "SOMA")) {
            int li, lf, ci, cf;
            fscanf(entrada, "%d %d %d %d", &li, &lf, &ci, &cf);
            fprintf(saida, "SOMA %d %d %d %d %d\n", li, lf, ci, cf, somar_intervalo(&p, li, lf, ci, cf));
        } else if (igual(cmd, "CONT")) {
            fprintf(saida, "CONT %d\n", contar_nao_nulas(&p));
        } else if (igual(cmd, "DESFAZER")) {
            if (!desfazer(&p)) {
                fprintf(saida, "HISTORICO VAZIO\n");
            }
        } else if (igual(cmd, "EXIBIR")) {
            if (contar_nao_nulas(&p)) {
                printf("PLANILHA\n");
                exibir_planilha(&p);
            } else {
                printf("PLANILHA VAZIA\n");
            }
        } else if (igual(cmd, "HIST")) {
            if (p.historico.topo) {
                printf("HISTORICO\n");
                exibir_historico(&p);
            }
            else {
                printf("HISTORICO VAZIO\n");
            }
        } else if (igual(cmd, "TRANS")) {
            int lin, col, tam;
            fscanf(entrada, "%d %d %d", &lin, &col, &tam);
            transpor(&p, lin, col, tam);
        }
    }

    fclose(entrada);
    fclose(saida);

    liberar_tudo(&p);
    return 0;
}