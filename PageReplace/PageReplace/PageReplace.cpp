// PageReplace.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <stdlib.h>
#include <time.h> 
#include <vector>
#include<fstream>
using namespace std;

int seed;//伪随机数种子
int capacity;//页框数量
int maxNumber;//最大页面编号
int seqLen;//页面序列长度
double missRatio;//缺页率

struct node {
	int page;//页面编号
	int v;//置换的权值
	node() {};
};
vector<node> queue;

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

int find(vector<node> q, int page) {
	for (int i = 0; i < q.size(); i++) {
		if (q[i].page == page) return i;
	}
	return -1;
}

int findMax(vector<node> q) {
	int m = -1;
	int pos = -1;
	for (int i = 0; i < q.size(); i++) {
		if (q[i].v > m) {
			m = q[i].v;
			pos = i;
		}
	}
	return pos;
}

void addV(vector<node> &q) {
	for (int i = 0; i < q.size(); i++) {
		q[i].v++;
	}
}

void print(vector<node> q){
	for (int i = 0; i < q.size(); i++) {
		cout << q[i].page << ":"<<q[i].v<<"  ";
	}
	cout << endl;
}
void FIFO() {
	cout << "\nFIFO算法:" << endl;
	while (!queue.empty()) queue.pop_back();
	int cnt = 0;
	int hit = 0;
	while (cnt < seqLen) {
		cnt++;
		int p = rand() % (maxNumber + 1);
		int pos = find(queue, p);
		if (pos != -1) {//命中
			cout << p << " 命中" << endl;
			hit++;
			//continue;
		}
		else {//未命中，如果队列已经满了则需要替换
			if (queue.size() >= capacity) {
				//满了，需要置换
				//该位置指向新的结点
				int maxx = findMax(queue);
				queue[maxx].page = p;
				queue[maxx].v = 0;
			}
			else {
				//未满，在后面加即可
				node newNode;
				newNode.page = p;
				newNode.v = 0;
				queue.push_back(newNode);
			}
		}
		//所有结点的v++
		addV(queue);
		//打印此时的队列信息
		print(queue);
	}
	cout << "缺页率：" << 1 - 1.0*hit / seqLen << endl;
}

void LRU() {
	cout << "\nLRU算法:" << endl;
	while (!queue.empty()) queue.pop_back();
	int cnt = 0;
	int hit = 0;
	while (cnt < seqLen) {
		cnt++;
		int p = rand() % (maxNumber + 1);
		int pos = find(queue, p);
		if (pos != -1) {//命中，需要把权值清零
			cout << p << " 命中" << endl;
			queue[pos].v = 0;
			hit++;
			//continue;
		}
		else {//未命中，如果队列已经满了则需要替换
			if (queue.size() >= capacity) {
				//满了，需要置换
				//该位置指向新的结点
				int maxx = findMax(queue);
				queue[maxx].page = p;
				queue[maxx].v = 0;
			}
			else {
				//未满，在后面加即可
				node newNode;
				newNode.page = p;
				newNode.v = 0;
				queue.push_back(newNode);
			}
		}
		//所有结点的v++
		addV(queue);
		//打印此时的队列信息
		print(queue);
	}
	cout << "缺页率：" << 1 - 1.0*hit / seqLen << endl;
}


int main()
{
	init();
	FIFO();
	LRU();
	return 0;
}


