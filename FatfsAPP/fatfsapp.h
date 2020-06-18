#ifndef FATFSAPP_H
#define FATFSAPP_H

#include "stm32f4xx.h"
#include "ff.h"	

int storage_init(void);
int storage_free(void);
int storage_open(char *filename);
int storage_close(char *filename);
int storage_write(void);

#endif
