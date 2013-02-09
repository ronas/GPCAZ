/*
 * Ronaldo Russel, Roger Russel e Robson Pires, 01/10/2008.
 * Biblioteca de funcoes para tratamento de matrizes.
 *
 */

// Prototipacao ...

const int TRUE = 1;																								// True ...

double  **AlocarMatriz(int c, int l);																			// Alocar Matriz ...
void DesalocarMatriz(double **matriz, int l);																	// Desalocar Matriz ...

double **TransposicaoMatriz(double **matrizEntrada, int c, int l);												// Transposicao de Matrizes ...
void RandomizarMatriz(double **matriz, int l, int c, int limite);												// Escreve dados randomicos em uma matriz existente ...
double **ObterSubMatriz(double **matrizEntrada, int q_linhas, int q_colunas, int f_L, int f_C);					// Aloca Matriz com Sub ...

double  MediaMatriz(double **matriz, int l, int c);																// Calculo da Media Aritmetica dos Elementos da Matriz ...
double  MedianaMatriz(double **matriz, int l, int c);

double  **SubtraiMatriz(char acao, double **matrizA, double **matrizB, int l, int c);								// subtracao de matrizes

void SubtracaoNumeroMatriz(double **matriz, int c, int l, double q_numero);										// Subtracao de um Numero de Uma Matriz ...

double **MultiplicacaoMatriz(char acao, double **matrizA, int cA, int lA, double **matrizB, int cB, int lB); 	// Multiplicacao de Matrizes

double ModuloMatriz(double **matriz,  int l, int c, char tipo);									// Calcula o módulo da matriz

void ImprimirMatriz(char tipo, double **matriz, int c, int l);													// Imprime uma matriz ...

double **VetorizarMatriz(double **matriz, int c, int l);														// Transforma Matriz em Vetor ...
void DesalocarVetor(double **vetor);																			// Desalocar um Vetor ...

double DetMatriz(double **matriz,int ordem);																	// Calculo do Determinante da Matriz
double DetMatriz_CalcMatriz(double **matriz,int ordem);															// Funcao Complementar da DetMatriz

double **RetornaMatrizIdentidade(int linha, int coluna);														// Retorna a matriz identidade ...

void AutoVV(double **a, double **matrizAVV, double **vetorAVV, int prmD, int prmC);
void Quicksort(double *array, int begin, int end);

// Funcoes ...

double CalcMediana(double *array, int elementos){

	double mediana = 0.0;
	int cont = 0 ;
	int coluna = (elementos / 2) - 1;

	if ( elementos % 2 == 0 ) { 

		mediana = (array[coluna] + array[coluna + 1 ])/2;

	} else { 

		mediana = array[coluna];

	}

	return mediana;

}

void Quicksort(double *array, int begin, int end){														

	int aux		= 0,
		pivot	= 0,
		left	= 0,
		right	= 0;

	if(end - begin > 0) {

		pivot = array[begin];
		left = begin + 1;
		right = end;

		while(left < right) {
			if(array[left] <= pivot) {
				left++;
			} else {
				
							// Troca o valor de array[left] com array[right]
				aux = array[left];
				array[left] = array[right];
				array[right] = aux;
							// Fim da troca ( equivale a função swap(array[left], array[right]) )
				right--;
			}
		}
		if(array[left] > pivot) {
			left--;
		}
		
			// Troca o valor de array[begin] com array[left]
		aux = array[begin];
		array[begin] = array[left];
		array[left] = aux;
			// Fim da troca ( equivale a função swap(array[begin], array[left]) )
	
			// Faz as chamadas recursivas para as duas partes da lista
		Quicksort(array, begin, left-1);
		Quicksort(array, right, end);
	}

}

