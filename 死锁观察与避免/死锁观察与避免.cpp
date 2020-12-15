#include <iostream>
#include<ctime>
#include<cstdlib>
#include<string>
#include<vector>
#define SysSources 10
using namespace std;
int nu=0;
void display(int mode, int INQUIRY, vector<int> VCOUNT, vector<vector<int>> AP, vector<int> ALLOCATION, int REMAINDER)
{
	if (VCOUNT[INQUIRY] < VCOUNT.size()) {
		cout << "(" << ++nu << ")\t\t";
		cout << INQUIRY + 1 << "\t\t" << AP[INQUIRY][VCOUNT[INQUIRY]] << "\t\t" << ALLOCATION[INQUIRY] << "\t\t\t" << REMAINDER << endl;
		//cout << VCOUNT[INQUIRY];
		if (mode == 0) {
			cout << "RESOURCE IS ALLOCATED TO PROGRESS " << INQUIRY + 1 << endl;
		}
		else if (mode == 1) {
			cout << "IF ALLOCATED, DEADLOCK MAY OCCUR" << endl;
		}
		else if (mode == 2) {
			cout << "THE REMAINDER IS LESS THAN PROGRESS " << INQUIRY + 1 << " CLAIMS" << endl;
		}
		else if (mode == 4) {
			cout << "THE WHOLE WORK IS COMPLETED\n****************************" << endl;
		}
	}
	else {
		cout << "(" << ++nu << ")\t\t";
		cout << INQUIRY + 1 << "\t\t" << AP[INQUIRY][VCOUNT[INQUIRY]-1] << "\t\t" << ALLOCATION[INQUIRY] << "\t\t\t" << REMAINDER << endl;
		//cout << VCOUNT[INQUIRY];
		if (mode == 0) {
			cout << "RESOURCE IS ALLOCATED TO PROGRESS " << INQUIRY + 1 << endl;
		}
		if (mode == 1) {
			cout << "IF ALLOCATED, DEADLOCK MAY OCCUR" << endl;
		}
		if (mode == 2) {
			cout << "THE REMAINDER IS LESS THAN PROGRESS " << INQUIRY + 1 << " CLAIMS" << endl;
		}
		if (mode == 3) {
			cout << "PROGRESS " << INQUIRY + 1 << " HAS FINISHED,RETURN ITS RESOURCES" << endl;
		}
		if (mode == 4) {
			cout << "THE WHOLE WORK IS COMPLETED\n****************************" << endl;
		}
	}
}

bool Safe(vector<int> VCOUNT,
	int REMAINDER, int TOTAL, vector<int> VALLOCATION, int INQUIRY,
	vector<int> VPMAXCLAIM, vector<vector<int>> AP, vector<bool> VPSTATUS) {
	/*
		查看是否所有进程都运行完毕
	*/
	VALLOCATION[INQUIRY] += AP[INQUIRY][VCOUNT[INQUIRY]];
	TOTAL += AP[INQUIRY][VCOUNT[INQUIRY]];
	REMAINDER -= AP[INQUIRY][VCOUNT[INQUIRY]];
	VCOUNT[INQUIRY]++;
	bool PROGRESS = 1;
	while (PROGRESS) {
		int p = 0;
		for (int i = 0; i < VPSTATUS.size(); i++) {
			if (VPSTATUS[i] == true) {
				//cout << "VPSTATUS[i] == true进入" << endl;
				if (REMAINDER + VALLOCATION[i] >= VPMAXCLAIM[i]) {
					//cout << "REMAINDER + VALLOCATION[i] >= VPMAXCLAIM[i]进入" << endl;
					VPSTATUS[i] = false;
					REMAINDER += VALLOCATION[i];
					TOTAL -= VALLOCATION[i];
					VALLOCATION[i] = 0;
					p = p + 1;
					//cout << p << endl;
				}
			}
		}
		if (p==0) {
			//cout << "p==0" << endl;
			//cout << "不安全" << endl;
			return false;
		}
		for (int i = 0; i < VPSTATUS.size(); i++) {
			 PROGRESS*= -VPSTATUS[i];
		}
		PROGRESS = -PROGRESS;
	}
	//cout << "安全" << endl;
	return true;
}

