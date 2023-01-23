/***************************************************************************
    Programa Principal
***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/************************************************************************************************
     Função abre ficheiro

     Parâmetros:
     Modo c = Criar ficheiro
     Modo g = Abrir ficheiro para gravação
     Modo l = Abrir ficheiro para leitura
     Modo a = Abrir ficheiro para adcionar registos
************************************************************************************************/

FILE* AbreArquivo(char modo, char caminho[30]){
    FILE *arquivo;
    switch(modo){
    	case 'c':
    		//abrir o arquivo em modo de leitura
            arquivo = fopen(caminho,"rt");
            //Se estiver vazio, será aberto no modo de escrita
            if(arquivo==NULL)
			{
        		arquivo = fopen(caminho,"wt");
    		}
            break;
        case 'g':
            arquivo = fopen(caminho,"wt");
            break;
        case 'l':
            arquivo = fopen(caminho,"rt");
            break;
        case 'a':
            arquivo = fopen(caminho,"a");
            break;

    }

    if(arquivo==NULL){      //Se houver algum erro, o ponteiro apontar? para NULL
        printf("Nao foi possivel abrir o arquivo");
        exit(0);
    }
    return arquivo;
}

/************************************************************************************************
     Função Fecha arquivo
     
     Parâmetros:
     *arquivo = Nome do ficheiro
************************************************************************************************/

//Esse comando será repetido muitas vezes, então o tornamos em uma função
void FechaArquivo(FILE *arquivo)
{
    fclose(arquivo);
}

/************************************************************************************************
     Função Deleta Linha N de um Ficheiro

     Parâmetros:
     caminho = Nome do Arquivo
     linha   = linha que será excluída
************************************************************************************************/

int deleteLine(char caminho[30], const int line){

    char buffer[1000];
    int count = 1;
    int retorno = -1;
    FILE *arquivo;
    FILE *arquivoTemp;

    // Abre Ficheiros
    arquivo     = fopen(caminho, "rt");
    arquivoTemp = fopen("temp.txt"    , "wt");

    while ((fgets(buffer, 1000, arquivo)) != NULL){
        if (line != count)
        {
            fputs(buffer, arquivoTemp);
        } else {
            retorno = 0;
        }
        count++;
    }

    // Fecha Ficheiros
    fclose(arquivo);
    fclose(arquivoTemp);

    // Remove o ficheiro original
    remove(caminho);

    // Renomeia o ficheiro
    rename("temp.txt", caminho);

    return retorno;
}

/************************************************************************************************
    Função Replace Char

    Parâmetros:
    str = String original que será modificada
    ch1 = Caracter que será trocado pelo ch2
    ch2 = Caracter que substituirá o ch1
************************************************************************************************/
void replace(char *s,char c1,char c2) {

	int i = 0;

    for(i=0;s[i];i++)
	{
		if(s[i] == c1)
		{
		   s[i] = c2;
	     }
 	}
}

/************************************************************************************************
     Função Percorre Users

     Parâmetros:
     operacao = Operação que será utilizada
     P_nickName = Nome de usuário externo à função
     P_palavraPasse = palavra passe externa à função
************************************************************************************************/
int percorreUsers(char operacao, char P_nickName[20], char P_palavraPasse[20])
{
	// Variáveis com L_: Internas à função
	FILE *arquivo;
    char L_nickName[20];
	char L_nome[100];
	char L_email [100];
	char L_palavraPasse[20];
	char L_numeroTelemovel[20];
	char L_dataNascimento[20];
	int retorno;

	if (operacao == 's')
	{
		retorno = -1;
	}
	else
	{
		retorno = 0;
	}

    arquivo = AbreArquivo('l', "tb_users.txt");

	//Enquanto não encontrar o fim do ficheiro...
    while(!feof(arquivo)){

		//Leitura de dados do ficheiro
    	fscanf(arquivo,"%s %s %s %s %s %s\n", L_nickName, L_palavraPasse, L_nome, L_email, L_numeroTelemovel, L_dataNascimento);

		//Recupera os Espaços no nome com Gravado no Ficheiro com '|'
		replace(L_nome,'|',' ');

    	switch(operacao)
    	{
    		case 'l': //Lista as informações das variáveis a seguir
				printf("Nickname: %s  -  Nome: %s - Email %s %s %s\n", L_nickName, L_nome, L_email, L_numeroTelemovel, L_dataNascimento);
				break;
			case 'c': // Verifica a existência do nickName inserido
				if (strcmp(L_nickName, P_nickName ) == 0)
				{
					retorno = -1;
				}
				break;
			case 's':  // Valida Login p/Iniciar a Sessão
				if (strcmp(L_nickName, P_nickName ) == 0 && (strcmp(L_palavraPasse, P_palavraPasse ) == 0))
				{
					retorno = 0;
				}
				break;
			case 'e':  // Estatisticas - Contagem de  Users
				retorno++;
				break;
		}
    }

    FechaArquivo(arquivo);

    return retorno;
}

