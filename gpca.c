/*****
 * Ronaldo Russel, Roger Russel e Robson Pires, 2008.
 * Compressao de imagens atraves do GPCA.
 *
 * ------------------------------------------------------------------------
 * ToDo													Situacao	    Cod
 * ------------------------------------------------------------------------
 * Implementar rotinas de arredondamento ...			[descartado]	001
 * Melhorar metodo de orgalizacao da class. do AVV		[descartado]	002
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "fmatriz.h"												// Funcoes para manipulacoes e calculos com matrizes ...
#include "fimage.h"													// Funcoes para manupulacoes de imagens ...

int	*mtxImageWidth , *mtxImageHeight;
double **mtxImage;													// Matriz Principal (Carga da Imagem)

int mtxWidth=0, mtxHeight=0, mtxFwidth=0, mtxFheight=0;

int main(int argc, char **argv) {

	double **R, **AV, **ML, **subML, **MR, **subMR, **MAtil,**subM, **M,**Atil, **AtilT, **subTrans,**L, **subFro, **Dn, **matrizSwap;

	double *mediana;

	double	RMSE[]={0.0,0.0},
			erro=0.0,
			dblSwap=0.0;

	int		intSwap=0;

	int subMatrizD = 0,
		xSubMatriz = 0,
		ySubMatriz = 0;

	int cont   = 0,													// Contadores
	    contM  = 0,
	    contL  = 0,
	    contC  = 0,
	    contSC = 0,
	    contSL = 0,
	    contD1 = 0,
	    contD2 = 0,
	    contMC = 0,
	    contML = 0,
		contI  = 0;

	FILE *ARQUIVO;

	int	DEBUG	= 0, // Padrao: Sem Debug ...
		LTIPO	= 0, // Tipo inicial da Matriz L ( Identidade ou Randomica ) ...
		MMEDIA	= 0, // Tipo de Media ( media ou mediana ) ...
		TIPODD	= 0, // Tipo de Dado Salvo no Arquivo 4r, Padrao: Duplo ...
		DUPLO	= 0,
		INTEIRO	= 1;

	system("clear");

 // Validacao dos argumentos ...
	if ( argc < 7 ) {
		printf("USO: gpca.bin arquivo.de.entrada.bmp arquivo.de.saida.4r D X Y TipoMedia tipoL TipoDeDado <debug>\n\n");
		printf("-----------------------------------------------------------------------------------------\n");
		printf("Parametros\n");
		printf("-----------------------------------------------------------------------------------------\n");
		printf("E             :\tO erro minimo do RMSE (Ex: 0.05)\n");
		printf("D             :\tTamanho do Elemento D, para a compressao.\n");
		printf("X             :\tLargura da SubMatriz.\n");
		printf("Y             :\tAltura da SubMatriz.\n");
		printf("Media         :\tMetodo de calculo da media inicial (media | mediana). \n");
		printf("TipoL         :\tTipo de matriz inicial L que sera utilizada (identidade | randomica).\n");
		printf("Tipo de Dado  :\tFormato como o arquivo sera salvo ( duplo | inteiro ).\n");
		printf("-----------------------------------------------------------------------------------------\n\n");
		printf("Exemplo: ./gpca.bin matrix2.bmp imagem.4r 0.05 2 3 3 media identidade inteiro debug\n\n");
		exit(1);
	}

    erro        = atof(argv[3]);                					// Erro
    subMatrizD  = atoi(argv[4]);                 					// Tamanho dos Ds
    xSubMatriz  = atoi(argv[5]);                 					// Larguca da SubMatriz
    ySubMatriz  = atoi(argv[6]);                 					// Altura da SubMatriz

	if ( strcmp(argv[7], "mediana")   == 0 ) MMEDIA = 1;
	if ( strcmp(argv[8], "randomica") == 0 ) LTIPO = 1;
	if ( strcmp(argv[9], "inteiro")   == 0 ) TIPODD = INTEIRO;

	if ( argc == 11 ) if ( strcmp(argv[10], "debug") == 0 ) DEBUG = 1;
	
 // Carga da Imagem na Matriz ...
    printf("\nCarga da Imagem na Matriz ... " );
	mtxImage = load_image_to_matrix(argv[1], &mtxImageWidth, &mtxImageHeight);
    
	if ( mtxImage == NULL ) {
		printf("Erro durante a carga da matriz ...\n\n");
		exit(99);
	}

 // Abre arquivo de saida dos dados ...
	if ( DEBUG ) {
		if ( ( ARQUIVO = fopen(argv[2],"w")) == NULL ) {
			printf("Erro abrindo arquivo %s\n", argv[2]);
			exit(1);
		} 	
	} else {
		if ( ( ARQUIVO = fopen(argv[2],"wb")) == NULL ) {
			printf("Erro abrindo arquivo %s\n", argv[2]);
			exit(1);
		} 	
	}

 // Descobre a largura da Matriz
	if ( mtxImageWidth == 0 ){
		mtxWidth  = 0;
		mtxFwidth = 0;
	} else { 
		mtxWidth  = (int)mtxImageWidth;
		mtxFwidth = mtxWidth - xSubMatriz;
		mtxWidth /= xSubMatriz;   
	}

 // Descobre a altura da Matriz
	if ( mtxImageHeight == 0 ) {
		mtxHeight  = 0;
		mtxFheight = 0;
	} else {
		mtxHeight  = (int)mtxImageHeight;
		mtxFheight = mtxHeight - ySubMatriz;
		mtxHeight /= ySubMatriz;
	}
	
 // Alocacao de Matrizes ...
	printf("\nAlocacao das matrizes ...\n");

	M			= AlocarMatriz(xSubMatriz, ySubMatriz);
	Atil		= AlocarMatriz((int)mtxImageWidth,(int)mtxImageHeight);
	AtilT		= AlocarMatriz((int)mtxImageWidth,(int)mtxImageHeight);
	L			= AlocarMatriz(subMatrizD, xSubMatriz);
	R			= AlocarMatriz(subMatrizD, ySubMatriz);
	AV			= AlocarMatriz(xSubMatriz, ySubMatriz);
	MR			= AlocarMatriz(xSubMatriz, ySubMatriz);
	ML			= AlocarMatriz(xSubMatriz, ySubMatriz);
	subM   		= AlocarMatriz(xSubMatriz, ySubMatriz);
	matrizSwap	= AlocarMatriz(xSubMatriz, ySubMatriz);
	
	mediana		= malloc((mtxFwidth * mtxFheight) * sizeof(double)); // ToDo o espaco alocado e maior do que o necessario,
																	 // pois deve ser dividido pelo tamanho das submatrizes.
	printf("\nMedia das submatrizes ...\n");

	if ( DEBUG ) {
		fprintf(ARQUIVO,"\n= = = = =[ DADOS ]= = = = = = = = = = = = = = = = = = = = = = = = =\n\n");
		fprintf(ARQUIVO,"Largura    : %4d\n", mtxImageWidth);
		fprintf(ARQUIVO,"Altura     : %4d\n\n", mtxImageHeight);

		fprintf(ARQUIVO,"Matriz    A: ");

		for( contL=0; contL<(int)mtxImageHeight;contL++){
			fprintf(ARQUIVO,"|");

			for ( contC=0; contC<(int)mtxImageWidth; contC++ )
				fprintf(ARQUIVO,"%4d ", (int)mtxImage[contL][contC]);

			fprintf(ARQUIVO,"|\n             ");
		}
		fprintf(ARQUIVO,"\n");
	} else {
		fprintf(ARQUIVO, "%s", "4R");
		fwrite(&mtxImageWidth,  4, 1, ARQUIVO); // Largura da Imagem ...
		fwrite(&mtxImageHeight, 4, 1, ARQUIVO); // Altura da Imagem ...
		fwrite(&subMatrizD,     4, 1, ARQUIVO); // Tamanho do D ...
		fwrite(&xSubMatriz,     4, 1, ARQUIVO); // Largura SubMatriz ...
		fwrite(&ySubMatriz,     4, 1, ARQUIVO); // Altura SubMatriz ...
		fwrite(&TIPODD,         4, 1, ARQUIVO); // Tipo do Dado Salvo ...
	}

	if ( MMEDIA == 0 ) {

		// Calcula a Media das SubMatrizes ...
		for( contL=0; contL<ySubMatriz;contL++){
			for ( contC=0; contC<xSubMatriz; contC++ ){
				M[contL][contC]=0;
				for (contMC=0;contMC<= mtxFwidth;contMC +=xSubMatriz){
					for ( contML=0;contML<=mtxFheight;contML +=ySubMatriz){
						M[contL][contC] += mtxImage[contL+contML][contC+contMC];
					}
				}
		
				M[contL][contC] /= mtxWidth*mtxHeight;
				dblSwap = M[contL][contC];
				if ( !DEBUG ) {
					fwrite(&dblSwap, 8, 1, ARQUIVO);
				}
			}
		}

	} else {

		// Calcula a Mediana das SubMatrizes ...
		for( contL=0; contL<ySubMatriz;contL++){
			for ( contC=0; contC<xSubMatriz; contC++ ){
				M[contL][contC]=0;
				cont = 0;
				mediana[cont] = 0.0;

				for (contMC=0;contMC<= mtxFwidth;contMC +=xSubMatriz){
					for ( contML=0;contML<=mtxFheight;contML +=ySubMatriz){
						mediana[cont] = mtxImage[contL+contML][contC+contMC];
						cont++;
					}
				}

				Quicksort(mediana,0,cont-1);								// cont -1 por que e o valor de todas as
				M[contL][contC] = CalcMediana(mediana,cont-1); 				// iteracoes -1 por que comeca do zero...
		
				dblSwap = M[contL][contC];
				if ( !DEBUG ) {
					fwrite(&dblSwap, 8, 1, ARQUIVO);
				}
			}
		}

	}

	if ( DEBUG ) {
		fprintf(ARQUIVO,"\nMatriz    M: ");
		for( contL=0; contL<(int)ySubMatriz;contL++){
			fprintf(ARQUIVO,"|");

			for ( contC=0; contC<(int)xSubMatriz; contC++ )
				fprintf(ARQUIVO,"%4d ", (int)M[contL][contC]);

			fprintf(ARQUIVO,"|\n             ");
		}
		fprintf(ARQUIVO,"\n");
	}

 // Calcula a Matriz Atil ...
	printf("\nMatrizes Atil ...");
	contSL = contSC = 0;
	for( contC=0; contC<(int)mtxImageWidth;contC++){
		for ( contL=0; contL<(int)mtxImageHeight; contL++ ) {
			Atil[contL][contC] = mtxImage[contL][contC] - M[contSL][contSC];
			contSL++;
			if (contSL >= ySubMatriz) contSL=0;
		}
		contSC++;
		if (contSC >= xSubMatriz) contSC=0;
	}

	if ( DEBUG ) {
		fprintf(ARQUIVO,"\nMatriz Atil: ");
		for( contL=0; contL<(int)mtxImageHeight;contL++){
			fprintf(ARQUIVO,"|");

			for ( contC=0; contC<(int)mtxImageWidth; contC++ )
				fprintf(ARQUIVO,"%4d ", (int)Atil[contL][contC]);

			fprintf(ARQUIVO,"|\n             ");
		}
		fprintf(ARQUIVO,"\n");
	}

 // Obtem Matriz Tranposta da Atil
	printf("\nMatriz Tranposta A~ ...");

	cont = contSL = contSC = 0;  
	for ( contML=0;contML<mtxHeight;contML++){
		for ( contMC=0;contMC<mtxWidth;contMC++){
			subTrans  = TransposicaoMatriz(ObterSubMatriz(Atil, xSubMatriz, ySubMatriz, contMC, contML), xSubMatriz, ySubMatriz);

			for (contC=0;contC<xSubMatriz;contC++){
				for (contL=0;contL<ySubMatriz;contL++){
					AtilT[contSL+cont][contSC]=subTrans[contL][contC];

					contSL++;
					if ( contSL >= ySubMatriz ) contSL = 0 ;
				}

				if ( contSC < (int)mtxImageWidth - 1) {
					contSC++;
				} else {
					contSC= 0;
					cont += xSubMatriz;
				}
			}

			DesalocarMatriz(subTrans,xSubMatriz);
		}
	}

	if ( DEBUG ) {
		fprintf(ARQUIVO,"\nMatriz AtiT: ");
		for( contL=0; contL<(int)mtxImageHeight;contL++){
			fprintf(ARQUIVO,"|");

			for ( contC=0; contC<(int)mtxImageWidth; contC++ )
				fprintf(ARQUIVO,"%4d ", (int)AtilT[contL][contC]);

			fprintf(ARQUIVO,"|\n             ");
		}
		fprintf(ARQUIVO,"\n");
	}

 // Inicializacao de L  ...
	if ( LTIPO == 1 )
		RandomizarMatriz(L, subMatrizD,ySubMatriz, 256);
	else
		L = RetornaMatrizIdentidade(ySubMatriz,subMatrizD);

 // Calculo do RMSE
	printf("\nExecutando Compressao (Calculo do RMSE) ...\n");
	printf("Iteracao:\n");

	if ( DEBUG ) fprintf(ARQUIVO,"Inicio do Loop para calculo do RMSE!\n\n");

	do {
		contI++;
		printf("%d ...\n", contI);

		if ( DEBUG ) fprintf(ARQUIVO,"\nRMSE - Iteracao %03d\n\n", contI);

		RMSE[0] = RMSE[1];
		RMSE[1] = 0.0;

	 // Calculo de MR ...
		for (contC=0;contC<xSubMatriz;contC++)
			for (contL=0;contL<ySubMatriz;contL++)
				MR[contL][contC]=0.0;

		for (contML=0;contML<mtxHeight;contML++){
			for (contMC=0;contMC<mtxWidth;contMC++){

				subMR  = ObterSubMatriz(AtilT, xSubMatriz, ySubMatriz, contMC, contML);
				subMR  = MultiplicacaoMatriz('r',subMR, xSubMatriz, ySubMatriz,L,subMatrizD,xSubMatriz);																			//  AtilT * Lo
				subMR  = MultiplicacaoMatriz('r',subMR, subMatrizD, xSubMatriz,TransposicaoMatriz(L, subMatrizD, xSubMatriz), xSubMatriz, subMatrizD); 							// (AtilT * Lo ) * LoT
				subMR  = MultiplicacaoMatriz('r',subMR, xSubMatriz, ySubMatriz,ObterSubMatriz(Atil, xSubMatriz, ySubMatriz, contMC, contML), xSubMatriz, ySubMatriz);	// (AtilT * Lo   * LoT) * Atil]

				for (contC=0;contC<xSubMatriz;contC++)
					for (contL=0;contL<ySubMatriz;contL++)
						MR[contL][contC]+=subMR[contL][contC];

				DesalocarMatriz(subMR, ySubMatriz);
			}
		}

		if ( DEBUG ) {
			fprintf(ARQUIVO,"\n             Matriz   MR: ");
			for( contL=0; contL<(int)ySubMatriz;contL++){
				fprintf(ARQUIVO,"|");

				for ( contC=0; contC<(int)xSubMatriz; contC++ )
					fprintf(ARQUIVO,"%10f ", MR[contL][contC]);

				fprintf(ARQUIVO,"|\n                          ");
			}
			fprintf(ARQUIVO,"\n");
		}

		AutoVV(MR, R, AV, subMatrizD, ySubMatriz);

		if ( DEBUG ) {
			fprintf(ARQUIVO,"\n             Matriz    R: ");
			for( contL=0; contL<(int)xSubMatriz;contL++){
				fprintf(ARQUIVO,"|");

				for ( contC=0; contC<(int)subMatrizD; contC++ )
					fprintf(ARQUIVO,"%10f ", R[contL][contC]);

				fprintf(ARQUIVO,"|\n                          ");
			}
			fprintf(ARQUIVO,"\n");
		}

	 // Calculo do ML
		for (contC=0;contC<xSubMatriz;contC++)						// Prepara a matriz ML
			for (contL=0;contL<ySubMatriz;contL++)
				ML[contL][contC] = 0.0;

		for (contML=0;contML<mtxHeight;contML++){
			for (contMC=0;contMC<mtxWidth;contMC++){

				subML = ObterSubMatriz(Atil, xSubMatriz, ySubMatriz, contMC, contML);
				subML = MultiplicacaoMatriz('r',subML, xSubMatriz, ySubMatriz,R, subMatrizD, ySubMatriz);
				subML = MultiplicacaoMatriz('r',subML,subMatrizD , xSubMatriz,TransposicaoMatriz(R,subMatrizD , ySubMatriz), ySubMatriz, subMatrizD);
				subML = MultiplicacaoMatriz('r',subML, xSubMatriz, ySubMatriz,ObterSubMatriz(AtilT, xSubMatriz, ySubMatriz, contMC, contML), xSubMatriz, ySubMatriz);

				for (contC=0;contC<xSubMatriz;contC++)
					for (contL=0;contL<ySubMatriz;contL++)
						ML[contL][contC] += subML[contL][contC];
			}
		}

		DesalocarMatriz(subML, xSubMatriz);    					

		if ( DEBUG ) {
			fprintf(ARQUIVO,"\n             Matriz   ML: ");
			for( contL=0; contL<(int)ySubMatriz;contL++){
				fprintf(ARQUIVO,"|");

				for ( contC=0; contC<(int)xSubMatriz; contC++ )
					fprintf(ARQUIVO,"%10f ", ML[contL][contC]);

				fprintf(ARQUIVO,"|\n                          ");
			}
			fprintf(ARQUIVO,"\n");
		}

		AutoVV(ML, L, AV, subMatrizD, xSubMatriz);

		if ( DEBUG ) {
			fprintf(ARQUIVO,"\n             Matriz    L: ");
			for( contL=0; contL<(int)ySubMatriz;contL++){
				fprintf(ARQUIVO,"|");

				for ( contC=0; contC<(int)subMatrizD; contC++ )
					fprintf(ARQUIVO,"%10f ", L[contL][contC]);

				fprintf(ARQUIVO,"|\n                          ");
			}
			fprintf(ARQUIVO,"\n");
		}

		for ( contML=0;contML<mtxHeight;contML++){
			for (contMC=0;contMC<mtxWidth;contMC++){

				subFro = MultiplicacaoMatriz('n', L, subMatrizD, xSubMatriz, TransposicaoMatriz(L, subMatrizD, xSubMatriz), xSubMatriz, subMatrizD);						//   L * LT
				subFro = MultiplicacaoMatriz('r', subFro, xSubMatriz, ySubMatriz, ObterSubMatriz(Atil, xSubMatriz, ySubMatriz, contMC, contML), xSubMatriz, ySubMatriz); 	// ( L * LT ) * Atil
				subFro = MultiplicacaoMatriz('r', subFro, xSubMatriz, ySubMatriz, R, subMatrizD, ySubMatriz);																// ( L * LT   * Atil ) * R
				subFro = MultiplicacaoMatriz('r', subFro, subMatrizD, ySubMatriz, TransposicaoMatriz(R, subMatrizD, ySubMatriz), ySubMatriz, subMatrizD);					// ( L * LT   * Atil   * R ) * RT

				subFro = SubtraiMatriz('r', subFro, ObterSubMatriz(Atil, xSubMatriz, ySubMatriz, contMC, contML), xSubMatriz, ySubMatriz);									// Atil - ( L * LT * Atil  * R  * RT )

				RMSE[1] += ModuloMatriz(subFro, xSubMatriz, ySubMatriz, 'f');
	
				DesalocarMatriz(subFro, ySubMatriz);
			}
		}

		RMSE[1] /= mtxWidth * mtxHeight;

		if ( DEBUG ) fprintf(ARQUIVO,"\n             Erro   RMSE: %10.8f\n\n", fabs(RMSE[0] - RMSE[1]));

	} while ( fabs(RMSE[0] - RMSE[1]) > erro );

	printf("\nTermino da Compressao. RMSE: %20.18f ...", fabs(RMSE[0] - RMSE[1]));
	printf("\nGravando dados no arquivo de saida ...");

	if ( DEBUG ) fprintf(ARQUIVO,"\nR = {");

	for ( contL=0;contL<ySubMatriz;contL++){
		for ( contC=0;contC<subMatrizD;contC++){
			if ( DEBUG ) {
				fprintf(ARQUIVO,"%10.4f",R[contL][contC]);
				if ( contC < subMatrizD - 1 ) fprintf(ARQUIVO,",");
			} else {
				dblSwap = R[contL][contC];
				fwrite(&dblSwap, 8, 1, ARQUIVO);
			}
		}

		if ( DEBUG ) if ( contL < subMatrizD ) fprintf(ARQUIVO,";");
	}

	if ( DEBUG ) {
		fprintf(ARQUIVO,"}");
		fprintf(ARQUIVO,"\nL = {");
	}

	for ( contL=0;contL<ySubMatriz;contL++){
		for ( contC=0;contC<subMatrizD;contC++){
			if ( DEBUG ) {
				fprintf(ARQUIVO,"%10.4f",L[contL][contC]);
				if ( contC < subMatrizD - 1 ) fprintf(ARQUIVO,",");
			} else {
				dblSwap = L[contL][contC];
				fwrite(&dblSwap, 8, 1, ARQUIVO);
			}
		}

		if ( DEBUG ) if ( contL < subMatrizD ) fprintf(ARQUIVO,";");
	}

	if ( DEBUG ) fprintf(ARQUIVO,"}");

	cont=0;

	for (contML=0;contML<mtxHeight;contML++){	
		for (contMC=0;contMC<mtxWidth;contMC++){	

			cont++;

			Dn =  MultiplicacaoMatriz('n', TransposicaoMatriz(L, subMatrizD, xSubMatriz), xSubMatriz, subMatrizD, ObterSubMatriz(Atil, xSubMatriz, ySubMatriz, contMC, contML), xSubMatriz, ySubMatriz);
			Dn =  MultiplicacaoMatriz('r', Dn, xSubMatriz, subMatrizD, R, subMatrizD, ySubMatriz); // Erro ... verificar desalocacao desta matriz ...

			if ( DEBUG ) fprintf(ARQUIVO,"\nD = {");

			for ( contL=0;contL<subMatrizD;contL++){
				for ( contC=0;contC<subMatrizD;contC++){
					if ( DEBUG ) {
						if ( TIPODD == DUPLO )
							fprintf(ARQUIVO,"%10.4f",Dn[contL][contC]);
						else
							fprintf(ARQUIVO,"%10d",(int)(Dn[contL][contC] * 1000000.0));

						if ( contC < 1 ) fprintf(ARQUIVO,",");
					} else {
						if ( TIPODD == DUPLO ) {
							dblSwap = Dn[contL][contC];	
							fwrite(&dblSwap, 8, 1, ARQUIVO);
						} else {
							intSwap = (int)(Dn[contL][contC] * 1000000.0);	
							fwrite(&intSwap, 4, 1, ARQUIVO);
						}
					}
				}
				if ( DEBUG ) if ( contL < 1 ) fprintf(ARQUIVO,";");
			}

			if ( DEBUG ) fprintf(ARQUIVO,"}");
		}
	}

	fprintf(ARQUIVO,"\n\n");
	fclose(ARQUIVO);

	printf("\n\nProcesso concluido ...\n\n\n");

	DesalocarMatriz(MR, ySubMatriz);								// Desaloca MR
	DesalocarMatriz(ML, xSubMatriz);								// Desalocar ML

//  ToDo: Verificar erro na desalocacao destas submatrizes ...
//	DesalocarMatriz(AV, 2);	
//	DesalocarMatriz(R,  2);											// Desalocar R
//	DesalocarMatriz(L,  2);											// Desalocar L

	DesalocarMatriz(M,xSubMatriz);									// Desalocar M
	DesalocarMatriz(Atil,xSubMatriz);
	DesalocarMatriz(AtilT,ySubMatriz);
		
	exit(0);
}
