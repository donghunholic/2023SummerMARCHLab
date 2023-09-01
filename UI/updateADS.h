/*
TwinCAT과의 ADS 통신을 담당하는 헤더파일입니다. 기본적인 골자는 TwinCAT 튜토리얼에 있는 것과 동일하지만, 
일반적인 CPP Project에서는 Thread를 사용하지 않으므로 해당 부분을 개선하였습니다.
*/

#include "C:\TwinCAT\AdsApi\TcAdsDll\Include\TcAdsDef.h"
#include "C:\TwinCAT\AdsApi\TcAdsDll\Include\TcAdsAPI.h"
#include "ADSDataStructure.h"

/*
ADS Read가 이루어지는 함수입니다. 매개변수로 값을 적고자 하는 AdsRead 구조체의 주소를 넣으면 해당 구조체에 ADS로 연결된 Read 값을 적도록 되어있습니다.
*/
void ReadADS(AdsRead* data) {
	long nErr = 0;
	long nPort = AdsPortOpen();
	AmsAddr Addr;
	nErr = AdsGetLocalAddress(&Addr);
	(&Addr)->port = 350;

	unsigned long AreadSize = sizeof(*data);
	nErr = AdsSyncReadReq(&Addr, 0x1010010, 0x83000000, AreadSize, data);
}


void WriteADS(AdsWrite* data) {
	long nErr = 0;
	long nPort = AdsPortOpen();
	AmsAddr Addr;
	nErr = AdsGetLocalAddress(&Addr);
	(&Addr)->port = 350;

	unsigned long AWriteSize = sizeof(*data);
	//std::cout << data->Controlword_L << " " << data->desired_torque_L << std::endl;
	nErr = AdsSyncWriteReq(&Addr, 0x1010010, 0x84000000, AWriteSize, data);
	if (nErr != 0) {
		std::cout << "Write Error" << std::endl;
	}
}

