#include <iostream>
#include <string>
#include <fstream>
#define FILENAME "Airline.txt"
#define PASFILE "Passenger.txt"
using namespace std;

// 航班售票系统 -- by 龚今朝 - 2021302111432

int airline_num = 0; //航班总数

void show_Menu()
{
	cout << "=================================欢迎使用航班售票系统===============================" << endl;
	cout << "*                                0.退出系统                                        *" << endl;
	cout << "*                                1.航班初始化                                      *" << endl;
	cout << "*                                2.查看当前航班信息                                *" << endl;
	cout << "*                                3.根据航班号查看乘客名单                          *" << endl;
	cout << "*                                4.查找航班                                        *" << endl;
	cout << "*                                5.订票                                            *" << endl;
	cout << "*                                6.退票                                            *" << endl;
	cout << "*                                7.增加航班                                        *" << endl;
	cout << "*                                8.取消航班                                        *" << endl;
	cout << "*                                9.修改航班                                        *" << endl;
	cout << "====================================================================================" << endl;
}

//创建乘客对象
class Passenger
{
public:

	string name;
	string id;
	int pos;
	Passenger* next = NULL;
};

//用Airline对象创建节点
class Airline
{
public:

	string num;			 //航班编号
	string terminal;	 //终点站
	Passenger passenger; //乘客
	int all_ticket;		 //总座位数
	int left_ticket;	 //剩余票数
	int count = 1;		 //座位计数
	Airline* next = NULL;
	Passenger* headp = NULL;    //创建乘客链表

	void addPList(Airline* head, Airline* now);  //增加乘客

	void del_Passenger(Airline* head, string in_num, string in_id); //删除乘客 -- 退票

	void show_Passenger(Airline* now); //查看乘客
};

Airline* find_airline(Airline* head, string in_num);

int get_pnum(string air_num);

//读取文件中的数据
Airline* readList()
{
	Airline* head = new Airline;
	Airline* pre = head;

	ifstream ifs;
	ifs.open(FILENAME, ios::in);

	string num;
	string terminal;
	int all_ticket;
	int left_ticket;

	while (ifs >> num && ifs >> terminal && ifs >> all_ticket && ifs >> left_ticket)
	{
		Airline* p = new Airline;

		p->num = num;
		p->terminal = terminal;
		p->all_ticket = all_ticket;
		p->left_ticket = p->all_ticket - get_pnum(num);
		p->count = get_pnum(num) + 1;
		p->next = NULL;
		p->headp = new Passenger;
		p->headp->next = NULL;

		pre->next = p;
		pre = p;
	}

	return head;
}

void readPList(Airline* head)
{
	ifstream ifs;
	ifs.open(PASFILE, ios::in);

	string pnum;
	string name;
	string id;
	int pos;

	while (ifs >> pnum && ifs >> name && ifs >> id && ifs >> pos) //while整个是不是只能进行一次?
	{
		Airline* pre = head;
		Passenger* pp = new Passenger;

		for (int i = 0; i < airline_num; i++)
		{
			Airline* p = pre->next;
			Passenger* prep = p->headp;
			while (prep->next != NULL) //此时prep将是最后一个乘客
			{
				prep = prep->next;
			}
			pre = pre->next;

			if (p->num == pnum)
			{
				pp->name = name;
				pp->id = id;
				pp->pos = pos;
				prep->next = pp;
				prep = pp;
				pp->next = NULL;
			}
		}
	}
}

//统计航班数量
int get_num()
{
	ifstream ifs;
	ifs.open(FILENAME, ios::in); //打开文件 读文件

	string num;
	string terminal;
	int all_ticket;
	int left_ticket;

	int get_num = 0;

	while (ifs >> num && ifs >> terminal && ifs >> all_ticket && ifs >> left_ticket)
	{
		get_num++;
	}

	return get_num;
}

//统计一个航班中的人数
int get_pnum(string air_num)
{
	ifstream ifs;
	ifs.open(PASFILE, ios::in); //打开文件 读文件

	string num;
	string name;
	string id;
	int pos;

	int get_pnum = 0;

	while (ifs >> num && ifs >> name && ifs >> id && ifs >> pos)
	{
		if (air_num == num)
		{
			get_pnum++;
		}
	}

	return get_pnum;
}