/************************************************************************************************
     Função Percorre Amigos

     Parâmetros:
     operacao = Operação que será utilizada
     p_nickName_Logado = Nome de usuário externo, que equivalerá ao logado
     p_nickName_Post = Nome de usuário externo, que equivalerá ao do post

************************************************************************************************/

int percorreAmigos(char operacao, char P_nickName_Logado[20], char P_nickName_Post[20]){

	FILE *arquivo;
	//Nomes de usuário locais
    char L_nickName1[20];
	char L_nickName2[20];
	//Guarda o retorno da função
	int retorno;
	int idAmizade = 0;

	if (operacao == 'e')
	{
		retorno = 0;
	}
	else
	{
		retorno = -1;
	}

	//Abre arquivo "tb_friends.txt" no modo de leitura
    arquivo = AbreArquivo('l',"tb_friends.txt");

	//Enquanto não encontrar o fim do ficheiro...
    while(!feof(arquivo)){

		//Leitura, no ficheiro, de dois nomes de usuário
    	fscanf(arquivo,"%s %s\n", L_nickName1, L_nickName2);
    	idAmizade++;

    	switch(operacao)
    	{
    		//Listar amigos
    		case 'l':
    			//Se o primeiro nickName for igual ao logado...
				if (strcmp(L_nickName1, P_nickName_Logado) == 0)
                {
                	//Escrever o segundo nickName
                	printf("Amigo: %s\n", L_nickName2);
                }
                //Se o segundo nickName for igual ao logado...
                if (strcmp(L_nickName2, P_nickName_Logado) == 0 )
                {
                	//Escrever o primeiro nickName
                	printf("Amigo: %s\n", L_nickName1);
                }
				break;
			// Verificação de amizade
			case 'v':
				//Se o nickName logado for igual a um dos dois nickNames...
				if (strcmp(L_nickName1, P_nickName_Logado) == 0 || strcmp(L_nickName2, P_nickName_Logado) == 0 )
                {
                	//Se o nickName do post for igual a um dos dois nickNames...
                	if (strcmp(L_nickName1, P_nickName_Post) == 0 || strcmp(L_nickName2, P_nickName_Post) == 0 )
                	{
                		retorno = 0;
					}
                }
				break;
            // Devolve o ID da Amizade
			case 'i':
				//Se o nickName logado for igual a um dos dois nickNames...
				if (strcmp(L_nickName1, P_nickName_Logado) == 0 || strcmp(L_nickName2, P_nickName_Logado) == 0 )
                {
                	//Se o nickName do post for igual a um dos dois nickNames...
                	if (strcmp(L_nickName1, P_nickName_Post) == 0 || strcmp(L_nickName2, P_nickName_Post) == 0 )
                	{
                		retorno = idAmizade;
					}
                }
				break;
			case 'e':  // Estatisticas - Contagem de Amizades
				retorno++;
				break;
		}
    }

    FechaArquivo(arquivo);

    return retorno;
}

/************************************************************************************************
     Função Percorre Posts

     Parâmetros:
     operacao = Operação que será utilizada
     P_nickName = nickName externo
     line = Linha do ficheiro
************************************************************************************************/