double **AlocarMatriz(int c, int l) {
/*
 * Funcao para alocar uma matriz, do tipo double, dinamicamente.
 *
 * Parametros:
 *
 *	c		Numero de colunas para a matriz.
 *	l		Numero de linhas para a matriz.
 *
 * Retorno:		Ponteiro para a matriz alocada.
 *
 * Exemplo:
 *
 *			double **Matriz;
 *
 *			Matriz = AlocarMatriz(3, 3);
 *
 *			DesalocarMatriz(Matriz, 3);
 *
 */

	int cnt,
        contC=0,
        contL=0;
        
	double **matriz;

	matriz = (double **)malloc(sizeof(double *) * l);

	if (!matriz) return NULL;

	for(cnt = 0; cnt < l; cnt++)
	{
		matriz[cnt] = (double *)malloc(sizeof(double) * c);

		if (!matriz[cnt]) {
			DesalocarMatriz(matriz, cnt);
			return 0;
		}
	}

    for ( contC=0; contC<c ; contC++ ) 
        for ( contL=0; contL<l;contL++)
            matriz[contL][contC]=0.0;
    
	return matriz;
}

void DesalocarMatriz(double **matriz, int l) {
/*
 * Funcao para desalocar uma matriz, do tipo double, alocada dinamicamente.
 *
 * Parametros:
 *
 *	**matriz	Ponteiro da matriz dinamica que sera desalocada.
 *	l		Numero de linhas da matriz.
 *
 * Retorno:		Inexistente.
 *
 * Exemplo:
 *
 *			double **Matriz;
 *
 *			Matriz = AlocarMatriz(3, 3);
 *			DesalocarMatriz(Matriz, 3);
 *
 */

	int cnt;

	for(cnt = 0; cnt < l; cnt++) {
		if(matriz[cnt]) free(matriz[cnt]);
	}

	if(matriz) free(matriz);
}

double **TransposicaoMatriz(double **matrizEntrada, int c, int l) {
/*
 * Funcao para transposicao de matrizes. Uma matriz e passada como parametro e a transposta e retornada.
 *
 * Parametros:
 *
 *	**matrizEntrada	Ponteiro da matriz a ser transposta
 *	c		Numero de colunas da matriz.
 *	l		Numero de linhas da matriz.
 *
 * Retorno:		Ponteiro para a matriz transposta.
 *
 * Exemplo:
 *
 *			double **MatrizTransposta, **MatrizRandomica;
 *
 *			MatrizRandomica = AlocarMatriz(3, 3);
 *			RandomizarMatriz(MatrizRandomica, 3, 3, 256);
 *
 *			MatrizTransposta = TransposicaoMatriz(MatrizRandomica, 3, 3);
 *
 *			DesalocarMatriz(MatrizTransposta, 3);
 *			DesalocarMatriz(MatrizRandomica, 3);
 *
 */

	int cc, cl;
	double **matriz;

	matriz = AlocarMatriz(l, c); // Alocacao invertida ...

	if (!matriz) return NULL;

	for(cl=0; cl<l; cl++)
		for(cc=0; cc<c; cc++)
			matriz[cc][cl] = matrizEntrada[cl][cc];

	return matriz;
}

void RandomizarMatriz(double **matriz, int c, int l, int limite) {
/*
 * Funcao para randomizar os dados de uma matriz.
 *
 * Parametros:
 *
 *	**matriz	Ponteiro referente a matriz que sera randomizada.
 *	c		Numero de colunas existentes na matriz.
 *	l		Numero de linhas existentes na matriz.
 *	limite		limite numerico para os numeros randomicos. Ex: 256, Numeros apenas de 0-256.
 *
 * Retorno:		Inexistente, as atualizacoes sao aplicadas diretamente na matriz de origem.
 *
 * Exemplo:
 *
 *			double **MatrizRandomica;
 *
 *			MatrizRandomica = AlocarMatriz(3, 3);
 *			RandomizarMatriz(MatrizRandomica, 3, 3, 256);
 *
 *			DesalocarMatriz(MatrizRandomica, 3);
 *
 */

	int cntLinha, cntColuna;

	for (cntLinha=0; cntLinha<l; cntLinha++)
		for (cntColuna=0; cntColuna<c; cntColuna++)
			matriz[cntLinha][cntColuna] = rand() % limite;
}

