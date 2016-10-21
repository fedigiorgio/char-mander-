/*
 * osada.c
 *
 *  Created on: 9/9/2016
 *      Author: utnso
 */

#include "osada.h"
#include <errno.h>


void guardarEnOsada2(int desde, void *elemento, int tamaniaDelElemento){
	printf("iniciio guardarEnOsada2\n");
	memcpy(&OSADA[desde], elemento, tamaniaDelElemento);
	printf("fin guardarEnOsada2\n");
}

void guardarEnOsada(unsigned char *osada, int desde, void *elemento, int tamaniaDelElemento){
	memcpy(&osada[desde], elemento, tamaniaDelElemento );
	int status = munmap(osada, tamaniaDelElemento);

	if (status == -1)
		printf("Estado del munmap: %i\n", status);
}




char *obtenerBloqueDeDatos(unsigned char *osada, osada_header *osadaHeaderFile){
	//unsigned char *bloqueDeDatos = malloc(sizeof(char) * osadaHeaderFile->data_blocks);OLD
	unsigned char *bloqueDeDatos = malloc(sizeof(char) * OSADA_BLOCK_SIZE * osadaHeaderFile->data_blocks);

	memcpy(bloqueDeDatos, &osada[DESDE_PARA_BLOQUE_DE_DATOS], TAMANIO_QUE_OCUPA_EL_BLOQUE_DE_DATOS );
	return bloqueDeDatos;
}

void mostarAsignacion(int asignado){
	printf("Array tabla asignada: %i\n",asignado);
}

void mostrarTodosLosAsignados(int *arrayTabla, int numeroBloques){
	int pos = 0;

	for (pos = 0; pos < numeroBloques; pos++){
		mostarAsignacion(arrayTabla[pos]);
	}
}

int *obtenerTablaDeAsignacion(){
	int *arrayTabla = malloc(TAMANIO_QUE_OCUPA_LA_TABLA_DE_ASIGNACION);

	memcpy(arrayTabla, &OSADA[DESDE_PARA_TABLA_ASIGNACION], TAMANIO_QUE_OCUPA_LA_TABLA_DE_ASIGNACION );

	//mostrarTodosLosAsignados(arrayTabla, numeroBloques);
	ARRAY_TABLA_ASIGNACION = arrayTabla;
	return arrayTabla;
}

void mostrarTodaLaTablaDeArchivos(osada_file *tablaDeArchivo){
	int pos=0;
	for (pos=0; pos <= 2047; pos++){
		mostrarStructDeArchivos(tablaDeArchivo[pos], pos);
	}
}

void mostrarStructDeArchivos(osada_file tablaDeArchivo, int pos){
	printf("Empieza: %i****************\n",pos);
	printf("state_%i: %c\n",pos, tablaDeArchivo.state);
	printf("parent_directory_%i: %i\n",pos, tablaDeArchivo.parent_directory);
	printf("fname_%i: %s\n",pos, &tablaDeArchivo.fname);
	printf("file_size_%i: %i\n",pos, tablaDeArchivo.file_size);
	printf("lastmod_%i: %i\n",pos, tablaDeArchivo.lastmod);
	printf("first_block_%i: %i\n",pos, tablaDeArchivo.first_block);
	printf("Termina: %i****************\n",pos);
}

osada_file *obtenerTablaDeArchivos(unsigned char *osada, osada_header *osadaHeaderFile){
	osada_file *tablaDeArchivo = malloc(TAMANIO_TABLA_DE_ARCHIVOS);

	//2048*sizeof(osada_file) = 1024 bloques * 64 bytes ptr
	memcpy(tablaDeArchivo, &osada[DESDE_PARA_TABLA_DE_ARCHIVOS ], TAMANIO_TABLA_DE_ARCHIVOS);

	//mostrarTodaLaTablaDeArchivos(tablaDeArchivo);

	return tablaDeArchivo;
}

