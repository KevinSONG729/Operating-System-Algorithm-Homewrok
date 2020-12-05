#include <iostream>
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

void Display(PCB*, string, int, int, int, int, int, int, int, int);

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

int CreatProcess(PCB* &Array, int num, string mode) {
    for (int i = 0; i < num; i++) {
        Array[i].setID(i+1);
        if (mode == "Priority") {
            Array[i].setPriority(random(5, 50));
        }
        else if (mode == "Round_Robin") {
            Array[i].setPriority(random(2, 7));
        }
        else {
            Array[i].setPriority(49);
        }
        Array[i].SetAlltime(random(1, 10));
    }
    int* flag = new int[num]{0};
    int min = GetMin(Array, flag,num);
    int mintemp = -1;
    for (int j = 1; j < num; j++) {
        mintemp = GetMin(Array, flag,num);
        //cout << mintemp << endl;
        Array[mintemp-1].setNext(min);
        min = mintemp;
    }
    return mintemp-1;
};

void PriorityProcess(int &HEAD, int &TAIL, int &RUN, PCB* &Array) {
    Display(Array, "Priority", 5, RUN, HEAD, TAIL, -1, -1, -1, -1);
    RUN = HEAD;
    Array[RUN].setStatus(0);
    HEAD = Array[HEAD].getNext() - 1;
    while(Array[TAIL].getStatus()!=2){
        while (Array[RUN].getPriority() >= Array[HEAD].getPriority() && Array[RUN].getAlltime() != 0) {
            Array[RUN].SetAlltime(Array[RUN].getAlltime() - 1);
            Array[RUN].setPriority(Array[RUN].getPriority() - 3);
            Array[RUN].setCPUtime(Array[RUN].getCPUtime() + 1);
        }
        Display(Array, "Priority", 5, RUN, HEAD, TAIL, -1, -1, -1, -1);
        if (Array[RUN].getAlltime() == 0) {
            Array[RUN].setStatus(2);
            if (RUN == HEAD && HEAD == TAIL) continue;
            RUN = HEAD;
            Array[RUN].setStatus(0);
            if (HEAD != TAIL) {
                HEAD = Array[HEAD].getNext() - 1;
            }
            Display(Array, "Priority", 5, RUN, HEAD, TAIL, -1, -1, -1, -1);
            continue;
        }
        else if (Array[RUN].getPriority() < Array[HEAD].getPriority()) {
            int temp = RUN;
            Array[RUN].setStatus(1);
            RUN = HEAD;
            if (HEAD != TAIL) {
                HEAD = Array[HEAD].getNext() - 1;
            }
            else {
                HEAD = temp;
            }
            if (Array[temp].getPriority() > Array[HEAD].getPriority()) {
                Array[temp].setNext(HEAD + 1);
                HEAD = temp;
            }
            else if (Array[temp].getPriority()<Array[TAIL].getPriority()){
                Array[TAIL].setNext(temp + 1);
                Array[temp].setNext(0);
                TAIL = temp;
            }
            else {
                int sxw = HEAD;
                int xws = Array[sxw].getNext() - 1;
                while (Array[temp].getPriority() > Array[sxw].getPriority()) {
                    sxw = xws;
                    xws = Array[sxw].getNext() - 1;
                }
                Array[sxw].setNext(temp + 1);
                Array[temp].setNext(xws + 1);
            }
            Array[RUN].setStatus(0);
            Display(Array, "Priority", 5, RUN, HEAD, TAIL, -1, -1, -1, -1);
            //cout << "temp=" << temp;
        }
        else {
            continue;
        }
        //Display(Array, "Priority", 5);
        cout << "HEAD=" << HEAD <<" TAIL="<<TAIL<<" RUN="<<RUN<< endl;
    }
    Display(Array, "Priority", 5, RUN, HEAD, TAIL, -1, -1, -1, -1);
    //cout << "跳出！！HEAD=" << HEAD << " TAIL=" << TAIL << endl;
}

void RoundRobinProcess(int& HEAD, int& TAIL, int& RUN, PCB*& Array) {
    Display(Array, "Round Robin", 5, RUN, HEAD, TAIL, -1, -1, -1, -1);
    RUN = HEAD;
    Array[RUN].setStatus(0);
    HEAD = Array[HEAD].getNext() - 1;
    while (Array[TAIL].getStatus() != 2) {
        while (Array[RUN].getCPUtime() != Array[RUN].getPriority() && Array[RUN].getAlltime()!=0) {
            Array[RUN].setCPUtime(Array[RUN].getCPUtime() + 1);
            Array[RUN].SetAlltime(Array[RUN].getAlltime() - 1);
            Display(Array, "Round Robin", 5, RUN, HEAD, TAIL, -1, -1, -1, -1);
        }
        if (Array[RUN].getAlltime() == 0) {
            Array[RUN].setStatus(2);
            //RUN = HEAD;
            //Array[RUN].setStatus(0);
            //HEAD = Array[HEAD].getNext() - 1;
            if (RUN == HEAD && HEAD == TAIL) continue;
            RUN = HEAD;
            Array[RUN].setStatus(0);
            if (HEAD != TAIL) {
                HEAD = Array[HEAD].getNext() - 1;
            }
        }
        else if (Array[RUN].getCPUtime() == Array[RUN].getPriority()) {
            int temp = RUN;
            Array[RUN].setCPUtime(0);
            Array[RUN].setStatus(1);
            RUN = HEAD;
            Array[RUN].setStatus(0);
            if (HEAD != TAIL) {
                HEAD = Array[HEAD].getNext() - 1;
            }
            else {
                HEAD = temp;
            }
            Array[TAIL].setNext(temp + 1);
            Array[temp].setNext(0);
            TAIL = temp;
        }
        Display(Array, "Round Robin", 5, RUN, HEAD, TAIL, -1, -1, -1, -1);
    }
    Display(Array, "Round Robin", 5, RUN, HEAD, TAIL, -1, -1, -1, -1);
}

