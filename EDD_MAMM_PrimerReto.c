//Sopa de Letras -> Marvin Antonio Martinez
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_PALABRAS 50
#define MAX_LONGITUD 20


//estructuras
struct SopaLetras
{
	int N;
	char **matriz;
};

struct Nodo
{
	char palabra[MAX_PALABRAS];
	bool encontrada;
	struct Nodo *siguiente;
};


//prototipos de funcion
void inicializarTablero(struct SopaLetras *sopa);
void imprimirTablero(struct SopaLetras *sopa);
void insertarPalabraFacil(struct SopaLetras *sopa, char *palabra);
void convertirMayus(char *palabra);
bool espacioDispo(struct SopaLetras *sopa, char *palabra, int x, int y, int direccion);
void rellenarEspacios(struct SopaLetras *sopa);
void agregarPalabra(struct Nodo **lista, char * palabra);
bool buscarPalabra(struct Nodo *lista, char *palabra);
void imprimirLista(struct Nodo *lista);

int main(int argc, char const *argv[])
{
	srand(time(NULL));

	int N, numPalabras, nivel, opcion;
	char palabras[MAX_PALABRAS][MAX_LONGITUD];

	printf("Introduce el tamaño del tablero (N x N): ");
	scanf("%d", &N);

	if (N<5 || N> 20)
	{
		printf("Tamaño inválido, debe estar entre 5 y 20.\n");
		return 1;
	}

	printf("Selecciona el nivel de dificultad\n1->Facil\n2->Medio\n3->Dificil😈\n");
	scanf("%d", &nivel);
	if(nivel != 1 & nivel != 2 & nivel !=3)
	{
		return 1;
	}

	struct SopaLetras sopa;
	sopa.N = N;

	//se asigna memoria dinam
	sopa.matriz= (char **)malloc(N * sizeof(char *));
	for (int i = 0; i < N; ++i)
	{
		sopa.matriz[i] = (char *)malloc(N * sizeof(char));
	}

	inicializarTablero(&sopa);

	printf("Introduce el número de palabras (entre 10 y 50): ");
	scanf("%d", &numPalabras);

	if (numPalabras < 10 || numPalabras > MAX_PALABRAS)
	{
		printf("Número de palabras invalido.\n");
		return 1;
	}


	struct Nodo *lista_plbras= NULL;

	for (int i = 0; i < numPalabras; ++i)
	{
		printf("Introduce la palabra %d: ", i+1);
		scanf("%s", palabras[i]);
		convertirMayus(palabras[i]);

		agregarPalabra(&lista_plbras, palabras[i]);
		//insertarPalabraFacil(&sopa, palabras[i]);
		switch(nivel)
		{
		case 1: 
			insertarPalabraFacil(&sopa, palabras[i]);
			break;
		case 2:
			break;
		case 3:
			break;

		}
	}

	rellenarEspacios(&sopa);

	printf("\nElige una opción:\n");
	printf("1. Jugar en Terminal\n");
	printf("2. Exportar a un archivo txt\n");
	printf("Opción: ");
	scanf("%d", &opcion);

	if (opcion==1)
	{
		system("clear");
		imprimirTablero(&sopa);

		char palabra_buscada[MAX_LONGITUD];
		int palabra_encontrada=0;
		struct Nodo *lista_palabras=lista_plbras;

		do
		{
			struct Nodo*temp=lista_palabras;
			int palabras_restantes=0;
			while(temp!=NULL)
			{
				if(!temp->encontrada)
				{
					palabras_restantes++;
				}
				temp=temp->siguiente;
			}
			printf("Palabras restantes: %d\n", palabras_restantes);

			if (palabras_restantes==0)
			{
				break;
			}
			
			printf("Introduce la palabra a buscar: ");
			scanf("%s", palabra_buscada);
			convertirMayus(palabra_buscada);

			if (buscarPalabra(lista_palabras, palabra_buscada))
			{
				printf("Palabra encontrada. 🤓\n");
			}else
			{
				printf("Palabra no encontrada o ya encontrada🫨\n");
			}
		}while(palabra_encontrada<numPalabras);	

		printf("Felicidades, no pensé que lo lograras xd🫵🏼😛\n");	
	}else if(opcion==2)
	{

	}else
	{
		printf("Opción invalida.😆\n");
	}

	//printf("\nTablero de Sopa de Letras: \n");

	//imprimirTablero(&sopa);

	//liberar memoria
	struct Nodo *temp;
	
	while(lista_plbras!=NULL)
	{
		temp=lista_plbras;
		lista_plbras=lista_plbras->siguiente;
		free(temp);
	}

	for (int i = 0; i < N; ++i)
	{
		free(sopa.matriz[i]);
	}
	free(sopa.matriz);

	return 0;
}

