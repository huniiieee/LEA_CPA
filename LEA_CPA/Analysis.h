#ifndef _ANALYIS_H_
#define _ANALYIS_H_

/*
*분석할 파형과 평문이 위치한 폴더 경로
*파형 이름
*평문 이름
*/

#define Folder_Path "E:\LEA_8bit_add_3000_traces"
#define Trace_Path "2022.02.28-10.51.27-2022.02.28-10.51.27_3000tr_23828pt"
#define Plain_Path "2022.02.28-10.51.27-2022.02.28-10.51.27_3000tr_23828pt_plain"
#define Cipher_Path "2022.02.28-10.51.27-2022.02.28-10.51.27_3000tr_23828pt_cipher"

/*
* 분석할 파형 수
* 분석 시작 포인트 수
* 분석 종료 포인트 수
* 총 분석 포인트 수
*/
#define Trace_Num 500
#define Start_Point 1
#define End_Point  23828
#define Point_Num 23828

/*----------------------------------------------------------------------------------*/
/*  분석 시작할 키																	*/
/*  분석할 키 수																	*/
/*----------------------------------------------------------------------------------*/
#define Guess_Key_Start				0
#define Guess_Key_Num				256

/*
* 후보키 개수
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

//파일에 출력할지 창에 출력할지
#define File_out 1

typedef uint8_t byte;
typedef uint32_t word;
void Point_Verify(unsigned int* POINTS, unsigned int* TRACE_NUM);
void LEA_CPA(struct tm* Time, FILE* pt, FILE* trace, FILE* ct, unsigned int Total_Point);
word ROR9(word text);

#endif