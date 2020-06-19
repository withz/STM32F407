#ifndef FATFSAPP_H
#define FATFSAPP_H


#include "ff.h"	
#include "stdlib.h"

int storage_init(void);
int storage_free(void);
int storage_open(char *filename);
int storage_close(char *filename);
int storage_write(void);
int storage_read(void);

#endif
