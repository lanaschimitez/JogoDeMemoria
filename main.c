#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include <time.h>

typedef struct
{
    char snomeJogador[10];
    int spontuacao;
} auxRanking;

typedef struct
{
    char user[10];
    char senha[10];
} login;

typedef struct
{
    char textoResposta[100];
    int verdade;
    int indiceResp;
    int localizacao;
} auxResposta;

typedef struct
{
    char textoPergunta[200];
    int indicePergunta;
} auxPergunta;

typedef struct
{
    auxPergunta pergunta;
    auxResposta resposta1;
    auxResposta resposta2;
    auxResposta resposta3;
} questoes;

void jogabilidade(int *pontos, char confUser[10]); //tudo certo
void gravarRanking(char confUser[10], int *pontuacaoRanking); //tudo certo
void lerRanking(int *auxPontos); //tudo certo
void mostrarTodasPergResp(); // LER TODAS AS QUESTOES - PARA EDICAO POSTERIORMENTE -tudo certo
void lerIndicePergunta(int *auxIndicePerg) ;//LER INDICE PARA ANEXAR NA PERGUNTA
void criaIndicePergunta(int *auxIndicePerg); //INDICE DA PERGUNTA QUE IRA FICAR EM UM ARQUIVO SEPARADO
void textcolor (int color);
void cabecalho();
void verificarLogin(char confSenha[10], char confUser[10], int *auxLogin, char adm[]);  //verificar a login da pessoa tudo certo
void menuUser(int *selecaoInicial, int *auxIndicePerg, char confUser[10]);  //menu do usuario
void menuAdmin(int *selecaoInicial, int *auxIndicePerg); //configurar opcaoes
void criarLogin(char confSenha[10],char confUser[10], char adm[], int *auxLogin ) ; //criar e editar login tudo certo
void menuLogin(char confSenha[10],char confUser[10], char adm[], int *auxLogin );  //menu para criar login tudo certo
void areaLogin(char confSenha[10], char confUser[10], char adm[], int *auxLogin);  //colocar a senha e o login do usuario tudo certo
void criaPergunta(int *auxIndicePerg);  //CRIA TODA A QUESTAO DO JOGO tudo certo
void loginsAdmin();
void zerarRanking();
void visualizarRanking();
void apagarPergunta();
void editarPergunta();
void primeiroLogin();
void reset();
void apagarQuestoes();
void respostaCorreta();
void respostaIncorreta();
void recorde();
void extras();

