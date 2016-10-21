/*

 * directorio.c
 *
 *  Created on: 16/9/2016
 *      Author: utnso

#include "osada.h"
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>

//Desarrollar las funciones para leer el contenido del árbol de directorios del filesystem OSADA.
//comprobar cual es root
//

//LEER LA SECUENCIA DE BLOQUES

 * A - LEER first_block
 * B - IR A TABLA ASIGANACION Y LEER EL BLOQUE
 * C - EL BLOQUE NOS DA EL PROXIMO BLOQUE A LEER(POSICIONES DEL ARRAY PONELE) Y EL FIN ES FFFFFFF
 * D - SUPONGO QUE ESOS BLOQUE SON LAS POSICIONES DEL BLOQUE DATOS
 *
 *
 *
***************LISTAR TODO ************************************************
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
***************FIN LISTAR TODO ************************************************
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

***************************************************

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
*******************************************************************************************************
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

***************************************************
void crearUnDirectorio(){

}
*/
