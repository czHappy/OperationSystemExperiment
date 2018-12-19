// FileOperation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<Windows.h>
#include<iostream>
#include<string.h>
using namespace std;
HANDLE hFile;//�ļ����
LPSTR filePath = ".\\test.txt";//�ļ�·�� ���·��
HANDLE hMapFile;//�ļ�ӳ���� ʹ��paging file
LPSTR mappingName = "mappingObject";//ӳ���������
char* message;//������Ϣ

//�����ļ�����
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
		printf("�����ɹ���\n");
	}
}
//���ļ�
DWORD ReadFileOp(HANDLE hfile){
	//�����ļ�ָ�뵽�ļ���ʼ
	SetFilePointer(hfile,0,0,FILE_BEGIN);
	//�ļ���С
	LARGE_INTEGER fsz;
	//ʵ�ʶ�ȡ���ֽ���
	DWORD readSz;
	//�ܹ��������ֽ���
	LONGLONG totalRead = 0;

	char dataBuf[32];//������

	if(!GetFileSizeEx(hfile , &fsz))
    {
        printf("��ȡ�ļ���Сʧ�ܣ� %d",GetLastError());
    }
    else
    {
		printf("�ļ���СΪ�� %lld\n",fsz.QuadPart);
    }
    //ѭ����ȡ����ӡ����
    while(TRUE)
    {
        DWORD i;
		
        if(!ReadFile(hfile,//��ȡ�ļ����
			dataBuf,  //�洢��ȡ�ļ�����
            32,             //��ȡ�Ĵ�С(�ֽ�)
            &readSz,       //ʵ�ʶ�ȡ�Ĵ�С
            NULL))         //��ʹ��Overlapped
        {
            printf("��ȡ�ļ����� %d\n",GetLastError());
            break;
        }
        printf("��ȡ��%d�ֽڣ��ļ������ǣ� ",readSz);
		//��ӡ����������
        for(i=0;i<readSz;i++)
        {
            printf("%c",dataBuf[i]);
        }
        printf("\n");
        totalRead += readSz;
		//����Ѿ���ȡ�����ֽ��������ļ���С��˵���ļ���ȡ���

        if(totalRead==fsz.QuadPart)
        {
            printf("��ȡ�ļ�����\n");
            break;
        }
    }
	return 0;
}
//д�ļ�
DWORD WriteFileOp(HANDLE hfile , LPVOID lpData, DWORD writeSz){
	DWORD wsz;
	//�����ļ�ָ�뵽�ļ�β��
	SetFilePointer(hfile,0,0,FILE_END);

    //������д���ļ�
    if(!WriteFile(hfile,lpData,writeSz,&wsz,NULL))
    {
        printf("д�ļ�ʧ�ܣ� %d\n",GetLastError());
    }
    else
    {
        printf("д�ļ��ɹ���д��%d�ֽڡ�\n",wsz);
    }

	return 0;
}
//�ر��ļ�
DWORD CloseFileOp(HANDLE hfile){
	return CloseHandle(hfile);
}
//ɾ���ļ�
DWORD DeleteFileOp(LPSTR filePath){

	if (DeleteFileA(filePath)) {
		printf("ɾ���ļ��ɹ���\n");
		return 1;
	}
	else {
		printf("ɾ���ļ�ʧ�ܣ�\n");
		return 0;
	}
	
}

DWORD ReadAsync()
{
	HANDLE hFile = CreateFileA(".\\readAsync.txt", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);//��ǰ�洴�����ļ�
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile failed!");
		CloseHandle(hFile);
		return 0;
	}
	char dataBuf[32];//������
	OVERLAPPED ol = { 0 };
	ol.Offset = 0;
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	ol.hEvent = hEvent;//����һ���¼�����
	DWORD read ;
	ReadFile(hFile, dataBuf, 32, &read, &ol);//�ύһ���첽������

	
	
	if (GetLastError() == ERROR_IO_PENDING)
	{
		WaitForSingleObject(ol.hEvent, INFINITE);//�ȴ��¼����󱻴�����
		//printf("read = %d\n", read);//�������Ϊʲôread=0
		read = ol.InternalHigh;
		dataBuf[read] = '\0';//��Ȼû�н�β���������
		printf("%s\n", dataBuf);
	}
	else {
		printf("�����ֽ�%d\n", sizeof(dataBuf));
	}

	CloseHandle(hFile);
	CloseHandle(hEvent);
	return 1;
		 
}

DWORD  DataCom() {
	printf("���ļ�ӳ��ʵ�ֽ��̼�����ͨ�ţ�\n");
	//�����ļ�ӳ����󣬷��ؾ��
	hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 256, mappingName);

	if (hMapFile == NULL) {
		printf("�����ļ�ӳ��ʧ�ܣ�");
		return -1;
	}
	//���ļ�ӳ����� ӳ�䵽���̵��ڴ���
	message = (LPSTR)MapViewOfFile(hMapFile, FILE_MAP_WRITE, 0, 0, 256);
	if (message == NULL) {
		printf("���ļ�ӳ���ں˶���ӳ�䵽���ý��̵ĵ�ַ�ռ�ʧ�ܣ�");
		return -1;
	}
	while (true) {
		printf("������Ϣ��");
		cin >> message;
		if (strcmp(message,"end") == 0) break;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	char ch;
	LPSTR toWrite = "OK , let's go !";//��д�������
	CreateFileOp();//�����ļ�
	WriteFileOp(hFile, toWrite, lstrlenA(toWrite));//д������
	ReadFileOp(hFile);//���ļ�
	CloseFileOp(hFile);//�ر��ļ�
	printf("�Ƿ�ɾ���ļ���y/n\n");
	scanf_s("%c",&ch);
	if(ch == 'y')
		DeleteFileOp(filePath);//ɾ���ļ�

	DataCom();//�����ļ�ӳ��ʵ�ֽ��̼�����ͨ��
	printf("�첽��ȡ�ļ���\n");
	ReadAsync();//�첽��ȡ�ļ�

	return 0;
}
