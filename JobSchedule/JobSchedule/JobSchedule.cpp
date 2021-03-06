// JobSchedule.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <cstring>
#include<iomanip>
using namespace std;

int total = 5;//总作业数量

struct Time {
	int h;
	int m;
	Time() {}
};

void getTime(Time a) {
	printf("%.2d:%.2d", a.h, a.m);
}

struct  Job
{
	int id;//作业编号
	string name;//作业名
	int submit;//提交时间
	int len;//服务时间
	int wait = 0;//等待时间
	int start;//开始时间
	int finish;//完成时间
	int priod = 0;//周转时间
	Job() {};
};

bool cmp1(const Job &a, const Job &b) {
	return a.submit < b.submit;
}

bool cmp2(const Job &a, const Job &b) {
	return a.len < b.len;
}

bool cmp3(Job a, Job b) {
	//cout << 1.0*a.wait / 1.0*a.len << " " << 1.0*b.wait / 1.0*b.len << endl;
	return (1.0*a.wait) / (1.0*a.len) > (1.0*b.wait) / (1.0*b.len);
}


Job a[10];
Job b[10];
Job c[10];
vector<Job> workSeq;

void getTime(int time) {
	int h = time / 60;
	int m = time % 60;
	printf("%.2d:%.2d", h, m);
}

void copy(Job *a, Job *b) {
	for (int i = 0; i < 10; i++) {
		a[i].finish = b[i].finish;
		a[i].id = b[i].id;
		a[i].len = b[i].len;
		a[i].name = b[i].name;
		a[i].priod = b[i].priod;
		a[i].start = b[i].start;
		a[i].submit = b[i].submit;
		a[i].wait = b[i].wait;
	}
}

void init() {
	for (int i = 0; i < 5; i++) {
		a[i].id = i+1;
		//a[i].name = "J"
	}
	a[0].name = "JA";
	a[1].name = "JB";
	a[2].name = "JC";
	a[3].name = "JD";
	a[4].name = "JE";

	a[0].submit = 160;
	a[1].submit = 170;
	a[2].submit = 175;
	a[3].submit = 180;
	a[4].submit = 185;

	a[0].len = 20;
	a[1].len = 30;
	a[2].len = 10;
	a[3].len = 24;
	a[4].len = 6;

	copy(b, a);
	copy(c, a);
}


void printSeq() {
	double avgP = 0;
	double avgPW = 0;
	string g = "\t\t";
	cout << "作业编号\t" << "作业名称\t" << "提交时间\t" << "要求时间\t" << "开始时间\t" << "完成时间\t" << "等待时间\t" << "周转时间\t" << endl;
	for (int i = 0; i < workSeq.size(); i++) {
		Job x = workSeq[i];

		avgP += x.priod;
		avgPW += 1.0*x.priod / x.len;
		//<<"平均周转时间\t"<<"带权周转时间\t"

		cout << x.id << g << x.name << g;
		getTime(x.submit);
		cout << g << x.len << g;
		getTime(x.start);
		cout << g;
		getTime(x.finish);
		cout << g;
		cout << x.wait;
		cout << g << x.priod << endl;
	}
	cout << "\n" << "平均周转时间 : " << avgP / total << "   带权周转时间: " << avgPW / total << endl;
	workSeq.clear();
}

int getJob(Job a[], bool finish[] , int time) {
	for (int i = 0; i < total; i++) { //a数组已经按照条件排好序
		if (!finish[i] && a[i].submit <= time) { //该作业必须没有做 并且提交时间早于当前时间
			return i;
		}
	}
	return -1;
}

int getPos(Job a[], int id) {
	for (int i = 0; i < total; i++) {
		if (a[i].id == id) return i;
	}
	return -1;
}

