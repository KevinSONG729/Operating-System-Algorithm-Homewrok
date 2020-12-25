#include<iostream>
#include<list>
#include<ctime>
#define virtualSize 32
using namespace std;

struct Node {
	int no;
	int time = 0;
	int Fre = 0;
};

void createRandom(int(&a)[300], int(&b)[300], int pagesize) {
	int m = 0, m1 = 0, m2 = 0,i = 0;
	while (i<256) {
		m = rand() % 32767;
		a[i] = m + 1; ++i;
		m1 = rand() % (m + 1);
		a[i] = m1; ++i;
		a[i] = m1 + 1; ++i;
		if (m1 < 32765) {
			m2 = rand() % (32767 - (m1 + 2)) + m1 + 2;
			a[i] = m2; ++i;
		}
	}
	for (int i = 0; i < 256; i++) {
		b[i] = a[i] / (1024 * pagesize);
	}
}

void OPT(int b[300], int called, list<Node>& l) {
	list<Node>::iterator it;
	int p, q=0;
	int max = 0;
	for (it = l.begin(); it != l.end(); it++) {
		for (p = called; p < 256; p++) {
			if ((*it).no == b[p]) {
				q = p;
				break;
			}
		}
		if (q > max) {
			max = q;
		}
	}
	for (it = l.begin(); it != l.end(); it++) {
		if ((*it).no == b[max]) (*it).no = b[called];
	}
}

void FIFO(int b[300], int called, list<Node>& l) {
	l.pop_front();
	l.push_back({ b[called],0 });
}

void LRU(int b[300], int called, list<Node>& l) {
	int max = 0;
	list<Node>::iterator it;
	list<Node>::iterator maxit;
	for (it = l.begin(); it != l.end();it++) {
		if ((*it).time > max) {
			max = (*it).time;
			maxit = it;
		}
	}
	(*maxit).no = b[called];
	(*maxit).time = 0;
}

void LFU(int b[300], int called, list<Node>& l) {
	int min = 100;
	list<Node>::iterator it;
	list<Node>::iterator minit;
	for (it = l.begin(); it != l.end(); it++) {
		if ((*it).time < min) {
			min = (*it).Fre;
			minit = it;
		}
	}
	(*minit).no = b[called];
	(*minit).Fre = 0;
}

void display(int(&a)[300], int(&b)[300], int pagesize) {
	cout << "THE VIRTUALADDRESS STREAM AS FOLLOWS:" << endl;
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 4; j++) {
			cout << "a[" << 4 * i + j << "]=" << a[4 * i + j] << "\t";
		}
		cout << endl;
	}
	cout << "==============================================================" << endl;
	cout << "PAGE NUMBER WITH SIZE " << pagesize << "k FOR EACH ADDRESS IS:" << endl;
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 4; j++) {
			cout << "pageno[" << 4 * i + j << "]=" << b[4 * i + j] << "\t";
		}
		cout << endl;
	}
	cout << "vmsize=" << virtualSize << "k\tpagesize=" << pagesize << "k" << endl;
}

void page_assigned(int pagesize, int mode, int b[300]) { 
	cout << "-------------------------------------------" << endl;
	cout << "page assigned\t\t\tpages_in/total references" << endl;
	double rate = 0;
	list<Node> l;
	list<Node>::iterator it;
	for (int i = 4; i <= virtualSize / pagesize; i += 2) {
		int time[32] = { 0 };
		double page_in = 0, page_out = 0;
		for (int j = 0; j < 256; j++) {
			if (!l.empty()) {
				int p = 0;
				for (it = l.begin(); it != l.end(); it++) {
					if ((*it).no == b[j]) {
						page_in++;
						(*it).time = 0;
						(*it).Fre++;
						p = 1;
						break;
					}
				}
				if (p == 0) {
					for (it = l.begin(); it != l.end(); it++) {
						(*it).time++;
					}
					if (l.size() < i) {
						l.push_back({ b[j],0,0});
						page_out++;
					}
					else if (l.size() == i) {
						switch (mode)
						{
						case 0:
							OPT(b, j, l); break;
						case 1:
							FIFO(b, j, l); break;
						case 2:
							LRU(b, j, l); break;
						case 3:
							LFU(b, j, l); break;
						}
						page_out++;
					}
				}
			}
			else {
				l.push_back({b[j],0,0});
				page_out++;
			}
		}
		cout << i << "\t\t\t\t" << page_in/(page_out+page_in) << endl;
		l.clear();
	}
}

int main() {
	string mode;
	int pagesize = 1; //1k~8k
	srand(time(NULL));
	cout << "pagesize="; cin >> pagesize;
	int a[300] = { 0 };
	int b[300] = { 0 };
	createRandom(a,b,pagesize);
	display(a,b,pagesize);
	cout << "The algorithm is:";
	while (cin >> mode) {
		if (mode == "OPT") {
			page_assigned(pagesize, 0, b);
		}
		else if (mode == "FIFO") {
			page_assigned(pagesize, 1, b);
		}
		else if (mode == "LRU") {
			page_assigned(pagesize, 2, b);
		}
		else if (mode == "LFU") {
			page_assigned(pagesize, 3, b);
		}
		cout << "The algorithm is:";
	}
}