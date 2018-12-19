// Banker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include<iostream>
#include<vector>
#include<cstring>
#include<fstream>
#include<io.h>
using namespace std;
const int RES = 3;
const int PRO = 5;
int request[RES];//��������
int id;//����������Դ�Ľ��̺�

int Max[PRO][RES];
int Allocation[PRO][RES];
int Need[PRO][RES];
int Available[RES];
int Avail[RES];
void init() {
	for (int i = 0; i < PRO; i++) {
		for (int j = 0; j < RES; j++) {
			Available[j] -= Allocation[i][j];
			Need[i][j] = Max[i][j] - Allocation[i][j];
		}
	}
	memcpy(Avail, Available, sizeof(Available));
}


void getData() {
	ifstream in;
	in.open("in.txt");

	for (int j = 0; j < RES; j++) {
		in >> Available[j];
	}
	for (int i = 0; i < PRO; i++) {
		for (int j = 0; j < RES; j++) {
			in >> Max[i][j];
		}
		for (int j = RES; j < 2 * RES; j++) {
			in >> Allocation[i][j - RES];
		}
	}
	init();
}

//�ж�a���� �Ƿ�С�ڵ���  b����
bool cmp(int* a, int* b) {
	for (int i = 0; i < RES; i++) {
		if (a[i] > b[i]) return 0;
	}
	return 1;
}

void _plus(int a[], int b[]) {
	for (int i = 0; i < RES; i++) {
		a[i] += b[i];
	}
}

void print(int a[]) {
	for (int i = 0; i < RES; i++) {
		cout << a[i] << " ";
	}
	cout << endl;
}

//�ж��Ƿ�ȫ ���ǣ����һ����ȫ����
bool judge() {
	//Ԥ���ж�
	if (!cmp(request, Need[id]) || !cmp(request, Available)) {
		cout << "����������������" << endl;
		return 0;
	}

	bool flag = false;
	vector<int> seq;//�������д洢 
	bool finish[PRO];//������ 
	memset(finish, 0, sizeof finish);
	int cnt = 0;//�����

	while (cnt < PRO) {
		bool f = 0;//һ��֮���Ƿ�������������
		for (int i = 0; i < PRO; i++) {
			if (finish[i]) continue;//����Ѿ����������
			//�ж����Ƿ�������
			if (cmp(Need[i], Available)) {
				cout << "��" << i << "����" << ":" << endl;
				cout << "Need : ";
				print(Need[i]);
				cout << "Available : ";
				print(Available);
				cout << endl;
				finish[i] = 1;
				_plus(Available, Allocation[i]);
				cnt++;
				seq.push_back(i);
			}
		}

		if (!f) flag = 0;//��������
		if (cnt >= PRO) flag = 1;
	}


	if (flag) {
		cout << "��ȫ���У�" << endl;
		for (int i = 0; i < PRO; i++) {
			cout << seq[i] << " " << endl;
		}
	}

	return flag;
}

void input() {
	memcpy(Available, Avail, sizeof Available);
	cout << "������Դ�Ľ���:" << endl;
	cin >> id;
	cout << "������������:" << endl;
	int x;
	for (int i = 0; i < RES; i++) {
		cin >> request[i];
	}
}

int main()
{
	getData();
	while (true) {
		input();
		judge();
	}

	return 0;
}

