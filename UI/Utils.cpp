#include "Utils.h"


float LowPassFilter(float Input_prev,
	float Input_cur,
	float Output_prev,
	float w_c,
	float TS) {
	float A_1 = (TS * w_c) / (float)(TS * w_c + 2.0);
	float A_2 = (TS * w_c) / (float)(TS * w_c + 2.0);

	float B_1 = -(TS * w_c - 2.0) / (float)(TS * w_c + 2.0);

	float LPF_val = (A_1 * Input_cur + A_2 * Input_prev)
		+ (B_1 * Output_prev);

	return LPF_val;
}

float FilteredDerivative(float Input_prev,
	float Input_cur,
	float Output_prev,
	float w_c,
	float TS) {
	float A_1 = (2 * w_c) / (TS * w_c + 2);
	float A_2 = -(2 * w_c) / (TS * w_c + 2);

	float B_1 = -(TS * w_c - 2) / (TS * w_c + 2);

	float FD_val = (A_1 * Input_cur + A_2 * Input_prev)
		+ (B_1 * Output_prev);

	return FD_val;
}

float FilteredDerivativeTwotimes(float Input_pprev,
	float Input_prev,
	float Input_cur,
	float Output_pprev,
	float Output_prev,
	float w_c,
	float TS) {

	float FDD_val = (4 * w_c * Input_cur - 8 * w_c * Input_prev + 4 * w_c * Input_pprev - 2 * w_c * TS * TS * Output_prev
		+ (2 * TS - w_c * TS * TS) * Output_pprev) / (2 * TS + w_c * TS * TS);
	return FDD_val;
}

int InitMatch() {
	int SenNum = 0;
	int rtn = 0;

	SenNum = LufScanSensor();
	if (SenNum) {
		rtn = LufInitDev(DEVICE_ID, SEN_NUM, SAMPLE_PERIOD_MS, BUF_SIZE, FLT_WIN_SIZE);
	}

	return rtn;
}

int OpenMatch() {
	int rtn;
	rtn = LufOpenDev(DEVICE_ID, SEN_NUM, SAMPLE_PERIOD_MS, BUF_SIZE, FLT_WIN_SIZE);

	return rtn;
}

int CloseMatch() {
	int rtn;

	rtn = LufCloseDev(DEVICE_ID);
	return rtn;
}