double **ObterSubMatriz(double **matrizEntrada, int q_colunas, int q_linhas, int f_C, int f_L) {
/*
 * Funcao para obter sub-matrizes de matrizes. Uma matriz e passada como parametro e a sub e retornada.
 *
 * Parametros:
 *
 *	**matrizEntrada	Ponteiro da matriz de origem dos dados.
 *	q_colunas	Quantidade de Colunas da Sub-Matriz.
 *	q_linhas	Quantidade de Linhas da Sub-Matriz.
 *	f_C		Fator de Deslocamento horizontal em grupos de n sub-matrizes.
 *	f_L		Fator de Deslocamento vertical em grupos de n sub-matrizes.
 *
 * Retorno:		Ponteiro para a sub-matriz.
 *
 * Exemplo:
 *
 *			double **SubMatriz, **MatrizRandomica;
 *
 *			MatrizRandomica = AlocarMatriz(10, 10);
 *			RandomizarMatriz(MatrizRandomica, 10, 10, 256);
 *			SubMatriz = ObterSubMatriz(MatrizRandomica, 3, 3, 0, 0);
 *
 *			DesalocarMatriz(SubMatriz, 3);
 *			DesalocarMatriz(MatrizRandomica, 10);
 *
 */

	double **matriz;

	int cnt_linhas = 0, cnt_colunas = 0;

	matriz = AlocarMatriz(q_colunas, q_linhas);
	if ( !matriz ) return NULL;

	for (cnt_colunas=0; cnt_colunas<q_colunas; cnt_colunas++)
		for (cnt_linhas=0; cnt_linhas<q_linhas; cnt_linhas++)
			matriz[cnt_linhas][cnt_colunas] = matrizEntrada[cnt_linhas + (f_L * q_linhas)][cnt_colunas + (f_C * q_colunas)];

	return matriz;
}

double MediaMatriz(double **matriz, int c, int l) {
/*
 * Funcao para calcular a media aritmetica dos dados de uma matriz.
 *
 * Parametros:
 *
 *	**matriz	Ponteiro referente a matriz que sera calculada.
 *	c		Numero de colunas existentes na matriz.
 *	l		Numero de linhas existentes na matriz.
 *
 * Retorno:		Media Aritmetica dos Elementos da Matriz.
 *
 * Exemplo:
 *
 *			double varMedia, **MatrizRandomica;
 *
 *			MatrizRandomica = AlocarMatriz(3, 3);
 *			RandomizarMatriz(MatrizRandomica, 3, 3, 256);
 *			varMedia = varMediaMatriz(MatrizRandomica, 3, 3);
 *
 *			DesalocarMatriz(MatrizRandomica, 10);
 *
 */

	int cntLinha, cntColuna;
	double retResultado = 0.0;

	for (cntLinha=0; cntLinha<l; cntLinha++)
		for (cntColuna=0; cntColuna<c; cntColuna++)
			retResultado += matriz[cntLinha][cntColuna];

	return( retResultado/(c*l) );
}

