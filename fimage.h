double **load_image_to_matrix(char *arquivo, int *p_width, int *p_height);					// Carga Inicial da imagem em uma Matriz ...
int    save_matrix_to_file(double **p_matrix, int p_width, int p_height, char p_file[1000]);
int    **allocate_matrix_to_image(int p_width, int p_height);								// Aloca memoria para armazenamento de imagem ...

double **load_image_to_matrix(char *p_file, int *p_width, int *p_height) {
/*
 * Funcao para carregar uma imagem em uma Matriz.
 *
 * OBS: Os dados carregados ja sao convertidos para o formato RGB e as linhas
 * invertidas para o formato natural (esquerda para a direita e de cima para baixo).
 *
 * Parametros:
 *
 *	*p_file		Caminho do arquivo que sera carregado.
 *	*p_width	Variavel que recebera a largura da matriz que foi carregada.
 *	*p_height	Variavel que recebera a altura da matriz que foi carregada.
 *
 * Retorno:		A funcao retorna um ponteiro para a matriz que foi carregada.
 *
 * Exemplo:
 *
 *	int	**mtxImage, mtxImageWidth, mtxImageHeight;
 *
 *	int main(int argc, char **argv) {
 *
 *		mtxImage = load_image_to_matrix(argv[1], &mtxImageWidth, &mtxImageHeight);
 *		if ( mtxImage == NULL ) {
 *			printf("Erro durante a carga da matriz ...\n\n");
 *			exit(99);
 *		}
 *
 *		printf("\n\nMatriz:\n\n");
 *		ImprimirMatriz(mtxImage, mtxImageWidth, mtxImageHeight);
 *
 *		DesalocarMatriz(mtxImage, mtxImageHeight);
 *
 *	}
 */


	typedef unsigned short WORD;
	typedef unsigned long  DWORD;

	typedef struct tagBitmapFileHeader	// Cabecalho do Arquivo ...
	{
		WORD	bfType;
		DWORD   bfSize;
		WORD 	bfReserved1;
		WORD 	bfReserved2;
		DWORD   bfOffBits;
	} BitmapFileHeader;

	typedef struct tagBitmapInfoHeader	// Cabecalho da Imagem ...
	{
		DWORD   biSize;
		DWORD   biWidth;
		DWORD   biHeight;
		WORD 	biPlanes;
		WORD 	biBitCount;
		DWORD   biCompression;
		DWORD   biSizeImage;
		DWORD   biXPelsPerMeter;
		DWORD   biYPelsPerMeter;
		DWORD   biClrUsed;
		DWORD   biClrImportant;
	} BitmapInfoHeader;

	BitmapFileHeader FH;				// Variaveis de estrutura dos dados do arquivo ...
	BitmapInfoHeader IH;

	FILE	*fBMP;						// Ponteiro para o arquivo de imagem ...
	int	*ImagemBitmap;					// Ponteiro para a memoria alocada para receber a imagem ...
	
	int	varByte;
	int	cnt;

	int cp, cl=0, cc=0, bs, bi;
	
	double	**retMatrix;				// Matriz de Retorno ...

     // Abre o arquivo ...

	if ( ( fBMP = fopen(p_file,"rb")) == NULL ) {
		printf("Erro abrindo arquivo %s.", p_file);
		exit(1);
	}


     // Obtem dados do cabecalho do arquivo ...

	fread(&FH.bfType,2,1,fBMP);
	fread(&FH.bfSize,4,1,fBMP);
	fread(&FH.bfReserved1,2,1,fBMP);
	fread(&FH.bfReserved2,2,1,fBMP);
	fread(&FH.bfOffBits,4,1,fBMP);

	fread(&IH.biSize,4,1,fBMP);
	fread(&IH.biWidth,4,1,fBMP);
	fread(&IH.biHeight,4,1,fBMP);
	fread(&IH.biPlanes,2,1,fBMP);
	fread(&IH.biBitCount,2,1,fBMP);
	fread(&IH.biCompression,4,1,fBMP);
	fread(&IH.biSizeImage,4,1,fBMP);
	fread(&IH.biXPelsPerMeter,4,1,fBMP);
	fread(&IH.biYPelsPerMeter,4,1,fBMP);
	fread(&IH.biClrUsed,2,1,fBMP);
	fread(&IH.biClrImportant,2,1,fBMP);


     // Valida caracteres iniciais do arquivo ( Magic Number: BM ) ...

	if ( FH.bfType != 0x4d42 ) {
		printf("O arquivo informado nao e um arquivo BMP.");
		exit(2);
	}

   
     // Aloca memoria para armazenar os dados da imagem ...

	ImagemBitmap = malloc(sizeof(double) * IH.biSizeImage);
	if (!ImagemBitmap) {
		free(ImagemBitmap);
		fclose(fBMP);
		printf("Nao foi possivel alocar a memoria para armazenar a imagem ...");
		exit(3);
	}

     // Mover o cursor para o inicio dos dados da imagem ...

	fseek(fBMP, FH.bfOffBits, SEEK_SET);

  
     // Carrega dados da imagem no ponteiro ...
    
    
	for(cnt = 0; cnt < IH.biSizeImage; cnt++) {
		fread(&varByte, 1, 1, fBMP);
		ImagemBitmap[cnt] = varByte;
	}

	
	

     // Apresenta dados na tela ...
	printf("\n");


     // Carga da Imagem na Matriz, Funcao de Vetorizacao ................................................................................
	retMatrix = AlocarMatriz(IH.biWidth * 3, IH.biHeight);							// Alocar uma matriz para receber a imagem, Colunas X Linhas ... RGB*3
	if(!retMatrix)
	{
		printf("Erro de Alocacao da Matriz!\n");
		return(NULL);
	}

	if ( IH.biWidth % 4 > 0 ) {
		bs = IH.biWidth % 4;
		bi = ( 3 * IH.biWidth) + bs;
	} else {
		bi = bs = 3 * IH.biWidth;
	}

	cl=0;	

	for(cp=IH.biHeight; cp > 0; cp--) {												// Carrega a imagem na Matriz, respeitando o conjunto RGB ...
		for(cc=0;cc<IH.biWidth;cc++) {
			retMatrix[cl][((cc * 3) + 0)] = (double)ImagemBitmap[ (bi * (cp - 1)) + (cc * 3) + 2 ];
			retMatrix[cl][((cc * 3) + 1)] = (double)ImagemBitmap[ (bi * (cp - 1)) + (cc * 3) + 1 ];
			retMatrix[cl][((cc * 3) + 2)] = (double)ImagemBitmap[ (bi * (cp - 1)) + (cc * 3)     ];
		}

		cl++;
	}

     // Limpa dados e fecha arquivos abertos ...
	fclose(fBMP);
	free(ImagemBitmap);

     // Exporta dados do cabecalho da imagem ...
	*p_width  = IH.biWidth * 3;
	*p_height = IH.biHeight;

	return retMatrix;
}

