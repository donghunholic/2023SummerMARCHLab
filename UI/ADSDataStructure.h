#pragma once

// TC INT == SHORT
// TC DINT == LONG/INT
// TC BOOL == bool

#pragma pack(push, 1)
struct AdsRead {
	float PosInc_L = 0;
	float PosInc_R = 0;
	//float PosOff_L = 0;
	//float PosOff_R = 0;
	float VelAct_L = 0;
	float VelAct_R = 0;
	float Torque_raw = 0;
	float sEMG_Ch1 = 0;
	float sEMG_Ch2 = 0;
};

struct AdsWrite {
	USHORT Controlword_L = 0x06;
	USHORT Controlword_R = 0x06;
	//bool Pos_rst = false;
	SHORT desired_torque_L = 0;
	SHORT desired_torque_R = 0;
};
#pragma pack(pop)