void contarBloques(t_bitarray *bitMap){
	int bloquesOcupados  = 0;
	int bloquesLibres = 0;
	int i = 0;

	for (i=0; i < HEADER->fs_blocks; i++){//para 150k

		if(bitarray_test_bit(bitMap, i) == 0){
			bloquesLibres++;
			//printf("Bloque - %i - LIBRE\n",i);
		}

		if(bitarray_test_bit(bitMap, i) == 1){
			bloquesOcupados++;
			//printf("Bloque - %i - OCUPADO\n",i);
		}

	}
	//printf("Bloques Ocupados: %i\n",bloquesOcupados);
	//printf("Bloques Libres: %i\n",bloquesLibres);
	//printf("bytes libres: %i\n",bloquesLibres*64);
	BYTES_LIBRES = bloquesLibres*64;
	BYTES_OCUPADOS = bloquesOcupados*64;
	printf("kb libres: %d\n", (bloquesLibres*64)/1024);

}

int bytesLibres(){
	return BYTES_LIBRES;
}

int bytesOcupados(){
	return BYTES_OCUPADOS;
}

t_bitarray *obtenerBitmap(){
	t_bitarray *bitMap;
	unsigned char *unBitMapSinFormato;

	unBitMapSinFormato = malloc(TAMANIO_DEL_BITMAP );
	memcpy(unBitMapSinFormato, &OSADA[DESDE_PARA_BITMAP], TAMANIO_DEL_BITMAP );
	bitMap = bitarray_create(unBitMapSinFormato, TAMANIO_DEL_BITMAP );

	contarBloques(bitMap);

	return bitMap;

}



void mostrarHeader(osada_header *osadaHeaderFile){
	printf("magic_number 2: %s\n",  osadaHeaderFile->magic_number);
	printf("version: %i\n", osadaHeaderFile->version);
	printf("fs_blocks: %i\n", osadaHeaderFile->fs_blocks);
	printf("bitmap_blocks: %i\n", osadaHeaderFile->bitmap_blocks);
	printf("allocations_table_offset: %i\n", osadaHeaderFile->allocations_table_offset);
	printf("data_blocks: %i\n", osadaHeaderFile->data_blocks);
	printf("padding: %s\n",   osadaHeaderFile->padding);
}

osada_header *obtenerHeader(unsigned char *osada){
	osada_header *osadaHeaderFile = malloc(sizeof(osada_header));
	memcpy(osadaHeaderFile, osada, OSADA_BLOCK_SIZE);


	mostrarHeader(osadaHeaderFile);
	HEADER = osadaHeaderFile;
	return osadaHeaderFile;

}

/*SETEA Y GUARDA EN LA VARIABLE GLOBAL, ADEMAS SI SE NECESITA SE DEVUELVE EL TAMAÑO PARA USARLOS EN MOCKS*/
int setearTamanioDelArchivo(int archivoID){
	struct stat buffer;
	fstat(archivoID, &buffer);
	TAMANIO_DEL_ARCHIVO_OSADA_EN_BYTES = buffer.st_size;
	return TAMANIO_DEL_ARCHIVO_OSADA_EN_BYTES;
}