int percorrePosts(char operacao, char P_nickName[12], int line){

	FILE *arquivo;
    char L_nickName[20];
	char L_dataPost[20];
	char L_VisibilidadePost[20];
	char L_textoPost[150];
	int retorno, mostraPost, idPost, line_no, page;

	idPost  = 0;
	page    = 0;

	if (operacao == 'e')
	{
		retorno = 0;
	}
	else
	{
		retorno = -1;
	}

    arquivo = AbreArquivo('l',"tb_posts.txt");

    while(!feof(arquivo)){

    	fscanf(arquivo,"%s %s %s %s\n", L_nickName, L_dataPost, L_VisibilidadePost, L_textoPost);
    	idPost++;

		// Recupera os Espaços no post com Gravado no Ficheiro com '|'
		replace(L_textoPost,'|',' ');

    	switch(operacao)
    	{
    		case 'l': //Lista Posts
                mostraPost = -1;

				if (strcmp(L_nickName, P_nickName ) == 0)
                {
                    mostraPost = 0;
                }
                else if (strcmp(L_VisibilidadePost, "PRI") == 0 && percorreAmigos('v', P_nickName, L_nickName) == 0)
                {
                    mostraPost = 0;
				}
				else if (strcmp(L_VisibilidadePost, "RES") == 0)
                {
                    mostraPost = 0;
                }
				else if (strcmp(L_VisibilidadePost, "PUB") == 0)
                {
                    mostraPost = 0;
                }

                if (mostraPost == 0)
                {
                    printf("----------------------------------------------------------------------\n");
                    printf("Autor: %s\nPost.: %s\n%s - %s - ID Post: %d\n", L_nickName, L_textoPost, L_dataPost, L_VisibilidadePost, idPost);
                    page++;
                    if (page == 6) {
                        printf("----------------------------------------------------------------------\n");
                        system("pause");
                        system("cls");
                        page = 0;
                    }
                }

				break;
    		case 'a': //Lista Posts Anonimos
				if (strcmp(L_VisibilidadePost, "PUB") == 0)
                {
                    printf("----------------------------------------------------------------------\n");
                    printf("Autor: @%s\nPost.: %s\n%s - %s - ID Post: %d\n", L_nickName, L_textoPost, L_dataPost, L_VisibilidadePost, idPost);
                    page++;
                    if (page == 6) {
                        printf("----------------------------------------------------------------------\n");
                        system("pause");
                        system("cls");
                        page = 0;
                    }
                }
				break;
    		case 'p': // Verifica Propriedade do Post
				if (strcmp(L_nickName, P_nickName ) == 0 && (line == idPost))
                {
                    retorno = 0;
                }
				break;
			case 'e':  // Estatisticas - Contagem de Posts
				retorno++;
				break;
		}
    }

    FechaArquivo(arquivo);

    return retorno;
}

/************************************************************************************************
     Função Cria conta
************************************************************************************************/

