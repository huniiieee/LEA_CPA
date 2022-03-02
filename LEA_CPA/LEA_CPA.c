#include "Analysis.h"


#if 0 //2바이트 추정하고 덧셈을 중간값으로 설정
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

	//중간값과 그것의 헤밍웨이 값
	__int64 mid = 0;
	unsigned int hw = 0;
	
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
	for (int i = 0; i < 1; i++)
	{
		_fseeki64(pt, (__int64)2 * (__int64)3, SEEK_SET);
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
				TRR_POINTS[point] += Temp_Points[point] * Temp_Points[point]; 
			}
			_fseeki64(trace, ((__int64)Total_Point - (__int64)End_Point + (__int64)Start_Point - (__int64)1) * (__int64)4, SEEK_CUR);

			for (__int64 guess_key = Guess_Key_Start; guess_key < Guess_Key_Start + Guess_Key_Num; guess_key++)
			{
				mid=(Plain1 ^ (guess_key >> 8)) + (Plain2 ^ (guess_key & 0xff))+1;
				hw = (mid & 1) + ((mid >> 1) & 1) + ((mid >> 2) & 1) + ((mid >> 3) & 1) + ((mid >> 4) & 1) + ((mid >> 5) & 1) + ((mid >> 6) & 1) + ((mid >> 7) & 1);
				HW_BYTES[guess_key] += (__int64)hw;
				HWW_BYTES[guess_key] += (__int64)(hw*hw);

				for (__int64 point = 0; point < Point_Num; point++)
				{
					HW_TR[guess_key][point] += (double)hw * Temp_Points[point];
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
		printf("\n 0번째 후보 RK[4],RK[5] ==>>>%08x %f\n", key_can, max_key);
		Max[key_can] = 0.0;
		
		for (int i = 0; i < 10; i++)
		{
			max_key = 0.0;
			for (int j = 0; j < Guess_Key_Num; j++)
			{
				if (Max[j] > max_key)
				{
					max_key = Max[j];
					key_can = j;
				}
			}
			printf("\n %d번째 후보 RK[4],RK[5] ==>>>%08x %f\n", i+1, key_can, max_key);
			Max[key_can] = 0.0;
		}

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
#endif



//오른쪽으로 9비트 순환이동
word ROR9(word text)
{
	return (text >> 9) ^ ((text & 0x1ff) << 23);
}

#if 1 //암호문 사용해서 1바이트 추정하고 그거 가지고 덧셈중간값 사용해서 키찾기
void LEA_CPA(struct tm* Time,FILE* pt, FILE* trace, FILE* ct, unsigned int Total_Point)
{

	char FOLD_MERGE[_FILE_NAME_SIZE_] = "";
	char FILE_MERGE[_FILE_NAME_SIZE_] = "";

	FILE* fp = NULL;
	FILE* fp2 = NULL;

	//중간값
	__int64* HW_BYTES = NULL;
	__int64* HWW_BYTES = NULL;

	//파형, Max
	double* TR_POINTS = NULL;
	double* TRR_POINTS = NULL;
	double** HW_TR = NULL;
	double* Max = NULL;
	double* Temp_Points = NULL;

	//Borrow
	byte** Borrow = NULL;

	//올바른 바이트 키
	word Key = 0;
	
	//임시변수
	word temp = 0;

	//평문 1바이트 불러오기
	byte Plain1 = 0;


	//암호문 1바이트 불러오기
	byte Cipher1 = 0;
	byte Cipher1_1 = 0;

	//암호문 4바이트 일기
	word Cipher4 = 0;
	word Cipher4_1 = 0;

	//파형 포인트 읽을 임시변수
	float F_Temp = 0.0;

	//중간값과 그것의 헤밍웨이 값
	byte mid = 0;
	unsigned int hw = 0;

	//상관계수 구할 때 필요한 변수
	double corr_m = 0.;
	double corr_d = 0.;
	double corr = 0.;
	double max_cor = 0.;
	double sec_cor = 0.;
	double max_key = 0.;
	double Ratio = 0.;

	//키 후보 변수
	__int64 key_can = 0;
	__int64 sec_key = 0.;

	Borrow = (byte**)malloc(Trace_Num * sizeof(byte*));
	for (int i = 0; i < Trace_Num; i++)
		Borrow[i] = (byte*)malloc(Guess_Key_Num * sizeof(byte));

	HW_BYTES = (__int64*)malloc(Guess_Key_Num * sizeof(__int64));
	HWW_BYTES = (__int64*)malloc(Guess_Key_Num * sizeof(__int64));

	TR_POINTS = (double*)malloc((unsigned int)Point_Num * sizeof(double));
	TRR_POINTS = (double*)malloc((unsigned int)Point_Num * sizeof(double));
	Max = (double*)malloc(Guess_Key_Num * sizeof(double));
	Temp_Points = (double*)malloc((unsigned int)Point_Num * sizeof(double));

	HW_TR = (double**)malloc(Guess_Key_Num * sizeof(double*));
	for (unsigned int i = 0; i < Guess_Key_Num; i++)
		HW_TR[i] = (double*)malloc((unsigned int)Point_Num * sizeof(double));

	//Borrow값 초기화
	for (int i = 0; i < Trace_Num; i++)
	{
		for (int j = 0; j < Guess_Key_Num; j++)
			Borrow[i][j] = 0;
	}
#if File_out==1
	// 결과 저장할 폴더 생성
	sprintf_s(FOLD_MERGE, _FILE_NAME_SIZE_ * sizeof(char), "%s\\%04d_%02d_%02d_%02d_%02d_%02d", Folder_Path, Time->tm_year + 1900, Time->tm_mon + 1, Time->tm_mday, Time->tm_hour, Time->tm_min, Time->tm_sec);
	_mkdir(FOLD_MERGE);
#endif
	//RK[23] 키 추측
	for (int _byte_ = 0; _byte_ < 4; _byte_++)
	{
		//값 초기화
		printf("-----%d번 째 바이트 분석-----\n", _byte_);
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

		_fseeki64(ct, 0, SEEK_SET);
		_fseeki64(trace, (__int64)32 + ((__int64)Start_Point - (__int64)1) * (__int64)4, SEEK_SET);
		for (__int64 tn = 0; tn < Trace_Num; tn++)
		{
			Cipher4 = 0;
			Cipher4_1 = 0;
			for (int j = 0; j < 4; j++)
			{
				fscanf_s(ct, "%hhx", &Cipher1);
				Cipher4 += ((word)Cipher1 << (8 * j));
			}

			_fseeki64(ct, (__int64)8 * (__int64)3, SEEK_CUR);
			for (int j = 0; j < 4; j++)
			{
				fscanf_s(ct, "%hhx", &Cipher1);
				Cipher4_1 += ((word)Cipher1 << (8 * j));
			}
			_fseeki64(ct, (__int64)2, SEEK_CUR);

			Cipher4 = ROR9(Cipher4);

			for (__int64 point = 0; point < Point_Num; point++)
			{
				fread(&F_Temp, sizeof(float), 1, trace);
				Temp_Points[point] = (double)F_Temp;

				TR_POINTS[point] += Temp_Points[point];
				TRR_POINTS[point] += Temp_Points[point] * Temp_Points[point];
			}
			_fseeki64(trace, ((__int64)Total_Point - (__int64)End_Point + (__int64)Start_Point - (__int64)1) * (__int64)4, SEEK_CUR);

			for (__int64 guess_key = Guess_Key_Start; guess_key < Guess_Key_Start + Guess_Key_Num; guess_key++)
			{
				//첫번 째 바이트

				temp = guess_key << (8 * _byte_);
				temp ^= Key;
				mid = (byte)((Cipher4 - (Cipher4_1 ^ temp)) >> (8 * _byte_));
				mid = mid - Borrow[tn][guess_key];

				if ((byte)(Cipher4 >> (8 * _byte_)) <= mid)
					Borrow[tn][guess_key] = 1;
				else
					Borrow[tn][guess_key] = 0;

				hw = (mid & 1) + ((mid >> 1) & 1) + ((mid >> 2) & 1) + ((mid >> 3) & 1) + ((mid >> 4) & 1) + ((mid >> 5) & 1) + ((mid >> 6) & 1) + ((mid >> 7) & 1);
				HW_BYTES[guess_key] += (__int64)hw;
				HWW_BYTES[guess_key] += (__int64)(hw * hw);

				for (__int64 point = 0; point < Point_Num; point++)
				{
					HW_TR[guess_key][point] += (double)hw * Temp_Points[point];
				}
			}
		}
#if File_out==1

		//결과출력 파일
		sprintf_s(FILE_MERGE, _FILE_NAME_SIZE_ * sizeof(char), "%s\\%d", FOLD_MERGE, _byte_ + 1);
		_mkdir(FILE_MERGE);

		max_key = 0.0;
		for (__int64 guess_key = Guess_Key_Start; guess_key < Guess_Key_Start + Guess_Key_Num; guess_key++)
		{
			sprintf_s(FILE_MERGE, _FILE_NAME_SIZE_ * sizeof(char), "%s\\%d\\%03d(0x%02X).txt", FOLD_MERGE, _byte_ + 1, guess_key + Guess_Key_Start, guess_key + Guess_Key_Start);
			fopen_s(&fp, FILE_MERGE, "w");

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

				fprintf_s(fp, "%f\n", corr);

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

			fclose(fp);
		}

		// Guess Key Peak 저장
		sprintf_s(FILE_MERGE, _FILE_NAME_SIZE_ * sizeof(char), "%s\\%d_GUESS_KEY_PEAK.txt", FOLD_MERGE, _byte_ + 1);
		fopen_s(&fp, FILE_MERGE, "w");

		// 최종 분석 키 저장
		sprintf_s(FILE_MERGE, _FILE_NAME_SIZE_ * sizeof(char), "%s\\%d_RIGHT_KEY.txt", FOLD_MERGE, _byte_ + 1);
		fopen_s(&fp2, FILE_MERGE, "w");

		max_cor = 0;
		for (__int64 guess_key = 0; guess_key < Guess_Key_Num; guess_key++) {
			fprintf_s(fp, "%f\n", Max[guess_key]);

			if (max_cor < Max[guess_key]) {
				max_cor = Max[guess_key];
				key_can = guess_key;
			}
		}
		Key ^= key_can << (8 * _byte_);

		fclose(fp);

		fprintf_s(fp2, "  1st  0x%02X  %f\n", key_can, max_cor);

		Max[key_can] = 0.0;

		for (int i = 1; i < Candidates; i++) {
			for (__int64 guess_key = 0; guess_key < Guess_Key_Num; guess_key++) {
				if (sec_cor < Max[guess_key]) {
					sec_cor = Max[guess_key];
					sec_key = guess_key;
				}
			}

			if (i == 1) {
				fprintf_s(fp2, "  2nd  0x%02X  %f\n", sec_key, sec_cor);
				Ratio = max_cor / sec_cor;
			}
			else if (i == 2) {
				fprintf_s(fp2, "  3rd  0x%02X  %f\n", sec_key, sec_cor);
			}
			else {
				fprintf_s(fp2, "%3dth  0x%02X  %f\n", i + 1, sec_key, sec_cor);
			}

			Max[sec_key] = 0.0;
			sec_cor = 0.0;
			sec_key = 0;
		}

		fprintf_s(fp2, "\nRatio  %f\n", Ratio);

		fclose(fp2);
#else 
		max_key = 0.0;
		for (__int64 guess_key = Guess_Key_Start; guess_key < Guess_Key_Start + Guess_Key_Num; guess_key++)
		{
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
		Key ^= key_can << (8 * _byte_);
		printf("\n 0번째 후보 RK[23]==>>>%08x %f\n", key_can, max_key);
		Max[key_can] = 0.0;
		for (int i = 0; i < 10; i++)
		{
			max_key = 0.0;
			for (int j = 0; j < Guess_Key_Num; j++)
			{
				if (Max[j] > max_key)
				{
					max_key = Max[j];
					key_can = j;
				}
			}
			printf("\n %d번째 후보 RK[23] ==>>>%08x %f\n", i + 1, key_can, max_key);
			Max[key_can] = 0.0;
		}

#endif
		
	}
	printf("Correct RK[23] is %08x", Key);
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
#endif