#include <stdio.h>
#include <stdlib.h>

//Cria uma struct para informações de uma disciplina
typedef struct disciplina{    
    int id;
    char *nome;
    double notaFinal;
}DI;

//Cria uma struct para informações pessoais do aluno
typedef struct no{          
    int count;
    int chave;
    int idade;
    char *nome;
    DI *disciplinas[10]; //Vetor, onde cada célula aponta para uma struct "Disciplina"
    struct no *esq;
    struct no *dir;
}NO;

//Cria uma variável que armazena o endereço do primeiro nó da ABB
NO* raiz = NULL;            

//busca nó referente à informação passada como parâmetro, que nesse caso será chave do aluno(matrícula)
NO* busca(int x, NO* aux){  
    if(aux == NULL){ //verifica se a raíz é nula, ou seja, se a árvore é vazia 
        return NULL; 
    }else if(x == aux->chave){//Caso não seja vazia, verifica se a informação passada como parâmetro é a mesma no nó
        return aux; 
    }else if(x<aux->chave){ //Caso o valor procurado seja menor que a do nó, será feita uma chamada recursiva passando o nó da esquerda como parâmetro
        if(aux->esq != NULL){ // verifica se existe nó à esquerda
            return busca(x, aux->esq);
        }else{
            return aux; //Caso não exista e o valor ainda não foi encontrado, ele retorna o nó que deverá anteceder o valor desejado(nó pai)
        }
    }else{// Caso o valor procurado seja maior que o do nó, a chamada recursiva será feita, dessa vez passando o nó à direita como parâmetro
        if(aux->dir != NULL){// verifica se exíste nó à direita
            return busca(x, aux->dir);
        }else{//dir esta vazia
            return aux; // retorna o nó que deverá preceder o valor desejado (pai) 
        }
    }
}

void add(int chave, char *nomeAluno, int idade){
    NO* resp = busca(chave, raiz);  //Busca aluno 
    NO *novo = malloc(sizeof(NO));
    novo->chave = chave;
    novo->nome = nomeAluno;
    novo->idade = idade;
    novo->count = 0; // conta quantas disciplinas tem no vetor 
    novo->esq = NULL;
    novo->dir = NULL;
        
    if(resp == NULL){ 
        raiz = novo;
    }else{
        if(chave < resp->chave){
            resp->esq = novo;
        }else{
            resp->dir = novo;
        }
    }
}

void add_disciplinas(int matricula, int codigo, char *nome, float nota){//função recebe como parâmetros os dados pessoais e a matrícula, que funciona como chave identificadora do aluno
    NO *aluno = busca(matricula, raiz);           
    if(aluno->count<10){
        DI *novo = malloc(sizeof(DI));
        novo->id = codigo;
        novo->nome =  nome;
        novo->notaFinal = nota;
        aluno->disciplinas[aluno->count] = novo;
        aluno->count++;
    }
}

void alterar(int matricula){
    NO* aux = busca(matricula, raiz);
    int escolha;
    printf("O que deseja alterar?\n[1] - Idade\n[2] - Nome\nEscolher: ");
    scanf("%d", &escolha);
    if(escolha == 1){
        printf("Digite a nova idade: ");
        scanf("%d", &aux->idade);
    }else if(escolha == 2){
        printf("Digite o novo nome: ");
        aux->nome = malloc (sizeof(char)*100);//Aloca memória sempre que receber novo valor como parâmetro.
        scanf("%s", aux->nome);
        printf("Alteracao concluida \n");
    }else{
        printf("Opção Invalida.");
    }
}

NO* remover(NO *raiz, int chave){
    if(raiz == NULL){ 
        printf("Aluno nao encontrado\n");
        return 0;
    }   
    else{ 
        if(raiz->chave == chave){ 
            if(raiz->esq == NULL && raiz->dir == NULL){  
                free(raiz);
                return NULL; 
            }else{ 
                if(raiz->dir == NULL || raiz->esq == NULL){
                    NO *aux;
                    if(raiz->esq != NULL){     
                        aux = raiz->esq;       
                    }else{                          
                        aux = raiz->dir;
                    }
                    free(raiz);  
                    return aux;  
                }else{
                    NO *aux = raiz->esq;
                    while(aux->dir != NULL){
                        aux = aux->dir;
                    }
                    raiz->chave = aux->chave;
                    aux->chave = chave;   
                    raiz->esq = remover(raiz->esq, chave);  
                    return raiz;
                }
            }
        }
        else{ 
            if(chave < raiz->chave){ 
                raiz->esq = remover(raiz->esq, chave);  
            }else{
                raiz->dir = remover(raiz->dir, chave);
            }
            return raiz;
        }
    }
}

void ordenar(int matricula){
    DI *aux2;
    NO* aux = busca(matricula, raiz);
    if(aux == NULL || aux->chave != matricula){
        printf("\nAluno nao encontrado\n");
        return;
    }
    for (int i = 0; i < aux->count; i++) {
        float maior = aux->disciplinas[i]->notaFinal;
        int pos = i;

        for (int j = i; j < aux->count; j++) {
            if (maior < aux->disciplinas[j]->notaFinal){
                maior = aux->disciplinas[j]->notaFinal;
                pos = j;
            }
        }  
        aux2 = aux->disciplinas[i];
        aux->disciplinas[i] = aux->disciplinas[pos];
        aux->disciplinas[pos] = aux2;
    }
}

