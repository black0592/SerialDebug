typedef struct _SERIALCALLBACK{
	BYTE	*lpBuffer;				//Ҫ���͵����ݻ��棬����callback�����з����ڴ棬����Ҫ����ڴ�
	DWORD	dwLength;				//Ҫ���͵����ݳ���
	BOOL	bRepeat;				//�Ƿ��ڷ�����ɺ��ٴε���
}SERIALCALLBACK, *LPSERIALCALLBACK;

typedef struct _SERIALUSERDATA{
	DWORD dwlength;					//Ϊ����USERDATA�ṹ�峤��
	BYTE  data[1020];				//�û��Զ�������
}SERIALUSERDATA, *LPSERIALUSERDATA;

typedef bool (WINAPI *DllTestProc)(void);
typedef void (WINAPI *SerialCallbackProc)(LPSERIALCALLBACK lpSerialCallback,LPSERIALUSERDATA lpSerialUserData);