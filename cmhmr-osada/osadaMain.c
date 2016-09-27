/*
 * osadaMain.c
 *
 *  Created on: 19/9/2016
 *      Author: utnso
 */
#include "osada.h"
#include "mockOsada.h"


int main(int argc, char *argv[]){
	int archivoID = obtenerIDDelArchivo(argv[1]);
	int tamanio = obtenerTamanioDelArchivo(archivoID);

	unsigned char *osada = inicializarOSADA(archivoID, tamanio);
	osada_header *osadaHeaderFile = obtenerHeader(osada);
	setearConstantesDePosicionDeOsada(osadaHeaderFile);


	t_bitarray *bitMap = obtenerBitmap(osada, osadaHeaderFile);
	osada_file *tablaDeArchivo = obtenerTablaDeArchivos(osada, osadaHeaderFile);
	int *tablaDeAsignacion = obtenerTablaDeAsignacion(osada, osadaHeaderFile);
	char *bloqueDeDatos = obtenerBloqueDeDatos(osada, osadaHeaderFile);

	//mock_Crear_Directorios(osada, osadaHeaderFile, tamanio);
	//mock_Crear_La_Tabla_De_Asignacion_De_Los_Directorios(osada, tablaDeAsignacion, tamanio, osadaHeaderFile);
	//mock_Modificar_Los_Bloques_Del_Bitmap(osada,bitMap , tamanio, osadaHeaderFile);


	printf("************TERMINO TODO************\n");

	//free(bitMap);
	free(osadaHeaderFile);
	//free(tablaDeArchivo);

	//free(tablaDeAsignacion);//Error in `./cmhmr-osada': double free or corruption (out): 0x097611b0
	//free(bloqueDeDatos);//Error in `./cmhmr-osada': double free or corruption (out): 0x097611b0

	return 0;
}