int obtenerIDDelArchivo(char *ruta){
	printf("ruta: %s\n", ruta);
	return open(ruta, O_RDWR, (mode_t)0777);
}
void setearConstantesDePosicionDeOsada(osada_header *osadaHeaderFile){
	TAMANIO_QUE_OCUPA_EL_HEADER = OSADA_BLOCK_SIZE;
	TAMANIO_DEL_BITMAP = osadaHeaderFile->bitmap_blocks * OSADA_BLOCK_SIZE;
	TAMANIO_TABLA_DE_ARCHIVOS =  2048 * sizeof(osada_file);
	TAMANIO_QUE_OCUPA_LA_TABLA_DE_ASIGNACION = (osadaHeaderFile->fs_blocks - 1 - osadaHeaderFile->bitmap_blocks - 1024) * 4;
	TAMANIO_QUE_OCUPA_LA_TABLA_DE_ASIGNACION_EN_BLOQUES = (osadaHeaderFile->fs_blocks - 1 - osadaHeaderFile->bitmap_blocks - 1024) * 4 / OSADA_BLOCK_SIZE;
	TAMANIO_QUE_OCUPA_EL_BLOQUE_DE_DATOS = OSADA_BLOCK_SIZE* osadaHeaderFile->data_blocks;
	DATA_BLOCKS= (osadaHeaderFile->fs_blocks - osadaHeaderFile->data_blocks)*64;
	//dataBlocks=  osadaHeaderFile->allocations_table_offset + tamanioQueOcupaLaTablaDeAsignacionEnBloques;

	printf("osadaHeaderFile->fs_blocks - osadaHeaderFile->data_blocks: %i\n",osadaHeaderFile->fs_blocks - osadaHeaderFile->data_blocks);
	printf("dataBlocks: %i\n",DATA_BLOCKS);

	DESDE_PARA_BITMAP = OSADA_BLOCK_SIZE;//LO QUE OCUPA EL HEADER
	DESDE_PARA_TABLA_DE_ARCHIVOS  = OSADA_BLOCK_SIZE + TAMANIO_DEL_BITMAP;
	DESDE_PARA_TABLA_ASIGNACION  = TAMANIO_QUE_OCUPA_EL_HEADER + TAMANIO_DEL_BITMAP + TAMANIO_TABLA_DE_ARCHIVOS;
	DESDE_PARA_BLOQUE_DE_DATOS = TAMANIO_QUE_OCUPA_EL_HEADER + TAMANIO_DEL_BITMAP + TAMANIO_TABLA_DE_ARCHIVOS + TAMANIO_QUE_OCUPA_LA_TABLA_DE_ASIGNACION;
	printf("desdeParaTablaAsigancion: %i\n",DESDE_PARA_TABLA_ASIGNACION );
	printf("desdeParaBloqueDeDatos: %i\n",DESDE_PARA_BLOQUE_DE_DATOS);
}


unsigned char *inicializarOSADA(int archivoID){
	unsigned char *osada;

	/************************************************************/
	printf("Que paso?: %s\n", strerror(errno));
	printf("archivoID: %i\n", archivoID);
	printf("tamanio: %i\n", TAMANIO_DEL_ARCHIVO_OSADA_EN_BYTES);
	/************************************************************/


	osada = mmap(0, TAMANIO_DEL_ARCHIVO_OSADA_EN_BYTES, PROT_READ|PROT_WRITE,MAP_SHARED, archivoID, 0);
	int statusCerrar = close(archivoID);
	OSADA = osada;
	return osada;

}
/********************************************ARCHIVOS**************************************/

void _iterarParaVerContenido(int bloque){

	char *bloqueDeDatos = malloc(OSADA_BLOCK_SIZE + 1);
	int bloque2 = bloque *64;
	int i;
	//tamanioQueOcupaElBloqueDeDatos ir de atras con los bloques.
	//printf("%i\n", dataBlocks);
	memcpy(bloqueDeDatos, &OSADA[DATA_BLOCKS+bloque2], OSADA_BLOCK_SIZE );

//	for(i=1; i<=64; i++){
	bloqueDeDatos[OSADA_BLOCK_SIZE + 1] = '\0';
		printf("%s", bloqueDeDatos);
	//}
	//printf("\nTERMINO\n");
	free(bloqueDeDatos);



}

void verContenidoDeArchivo(t_list *conjuntoDeBloques){
	list_iterate(conjuntoDeBloques, (void*) _iterarParaVerContenido);
}

void _iterarBloques(int bloque){
	printf("_iterarBloques el proximo: %i\n", bloque);
}



t_list *crearPosicionesDeBloquesParaUnArchivo(int *arrayTabla, int numeroBloques){
	int elProximo = 0;
	t_list *proximo = list_create();

	list_add(proximo, numeroBloques);
	while ((elProximo = arrayTabla[numeroBloques]) != -1){
		list_add(proximo, elProximo);
		numeroBloques = elProximo;

	}

	list_iterate(proximo, (void*) _iterarBloques);

	return proximo;
}