double **MultiplicacaoMatriz(char acao, double **matrizA, int cA, int lA, double **matrizB, int cB, int lB){

/*
 * Funcao para calcular Multiplicar duas matrizes
 *
 * Parametros:
 *
 *  acao		A acao define se ira retornar um ponteiro para uma matriz nova ou se ira retornar o valor na matrizA
 *			Parametros: 'n' nova matrizes
 *							'r' se ira retornar na matrizA
 *
 *	**matrizA	Ponteiro referente a primeira matriz que sera multiplicada.
 *	cA		Quantidade de colunas da matrizA
 *  	lA		Quantidade de linhas da matrizA
 *
 *	**matrizB	Ponteiro referente a primeira matriz que sera multiplicada.
 *	cB		Quantidade de colunas da matrizB
 *  	lB		Quantidade de linhas da matrizB
 *
 * Exemplo:
 *
 *			matrizA = AlocarMatriz(3, 3);
 *			matrizB = AlocarMatriz(3, 3);
 *
 *			RandomizarMatriz(matrizA, 3, 3, 256);
 *			RandomizarMatriz(matrizB, 3, 3, 256);
 *
 *			Matriz = MultiplicacaoMatriz('n',matrizA, 3, 3,matrizb, 3, 3);
 *
 *			DesalocarMatriz(Matriz,  3);
 *			DesalocarMatriz(matrizA, 3);
 *			DesalocarMatriz(matrizB, 3);
 *
 */


	if ( cA != lB ){
		printf("\n\n A quantidade de Colunas na Matriz A é diferente da quantidade de Linhas na Matriz B, uso incorreto da função\n\n");
		return NULL; // Se a quantidade de colunas em A for diferente da quantidade de Linhas em B nao sera possivel efetuar a multiplicacao
	}

	int	cont,
		contC,
		contL;

	double **matriz;

	matriz = AlocarMatriz(cB,lA); // Quantidade de colunas que tem na segunda matriz e a quantidade de linhas que tem na primeira.

	for (contL=0;contL<lA;contL++){
		for (contC=0;contC<cB;contC++){
			matriz[contL][contC] = 0.0 ;
			for (cont=0;cont<cA;cont++){
				matriz[contL][contC] += matrizA[contL][cont] * matrizB[cont][contC];
			}
		}
	}


	if ( acao = 'n' ) return matriz;

	for ( contL=0;contL<lA;contL++)
		for (contC=0;contC<cB;contC++)
			matrizA[contL][contC] = matriz[contL][contC];

	DesalocarMatriz(matriz,lA);

	return matrizA;

}

double ModuloMatriz(double **matriz,  int c, int l, char tipo){									// Calcula o módulo da matriz

	int contC,
		contL;

	double modulo = 0.0;

	switch ( tipo ){

	case 'f':
	default:

		for (contC=0 ; contC<c ; contC++)
			for (contL=0 ; contL<c ; contL++)
				modulo += matriz[contL][contC] * matriz[contL][contC];

		modulo = sqrt(modulo);

	} // Fim do Switch

	return modulo;

}

void SubtracaoNumeroMatriz(double **matriz, int c, int l, double q_numero) {
/*
 * Funcao para Subtrair um numero de uma Matriz.
 *
 * Parametros:
 *
 *	**matriz	Ponteiro referente a matriz que sera processada.
 *	c		Numero de colunas existentes na matriz.
 *	l		Numero de linhas existentes na matriz.
 *	q_numero	limite numerico para os numeros randomicos. Ex: 256, Numeros apenas de 0-256.
 *
 * Retorno:		Inexistente, o processamento e efetuado direto na matriz
 *			de origem.
 *
 * Exemplo:
 *
 *			double **matrizRandSubtracao;
 *
 *			MatrizRandSubtracao = AlocarMatriz(4, 4);
 *			RandomizarMatriz(MatrizRandSubtracao, 4, 4, 256);
 *
 *			SubtracaoNumeroMatriz(MatrizRandSubtracao, 4, 4, 1);
 *
 *			DesalocarMatriz(MatrizRandSubtracao, 4);
 *
 */

	int cntLinha, cntColuna;

	for (cntLinha=0; cntLinha<l; cntLinha++)
		for (cntColuna=0; cntColuna<c; cntColuna++)
			matriz[cntLinha][cntColuna] = matriz[cntLinha][cntColuna] - q_numero;
}

