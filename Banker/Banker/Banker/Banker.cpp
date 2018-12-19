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
int request[RES];//请求向量
int id;//正在请求资源的进程号

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

//判断a向量 是否小于等于  b向量
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

//判断是否安全 若是，输出一个安全序列
bool judge() {
	//预先判断
	if (!cmp(request, Need[id]) || !cmp(request, Available)) {
		cout << "不满足请求条件！" << endl;
		return 0;
	}

	bool flag = false;
	vector<int> seq;//工作序列存储 
	bool finish[PRO];//完成情况 
	memset(finish, 0, sizeof finish);
	int cnt = 0;//完成数

	while (cnt < PRO) {
		bool f = 0;//一趟之内是否有任务可以完成
		for (int i = 0; i < PRO; i++) {
			if (finish[i]) continue;//如果已经完成则跳过
			//判断其是否可以完成
			if (cmp(Need[i], Available)) {
				cout << "第" << i << "进程" << ":" << endl;
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

		if (!f) flag = 0;//死锁发生
		if (cnt >= PRO) flag = 1;
	}


	if (flag) {
		cout << "安全序列：" << endl;
		for (int i = 0; i < PRO; i++) {
			cout << seq[i] << " " << endl;
		}
	}

	return flag;
}

void input() {
	memcpy(Available, Avail, sizeof Available);
	cout << "请求资源的进程:" << endl;
	cin >> id;
	cout << "输入请求向量:" << endl;
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

