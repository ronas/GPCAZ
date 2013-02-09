/*****
 * Ronaldo Russel, Roger Russel e Robson Pires, 2008.
 * Compressao de imagens atraves do GPCA.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "fmatriz.h"												// Funcoes para manipulacoes e calculos com matrizes ...
#include "fimage.h"													// Funcoes para manipulacoes de imagens ...

int main(int argc, char **argv) {

	double **R,				// Matriz R
		   **L, 			// Matriz L
		   **M, 			// Matriz Medias
		   **D, 			// Matriz D
		   **O; 			// Matriz Resultado, Dados [O]riginais

	double **I;				// Matriz da Imagem restaurada ...
	
	short fType;

	double qtdeD   = 0.0,											// Quantidade de Ds existentes no arquivo ...
		   qtdeX   = 0.0,											// Quantidade de Submatrizes no Eixo X ..
		   qtdeY   = 0.0;											// Quantidade de Submatrizes no Eixo Y ..
	
	int	contC      = 0,
		contL      = 0,
		contG	   = 0,												// Contador Generico ...
		contX	   = 0,												// Eixo X Matriz Final
		contY	   = 0;												// Eixo y Matriz Final

	int tamWidth   = 0,
		tamHeight  = 0,
		tamD       = 0,
    	tamX       = 0,
    	tamY       = 0,
		tmpWidth   = 0;

	int	DEBUG     = 0,
		TIPODD    = 0,
		DUPLO     = 0,
		INTEIRO   = 1;

	int	intSwap   = 0;

	FILE	*f4R, fBMP;												// Ponteiro para o arquivo de imagem ...

	// Validacao dos argumentos ...
	if ( argc < 3 ) {
		printf("\n\n");
		printf("USO: 4r2bmp.bin arquivo.de.entrada.4r arquivo.de.saida.bmp [debug]\n");
		printf("Exemplo: ./4r2bmp.bin imagem.4r imagem.bmp\n");
		printf("\n\n");
		exit(1);
	}

	if ( argc == 4 ) if ( strcmp(argv[3], "debug") == 0 ) DEBUG = 1;

	// Limpa a tela ... ToDo: Procurar metodo melhor e que funcione em qq S.O.
	if ( DEBUG ) for(contG=0;contG<24;contG++) printf("\n\n");

	// Abertura do arquivo de entrada ...
	if ( ( f4R = fopen(argv[1],"rb")) == NULL ) {
		printf("Erro abrindo arquivo %s.", argv[1]);
		exit(1);
	}

	// Obtem dados do cabecalho do arquivo ...
	fread(&fType,2,1,f4R);
	fread(&tamWidth,4,1,f4R);
	fread(&tamHeight,4,1,f4R);
	fread(&tamD,4,1,f4R);
	fread(&tamX,4,1,f4R);
	fread(&tamY,4,1,f4R);
	fread(&TIPODD,4,1,f4R);

	if ( DEBUG ) {
			printf("\nDados do Cabecalho:\t");
			printf("Tipo   : %x\n", fType);
			printf("\t\t\tLargura: %d\n", tamWidth);
			printf("\t\t\tAltura : %d\n", tamHeight);
			printf("\t\t\tD      : %d\n", tamD);
			printf("\t\t\tX      : %d\n", tamX);
			printf("\t\t\tY      : %d\n", tamY);
	}

	// Valida caracteres iniciais do arquivo ( Magic Number: 4R ) ...
	if ( fType != 0x5234 ) {
		printf("O arquivo informado nao e um arquivo 4r.\n");
		exit(2);
	}

	if ( tamWidth % 4 > 0 ) 
		for(tmpWidth=tamWidth;tmpWidth % 4;tmpWidth++);
	else
		tmpWidth=tamWidth;

	// Aloca memoria para armazenar as matrizes iniciais ...
	M = AlocarMatriz(tamX, tamY);
	R = AlocarMatriz(tamD, tamY);
	L = AlocarMatriz(tamD, tamX);
	D = AlocarMatriz(tamD, tamD);
	O = AlocarMatriz(tamX, tamY);

	// Aloca memoria para armazenar a matriz final ...
	I = AlocarMatriz(tmpWidth, tamHeight);

	// Carga da Matriz das Medias ...
	for (contL=0;contL<tamY;contL++) 
		for (contC=0;contC<tamX;contC++) fread(&M[contL][contC],8,1,f4R);
	
	if ( DEBUG ) {
		printf("\n\nCarga da Matriz M:\t");
		for (contL=0;contL<tamY;contL++) {
			for (contC=0;contC<tamX;contC++) printf("%10.4f",M[contL][contC]);
			printf("\n\t\t\t");
		}
	}

	// Carga da Matriz R ...
	for (contL=0;contL<tamY;contL++) 
		for (contC=0;contC<tamD;contC++) fread(&R[contL][contC],8,1,f4R);

	if ( DEBUG ) {
		printf("\n\nCarga da Matriz R:\t");
		for (contL=0;contL<tamY;contL++) {
			for (contC=0;contC<tamD;contC++) printf("%10.4f ",R[contL][contC]);
			printf("\n\t\t\t");
		}
	}

	// Carga da Matriz L ...
	for (contL=0;contL<tamX;contL++) 
		for (contC=0;contC<tamD;contC++) fread(&L[contL][contC],8,1,f4R);

	if ( DEBUG ) {
		printf("\n\nCarga da Matriz L:\t");
		for (contL=0;contL<tamX;contL++) {
			for (contC=0;contC<tamD;contC++) printf("%10.4f ",L[contL][contC]);
			printf("\n\t\t\t");
		}
	}
	
	// Tratamento para os Ds ... 
	qtdeD = (tamWidth/tamX) * (tamHeight/tamY);
	qtdeX = (tamWidth/tamX);
	qtdeY = (tamHeight/tamY);
	
	for (contG=0;contG<(int)qtdeD;contG++)  {
		
		// Ler informacao do arquivo ...
		if ( TIPODD == DUPLO )
			for (contL=0;contL<tamD;contL++)
				for (contC=0;contC<tamD;contC++) fread(&D[contL][contC],8,1,f4R);
		else
			for (contL=0;contL<tamD;contL++)
				for (contC=0;contC<tamD;contC++) {
					fread(&intSwap,4,1,f4R);
					D[contL][contC]=((double)intSwap)/1000000.0;
				}

   		// Aplicacao da formula reversa: AtilN = L * D * Rt 
		O = MultiplicacaoMatriz('n', L, tamD, tamX, D, tamD, tamD);
		O = MultiplicacaoMatriz('r', O, tamD, tamY, TransposicaoMatriz(R, tamD, tamY), tamY, tamD);

		// Retorno da Media dos elementos: A = AtilN + M
		for (contL=0;contL<tamY;contL++)
	   		for (contC=0;contC<tamX;contC++) O[contL][contC]+=M[contL][contC];
   		
		// Armazenamento do valor na matriz final ...
		for (contL=0;contL<tamY;contL++)
	   		for (contC=0;contC<tamX;contC++)
				I[contL + (tamY*contY)][contC + (tamX*contX)] = O[contL][contC];

		if (contX == (int)qtdeX - 1) {
			contX=0;
			contY++;
		} else
			contX++;

		if ( DEBUG ) {
			printf("\n\n\n\nCarga da Matriz D:\t");
			for (contL=0;contL<tamD;contL++) {
					for (contC=0;contC<tamD;contC++) printf("%10.4f ",D[contL][contC]);
					printf("\n\t\t\t");
			}
			printf("\n         Matriz O:\t");
      		for (contL=0;contL<tamX;contL++) {
    	   		for (contC=0;contC<tamY;contC++) {
					printf("%10.4f ",O[contL][contC]);
				}
				printf("\n\t\t\t");
      		}
		}

		DesalocarMatriz(O,tamX);
	}

	if ( DEBUG ) {
		printf("\nMatriz Final    I:\t");
		for (contL=0;contL<tamHeight;contL++) {
			for (contC=0;contC<tmpWidth;contC++) {
					printf("%d ",(int)I[contL][contC]);
			}
			printf("\n\t\t\t");
		}
	} 

	// Descarregar imagem em arquivo ...
	if ( save_matrix_to_file(I, tamWidth, tamHeight, argv[2]) != 0 ) {
		printf("\nHouve um erro no processo de salvamento da imagem no disco ...\n");
		exit(44);
	}

	DesalocarMatriz(M,tamX);
	DesalocarMatriz(R,tamY);
	DesalocarMatriz(L,tamX);
	DesalocarMatriz(D,tamD);

	fclose(f4R);

	if ( DEBUG ) printf("\n\n");

	exit(0);
}

