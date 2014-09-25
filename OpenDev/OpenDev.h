typedef struct _SERIALCALLBACK{
	BYTE	*lpBuffer;				//要发送的数据缓存，需在callback函数中分配内存，但不要清空内存
	DWORD	dwLength;				//要发送的数据长度
	BOOL	bRepeat;				//是否在发送完成后再次调用
}SERIALCALLBACK, *LPSERIALCALLBACK;

typedef struct _SERIALUSERDATA{
	DWORD dwlength;					//为整个USERDATA结构体长度
	int	  i;
}SERIALUSERDATA, *LPSERIALUSERDATA;