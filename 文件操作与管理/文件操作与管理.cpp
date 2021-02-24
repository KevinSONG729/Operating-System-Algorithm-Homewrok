#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#define MAX_USER_NUM 10
#define MAX_OPEN_NUM 5

using namespace std;

struct MFD_Entry {
	string UserName;				// 用户名。
	struct UFD_Entry* fp{};		   // UFD 的指针——用于来模拟目录指针。
};

struct UFD_Entry {
	string FileName;				// 文件名。
	int length{};				   // 文件长度。
	string fileContent;           // 文件内容。
	bool protectCode[3]{};	     // 文件保护码。
	bool isWork{};              // 工作状态判定。
};

class FileManage
{
private:
	MFD_Entry MFD[MAX_USER_NUM];				// MFD 数组，用于存储用户名
	UFD_Entry* UFD{};						   // UFD 指针
	UFD_Entry* openFiles[MAX_OPEN_NUM]{};	  // 打开文件后的，文件指针。（这里简化，直接用 UFD 来模拟）
	int MFD_Length{};						 // MFD 目录使用的数量
	int Openfile_Length{};					// 打开文件的数量
	bool isLogin{};						   // 标记用户登陆状态

public:
	void init();							// 初始化函数
	void help();						   // 帮助函数

	void adduser();							// 添加用户函数
	void deleteuser();					   // 删除用户函数

	void login();							// 用户登陆函数
	void logout();						   // 用户登出函数

	void createfile();						// 新建文件函数
	void deletefile();					   // 删除文件函数

	void openfile();						// 打开文件函数
	void closefile();					   // 关闭文件函数

	void readfile();						// 读文件函数
	void writefile();					   // 写文件函数
	void changeprotectCode();			  // 修改保护码
	void changeFileName();               // 修改文件名

	void lsfunc();						   // 查看文件

	void BYE();                            // 显示MFD并退出

};

char const* commands_str[] = {
	"LOGIN",
	"LOGOUT",
	"ADDUSER",
	"DELETEUSER",
	"CREATE",
	"DELETE",
	"OPEN",
	"CLOSE",
	"READ",
	"WRITE",
	"LS",
	"CHANGEPROTECT",
	"CHANGENAME",
	"HELP",
	"BYE"
};

//函数指针数组
void (FileManage::* commands_func[])() = {
	&FileManage::login,
	&FileManage::logout,
	&FileManage::adduser,
	&FileManage::deleteuser,
	&FileManage::createfile,
	&FileManage::deletefile,
	&FileManage::openfile,
	&FileManage::closefile,
	&FileManage::readfile,
	&FileManage::writefile,
	&FileManage::lsfunc,
	&FileManage::changeprotectCode,
	&FileManage::changeFileName,
	&FileManage::help,
	&FileManage::BYE
};

void FileManage::init()
{
	MFD_Length = 0;
	Openfile_Length = 0;
	UFD = nullptr;
	isLogin = false;
	for (auto& i : MFD)
	{
		i.UserName = "";
		i.fp = nullptr;
	}

	for (auto& openFile : openFiles)
	{
		openFile = nullptr;
	}
}

void FileManage::help()
{
	cout << "THESE SHELL COMMANDS ARE DEFINED BELOWS." << endl;
	cout << "CREATE, DELETE, OPEN, CLOSE, READ, WRITE, ADDUSER, DELETEUSER, LS, CHANGEPROTECT, CHANGENAME, LOGOUT, BYE" << endl;
}

