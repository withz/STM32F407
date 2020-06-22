#include "fatfsapp.h"



FATFS *storage = NULL;
FIL *file_ptr = NULL;
unsigned char storage_buf[4096];

int Storage_Init(void)
{
    FRESULT feedback = FR_OK;
    DWORD nc=0;
    DIR dir_item;
    FILINFO info;
    
    storage = (FATFS*)malloc(sizeof(FATFS));
    feedback = f_mount(storage, "1:", 1);
    if(feedback == FR_NO_FILESYSTEM){
        feedback = f_mkfs("1:", 0, storage_buf, sizeof(storage_buf));
        feedback = f_mount(NULL, "1:", 1);
		feedback = f_mount(storage, "1:", 1);
    }
    feedback = f_getfree("1:", &nc, &storage);
    f_findfirst(&dir_item, &info, "1:/", "");
    while (feedback == FR_OK && info.fname[0]) {  
        feedback = f_findnext(&dir_item, &info); 
    }
    return feedback;
}

int Storage_Free(void)
{
    free(storage);
    storage = NULL;
    return FR_OK;
}



