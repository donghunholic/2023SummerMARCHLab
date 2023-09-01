/*
TwinCAT���� ADS ����� ����ϴ� ��������Դϴ�. �⺻���� ���ڴ� TwinCAT Ʃ�丮�� �ִ� �Ͱ� ����������, 
�Ϲ����� CPP Project������ Thread�� ������� �����Ƿ� �ش� �κ��� �����Ͽ����ϴ�.
*/

#include "C:\TwinCAT\AdsApi\TcAdsDll\Include\TcAdsDef.h"
#include "C:\TwinCAT\AdsApi\TcAdsDll\Include\TcAdsAPI.h"
#include "ADSDataStructure.h"

/*
ADS Read�� �̷������ �Լ��Դϴ�. �Ű������� ���� ������ �ϴ� AdsRead ����ü�� �ּҸ� ������ �ش� ����ü�� ADS�� ����� Read ���� ������ �Ǿ��ֽ��ϴ�.
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