int main()
{
    setlocale(LC_ALL, "portuguese");
    textcolor(13);
    int selecaoInicial, auxLogin=0, auxIndicePerg=0;
    char confSenha[10], confUser[10], adm[]="ADMIN";
    FILE* arquivo;
    arquivo= fopen("Perfil.qz", "rb");
    if(arquivo==NULL)
    {
        primeiroLogin(confSenha, confUser, adm, &auxLogin);
    }
    else
    {
        menuLogin(confSenha, confUser, adm, &auxLogin);
    }
    if (auxLogin==1)
    {
        do
        {
            selecaoInicial='\0';
            menuUser(&selecaoInicial, &auxIndicePerg, confUser);
        }
        while(selecaoInicial!=4);
    }
    else if (auxLogin==2)
    {
        do
        {
            menuAdmin(&selecaoInicial, &auxIndicePerg);
            auxIndicePerg='\0';
        }
        while (selecaoInicial!=9);
    }
    else if(auxLogin==0)
    {
        printf("\n\nSenha incorreta, o jogo será encerrado\n\n");
    }
}
void jogabilidade(int *pontos, char confUser[10])
{
    int aux, alea, auxPontos, cor=1;
    *pontos=0;
    lerRanking(&auxPontos);
    FILE* arqQuestoes;
    questoes i;
    arqQuestoes=fopen("questoes.qz","rb");
    if (arqQuestoes==NULL)
    {
        printf("Nenhuma questão criada! Entre como administrador e crie suas questões");
    }

    else
    {
        for (;;)
        {
            if (cor>15)
            {
                cor=1;
            }
            if (*pontos<0)
            {
                *pontos=0;
            }
            textcolor(cor);
            cor++;
            system("cls");
            fread(&i, sizeof(questoes), 1, arqQuestoes);
            if(feof(arqQuestoes))
            {
                break;
            }
            srand(time(NULL));
            alea=rand()%3;
            i.resposta1.localizacao=alea;
            do
            {
                alea=rand()%3;
            }
            while(alea==i.resposta1.localizacao);
            i.resposta2.localizacao=alea;
            do
            {
                alea=rand()%3;
            }
            while(alea==i.resposta1.localizacao || alea==i.resposta2.localizacao);
            i.resposta3.localizacao=alea;
            printf("* %s *", i.pergunta.textoPergunta);
            for (int x=0; x<3; x++)
            {
                if (i.resposta1.localizacao==x)
                {
                    printf("\n\n%i- %s",x+1, i.resposta1.textoResposta);
                }
                else if (i.resposta2.localizacao==x)
                {
                    printf("\n\n%i- %s",x+1, i.resposta2.textoResposta);
                }
                else if (i.resposta3.localizacao==x)
                {
                    printf("\n\n%i- %s",x+1, i.resposta3.textoResposta);
                }
            }
            printf("\n\n");
            printf("Digite a opcao correta: ");
            fflush(stdin);
            do
            {
                scanf("%i", &aux);
                if (aux!=1 && aux!=2 && aux!=3)
                {
                    printf("\nOpção invalida, tente novamente: ");
                }
            }
            while(aux!=1 && aux!=2 && aux!=3);
            aux--;
            if (aux==i.resposta1.localizacao)
            {
                if (i.resposta1.verdade==1)
                {
                    *pontos= *pontos+1;
                    printf("\nSua pontuação atual %i\n\n", *pontos);
                    respostaCorreta();
                }
                else
                {
                    respostaIncorreta();
                }
            }
            if (aux==i.resposta2.localizacao)
            {
                if (i.resposta2.verdade==1)
                {
                    *pontos= *pontos+1;
                    printf("\nSua pontuação atual %i\n\n", *pontos);
                    respostaCorreta();
                }
                else
                {
                    respostaIncorreta();
                }
            }
            if (aux==i.resposta3.localizacao)
            {
                if (i.resposta3.verdade==1)
                {

                    *pontos= *pontos+1;
                    printf("\nSua pontuação atual %i\n\n", *pontos);
                    respostaCorreta();
                }
                else
                {
                    respostaIncorreta();

                }
            }
            system("pause");
        }
        if (auxPontos<*pontos)
        {
            gravarRanking(confUser, pontos);
            recorde();
        }
    }
    fclose(arqQuestoes);
    textcolor(14);
    printf("                ---------------------------  \n");
    printf("               |                           |    \n");
    printf("               |     OBRIGADO POR JOGAR    |\n");
    printf("               |                           |\n");
    printf("                ---------------------------  \n");
    system("pause");
    extras();
}

void recorde()
{
    textcolor(9);
    printf("               *       *       *        *\n");
    printf("            *   ---------------------------  *\n");
    printf("               |                           |    *\n");
    printf("              *|       NOVO RECORDE        |*\n");
    printf("         *     |                           |\n");
    printf("             *  ---------------------------  *\n");
    printf("                  *	     *         *  \n");
}

void respostaCorreta()
{

    textcolor(9);
    printf("	                *            \n ");
    printf("   *                             * \n");
    printf("          *          *              *\n ");
    printf("                                        *  \n ");
    printf("     *            *           *\n");
    printf("           *             *          *\n");
    printf("          ----------------               *\n");
    printf("   *     |RESPOSTA CORRETA|    *\n");
    printf("          ----------------            *\n");
    printf("               *           *\n");
    printf("        *             *\n");
    printf("*           *                    *\n");
    printf("               *           \n ");
    printf("                        *        *\n");
    printf("     *             *                    *  \n ");
    printf("           *              * \n");
}