osada_block_pointer devolverBloque(osada_file tablaDeArchivo, char *nombre){
	char *nac;
	char *n;
	nac = string_duplicate(&tablaDeArchivo.fname);
	n = string_duplicate(nombre);
	string_trim(&nac);
	string_trim(&n);

	if (tablaDeArchivo.state == REGULAR && strcmp(nac, n) == 0){
		free(nac);
		free(n);
		printf("state_: %c\n", tablaDeArchivo.state);
		printf("parent_directory_: %i\n", tablaDeArchivo.parent_directory);
		printf("fname_: %s\n",&tablaDeArchivo.fname);
		printf("file_size_: %i\n",tablaDeArchivo.file_size);
		printf("lastmod_: %i\n", tablaDeArchivo.lastmod);
		printf("first_block_: %i\n",tablaDeArchivo.first_block);

		return tablaDeArchivo.first_block;
	}

	free(nac);
	free(n);
	return -666;
}

osada_block_pointer buscarArchivo(osada_file *tablaDeArchivo, char *nombre){
	int pos=0;
	osada_block_pointer posicion = 0;

	for (pos=0; pos <= 2047; pos++){
		if ((posicion = devolverBloque(tablaDeArchivo[pos],  nombre)) != -666){
			printf("encontro>! , %i\n", pos);
			return posicion;
		};
	}
	return posicion;
}

int noEsVacio(int tamanio){
	return tamanio !=0;
}

int elTamanioDelArchivoEntraEnElOsada(int tamanio){
	printf("BYTES_LIBRES: %i\n",BYTES_LIBRES);
 return tamanio<=BYTES_LIBRES;
}

void escribirEnLaTablaDeArchivos(){

};

t_list* obtenerLosIndicesDeLosBloquesDisponibles(int cantidadBloques){
	t_bitarray *bitMap = obtenerBitmap();
	t_list *listDeBloques = list_create();

	int bloquesOcupados  = 0;
	int bloquesLibres = 0;
	int i = 0;

	for (i=0; i < HEADER->fs_blocks; i++){//para 150k

		if(bitarray_test_bit(bitMap, i) == 0){
			list_add(listDeBloques, i);
			bloquesLibres++;
			printf("Bloque - %i - LIBRE\n",i);
			bitarray_set_bit(bitMap, i);
		}

		if (cantidadBloques == bloquesLibres)
			break;

	}

	printf("DESDE_PARA_BITMAP - %i\n",DESDE_PARA_BITMAP);
	printf("TAMANIO_DEL_BITMAP - %i\n",TAMANIO_DEL_BITMAP);
	guardarEnOsada2(DESDE_PARA_BITMAP, bitMap->bitarray, TAMANIO_DEL_BITMAP);

	return listDeBloques;
}

void escribirElBitmap(int bloques){

}

void escribirTablaDeAsignacion(int pos, int bloqueSiguiente){
	ARRAY_TABLA_ASIGNACION[pos] = bloqueSiguiente;
}

void escribirBloquesDeDatos(){

}

void _interarBloquesQueSeranAsignados(int bloque,int hola){
	printf("el proximo: %i\n", bloque);
}

void _recorrerComoSeriaElArray(char* bloquePos, int bloqueSig) {

	printf("Bloque Pos: %i\n", atoi(bloquePos));
	printf("Bloque Sig: %i\n", bloqueSig);
	escribirTablaDeAsignacion(atoi(bloquePos), bloqueSig);


	//free(archivo);

}

void _guardarEnTablaDeDatos(char* bloquePos, char* contenido){
	printf("Bloque Pos: %i\n", atoi(bloquePos));
	int bloquePosInt = 0;
	bloquePosInt = atoi(bloquePos);

	char *bloqueDeDatos = malloc(OSADA_BLOCK_SIZE);

	int bloque2 = bloquePosInt *64;
	strcpy(bloqueDeDatos, contenido);
	printf("_guardarEnTablaDeDatos - bloqueDeDatos: %s\n",bloqueDeDatos);
	memcpy(&OSADA[DATA_BLOCKS+bloque2], bloqueDeDatos, OSADA_BLOCK_SIZE );

}

