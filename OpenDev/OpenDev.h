typedef struct _SERIALCALLBACK{
	BYTE	*lpBuffer;				//Ҫ���͵����ݻ��棬����callback�����з����ڴ棬����Ҫ����ڴ�
	DWORD	dwLength;				//Ҫ���͵����ݳ���
	BOOL	bRepeat;				//�Ƿ��ڷ�����ɺ��ٴε���
}SERIALCALLBACK, *LPSERIALCALLBACK;

typedef struct _SERIALUSERDATA{
	DWORD dwlength;					//Ϊ����USERDATA�ṹ�峤��
	int	  i;
}SERIALUSERDATA, *LPSERIALUSERDATA;