void respostaIncorreta()
{

    textcolor(9);
    printf(".	     .           .         .   \n ");
    printf("   .     .                        . \n");
    printf("          .     .     .       .       .\n ");
    printf(" .                       .                .  \n ");
    printf("     .            .           .\n");
    printf("           .             .          .\n");
    printf("          ---------------               .\n");
    printf("   .     |RESPOSTA ERRADA|    .\n");
    printf("          ---------------            .\n");
    printf("      .         .           .       .\n");
    printf("                     .\n");
    printf(".           .                    .\n");
    printf("    .           .        ..   \n ");
    printf("           .             .        .\n");
    printf("     .             .                    .  \n ");
    printf("           .              . \n");
}

void apagarQuestoes()
{
    int aux=0;
    printf("\nDeseja remover todas as questões? 1-NÃO 2-SIM    ");
    scanf("%i", &aux);
    if (aux==2)
    {
        remove("questoes.qz");
        remove("IndicePergunta.id");
    }
    else
    {
        printf("\nRetorno ao menu do administrador\n\n");
    }
}

void gravarRanking(char confUser[10], int *pontuacaoRanking)
{
    auxRanking r;
    strcpy(r.snomeJogador, confUser);
    r.spontuacao= *pontuacaoRanking;
    FILE* arqRanking;
    arqRanking=fopen("Ranking.pont","wb");
    if (arqRanking==NULL)
    {
        printf("\nOcorreu um erro");
    }
    else
    {
        fwrite(&r, sizeof(auxRanking),1, arqRanking);
    }
    fclose(arqRanking);
}
void lerRanking(int *auxPontos)
{
    auxRanking r;
    FILE* arqRanking;
    arqRanking=fopen("Ranking.pont","rb");
    if (arqRanking==NULL)
    {
        printf("\nOcorreu um erro");
    }
    else
    {
        fread(&r, sizeof(auxRanking),1, arqRanking);
    }
    fclose(arqRanking);
    *auxPontos=r.spontuacao;
}
void mostrarTodasPergResp() //TUDO CERTO
{
    system("cls");
    printf("Questões:\n");
    FILE* arqIndice;
    questoes i;
    arqIndice=fopen("questoes.qz","rb");
    if (arqIndice==NULL)
    {
        printf("Ocorreu um erro");
    }
    else
    {
        for(;;)
        {
            fread(&i, sizeof(questoes), 1, arqIndice);
            if(feof(arqIndice))
            {
                break;
            }
            textcolor(12);
            printf("\n%i- ", i.pergunta.indicePergunta);
            printf(" %s", i.pergunta.textoPergunta);
            textcolor(11);
            printf("\n%s", i.resposta1.textoResposta);
            printf(" %i", i.resposta1.verdade);
            printf("\n%s", i.resposta2.textoResposta);
            printf(" %i", i.resposta2.verdade);
            printf("\n%s", i.resposta3.textoResposta);
            printf(" %i", i.resposta3.verdade);
            printf("\n0-resposta errada e 1-resposta correta");
            printf("\n\n");
        }
    }
    fclose(arqIndice);
}

void lerIndicePergunta(int *auxIndicePerg) //LER INDICE PARA ANEXAR NA PERGUNTA
{
    int x=0;
    FILE* arqIndice;
    arqIndice=fopen("IndicePergunta.id","rb");
    if (arqIndice==NULL)
    {
    }
    else
    {
        fread(&x, sizeof(int), 1, arqIndice);
    }
    fclose(arqIndice);
    *auxIndicePerg=x;
}

void criaIndicePergunta(int *auxIndicePerg) //INDICE DA PERGUNTA QUE IRA FICAR EM UM ARQUIVO SEPARADO
{
    int x;
    x= *auxIndicePerg;
    FILE* arqIndice;
    arqIndice=fopen("IndicePergunta.id","wb");
    if (arqIndice==NULL)
    {
        printf("Ocorreu um erro");
    }
    else
    {
        x= x+1;
        fwrite(&x, sizeof(int), 1, arqIndice);
    }
    fclose(arqIndice);
}

void textcolor (int color)
{
    static int __BACKGROUND;

    HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;


    GetConsoleScreenBufferInfo(h, &csbiInfo);

    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE),
                             color + (__BACKGROUND << 4));
}
void cabecalho()
{
    printf("###################################### \n");
    printf("#####   RESPONDA O QUIZ QUISER   ##### \n");
    printf("###################################### \n\n");
}

