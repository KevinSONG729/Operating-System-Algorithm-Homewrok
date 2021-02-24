#include<iostream>
#include<list>
#include<ctime>
#define virtualSize 32
#define len 256
#define INF 1000
using namespace std;

struct Node {
	int no;
	int time = 0;
	int Fre = 0;
};

bool SortStruct1(const Node& n1, const Node& n2) {
	return n1.time < n2.time;
}

bool SortStruct2(const Node& n1, const Node& n2) {
	return n1.Fre > n2.Fre;
}

void createRandom(int(&a)[300], int(&b)[300], int pagesize) {
	int m = 0, m1 = 0, m2 = 0,i = 0;
	while (i<len) {
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
	for (int i = 0; i < len; i++) {
		b[i] = a[i] / (1024 * pagesize);
	}
}

void OPT(int b[300], int called, list<Node>& l) {
	list<Node>::iterator it;
	/*cout << "修改前：" << endl;
	for (it = l.begin(); it != l.end(); it++) {
		cout << (*it).no << "->";
	}
	cout << endl;*/
	int p, q=0;
	int f = 0;
	int max = 0;
	int maxl = (*l.begin()).no;
	for (it = l.begin(); it != l.end(); it++) {
		f = 0;
		for (p = called+1; p < len; p++) {
			q = 0;
			if ((*it).no == b[p]) {
				q = p;
				f = 1;
				break;
			}
		}
		if (!f) q = INF;
		if (q > max) {
			max = q;
			maxl = (*it).no;
		}
	}
	/*cout << "max的值等于" << max << " 该置换出的页号为：" << maxl << endl;*/
	for (it = l.begin(); it != l.end(); it++) {
		if ((*it).no == maxl) (*it).no = b[called];
	}
	/*cout << "修改后：" << endl;
	for (it = l.begin(); it != l.end(); it++) {
		cout << (*it).no << "->";
	}
	cout << endl;*/
}

void FIFO(int b[300], int called, list<Node>& l) {
	l.pop_front();
	l.push_back({ b[called],0 });
}

void LRU(int b[300], int called, list<Node>& l) {
	Node x{ b[called] };
	l.sort(SortStruct1);
	l.pop_back();
	l.push_back(x);
}

void LFU(int b[300], int called, list<Node>& l) {
	Node x{ b[called] };
	list<Node>::iterator it;
	l.sort(SortStruct2);
	it = l.end(); it--;
	if((*it).no == b[called-1]) {
		--it;
	}
	(*it).no = b[called];
	(*it).Fre = 0;
}

void display(int(&a)[300], int(&b)[300], int pagesize) {
	cout << "THE VIRTUALADDRESS STREAM AS FOLLOWS:" << endl;
	for (int i = 0; i < len/4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << "a[" << 4 * i + j << "]=" << a[4 * i + j] << "\t";
		}
		cout << endl;
	}
	cout << "==============================================================" << endl;
	cout << "PAGE NUMBER WITH SIZE " << pagesize << "k FOR EACH ADDRESS IS:" << endl;
	for (int i = 0; i < len/4; i++) {
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
		for (int j = 0; j < len; j++) {
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