void prepararBloquesDeDatos(t_list* listado, char *contenido){
	int cantidadDeElemento = list_size(listado);
	int bloquePos;
	int i;
	t_dictionary *dicBloqueDeDatos = dictionary_create();
	char *bloqueConDatos = malloc(64);
	printf("cantidadDeElemento: %i\n",cantidadDeElemento);

	for(i=0;i<cantidadDeElemento;i++){
		bloquePos = list_get(listado, i);
		printf("prepararBloquesDeDatos - bloquePos: %i\n",bloquePos);

		char bloquePosStr[10];
		sprintf(bloquePosStr, "%d", bloquePos);
		sprintf(bloqueConDatos, "%d", i);
		printf("prepararBloquesDeDatos - bloqueConDatos: %i\n",i);

		strcat(bloqueConDatos, "hola");

		printf("prepararBloquesDeDatos - bloquePos: %i\n",bloquePos);
		printf("prepararBloquesDeDatos - bloqueConDatos: %s\n",bloqueConDatos);

		dictionary_put(dicBloqueDeDatos, bloquePosStr, bloqueConDatos);

	}
	dictionary_iterator(dicBloqueDeDatos, (void*) _guardarEnTablaDeDatos);
	free(bloqueConDatos);
}



void crearUnArchivo(char *contenido, int tamanio){
	int cantidadDeBloquesParaGrabar = 0;
	t_list* listado;
	int i=0;
	int cantidadDeElemento = 0;

	t_dictionary *comoSeriaElArray = dictionary_create();
	int bloquePos;
	int bloqueSig;

	if(elTamanioDelArchivoEntraEnElOsada(tamanio) && noEsVacio(tamanio)){

		cantidadDeBloquesParaGrabar = tamanio /64;
		listado = obtenerLosIndicesDeLosBloquesDisponibles(cantidadDeBloquesParaGrabar);
		cantidadDeElemento = list_size(listado);

		for(i=0;i<cantidadDeElemento;i++){
			bloquePos = list_get(listado, i);
			bloqueSig = list_get(listado, i+1);
			//printf("bloquePos: %i\n", bloquePos);

			if(bloqueSig==0){
				bloqueSig =-1;
			}

			char bloquePosStr[10];
			sprintf(bloquePosStr, "%d", bloquePos);

			dictionary_put(comoSeriaElArray, bloquePosStr, bloqueSig);

			//printf("bloqueSig: %i\n",bloqueSig);

		}

		//QUE HACER CUANDO LOS BLOQUES NO SON ENTEROS.
		//list_iterate(listado, (int)_interarBloquesQueSeranAsignados);
		dictionary_iterator(comoSeriaElArray, (void*) _recorrerComoSeriaElArray);
		guardarEnOsada2(DESDE_PARA_TABLA_ASIGNACION, ARRAY_TABLA_ASIGNACION, TAMANIO_QUE_OCUPA_LA_TABLA_DE_ASIGNACION);

		prepararBloquesDeDatos(listado, contenido);
	}


}
/************************FIN ARCHIVO************************************************/

/**************************INICIO DIRECTORIOS**************************************/
/****************LISTAR TODO *************************************************/
void mostrarLosDirectorios(osada_file tablaDeArchivo, int pos){
	if (tablaDeArchivo.state == DIRECTORY){
		printf("Empieza: %i****************\n",pos);
		printf("state_%i: %c\n",pos, tablaDeArchivo.state);
		printf("parent_directory_%i: %i\n",pos, tablaDeArchivo.parent_directory);
		printf("fname_%i: %s\n",pos, &tablaDeArchivo.fname);
		printf("file_size_%i: %i\n",pos, tablaDeArchivo.file_size);
		printf("lastmod_%i: %i\n",pos, tablaDeArchivo.lastmod);
		printf("first_block_%i: %i\n",pos, tablaDeArchivo.first_block);
		printf("Termina: %i****************\n",pos);
	}
}