void ImprimirMatriz(char tipo, double **matriz, int c, int l) {
/*
 * Funcao para imprimir uma matriz na saida padrao.
 *
 * Parametros:
 *
 *	tipo		O tipo da variavel que sera impressa.
 *	**matriz	Ponteiro referente a matriz que sera impressa.
 *	c	     	Numero de colunas existentes na matriz.
 *	l		Numero de linhas existentes na matriz.
 *
 * Retorno:		Inexistente.
 *
 * Exemplo:
 *
 *			double **matrizRandomica;
 *
 *			MatrizRandomica = AlocarMatriz(3, 3);
 *			RandomizarMatriz(MatrizRandomica, 3, 3, 256);
 *
 *			ImprimirMatriz('d',MatrizRandomica, 3, 3);
 *
 *			DesalocarMatriz(MatrizRandomica, 10);
 *
 */

	int cntC, cntL;
	int teste;

	switch(tipo){

	case 'd':	for(cntL=0; cntL < l; cntL++) {
        			for(cntC=0; cntC < c; cntC++) printf("%02d ", matriz[cntL][cntC]);
					printf("\n");
				}
			break;

	case 'f':	for(cntL=0; cntL < l; cntL++) {
        			for(cntC=0; cntC < c; cntC++) printf("%f ", matriz[cntL][cntC]);
					printf("\n");
				}
			break;

	case 'x':	for(cntL=0; cntL < l; cntL++) {
				for(cntC=0; cntC < c; cntC++) { // printf("%02x ", matriz[cntL][cntC]);
					teste = (int)matriz[cntL][cntC];
					printf("%02d ", teste );
					}
					printf("\n");
				}
			break;

	case 's':	for(cntL=0; cntL < l; cntL++) {
				for(cntC=0; cntC < c; cntC++) printf("%s", matriz[cntL][cntC]);
					printf("\n");
				}
			break;

	default:
				for(cntL=0; cntL < l; cntL++) {
				for(cntC=0; cntC < c; cntC++) printf("%02f ", matriz[cntL][cntC]);
					printf("\n");
				}
	}
}

double **VetorizarMatriz(double **matriz, int c, int l){
/*
 * Funcao para transformar uma matriz em um vetor.
 *
 * Parametros:
 *
 *	**matriz	Ponteiro referente a matriz que sera vetorizada.
 *	c		Numero de colunas existentes na matriz.
 *	l		Numero de linhas existentes na matriz.
 *
 * Retorno:		Ponteiro para o vetor, de tamanho c * l, que foi criado.
 *
 * Exemplo:
 *
 *			double **matrizRandomica, **vetor, cntVetor;
 *
 *			MatrizRandomica = AlocarMatriz(4, 4);
 *			RandomizarMatriz(MatrizRandomica, 4, 4, 256);
 *
 *			vetor = VetorizarMatriz(MatrizRandomica, 4, 4);
 *
 *			DesalocarVetor(vetor);
 *			DesalocarMatriz(MatrizRandomica, 4);
 */

	double **vetor;
	int	cntVetor = 0, cntColuna, cntLinha;

	vetor = (double **)malloc(sizeof(double *) * c * l);		// Gera um vetor do tamanho da Matriz ...

	for (cntColuna=0; cntColuna < c; cntColuna++)
		for (cntLinha=0; cntLinha < l; cntLinha++) {
//			vetor[cntVetor] = (double *)matriz[cntColuna][cntLinha];
			cntVetor++;
		}

	return vetor;
}

void DesalocarVetor(double **vetor) {
/*
 * Funcao para desalocar um vetor.
 *
 * Parametros:
 *
 *	**vetor		Ponteiro referente ao vetor a ser desalocado.
 *
 * Retorno:		Inexistente.
 *
 * Exemplo:
 *
 *			double **matrizRandomica, **vetor;
 *
 *			MatrizRandomica = AlocarMatriz(4, 4);
 *			RandomizarMatriz(MatrizRandomica, 4, 4, 256);
 *
 *			vetor = VetorizarMatriz(MatrizRandomica, 4, 4);
 *
 *			DesalocarVetor(vetor);
 *
 *			DesalocarMatriz(MatrizRandomica, 4);
 */

	if ( vetor != NULL ) free(vetor);
}

