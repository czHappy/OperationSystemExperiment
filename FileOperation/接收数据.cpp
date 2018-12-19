
#include<Windows.h>
#include<iostream>
using namespace std;

HANDLE hMapFile;//文件映射句柄 使用paging file
LPSTR mappingName = "mappingObject";//映射对象名字
LPSTR message;//数据消息


int main(){
	hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 256, mappingName);
	if (hMapFile == NULL) {
		printf("创建文件映射失败！");
		return -1;
	}
	//把文件映射对象 映射到进程的内存中
	message = (LPSTR)MapViewOfFile(hMapFile,FILE_MAP_WRITE,0,0,256);
	if (message == NULL) {
		printf("将文件映射内核对象映射到调用进程的地址空间失败！");
		return -1;
	}
	while (true) {
		printf("接收消息：");
		printf("%s\n", message);
		getchar();
	
	}

	return 0;
} 
