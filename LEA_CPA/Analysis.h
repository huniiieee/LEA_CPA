#ifndef _ANALYIS_H_
#define _ANALYIS_H_

/*
*�м��� ������ ���� ��ġ�� ���� ���
*���� �̸�
*�� �̸�
*/

//#define Folder_Path "E:\LEA_32bit_add_10000_traces"
//#define Trace_Path "2022.03.02-12.47.51-2022.03.02-12.47.51_10000tr_488pt"
//#define Plain_Path "2022.03.02-12.47.51-2022.03.02-12.47.51_10000tr_488pt_plain"
//#define Cipher_Path "2022.03.02-12.47.51-2022.03.02-12.47.51_10000tr_488pt_cipher"

#define Folder_Path "E:\\LEA_8bit_add_1000_traces"
#define Trace_Path "2022.03.07-09.19.37-2022.03.07-09.19.37_1000tr_23828pt"
#define Plain_Path "2022.03.07-09.19.37-2022.03.07-09.19.37_1000tr_23828pt_plain"
#define Cipher_Path "2022.03.07-09.19.37-2022.03.07-09.19.37_1000tr_23828pt_cipher"

#define Folder_Path2 "E:\\LEA_8bit_add_1000_traces_1ROUND"
#define Trace_Path2 "2022.03.04-10.29.52-2022.03.04-10.29.52_1000tr_23824pt"
#define Plain_Path2 "2022.03.04-10.29.52-2022.03.04-10.29.52_1000tr_23824pt_plain"
#define Cipher_Path2 "2022.03.04-10.29.52-2022.03.04-10.29.52_1000tr_23824pt_cipher"

/*
* �м��� ���� ��
* �м� ���� ����Ʈ ��
* �м� ���� ����Ʈ ��
* �� �м� ����Ʈ ��
*/
#define Trace_Num 1000
#define Start_Point 1
#define End_Point 23828
#define Point_Num 23828

/*----------------------------------------------------------------------------------*/
/*  �м� ������ Ű																	*/
/*  �м��� Ű ��																	*/
/*----------------------------------------------------------------------------------*/
#define Guess_Key_Start				0
#define Guess_Key_Num				256

/*
* �ĺ�Ű ����
*/
#define Candidates 5

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

/*
* ���Ͽ� ������� �ܼ�â�� �������
* 1: ���Ͽ� ��� 0: �ܼ�â�� ���
*/
#define File_out 0

/*
* 8��Ʈ ���� ���� ���� �������
* 1: 8��Ʈ ���� 0: 32��Ʈ ����
*/
#define Eight_bit_version 1


typedef uint8_t byte;
typedef uint32_t word;
void Point_Verify(unsigned int* POINTS, unsigned int* TRACE_NUM);
void LEA_CPA(struct tm* Time, FILE* pt, FILE* trace, FILE* trace2, FILE* ct, unsigned int Total_Point);
word ROR9(word text);
word ROR(word text, int i);
word ROL(word text, int n);

#endif