double  **SubtraiMatriz(char acao, double **matrizA, double **matrizB, int l, int c){

	double **matriz;

	int contC,
		contL;

	matriz = AlocarMatriz(c,l);

	for ( contC = 0 ; contC < c ; contC ++ )
		for ( contL = 0 ; contL < l ; contL++)
			matriz[contL][contC] = matrizA[contL][contC] - matrizB[contL][contC];

	switch (acao){

	case 'r': // Recursivo				Pesquizar se eu consigo simplesmente trocar o endereço da matriz alocada, mas depois.

			for (contC=0;contC<c;contC++)
				for (contL=0;contL<l;contL++)
					matrizA[contL][contC] = matriz[contL][contC];
					return matrizA;

			break;

	case 'n': // Nova matriz
	default :
			return matriz;
	}
}


double DetMatriz(double **matriz,int ordem) {
/*
 * Funcao para calcular o determinante da matriz.
 *
 * Parametros:
 *
 * **matriz		Ponteiro para a Matriz a qual deseja saber o determinante.
 * ordem		Se a matriz � 2x2, 3x3, 4x4, 5x5, 6x6; no caso s� � utilizado
 *                      a ordem ->[2]x2 ->[3]x3, visto que s� h� determinante de
 *                      matrizes quadradas...
 *
 * Retorno:		Determinante da Matriz.
 *
 * Exemplo:
 *
 *			double **matriz;
 *
 *			Matriz = AlocarMatriz(3, 3);
 *			RandomizarMatriz(Matriz, 3, 3, 256);
 *
 *			Determinante = DetMatriz(Matriz, 3);
 *
 *			DesalocarMatriz(Matriz, 3);
 *
 *
 */

	double determinante =0.0,subdet=0.0;

	// Metodo de calculo de cofatores

	switch(ordem) {
	case 0:	 determinante = 0.0;
		 break;

	case 1:	 determinante = matriz[0][0];
		 return determinante;
		 break;

	case 2:	 determinante = (matriz[0][0]*matriz[1][1])-(matriz[0][1]*matriz[1][0]);
		 return determinante;
		 break;

	default: determinante = DetMatriz_CalcMatriz(matriz,ordem);		// In�cio da recurs�o para o c�lculo dos cofatores ...
		 return determinante;
		 break;
	}

}

double DetMatriz_CalcMatriz(double **matriz,int ordem){
/*
 * Funcao complementar da DetMatriz, sem ela a DetMatriz n�o calcula determinantes maiores do que 2
 */

	int contL,contC,contM,subL=0,subC=0;
	double **submatriz,determinante=0.0,cofator=0.0,bufDet=0.0;

	submatriz = AlocarMatriz((ordem - 1),(ordem - 1));

	for(contM=1 ;contM <= ordem ; contM++){ // Loop responsavel por passar em todos os cofatores

		for(contL=1; contL<ordem ;contL++){ // Loop responsavel por passar em um cofator

			for (contC=0 ; contC<ordem ; contC++){

				if ( contC != (contM-1) ){
					submatriz[subL][subC]=matriz[contL][contC]; // Coloca os valores do Cofator na submatriz
					subC++; // incrementa o contador de colunas da submatriz
				}
			}
			subL++; // Incrementa o contador de linhas da submatriz
			subC=0; // Zera o contador de colunas do submatriz
		}

		subL=0; // Zera o contador de linhas da submatriz

		if ( ordem > 3 ) {
			bufDet = DetMatriz_CalcMatriz(submatriz,ordem-1); // Coloca no Buffer os CoDeterminante
		} else {
			bufDet = (submatriz[0][0]*submatriz[1][1])-(submatriz[0][1]*submatriz[1][0]); // Calcula o CoDeterminante
		}
		if ((contM+1)%2){ // Respons�vel por fazer o pow(-1,j+i)*Matriz
			bufDet *= -1;
		}
		cofator = matriz[0][contM-1]; // Separa o Cofator
		determinante = determinante + bufDet*cofator;
	}
	DesalocarMatriz(submatriz, ordem -1);
	return determinante;
}