int **allocate_matrix_to_image(int p_width, int p_height) {
/*
 * Funcao para alocar uma matriz para a recepcao de uma imagem.
 */
	
	int	**ImagemBitmap;
	
	// Aloca memoria para armazenar os dados da imagem ...

	if ( p_width % 4 > 0 ) for(;p_width % 4;p_width++);

	ImagemBitmap = malloc(sizeof(int) * p_width);
	if (!ImagemBitmap) {
		free(ImagemBitmap);
		printf("Nao foi possivel alocar a memoria para armazenar a imagem ...");
		exit(33);
	}
	
	return ImagemBitmap;
}

int save_matrix_to_file(double **p_matrix, int p_width, int p_height, char p_file[1000]) {
/*
 * Funcao para descarregar uma imagem de uma matriz em um arquivo bitmap.
 */

	typedef unsigned short WORD;
	typedef unsigned long  DWORD;

	typedef struct tagBitmapFileHeader	// Cabecalho do Arquivo ...
	{
		WORD	bfType;
		DWORD   bfSize;
		WORD 	bfReserved1;
		WORD 	bfReserved2;
		DWORD   bfOffBits;
	} BitmapFileHeader;

	typedef struct tagBitmapInfoHeader	// Cabecalho da Imagem ...
	{
		DWORD   biSize;
		DWORD   biWidth;
		DWORD   biHeight;
		WORD 	biPlanes;
		WORD 	biBitCount;
		DWORD   biCompression;
		DWORD   biSizeImage;
		DWORD   biXPelsPerMeter;
		DWORD   biYPelsPerMeter;
		DWORD   biClrUsed;
		DWORD   biClrImportant;
	} BitmapInfoHeader;

	BitmapFileHeader FH;															// Variaveis de estrutura dos dados do arquivo ...
	BitmapInfoHeader IH;

	FILE	*fBMP;																	// Ponteiro para o arquivo de imagem ...
	int	    *ImagemBitmap;															// Ponteiro para a imagem alocada na memoria ...
	
	int		contL = 0,
			contC = 0,
			contG = 0;

	int		intSwap;

	int		tmpWidth = 0;

	// Calcula largura real da imagem ...
	if ( p_width % 4 > 0 )
		for(tmpWidth=p_width;tmpWidth % 4;tmpWidth++);
	else
		tmpWidth = p_width;

    // Abre o arquivo ...
	if ( ( fBMP = fopen(p_file, "wb")) == NULL ) {
		printf("Erro abrindo arquivo %s.", p_file);
		exit(1);
	}
	
    // Salva dados do cabecalho do arquivo ...
	FH.bfType          = 0x4d42;		// BM
	FH.bfSize          = tmpWidth * p_height + 54;
	FH.bfReserved1     = 0;				// 0x00
	FH.bfReserved2     = 0;				// 0x00
	FH.bfOffBits       = 54;			// 0x36

	IH.biSize          = 40;			// 0x28
	IH.biWidth         = p_width / 3;
	IH.biHeight        = p_height;
	IH.biPlanes        = 1;				// 0x01
	IH.biBitCount      = 24;			// 0x18
	IH.biCompression   = 0;				// 0x00
	IH.biSizeImage     = tmpWidth * p_height;
	IH.biXPelsPerMeter = 2835;														// ToDo: Obter via parametro ...
	IH.biYPelsPerMeter = 2835;														// ToDo: Obter via parametro ...
	IH.biClrUsed       = 0;				// 0x00
	IH.biClrImportant  = 0;				// 0x00
	
	fwrite(&FH.bfType,2,1,fBMP);
	fwrite(&FH.bfSize,4,1,fBMP);
	fwrite(&FH.bfReserved1,2,1,fBMP);
	fwrite(&FH.bfReserved2,2,1,fBMP);
	fwrite(&FH.bfOffBits,4,1,fBMP);

	fwrite(&IH.biSize,4,1,fBMP);
	fwrite(&IH.biWidth,4,1,fBMP);
	fwrite(&IH.biHeight,4,1,fBMP);
	fwrite(&IH.biPlanes,2,1,fBMP);
	fwrite(&IH.biBitCount,2,1,fBMP);
	fwrite(&IH.biCompression,4,1,fBMP);
	fwrite(&IH.biSizeImage,4,1,fBMP);
	fwrite(&IH.biXPelsPerMeter,4,1,fBMP);
	fwrite(&IH.biYPelsPerMeter,4,1,fBMP);
	fwrite(&IH.biClrUsed,2,1,fBMP);
	fwrite(&IH.biClrImportant,2,1,fBMP);

	fwrite(&FH.bfReserved1,2,1,fBMP);												// ToDo: verificar ...
	fwrite(&FH.bfReserved1,2,1,fBMP);												// ToDo: verificar ...

	for(contL=p_height-1;contL>=0;contL--)
		for(contC=0;contC<tmpWidth;contC+=3) {
			intSwap=(int)p_matrix[contL][contC + 2];
			fwrite(&intSwap, 1, 1,fBMP);

			intSwap=(int)p_matrix[contL][contC + 1];
			fwrite(&intSwap, 1, 1,fBMP);

			intSwap=(int)p_matrix[contL][contC];
			fwrite(&intSwap, 1, 1,fBMP);
		}
	
	fclose(fBMP);
}