void read_Airline(int& airline_num, Airline*& head)
{
	ifstream ifs;
	ifs.open(FILENAME, ios::in); //读文件

	//1.文件不存在
	if (!ifs.is_open())
	{
		ifs.close();
		return;
	}

	//2.文件存在 数据为空
	char ch;
	ifs >> ch;
	if (ifs.eof())
	{
		ifs.close();
		return;
	}

	//3.当文件存在 并且记录数据
	airline_num = get_num();
	head = readList();
	readPList(head);
}

//保存
void save(Airline* head)
{
	ofstream ofs;
	ofs.open(FILENAME, ios::out); //用输出的方式打开文件 -- 写文件

	//ofstream opas;
	//opas.open(PASFILE, ios::out);

	Airline* p = head->next;

	while (p != NULL)
	{
		ofs << p->num << " "
			<< p->terminal << " "
			<< p->all_ticket << " "
			<< p->left_ticket << endl;

		p = p->next;
	}

	//关闭文件
	ofs.close();
	//opas.close();
}

void saveP(Airline* head)
{
	ofstream ofs;
	ofs.open(PASFILE, ios::out);

	Airline* pre = head;

	for (int i = 0; i < airline_num; i++)
	{
		Airline* p = pre->next;
		Passenger* pp = p->headp->next;

		while (pp != NULL)
		{
			ofs << p->num << " "
				<< pp->name << " "
				<< pp->id << " "
				<< pp->pos << endl;
			pp = pp->next;
		}

		pre = pre->next;
	}

	ofs.close();
}

//添加乘客
void Airline::addPList(Airline* head, Airline* now)
{
	Passenger* pre = now->headp;
	for (int i = 0; i < all_ticket - left_ticket; i++)
	{
		pre = pre->next;
	}
	Passenger* newNode = new Passenger;
	cout << "请输入您的姓名：\n";
	cin >> newNode->name;
	cout << "请输入您的证件号：\n";
	cin >> newNode->id;

	newNode->next = NULL;
	pre->next = newNode;
	left_ticket--;
	cout << "订票成功！" << endl;
	cout << "您的座位号是：" << now->count << endl;
	newNode->pos = now->count;
	now->count++;

	save(head);
	saveP(head);
}

//删除乘客 -- 退票
void Airline::del_Passenger(Airline* head, string in_num, string in_id)
{
	Passenger* pre = headp;
	for (int i = 0; i < all_ticket - left_ticket && pre->next->id != in_id; i++)
	{
		pre = pre->next;
	}
	if (pre->next == NULL)
	{
		cout << "该乘客不存在！" << endl;
	}
	else
	{
		left_ticket++;
		Passenger* p = pre->next; //p就是要删除的节点
		pre->next = pre->next->next;
		delete p;
		find_airline(head, in_num)->count--;
		cout << "删除成功！" << endl;
	}

	save(head);
	saveP(head);
}

//查看乘客
void Airline::show_Passenger(Airline* now)
{
	if (now->headp->next == NULL)
	{
		cout << "没有乘客！" << endl;
	}
	else
	{
		Passenger* p = now->headp->next;

		cout << "姓名\t" << "证件号\t" << "座位号" << endl;
		while (p != NULL)
		{
			cout << p->name << "\t" << p->id << "\t" << p->pos << endl;
			p = p->next;
		}
	}
}

//退出系统
void ExitSystem()
{
	cout << "欢迎下次使用" << endl;
	system("pause");
	exit(0);; //退出程序
}

//创建Airline链表
Airline* creatList(int n)
{
	Airline* head = new Airline;
	Airline* pre = head;

	for (int i = 0; i < n; i++)
	{
		Airline* p = new Airline;
		cout << "请输入第" << i + 1 << "个航班编号" << endl;
		cin >> p->num;
		cout << "请输入第" << i + 1 << "个航班终点站" << endl;
		cin >> p->terminal;
		cout << "请输入第" << i + 1 << "个航班总座位数" << endl;
		cin >> p->all_ticket;
		p->left_ticket = p->all_ticket;

		p->headp = new Passenger;
		p->headp->next = NULL;
		pre->next = p;
		pre = p;
		p->next = NULL;
	}

	system("pause");
	system("cls");

	return head;
}