double **RetornaMatrizIdentidade(int linha, int coluna){
/*
 * Funcao para obter a matriz identidade
 *
 * Parametros:
 *
 *	linha		Numero de linhas da matriz.
 *	coluna		Numero de colunas da matriz.
 *
 * Retorno:		Ponteiro para a matriz Identidade.
 *
 * Exemplo:
 *
 *			double **matrizIdentidade;
 *
 *			MatrizIdentidade = RetornaMatrizIdentidade(3, 3);
 *
 *			DesalocarMatriz(MatrizIdentidade, 3);
 *
 */

    int contL,contC;
    double **matriz;

	matriz = AlocarMatriz(coluna, linha);

    for(contL=0;contL<linha;contL++){
        for(contC=0;contC<coluna;contC++){
            if (contC==contL){
                matriz[contL][contC]=1.0;
            } else {
                matriz[contL][contC]=0.0;
            }
        }
    }

    return matriz;
}


void AutoVV(double **a, double **matrizAVV, double **vetorAVV, int prmD, int prmC) {
/*
 * Funcao para calculo de autovalores e autovetores de uma matriz.
 *
 * Parametros:
 *
 *	**a		Ponteiro referente a matriz contendo os dados a serem processados.
 *	**matrizAVV	Ponteiro para a matriz que recebera os autovalores calculados.
 *	**vetorAVV	Ponteiro para a matirz que recebera os autovetores calculados.
 *
 * Retorno:		Inexistente, as atualizacoes sao aplicadas diretamente na matriz de origem.
 *
 * Exemplo:
 *
 *			double	**MatrizRandAV, **MatrizAutoVetor, **MatrizAutoValor;
 *			int	cntAV1, cntAV2;
 *
 *			MatrizRandAV	= AlocarMatriz(3, 3);
 *			MatrizAutoValor = AlocarMatriz(3, 3);
 *			MatrizAutoVetor = AlocarMatriz(3, 3);
 *
 *			RandomizarMatrizDouble(MatrizRandAV, 3, 3, 256);
 *			printf("\n\n\nMatriz Randomica A para exemplo de calculo de autovalor/autovetor:\n\n");
 *			for (cntAV1=0; cntAV1<3; cntAV1++) {
 *			for (cntAV2=0; cntAV2<3; cntAV2++) printf("%6.0f\t", MatrizRandAV[cntAV1][cntAV2]);
 *			printf("\n");
 *			}
 *
 *			AutoVV(MatrizRandAV, MatrizAutoVetor, MatrizAutoValor);
 *
 *			printf("\n\n\nAutovalores:\n\n");
 *			for (cntAV1=0; cntAV1<3; cntAV1++) printf("%20.4f\t", MatrizAutoValor[cntAV1][cntAV1]);
 *
 *			printf("\n\n\nAutovetores:\n\n");
 *			for (cntAV1=0; cntAV1<3; cntAV1++) {
 *			for (cntAV2=0; cntAV2<3; cntAV2++) printf("%20.4f\t", MatrizAutoVetor[cntAV1][cntAV2]);
 *			printf("\n");
 *			}
 *
 *			DesalocarMatriz(MatrizAutoValor, 3);
 *			DesalocarMatriz(MatrizAutoVetor, 3);
 *			DesalocarMatriz(MatrizRandAV, 3);
 *
 */
	double	**b;

	double	ANORM = 0;
	double	FNORM = 0;
	double	THR   = 0;

	int	N = prmC;
	int fezSort=0;
	int	cntI, cntJ, cntK, cntXX, cntYY;

	double	AL, AM, AO, SINX, SINX2, COSX, COSX2, BT, XT, AT, XX;
	int	IND, I1, flagZ = 1;

	b = AlocarMatriz(prmC, prmC);

	for(cntI=0; cntI < N; cntI++)
		for(cntJ=0; cntJ < N; cntJ++)
			if ( cntI == cntJ ) {
				b[cntI][cntJ] = 1;
			} else {
				b[cntI][cntJ] = 0;
				ANORM += a[cntI][cntJ] * a[cntI][cntJ];
			}

	ANORM = sqrt(ANORM);
	FNORM = ANORM * 0.000000001 / N;
	THR   = ANORM;

	while ( ( THR - FNORM ) > 0 ) {
		THR = THR / N;
		do {
			IND = 0;
			for (cntI = 1; cntI < N; cntI++) {
				I1 = cntI - 1;
				for (cntJ = 0; cntJ <= I1; cntJ++) {
					if ( fabs(a[cntJ][cntI]) - THR < 0.0 ) continue;

					IND = 1;					// Computar SENO e COSENO ...
					AL  = -1 * a[cntJ][cntI];
					AM  = ( a[cntJ][cntJ] - a[cntI][cntI] ) / 2;
					AO  = AL / sqrt(AL * AL + AM * AM );
					if ( AM < 0 )
						AO = -1 * AO;

					SINX  = AO / sqrt(2 * (1 + sqrt(1 - AO * AO)));
					SINX2 = SINX * SINX;
					COSX  = sqrt(1 - SINX2);
					COSX2 = COSX * COSX;

					for ( cntK = 0; cntK < N ; cntK++ ) {	// Rotacionar as colunas cntI e cntJ
			                    if ((cntK - cntJ) != 0)
			                        if ((cntK - cntI) != 0) {
			                            AT = a[cntK][cntJ];
			                            a[cntK][cntJ] = AT * COSX - a[cntK][cntI] * SINX;
			                            a[cntK][cntI] = AT * SINX + a[cntK][cntI] * COSX;
			                        }

			                    BT = b[cntK][cntJ];
			                    b[cntK][cntJ] = BT * COSX - b[cntK][cntI] * SINX;
			                    b[cntK][cntI] = BT * SINX + b[cntK][cntI] * COSX;
					}

					XT = 2 * a[cntJ][cntI] * SINX * COSX;
					AT = a[cntJ][cntJ];
					BT = a[cntI][cntI];

					a[cntJ][cntJ] = AT * COSX2 + BT * SINX2 - XT;
					a[cntI][cntI] = AT * SINX2 + BT * COSX2 + XT;
					a[cntJ][cntI] = (AT - BT) * SINX * COSX + a[cntJ][cntI] * (COSX2 - SINX2);
					a[cntI][cntJ] = a[cntJ][cntI];

					for (cntK=0; cntK<N; cntK++) {
						a[cntJ][cntK] = a[cntK][cntJ];
						a[cntI][cntK] = a[cntK][cntI];
					}
				}
			}
		} while ( IND > 0 );
	}

	// ToDo: 002
	do {
		fezSort=0;
		for (cntI=1; cntI<N; cntI++) {

			if ( flagZ ) cntJ = cntI;

			flagZ = 1;

			if ( (a[cntJ - 1][cntJ - 1] - a[cntJ][cntJ]) < 0 ) {
				AT = a[cntJ - 1][cntJ - 1];
				a[cntJ - 1][cntJ - 1] = a[cntJ][cntJ];
				a[cntJ][cntJ] = AT;

				for (cntK=0; cntK<N; cntK++) {
					AT = b[cntK][cntJ - 1];
					b[cntK][cntJ - 1] = b[cntK][cntJ];
					b[cntK][cntJ] = AT;
				}

				cntJ = cntJ - 1;

				if ((cntJ - 1) > 0) flagZ = 0;

				fezSort=1;
			}
		}
	} while(fezSort);

	for (cntXX=0; cntXX<prmC; cntXX++)
		for (cntYY=0; cntYY<prmD; cntYY++) {
			vetorAVV[cntXX][cntYY]  = a[cntXX][cntYY];
			matrizAVV[cntXX][cntYY] = b[cntXX][cntYY];
		}

	DesalocarMatriz(b, prmD);
}