void mostrarLosRegulares(osada_file tablaDeArchivo, int pos){
	if (tablaDeArchivo.state == REGULAR){
		printf("Empieza: %i****************\n",pos);
		printf("state_%i: %c\n",pos, tablaDeArchivo.state);
		printf("parent_directory_%i: %i\n",pos, tablaDeArchivo.parent_directory);
		printf("fname_%i: %s\n",pos, &tablaDeArchivo.fname);
		printf("file_size_%i: %i\n",pos, tablaDeArchivo.file_size);
		printf("lastmod_%i: %i\n",pos, tablaDeArchivo.lastmod);
		printf("first_block_%i: %i\n",pos, tablaDeArchivo.first_block);
		printf("Termina: %i****************\n",pos);
	}
}

void mostrarLosBorrados(osada_file tablaDeArchivo, int pos){
	if (tablaDeArchivo.state == DELETED){
		printf("Empieza: %i****************\n",pos);
		printf("state_%i: %c\n",pos, tablaDeArchivo.state);
		printf("parent_directory_%i: %i\n",pos, tablaDeArchivo.parent_directory);
		printf("fname_%i: %s\n",pos, &tablaDeArchivo.fname);
		printf("file_size_%i: %i\n",pos, tablaDeArchivo.file_size);
		printf("lastmod_%i: %i\n",pos, tablaDeArchivo.lastmod);
		printf("first_block_%i: %i\n",pos, tablaDeArchivo.first_block);
		printf("Termina: %i****************\n",pos);
	}
}

void mostrarOtrosEstados(osada_file tablaDeArchivo, int pos){
	if (tablaDeArchivo.state != DELETED && tablaDeArchivo.state != REGULAR && tablaDeArchivo.state !=DIRECTORY){
		printf("Empieza: %i****************\n",pos);
		printf("state_%i: %c\n",pos, tablaDeArchivo.state);
		printf("parent_directory_%i: %i\n",pos, tablaDeArchivo.parent_directory);
		printf("fname_%i: %s\n",pos, &tablaDeArchivo.fname);
		printf("file_size_%i: %i\n",pos, tablaDeArchivo.file_size);
		printf("lastmod_%i: %i\n",pos, tablaDeArchivo.lastmod);
		printf("first_block_%i: %i\n",pos, tablaDeArchivo.first_block);
		printf("Termina: %i****************\n",pos);
	}
}



void dameTodosLosDirectorios(osada_file *tablaDeArchivo){
	int pos=0;
	for (pos=0; pos <= 2047; pos++){
		mostrarLosDirectorios(tablaDeArchivo[pos], pos);
	}
}

void dameTodosLosArchivosRegulares(osada_file *tablaDeArchivo){
	int pos=0;
	for (pos=0; pos <= 2047; pos++){
		mostrarLosRegulares(tablaDeArchivo[pos], pos);
	}
}

void dameTodosLosBorrados(osada_file *tablaDeArchivo){
	int pos=0;
	for (pos=0; pos <= 2047; pos++){
		mostrarLosBorrados(tablaDeArchivo[pos], pos);
	}
}

void dameTodosLosOtrosEstados(osada_file *tablaDeArchivo){
	int pos=0;
	for (pos=0; pos <= 2047; pos++){
		mostrarOtrosEstados(tablaDeArchivo[pos], pos);
	}
}
/****************FIN LISTAR TODO *************************************************/
//UN DICCIONARIO CON LISTAS PARA REPRESENTAR JERARQUIAS.

void _recorrerDirectoriosPadres(char* key,t_list *datos) {

	osada_file *archivo =  malloc(sizeof(osada_file));
	archivo = list_get(datos, 0);
	printf("Diccionario - Carpeta en el root: %s\n", key);
	printf("Un elemento de la lista list, que seria el primer hijo del root: %s\n", archivo->fname);
	printf("archivo->parent_directory: %i\n", archivo->parent_directory);

	//free(archivo);

}

void reconocerDirectorio(osada_file *archivo, int pos, t_dictionary *dictionary){

	t_list *list = list_create();

	if (archivo->state == DIRECTORY  && archivo->parent_directory == 65535){
		char str[10];
		sprintf(str, "%d", pos);
		list_add(list, archivo);

		//printf("pos: %s", str);

		//dictionary_put(dictionary, (char *)archivo->fname , list);
		dictionary_put(dictionary, str , list);

	}

}

