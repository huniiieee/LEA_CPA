#ifndef _ANALYIS_H_
#define _ANALYIS_H_

/*
*분석할 파형과 평문이 위치한 폴더 경로
*파형 이름
*평문 이름
*/

#define Folder_Path "C:\\Users\\leesehun\\chipwhisperer\\projects\\tmp\\default_data\\traces"
#define Trace_Path "2022.01.19-16.12.10-2022.01.19-16.12.10_1000tr_3736pt"
#define Plain_Path "2022.01.19-16.12.10-2022.01.19-16.12.10_1000tr_3736pt_plain"

/*
* 분석할 파형 수
* 분석 시작 포인트 수
* 분석 종료 포인트 수
* 총 분석 포인트 수
*/
#define Trace_Num 1000
#define Start_Point 1
#define End_Point 3736	
#define Point_Num 3736

/*----------------------------------------------------------------------------------*/
/*  분석 시작할 키																	*/
/*  분석할 키 수																	*/
/*----------------------------------------------------------------------------------*/
#define Guess_Key_Start				0
#define Guess_Key_Num				65536

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
void Point_Verify(unsigned int* POINTS, unsigned int* TRACE_NUM);
void LEA_CPA(FILE* pt, FILE* trace, unsigned int Total_Point);

#endif