void criarConta (){
	char nickName[20];
	char nome[100];
	char email [100];
	char palavraPasse [20];
	char palavraPasse2[20];
	char numeroTelemovel[20];
	char dataNascimento[20];
	int comparacao, tentativas=0;

	printf("Bem vindo a criar conta!\n\n");

	//Variável e loop utilizados para manter o programa rodando até não resultar em erro
	comparacao = 0;
	while(comparacao==0)
	{
		printf("Escreva o seu nome de usuario..: ");
		//Limpeza do "lixo" da memória, removendo-o do buffer
		setbuf(stdin,NULL);
		//Lê uma linha do parâmetro, parando se encontrar um /n ou EOF
		gets(nickName);
		//Tamanho mínimo do nickName
		if (strlen(nickName) < 3) {
			printf("O nome de usuario deve ter no minimo 3 caracteres.\n\n");
			continue;
		}
		//Tamanho máximo do nickName
		if (strlen(nickName) > 12) {
			printf("O nome de usuario deve ter no maximo 12 caracteres.\n\n");
			continue;
		}
		//Guarda o retorno do teste de existência do nickName inserido
		int x = percorreUsers('c', nickName, "");
		//Se o nickName inserido já existir no ficheiro...
		if (x == -1)
		{
			printf("Esse nome de usuario nao esta disponivel.\n\n");
			continue;
		}
		comparacao = 1;
	}

	comparacao = 0;
	while(comparacao==0)
	{
		printf("\nEscreva o seu Nome Completo ...: ");
		setbuf(stdin,NULL);
		gets(nome);
		//Tamanho mínimo do nickName
		if (strlen(nome) < 4) {
			printf("O Nome deve ter no minimo 4 caracteres.\n\n");
			continue;
		}
		
		if (strlen(nome) > 40) {
			printf("O Nome deve ter no maximo 40 caracteres.\n\n");
			continue;
		}
		comparacao = 1;
	}

	// Troca os Espaços por '|' para Gravar no Ficheiro
	replace(nome,' ','|');

	comparacao = 0;
	while(comparacao==0)
	{
		printf("\nEscreva o seu endereco de email: ");
		setbuf(stdin,NULL);
		gets(email);
		
		if (strlen(email) < 8){
			printf("O email deve ter no minimo 8 caracteres.\n\n");
			continue;
		}
		
		if (strlen(email) > 40){
			printf("O email deve ter no maximo 40 caracteres.\n\n");
			continue;
		}
		comparacao = 1;
	}

	printf("\nAviso: A palavra passe deve ir de 6 a 12 caracteres!\n\n");

	comparacao = 0;
	while(comparacao==0)
	{
		printf("Escreva a sua palavra passe....: ");
		setbuf(stdin,NULL);
		gets(palavraPasse);
		
		if (strlen(palavraPasse) < 6 || strlen(palavraPasse) > 12)
		{
			printf("A palavra passe deve ir de 6 a 12 caracteres.\n\n");
			continue;
		}

		printf("\nConfirme a sua palavra passe...: ");
		setbuf(stdin,NULL);
		gets(palavraPasse2);
		
		if (strlen(palavraPasse2) < 6 || strlen(palavraPasse2) > 12)
		{
			printf("A palavra passe deve ir de 6 a 12 caracteres.\n\n");
			continue;
		}
		if(strcmp(palavraPasse, palavraPasse2) != 0)
		{
            printf("As palavras passe nao sao iguais.\n");
			continue;
		}
		comparacao=1;
	}

	comparacao = 0;
	while(comparacao==0)
	{
		printf("\nEscreva sua Data de Nascimento (DD/MM/AAAA): ");
		setbuf(stdin,NULL);
		gets(dataNascimento);

		if (strlen(dataNascimento) < 10 || strlen(dataNascimento) > 10)
		{
			printf("\nA Data de Nascimento deve ser informada no formato (DD/MM/AAAA).\n\n");
			continue;
		}
		comparacao = 1;

	}

	comparacao = 0;
	while(comparacao==0)
	{
		printf("\nEscreva o seu numero de telemovel de 9 digitos: ");
		setbuf(stdin,NULL);
		gets(numeroTelemovel);

		if (strlen(numeroTelemovel) < 9 || strlen(numeroTelemovel) > 9)
		{
			printf("\nO numero de telemovel deve ter 9 caracteres.\n\n");
			continue;
		}
		comparacao = 1;

	}

    // Salvar Dados da Conta em Ficheiro
	FILE* arquivo;
	arquivo = AbreArquivo('a',"tb_users.txt");
	replace(nome,'|',' ');
	fprintf(arquivo,"%s %s %s %s %s %s\n", nickName, palavraPasse, nome, email, numeroTelemovel, dataNascimento);
	FechaArquivo(arquivo);

    // Mensagem de Criação da Conta
    system("cls");
	printf("Conta criada com sucesso!\n");
	printf("-----------------------------------------------\n");
	printf("Nome de usuario....: %s\n", &nickName);
	printf("Nome completo......: %s\n", &nome);
	printf("Email..............: %s\n", &email);
	printf("TeleMovel..........: %s\n", &numeroTelemovel);
	printf("Data de Nascimento.: %s\n", &dataNascimento);
	printf("Palavra passe......: %s\n", &palavraPasse);
	printf("-----------------------------------------------\n\n");

	system("pause");
}