//查看当前航班信息
void show_Airline(Airline* head)
{
	if (head == NULL)
	{
		cout << "当前没有航班信息！" << endl;
	}
	else
	{
		Airline* p = head->next;

		cout << "航班编号\t终点站\t\t总票数\t\t余票额" << endl;

		while (p != NULL)
		{
			cout << p->num << "\t\t" << p->terminal << "\t\t" << p->all_ticket << "\t\t" << p->left_ticket << endl;
			p = p->next;
		}
	}
	save(head);

	system("pause");
	system("cls");
}

//根据航班号查找航班
void find_Airline1(Airline* head, string in_num)
{
	if (head != NULL)
	{
		Airline* p = head->next;

		int xxx = -1; //判断是否找到该航班

		while (p != NULL)
		{
			if (p->num == in_num)
			{
				cout << p->num << "\t\t" << p->terminal << "\t\t" << p->all_ticket << "\t\t" << p->left_ticket << endl;
				xxx = 1;
			}
			p = p->next;
		}

		if (xxx == -1)
		{
			cout << "没有找到该航班！" << endl;
		}
	}
	else
	{
		cout << "没有找到该航班！" << endl;
	}
}

//根据终点站查找航班
void find_Airline2(Airline* head, string in_terminal)
{
	if (head != NULL)
	{
		Airline* p = head->next;

		int xxx = -1; //判断是否找到该航班

		while (p != NULL)
		{
			if (p->terminal == in_terminal)
			{
				cout << p->num << "\t\t" << p->terminal << "\t\t" << p->all_ticket << "\t\t" << p->left_ticket << endl;
				xxx = 1;
			}
			p = p->next;
		}

		if (xxx == -1)
		{
			cout << "没有找到该航班！" << endl;
		}
	}
	else
	{
		cout << "没有找到该航班！" << endl;
	}
}

//增加航班 -- 增加在最后一位
void add_Airline(Airline* head, string in_num)
{
	Airline* pre = head;
	for (int i = 0; i < airline_num; i++)
	{
		pre = pre->next;
	}

	Airline* newNode = new Airline;
	cout << "请输入新航班的终点站：" << endl;
	cin >> newNode->terminal;
	cout << "请输入新航班的总座位数：" << endl;
	cin >> newNode->all_ticket;
	newNode->left_ticket = newNode->all_ticket;
	newNode->num = in_num;
	newNode->next = NULL;
	pre->next = newNode;
	save(head);
	saveP(head);
	cout << "添加成功！" << endl;

	system("pause");
	system("cls");
}

//删除航班
void del_Airline(Airline* head, string in_num)
{
	Airline* pre = head;
	pre->num = "000";
	for (int i = 0; i < airline_num && pre->next->num != in_num; i++)
	{
		pre = pre->next;
	}
	if (pre->next == NULL)
	{
		cout << "该航班不存在！" << endl;
	}
	else
	{
		Airline* p = pre->next; //p就是要删除的节点
		pre->next = pre->next->next;
		delete p;
		save(head);
		cout << "删除成功！" << endl;
	}
	save(head);
	saveP(head);

	system("pause");
	system("cls");
}

//找到并返回该航班的地址
Airline* find_airline(Airline* head, string in_num)
{
	Airline* p = NULL;
	Airline* pre = head;

	for (int i = 0; i < airline_num; i++)
	{
		if (in_num == pre->next->num)
		{
			p = pre->next;
		}
		pre = pre->next;
	}

	return p;
}

//修改航班
void cha_Airline(Airline* head, string in_num)
{
	cout << "请输入新的终点站：" << endl;
	cin >> find_airline(head, in_num)->terminal;
	cout << "请输入新的总座位数：" << endl;
	cin >> find_airline(head, in_num)->all_ticket;
	cout << "请输入新的剩余票数：" << endl;
	cin >> find_airline(head, in_num)->left_ticket;
	save(head);
	saveP(head);
	cout << "修改成功！" << endl;
}