void inicializarTablero(struct SopaLetras *sopa)
{
	for (int i = 0; i < sopa->N ; ++i)
	{
		for (int j = 0; j < sopa->N; ++j)
		{
			sopa->matriz[i][j] = ' '; // + rand() % 26; 
		}
	}
}

void imprimirTablero(struct SopaLetras *sopa)
{
	for (int i = 0; i < sopa->N; ++i)
	{
		for (int j = 0; j < sopa->N; ++j)
		{
			printf("%c  ", sopa->matriz[i][j]);
		}
		printf("\n\n");
	}
}

void insertarPalabraFacil(struct SopaLetras *sopa, char *palabra)
{
	int len=0;
	while(palabra[len] != '\0') len++;

	if(len > sopa->N)
	{
		return;
	}

	int intentos =100;

	while(intentos --)
	{
		int direccion = rand() % 2; // 0= hori ; 1= verti
		int x = rand() % (direccion ==0 ? sopa->N: sopa->N -len +1);
		int y = rand() % (direccion ==1 ? sopa->N: sopa->N -len +1);

		if(espacioDispo(sopa, palabra, x,y,direccion))
		{
			for(int i=0; i<len; i++)
			{
				int nx=direccion ==0 ? x : x+i;
				int ny=direccion==1? y:y+i;
				sopa->matriz[nx][ny]= palabra[i];
			}
			return;
		}
	}
	
	printf("No se pudo insertar la palabra %s\n", palabra);
}

void convertirMayus(char *palabra)
{
	for (int i = 0; i < palabra[i] != '\0'; ++i)
	{
		palabra[i]= toupper(palabra[i]);
	}
}

bool espacioDispo(struct SopaLetras *sopa, char *palabra, int x, int y, int direccion)
{
	int len =0;
	while(palabra[len] != '\0') len++;

	for (int i = 0; i < len; ++i)
	{
		int nx = direccion ==0 ? x : x +i;
		int ny = direccion ==1 ? y : y +i;

		if(sopa->matriz[nx][ny] != ' ' && sopa->matriz[nx][ny] != palabra[i])
		{
			return false; // hay colsion
		}
	}
	return true;
}

void rellenarEspacios(struct SopaLetras *sopa)
{
	for (int i = 0; i < sopa->N; ++i)
	{
		for(int j=0; j< sopa->N; ++j)
		{
			if (sopa->matriz[i][j]==' ')
			{
				sopa->matriz[i][j] = 'A' + rand() % 26; 
			}
		}
	}
}

void agregarPalabra(struct Nodo **lista, char * palabra)
{
	struct Nodo *nuevoNodo = (struct Nodo *)malloc(sizeof(struct Nodo));
	strcpy(nuevoNodo->palabra, palabra);
	nuevoNodo->siguiente= *lista;
	*lista= nuevoNodo;
}

bool buscarPalabra(struct Nodo *lista, char *palabra)
{
	convertirMayus(palabra);
	while(lista!=NULL)
	{
		if (strcmp(lista->palabra, palabra)==0)
		{
			if (lista->encontrada)
			{
				return false;
			}
			else
			{
				lista->encontrada=true; //se marca como encontrada
				return true;
			}
		}
		lista = lista->siguiente;
	}
	return false;
}

void imprimirLista(struct Nodo *lista)
{
	while(lista != NULL)
	{
		printf("%s\n", lista->palabra);
		lista=lista->siguiente;
	}
}