/************************************************************************************************
     Função Cria Post
     
     Parâmetros:
     P_nickName = Nome de usuário
     
************************************************************************************************/
void criarPost (char P_nickName[20]){

	char textoPost[151];
	char visibilidadePost[10];
	int op, comparacao;

	char op1Aux[10] = "PUB";
	char op2Aux[10] = "RES";
	char op3Aux[10] = "PRI";

	//Variável e loop utilizados para manter o programa rodando até não resultar em erro
	comparacao = 0;
	while(comparacao==0)
	{
		printf("Escreva o seu Post..: ");
		setbuf(stdin,NULL);
		gets(textoPost);
		
		if (strlen(textoPost) < 1) {
			printf("O Post não pode ser vazio.\n\n");
			continue;
		}
		
		if (strlen(textoPost) > 150) {
			printf("O Post deve ter no maximo 150 caracteres.\n\n");
			continue;
		}
		comparacao = 1;
	}

	// Troca os Espaços por '|' para Gravar no Ficheiro
	replace(textoPost,' ','|');

	// Recebe Visibilidade do Post
	do {
		printf("Visibilidade do seu Post..:\n");
		printf("   1 = Publico\n");
		printf("   2 = Reservado\n");
		printf("   3 = Privado\n");
		printf("Opcao: ");
		scanf("%d", &op);
		switch(op){
			case 1:
			    sprintf(visibilidadePost, "%s", op1Aux);
                break;
			case 2:
			    sprintf(visibilidadePost, "%s", op2Aux);
                break;
			case 3:
			    sprintf(visibilidadePost, "%s", op3Aux);
                break;
			default:
				printf("Opcao invalida! \n");
                break;
		}

	} while (op != 1 && op != 2 && op != 3);

    // Data e Hora
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int ano, mes, dia, hora, minuto, segundo;
    char dataPost[10];
    ano = tm.tm_year + 1900;
    mes = tm.tm_mon + 1;
    dia = tm.tm_mday;
    sprintf(dataPost, "%d-%d-%d", ano, mes, dia);

    // Grava Post
	FILE* arquivo;
	arquivo = AbreArquivo('a',"tb_posts.txt");
	fprintf(arquivo,"%s %s %s %s\n", P_nickName, dataPost, visibilidadePost, textoPost);
	FechaArquivo(arquivo);

	system("pause");
}

/************************************************************************************************
     Função Cria Amizade
     
     Parâmetros:
     P_nickName = Nome de usuário de quem está logado
************************************************************************************************/
void criarAmizade (char P_nickName[20]){

	char nickName[20];
	int comparacao;

	comparacao = 0;
	while(comparacao==0)
	{
		printf("Escreva o nome de usuario do amigo..: ");
		setbuf(stdin,NULL);
		gets(nickName);
		
		if (strlen(nickName) < 1) {
			printf("O nome de usuario não pode ser vazio.\n\n");
			continue;
		}
		if (strlen(nickName) > 12) {
			printf("O nome de usuario deve ter no maximo 280 caracteres.\n\n");
			continue;
		}

		if (strcmp(P_nickName, nickName) == 0)
		{
			printf("Voce nao pode se adcionar como amigo.\n\n");
			continue;
		}

		int x = percorreUsers('c', nickName, "");
		if (x == 0)
		{
			printf("Esse nome de usuario nao existe.\n\n");
			continue;
		}

		comparacao = 1;
	}

	if (percorreAmigos('i', P_nickName, nickName) > 0) {
        printf("\n\nAmizade Ja Existe!\n\n");
        system("pause");
        return;
	}

    // Grava Amizade
	FILE* arquivo;
	arquivo = AbreArquivo('a',"tb_friends.txt");
	fprintf(arquivo,"%s %s\n", P_nickName, nickName);
	FechaArquivo(arquivo);

	system("pause");
}

/************************************************************************************************
     Função Remover Amizade
     
     Parâmetros:
     P_nickName = Nome de usuário de quem está logado
************************************************************************************************/

void RemoverAmizade(char P_nickName[20]){

	char nickName[20];
	int comparacao, idAmizade, resultadoRemocao;

	//Variável e loop utilizados para manter o programa rodando até não resultar em erro
	comparacao = 0;
	while(comparacao==0)
	{
		printf("Escreva o nome de usuario do amigo a remover a Amizade..: ");
		//Limpeza do "lixo" da memória, removendo-o do buffer
		setbuf(stdin,NULL);
		//Lê uma linha do parâmetro, parando se encontrar um /n ou EOF
		gets(nickName);
		if (strlen(nickName) < 1) {
			printf("O nome de usuario não pode ser vazio.\n\n");
			continue;
		}
		if (strlen(nickName) > 12) {
			printf("O nome de usuario deve ter no maximo 280 caracteres.\n\n");
			continue;
		}

		if (strcmp(P_nickName, nickName) == 0)
		{
			printf("Nome de Usuario Invalido.\n\n");
			continue;
		}

		int x = percorreUsers('c', nickName, "");
		if (x == 0)
		{
			printf("Esse nome de usuario nao existe.\n\n");
			continue;
		}

		comparacao = 1;
	}

	idAmizade = percorreAmigos('i', P_nickName, nickName);
	if (idAmizade > 0) {
        resultadoRemocao = deleteLine("tb_friends.txt", idAmizade);
        if (resultadoRemocao == 0)
        {
            printf("\n\nAmizade removida com Sucesso!\n\n");
        }
	}
    else
    {
        printf("\n\nAmizade NAO Encontrada!\n\n");
    }

	system("pause");
}