void verificarLogin(char confSenha[10], char confUser[10], int *auxLogin, char adm[]) //verificar a login da pessoa
{
    FILE* arqLogin;
    login c;
    arqLogin = fopen("Perfil.qz", "rb");
    if (arqLogin==NULL)
    {
        printf("ocorreu um erro");
    }
    else
    {
        for (;;)
        {
            fread(&c,sizeof(login),1, arqLogin );
            if (feof(arqLogin))
            {
                break;
            }
            if ((strcmp(c.senha, confSenha)==0) && (strcmp(c.user, confUser)==0))
            {
                if (strcmp(confUser, adm)==0 && strcmp(confSenha, adm)==0)
                {
                    *auxLogin=2;
                }
                else
                {
                    *auxLogin=1;
                }
            }
        }
        if (*auxLogin==0)
        {
            printf("\nLogin ou senha incorretos,o programa será encerrado\n");
        }
    }
    fclose(arqLogin);
}
void menuUser(int *selecaoInicial, int *auxIndicePerg, char confUser[10]) //menu do usuario
{
    cabecalho();
    printf("1- Jogar\n\n");
    printf("2- Ver placar\n\n");
    printf("3- Dicas\n\n");
    printf("4- Sair\n\n\n");
    printf("Digite sua opção:  ");
    do
    {
        fflush(stdin);
        scanf("%i", selecaoInicial);
        if (*selecaoInicial<1 || *selecaoInicial>4)
        {
            printf("\n\nOpção invalida, tente novamente:  ");
        }
    }
    while(*selecaoInicial<1 || *selecaoInicial>4);
    switch(*selecaoInicial)
    {
    case 1:
        lerIndicePergunta(auxIndicePerg);
        jogabilidade(auxIndicePerg, confUser);
        break;

    case 2:
        visualizarRanking();
        break;

    case 3:
        extras();
        break;

    case 4:
        exit(1);
        break;

    default:
        printf("\nERRO\n");
    }
    system("pause");
    system("cls");
}
void menuAdmin(int *selecaoInicial, int *auxIndicePerg)
{
    textcolor(11);
    cabecalho();
    printf("1- Criar perguntas\n\n");
    printf("2- Editar perguntas\n\n");
    printf("3- Apagar uma questão\n\n");
    printf("4- Apagar todas as questões\n\n");
    printf("5- Visualizar todas as questões e respostas\n\n");
    printf("6- Verificar logins\n\n");
    printf("7- Zerar ranking\n\n");
    printf("8- Resetar o jogo\n\n");
    printf("9- Sair\n");
    do
    {
        printf("\n\nDigite sua opção: ");
        fflush(stdin);
        scanf("%i", selecaoInicial);
        if (*selecaoInicial<1 || *selecaoInicial>9)
        {
            printf("\nOpção invalida, tente novamente:  ");
        }
    }
    while(*selecaoInicial<1 || *selecaoInicial>9);
    switch(*selecaoInicial)
    {
    case 1:
        *auxIndicePerg=0;
        lerIndicePergunta(auxIndicePerg);
        criaIndicePergunta(auxIndicePerg);
        lerIndicePergunta(auxIndicePerg);
        criaPergunta(auxIndicePerg);
        break;
    case 2:
        editarPergunta();
        break;

    case 3:
        apagarPergunta();
        break;

    case 4:
        apagarQuestoes();
        break;
    case 5:
        mostrarTodasPergResp();
        break;
    case 6:
        loginsAdmin();
        break;
    case 7:
        zerarRanking();
        break;
    case 8:
        reset();
        break;
    case 9:
        printf("\nPrograma encerrado\n");
        exit(0);
        break;
    default:
        printf("\nERRO\n");
    }
    system("pause");
    system("cls");
}

