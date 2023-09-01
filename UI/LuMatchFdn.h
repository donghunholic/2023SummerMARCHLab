#ifndef _LU_MATCH_FDN_H_
#define _LU_MATCH_FDN_H_

/*!
\file		Lu
Fdn.h
\brief		Dll Lib. header for LogonU MATCH Foundation
\author		Dong-hyun Lee, dh_wizard@logonu.com
\date		2017. 09. 27
\version	1.0.0
\copyright	(c) LogonU Corp. All rights reserved.
*/

#pragma once
#include <Windows.h>


#ifdef LUMATCH_EXPORTS
#define LUMATCH_API __declspec(dllexport)
#else
#ifdef NDEBUG
#pragma comment(lib, "LuMatchFdn.lib")			
#else
#pragma comment(lib, "LuMatchFdn_d.lib")
#endif
#define LUMATCH_API __declspec(dllimport)
#endif // LUFEAGM_EXPORTS

/*! Error Return */
enum LuFdnErr
{
	eErrNotInitialized = -1,
	eErrAlreadyConnected = -2,
	eErrCantConnect = -3,
	eErrCantMakeThread = -4,
	eErrCmdSendFail = -5,
	eErrParamFault = -6

};

/*! Option Data Type */
enum LuOptDataType
{
	eTypeDoNotChange = 0,
	eTypeAccel,
	eTypeGyro,
	eTypeMagnet,
	eTypeLed
};

/*! sEmg Amp Gain */
enum LuEmgAmpGain
{
	eGain10 = 1,
	eGain20,
	eGain50,
	eGain100
};

/*! Motion Algorithm */
enum LuMotionAlgo
{
	eAlgoAGM = 1,
	eAlgoAG
};

/*! LED and Vibration */
enum LuLedVib
{
	eLedRed = 1,
	eLedBlue = 2,
	eLedGreen = 4
};

/*! Axis Index */
enum LuAxis
{
	eAxisX = 0,
	eAxisY,
	eAxisZ
};

