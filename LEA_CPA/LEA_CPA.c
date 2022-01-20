#include "Analysis.h"

void LEA_CPA(FILE* pt, FILE* trace, unsigned int Total_Point)
{
	//중간값
	__int64* HW_BYTES = NULL;
	__int64* HWW_BYTES = NULL;

	//파형, Max
	double* TR_POINTS = NULL;
	double* TRR_POINTS = NULL;
	double** HW_TR = NULL;
	double* Max = NULL;
	double* Temp_Points = NULL;

	//carry
	byte carry = 0;

	//평문 2바이트 불러오기
	byte Plain1 = 0;
	byte Plain2 = 0;

	//파형 포인트 읽을 임시변수
	float F_Temp=0.0 ; 

	//중간값
	__int64 mid = 0;

	//상관계수 구할 때 필요한 변수
	double corr_m = 0.;
	double corr_d = 0.;
	double corr = 0.;
	double max_cor = 0.;
	double max_key = 0.;

	//키 후보 변수
	__int64 key_can = 0;

	HW_BYTES = (__int64*)malloc(Guess_Key_Num * sizeof(__int64));
	HWW_BYTES = (__int64*)malloc(Guess_Key_Num * sizeof(__int64));

	TR_POINTS = (double*)malloc((unsigned int)Point_Num * sizeof(double));
	TRR_POINTS = (double*)malloc((unsigned int)Point_Num * sizeof(double));
	Max = (double*)malloc(Guess_Key_Num * sizeof(double));
	Temp_Points = (double*)malloc((unsigned int)Point_Num * sizeof(double));

	HW_TR = (double**)malloc(Guess_Key_Num * sizeof(double*));
	for (unsigned int i = 0; i < Guess_Key_Num; i++)
		HW_TR[i] = (double*)malloc((unsigned int)Point_Num * sizeof(double));

	//값 초기화
	for (unsigned int guess_key = Guess_Key_Start; guess_key < Guess_Key_Start + Guess_Key_Num; guess_key++)
	{
		HW_BYTES[guess_key] = 0;
		HWW_BYTES[guess_key] = 0;
		Max[guess_key] = 0.0;
	}

	for (unsigned int point = 0; point < Point_Num; point++)
	{
		TR_POINTS[point] = 0.0;
		TRR_POINTS[point] = 0.0;
		for (unsigned int guess_key = Guess_Key_Start; guess_key < Guess_Key_Start + Guess_Key_Num; guess_key++)
			HW_TR[guess_key][point] = 0.0;
	}

	//RK[0]와 RK[1] 구하는 과정
	for (int i = 0; i < 4; i++)
	{
		_fseeki64(pt, (__int64)i * (__int64)3, SEEK_SET);
		_fseeki64(trace, (__int64)32 + ((__int64)Start_Point - (__int64)1) * (__int64)4, SEEK_SET);
		for (__int64 tn = 0; tn < Trace_Num; tn++)
		{
			printf("%d", tn);

			fscanf_s(pt, "%hhx", &Plain1);
			_fseeki64(pt,  (__int64)9, SEEK_CUR);  //다음 읽을 평문 1바이트 위치
			fscanf_s(pt, "%hhx", &Plain2);
			_fseeki64(pt, (__int64)11 * (__int64)3 + (__int64)2, SEEK_CUR);

			for (__int64 point = 0; point < Point_Num; point++)
			{
				fread(&F_Temp, sizeof(float), 1, trace);
				Temp_Points[point] = (double)F_Temp;

				TR_POINTS[point] += Temp_Points[point];
				TRR_POINTS[point] += Temp_Points[point] * Temp_Points[point]; fscanf_s(pt, "%hhx", &Plain1);
			}
			_fseeki64(trace, ((__int64)Total_Point - (__int64)End_Point + (__int64)Start_Point - (__int64)1) * (__int64)4, SEEK_CUR);

			for (__int64 guess_key = Guess_Key_Start; guess_key < Guess_Key_Start + Guess_Key_Num; guess_key++)
			{
				mid=(Plain1 ^ (guess_key >> 8)) + (Plain2 ^ (guess_key & 0xff));
				HW_BYTES[guess_key] += mid;
				HWW_BYTES[guess_key] += mid * mid;

				for (__int64 point = 0; point < Point_Num; point++)
				{
					HW_TR[guess_key][point] += (double)mid * Temp_Points[point];
				}
			}
		}

		max_key = 0.0;
		for (__int64 guess_key = Guess_Key_Start; guess_key < Guess_Key_Start+Guess_Key_Num; guess_key++)
		{
			printf("%d", guess_key);
			max_cor = 0.0;
			for (__int64 point = 0; point < Point_Num; point++)
			{
				corr_m = (double)Trace_Num * HW_TR[guess_key][point] - (double)HW_BYTES[guess_key] * TR_POINTS[point];
				corr_d = ((double)Trace_Num * (double)HWW_BYTES[guess_key] - (double)HW_BYTES[guess_key] * (double)HW_BYTES[guess_key]) * ((double)Trace_Num * TRR_POINTS[point] - TR_POINTS[point] * TR_POINTS[point]);
				if (corr_d <= (double)0)
					corr = 0.0;
				else
				{
					corr = corr_m / sqrt(corr_d);
					corr = fabs(corr);
				}
				if (corr > max_cor)
				{
					max_cor = corr;
					Max[guess_key] = max_cor;
				}
			}
			if (Max[guess_key] > max_key)
			{
				max_key = Max[guess_key];
				key_can = guess_key;
			}

		}
		printf("\n RK[0],RK[1] ==>>>%04x\n", key_can);

	}
	free(HW_BYTES);
	free(HWW_BYTES);
	free(TR_POINTS);
	free(TRR_POINTS);
	free(Max);
	free(Temp_Points);
	for (unsigned int i = 0; i < Guess_Key_Num; i++)
		free(HW_TR[i]);
	free(HW_TR);

}