void PBProcess(int& HEAD, int& TAIL, int& RUN, PCB*& Array, int num) {
//设置三级反馈队列，每级队列优先级是Priority，轮转时间片数是50-Priority，每级队列之间优先级差3，最后一级队列采用简单轮转法，其他队列用于先来先出。
    int HEAD1=-1, TAIL1=-1, HEAD2=-1, TAIL2=-1;
    //Display(Array, "PB", num, RUN, HEAD, TAIL, HEAD1, TAIL1, HEAD2, TAIL2);
    int num1 = num, num2 = 0, num3 = 0;
    int numall = num;
    int addtime = 0;
    while (num != 0) {
        if (addtime == 7) {
            PCB add(6,49,7,0,1,0);
            Array[5] = add;
            ++num;
            cout << "突然插入一个进程！！"<<endl;
            if (num1 == 0) {
                HEAD = TAIL = 5;
                Array[TAIL].setNext(0);
            }
            else {
                Array[TAIL].setNext(5);
                TAIL = 5;
                Array[TAIL].setNext(0);
            }
            ++num1;
        }
        if (HEAD != -1) {
            RUN = HEAD;
            Array[RUN].setStatus(0);
            HEAD = Array[HEAD].getNext() - 1;
            Array[RUN].setCPUtime(Array[RUN].getCPUtime() + 1);
            Array[RUN].SetAlltime(Array[RUN].getAlltime() - 1);
            if (Array[RUN].getAlltime() <= 0) {
                Array[RUN].setStatus(2);
                Array[RUN].setCPUtime(Array[RUN].getCPUtime() + Array[RUN].getAlltime());
                Array[RUN].SetAlltime(0);
                --num1;
                --num;
                Display(Array, "PB", numall, RUN, HEAD, TAIL, HEAD1, TAIL1, HEAD2, TAIL2);
                cout << "一级队列任务结束" << endl;
                cout << "HEAD = " << HEAD << " TAIL = " << TAIL << " HEAD1 = " << HEAD1 << " TAIL1 = " << TAIL1 << " HEAD2 = " << HEAD2 << " TAIL2 = " << TAIL2 << endl;
            }
            else {
                --num1;
                Array[RUN].setStatus(1);
                Array[RUN].setPriority(Array[RUN].getPriority()-3);
                if (num2==0) {
                    HEAD1 = TAIL1 = RUN;
                    Array[TAIL1].setNext(0);
                }
                else {
                    Array[TAIL1].setNext(RUN+1);
                    TAIL1 = RUN;
                    Array[TAIL1].setNext(0);
                }
                ++num2;
                Display(Array, "PB", numall, RUN, HEAD, TAIL, HEAD1, TAIL1, HEAD2, TAIL2);
                cout << "一级队列任务放入二级队列" << endl;
                cout << "HEAD = " << HEAD << " TAIL = " << TAIL << " HEAD1 = " << HEAD1 << " TAIL1 = " << TAIL1 << " HEAD2 = " << HEAD2 << " TAIL2 = " << TAIL2 << endl;
            }
        }
        else if (HEAD1 != -1) {
            RUN = HEAD1;
            Array[RUN].setStatus(0);
            HEAD1 = Array[HEAD1].getNext() - 1;
            Array[RUN].setCPUtime(Array[RUN].getCPUtime() + 4);
            Array[RUN].SetAlltime(Array[RUN].getAlltime() - 4);
            if (Array[RUN].getAlltime() <= 0) {
                Array[RUN].setStatus(2);
                Array[RUN].setCPUtime(Array[RUN].getCPUtime() + Array[RUN].getAlltime());
                Array[RUN].SetAlltime(0);
                --num2;
                --num;
                Display(Array, "PB", numall, RUN, HEAD, TAIL, HEAD1, TAIL1, HEAD2, TAIL2);
                cout << "二级队列任务结束" << endl;
                cout << "HEAD = " << HEAD << " TAIL = " << TAIL << " HEAD1 = " << HEAD1 << " TAIL1 = " << TAIL1 << " HEAD2 = " << HEAD2 << " TAIL2 = " << TAIL2 << endl;
            }
            else {
                --num2;
                Array[RUN].setStatus(1);
                Array[RUN].setPriority(Array[RUN].getPriority() - 3);
                if (num3==0) {
                    HEAD2 = TAIL2 = RUN;
                    Array[TAIL2].setNext(0);
                }
                else {
                    Array[TAIL2].setNext(RUN+1);
                    TAIL2 = RUN;
                    Array[TAIL2].setNext(0);
                }
                ++num3;
                Display(Array, "PB", numall, RUN, HEAD, TAIL, HEAD1, TAIL1, HEAD2, TAIL2);
                cout << "二级队列任务放入三级队列" << endl;
                cout << "HEAD = " << HEAD << " TAIL = " << TAIL << " HEAD1 = " << HEAD1 << " TAIL1 = " << TAIL1 << " HEAD2 = " << HEAD2 << " TAIL2 = " << TAIL2 << endl;
            }
        }
        else if(HEAD2 != -1) {
            RUN = HEAD2;
            Array[RUN].setStatus(0);
            HEAD2 = Array[HEAD2].getNext() - 1;
            Array[RUN].setCPUtime(Array[RUN].getCPUtime() + 7);
            Array[RUN].SetAlltime(Array[RUN].getAlltime() - 7);
            if (Array[RUN].getAlltime() <= 0) {
                Array[RUN].setStatus(2);
                Array[RUN].setCPUtime(Array[RUN].getCPUtime() + Array[RUN].getAlltime());
                Array[RUN].SetAlltime(0);
                --num3;
                --num;
                Display(Array, "PB", numall, RUN, HEAD, TAIL, HEAD1, TAIL1, HEAD2, TAIL2);
                cout << "三级队列任务结束" << endl;
                cout << "HEAD = " << HEAD << " TAIL = " << TAIL << " HEAD1 = " << HEAD1 << " TAIL1 = " << TAIL1 << " HEAD2 = " << HEAD2 << " TAIL2 = " << TAIL2 << endl;
            }
            else {
                Array[RUN].setStatus(1);
                if (num3 == 0) {
                    HEAD2 = TAIL2 = RUN;
                    Array[TAIL2].setNext(0);
                }
                else {
                    Array[TAIL2].setNext(RUN+1);
                    TAIL2 = RUN;
                    Array[TAIL2].setNext(0);
                }
                Display(Array, "PB", numall, RUN, HEAD, TAIL, HEAD1, TAIL1, HEAD2, TAIL2);
                cout << "三级队列任务轮转" << endl;
                cout << "HEAD = " << HEAD << " TAIL = " << TAIL << " HEAD1 = " << HEAD1 << " TAIL1 = " << TAIL1 << " HEAD2 = " << HEAD2 << " TAIL2 = " << TAIL2 << endl;
            }
        }
        ++addtime;
    }
}

