#ifndef _ANALYIS_H_
#define _ANALYIS_H_

/*
*�м��� ������ ���� ��ġ�� ���� ���
*���� �̸�
*�� �̸�
*/

#define Folder_Path "C:\\Users\\leesehun\\chipwhisperer\\projects\\tmp\\default_data\\traces_23"
#define Trace_Path "2022.01.25-16.09.14-2022.01.25-16.09.14_1000tr_3740pt"
#define Plain_Path "2022.01.25-16.09.14-2022.01.25-16.09.14_1000tr_3740pt_plain"
#define Cipher_Path "2022.01.25-16.09.14-2022.01.25-16.09.14_1000tr_3740pt_cipher"

/*
* �м��� ���� ��
* �м� ���� ����Ʈ ��
* �м� ���� ����Ʈ ��
* �� �м� ����Ʈ ��
*/
#define Trace_Num 1000
#define Start_Point 1
#define End_Point  3740
#define Point_Num 3740

/*----------------------------------------------------------------------------------*/
/*  �м� ������ Ű																	*/
/*  �м��� Ű ��																	*/
/*----------------------------------------------------------------------------------*/
#define Guess_Key_Start				0
#define Guess_Key_Num				256

typedef struct {
	unsigned char strID[20];
	unsigned int Trace_No;
	unsigned int Point_No;
	unsigned char strEND[4];
} Trace_Header;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <direct.h>

#define _FILE_NAME_SIZE_	1000

typedef uint8_t byte;
typedef uint32_t word;
void Point_Verify(unsigned int* POINTS, unsigned int* TRACE_NUM);
void LEA_CPA(FILE* pt, FILE* trace,FILE* ct, unsigned int Total_Point);
word ROR9(word text);

#endif