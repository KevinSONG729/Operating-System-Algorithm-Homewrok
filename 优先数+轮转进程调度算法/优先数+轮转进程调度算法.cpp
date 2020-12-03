﻿#include <iostream>
#include<ctime>
#include<cstdlib>
#include<string>
using namespace std;
int random(double, double);
class PCB {
public:
    PCB(int id = 0, int priority = 0, int alltime = 0, int cputime = 0 , short int status = 1, short int next = 0)
    {
        ID = id;
        PRIORITY = priority;
        CPUtime = cputime;
        Alltime = alltime;
        Next = next;
        STATUS = status;
    };
    ~PCB() {};
    void setID(int id) {
        ID = id;
    };
    void setNext(short int next) {
        Next = next;
    };
    void setPriority(int pri) {
        PRIORITY = pri;
    };
    void setCPUtime(int ctime) {
        CPUtime = ctime;
    };
    void SetAlltime(int all) {
        Alltime = all;
    };
    void setStatus(short int status) {
        STATUS = status;
    };
    int getID() const {
        return ID;
    };
    short int getNext() const {
        return Next;
    };
    int getPriority() const {
        return PRIORITY;
    };
    int getCPUtime() const {
        return CPUtime;
    };
    int getAlltime() const {
        return Alltime;
    };
    short int getStatus() const {
        return STATUS;
    };
private:
    int ID;
    short int Next;
    int PRIORITY;
    int CPUtime;
    int Alltime;
    short int STATUS;  //0:RUN 1:WAIT 2:FINISH
};

int GetMin(PCB* Array, int* &flag,int num) {
    int i = 0;
    while (flag[i] == 1) i++;
    if (i < num) {
        int temp = Array[i].getPriority();
        short int min = Array[i].getID();
        for (i = i + 1; i < num; i++) {
            if (flag[i] == 0) {
                if (Array[i].getPriority() < temp) {
                    temp = Array[i].getPriority();
                    min = Array[i].getID();
                }
            }
        }
        flag[min-1] = 1;
        return min;
    }
    return -1;
}

int CreatProcess(PCB* &Array, int num) {
    for (int i = 0; i < num; i++) {
        Array[i].setID(i+1);
        Array[i].setPriority(random(5, 50));
        Array[i].SetAlltime(random(1, 10));
    }
    int* flag = new int[num]{0};
    int min = GetMin(Array, flag,5);
    int mintemp = -1;
    for (int j = 1; j < num; j++) {
        mintemp = GetMin(Array, flag,5);
        //cout << mintemp << endl;
        Array[mintemp-1].setNext(min);
        min = mintemp;
    }
    return mintemp-1;
};

void PriorityProcess(int HEAD, int TAIL, int RUN, PCB* &Array) {
    RUN = HEAD;
    HEAD = Array[HEAD].getNext() - 1;
    Array[RUN].setStatus(0);
    Array[RUN].SetAlltime(Array[RUN].getAlltime() - 1);
    Array[RUN].setPriority(Array[RUN].getPriority() - 3);
    Array[RUN].setCPUtime(Array[RUN].getCPUtime() + 1);
    if (Array[RUN].getAlltime() == 0) {
        Array[RUN].setStatus(2);  //finish
    }
    else {
        if (Array[RUN].getPriority() >= Array[HEAD].getPriority()) {

        }
    }

}

void RoundRobinProcess() {

}

void Display(PCB* Array, string mode, int num) {
    cout << "OUTPUT OF " << mode << endl;
    cout << "==================================================" << endl;
    cout << "RUNNING PROC.\t\t\tWAITING QUEUE" << endl;

    cout << "==================================================" << endl;
    cout << "ID\t\t1\t2\t3\t4\t5" << endl;
    cout << "PRIORITY\t";
    for (int i = 0; i < num; i++) {
        cout << Array[i].getPriority() << "\t";
    }
    cout << endl;
    cout << "CPUTIME\t\t";
    for (int i = 0; i < num; i++) {
        cout << Array[i].getCPUtime() << "\t";
    }
    cout << endl;
    cout << "ALLTIME\t\t";
    for (int i = 0; i < num; i++) {
        cout << Array[i].getAlltime() << "\t";
    }
    cout << endl;
    cout << "STATUS\t\t";
    for (int i = 0; i < num; i++) {
        if (Array[i].getStatus() == 0) {
            cout <<"R"<< "\t";
        }
        else if (Array[i].getStatus() == 1) {
            cout << "W" << "\t";
        }
        else if (Array[i].getStatus() == 2) {
            cout << "F" << "\t";
        }
    }
    cout << endl;
    cout << "NEXT\t\t";
    for (int i = 0; i < num; i++) {
        cout << Array[i].getNext() << "\t";
    }
    cout << endl;
    cout << "==================================================" << endl;
}

int main()
{
    srand((unsigned)time(NULL));
    PCB* Array = new PCB[5]();
    int HEAD=-1;
    int TAIL = -1;
    int RUN = -1;
    HEAD = CreatProcess(Array, 5);
    for (int i = 0; i < 5; i++) {
        if (Array[i].getNext() == 0) TAIL = Array[i].getID() - 1;
    }
    string mode;
    cout << "TYPE THE ALGORITHM:";
    cin >> mode;
    Display(Array, mode,5);
}

int random(double start, double end) {
    return (int)(start + (end - start) * rand() / (RAND_MAX + 1.0));
}