void Display(PCB* Array, string mode, int num, int RUN, int HEAD, int TAIL, int HEAD1, int TAIL1, int HEAD2, int TAIL2) {
    cout << "OUTPUT OF " << mode << endl;
    cout << "==================================================" << endl;
    cout << "RUNNING PROC.\t\t\tWAITING QUEUE" << endl;
    cout << Array[RUN].getID()<<"\t\t";
    while (HEAD != -1) {
        cout << Array[HEAD].getID() << "\t";
        HEAD = Array[HEAD].getNext() - 1;
    }
    cout << endl;
    if (mode == "PB") {
        cout << "\t\t";
        while (HEAD1 != -1) {
            cout << Array[HEAD1].getID() << "\t";
            HEAD1 = Array[HEAD1].getNext() - 1;
        }
        cout << endl;
        cout << "\t\t";
        while (HEAD2 != -1) {
            cout << Array[HEAD2].getID() << "\t";
            HEAD2 = Array[HEAD2].getNext() - 1;
        }
        cout << endl;
    }
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
    int HEAD=-1;
    int TAIL = -1;
    int RUN = -1;
    string Prio = "Priority";
    string Roun = "Round_Robin";
    string PB = "PB";
    string mode;
    cout << "TYPE THE ALGORITHM:";
    while (cin >> mode) {
        PCB* Array = new PCB[6]();
        HEAD = CreatProcess(Array, 5, mode);
        for (int i = 0; i < 5; i++) {
            if (Array[i].getNext() == 0) TAIL = Array[i].getID() - 1;
        }
        if (mode == Prio) {
            PriorityProcess(HEAD, TAIL, RUN, Array);
        }
        else if (mode == Roun) {
            RoundRobinProcess(HEAD, TAIL, RUN, Array);
        }
        else if (mode == PB) {
            PBProcess(HEAD, TAIL, RUN, Array, 5);
        }
        cout << "\nTYPE THE ALGORITHM:";
    }
}

int random(double start, double end) {
    return (int)(start + (end - start) * rand() / (RAND_MAX + 1.0));
}