int main()
{
	int choice = 0; //用来存储用户的选项

	Airline* head = NULL;

	airline_num = get_num(); //读取文件中的航班数量

	read_Airline(airline_num, head); //读取文件中的数据

	while (true)
	{
		//调用展示菜单成员函数
		show_Menu();

		cout << "请输入您的选择：" << endl;

		cin >> choice; //接受用户的选项

		switch (choice)
		{
		case 0: //退出系统
			ExitSystem();
			break;
		case 1: //初始化航班
		{
			int n = 0;
			cout << "该操作会重置原先的航班！\n";
			cout << "请输入创建的航班数量：" << endl;
			cin >> n;
			airline_num = n;
			head = creatList(n);
			save(head);
			break;
		}
		case 2: //显示航班信息
			show_Airline(head);
			break;
		case 3: //根据航班号查看乘客名单
		{
			string in_num;
			cout << "请输入要查看的航班编号：" << endl;
			cin >> in_num;

			if (find_airline(head, in_num) == NULL)
			{
				cout << "该航班不存在！" << endl;
			}
			else
			{
				if (find_airline(head, in_num)->headp == NULL)
				{
					cout << "没有乘客!" << endl;
				}
				else
				{
					find_airline(head, in_num)->show_Passenger(find_airline(head, in_num));
				}
			}
			system("pause");
			system("cls");
			break;
		}
		case 4: //查找航班
		{
			cout << "1.根据航班号查找航班" << endl;
			cout << "2.根据终点站查找航班" << endl;
			int n = 0;
			cin >> n;
			switch (n)
			{
			case 1:
			{
				string in_num;
				cout << "请输入要查找的航班编号：" << endl;
				cin >> in_num;

				find_Airline1(head, in_num);
				break;
			}
			case 2:
			{
				string in_terminal;
				cout << "请输入要查找的航班编号：" << endl;
				cin >> in_terminal;

				find_Airline2(head, in_terminal);
				break;
			}
			default:
				cout << "请重新输入" << endl;
				break;
			}
			system("pause");
			system("cls");
			break;
		}
		case 5: //订票
		{
			string in_num;
			cout << "请输入选择的航班编号：" << endl;
			cin >> in_num;
			if (find_airline(head, in_num) == NULL)
			{
				cout << "该航班不存在！" << endl;
			}
			else
			{
				if (find_airline(head, in_num)->left_ticket > 0)
				{
					find_airline(head, in_num)->addPList(head, find_airline(head, in_num));
				}
				else
				{
					cout << "此航班已满！" << endl;
					cout << "航班编号\t\t" << "终点站\t\t" << "余票额" << endl;
					cout << find_airline(head, in_num)->num << "\t\t"
						<< find_airline(head, in_num)->terminal << "\t\t"
						<< find_airline(head, in_num)->left_ticket << endl;
				}
			}
			system("pause");
			system("cls");
			break;
		}
		case 6: //退票
		{
			string in_num;
			cout << "请输入选择的航班编号：" << endl;
			cin >> in_num;
			if (find_airline(head, in_num) == NULL)
			{
				cout << "该航班不存在！" << endl;
			}
			else
			{
				string in_id;
				cout << "请输入您的身份证号" << endl;
				cin >> in_id;
				find_airline(head, in_num)->del_Passenger(head, in_num, in_id);

			}
			system("pause");
			system("cls");
			break;
		}
		case 7: //增加航班
		{
			if (head != NULL)
			{
				string in_num;
				cout << "请输入要增加的航班编号：" << endl;
				cin >> in_num;

				add_Airline(head, in_num);
			}
			else
			{
				cout << "请先初始化航班！" << endl;
				system("pause");
				system("cls");
			}
			break;
		}
		case 8: //删除航班
		{
			if (head != NULL)
			{
				string in_num;
				cout << "请输入要删除的航班编号：" << endl;
				cin >> in_num;

				del_Airline(head, in_num);
			}
			else
			{
				cout << "请先初始化航班！" << endl;
				system("pause");
				system("cls");
			}
			break;
		}
		case 9: //修改航班
		{
			if (head != NULL)
			{
				string in_num;
				cout << "请输入要修改的航班编号：" << endl;
				cin >> in_num;
				if (find_airline(head, in_num) == NULL)
				{
					cout << "该航班不存在！" << endl;
				}
				else
				{
					cha_Airline(head, in_num);
				}
				system("pause");
				system("cls");
			}
			else
			{
				cout << "请先初始化航班！" << endl;
				system("pause");
				system("cls");
			}
			break;
		}
		default:
			system("cls"); //清屏
			break;
		}
	}

	system("pause");

	return 0;
}