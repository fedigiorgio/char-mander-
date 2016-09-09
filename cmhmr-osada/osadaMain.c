/*
 * osadaMain.c
 *
 *  Created on: 5/9/2016
 *      Author: utnso
 */

#include "osada.h"

#include <stdio.h>
/*
 	 	 	 	 Antes hay que hacer esto:
 	 	 	 	 truncate --size 100M disco.bin
 	 	 	 	 ./osada-format disco.bin
*/
int main(void) {
	char *ruta = "/home/utnso/tp-2016-2c-CompuMundoHiperMegaRed/cmhmr-osada/disco.bin";
	osada_file *osadaFile = leerArchivoParaOsadaFile(ruta);
	osada_header *osadaHeader = leerArchivoParaHeader(ruta);

	printf("Tamanio: %i\n", devolverTamanio(osadaFile));
	printf("Nombre: %d\n", (unsigned char)devolverNombreDelArchivo(osadaFile));
	printf("Directorio Padre: %i\n", devolverDirectorioPadre(osadaFile));
	printf("Ultima Mod: %d\n", devolverUltimaMod(osadaFile));
	printf("Primer Bloque: %i\n", devolverPrimerBloque(osadaFile));


	printf("magic_number: %i\n", devolverMagicNumbre(osadaHeader));
	printf("version: %i\n", devolverVersion(osadaHeader));
	printf("fs_blocks: %i\n", devolverFSBloques(osadaHeader));
	printf("bitmap_blocks: %i\n", devolverBitMapBlocks(osadaHeader));
	printf("allocations_table_offset: %i\n", devolverAllocations_table_offset(osadaHeader));
	printf("fs_blocks: %i\n", devolverFSBloques(osadaHeader));
	printf("data_blocks: %i\n", devolverDataBlocks(osadaHeader));
	printf("padding: %i\n", devolverPadding(osadaHeader));




	ruta = "";
	free(osadaFile);
	free(osadaHeader);

	return 0;

}
