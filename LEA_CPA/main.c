#include "Analysis.h"

int main()
{
	FILE* pt = NULL;
	FILE* trace = NULL; 
	char FILE_MERGE[_FILE_NAME_SIZE_] = "";
	// 
	sprintf_s(FILE_MERGE, _FILE_NAME_SIZE_ * sizeof(char), "%s\\%s.trace", Folder_Path, Trace_Path);
	fopen_s(&trace, FILE_MERGE, "rb");
	if (trace == NULL) {
		printf(" -----------------------------------------------------------------------\n");
		printf("|                          Failed To Read Trace                         |\n");
		printf(" -----------------------------------------------------------------------\n");
		return;
	}

	sprintf_s(FILE_MERGE, _FILE_NAME_SIZE_ * sizeof(char), "%s\\%s.txt", Folder_Path, Plain_Path);
	fopen_s(&pt, FILE_MERGE, "r");
	if (pt == NULL) {
		printf(" -----------------------------------------------------------------------\n");
		printf("|                          Failed To Read Plaintext                         |\n");
		printf(" -----------------------------------------------------------------------\n");
		return;
	}
	unsigned int Points = 3736;
	unsigned int Trace= 0;
	Point_Verify(&Points, &Trace);
	LEA_CPA(pt, trace, Points);
	fclose(pt);
	fclose(trace);
}