void DeadLockHandler(int OPTION, int &ALLEND, vector<int>& VCOUNT, 
	int& REMAINDER, int& TOTAL, vector<int>& VALLOCATION, int INQUIRY, 
	vector<int> VPMAXCLAIM, vector<vector<int>> AP, vector<bool> VPSTATUS, int &p1) {
	if (REMAINDER >= VPMAXCLAIM[INQUIRY] - VALLOCATION[INQUIRY]) {
		if (Safe(VCOUNT, REMAINDER, TOTAL, VALLOCATION, INQUIRY, VPMAXCLAIM, AP, VPSTATUS)) {
			VALLOCATION[INQUIRY] += AP[INQUIRY][VCOUNT[INQUIRY]];
			TOTAL += AP[INQUIRY][VCOUNT[INQUIRY]];
			REMAINDER -= AP[INQUIRY][VCOUNT[INQUIRY]];	
			p1++;
			display(0, INQUIRY, VCOUNT, AP, VALLOCATION, REMAINDER);
			VCOUNT[INQUIRY]++;
			return;
		}
		else {
			display(1, INQUIRY, VCOUNT, AP, VALLOCATION, REMAINDER);
			if (OPTION == 1) {
				cout << "采用死锁预防算法，进程" << INQUIRY+1 << "分配不安全，不予分配" << endl;
			}
			else {
				cout << "不采用死锁预防算法，进程" << INQUIRY+1 << "虽不安全，但还是分配" << endl;
				//display(0, INQUIRY, VCOUNT, AP, VALLOCATION, REMAINDER);
				VALLOCATION[INQUIRY] += AP[INQUIRY][VCOUNT[INQUIRY]];
				TOTAL += AP[INQUIRY][VCOUNT[INQUIRY]];
				REMAINDER -= AP[INQUIRY][VCOUNT[INQUIRY]];
				VCOUNT[INQUIRY]++;
				p1++;
			}
		}
	}
	else {
		display(2, INQUIRY, VCOUNT, AP, VALLOCATION, REMAINDER);
		if (OPTION == 0) {
			cout << "不采用死锁预防，进程" << INQUIRY << "发生死锁" << endl;
			exit(0);
		}
		else {
			cout << "采用死锁预防，进程" << INQUIRY << "当前申请不发生分配" << endl;
			ALLEND = 1;
			return;
		}
	}
}

void DeadLock(int OPTION, vector<vector<int>> AP, 
			  vector<int> VPMAXCLAIM, vector<int> &VALLOCATION, 
			  vector<bool> &VPSTATUS, vector<int> &VCOUNT,
			  int TOTAL, int REMAINDER, int &INQUIRY)
{
	int nump=0,numc=0;
	cout << "INPUT:\nOPTION = ";
	cin >> OPTION;
	cout << "Process Num = ";
	cin >> nump;
	cout << "Clain Num = ";
	cin >> numc;
	AP.resize(nump);
	for (int i = 0; i < (int)AP.size(); i++) {
		AP[i].resize(numc);
	}
	for (int i = 0; i < nump; i++) {
		VPSTATUS.push_back(true);
		VCOUNT.push_back(0);
		VALLOCATION.push_back(0);
		cout << "CLAIM OF PROGRSS " << i + 1 << " IS:";
		int clain = 0;
		for (int j = 0; j < numc; j++) {
			cin >> clain;
			AP[i][j]=clain;
		}
	}
	for (int i = 0; i < nump; i++) {
		int max = 0;
		cout << "MAXCLAIM OF PROGRESS IS:";
		cin >> max;
		if (max > SysSources) {
			cout << "第" << i << "个最大资源需求量超过系统资源！！";
			return;
		}
		VPMAXCLAIM.push_back(max);
	}
	cout << "THE SYSTEM ALLOCATION PROGRESS IS AS FOLLOWS:\n";
	cout << "\t\tPROCESS\t\tCLAIM\t\tALLOCATION\t\tREMAINDER\n";
	int ALLEND = 0;
	while (!ALLEND) {
		int p1 = 0;
		for (int i = 0; i < VPSTATUS.size(); i++) {
			if (VPSTATUS[i] == true) {
				if (VCOUNT[i] == numc) {
					int p = VALLOCATION[i];
					TOTAL -= p;
					REMAINDER += p;
					VALLOCATION[i] = 0;
					VPSTATUS[i] = false;
					display(3, i, VCOUNT, AP, VALLOCATION, REMAINDER);
					p1++;
					continue;
				}
				else {
					if (VALLOCATION[i] + AP[i][VCOUNT[i]]>VPMAXCLAIM[i]) {
						cout << "第" << i << "个进程申请量超过了需求量！！" << endl;
						return;
					}
					else {
						INQUIRY = i;
						DeadLockHandler(OPTION, ALLEND, VCOUNT, REMAINDER, TOTAL, VALLOCATION, INQUIRY, VPMAXCLAIM, AP, VPSTATUS, p1);
					}
				}
			}
		}
		if (!p1 && OPTION) {
			cout << "当前所有进程资源申请都不安全!" << endl;
			ALLEND = 1;
			continue;
		}
		for (int i = 0; i < VPSTATUS.size(); i++) {
			if (VPSTATUS[i] == true) {
				ALLEND = -1;
				break;
			}
		}
		if (ALLEND == -1) {
			ALLEND = 0;
		}
		else {
			ALLEND = 1;
		}
	}
	display(4, INQUIRY, VCOUNT, AP, VALLOCATION, REMAINDER);
}

int main() {
	int OPTION = 0;  //0：防止死锁  1：不防止死锁
	int TOTAL = 0;  //已分配的系统资源总数
	int REMAINDER = SysSources;  //剩余的系统资源总数
	int INQUIRY = -1;  //当前运行进程号
	vector<vector<int> > AP;  //资源请求矢量
	vector<int> VPMAXCLAIM;  //第I进程对资源的最大需求量
	vector<int> VALLOCATION;  //第I进程已分配到的资源数
	vector<bool> VPSTATUS;  //第I进程完成请求标志，为 1 时表示已完成各次请求
	vector<int> VCOUNT;  //第 I 进程请求次数计数器。其值表示该进程的第几次请求
	DeadLock(OPTION, AP, VPMAXCLAIM, VALLOCATION, VPSTATUS, VCOUNT, TOTAL, REMAINDER, INQUIRY);
	return 0;
}