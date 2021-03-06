// ProducConsum.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include "windows.h"
#include <conio.h>
#include <stdlib.h>
#include <fstream>
#include <io.h>
#include <string.h>
#include <stdio.h>
#include<iostream>
#define INTE_PER_SEC 100 
#define MAX_THREAD_NUM  10
#define WRITE 'W'
#define READ 'R'

using namespace  std;

struct ThreadInfo
{
	int    serial;
	char   entity;
	double delay;
	double persist;
};

int read_count = 0;
CRITICAL_SECTION sem_mutex;
HANDLE W;

void  ReadWrite(char *file);
void  Thread_Read(void *p);
void  Thread_Write(void *p);

int main(int argc, char* argv[])
{
	ReadWrite("rw_data.txt");
	return 0;
}

///////////////////////////////////////////
void ReadWrite(char *file)
{
	DWORD n_thread = 0;
	DWORD thread_ID;

	HANDLE		h_Thread[MAX_THREAD_NUM];
	ThreadInfo  thread_info[MAX_THREAD_NUM];


	ifstream inFile;
	inFile.open(file);
	puts("Read Data File \n");

	while (inFile >> thread_info[n_thread].serial)
	{
		inFile >> thread_info[n_thread].entity;
		inFile >> thread_info[n_thread].delay;
		inFile >> thread_info[n_thread].persist;
		n_thread++;
		inFile.get();
	}


	for (int i = 0; i < (int)(n_thread); i++)
	{
		if (thread_info[i].entity == READ)

			h_Thread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(Thread_Read),
				&thread_info[i], 0, &thread_ID);
		else
		{
			if (thread_info[i].entity == WRITE)

				h_Thread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(Thread_Write),
					&thread_info[i], 0, &thread_ID);

			else
			{
				puts("Bad File\n");
				exit(0);
			}

		}
	}

	InitializeCriticalSection(&sem_mutex);
	W = CreateSemaphore(NULL, 1, 1, "W");
	WaitForMultipleObjects(n_thread, h_Thread, TRUE, INFINITE);

	printf("Task is Finished!\n");
}


///////////////////////////////////////////


void  Thread_Write(void *p)
{

	DWORD m_delay;
	DWORD m_persist;
	int		m_serial;

	//读参数
	m_serial = ((ThreadInfo*)(p))->serial;
	m_delay = (DWORD)(((ThreadInfo*)(p))->delay*INTE_PER_SEC);
	m_persist = (DWORD)(((ThreadInfo*)(p))->persist*INTE_PER_SEC);
	while (true)
	{
		Sleep(m_delay);
		printf("W thread %d send the Write require\n", m_serial);
		WaitForSingleObject(W, INFINITE);
		printf("W thread %d Begin to Write\n", m_serial);
		Sleep(m_persist);
		printf("W thread %d Finish Write.\n", m_serial);
		ReleaseSemaphore(W, 1, NULL);
	}

}


///////////////////////////////////////////

void  Thread_Read(void *p)
{

	DWORD	m_delay;
	DWORD	m_persist;
	int		m_serial;

	m_serial = ((ThreadInfo*)(p))->serial;
	m_delay = (DWORD)(((ThreadInfo*)(p))->delay*INTE_PER_SEC);
	m_persist = (DWORD)(((ThreadInfo*)(p))->persist*INTE_PER_SEC);

	while (true)
	{
		Sleep(m_delay);
		printf("R thread %d send the Read require\n", m_serial);
		EnterCriticalSection(&sem_mutex);
		read_count++;
		if (read_count == 1) WaitForSingleObject(W, INFINITE);
		LeaveCriticalSection(&sem_mutex);

		printf("R thread %d Begin to Read\n", m_serial);
		Sleep(m_persist);
		printf("R thread %d Finish Read.\n", m_serial);

		EnterCriticalSection(&sem_mutex);
		read_count--;
		if (read_count == 0)	ReleaseSemaphore(W, 1, NULL);
		LeaveCriticalSection(&sem_mutex);
	}
}




