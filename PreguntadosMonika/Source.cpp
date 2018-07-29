#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define CATEGORIA_TAMANIO 4
#define PREGUNTA_TAMANIO 3
#define PREGUNTA_TAMANIO_JUEGO 3
#define RESPUESTA_TAMANIO 3

struct respuesta
{
	int id = 0;
	char detalle[256] = "";
	int estado = 0;
};

struct pregunta
{
	int id = 0;
	char detalle[256] = "";
	respuesta respuestas[RESPUESTA_TAMANIO];
};

struct categoria
{
	int id = 0;
	char nombre[40] = "";
	pregunta preguntas[PREGUNTA_TAMANIO];
};

int mostrarMenuPrincipal();
void buscarCategorias(categoria[], int);
int mostrarCategorias(categoria[], int);
categoria buscarCategoria(categoria[], int, int);
char mostrarInstrucciones();
categoria buscarDatos(categoria, int);
void mostrarDatos(categoria, int, int);
void convertirCadenaAMinusculas(char[], char[]);

int main()
{
	int opc, categoriaId;
	categoria categorias[CATEGORIA_TAMANIO];
	categoria unaCategoria;
	char iniciar;

	srand(time(NULL));
	
	opc = mostrarMenuPrincipal();
	switch (opc)
	{
		case 1:
			iniciar = mostrarInstrucciones();
			if (iniciar == 's')
			{
				buscarCategorias(categorias, CATEGORIA_TAMANIO);
				categoriaId = mostrarCategorias(categorias, CATEGORIA_TAMANIO);
				unaCategoria = buscarCategoria(categorias, CATEGORIA_TAMANIO, categoriaId);
				unaCategoria = buscarDatos(unaCategoria, PREGUNTA_TAMANIO);
				mostrarDatos(unaCategoria, PREGUNTA_TAMANIO, RESPUESTA_TAMANIO);
			}
			break;

		case 2:
			buscarCategorias(categorias, CATEGORIA_TAMANIO);
			categoriaId = mostrarCategorias(categorias, CATEGORIA_TAMANIO);
			unaCategoria = buscarCategoria(categorias, CATEGORIA_TAMANIO, categoriaId);
			unaCategoria = buscarDatos(unaCategoria, PREGUNTA_TAMANIO);
			mostrarDatos(unaCategoria, PREGUNTA_TAMANIO, RESPUESTA_TAMANIO);
			break;

		case 3:
			printf("\n \t HASTA PRONTO \n \n \n ");
			break;

		default:
			break;
	}

	system("pause");

	return 0;
}

int mostrarMenuPrincipal()
{
	int opc;

	system("color 0A");

	printf("\n \n \t TRIVISIMO \n");
	printf("\t ******** \n");

	printf("\n MENU PRINCIPAL\n");
	printf("\n [1]........Instrucciones");
	printf("\n [2]........Jugar");
	printf("\n [3]........Salir\n ");

	printf("Elija una opcion: ");
	scanf("%d", &opc);

	system("cls");

	return opc;
}

void buscarCategorias(categoria categorias[], int tamanio)
{
	int i = 0;
	FILE *doc;

	if ((doc = fopen("categorias.txt", "r")) == NULL)
	{
		printf("El archivo de las categorias no pudo abrirse.\n");
	}
	else
	{
		do
		{
			fscanf(doc, "%d%s", &categorias[i].id, categorias[i].nombre);
			i++;
		} while (!feof(doc));

		fclose(doc);
	}		
}

int mostrarCategorias(categoria categorias[], int tamanio)
{
	int i = 0, categoriaElegida, existeCategoria = 0;
	printf("Categorias\n");
	printf("==========\n\n");

	for (i = 0; i < tamanio; i++)
	{
		printf("[%d]\t%s\n", categorias[i].id, categorias[i].nombre);
	}

	do
	{
		printf("Seleccione una categoria: ");
		scanf("%d", &categoriaElegida);

		for (i = 0; i < tamanio; i++)
		{
			if (categorias[i].id == categoriaElegida)
			{
				existeCategoria = 1;
				break;
			}
		}

		if (existeCategoria == 0)
		{
			printf("Error, la categoria no existe.\n");
		}
	} while (existeCategoria == 0);

	system("cls");

	return categoriaElegida;
}

categoria buscarCategoria(categoria categorias[], int tamanio, int id)
{
	int i = 0;
	categoria categoriaEncotrada;

	for (i = 0; i < tamanio; i++)
	{
		if (categorias[i].id == id)
		{
			categoriaEncotrada.id = id;
			strcpy(categoriaEncotrada.nombre, categorias[i].nombre);
			break;
		}
	}

	return categoriaEncotrada;
}