void FileManage::adduser()
{
	if (isLogin)
	{
		cout << "ILLEGAL OPERATION! PLEASE LOGOUT!" << endl;
		return;
	}
	string name;
	cout << "PLEASE INPUT THE USERNAME: ";
	cin >> name;

	if (name == "BACK") {
		return;
	}

	// 用于判断是否重名的标志
	bool flag = true;

	// 用户没有到达上限
	if (MFD_Length >= 0 && MFD_Length <= 10)
	{
		// 遍历，判断用户名是否重名
		for (auto& i : MFD)
		{
			if (i.UserName == name)
			{
				flag = false;
				cout << "THE USERNAME HAS ALREADY EXISTS." << endl;
				break;
			}
		}

		if (flag)
		{
			// 创建新的用户
			for (auto& i : MFD)
			{
				if (i.UserName.empty())
				{
					i.UserName = name;
					// 创建，并初始化
					UFD_Entry* temp;
					temp = new UFD_Entry[11];
					for (int j = 0; j < MAX_USER_NUM; j++)
					{
						temp[j].FileName = "*********";
						temp[j].length = 0;
						temp[j].isWork = false;
						for (bool& k : temp[j].protectCode)
						{
							k = false;
						}
					}
					i.fp = temp;
					cout << "USER CREATED." << endl;
					MFD_Length++;
					break;
				}
			}
		}
	}
}

void FileManage::deleteuser()
{
	// 在用户登陆后删除用户操作，不合法，应该禁止
	if (isLogin)
	{
		cout << "ILLEGAL OPERATION! PLEASE LOGOUT!" << endl;
		return;
	}
	string name;
	cout << "PLEASE INPUT THE USERNAME: ";
	cin >> name;

	if (name == "BACK") {
		return;
	}

	// 用户判断
	if (MFD_Length > 0 && MFD_Length <= 10)
	{
		// 遍历，寻找指定用户
		for (auto& i : MFD)
		{
			if (i.UserName == name)
			{
				i.UserName = "";
				UFD_Entry* temp = i.fp;
				delete[] temp;
				i.fp = nullptr;
				MFD_Length--;
				cout << "DELETED." << endl;
				break;
			}
		}
	}
}

void FileManage::login()
{
	if (isLogin)
	{
		cout << "ILLEGAL OPERATION! PLEASE DON'T LOGIN AGAIN!" << endl;
		return;
	}

	string inBuf, temp;
	temp = "UNSUCCESSFULLY！THE USERNAMSE DOES NOT EXIST！";
	cout << "YOUR NAME ? ";
	cin >> inBuf;

	if (inBuf == "BACK") {
		return;
	}

	// 在 MFD 中查询用户
	for (auto& i : MFD)
	{
		if (i.UserName == inBuf)
		{
			// 获取用户目录的目录指针
			UFD = i.fp;
			isLogin = true;
			temp = "SUCCESSFULLY";
			break;
		}
	}
	cout << "LOGIN " << temp << endl;
}

void FileManage::logout()
{
	if (!isLogin)
	{
		cout << "ILLEGAL OPERATION! PLEASE LOGIN!" << endl;
		return;
	}
	UFD = nullptr;
	isLogin = false;
	cout << "LOGOUT SUCCESSFULLY." << endl;
}

void FileManage::createfile()
{
	if (!isLogin)
	{
		cout << "ILLEGAL OPERATION! PLEASE LOGIN!" << endl;
		return;
	}

	string inBuf;
	bool flag;
	cout << "THE NEW FILE'S NAME(LESS THAN 9 CHARS)?  ";
	cin >> inBuf;

	if (inBuf == "BACK") {
		return;
	}

	if (UFD[0].length >= 0 && UFD[0].length <= 10)
	{
		// 判断重名
		for (int i = 1; i <= MAX_USER_NUM; i++)
		{
			flag = false;
			if (UFD[i].FileName == inBuf)
			{
				cout << "THE FILENAME HAS ALREADY EXISTS." << endl;
				break;
			}
			flag = true;
		}

		if (flag == 1)
		{
			for (int i = 1; i <= MAX_USER_NUM; i++)
			{
				// 找到空闲区域，创建文件
				if (UFD[i].FileName == "*********")
				{
					// 辨别系统保留字符串
					if (inBuf == "*********")
					{
						cout << "PLEASE CHANGE THE FILENAME." << endl;
						return;
					}
					UFD[i].FileName = inBuf;

					cout << "THE NEW FILE’S PROTECTION CODE? ";
					cin >> inBuf;
					if (inBuf == "BACK") {
						return;
					}
					for (int j = 0; j < 3; j++)
					{
						if (inBuf[j] == '0')
						{
							UFD[i].protectCode[j] = false;
						}
						else {
							UFD[i].protectCode[j] = true;
						}
					}
					UFD[0].length++;
					cout << "THE NEW FILE IS CREATED." << endl;
					break;
				}
			}

		}

	}
	else {
		cout << "NO FREE SPACE." << endl;
	}
}

