// OpenDev.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "OpenDev.h"

//此函数不做数据通讯用，但需要保留
extern "C" _declspec (dllexport) bool DllTest(void){
	return true;
}


extern "C" _declspec (dllexport) void SerialCallback(LPSERIALCALLBACK lpSerialCallback ,
													 LPSERIALUSERDATA lpSerialUserData )
{
	
	//UserData为用户要保存数据，这里的数据会在下次回调的时候原样返回，最多不超过1024字节，可以用于保存状态信息，数据格式可以自行定义不用请将长度设置为0，
	//UserData数据的默认值为0
	lpSerialUserData->dwlength = 0;

	//为要发送的数据分配内存
	BYTE *toSend = new BYTE[1024];
	
	//填写要发送的数据
	int i;
	for (i = 0; i < 1024; i++){
		toSend[i] = i%256;
	}

	//将要发送的数据填写到数据包中
	lpSerialCallback->lpBuffer = toSend;
	lpSerialCallback->dwLength = 1024;
	if (lpSerialUserData->i<2){
		lpSerialCallback->bRepeat = true;
		lpSerialUserData->i++;
	}
	else{
		lpSerialCallback->bRepeat = false;
	}
	

}