void buscarAluno(int matricula){//Busca aluno apartir da matricula e caso ele esteja cadastrado exíbe suas informações, assim como das suas disciplinas
    DI *aux2;
    NO* aux = busca(matricula, raiz);
    if(aux == NULL || aux->chave != matricula){
        printf("Aluno nao encontrado.\n");
        return;
    }
    ordenar(matricula);
    printf("Nome: %s\n", aux->nome);
    printf("Idade: %d\n", aux->idade);

    for(int i = 0; i < aux->count; i++){
        aux2 = aux->disciplinas[i];
        printf("Disciplina %d:\n", i + 1);
        printf("Codigo: %d\n", aux2->id);
        printf("Nome: %s\n", aux2->nome);
        printf("Nota Final: %.2f\n", aux2->notaFinal);
    }
}



void imprimirInOrdem(NO *raiz){//Percorre a ABB imprimindo informações de todos de forma ordenada.
    if(raiz != NULL){
        imprimirInOrdem(raiz->esq);
        printf("[%s] ", raiz->nome);     
        printf("[%d] ", raiz->chave);   
        printf("[%d]\n", raiz->idade);
        imprimirInOrdem(raiz->dir);       
    }
}
    
int main(){
    int opcao, matricula, idade, choose, quant, codigo;
    float nota;
    char *nome, *nomeDis;
    NO *aluno;
    //Adicionamos 10 alunos 
    add(1, "Vitor", 20);
    add(2, "Daniel", 21);
    add(3, "Lydiana", 21);
    add(4, "Rafael", 19);
    add(5, "Kawan", 19);
    add(6, "Caio", 22);
    add(7, "Paulo", 21);
    add(8, "Hernestiana", 20);
    add(9, "Eduarda", 20);
    add(10, "Isabelle", 20);//Adicionamos 10 disciplinas no último aluno apenas, por ser o caso mais extremo.
    add_disciplinas(10, 11, "Estrutura de dados", 4.3);
    add_disciplinas(10, 12, "Matemática Discreta", 7.5);
    add_disciplinas(10, 13, "Arquitetura de computadores", 8.0);
    add_disciplinas(10, 14, "Algebra linear", 7.0);
    add_disciplinas(10, 15, "Bancos de dados", 4.5);
    add_disciplinas(10, 16, "Probabilidade e estatística", 9.87);
    add_disciplinas(10, 17, "Grafos", 10.0);
    add_disciplinas(10, 18, "PAA", 5.0);
    add_disciplinas(10, 19, "Eng. de Softwere", 4.0);
    add_disciplinas(10, 20, "Empeendedorismo", 3.0);

    do{
        printf("====================\n Binary Tree\n====================\n[1] - Adicionar\n[2] - Remover\n[3] - Alterar\n[4] - Buscar\n[5]- Imprimir\n[6] - Adicionar disciplina\n[0] - Sair\nEscolha: ");
        scanf("%d", &opcao);
        switch (opcao){
        case 1:
            printf("---------------------\n");
            nome = malloc(sizeof(char)*100);
            printf("Nova Matricula: ");
            scanf("%d", &matricula);
            aluno = busca(matricula, raiz);
            if(aluno == NULL || aluno->chave == matricula){
                printf("Matricula ja existe.\n");
            }else{
                printf("Nome do Aluno: ");
                scanf("%s", nome);
                printf("Idade: ");
                scanf("%d", &idade);
                add(matricula, nome, idade);
                printf("[*] Aluno Adicinado!\n");
                printf("Deseja adicionar disciplinas do aluno?\n[1] - Sim\n[2] - Nao\nEscolha: ");
                scanf("%d", &choose);
                if(choose == 1){
                    printf("Quantas deseja adicionar: ");
                    scanf("%d", &quant);
                    for(int i = 0; i < quant; i++){
                        printf("---------------------\n");
                        nomeDis = malloc(sizeof(char)*100);
                        printf("Nome da disciplina: ");
                        scanf("%s", nomeDis);
                        printf("Codigo da disciplina: ");
                        scanf("%d", &codigo);
                        printf("Nota Final: ");
                        scanf("%f", &nota);
                        add_disciplinas(matricula, codigo, nomeDis, nota);
                    }
                    printf("[*] Tudo Feito!\n");
                }
            }
            break;
        case 2:
            printf("---------------------\n");
            printf("Matricula do Aluno: ");
            scanf("%d", &matricula);
            raiz = remover(raiz, matricula);
            printf("Operação concluida !");
            break;
        case 3:
            printf("Matricula do Aluno: ");
            scanf("%d", &matricula);
            alterar(matricula);
            break;
        case 4://Retorna informações de um aluno expecifico.
            printf("Matricula do Aluno: ");
            scanf("%d", &matricula);
            buscarAluno(matricula);
            break; 
        case 5:
            printf("---------------------\n");
            imprimirInOrdem(raiz);
            break;
        case 6://Permite que o usuário insira informações de uma disciplina para um usuário.
            printf("---------------------\n");
            printf("Matricula: ");
            scanf("%d", &matricula);
            NO *aluno = busca(matricula, raiz);
            if(aluno == NULL || aluno->chave != matricula){
                printf("Aluno nao encontrado\n");
            }else{
                nomeDis = malloc(sizeof(char)*100);
                printf("Nome da disciplina: ");
                scanf("%s", nomeDis);
                printf("Codigo da disciplina: ");
                scanf("%d", &codigo);
                printf("Nota Final: ");
                scanf("%f", &nota);
                add_disciplinas(matricula, codigo, nomeDis, nota);
            }
            break;
        case 0:
            printf("Operacao finalizada pelo usuario...");
            break;
        default:
            printf("Opcao Invalida\n");
        }
    }while(opcao != 0);
    return 0;
}
