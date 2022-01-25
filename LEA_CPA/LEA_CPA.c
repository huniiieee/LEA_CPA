#include "Analysis.h"


#if 0 //2����Ʈ �����ϰ� ������ �߰������� ����
void LEA_CPA(FILE* pt, FILE* trace, unsigned int Total_Point)
{
	//�߰���
	__int64* HW_BYTES = NULL;
	__int64* HWW_BYTES = NULL;

	//����, Max
	double* TR_POINTS = NULL;
	double* TRR_POINTS = NULL;
	double** HW_TR = NULL;
	double* Max = NULL;
	double* Temp_Points = NULL;

	//carry
	byte carry = 0;

	//�� 2����Ʈ �ҷ�����
	byte Plain1 = 0;
	byte Plain2 = 0;

	//���� ����Ʈ ���� �ӽú���
	float F_Temp=0.0 ; 

	//�߰����� �װ��� ��ֿ��� ��
	__int64 mid = 0;
	unsigned int hw = 0;
	
	//������ ���� �� �ʿ��� ����
	double corr_m = 0.;
	double corr_d = 0.;
	double corr = 0.;
	double max_cor = 0.;
	double max_key = 0.;

	//Ű �ĺ� ����
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

	//�� �ʱ�ȭ
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

	//RK[0]�� RK[1] ���ϴ� ����
	for (int i = 0; i < 1; i++)
	{
		_fseeki64(pt, (__int64)2 * (__int64)3, SEEK_SET);
		_fseeki64(trace, (__int64)32 + ((__int64)Start_Point - (__int64)1) * (__int64)4, SEEK_SET);
		for (__int64 tn = 0; tn < Trace_Num; tn++)
		{
			printf("%d", tn);

			fscanf_s(pt, "%hhx", &Plain1);
			_fseeki64(pt,  (__int64)9, SEEK_CUR);  //���� ���� �� 1����Ʈ ��ġ
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
		printf("\n 0��° �ĺ� RK[4],RK[5] ==>>>%08x %f\n", key_can, max_key);
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
			printf("\n %d��° �ĺ� RK[4],RK[5] ==>>>%08x %f\n", i+1, key_can, max_key);
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

#if 0 //�ѹ���Ʈ �����ؼ� xor������� �߰���
void LEA_CPA(FILE* pt, FILE* trace, unsigned int Total_Point)
{
	//�߰���
	__int64* HW_BYTES = NULL;
	__int64* HWW_BYTES = NULL;

	//����, Max
	double* TR_POINTS = NULL;
	double* TRR_POINTS = NULL;
	double** HW_TR = NULL;
	double* Max = NULL;
	double* Temp_Points = NULL;

	//carry
	byte carry = 0;

	//�� 2����Ʈ �ҷ�����
	byte Plain1 = 0;
	byte Plain2 = 0;

	//���� ����Ʈ ���� �ӽú���
	float F_Temp = 0.0;

	//�߰����� �װ��� ��ֿ��� ��
	__int64 mid = 0;
	unsigned int hw = 0;

	//������ ���� �� �ʿ��� ����
	double corr_m = 0.;
	double corr_d = 0.;
	double corr = 0.;
	double max_cor = 0.;
	double max_key = 0.;

	//Ű �ĺ� ����
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

	//�� �ʱ�ȭ
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

	//RK[0]�� RK[1] ���ϴ� ����
	for (int i = 0; i < 1; i++)
	{
		_fseeki64(pt, (__int64)0 * (__int64)3, SEEK_SET);
		_fseeki64(trace, (__int64)32 + ((__int64)Start_Point - (__int64)1) * (__int64)4, SEEK_SET);
		for (__int64 tn = 0; tn < Trace_Num; tn++)
		{
			printf("%d", tn);

			fscanf_s(pt, "%hhx", &Plain1);
			_fseeki64(pt, (__int64)9, SEEK_CUR);  //���� ���� �� 1����Ʈ ��ġ
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
				mid = Plain1 ^ guess_key;
				hw = (mid & 1) + ((mid >> 1) & 1) + ((mid >> 2) & 1) + ((mid >> 3) & 1) + ((mid >> 4) & 1) + ((mid >> 5) & 1) + ((mid >> 6) & 1) + ((mid >> 7) & 1);
				HW_BYTES[guess_key] += (__int64)hw;
				HWW_BYTES[guess_key] += (__int64)(hw * hw);

				for (__int64 point = 0; point < Point_Num; point++)
				{
					HW_TR[guess_key][point] += (double)hw * Temp_Points[point];
				}
			}
		}

		max_key = 0.0;
		for (__int64 guess_key = Guess_Key_Start; guess_key < Guess_Key_Start + Guess_Key_Num; guess_key++)
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
		printf("\n 0��° �ĺ� RK[4],RK[5] ==>>>%08x %f\n", key_can, max_key);
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
			printf("\n %d��° �ĺ� RK[4],RK[5] ==>>>%08x %f\n", i + 1, key_can, max_key);
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

//���������� 3��Ʈ ��ȯ�̵�
word ROR9(word text)
{
	return (text >> 9) ^ ((text & 0x1ff) << 23);
}

#if 1 //��ȣ�� ����ؼ� 1����Ʈ �����ϰ� �װ� ������ �����߰��� ����ؼ� Űã��
void LEA_CPA(FILE* pt, FILE* trace, FILE* ct, unsigned int Total_Point)
{
	//�߰���
	__int64* HW_BYTES = NULL;
	__int64* HWW_BYTES = NULL;

	//����, Max
	double* TR_POINTS = NULL;
	double* TRR_POINTS = NULL;
	double** HW_TR = NULL;
	double* Max = NULL;
	double* Temp_Points = NULL;

	//carry
	byte carry = 0;

	//�� 1����Ʈ �ҷ�����
	byte Plain1 = 0;

	//��ȣ�� 1����Ʈ �ҷ�����
	byte Cipher1 = 0;
	byte Cipher1_1 = 0;

	//��ȣ�� 4����Ʈ �ϱ�
	word Cipher4 = 0;

	//���� ����Ʈ ���� �ӽú���
	float F_Temp = 0.0;

	//�߰����� �װ��� ��ֿ��� ��
	byte mid = 0;
	unsigned int hw = 0;

	//������ ���� �� �ʿ��� ����
	double corr_m = 0.;
	double corr_d = 0.;
	double corr = 0.;
	double max_cor = 0.;
	double max_key = 0.;

	//Ű �ĺ� ����
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

	//�� �ʱ�ȭ
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

	//RK[23] Ű ����
	for (int i = 0; i < 1; i++)
	{
		_fseeki64(trace, (__int64)32 + ((__int64)Start_Point - (__int64)1) * (__int64)4, SEEK_SET);
		for (__int64 tn = 0; tn < Trace_Num; tn++)
		{
			Cipher4 = 0;
			for (int j = 0; j < 4; j++)
			{
				fscanf_s(ct, "%hhx", &Cipher1);
				Cipher4 += ((word)Cipher1 << (8 * j));
			}
			
			_fseeki64(ct, (__int64)8 * (__int64)3, SEEK_CUR);
			fscanf_s(ct, "%hhx", &Cipher1_1);
			_fseeki64(ct, (__int64)3 * (__int64)3 + (__int64)2, SEEK_CUR);
			Cipher4 = ROR9(Cipher4);
			Cipher1 = (byte)(Cipher4 & 0xff);

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
				mid = Cipher1 - (Cipher1_1 ^ (byte)guess_key);
				hw = (mid & 1) + ((mid >> 1) & 1) + ((mid >> 2) & 1) + ((mid >> 3) & 1) + ((mid >> 4) & 1) + ((mid >> 5) & 1) + ((mid >> 6) & 1) + ((mid >> 7) & 1);
				HW_BYTES[guess_key] += (__int64)hw;
				HWW_BYTES[guess_key] += (__int64)(hw * hw);

				for (__int64 point = 0; point < Point_Num; point++)
				{
					HW_TR[guess_key][point] += (double)hw * Temp_Points[point];
				}
			}
		}

		max_key = 0.0;
		for (__int64 guess_key = Guess_Key_Start; guess_key < Guess_Key_Start + Guess_Key_Num; guess_key++)
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
		printf("\n 0��° �ĺ� RK[23]==>>>%08x %f\n", key_can, max_key);
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
			printf("\n %d��° �ĺ� RK[23] ==>>>%08x %f\n", i + 1, key_can, max_key);
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