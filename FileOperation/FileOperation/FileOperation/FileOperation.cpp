// FileOperation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<Windows.h>
#include<iostream>
#include<string.h>
using namespace std;
HANDLE hFile;//文件句柄
LPSTR filePath = ".\\test.txt";//文件路径 相对路径
HANDLE hMapFile;//文件映射句柄 使用paging file
LPSTR mappingName = "mappingObject";//映射对象名字
char* message;//数据消息

//创建文件操作
DWORD CreateFileOp(){
	hFile = CreateFileA(filePath, GENERIC_WRITE | GENERIC_READ, 0,
	NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile failed!");
		CloseHandle(hFile);
		return -1;
	}
	else{
		printf("创建成功！\n");
	}
}
//读文件
DWORD ReadFileOp(HANDLE hfile){
	//设置文件指针到文件开始
	SetFilePointer(hfile,0,0,FILE_BEGIN);
	//文件大小
	LARGE_INTEGER fsz;
	//实际读取的字节数
	DWORD readSz;
	//总共读出的字节数
	LONGLONG totalRead = 0;

	char dataBuf[32];//缓冲区

	if(!GetFileSizeEx(hfile , &fsz))
    {
        printf("获取文件大小失败： %d",GetLastError());
    }
    else
    {
		printf("文件大小为： %lld\n",fsz.QuadPart);
    }
    //循环读取并打印内容
    while(TRUE)
    {
        DWORD i;
		
        if(!ReadFile(hfile,//读取文件句柄
			dataBuf,  //存储读取文件内容
            32,             //读取的大小(字节)
            &readSz,       //实际读取的大小
            NULL))         //不使用Overlapped
        {
            printf("读取文件错误： %d\n",GetLastError());
            break;
        }
        printf("读取了%d字节，文件内容是： ",readSz);
		//打印缓冲区内容
        for(i=0;i<readSz;i++)
        {
            printf("%c",dataBuf[i]);
        }
        printf("\n");
        totalRead += readSz;
		//如果已经读取到的字节数等于文件大小，说明文件读取完毕

        if(totalRead==fsz.QuadPart)
        {
            printf("读取文件结束\n");
            break;
        }
    }
	return 0;
}
//写文件
DWORD WriteFileOp(HANDLE hfile , LPVOID lpData, DWORD writeSz){
	DWORD wsz;
	//设置文件指针到文件尾部
	SetFilePointer(hfile,0,0,FILE_END);

    //将数据写入文件
    if(!WriteFile(hfile,lpData,writeSz,&wsz,NULL))
    {
        printf("写文件失败： %d\n",GetLastError());
    }
    else
    {
        printf("写文件成功，写入%d字节。\n",wsz);
    }

	return 0;
}
//关闭文件
DWORD CloseFileOp(HANDLE hfile){
	return CloseHandle(hfile);
}
//删除文件
DWORD DeleteFileOp(LPSTR filePath){

	if (DeleteFileA(filePath)) {
		printf("删除文件成功！\n");
		return 1;
	}
	else {
		printf("删除文件失败！\n");
		return 0;
	}
	
}

DWORD ReadAsync()
{
	HANDLE hFile = CreateFileA(".\\readAsync.txt", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);//打开前面创建的文件
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile failed!");
		CloseHandle(hFile);
		return 0;
	}
	char dataBuf[32];//缓冲区
	OVERLAPPED ol = { 0 };
	ol.Offset = 0;
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	ol.hEvent = hEvent;//传递一个事件对象。
	DWORD read ;
	ReadFile(hFile, dataBuf, 32, &read, &ol);//提交一个异步读操作

	
	
	if (GetLastError() == ERROR_IO_PENDING)
	{
		WaitForSingleObject(ol.hEvent, INFINITE);//等待事件对象被触发。
		//printf("read = %d\n", read);//奇怪这里为什么read=0
		read = ol.InternalHigh;
		dataBuf[read] = '\0';//不然没有结尾会出现烫烫
		printf("%s\n", dataBuf);
	}
	else {
		printf("读出字节%d\n", sizeof(dataBuf));
	}

	CloseHandle(hFile);
	CloseHandle(hEvent);
	return 1;
		 
}

DWORD  DataCom() {
	printf("用文件映射实现进程间数据通信：\n");
	//建立文件映射对象，返回句柄
	hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 256, mappingName);

	if (hMapFile == NULL) {
		printf("创建文件映射失败！");
		return -1;
	}
	//把文件映射对象 映射到进程的内存中
	message = (LPSTR)MapViewOfFile(hMapFile, FILE_MAP_WRITE, 0, 0, 256);
	if (message == NULL) {
		printf("将文件映射内核对象映射到调用进程的地址空间失败！");
		return -1;
	}
	while (true) {
		printf("发送消息：");
		cin >> message;
		if (strcmp(message,"end") == 0) break;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	char ch;
	LPSTR toWrite = "OK , let's go !";//待写入的内容
	CreateFileOp();//创建文件
	WriteFileOp(hFile, toWrite, lstrlenA(toWrite));//写入内容
	ReadFileOp(hFile);//读文件
	CloseFileOp(hFile);//关闭文件
	printf("是否删除文件？y/n\n");
	scanf_s("%c",&ch);
	if(ch == 'y')
		DeleteFileOp(filePath);//删除文件

	DataCom();//利用文件映射实现进程间数据通信
	printf("异步读取文件：\n");
	ReadAsync();//异步读取文件

	return 0;
}