void FCFS() {
	sort(a, a + total,cmp1);//先按提交时间先后进行排序
	int curtime = a[0].submit;
	bool finish[10];
	memset(finish, 0, sizeof finish);
	bool busy = 0;//忙碌标志
	Job curJob;//当前作业
	int len = 0;//当前作业执行时间
	while(true) {
		if (workSeq.size() >= total) {
			break;
		}
		//如果没有作业正在执行
		if (!busy) {
			curJob = a[getJob(a,finish , curtime)];//获取新作业
			curJob.start = curtime; //该作业开始时间
			curJob.wait = curtime - curJob.submit; //该作业等待时间
			len = 0;//执行时间清零
			busy = 1;//忙碌置为1
			
		}
		//如果当前作业执行完毕
		else if(busy && len >= curJob.len){
			finish[getPos(a,curJob.id)] = 1;//标记该工作已经完成 注意在数组中它的位置等于其标号减1
			busy = 0;//忙碌置为0
			curJob.finish = curtime;//完成时间
			curJob.priod = curJob.finish - curJob.submit;//周转时间
			workSeq.push_back(curJob);//压入工作序列

		}
		////当前作业正在执行中
		if (busy) {
			len++;
			curtime++;
		}
		
	}

}


void SJF() {
	sort(b, b + total, cmp2);//先按作业长短进行排序
	int curtime = a[0].submit;
	bool finish[10];
	memset(finish, 0, sizeof finish);
	bool busy = 0;//忙碌标志
	Job curJob;//当前作业
	int len = 0;//当前作业执行时间
	while (true) {
		if (workSeq.size() >= total) {
			break;
		}
		//如果没有作业正在执行
		if (!busy) {
			int newJob = getJob(b, finish, curtime);
			if (newJob == -1) { curtime++;  continue; }
			curJob = b[newJob];//获取新作业
			curJob.start = curtime; //该作业开始时间
			curJob.wait = curtime - curJob.submit; //该作业等待时间
			len = 0;//执行时间清零
			busy = 1;//忙碌置为1

		}
		//如果当前作业执行完毕
		else if (busy && len >= curJob.len && !finish[getPos(b , curJob.id)]) {
			finish[getPos(b, curJob.id)] = 1;//标记该工作已经完成 注意在数组中它的位置等于其标号减1
			busy = 0;//忙碌置为0
			curJob.finish = curtime;//完成时间
			curJob.priod = curJob.finish - curJob.submit;//周转时间
			workSeq.push_back(curJob);//压入工作序列

		}
		////当前作业正在执行中
		if (busy ) {
			len++;
			curtime++;
		}

	}
}

bool contain(int id) {
	for (int i = 0; i < workSeq.size(); i++) {
		if (workSeq[i].id == id) return 1;
	}
	return 0;
}

void waitInc( int id, int time) {
	for (int i = 0; i < total; i++) {
		if (contain(c[i].id) || c[i].submit >= time || c[i].id == id) continue;
		c[i].wait++;
	}
}

void HRRN() {
	sort(c, c + total, cmp3);
	int curtime = a[0].submit;
	bool finish[10];
	memset(finish, 0, sizeof finish);
	bool busy = 0;//忙碌标志
	Job curJob;//当前作业
	int len = 0;//当前作业执行时间
	while (true) {
		if (workSeq.size() >= total) {
			break;
		}
		//如果没有作业正在执行
		if (!busy) {
			int newJob = getJob(c, finish, curtime);
			//if (newJob == -1) { cout << "?" << endl; curtime++;  continue; }
			curJob = c[newJob];//获取新作业
			curJob.start = curtime; //该作业开始时间
			//curJob.wait = curtime - curJob.submit; //该作业等待时间
			len = 0;//执行时间清零
			busy = 1;//忙碌置为1

		}
		//如果当前作业执行完毕
		else if (busy && len >= curJob.len) {

			finish[getPos(c, curJob.id)] = 1;//标记该工作已经完成

			busy = 0;//忙碌置为0
			curJob.finish = curtime;//完成时间
			curJob.priod = curJob.finish - curJob.submit;//周转时间
			workSeq.push_back(curJob);//压入工作序列

		}
		////当前作业正在执行中
		if (busy) {
			len++;
			curtime++;

			//其他作业等待时间不断增加
			waitInc(curJob.id, curtime);

			
		}
		//更新所有作业优先权
		sort(c, c + total, cmp3);
	}
}

int main() {
	init();
	FCFS();
	printSeq();

	SJF();
	printSeq();

	HRRN();
	printSeq();
	return 0;
}