#ifndef USART_H
#define USART_H

#include "stm32f4xx.h"

void USART1_Configuration(void);
void USART2_Configuration(void);
void USART3_Configuration(void);

void USART1_SendChar(char b);
void USART2_SendChar(char b);
void USART3_SendChar(char b);

void USART1_SendWords(char* c);
void USART2_SendWords(char* c);
void USART3_SendWords(char* c);

void USART1_GetData(char* s);
void USART2_GetData(char* s);
void USART3_GetData(char* s);

void USART1_GetWord(char* s);
void USART2_GetWord(char* s);
void USART3_GetWord(char* s);



#endif


