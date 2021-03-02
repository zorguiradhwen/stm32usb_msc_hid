/*
 * File_Handling_RTOS.c
 *
 *  Created on: 14-May-2020
 *      Author: Controllerstech
 */

#include "File_Handling.h"

#include "stm32f4xx_hal.h"


//extern UART_HandleTypeDef huart1;
//#define UART &huart1



/* =============================>>>>>>>> NO CHANGES AFTER THIS LINE =====================================>>>>>>> */

FATFS fs;  // file system
FIL fil; // File
FILINFO fno;
FRESULT fresult;  // result
UINT br, bw;  // File read/write count

/**** capacity related *****/
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;



void Mount_SD (const TCHAR* path)
{
	fresult = f_mount(&fs, path, 1);
	if (fresult != FR_OK) printf ("ERROR!!! in mounting SD CARD...\n\n\r");
	else printf("SD CARD mounted successfully...\n\r");
}

void Unmount_SD (const TCHAR* path)
{
	fresult = f_mount(NULL, path, 1);
	if (fresult == FR_OK) printf ("SD CARD UNMOUNTED successfully...\n\n\n\r");
	else printf("ERROR!!! in UNMOUNTING SD CARD\n\n\n\r");
}

/* Start node to be scanned (***also used as work area***) */
FRESULT Scan_SD (char* pat)
{
    DIR dir;
    UINT i;
    char path[20] = "";
    sprintf (path, "%s",pat);

    fresult = f_opendir(&dir, path);                       /* Open the directory */
    if (fresult == FR_OK)
    {
        for (;;)
        {
            fresult = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (fresult != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR)     /* It is a directory */
            {
            	if (!(strcmp ("SYSTEM~1", fno.fname))) continue;
            	printf ( "Dir: %s\r\n\r", fno.fname);
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
                fresult = Scan_SD(path);                     /* Enter the directory */
                if (fresult != FR_OK) break;
                path[i] = 0;
            }
            else
            {   /* It is a file. */
               printf("File: %s/%s\n\r", path, fno.fname);
            }
        }
        f_closedir(&dir);
    }
    return fresult;
}

/* Only supports removing files from home directory */
FRESULT Format_SD (void)
{
    DIR dir;
    char path[20] = "";
    sprintf (path, "%s","/");

    fresult = f_opendir(&dir, path);                       /* Open the directory */
    if (fresult == FR_OK)
    {
        for (;;)
        {
            fresult = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (fresult != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR)     /* It is a directory */
            {
            	if (!(strcmp ("SYSTEM~1", fno.fname))) continue;
            	fresult = f_unlink(fno.fname);
            	if (fresult == FR_DENIED) continue;
            }
            else
            {   /* It is a file. */
               fresult = f_unlink(fno.fname);
            }
        }
        f_closedir(&dir);
    }
    return fresult;
}


FRESULT Write_File (char *name, char *data)
{

	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &fno);
	if (fresult != FR_OK)
	{
		printf ("ERROR!!! *%s* does not exists\n\n\r", name);
	    return fresult;
	}
	else
	{
	    /* Create a file with read write access and open it */
	    fresult = f_open(&fil, name, FA_OPEN_EXISTING | FA_WRITE);
	    if (fresult != FR_OK)
	    {
	    	printf ("ERROR!!! No. %d in opening file *%s*\n\n\r", fresult, name);
	        return fresult;
	    }
	    else
	    {
	    	fresult = f_write(&fil, data, strlen(data), &bw);
	    	if (fresult != FR_OK)
	    	{
	    		printf ("ERROR!!! No. %d while writing to the FILE *%s*\n\n\r", fresult, name);
	    	}
	    	/* Close file */
	    	fresult = f_close(&fil);
	    	if (fresult != FR_OK)
	    	{
	    		printf ("ERROR!!! No. %d in closing file *%s* after writing it\n\n\r", fresult, name);
	    	}
	    	else
	    	{
	    		printf ("File *%s* is WRITTEN and CLOSED successfully\n\r", name);
	    	}
	    }
	    return fresult;
	}
}

