/*
 * osadaMain.c
 *
 *  Created on: 5/9/2016
 *      Author: utnso
 */
#include <stdint.h>
#include "osada.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    FILE *file = fopen("home/utnso/osada-utils/unArchivo", "r");
    osada_file *p = malloc(sizeof (osada_file));
    printf("entro");
    //fscanf(file, "%s", buff);
    //printf("1 : %s\n", buff );
    fread(p, sizeof (osada_file), 1, file);
    /*
    while(!feof(file))
    {
    	/*
    	osada_file ip;
        int ret = fscanf(file,"%*[ \t\n]%i",&ip.file_size);
        printf("1 : %i\n", ip.file_size );
        if (ret !=5)
        {
            //skip this record
            continue;
        }
        //process record ip
         */

    //}

	return 0;

}
