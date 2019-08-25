#pragma once

#include <stdint.h>

typedef struct _SUBSTRING
{
	const char* Value;
	size_t Size;
} SUBSTRING;

#define SUBSTRING_Init2(S,VAL,SZ) \
	(S)->Value = VAL; \
	(S)->Size = SZ;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void SUBSTRING_Init3(SUBSTRING* S, const char* Begin, const char* End);

int SUBSTRING_Compare(SUBSTRING S1, SUBSTRING S2);
int SUBSTRING_Compare2(SUBSTRING S1, const char* S2_VAL, size_t S2_SZ);
void SUBSTRING_Trim(SUBSTRING* S);

#define SUBSTRING_CompareWitchConst(S1, CS2) (SUBSTRING_Compare2(S1, CS2, sizeof(CS2)-1))

#ifdef __cplusplus
}
#endif // __cplusplus
	