void FileManage::deletefile()
{
	if (!isLogin)
	{
		cout << "ILLEGAL OPERATION! PLEASE LOGIN!" << endl;
		return;
	}
	string inBuf, temp;
	temp = "THE FILE NOT FOUND!";
	cout << "PLEASE INPUT THE FILE'S NAME: ";
	cin >> inBuf;

	if (inBuf == "BACK") {
		return;
	}

	// 用户判断
	if (MFD_Length > 0 && MFD_Length <= 10)
	{
		// 遍历，寻找指定文件
		for (int i = 1; i <= MAX_USER_NUM; i++)
		{
			if (UFD[i].FileName == inBuf)
			{
				// 检查文件是否占用
				if (UFD[i].isWork)
				{
					cout << "THIS FILE IS OCCUPIED AND CANNOT BE DELETED. " << endl;
					return;
				}

				UFD[i].FileName = "*********";
				UFD[i].length = 0;
				UFD[0].length--;

				temp = "DELETED FILE SUCCESSFULLY.";
				break;
			}
		}
		cout << temp << endl;
	}
	else {
		cout << "ERROR." << endl;
	}
}

void FileManage::openfile()
{
	// 用户登陆后，才能进行操作。
	if (!isLogin)
	{
		cout << "ILLEGAL OPERATION! PLEASE LOGIN!" << endl;
		return;
	}
	string inBuf;
	bool isFind = false;
	cout << "PLEAE INPUT THE FILE'S NAME: ";
	cin >> inBuf;

	if (inBuf == "BACK") {
		return;
	}

	if (UFD[0].length > 0 && UFD[0].length <= 10)
	{
		// 遍历，寻找指定文件
		for (int i = 1; i <= MAX_USER_NUM; i++)
		{
			if (UFD[i].FileName == inBuf)
			{
				if (UFD[i].isWork)
				{
					cout << "THE FILE HAS OPENED." << endl;
					return;
				}
				// 判断打开文件是否已经到达上限
				if (Openfile_Length >= 0 && Openfile_Length < MAX_OPEN_NUM)
				{
					for (int j = 0; j < MAX_OPEN_NUM; j++)
					{
						// 寻找空文件指针
						if (openFiles[j] == nullptr)
						{
							UFD[i].isWork = true;
							// 获取文件指针
							openFiles[j] = &UFD[i];
							cout << "THE FILE IS OPENED.IT'S OPEN NUM IS " << j << "." << endl;
							isFind = true;
							break;
						}
					}

					if (isFind)
					{
						break;
					}
				}
				else {
					cout << "OPEN FILES ERROR." << endl;
				}
			}
		}
		if (!isFind)
		{
			cout << "THE FILE HAS NOT FOUND." << endl;
		}
	}
	else
	{
		cout << "ERROR." << endl;
	}
}

void FileManage::closefile()
{
	if (!isLogin)
	{
		cout << "ILLEGAL OPERATION! PLEASE LOGIN!" << endl;
		return;
	}
	int inBuf;
	string inBuf1;
	cout << "PLEASE INPUT THE FILE'S OPEN NUM: ";
	cin >> inBuf1;

	if (inBuf1 == "BACK") {
		return;
	}
	else if (inBuf1 >= string{ '0' } && inBuf1 < string{ "5" }) {
		inBuf = *(inBuf1.data()) - '0';
	}
	else {
		cout << "INPUT IS INVALID." << endl;
		return;
	}

	// open num 判断有效性，避免指针误操作
	if (inBuf >= 0 && inBuf < 5)
	{
		// 解除文件占用
		openFiles[inBuf]->isWork = false;
		openFiles[inBuf] = nullptr;
		cout << "CLOSED." << endl;
	}
}