void reset()
{
    int i=0;
    printf("\nDeseja remover todas os dados do jogo? 1-NÃO 2-SIM ");
    scanf("%i", &i);
    if (i==2)
    {
        remove("questoes.qz");
        remove("Ranking.pont");
        remove("Perfil.qz");
        remove("IndicePergunta.id");
        printf("\nDados apagados com sucesso, o jogo será encerrado!\n\n");
        exit(1);
    }
    else
    {
        printf("\nRetorno ao menu do administrador\n\n");
    }
}
void editarPergunta()
{

    int indiceEditar, a=0;

    mostrarTodasPergResp();

    printf("\nDigite a numero da questao que voce deseja alterar:");
    scanf("%i", &indiceEditar);
    questoes l;
    FILE* arqQuestoes;

    FILE * arqauxiliar;

    arqQuestoes = fopen("questoes.qz", "r+b");
    arqauxiliar=fopen("arquivo.aux", "w+b");

    if (arqQuestoes==NULL)

    {
        printf("ocorreu um erro");

    }
    else

    {
        for (;;)
        {

            fread(&l, sizeof(questoes), 1, arqQuestoes);
            if (feof(arqQuestoes))
            {
                break;
            }

            if (indiceEditar==l.pergunta.indicePergunta)
            {
                printf("O que vocé deseja alterar?\n1-Enunciado da questão\n2-Enunciado das perguntas:   ");
                fflush(stdin);
                do
                {
                    scanf("%i", &a);
                    if (a!=1 && a!=2)
                    {
                        printf("\nOpção invalida, tente novamente:");
                    }
                }
                while(a!=1 && a!=2);
                if (a==1)
                {
                    printf("\nDigite sua questao: ");
                    fflush(stdin);
                    l.pergunta.indicePergunta=indiceEditar;
                    fgets(l.pergunta.textoPergunta,200,stdin);
                    l.pergunta.textoPergunta[strlen(l.pergunta.textoPergunta)-1]=NULL;
                }
                else if (a==2)
                {

                    printf("\nDigite uma resposta:");

                    fflush(stdin);

                    fgets(l.resposta1.textoResposta,100,stdin);

                    l.resposta1.indiceResp=1;
                    l.resposta1.textoResposta[strlen(l.resposta1.textoResposta)-1]=NULL;

                    printf("\nDigite uma resposta:");

                    fflush(stdin);

                    fgets(l.resposta2.textoResposta,100,stdin);

                    l.resposta2.indiceResp=2;

                    l.resposta2.textoResposta[strlen(l.resposta2.textoResposta)-1]=NULL;

                    printf("\nDigite uma resposta:");
                    fflush(stdin);

                    fgets(l.resposta3.textoResposta,100,stdin);

                    l.resposta3.indiceResp=3;

                    l.resposta3.textoResposta[strlen(l.resposta3.textoResposta)-1]=NULL;
                }

                fwrite(&l,sizeof(questoes),1, arqauxiliar );

                if(ferror(arqauxiliar)!=0)
                {

                    printf("\nErro na gravação");
                }

                else
                {

                    printf("Questao alterada com sucesso\n");

                }
            }

            if (indiceEditar!=l.pergunta.indicePergunta)

            {
                fwrite(&l, sizeof(questoes), 1, arqauxiliar);
            }
        }
    }

    fclose(arqQuestoes);
    fclose(arqauxiliar);

    remove("questoes.qz");

    rename("arquivo.aux","questoes.qz");
}
void apagarPergunta()
{

    int indiceEditar;

    mostrarTodasPergResp();

    printf("\nDigite o numero da questao que voce deseja alterar:");
    scanf("%i", &indiceEditar);
    questoes l;
    int indice=0;

    //limpaPersonagem(&c);

    FILE* arqQuestoes;

    FILE * arqauxiliar;

    arqQuestoes = fopen("questoes.qz", "r+b");
    arqauxiliar=fopen("arquivo.aux", "w+b");

    if (arqQuestoes==NULL)

    {
        printf("ocorreu um erro");

    }
    else

    {
        for (;;)
        {

            fread(&l, sizeof(questoes), 1, arqQuestoes);
            if (feof(arqQuestoes))
            {
                break;
            }

            if (indiceEditar==l.pergunta.indicePergunta)
            {

            }

            if (indiceEditar!=l.pergunta.indicePergunta)

            {
                indice++;
                l.pergunta.indicePergunta=indice;
                fwrite(&l, sizeof(questoes), 1, arqauxiliar);
            }
        }
    }
    indice--;
    criaIndicePergunta(&indice);
    fclose(arqQuestoes);
    fclose(arqauxiliar);

    remove("questoes.qz");

    rename("arquivo.aux","questoes.qz");
}

