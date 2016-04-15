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

int	pack(uint8_t* buffer) {
	for (int i = 0; i < sizeof(buffer) - 1; i++) {

	}
	return 5;
}

int calcPressure(uint32_t D1, uint32_t D2) {
	//uint32_t D1 = p->digital_pressure;
	//uint32_t D2 = p->digital_temperature;
	uint16_t C[8];
	C[0] = 0;
	C[1] = 34982;
	C[2] = 36352;
	C[3] = 20328;
	C[4] = 22354;
	C[5] = 26646;
	C[6] = 26146;
	C[7] = 0;

	int32_t dT;
	int64_t SENS;
	int64_t OFF;
	int32_t SENSi;
	int32_t OFFi;
	int32_t Ti;
	int64_t OFF2;
	int64_t SENS2;
	int64_t TEMP;
	int64_t P;

	// Terms called
	dT = D2 - uint32_t(C[5]) * 256l;
	SENS = int64_t(C[1]) * 32768l + (int64_t(C[3])*dT) / 256l;
	OFF = int64_t(C[2]) * 65536l + (int64_t(C[4])*dT) / 128l;


	//Temp and P conversion
	TEMP = 2000l + int64_t(dT)*C[6] / 8388608LL;
	P = (D1*SENS / (2097152l) - OFF) / (8192l);

	//Second order compensation
	if ((TEMP / 100)<20) {         //Low temp
		Ti = (3 * int64_t(dT)*int64_t(dT)) / (8589934592LL);
		OFFi = (3 * (TEMP - 2000)*(TEMP - 2000)) / 2;
		SENSi = (5 * (TEMP - 2000)*(TEMP - 2000)) / 8;
		if ((TEMP / 100)<-15) {    //Very low temp
			OFFi = OFFi + 7 * (TEMP + 1500l)*(TEMP + 1500l);
			SENSi = SENSi + 4 * (TEMP + 1500l)*(TEMP + 1500l);
		}
	}
	else if ((TEMP / 100) >= 20) {    //High temp
		Ti = 2 * (dT*dT) / (137438953472LL);
		OFFi = (1 * (TEMP - 2000)*(TEMP - 2000)) / 16;
		SENSi = 0;
	}

	OFF2 = OFF - OFFi;           //Calculate pressure and temp second order
	SENS2 = SENS - SENSi;

	TEMP = (TEMP - Ti);
	P = (((D1*SENS2) / 2097152l - OFF2) / 8192l);

	return P;
}