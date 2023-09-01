#include "LuMatchFdn.h"

/*----------DEFINE CONSTANTS----------*/
#define PI 3.14159265358979
#define MRT 635.800				//Motor Rated Torque
#define T_S 0.001
#define CUTOFF_VEL_HZ 2
#define INC_TO_RAD 0.00314159265				//  1 rotation == 2000inc, 2pi / 2000
#define RAD_TO_INC 318.309886184				//  2000 / 2pi
#define RPM_TO_RADS 0.00243534314				//  for RPM, we should consider gear ratio. gear ratio == 43:1, 43 axis rotate == 1 encoder rotate; 2pi / (60 * 43)
#define RADS_TO_RPM 410.619753177
#define TQ_TO_MN 0.6358				//   torque actual to Nm;   
#define MN_TO_TQ 1.57282164203
#define RAD_TO_DEG 57.2957795131
#define DEG_TO_RAD 0.01745329251

#define RST_CYCLE 100
#define MAV_WINSIZE 50

//TODO: 일단 복붙해서 사용하는 값. 나중에 한 번 확인해 볼 것.
#define TORQUE_SENSOR_REAL_TO_NM 0.1251564456 // From calibration data (w/ HBM torque sensor) (1.0 / 7.99)
#define AI_TO_V 0.0003051850948 // (10 / (2^15 - 1))

//MATCH Device 사용을 위해 필요한 값.

#define DEVICE_ID			(0)		/*!< Set device(MATCH Cradle) ID. Default value is 0.*/
#define SEN_NUM				(8)		/*!< Total sensor number. Default value is 8.*/
#define SAMPLE_PERIOD_MS	(1)		/*!< Data sampling time from MATCH.\n minimum value is 1.\n Unit: milisencond.*/
#define SAMPLE_FREQ			(1000.0f / SAMPLE_PERIOD_MS) /*!< Data sampling frequency.\n Unit: Hz*/
#define BUF_SIZE			(100)	/*!< Sensor data buffer for use in the LuFoundation Library.*/
#define	FLT_WIN_SIZE		(10)	/*!< Low pass filter window size for sEMG signal processing.*/
#define SCREEN_REFRESH_MS	(1)		/*!< Screen refresh rate.\n Unit: milisecond*/

// functions that will be used to calculate derivatives / filtering noises.

float LowPassFilter(float Input_prev, float Input_cur, float Output_prev, float w_c, float TS);
float FilteredDerivative(float Input_prev, float Input_cur, float Output_prev, float w_c, float TS);
float FilteredDerivativeTwotimes(float Input_pprev, float Input_prev, float Input_cur, float Output_pprev, float Output_prev, float w_c, float TS);

int InitMatch();
int OpenMatch();
int CloseMatch();