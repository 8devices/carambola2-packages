/*
 * Javier Abell�n. 14 de Abril de 2003
 *
 * Ejemplo de como un servidor puede manejar varios clientes con select().
 * Este programa hace de cliente de dicho servidor.
 */
#include "Socket.h"
#include "Socket_Cliente.h"

/* Programa principal. Abre la conexi�n, recibe su n�mero de cliente y
 * luego env�a dicho n�mero cada segundo */
main()
{
	int sock;		/* descriptor de conexi�n con el servidor */
	struct s_msg *buffer;		/* buffer de lectura de datos procedentes del servidor */
	int error;		/* error de lectura por el socket */
	buffer = malloc(sizeof(struct s_msg));
	/* Se abre una conexi�n con el servidor */
	sock = Abre_Conexion_Inet ("localhost", "cpp_java");

	/* Se lee el n�mero de cliente, dato que nos da el servidor. Se escribe
	 * dicho n�mero en pantalla.*/
	error = ReadTcpSocket (sock, &buffer);
//	struct s_msg Datos;
//	*Datos = malloc(sizeof struct s_msg);
//	error = ReadTcpSocket (sock, &Datos);
//	printf("%d %d %s\n",Datos.type, Datos.command, Datos.data);
	/* Si ha habido error de lectura lo indicamos y salimos */
	if (error < 1)
	{
		printf ("Me han cerrado la conexi�n\n");
		exit(-1);
	}
printf("Se leyeron %d bytes\n", error);
	/* Se escribe el n�mero de cliente que nos ha enviado el servidor */
	printf ("type=%d\ncommand=%d\nlen=%d\ndata=%s\n", buffer->type, buffer->command, buffer->len, buffer->data);
	printf ("Soy cliente\n%s\n", buffer);

	/* Bucle infinito. Envia al servidor el n�mero de cliente y espera un
	 * segundo */
//	while (1)
//	{
//		Escribe_Socket (sock, (char *)&buffer, sizeof(int));
//		sleep (1);
//	}
}