/************************************************************************************************
     Função Remover Post
     
     Parâmetros:
     P_nickName = Nome de usuário de quem está logado
     
************************************************************************************************/

void RemoverPost(char P_nickName[20]){

    int idPostDeletar, resultadoRemocao, temPermissaoRemover;

	system("cls");

	printf("Informe o ID do Post a ser removido: ");
	setbuf(stdin,NULL);
	scanf("%d", &idPostDeletar);

	temPermissaoRemover = percorrePosts('p', P_nickName, idPostDeletar);
	//Se o post é nosso, temos permissão para removê-lo
	if (temPermissaoRemover == 0)
    {
    	//Se o post existe...
        resultadoRemocao = deleteLine("tb_posts.txt", idPostDeletar);
        if (resultadoRemocao == 0)
        {
        	//post removido
            printf("\n\nPost removido com Sucesso!\n\n");
        }
        else
        {
            printf("\n\nPost NAO Encontrado!\n\n");
        }
    }
    else
    {	//Mensagem de erro
        printf("\n\nVoce nao tem permissao para Remover esse Post ou ele nao Existe!\n\n");
    }


	system("pause");
}

/************************************************************************************************
     Função Editar Conta
     
     Parâmetros:
     P_nickName = Nome de usuário de quem está logado
************************************************************************************************/

void EditarConta(char P_nickName[20]){

	FILE *arquivo;
	FILE *arquivoTemp;
    char L_nickName[20];
	char L_nome[100];
	char L_email [100];
	char L_palavraPasse[20];
	char L_numeroTelemovel[20];
	char L_dataNascimento[20];

	// Criação de variáveis de trabalho
	int op, opCampo;
	char textoEditado[100];

	// Menu inicial
	do {
		system("cls");

		printf("Editar Perfil: %s\n\n", P_nickName);

		printf("Qual Informacao Desejar Editar:\n");
		printf("1 - Nome\n");
		printf("2 - Email\n");
		printf("3 - Telemovel\n");
		printf("4 - Data Nascimento\n");
		printf("5 - Palavra Passe\n");
		printf("0 - Encerrar Edicao\n\n");
		printf("Escolha uma opcao: ");
		setbuf(stdin,NULL);
		scanf("%d", &op);
		opCampo = op;
		system("cls");

		//Código de escolha de opção e introdução do novo dado
		switch(op){
			case 1:
                printf("Informe o Novo NOME: ");
                setbuf(stdin,NULL);
                gets(textoEditado);
                op = 0;
                break;
			case 2:
                printf("Informe o Novo EMAIL: ");
                setbuf(stdin,NULL);
                gets(textoEditado);
                op = 0;
                break;
			case 3:
                printf("Informe o Novo TELEMOVEL: ");
                setbuf(stdin,NULL);
                gets(textoEditado);
                op = 0;
                break;
			case 4:
                printf("Informe a Nova DATA DE NASCIMENTO: ");
                setbuf(stdin,NULL);
                gets(textoEditado);
                op = 0;
                break;
            case 5:
                printf("Informe a Nova PALAVRA PASSE: ");
                setbuf(stdin,NULL);
                gets(textoEditado);
                op = 0;
                break;
			case 0:
				printf("Edicao Encerrada! ");
                break;
			default:
				printf("Opcao invalida! \n");
			break;
		}
	} while (op != 0);

    // Abre Arquivos
    arquivo     = AbreArquivo('l', "tb_users.txt");
    arquivoTemp = AbreArquivo('c', "temp.txt");

	//Enquanto não chegar no fim do ficheiro...
    while(!feof(arquivo)){

		//Leitura de valores do ficheiro
    	fscanf(arquivo,"%s %s %s %s %s %s\n", L_nickName, L_palavraPasse, L_nome, L_email, L_numeroTelemovel, L_dataNascimento);

		if (strcmp(L_nickName, P_nickName ) == 0)
            switch(opCampo){
            case 1:
                    sprintf(L_nome, "%s", textoEditado);
       				replace(L_nome,' ','|');
                   	opCampo = 0;
                    break;
			case 2:
                    sprintf(L_email, "%s", textoEditado);
                   	opCampo = 0;
                    break;
			case 3:
                    sprintf(L_numeroTelemovel, "%s", textoEditado);
                   	opCampo = 0;
                    break;
			case 4:
                    sprintf(L_dataNascimento, "%s", textoEditado);
                   	opCampo = 0;
                    break;
            case 5:
                    sprintf(L_palavraPasse, "%s", textoEditado);
                   	opCampo = 0;
                    break;
		}

       	fprintf(arquivoTemp,"%s %s %s %s %s %s\n", L_nickName, L_palavraPasse, L_nome, L_email, L_numeroTelemovel, L_dataNascimento);

    }

    FechaArquivo(arquivo);
    FechaArquivo(arquivoTemp);

    // Remove o ficheiro original
    remove("tb_users.txt");

    // Renomeia o ficheiro
    rename("temp.txt", "tb_users.txt");

	printf("\nPerfil Editado com Sucesso!\n\n");
	system("pause");

}