void FileManage::readfile()
{
	if (!isLogin)
	{
		cout << "ILLEGAL OPERATION! PLEASE LOGIN!" << endl;
		return;
	}

	int inBuf;
	string inBuf1;
	cout << "PLEASE INPUT THE FILE'S OPEN NUM: ";
	cin >> inBuf1;

	if (inBuf1 == "BACK") {
		return;
	}
	else if (inBuf1 >= string{ '0' } && inBuf1 < string{ '5' }) {
		inBuf = *(inBuf1.data()) - '0';
	}
	else {
		cout << "INPUT IS INVALID." << endl;
		return;
	}

	// open num 判断有效性，避免指针误操作
	if (inBuf >= 0 && inBuf < 5)
	{
		if (openFiles[inBuf] != nullptr)
		{
			if (openFiles[inBuf]->protectCode[1] == 1)
			{
				cout << "THE FILE LENGTH IS " << openFiles[inBuf]->length << endl;
			}
			else
			{
				cout << "THE FILE IS UNREADABLE!" << endl;
			}
		}
		else
		{
			cout << "FILE POINTER IS INVALID!" << endl;
		}
	}
}

void FileManage::writefile()
{
	if (!isLogin)
	{
		cout << "ILLEGAL OPERATION! PLEASE LOGIN!" << endl;
		return;
	}

	int inBuf1, inBuf2;
	string inBuf3;
	cout << "PLEASE INPUT THE FILE'S OPEN NUM: ";
	cin >> inBuf3;

	if (inBuf3 == "BACK") {
		return;
	}
	else if (inBuf3 >= string{ '0' } && inBuf3 < string{ '5' }) {
		inBuf1 = *(inBuf3.data()) - '0';
	}
	else {
		cout << "INPUT IS INVALID." << endl;
		return;
	}

	// open num 判断有效性，避免指针误操作
	if (inBuf1 >= 0 && inBuf1 < 5)
	{
		if (openFiles[inBuf1] != nullptr)
		{
			if (openFiles[inBuf1]->protectCode[0] == 1)
			{
				cout << "PLESE INPUT THE LENGTH: ";
				cin >> inBuf3;
				if (inBuf3 == "BACK") {
					return;
				}
				else if (inBuf3 >= string{ '0' } && inBuf3 < string{ '5' }) {
					inBuf2 = *(inBuf3.data()) - '0';
				}
				else {
					cout << "INPUT IS INVALID." << endl;
					return;
				}
				if (inBuf2 >= 0)
				{
					openFiles[inBuf1]->length = inBuf2;
				}
				else
				{
					cout << "INPUT ERROR!" << endl;
				}
			}
			else
			{
				cout << "THE FILE IS UNWRITABLE!" << endl;
			}
		}
		else
		{
			cout << "FILE POINTER IS INVALID!" << endl;
		}
	}
}

void FileManage::changeprotectCode()
{
	if (!isLogin)
	{
		cout << "ILLEGAL OPERATION! PLEASE LOGIN!" << endl;
		return;
	}

	string inBuf;
	bool isfind = false;
	cout << "PLEASE INPUT THE FILE'S NAME: ";
	cin >> inBuf;

	if (inBuf == "BACK") {
		return;
	}

	// 用户判断
	if (MFD_Length > 0 && MFD_Length <= 10)
	{
		// 遍历，寻找指定文件
		for (int i = 1; i <= MAX_USER_NUM; i++)
		{
			if (UFD[i].FileName == inBuf)
			{
				isfind = true;

				// 判断更改的文件是否被占用
				if (UFD[i].isWork)
				{
					cout << "THIS FILE IS OCCUPIED AND THE PROTECTCODE CANNOT BE CHANGE!" << endl;
					return;
				}

				cout << "ENTER THE OPEN MODE: ";
				cin >> inBuf;
				if (inBuf == "BACK") {
					return;
				}
				for (int j = 0; j < 3; j++)
				{
					if (inBuf[j] == '0')
					{
						UFD[i].protectCode[j] = false;
					}
					else {
						UFD[i].protectCode[j] = true;
					}
				}
				cout << "IT CHANGED!" << endl;
				break;
			}
		}
		if (!isfind)
		{
			cout << "THE FILE CAN'T FIND!" << endl;
		}
	}
	else {
		cout << "ERROR!" << endl;
	}
}

