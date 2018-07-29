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

struct listaCategoria
{
	categoria unaCategoria;
	struct listaCategoria *ptrSiguiente;
};

typedef struct listaCategoria ListaCategoria;
typedef ListaCategoria *ptrListaCategoria;

void empujarListaCategoria(ptrListaCategoria, categoria);
void imprimirListaCategorias(ptrListaCategoria);
void buscarCategorias(ptrListaCategoria);

int mostrarMenuPrincipal();
void buscarCategorias(categoria[], int);
int mostrarCategorias(categoria[], int);
categoria buscarCategoria(categoria[], int, int);
char mostrarInstrucciones();
categoria buscarDatos(categoria, int);
void mostrarDatos(categoria, int, int);
void convertirCadenaAMinusculas(char[], char[]);
void juego_perdido();
int esNumero(char[]);

int main()
{
	ptrListaCategoria ptrPila;

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
				buscarCategorias(ptrPila);
				imprimirListaCategorias(ptrPila);

				/*buscarCategorias(categorias, CATEGORIA_TAMANIO);
				categoriaId = mostrarCategorias(categorias, CATEGORIA_TAMANIO);
				unaCategoria = buscarCategoria(categorias, CATEGORIA_TAMANIO, categoriaId);
				unaCategoria = buscarDatos(unaCategoria, PREGUNTA_TAMANIO);
				mostrarDatos(unaCategoria, PREGUNTA_TAMANIO, RESPUESTA_TAMANIO);*/
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

void empujarListaCategoria(ptrListaCategoria *ptrCima, categoria dato)
{
	ptrListaCategoria ptrNuevo;

	ptrNuevo = malloc(sizeof(ListaCategoria));

	if (ptrNuevo != NULL)
	{
		ptrNuevo->unaCategoria = dato;
		ptrNuevo->ptrSiguiente = *ptrCima;
		*ptrCima = ptrNuevo;
	}
	else
	{
		printf("La categoria: %s no se inserto. Memoria insuficiente.", dato.nombre);
	}
}

void imprimirListaCategorias(ptrListaCategoria ptrActual)
{
	if (ptrActual == NULL)
	{
		printf("No hay categorias.\n\n");
	}
	else
	{
		printf("Categorias\n");
		printf("==========\n\n");
		while (ptrActual != NULL)
		{
			printf("[%d]\t%s\t", ptrActual->unaCategoria.id, ptrActual->unaCategoria.nombre);
		}
	}
}

void buscarCategorias(ptrListaCategoria ptrPila)
{
	int i = 0;
	categoria unaCategoria;

	FILE *doc;

	if ((doc = fopen("categorias.txt", "r")) == NULL)
	{
		printf("El archivo de las categorias no pudo abrirse.\n");
	}
	else
	{
		do
		{
			fscanf(doc, "%d%s", &unaCategoria.id, unaCategoria.nombre);
			empujarListaCategoria(ptrPila, unaCategoria);
			i++;
		} while (!feof(doc));

		fclose(doc);
	}
}

int mostrarMenuPrincipal()
{
	int opc;
	char respuesta[100];

	system("color 0A");

	printf("\n \n \t TRIVISIMO \n");
	printf("\t ******** \n");

	printf("\n MENU PRINCIPAL\n");
	printf("\n [1]........Instrucciones");
	printf("\n [2]........Jugar");
	printf("\n [3]........Salir\n ");

	do
	{
		printf("Elija una opcion [1-3]: ");
		scanf("%s", respuesta);

		if (esNumero(respuesta))
		{
			opc = atoi(respuesta);
			if (opc < 1 || opc > 3)
			{
				printf("ERROR ");
			}
		}
		else
		{
			printf("ERROR SOLO PUEDE INGRESAR NUMEROS ");
		}
	} while (opc < 1 || opc > 3);

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
	char respuesta[100];
	int longitud = 0;
	
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

	do
	{
		printf("Desea Iniciar el juego? [S/N]: ");
		scanf("%s", &respuesta);

		longitud = strlen(respuesta) - 1;

		if (longitud == 1)
		{
			tolower(respuesta[0]);

			if (respuesta[0] != 's' && respuesta[0] != 'n')
			{
				printf("ERROR ");
			}
		}
	} while (respuesta[0] != 's' && respuesta[0] != 'n');
	
	system("cls");

	return respuesta[0];
}

int esNumero(char entrada[])
{
	int longitud, i = 0, esValido = 1;
	longitud = strlen(entrada);

	for (i = 0; i < longitud; i++)
	{
		if (!isdigit(entrada[i]))
		{
			esValido = 0;
			return esValido;
		}
	}

	return esValido;
}

void juego_perdido()
{
	system("cls");
	system("color 0E");
	printf("\n  \t#######  #######  ####      ####  #######");
	printf("\n  \t##       ##   ##  ##  #    #  ##  ##");
	printf("\n  \t##  ###  #######  ##   ###    ##  #####");
	printf("\n  \t##   ##  ##   ##  ##          ##  ##");
	printf("\n  \t#######  ##   ##  ##          ##  #######\n");

	printf("\n  \t#######   ##          ##  #######  #######");
	printf("\n  \t##   ##    ##        ##   ##       ##   ##");
	printf("\n  \t##   ##     ##      ##    #####    ######");
	printf("\n  \t##   ##      ##    ##     ##       ##    ##");
	printf("\n  \t#######        #####      #######  ##     ##\n");

}