/************************************************************************************************
     Função Editar Post
     
     Parâmetros:
     P_nickName = Nome de usuário de quem está logado
************************************************************************************************/

void EditarPost(char P_nickName[20]){

	FILE *arquivo;
	FILE *arquivoTemp;
    char L_nickName[20];
	char L_dataPost[20];
	char L_visibilidadePost[20];
	char L_textoPost[150];

	// Criação de variáveis de trabalho
	int idPostEditar, temPermissaoEditar, line_no, comparacao;
	char textoEditado[100];

	line_no = 1;

	system("cls");

	printf("Informe o ID do Post a ser Editado: ");
	setbuf(stdin,NULL);
	scanf("%d", &idPostEditar);

	temPermissaoEditar = percorrePosts('p', P_nickName, idPostEditar);
	if (temPermissaoEditar != 0)
    {
        printf("\n\nVoce nao tem permissao para Remover esse Post ou ele nao Existe!\n\n");
        system("pause");
        return;
    }

   	// Variável e loop utilizados para manter o programa rodando até não resultar em erro
	comparacao = 0;
	while(comparacao==0)
	{
		printf("Escreva o seu Post..: ");
		setbuf(stdin,NULL);
		gets(textoEditado);
		
		if (strlen(textoEditado) < 1) {
			printf("O Post não pode ser vazio.\n\n");
			continue;
		}
		
		if (strlen(textoEditado) > 150) {
			printf("O Post deve ter no maximo 150 caracteres.\n\n");
			continue;
		}
		comparacao = 1;
	}

    // Abre Arquivos
    arquivo     = AbreArquivo('l', "tb_posts.txt");
    arquivoTemp = AbreArquivo('c', "temp.txt");

    while(!feof(arquivo)){

    	fscanf(arquivo,"%s %s %s %s\n", L_nickName, L_dataPost, L_visibilidadePost, L_textoPost);

		if (line_no == idPostEditar)
        {
            sprintf(L_textoPost, "%s", textoEditado);
       		replace(L_textoPost,' ','|');
		}

        fprintf(arquivoTemp,"%s %s %s %s\n", P_nickName, L_dataPost, L_visibilidadePost, L_textoPost);
        line_no++;
    }

    FechaArquivo(arquivo);
    FechaArquivo(arquivoTemp);

    // Remove o ficheiro original
    remove("tb_posts.txt");

    // Renomeia o ficheiro
    rename("temp.txt", "tb_posts.txt");

	printf("\nPost Editado com Sucesso!\n\n");
	system("pause");

}

/************************************************************************************************
     Função Menu da Sessão
     
    Parâmetros:
    P_nickName = Nome de usuário de quem está logado
************************************************************************************************/

