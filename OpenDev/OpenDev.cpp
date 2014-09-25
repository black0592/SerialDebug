// OpenDev.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "OpenDev.h"

//�˺�����������ͨѶ�ã�����Ҫ����
extern "C" _declspec (dllexport) bool DllTest(void){
	return true;
}


extern "C" _declspec (dllexport) void SerialCallback(LPSERIALCALLBACK lpSerialCallback ,
													 LPSERIALUSERDATA lpSerialUserData )
{
	
	//UserDataΪ�û�Ҫ�������ݣ���������ݻ����´λص���ʱ��ԭ�����أ���಻����1024�ֽڣ��������ڱ���״̬��Ϣ�����ݸ�ʽ�������ж��岻���뽫��������Ϊ0��
	//UserData���ݵ�Ĭ��ֵΪ0
	lpSerialUserData->dwlength = 0;

	//ΪҪ���͵����ݷ����ڴ�
	BYTE *toSend = new BYTE[1024];
	
	//��дҪ���͵�����
	int i;
	for (i = 0; i < 1024; i++){
		toSend[i] = i%256;
	}

	//��Ҫ���͵�������д�����ݰ���
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
