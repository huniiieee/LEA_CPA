#ifndef _ANALYIS_H_
#define _ANALYIS_H_

/*
*�м��� ������ ���� ��ġ�� ���� ���
*���� �̸�
*�� �̸�
*/

#define Folder_Path "E:\\ohman"
#define Trace_Path "trace1"
#define Plain_Path "plain"
#define Cipher_Path "cipher"

/*
* �м��� ���� ��
* �м� ���� ����Ʈ ��
* �м� ���� ����Ʈ ��
* �� �м� ����Ʈ ��
*/
#define Trace_Num 50000
#define Start_Point 1
#define End_Point  3896
#define Point_Num 3896

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