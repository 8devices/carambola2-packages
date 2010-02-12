/*
 * Javier Abell�n. 14 de Abril de 2003
 *
 * Ejemplo de como un servidor puede manejar varios clientes con select().
 * Este programa hace de servidor.
 */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <Socket_Servidor.h>
#include <Socket.h>

#define MAX_CLIENTES 10

/* Prototipos de las funciones definidas en este fichero */
void nuevoCliente (int servidor, int *clientes, int *nClientes);
int dameMaximo (int *tabla, int n);
void compactaClaves (int *tabla, int *n);

/*
 * Programa principal.
 * Crea un socket servidor y se mete en un select() a la espera de clientes.
 * Cuando un cliente se conecta, le atiende y lo a�ade al select() y vuelta
 * a empezar.
 */
main()
{
	int socketServidor;				/* Descriptor del socket servidor */
	int socketCliente[MAX_CLIENTES];/* Descriptores de sockets con clientes */
	int numeroClientes = 0;			/* N�mero clientes conectados */
	fd_set descriptoresLectura;	/* Descriptores de interes para select() */
	int buffer;							/* Buffer para leer de los socket */
	int maximo;							/* N�mero de descriptor m�s grande */
	int i;								/* Para bubles */

	/* Se abre el socket servidor, avisando por pantalla y saliendo si hay 
	 * alg�n problema */
	socketServidor = Abre_Socket_Inet ("cpp_java");
	if (socketServidor == -1)
	{
		perror ("Error al abrir servidor");
		exit (-1);
	}

	/* Bucle infinito.
	 * Se atiende a si hay m�s clientes para conectar y a los mensajes enviados
	 * por los clientes ya conectados */
	while (1)
	{
		/* Cuando un cliente cierre la conexi�n, se pondr� un -1 en su descriptor
		 * de socket dentro del array socketCliente. La funci�n compactaClaves()
		 * eliminar� dichos -1 de la tabla, haci�ndola m�s peque�a.
		 * 
		 * Se eliminan todos los clientes que hayan cerrado la conexi�n */
		compactaClaves (socketCliente, &numeroClientes);
		
		/* Se inicializa descriptoresLectura */
		FD_ZERO (&descriptoresLectura);

		/* Se a�ade para select() el socket servidor */
		FD_SET (socketServidor, &descriptoresLectura);

		/* Se a�aden para select() los sockets con los clientes ya conectados */
		for (i=0; i<numeroClientes; i++)
			FD_SET (socketCliente[i], &descriptoresLectura);

		/* Se el valor del descriptor m�s grande. Si no hay ning�n cliente,
		 * devolver� 0 */
		maximo = dameMaximo (socketCliente, numeroClientes);
		
		if (maximo < socketServidor)
			maximo = socketServidor;

		/* Espera indefinida hasta que alguno de los descriptores tenga algo
		 * que decir: un nuevo cliente o un cliente ya conectado que env�a un
		 * mensaje */
		select (maximo + 1, &descriptoresLectura, NULL, NULL, NULL);

		/* Se comprueba si alg�n cliente ya conectado ha enviado algo */
		for (i=0; i<numeroClientes; i++)
		{
			if (FD_ISSET (socketCliente[i], &descriptoresLectura))
			{
				/* Se lee lo enviado por el cliente y se escribe en pantalla */
				if ((Lee_Socket1 (socketCliente[i], (char *)&buffer, 100) > 0))
					printf ("Cliente %d env�a %s\n", i+1, buffer);
				else
				{
					/* Se indica que el cliente ha cerrado la conexi�n y se
					 * marca con -1 el descriptor para que compactaClaves() lo
					 * elimine */
					printf ("Cliente %d ha cerrado la conexi�n\n", i+1);
					socketCliente[i] = -1;
				}
			}
		}

		/* Se comprueba si alg�n cliente nuevo desea conectarse y se le
		 * admite */
		if (FD_ISSET (socketServidor, &descriptoresLectura))
			nuevoCliente (socketServidor, socketCliente, &numeroClientes);
	}
}

/*
 * Crea un nuevo socket cliente.
 * Se le pasa el socket servidor y el array de clientes, con el n�mero de
 * clientes ya conectados.
 */
void nuevoCliente (int servidor, int *clientes, int *nClientes)
{
	/* Acepta la conexi�n con el cliente, guard�ndola en el array */
	clientes[*nClientes] = Acepta_Conexion_Cliente (servidor);
	(*nClientes)++;

	/* Si se ha superado el maximo de clientes, se cierra la conexi�n,
	 * se deja todo como estaba y se vuelve. */
	if ((*nClientes) >= MAX_CLIENTES)
	{
		close (clientes[(*nClientes) -1]);
		(*nClientes)--;
		return;
	}
		
	/* Env�a su n�mero de cliente al cliente */

//	Escribe_Socket1 (clientes[(*nClientes)-1], (char *)nClientes, sizeof(int));
	char pepe[] = "Mensaje del server uno";
/*
	char mensa[] = "Mensaje del server uno";
	struct s_msg *pepe;
	pepe = calloc(pepe,12+strlen(mensa)+2);
	pepe->type = 1;
	pepe->command = 0;
	pepe->len = strlen(mensa);
	strcpy(pepe->data,mensa);
	printf("int=%d s_msg=%d pepe=%d\n",sizeof(int),sizeof(struct s_msg), sizeof((struct s_msg *)pepe));
	printf("type=%d command=%d len=%d data=%s\n", pepe->type, pepe->command, pepe->len, pepe->data);
*///
//	pepe.data = strcpy(pepe.data,"Mensaje del server uno");
	Escribe_Socket (clientes[(*nClientes)-1], pepe, strlen(pepe));
	int escribio;
//	escribio = WriteTcpSocket (clientes[(*nClientes)-1], 0,0, "Connected Ok" );
	/* Escribe en pantalla que ha aceptado al cliente y vuelve */
//	printf ("Aceptado cliente %d %d\n", *nClientes, escribio);
	return;
}

/*
 * Funci�n que devuelve el valor m�ximo en la tabla.
 * Supone que los valores v�lidos de la tabla son positivos y mayores que 0.
 * Devuelve 0 si n es 0 o la tabla es NULL */
int dameMaximo (int *tabla, int n)
{
	int i;
	int max;

	if ((tabla == NULL) || (n<1))
		return 0;
		
	max = tabla[0];
	for (i=0; i<n; i++)
		if (tabla[i] > max)
			max = tabla[i];

	return max;
}

/*
 * Busca en array todas las posiciones con -1 y las elimina, copiando encima
 * las posiciones siguientes.
 * Ejemplo, si la entrada es (3, -1, 2, -1, 4) con *n=5
 * a la salida tendremos (3, 2, 4) con *n=3
 */
void compactaClaves (int *tabla, int *n)
{
	int i,j;

	if ((tabla == NULL) || ((*n) == 0))
		return;

	j=0;
	for (i=0; i<(*n); i++)
	{
		if (tabla[i] != -1)
		{
			tabla[j] = tabla[i];
			j++;
		}
	}
	
	*n = j;
}

