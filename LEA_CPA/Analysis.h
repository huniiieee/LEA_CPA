#ifndef _ANALYIS_H_
#define _ANALYIS_H_

/*
*�м��� ������ ���� ��ġ�� ���� ���
*���� �̸�
*�� �̸�
*/

#define Folder_Path "E:\LEA_8bit_add_3000_traces"
#define Trace_Path "2022.02.28-10.51.27-2022.02.28-10.51.27_3000tr_23828pt"
#define Plain_Path "2022.02.28-10.51.27-2022.02.28-10.51.27_3000tr_23828pt_plain"
#define Cipher_Path "2022.02.28-10.51.27-2022.02.28-10.51.27_3000tr_23828pt_cipher"

/*
* �м��� ���� ��
* �м� ���� ����Ʈ ��
* �м� ���� ����Ʈ ��
* �� �м� ����Ʈ ��
*/
#define Trace_Num 500
#define Start_Point 1
#define End_Point  23828
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

//���Ͽ� ������� â�� �������
#define File_out 1

typedef uint8_t byte;
typedef uint32_t word;
void Point_Verify(unsigned int* POINTS, unsigned int* TRACE_NUM);
void LEA_CPA(struct tm* Time, FILE* pt, FILE* trace, FILE* ct, unsigned int Total_Point);
word ROR9(word text);

#endif