void menuSessao(char P_nickName[20]){

	// Criação de variáveis de trabalho
	int op;

	// Menu inicial
	do {
		system("cls");

		printf("Perfil: %s\n\n", P_nickName);

		printf("1 - Postar\n");
		printf("2 - Ver Linha do Tempo\n");
		printf("3 - Criar Amizade\n");
		printf("4 - Listar Amigos\n");
		printf("5 - Listar Todas as Contas\n");
		printf("6 - Remover Post\n");
		printf("7 - Editar Dados do Perfil\n");
		printf("8 - Remover Amizade\n");
		printf("9 - Editar Post\n");
		printf("0 - Encerrar a Sessao\n\n");
		printf("Escolha uma opcao: ");
		setbuf(stdin,NULL);
		scanf("%d", &op);
		system("cls");

		switch(op){
			case 1:
				criarPost(P_nickName);
                break;
			case 2:
                percorrePosts('l', P_nickName, 0);
				printf("\n\n");
				system("pause");
                break;
			case 3:
				criarAmizade(P_nickName);
                break;
			case 0:
				printf("Sessão Encerrada! ");
                break;
			case 4:
				percorreAmigos('l', P_nickName, "");
				system("pause");
                break;
            case 5:
				percorreUsers('l', "", "");
				system("pause");
                break;
			case 6:
				RemoverPost(P_nickName);
                break;
			case 7:
				EditarConta(P_nickName);
                break;
			case 8:
				RemoverAmizade(P_nickName);
                break;
			case 9:
                EditarPost(P_nickName);
                break;
			default:
				printf("Opcao invalida! \n");
			break;
		}
	} while (op != 0);

	getchar();
}

/************************************************************************************************
     Função Inciar Sessão
************************************************************************************************/

void iniciarSessao()
{
	char nickName[20];
	char palavraPasse[20];
	int comparacao = 0;

	while(comparacao==0)
	{
		printf("Escreva o seu nome de usuario..: ");
		setbuf(stdin,NULL);
		gets(nickName);
		comparacao = 1;
	}

	comparacao = 0;
	while(comparacao==0)
	{
		printf("Escreva a sua palavra passe....: ");
		setbuf(stdin,NULL);
		gets(palavraPasse);
		comparacao=1;
	}

	if (percorreUsers('s', nickName, palavraPasse) == -1)
	{
		printf("\nLogin invalido!\n\n");
		system("pause");
	}
	else
	{
		printf("Sessao iniciada!\n\n");
		menuSessao(nickName);
	}
}

/************************************************************************************************
     Função Mostrar Estatísticas
************************************************************************************************/

void MostraEstatisticas(){

    int quantidadeUsers, quantidadePosts, quantidadeAmizades;

	quantidadeUsers    = percorreUsers('e', "", "");
    quantidadeAmizades = percorreAmigos('e', "", "");
    quantidadePosts    = percorrePosts('e', "", 0);

	system("cls");
	printf("********    Estatisticas    *********\n\n");
	printf("     Quantidade de Contas..: %d\n", quantidadeUsers);
	printf("     Quantidade de Posts...: %d\n", quantidadePosts);
	printf("     Quantidade de Amizades: %d\n", quantidadeAmizades);
	printf("\n*************************************\n");

}

/************************************************************************************************
     Função Menu Inicial
************************************************************************************************/

void menu(){

	//Criação de variáveis de trabalho
	int op;

	//Menu inicial
	do{
		system("cls");
		printf("********    Bem-vindo    *********\n\n");
		printf("        1 - Iniciar sessao\n");
		printf("        2 - Criar conta\n");
		printf("        3 - Modo anonimo\n");
		printf("        4 - Mostra Estatisticas\n");
		printf("        0 - Sair\n\n");
		printf("        Escolha uma opcao: ");
		setbuf(stdin,NULL);
		scanf("%d", &op);
		system("cls");

		switch(op){
			case 1:
				iniciarSessao();
                break;
			case 2:
				criarConta();
				break;
			case 3:
                percorrePosts('a', "", 0);
				printf("\n\n");
				system("pause");
                break;
			case 4:
                MostraEstatisticas();
				printf("\n\n");
				system("pause");
                break;
			case 0:
				printf("Obrigado pela visita! \n\n");
                break;
			default:
				printf("Opcao invalida! \n");
                break;
		}
	} while (op!=0);
	getchar();
}

/************************************************************************************************
     Função Main
************************************************************************************************/

int main()
{
	//Inicialização dos ficheiros da aplicação
	FILE* arquivo;
	arquivo = AbreArquivo('c', "tb_users.txt");
	FechaArquivo(arquivo);
	arquivo = AbreArquivo('c', "tb_friends.txt");
	FechaArquivo(arquivo);
	arquivo = AbreArquivo('c', "tb_posts.txt");
	FechaArquivo(arquivo);

	//Chama o menu principal
	menu();

	return 0;
}
