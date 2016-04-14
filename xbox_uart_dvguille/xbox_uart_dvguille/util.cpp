#include "util.h"

// Convert to a wchar_t*
wchar_t* convert(char* dataprint) {
	size_t bufsize = strlen(dataprint) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t* wcstring = new wchar_t[newsize];//malloc(sizeof(wchar_t) * newsize);
	mbstowcs_s(&convertedChars, wcstring, bufsize, dataprint, _TRUNCATE);
	return wcstring;
}