void criarLogin(char confSenha[10],char confUser[10], char adm[], int *auxLogin )//criar e editar login
{
    login c;
    FILE* arqLogin;
    arqLogin = fopen("Perfil.qz", "ab");
    if (arqLogin==NULL)
    {
        printf("Ocorreu um erro");
    }
    else
    {
        printf("Digite seu login: ");
        fflush(stdin);
        fgets(c.user,10,stdin);
        c.user[strlen(c.user)-1]=NULL;
        printf("Digite a sua senha  ");
        fflush(stdin);
        fgets(c.senha, 10, stdin);
        c.senha[strlen(c.senha)-1]=NULL;
        fwrite(&c,sizeof(login),1, arqLogin );
        if(ferror(arqLogin)!=0)
        {
            printf("\nErro na gravação");
        }
        else
        {
            printf("Login criado com sucesso\n");
        }
        fclose(arqLogin);
    }
    system("pause");
    system("cls");
    menuLogin(confSenha, confUser, adm, auxLogin);
}
void menuLogin(char confSenha[10],char confUser[10], char adm[], int *auxLogin ) //menu para criar login
{
    int selecaoInicial;
    cabecalho();
    printf("1- Login\n\n");
    printf("2- Criar login\n\n");
    printf("3- Sair\n\n\n");
    printf("Digite sua opção:  ");
    do
    {
        scanf("%i", &selecaoInicial);
        if (selecaoInicial!=1 && selecaoInicial!=2 && selecaoInicial!=3)
        {
            printf("\nOpção invalida, tente novamente:  ");
        }
    }
    while(selecaoInicial!=1 && selecaoInicial!=2 && selecaoInicial!=3);
    switch(selecaoInicial)
    {
    case 1:
        areaLogin(confSenha, confUser, adm, auxLogin);
        break;

    case 2:
        criarLogin(confSenha, confUser, adm, auxLogin);
        break;

    case 3:
        printf("\nPrograma encerrado\n");
        exit(0);
        break;

    default:
        printf("\nErro de digitação\n");
    }
    system("cls");
}
void areaLogin(char confSenha[10], char confUser[10], char adm[], int *auxLogin) //colocar a senha e o login do usuario
{
    printf("Digite seu usuario:  ");
    fflush(stdin);
    fgets(confUser, 10, stdin);
    confUser[(strlen(confUser)-1)]=NULL;
    printf("\nDigite sua senha: ");
    fflush(stdin);
    fgets(confSenha, 10, stdin);
    confSenha[(strlen(confSenha)-1)]=NULL;
    verificarLogin(confSenha,confUser, auxLogin, adm);
}
void criaPergunta(int *auxIndicePerg) //CRIA TODA A QUESTAO DO JOGO
{
    questoes l;
    int auxCorreta;
    FILE* arqQuestoes;
    arqQuestoes = fopen("questoes.qz", "ab");
    if (arqQuestoes==NULL)
    {
        printf("ocorreu um erro");
    }
    else
    {
        printf("Digite sua questao: ");
        fflush(stdin);
        l.pergunta.indicePergunta=*auxIndicePerg;
        fgets(l.pergunta.textoPergunta,200,stdin);
        l.pergunta.textoPergunta[strlen(l.pergunta.textoPergunta)-1]=NULL;
        printf("\nDigite uma resposta:");
        fflush(stdin);
        fgets(l.resposta1.textoResposta,100,stdin);
        l.resposta1.indiceResp=1;
        l.resposta1.textoResposta[strlen(l.resposta1.textoResposta)-1]=NULL;
        printf("\nDigite uma resposta:");
        fflush(stdin);
        fgets(l.resposta2.textoResposta,100,stdin);
        l.resposta2.indiceResp=2;
        l.resposta2.textoResposta[strlen(l.resposta2.textoResposta)-1]=NULL;
        printf("\nDigite uma resposta:");
        fflush(stdin);
        fgets(l.resposta3.textoResposta,100,stdin);
        l.resposta3.indiceResp=3;
        l.resposta3.textoResposta[strlen(l.resposta3.textoResposta)-1]=NULL;
        do
        {
            printf("\nQual a resposta correta?");
            fflush(stdin);
            scanf("%i", &auxCorreta);
            if (auxCorreta!=1 && auxCorreta!=2 && auxCorreta!=3)
            {
                printf("Opção invalida, tente novamente");
            }
        }
        while(auxCorreta!=1 && auxCorreta!=2 && auxCorreta!=3);
        if (auxCorreta==l.resposta1.indiceResp)
        {
            l.resposta1.verdade=1;
            l.resposta2.verdade=0;
            l.resposta3.verdade=0;
        }
        else if (auxCorreta==l.resposta2.indiceResp)
        {
            l.resposta1.verdade=0;
            l.resposta2.verdade=1;
            l.resposta3.verdade=0;
        }
        else if (auxCorreta==l.resposta3.indiceResp)
        {
            l.resposta1.verdade=0;
            l.resposta2.verdade=0;
            l.resposta3.verdade=1;
        }
        fwrite(&l,sizeof(questoes),1, arqQuestoes );
        if(ferror(arqQuestoes)!=0)
        {
            printf("\nErro na gravação");
        }
        else
        {
            printf("Questao criada com sucesso\n");
        }
        fclose(arqQuestoes);
    }
}
void loginsAdmin() //poder verificar todos os usuarios cadastrados SEM VER A SENHA
{
    login r;
    FILE* arqLogin;
    arqLogin = fopen("Perfil.qz", "rb");
    if (arqLogin==NULL)
    {
        printf("Ocorreu um erro");
    }
    else
    {
        for(;;)
        {
            if(feof(arqLogin))
            {
                break;
            }
            fread(&r, sizeof(login), 1, arqLogin);
            printf("\n%s",r.user);
        }
    }
    fclose(arqLogin);

}
void zerarRanking()
{
    FILE* arquivo;
    arquivo= fopen("Ranking.pont", "rb");
    if(arquivo== NULL)
    {
        printf("\nErro\n");
    }
    else {}
    fclose(arquivo);
    remove("Ranking.pont");
    printf("\nRanking apagado com sucesso");
}
void visualizarRanking()
{
    auxRanking r;
    FILE* arquivo;
    arquivo= fopen("Ranking.pont", "rb");
    if(arquivo== NULL)
    {
        printf("\nErro\n");
    }
    else
    {
        printf("\nUsuario com recorde de pontos:\n");
        fread(&r, sizeof(auxRanking), 1, arquivo);
        printf("Jogador %s", r.snomeJogador);
        printf("\nPontuação %i\n\n", r.spontuacao);
    }
    fclose(arquivo);
}
void primeiroLogin(char confSenha[10],char confUser[10], char adm[], int *auxLogin )
{
    login t;
    FILE* arquivo;
    arquivo= fopen("Perfil.qz", "ab");
    if(arquivo==NULL)
    {
        printf("\nErro\n");
    }
    else
    {
        strcpy(t.user, adm);
        strcpy(t.senha, adm);
        fwrite(&t, sizeof(login), 1, arquivo);
        fclose(arquivo);
    }
    printf("\nPrimeiro login!\n\n");
    printf("Crie sua senha de acesso ao jogo.\n\nPara acesso a área de adminstrador e criar suas perguntas, utilize o login ADMIN e a senha ADMIN. Bom jogo!\n\n");
    system("pause");
    system("cls");
    criarLogin(confSenha, confUser, adm, auxLogin);
}
void extras()
{
    printf("Jogo criado para a disciplina de Programação II\n");
    printf("Desenvolvedora: Lana Eduarda Schimitez\n");
    printf("Tecnologia em Jogos Digitais - FATEC AMERICANA\n\n");
    printf("Caso deseje criar/apagar/editar as questões, utilize o login ADMIN e a senha ADMIN\n\n\n\n");
}