FRESULT Read_File (char *name)
{
	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &fno);
	if (fresult != FR_OK)
	{
		printf ("ERRROR!!! *%s* does not exists\n\n\r", name);
	    return fresult;
	}

	else
	{
		/* Open file to read */
		fresult = f_open(&fil, name, FA_READ);

		if (fresult != FR_OK)
		{
			printf ("ERROR!!! No. %d in opening file *%s*\n\n\r", fresult, name);
		    return fresult;
		}

		/* Read data from the file
		* see the function details for the arguments */

		char *buffer = malloc(sizeof(f_size(&fil)));
		fresult = f_read (&fil, buffer, f_size(&fil), &br);
		if (fresult != FR_OK)
		{
			free(buffer);
		 	printf ("ERROR!!! No. %d in reading file *%s*\n\n\r", fresult, name);
		}
		else
		{
			printf("%s\n\r", buffer);
			free(buffer);
			/* Close file */
			fresult = f_close(&fil);
			if (fresult != FR_OK)
			{
				printf ("ERROR!!! No. %d in closing file *%s*\n\n\r", fresult, name);
			}
			else
			{
				printf ("File *%s* CLOSED successfully\n\r", name);
			}
		}
	    return fresult;
	}
}

FRESULT Create_File (char *name)
{
	fresult = f_stat (name, &fno);
	if (fresult == FR_OK)
	{
		printf ("ERROR!!! *%s* already exists!!!!\n\r use Update_File \n\n\r",name);
	    return fresult;
	}
	else
	{
		fresult = f_open(&fil, name, FA_CREATE_ALWAYS|FA_READ|FA_WRITE);
		if (fresult != FR_OK)
		{
			printf ( "ERROR!!! No. %d in creating file *%s*\n\n\r", fresult, name);
		    return fresult;
		}
		else
		{
			printf ("*%s* created successfully\n\r Now use Write_File to write data\n\r",name);
		}
		fresult = f_close(&fil);
		if (fresult != FR_OK)
		{
			printf("ERROR No. %d in closing file *%s*\n\n\r", fresult, name);
		}
		else
		{
			printf ("File *%s* CLOSED successfully\n\r", name);
		}
	}
    return fresult;
}

FRESULT Update_File (char *name, char *data)
{
	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &fno);
	if (fresult != FR_OK)
	{
		printf ("ERROR!!! *%s* does not exists\n\n\r", name);
	    return fresult;
	}

	else
	{
		 /* Create a file with read write access and open it */
	    fresult = f_open(&fil, name, FA_OPEN_APPEND | FA_WRITE);
	    if (fresult != FR_OK)
	    {
	    	printf ("ERROR!!! No. %d in opening file *%s*\n\n\r", fresult, name);
	        return fresult;
	    }
	    /* Writing text */
	    fresult = f_write(&fil, data, strlen (data), &bw);
	    if (fresult != FR_OK)
	    {
	    	printf ("ERROR!!! No. %d in writing file *%s*\n\n\r", fresult, name);
	    }
	    else
	    {
	    	printf ("*%s* UPDATED successfully\n\r", name);
	    }

	    /* Close file */
	    fresult = f_close(&fil);
	    if (fresult != FR_OK)
	    {
	    	printf ("ERROR!!! No. %d in closing file *%s*\n\n\r", fresult, name);
	    }
	    else
	    {
	    	printf ("File *%s* CLOSED successfully\n\r", name);
	    }
	}
    return fresult;
}

FRESULT Remove_File (char *name)
{
	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &fno);
	if (fresult != FR_OK)
	{
		printf ("ERROR!!! *%s* does not exists\n\n\r", name);
		return fresult;
	}

	else
	{
		fresult = f_unlink (name);
		if (fresult == FR_OK)
		{
			sprintf ("*%s* has been removed successfully\n\r", name);
		}

		else
		{
			printf ("ERROR No. %d in removing *%s*\n\n\r",fresult, name);
		}
	}
	return fresult;
}

FRESULT Create_Dir (char *name)
{
    fresult = f_mkdir(name);
    if (fresult == FR_OK)
    {
    	printf ("*%s* has been created successfully\n\r", name);   	
    }
    else
    {
    	printf ("ERROR No. %d in creating directory *%s*\n\n\r", fresult,name);
    }
    return fresult;
}

void Check_SD_Space (void)
{
    /* Check free space */
    f_getfree("", &fre_clust, &pfs);

    total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
    printf("SD CARD Total Size: \t%lu\n\r",total);
    
    free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
    printf("SD CARD Free Space: \t%lu\n\r",free_space);
    
}