categoria buscarDatos(categoria unaCategoria, int tamanioPreguntas)
{
	char nombreArchivoPregunta[100], nombreArchivoPreguntaMinuscula[100];
	char preguntaId_chr[10];
	int i = 0, lineaPregunta = 0, j = 0, lineaRespuesta = 0;
	categoria categoriaArchivo;

	categoriaArchivo = unaCategoria;
	
	FILE *doc;
	
	// Carga las preguntas de la categoria elegida por el usuario.
	strcpy(nombreArchivoPreguntaMinuscula, "\0");
	strcpy(nombreArchivoPregunta, categoriaArchivo.nombre);
	strcat(nombreArchivoPregunta, ".txt");
	//convertirCadenaAMinusculas(nombreArchivoPreguntaMinuscula, nombreArchivoPregunta);

	if ((doc = fopen(nombreArchivoPregunta, "r")) == NULL)
	{
		printf("El archivo de la pregunta no pudo abrirse.\n");
	}
	else
	{
		do
		{
			fscanf(doc, "%d%s", &categoriaArchivo.preguntas[lineaPregunta].id, categoriaArchivo.preguntas[lineaPregunta].detalle);
			lineaPregunta++;
		} while (!feof(doc));

		fclose(doc);
	}

	// Carga las respuestas de las preguntas de la categoria elegida por el usuario.
	for (i = 0; i < tamanioPreguntas; i++)
	{
		strcpy(nombreArchivoPreguntaMinuscula, "\0");
		sprintf(preguntaId_chr, "%d", categoriaArchivo.preguntas[i].id);
		strcpy(nombreArchivoPregunta, categoriaArchivo.nombre);
		strcat(nombreArchivoPregunta, "_");
		strcat(nombreArchivoPregunta, preguntaId_chr);
		strcat(nombreArchivoPregunta, ".txt");
		//convertirCadenaAMinusculas(nombreArchivoPreguntaMinuscula, nombreArchivoPregunta);

		if ((doc = fopen(nombreArchivoPregunta, "r")) == NULL)
		{
			printf("El archivo de la respuesta no pudo abrirse.\n");
		}
		else
		{
			lineaRespuesta = 0;
			do
			{
				fscanf(doc, "%d%s%d", &categoriaArchivo.preguntas[i].respuestas[lineaRespuesta].id, categoriaArchivo.preguntas[i].respuestas[lineaRespuesta].detalle, &categoriaArchivo.preguntas[i].respuestas[lineaRespuesta].estado);
				lineaRespuesta++;
			} while (!feof(doc));

			fclose(doc);
		}				
	}

	return categoriaArchivo;
}

void mostrarDatos(categoria unaCategoria, int tamanioPregunta, int tamanioRespuesta)
{
	int i = 0, j = 0;
	
	for (i = 0; i < tamanioPregunta; i++)
	{
		printf("Categoria: %s\n\n", unaCategoria.nombre);		
		printf("Pregunta [%d]: ... %s:\n", i + 1, unaCategoria.preguntas[i].detalle);
		for (j = 0; j < tamanioRespuesta; j++)
		{
			printf("[%d]...%s\n", j + 1, unaCategoria.preguntas[i].respuestas[j].detalle);
		}

		system("pause");
		system("cls");
	}
}

void convertirCadenaAMinusculas(char destino[], char origen[])
{
	int i = 0;
	int longitud = strlen(origen);
	for (i = 0; i < longitud; i++)
	{
		destino[i] = tolower(origen[i]);
	}
}

char mostrarInstrucciones()
{
	char respuesta;
	
	printf("\n \t INSTRUCCIONES\n");
	printf("\n\t    ___     ");
	printf("\n\t __|   |__");
	printf("\n\t|__  O  __|   ");
	printf("\n\t   |___|     ");
	printf("\n\t    ||  __   ");
	printf("\n\t    || / / ");
	printf("\n\t    ||/ /     ");
	printf("\n\t    ||_/     ");
	printf("\n\t    ||       ");
	printf("\n\t============ ");
	printf("\n\t ========== ");
	printf("\n\t  ======== ");
	printf("\n\t   ====== ");
	printf("\n Bienvenido(a) querido jugador hay un par de cosas de debes saber antes de iniciar el juego.");
	printf("\n El objetivo de este juego es hacer que tu plantita crezca!, \n para hacer eso debes elegir una categoria, \n por cada categoria que respondas bien iras acumulando puntos");
	printf("\nCada categoria cuenta con 3 preguntas, si respondes correctamente las preguntas\n pasas a otra categoria a tu eleccion. \n entre mas puntos obtengas mas rapido crecera tu planta");
	printf("\n divierte y vence a tus amigos!!");

	printf("\n \n Desea Iniciar el juego? [S/N]: ");
	scanf(" %c", &respuesta);
	
	system("cls");

	return tolower(respuesta);
}