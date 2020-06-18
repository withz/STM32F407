#include "fatfsapp.h"
#include "stdlib.h"
#include "usart1.h"

FATFS *storage = NULL;
FIL *file_ptr = NULL;
u8 storage_buf[4096];


int storage_init(void)
{
    FRESULT feedback = FR_OK;
    MKFS_PARM parm;
    DWORD nc=0;
    
    parm.fmt = FM_ANY;
    parm.n_root = 1;
    parm.n_fat = 1;
    parm.align = 1;
    parm.au_size = 512;
    
    storage = (FATFS*)malloc(sizeof(FATFS));
    feedback = f_mount(storage, "0:", 1);
    if(feedback == FR_NO_FILESYSTEM){
        feedback = f_mkfs("0:", 0, storage_buf, sizeof(storage_buf));
        feedback = f_getfree("0:", &nc, &storage);
        feedback = f_mount(NULL, "0:", 1);
		feedback = f_mount(storage, "0:", 1);
    }
    return FR_OK;
}

int storage_free(void)
{
    FRESULT feedback = FR_OK;
    free(storage);
    storage = NULL;
    return FR_OK;
}

int storage_write(void)
{
    FRESULT feedback = FR_OK;
    char *buf = "Hello Wow\n";
    feedback = f_write(file_ptr, buf, sizeof(buf), NULL);
    feedback = f_sync(file_ptr);
    return FR_OK;
}

int storage_open(char *filename)
{
    FRESULT feedback = FR_OK;
    file_ptr = (FIL*)malloc(sizeof(FIL));
    feedback = f_open(file_ptr, filename, FA_CREATE_NEW);
    return FR_OK;
}

int storage_close(char *filename)
{
    FRESULT feedback = FR_OK;
    feedback = f_close(file_ptr);
    free(file_ptr);
    file_ptr = NULL;
    return FR_OK;
}


