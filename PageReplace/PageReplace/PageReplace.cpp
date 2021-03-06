// PageReplace.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <stdlib.h>
#include <time.h> 
#include <vector>
#include <fstream>
#include <list>
using namespace std;
const int MAXPAGE = 1e3;
int seed;//伪随机数种子
int capacity;//页框数量
int maxNumber;//最大页面编号
int seqLen;//页面序列长度
double missRatio;//缺页率


list<int> PageList;
int exist[MAXPAGE];
void init() {
	
	ifstream fin;
	fin.open("in.txt");
	fin >> capacity;
	fin >> maxNumber;
	fin >> seed;
	fin >> seqLen;
	cout << "页框数量:"<<capacity;
	
	cout << "\n最大页面序号:"<<maxNumber;
	
	cout << "\n随机数种子:"<<seed;
	
	cout << "\n序列长度:" << seqLen << endl;;
	
	//srand((unsigned)time(NULL));
	srand(seed);
}

void outputList()
{
	list<int>::iterator it = PageList.begin();
	for (; it != PageList.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;

}
void FIFO() {
	cout << "FIFO:" << endl;
	PageList.clear();
	memset(exist, 0, sizeof exist);
	int cnt = 0;
	int hit = 0;
	while (cnt < seqLen) {
		cout << "页面情况：";
		outputList();

		cnt++;
		int p = rand() % (maxNumber + 1);
		if (exist[p] == 1) {//如果命中
			hit++;
			cout << "页面" << p << "命中" << endl;
			continue;
		}
		else {
			if (PageList.size() >= capacity) {
				int old = PageList.front();
				PageList.pop_front();//链首部弹出
				cout << "页面" << old << "被页面"<< p << "置换" << endl;
				PageList.push_back(p);//新页面进队列尾部
				exist[p] = 1;//新页面换进
				exist[old] = 0;//老页面换出
			}
			else {
				PageList.push_back(p);
				cout << "页面" << p << "入队" << endl;
				exist[p] = 1;
			}
		}
	}
	cout << "缺页率：" << 1.0 * (seqLen - hit) / seqLen << endl;
}

void LRU() {
	cout << "LRU:" << endl;
	PageList.clear();
	memset(exist, 0, sizeof exist);
	int cnt = 0;
	int hit = 0;
	
	while (cnt < seqLen) {
		cout << "页面情况：";
		outputList();
		cnt++;
		int p = rand() % (maxNumber + 1);
		if (exist[p] == 1) {//如果命中
			hit++;
			cout << "页面" << p << "命中" << endl;
			//把该结点移动到链尾
			PageList.remove(p);
			PageList.push_back(p);
			continue;
		}
		else {//如果没有命中
			//如果队列已经满了则需要置换
			if (PageList.size() >= capacity) {
				int old = PageList.front();
				PageList.pop_front();
				cout << "页面" << old << "被页面" << p << "置换" << endl;
				PageList.push_back(p);
				exist[p] = 1;
				exist[old] = 0;
			}
			else {
				PageList.push_back(p);
				cout << "页面" << p << "入队" << endl;
				exist[p] = 1;
			}
		}
	}
	cout << "缺页率：" << 1.0 * (seqLen - hit) / seqLen << endl;
}

struct node {
	int page;
	int A = 1;//访问位置1
	int M = rand() % 2;//修改位随机
	node() { page = -1; A = 0; M = 0; }//供数组初始化用
	node(int x) { page = x; }
};

node queue[MAXPAGE];

void print() {
	for (int i = 0; i < capacity; i++) {
		if(queue[i].page != -1)
			cout <<i << "\t"<< queue[i].page <<"\t"<< queue[i].A << "\t" << queue[i].M << endl;
	}
	cout << endl;
}
int FirstScan(int cur) {
	int cnt = 0;
	int i = cur;
	while (cnt < capacity) {
		if (queue[i].A == 0 && queue[i].M == 0) {
			cout << "第一轮找到：" << queue[i].page << " : A=0 M=0" << endl;
			return i;
		}
		i++;
		cnt++;
		if (i >= capacity) {
			i %= capacity;
		}
	}
	return -1;
}

int SecondScan(int cur) {
	int cnt = 0;
	int i = cur;
	while (cnt < capacity) {

		if (queue[i].A == 1) {
			queue[i].A = 0;
			cout << queue[i].page<<"执行了A = 0" << endl;
		}

		else if (queue[i].A == 0 && queue[i].M == 1) {
			cout << "第二轮找到：" << queue[i].page << " : A=0 M=1" << endl;
			return i;
		}
		i++;
		cnt++;
		if (i >= capacity) {
			i %= capacity;
		}
	}
	return -1;
}
int ThirdScan(int cur) {
	cout << "第三轮扫描[1]：" << endl;
	int a = FirstScan(cur);
	if (a == -1) {
		cout << "第三轮扫描[2]：" << endl;
		return SecondScan(cur);
	}
	else {
		return a;
	}
}

void ModifiedClock() {
	int pos = 0;//队列指针
	int cnt = 0;//计数器
	int hit = 0;//命中次数
	memset(exist, -1, sizeof exist);
	while (cnt < seqLen)
	{
		
		cnt++;
		int p = rand() % (maxNumber + 1);

		if (exist[p] != -1) {//说明命中
			cout << "命中，页面：" << p << " 位置 ： " << exist[p] << endl;
			//更新一下该页的状态
			queue[exist[p]].A = 1;
			queue[exist[p]].M = rand()%2;

			//更新指针的位置
			pos =(exist[p] + 1) % capacity;

			hit++;

			print();
			cout << "指针位置：pos = " << pos << endl;
			continue;
		}
		else { //如果未命中
		

			//替换页面
			int a = FirstScan(pos);
			if (a == -1) {
				int b = SecondScan(pos);
				if (b == -1) {//第二遍找不到
					pos = ThirdScan(pos); //第三遍肯定能找到 

					
				}
				else {//第二遍找到了
					pos = b;
				}
			}
			else {//第一遍找到了
				pos = a;
			}
			//pos位置的页面被淘汰了，则该页面的存在位置-1
			exist[queue[pos].page] = -1;
			
			//新的页面替换进来
			queue[pos] = node(p);
			exist[queue[pos].page] = pos;//记录该页面位置
			pos = (pos + 1) % capacity;//指针指向下一个
			print();
		}
		cout << "指针位置：pos = " << pos << endl;
		
	}

	cout << "改进的Clock算法缺页率：" << 1 - (1.0 * hit / seqLen);
}

int main()
{
	init();
	FIFO();
	LRU();
	ModifiedClock();
	return 0;
}