void FileManage::changeFileName()
{
	if (!isLogin)
	{
		cout << "ILLEGAL OPERATION! PLEASE LOGIN!" << endl;
		return;
	}

	string inBuf;
	bool isfind = false;
	bool isovername = false;
	cout << "PLEASE INPUT THE FILE'S NAME: ";
	cin >> inBuf;

	if (inBuf == "BACK") {
		return;
	}

	// 用户判断
	if (MFD_Length > 0 && MFD_Length <= 10)
	{
		// 遍历，寻找指定文件
		for (int i = 1; i <= MAX_USER_NUM; i++)
		{
			if (UFD[i].FileName == inBuf)
			{
				isfind = true;

				// 判断更改的文件是否被占用
				if (UFD[i].isWork)
				{
					cout << "THIS FILE IS OCCUPIED AND THE PROTECTCODE CANNOT BE CHANGE!" << endl;
					return;
				}

				cout << "ENTER THE NEW FILE NAME: ";
				cin >> inBuf;
				if (inBuf == "BACK") {
					return;
				}
				for (int p = 1; p < MAX_USER_NUM; p++) {
					if (UFD[p].FileName == inBuf) {
						cout << "THE FILENAME HAS ALREADY EXISTS." << endl;
						isovername = true;
						break;
					}
				}
				if (!isovername) {
					UFD[i].FileName = inBuf;
					cout << "IT CHANGED!" << endl;
				}
				break;
			}
		}
		if (!isfind)
		{
			cout << "THE FILE CAN'T FIND!" << endl;
		}
	}
	else {
		cout << "ERROR!" << endl;
	}
}

void FileManage::lsfunc()					
{
	if (!isLogin)
	{
		cout << "ILLEGAL OPERATION! PLEASE LOGIN!" << endl;
		return;
	}

	if (UFD[0].length == 0)
	{
		cout << "NOTHING!" << endl;
		return;
	}
	else
	{
		cout << "FILENAME\t\tPROTECTION\t\tCODE LENGTH" << endl;
	}
	for (int i = 0; i < MAX_USER_NUM; i++)
	{
		if (UFD[i].FileName != "*********")
		{
			cout << UFD[i].FileName << "\t\t\t\t" << UFD[i].protectCode[0] << UFD[i].protectCode[1] << UFD[i].protectCode[2] << "\t\t\t" << UFD[i].length << endl;
		}
	}
}

void FileManage::BYE()                          
{
	if (!isLogin)
	{
		cout << "CURRENT LOGIN USERNAME IS UNKNOWN" << endl;
	}
	else {
		string username;
		for (auto& i : MFD) {
			if (i.fp == UFD) {
				username = i.UserName;
			}
		}
		cout << "CURRENT LOGIN USERNAME IS " << username << endl;
	}
	cout << "ALL USER LIST BELOWS:" << endl;
	for (auto& i : MFD) {
		if (i.UserName != "") {
			cout << i.UserName << "\t\t"<<i.fp[0].length<<" FILES EXIST."<<endl;
		}
	}
	cout << "BYE." << endl;
	exit(0);
}

int main()
{
	FileManage filemanage;
	filemanage.init();
	string commandBuf;
	while (true)
	{
		cout << "COMMAND NAME?";
		cin >> commandBuf;

		int len = (sizeof(commands_str) / sizeof(commands_str[0])); //求字符串数组长度
		bool isFind = false;

		for (int i = 0; i < len; i++)
		{
			if (!strcmp(commandBuf.data(), commands_str[i]))
			{
				(filemanage.*commands_func[i])();
				isFind = true;
				break;
			}
		}

		if (!isFind) // 未找到命令
		{
			cout << "COMMAND NAME GIVEN IS WRONG!" << endl;
			cout << "IT SHOULD BE ONE OF FOLLOWING :CREATE, DELETE, OPEN, CLOSE, READ, WRITE, ADDUSER, DELETEUSER, LS, CHANGEPROTECT, CHANGENAME, LOGOUT, BYE" << endl;
			continue;
		}
	}
	return 0;
}