#ifdef __cplusplus
extern "C" {
#endif
	// Connection
	LUMATCH_API const int LufScanSensor();	//!< Scaning connected MATCH devices.
	LUMATCH_API int LufInitDev(const int devIdx, const int senNum, const int smplPeriod_ms, const int bufSize, const int filterWinSz);	//!< Initializes the MATCH device and library.
	LUMATCH_API int LufOpenDev(const int devIdx, const int senNum, const int smplPeriod_ms, const int bufSize, const int filterWinSz);	//!< Opens the MATCH device and starts acquiring sensor data.
	LUMATCH_API int LufCloseDev(const int devIdx);	//!< Closes the MATCH device.
	LUMATCH_API int LufExitAllDev();				//!< Finalize the LuFoundation Library.
	LUMATCH_API int LufIsDevInit(const int devIdx);	//!< Indicates whether the specified MATCH device is initialization.

	// Data Save
	LUMATCH_API int LufSetStartSaveData(const int devIdx, LPCWSTR lpFileName);	//!< Starts saving data of MATCH sensors.
	LUMATCH_API int LufSetStopSaveData(const int devIdx);						//!< Stops saving data of MATCH sensors.

	// Device Port In/Out
	LUMATCH_API int LufSetMasterTriggerStart(const int devIdx);	//!< Generates an impulse signal at the start pin of the Trigger IN/OUT port.
	LUMATCH_API int LufSetMasterTriggerStop(const int devIdx);	//!< Generates an impulse signal at the stop pin of the Trigger IN/OUT port.

	// Sensor Param Setting
	LUMATCH_API int LufSetFactoryParamLoad(const int devIdx, const int senIdx);			//!< Loads factory setting.
	LUMATCH_API int LufSetSlaveID(const int devIdx, const int senIdx, const int newId);	//!< Sets the MATCH sensor ID.

	// Sensor Calibration
	LUMATCH_API int LufSetEmgAvgMaxCnt(const int devIdx, const int cnt);		//!< Sets the maximum number required to average sEMG signals of MATCH sensors.
	LUMATCH_API int LufSetEmgFilterWinSz(const int devIdx, const int winSize);	//!< Sets the filter window size for sEMG signals.
	LUMATCH_API int LufSetCalibEmgOffset(const int devIdx);						//!< Offset the raw sEMG signal bais to zero.
	LUMATCH_API int LufSetCalibInertiaBias(const int devIdx, const int senIdx);	//!< Offset the accelerometer and gyroscope bias.
	LUMATCH_API int LufSetMagnetCalibStart(const int devIdx, const int senIdx);	//!< Starts the a magnetometer calibration of a MATCH 

	LUMATCH_API int LufSetMagnetCalibStop(const int devIdx, const int senIdx);	//!< Stops the a magnetometer calibration of a MATCH sensor.

	// Sensor Control : Unified Command
	LUMATCH_API int LufSetPowerOff(const int devIdx, const int senIdx);						//!< Power off the MATCH sensor.
	LUMATCH_API int LufSetEmgAmpGain(const int devIdx, const int senIdx, const int gain);	//!< Sets the sEMG signal Amplifier gain.
	LUMATCH_API int LufSetRLedVib(const int devIdx, const int senIdx, const int ledOn, const int vibTime);	//!< Turns Red LED and Vibration on or off.
	LUMATCH_API int LufSetGLedVib(const int devIdx, const int senIdx, const int ledOn, const int vibTime);	//!< Turns Green LED and Vibration on or off.
	LUMATCH_API int LufSetBLedVib(const int devIdx, const int senIdx, const int ledOn, const int vibTime);	//!< Turns Blue LED and Vibration on or off.
	LUMATCH_API int LufSetLedsVib(const int devIdx, const int senIdx, const int ledsOn, const int vibTime);	//!< Turns LEDs and Vibration on or off.
	LUMATCH_API int LufSetVibration(const int devIdx, const int senIdx, const int vibTime);					//!< Turns on Vibration.
	LUMATCH_API int LufSetMotionAlgo(const int devIdx, const int senIdx, const int algo);					//!< Sets the algorithm for motion sensing.
	LUMATCH_API int LufSetOptionType(const int devIdx, const int senIdx, const int opt1, const int opt2);	//!< Sets the data type of option space.

	// Sensor Control : Divided Command
	LUMATCH_API int LufSetDivPowerOff(const int devIdx, const int* pTurnOff, const int turnOffSz_byte);		//!< Power off the MATCH sensor.
	LUMATCH_API int LufSetDivEmgAmpGain(const int devIdx, const int* pGain, const int gainSz_byte);			//!< Sets the sEMG signal Amplifier gain.
	LUMATCH_API int LufSetDivRLedVib(const int devIdx, const int* pLedOn, const int ledOnSz_byte, const int* pVibTime = NULL, const int vibTimeSz_byte = 0);	//!< Turns Red LED and Vibration on or off.
	LUMATCH_API int LufSetDivGLedVib(const int devIdx, const int* pLedOn, const int ledOnSz_byte, const int* pVibTime = NULL, const int vibTimeSz_byte = 0);	//!< Turns Green LED and Vibration on or off.
	LUMATCH_API int LufSetDivBLedVib(const int devIdx, const int* pLedOn, const int ledOnSz_byte, const int* pVibTime = NULL, const int vibTimeSz_byte = 0);	//!< Turns Blue LED and Vibration on or off.
	LUMATCH_API int LufSetDivLedsVib(const int devIdx, const int* pLedsOn, const int ledsOnSz_byte, const int* pVibTime = NULL, const int vibTimeSz_byte = 0);	//!< Turns LEDs and Vibration on or off.
	LUMATCH_API int LufSetDivVibration(const int devIdx, const int* pVibTime, const int vibTimeSz_byte);	//!< Turns on Vibration.
	LUMATCH_API int LufSetDivMotionAlgo(const int devIdx, const int* pAlgo, const int algoSz_byte);		//!< Sets the algorithm for motion sensing.
	LUMATCH_API int LufSetDivOptionType(const int devIdx, const int* pOpt1, const int opt1Sz_byte, const int* pOpt2, const int opt2Sz_byte); //!< Sets the data type of option space.

	// Gets Device Information
	LUMATCH_API const wchar_t* LufGetVendorString(const int devIndex);	//!< Gets the vendor name of a MATCH device.
	LUMATCH_API const wchar_t* LufGetProductString(const int devIndex);	//!< Gets the product name of a MATCH device.
	LUMATCH_API const wchar_t* LufGetDevSerialNum(const int devIndex);	//!< Gets the serial number of a MATCH device.
	LUMATCH_API const int LufGetSamplePeriod_ms(const int devIdx);		//!< Gets the current sampling time of a MATCH device.
	LUMATCH_API const int LufGetMasterTriggerIn(const int devIdx);		//!< Gets the Trigger IN state from the start or stop pin of the Trigger IN/OUT port.

	// Gets sEMG Filter Configuration
	LUMATCH_API const int LufGetEmgAvgMaxCnt(const int devIdx);		//!< Gets the maximum number required to average sEMG signals.
	LUMATCH_API const int LufGetEmgFilterWinSz(const int devIdx);	//!< Gets the window size of filter for sEMG signal.

	// Gets Internal Buffer Size
	LUMATCH_API const int LufGetInternalBufSize(const int devIdx);	//!< Gets the size of the internal buffer inside the library
	LUMATCH_API const int LufGetTimeBufSize(const int devIdx);		//!< Gets the size of the time buffer inside the library
	LUMATCH_API const int LufGetEmgBufSize(const int devIdx);		//!< Gets the size of the sEMG buffer inside the library
	LUMATCH_API const int LufGetAccelBufSize(const int devIdx);		//!< Gets the size of the accelerometers buffer inside the library.
	LUMATCH_API const int LufGetGyroBufSize(const int devIdx);		//!< Gets the size of the gyroscopes buffer inside the library.
	LUMATCH_API const int LufGetMagnetBufSize(const int devIdx);	//!< Gets the size of the magnetometers buffer inside the library.
	LUMATCH_API const int LufGetEulerBufSize(const int devIdx);		//!< Gets the size of the euler angles buffer inside the library.

	// Gets sensor data by copying the value
	LUMATCH_API int LufGetEmg_V(const int devIdx, const int senIdx, float* const pOutData, const int outDataSz_byte);		//!< Gets the sEMG signal values including raw, offset and filterd.
	LUMATCH_API int LufGetAccel_g(const int devIdx, const int senIdx, float* const pOutData, const int outDataSz_byte);		//!< Gets the accelerometer data.
	LUMATCH_API int LufGetGyro_dps(const int devIdx, const int senIdx, float* const pOutData, const int outDataSz_byte);	//!< Gets the gyroscope data.
	LUMATCH_API int LufGetMagnet_uT(const int devIdx, const int senIdx, float* const pOutData, const int outDataSz_byte);	//!< Gets the magnetometer data.
	LUMATCH_API int LufGetEuler_deg(const int devIdx, const int senIdx, float* const pOutData, const int outDataSz_byte);	//!< Gets the euler angles data.
	LUMATCH_API int LufGetLedStat(const int devIdx, const int senIdx, int* const pOutData);		//!< Gets the Led Status data.
	LUMATCH_API int LufGetBattery_pc(const int devIdx, const int senIdx, int* const pOutData);	//!< Gets the battery data.
	LUMATCH_API int LufGetEmgAmpGain(const int devIdx, const int senIdx, int* const pOutData);	//!< Gets the sEMG amplifier gain.
	LUMATCH_API int LufGetOpt1Type(const int devIdx, const int senIdx, int* const pOutData);		//!< Gets the data type of option1 space.
	LUMATCH_API int LufGetOpt2Type(const int devIdx, const int senIdx, int* const pOutData);		//!< Gets the data type of option2 space.
	LUMATCH_API int LufGetAlgoType(const int devIdx, const int senIdx, int* const pOutData);		//!< Gets the type of motion algorithm.

	// Gets a one axis or single sensor data by copying the value
	LUMATCH_API int LufGetRawEmg_V(const int devIdx, const int senIdx, float* const pOutData);		//!< Gets the raw sEMG data.
	LUMATCH_API int LufGetOffsetEmg_V(const int devIdx, const int senIdx, float* const pOutData);	//!< Gets the absolute sEMG data of the bias offset.
	LUMATCH_API int LufGetFilterEmg_V(const int devIdx, const int senIdx, float* const pOutData);	//!< Gets the filtered sEMG data.
	LUMATCH_API int LufGetAccelAxis_g(const int devIdx, const int senIdx, const int axis, float* const pOutData);	//!< Gets the specified axis accelerometer data.
	LUMATCH_API int LufGetGyroAxis_dps(const int devIdx, const int senIdx, const int axis, float* const pOutData);	//!< Gets the specified axis gyroscope data.
	LUMATCH_API int LufGetMagnetAxis_uT(const int devIdx, const int senIdx, const int axis, float* const pOutData);	//!< Gets the specified axis magnetometer data.
	LUMATCH_API int LufGetEulerAxis_deg(const int devIdx, const int senIdx, const int axis, float* const pOutData);	//!< Gets the specified axis euler angles data.

	// Gets all Sensor Data by copying the values
	LUMATCH_API int LufGetAllRawEmg_V(const int devIdx, float* const pOutData, const int outDataSz_byte);	//!< Gets the raw sEMG data for all sensors.
	LUMATCH_API int LufGetAllOffsetEmg_V(const int devIdx, float* const pOutData, const int outDataSz_byte);	//!< Gets the absolute sEMG data of the bias offset data for all sensors.
	LUMATCH_API int LufGetAllFilterEmg_V(const int devIdx, float* const pOutData, const int outDataSz_byte);	//!< Gets the filtered sEMG data for all sensors.
	LUMATCH_API int LufGetAllEmg_V(const int devIdx, float* const pOutData, const int outDataSz_byte);		//!< Gets the sEMG data including raw, offset and filted data for all sensors.
	LUMATCH_API int LufGetAllAccel_g(const int devIdx, float* const pOutData, const int outDataSz_byte);		//!< Gets the accelerometer data for all sensors.
	LUMATCH_API int LufGetAllGyro_dps(const int devIdx, float* const pOutData, const int outDataSz_byte);	//!< Gets the gyroscope data for all sensors.
	LUMATCH_API int LufGetAllMagnet_uT(const int devIdx, float* const pOutData, const int outDataSz_byte);	//!< Gets the magnetometer data for all sensors.
	LUMATCH_API int LufGetAllEuler_deg(const int devIdx, float* const pOutData, const int outDataSz_byte);	//!< Gets the euler angles data for all sensors.
	LUMATCH_API int LufGetAllLedStat(const int devIdx, int* const pOutData, const int outDataSz_byte);		//!< Gets the LED status data fo all sensors.
	LUMATCH_API int LufGetAllBattery_pc(const int devIdx, int* const pOutData, const int outDataSz_byte);	//!< Gets the battery data for all sensors.
	LUMATCH_API int LufGetAllEmgAmpGain(const int devIdx, int* const pOutData, const int outDataSz_byte);	//!< Gets the sEMG amplifier gain data for all sensors.
	LUMATCH_API int LufGetAllOpt1Type(const int devIdx, int* const pOutData, const int outDataSz_byte);		//!< Gets the data type of the option1 space for all sensors.
	LUMATCH_API int LufGetAllOpt2Type(const int devIdx, int* const pOutData, const int outDataSz_byte);		//!< Gets the data type of the option2 space for all sensors.
	LUMATCH_API int LufGetAllAlgoType(const int devIdx, int* const pOutData, const int outDataSz_byte);		//!< Gets the type of motion algorithm for all sensors.

	// Gets sensor buffer data by copying the value
	LUMATCH_API int LufGetTimeBuf(const int devIdx, float* const pOutData, const int outDataSz_byte);						//!< Gets the time buffer data.
	LUMATCH_API int LufGetEmgBuf_V(const int devIdx, const int senIdx, float* const pOutData, const int outDataSz_byte);		//!< Gets the sEMG buffer data.
	LUMATCH_API int LufGetAccelBuf_g(const int devIdx, const int senIdx, float* const pOutData, const int outDataSz_byte);	//!< Gets the accelerometer buffer data.
	LUMATCH_API int LufGetGyroBuf_dps(const int devIdx, const int senIdx, float* const pOutData, const int outDataSz_byte);	//!< Gets the gyroscope buffer data.
	LUMATCH_API int LufGetMagnetBuf_uT(const int devIdx, const int senIdx, float* const pOutData, const int outDataSz_byte);	//!< Gets the magnetometer buffer data.
	LUMATCH_API int LufGetEulerBuf_deg(const int devIdx, const int senIdx, float* const pOutData, const int outDataSz_byte);	//!< Gets the euler angles buffer data.

	// Gets a one axis or single sensor buffer data by copying the value
	LUMATCH_API int LufGetRawEmgBuf_V(const int devIdx, const int senIdx, float* const pOutData, const int outDataSz_byte);		//!< Gets the raw sEMG buffer data.
	LUMATCH_API int LufGetOffsetEmgBuf_V(const int devIdx, const int senIdx, float* const pOutData, const int outDataSz_byte);	//!< Gets the offset and absolute sEMG buffer data.
	LUMATCH_API int LufGetFilterEmgBuf_V(const int devIdx, const int senIdx, float* const pOutData, const int outDataSz_byte);	//!< Gets the filtered sEMG buffer data.
	LUMATCH_API int LufGetAccelAxisBuf_g(const int devIdx, const int senIdx, const int axis, float* const pOutData, const int outDataSz_byte);	//!< Gets the specified accelerometer buffer data.
	LUMATCH_API int LufGetGyroAxisBuf_dps(const int devIdx, const int senIdx, const int axis, float* const pOutData, const int outDataSz_byte);	//!< Gets the specified gyroscope buffer data.
	LUMATCH_API int LufGetMagnetAxisBuf_uT(const int devIdx, const int senIdx, const int axis, float* const pOutData, const int outDataSz_byte);	//!< Gets the specified magnetometer buffer data.
	LUMATCH_API int LufGetEulerAxisBuf_deg(const int devIdx, const int senIdx, const int axis, float* const pOutData, const int outDataSz_byte);	//!< Gets the specified euler angles buffer data.

	// Gets Sensor Data Address
	LUMATCH_API float* const LufGetAddrEmg_V(const int devIdx, const int senIdx);		//!< Returns a pointer to the sEMG data array including raw, offset and filtered data.
	LUMATCH_API float* const LufGetAddrAccel_g(const int devIdx, const int senIdx);		//!< Returns a pointer to the accelerometer data including x-, y- and z-axes.
	LUMATCH_API float* const LufGetAddrGyro_dps(const int devIdx, const int senIdx);	//!< Returns a pointer to the gyroscope data including x-, y- and z-axes.
	LUMATCH_API float* const LufGetAddrMagnet_uT(const int devIdx, const int senIdx);	//!< Returns a pointer to the magnetometer data including x-, y- and z-axes.
	LUMATCH_API float* const LufGetAddrEuler_deg(const int devIdx, const int senIdx);	//!< Returns a pointer to the euler angles data including x-, y- and z-axes.
	LUMATCH_API int* const LufGetAddrLedStat(const int devIdx, const int senIdx);		//!< Returns a pointer to the LED status data.
	LUMATCH_API int* const LufGetAddrBattery_pc(const int devIdx, const int senIdx);	//!< Returns a pointer to the battery data varibale.
	LUMATCH_API int* const LufGetAddrEmgAmpGain(const int devIdx, const int senIdx);	//!< Returns a pointer to the sEMG amp. gain data varibale.
	LUMATCH_API int* const LufGetAddrOpt1Type(const int devIdx, const int senIdx);	//!< Returns a pointer to the data-type variable of the option1 spaces.
	LUMATCH_API int* const LufGetAddrOpt2Type(const int devIdx, const int senIdx);	//!< Returns a pointer to the data-type variable of the option2 spaces.
	LUMATCH_API int* const LufGetAddrAlgoType(const int devIdx, const int senIdx);	//!< Returns a pointer to the type variable of the motion algorithm.

	// Gets Sensor Data Buffer Address
	LUMATCH_API float* const LufGetAddrTimeBuf(const int devIdx);							//!< Returns a pointer to the time buffer array.
	LUMATCH_API float* const LufGetAddrRawEmgBuf_V(const int devIdx, const int senIdx);		//!< Returns a pointer to the raw sEMG buffer array.
	LUMATCH_API float* const LufGetAddrOffsetEmgBuf_V(const int devIdx, const int senIdx);	//!< Returns a pointer to the offset and absolute sEMG buffer array.
	LUMATCH_API float* const LufGetAddrFilterEmgBuf_V(const int devIdx, const int senIdx);	//!< Returns a pointer to the filtered sEMG buffer array.
	LUMATCH_API float* const LufGetAddrAccelXBuf_g(const int devIdx, const int senIdx);		//!< Returns a pointer to the x-axis accelerometer buffer array.
	LUMATCH_API float* const LufGetAddrAccelYBuf_g(const int devIdx, const int senIdx);		//!< Returns a pointer to the y-axis accelerometer buffer array.
	LUMATCH_API float* const LufGetAddrAccelZBuf_g(const int devIdx, const int senIdx);		//!< Returns a pointer to the z-axis accelerometer buffer array.
	LUMATCH_API float* const LufGetAddrGyroXBuf_dps(const int devIdx, const int senIdx);	//!< Returns a pointer to the x-axis gyroscope buffer array.
	LUMATCH_API float* const LufGetAddrGyroYBuf_dps(const int devIdx, const int senIdx);	//!< Returns a pointer to the y-axis gyroscope buffer array.
	LUMATCH_API float* const LufGetAddrGyroZBuf_dps(const int devIdx, const int senIdx);	//!< Returns a pointer to the z-axis gyroscope buffer array.
	LUMATCH_API float* const LufGetAddrMagnetXBuf_uT(const int devIdx, const int senIdx);	//!< Returns a pointer to the x-axis magnetometer buffer array.
	LUMATCH_API float* const LufGetAddrMagnetYBuf_uT(const int devIdx, const int senIdx);	//!< Returns a pointer to the y-axis magnetometer buffer array.
	LUMATCH_API float* const LufGetAddrMagnetZBuf_uT(const int devIdx, const int senIdx);	//!< Returns a pointer to the z-axis magnetometer buffer array.
	LUMATCH_API float* const LufGetAddrEulerXBuf_deg(const int devIdx, const int senIdx);	//!< Returns a pointer to the x-axis euler angles buffer array.
	LUMATCH_API float* const LufGetAddrEulerYBuf_deg(const int devIdx, const int senIdx);	//!< Returns a pointer to the y-axis euler angles buffer array.
	LUMATCH_API float* const LufGetAddrEulerZBuf_deg(const int devIdx, const int senIdx);	//!< Returns a pointer to the z-axis euler angles buffer array.
	LUMATCH_API float* const LufGetAddrEmgBuf_V(const int devIdx, const int senIdx);		//!< Returns a pointer to the sEMG buffer array.
	LUMATCH_API float* const LufGetAddrAccelBuf_g(const int devIdx, const int senIdx);		//!< Returns a pointer to the accelerometer buffer array.
	LUMATCH_API float* const LufGetAddrGyroBuf_dps(const int devIdx, const int senIdx);		//!< Returns a pointer to the gyroscope buffer array.
	LUMATCH_API float* const LufGetAddrMagnetBuf_uT(const int devIdx, const int senIdx);	//!< Returns a pointer to the magnetometer buffer array.
	LUMATCH_API float* const LufGetAddrEulerBuf_deg(const int devIdx, const int senIdx);	//!< Returns a pointer to the euler angles buffer array.

#ifdef __cplusplus
}
#endif

#endif