#include "SubString.h"
#include <assert.h>
#include <string.h> // only for memcmp

void SUBSTRING_Init3(SUBSTRING* S, const char* Begin, const char* End)
{
	assert(End >= Begin);
	S->Value = Begin;
	S->Size = (size_t)(End - Begin);
}

int SUBSTRING_Compare(SUBSTRING S1, SUBSTRING S2)
{
	if (S1.Size < S2.Size)
		return -1;
	if (S1.Size > S2.Size)
		return 1;
	return memcmp(S1.Value, S2.Value, S1.Size);
}

int SUBSTRING_Compare2(SUBSTRING S1, const char* S2_VAL, size_t S2_SZ)
{
	SUBSTRING S2;
	SUBSTRING_Init2(&S2, S2_VAL, S2_SZ);
	return SUBSTRING_Compare(S1, S2);
}

void SUBSTRING_Trim(SUBSTRING* S)
{
	// TODO: implement this
}