t_dictionary *crearArbolDeDirectoriosDelRoot(osada_file *tablaDeArchivo){
	t_dictionary *dictionaryDirRoot = dictionary_create();
	int pos=0;

	for (pos=0; pos <= 2047; pos++){
		reconocerDirectorio(&tablaDeArchivo[pos], pos, dictionaryDirRoot);
	}

	dictionary_iterator(dictionaryDirRoot, (void*) _recorrerDirectoriosPadres);

	return dictionaryDirRoot;
}

/*****************************************************/

void reconocerDirectorioHijos(osada_file *archivo, int pos, t_dictionary *dictionaryDirRoot){


	if (archivo->state == DIRECTORY && archivo->parent_directory != 65535){
		//printf("error!");
		char str[10];
		sprintf(str, "%d", archivo->parent_directory);
		//list_add(list, archivo);

		//printf("pos: %i\n",  archivo->parent_directory);

		//dictionary_put(dictionary, (char *)archivo->fname , list);
		t_list *list = dictionary_get(dictionaryDirRoot, str);
		//printf("list: %i\n",  list);

		if (list != 0){
			list_add(list, archivo);
			dictionary_put(dictionaryDirRoot, str , list);
		}else{

			printf("Directorios sin padres: %i\n",  archivo->parent_directory);
			printf("Sin Nombre: %s\n", archivo->fname);
		}


	}

}

t_dictionary *crearArbolDeDirectoriosHijos(osada_file *tablaDeArchivo, t_dictionary *dictionaryDirRoot){
	int pos=0;

	for (pos=0; pos <= 2047; pos++){
		reconocerDirectorioHijos(&tablaDeArchivo[pos], pos, dictionaryDirRoot);
	}

	//dictionary_iterator(dictionaryDirRoot, (void*) _recorrerDirectoriosPadres);

	return dictionaryDirRoot;
}
/*********************************************************************************************************/
void reconocerArchivosParaArbol(osada_file *archivo, int pos, int padre ){



	if (archivo->parent_directory == padre){
		printf("EMPIEZA reconocerArchivosParaArbol %i: ****************\n", pos);
		printf("state_: %c\n", archivo->state);
		printf("parent_directory_: %i\n", archivo->parent_directory);
		printf("fname_: %s\n", &archivo->fname);
		printf("file_size_: %i\n", archivo->file_size);
		printf("lastmod_: %i\n", archivo->lastmod);
		printf("first_block_: %i\n", archivo->first_block);
		printf("Termina reconocerArchivosParaArbol %i ****************\n", pos);
	}

}

void reconocerDirectorioPadre(osada_file *archivo, int pos, int padre){
	if (archivo[padre].state == DIRECTORY  ){
		printf("EMPIEZA reconocerDirectorioPadre %i: ****************\n", pos);
		printf("state_: %c\n", archivo->state);
		printf("parent_directory_: %i\n", archivo->parent_directory);
		printf("fname_: %s\n", &archivo->fname);
		printf("file_size_: %i\n", archivo->file_size);
		printf("lastmod_: %i\n", archivo->lastmod);
		printf("first_block_: %i\n", archivo->first_block);
		printf("Termina reconocerDirectorioPadre %i: ****************\n", pos);
	}
}

void crearArbolAPartirDelPadre(osada_file *tablaDeArchivo, int padre){
	int pos=0;

	for (pos=0; pos <= 2047; pos++){
		reconocerArchivosParaArbol(&tablaDeArchivo[pos], pos, padre);
	}

}

void encontrarArbolPadre(osada_file *tablaDeArchivo, int padre){
	int pos=0;

	//for (pos=0; pos <= 2047; pos++){
		reconocerDirectorioPadre(&tablaDeArchivo[pos], pos, padre);
	//}

}

/*****************************************************/
void crearUnDirectorio(){

}
/*******************************************FIN DIRECTORIO*************************/
