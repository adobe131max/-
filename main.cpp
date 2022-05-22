#include <iostream>
#include <string>
#include <fstream>
#define FILENAME "Airline.txt"
#define PASFILE "Passenger.txt"
using namespace std;

// ������Ʊϵͳ -- by ���� - 2021302111432

int airline_num = 0; //��������

void show_Menu()
{
	cout << "=================================��ӭʹ�ú�����Ʊϵͳ===============================" << endl;
	cout << "*                                0.�˳�ϵͳ                                        *" << endl;
	cout << "*                                1.�����ʼ��                                      *" << endl;
	cout << "*                                2.�鿴��ǰ������Ϣ                                *" << endl;
	cout << "*                                3.���ݺ���Ų鿴�˿�����                          *" << endl;
	cout << "*                                4.���Һ���                                        *" << endl;
	cout << "*                                5.��Ʊ                                            *" << endl;
	cout << "*                                6.��Ʊ                                            *" << endl;
	cout << "*                                7.���Ӻ���                                        *" << endl;
	cout << "*                                8.ȡ������                                        *" << endl;
	cout << "*                                9.�޸ĺ���                                        *" << endl;
	cout << "====================================================================================" << endl;
}

//�����˿Ͷ���
class Passenger
{
public:

	string name;
	string id;
	int pos;
	Passenger* next = NULL;
};

//��Airline���󴴽��ڵ�
class Airline
{
public:

	string num;			 //������
	string terminal;	 //�յ�վ
	Passenger passenger; //�˿�
	int all_ticket;		 //����λ��
	int left_ticket;	 //ʣ��Ʊ��
	int count = 1;		 //��λ����
	Airline* next = NULL;
	Passenger* headp = NULL;    //�����˿�����

	void addPList(Airline* head, Airline* now);  //���ӳ˿�

	void del_Passenger(Airline* head, string in_num, string in_id); //ɾ���˿� -- ��Ʊ

	void show_Passenger(Airline* now); //�鿴�˿�
};

Airline* find_airline(Airline* head, string in_num);

int get_pnum(string air_num);

//��ȡ�ļ��е�����
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

	while (ifs >> pnum && ifs >> name && ifs >> id && ifs >> pos) //while�����ǲ���ֻ�ܽ���һ��?
	{
		Airline* pre = head;
		Passenger* pp = new Passenger;

		for (int i = 0; i < airline_num; i++)
		{
			Airline* p = pre->next;
			Passenger* prep = p->headp;
			while (prep->next != NULL) //��ʱprep�������һ���˿�
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

//ͳ�ƺ�������
int get_num()
{
	ifstream ifs;
	ifs.open(FILENAME, ios::in); //���ļ� ���ļ�

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

//ͳ��һ�������е�����
int get_pnum(string air_num)
{
	ifstream ifs;
	ifs.open(PASFILE, ios::in); //���ļ� ���ļ�

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
	ifs.open(FILENAME, ios::in); //���ļ�

	//1.�ļ�������
	if (!ifs.is_open())
	{
		ifs.close();
		return;
	}

	//2.�ļ����� ����Ϊ��
	char ch;
	ifs >> ch;
	if (ifs.eof())
	{
		ifs.close();
		return;
	}

	//3.���ļ����� ���Ҽ�¼����
	airline_num = get_num();
	head = readList();
	readPList(head);
}

//����
void save(Airline* head)
{
	ofstream ofs;
	ofs.open(FILENAME, ios::out); //������ķ�ʽ���ļ� -- д�ļ�

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

	//�ر��ļ�
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

//��ӳ˿�
void Airline::addPList(Airline* head, Airline* now)
{
	Passenger* pre = now->headp;
	for (int i = 0; i < all_ticket - left_ticket; i++)
	{
		pre = pre->next;
	}
	Passenger* newNode = new Passenger;
	cout << "����������������\n";
	cin >> newNode->name;
	cout << "����������֤���ţ�\n";
	cin >> newNode->id;

	newNode->next = NULL;
	pre->next = newNode;
	left_ticket--;
	cout << "��Ʊ�ɹ���" << endl;
	cout << "������λ���ǣ�" << now->count << endl;
	newNode->pos = now->count;
	now->count++;

	save(head);
	saveP(head);
}

//ɾ���˿� -- ��Ʊ
void Airline::del_Passenger(Airline* head, string in_num, string in_id)
{
	Passenger* pre = headp;
	for (int i = 0; i < all_ticket - left_ticket && pre->next->id != in_id; i++)
	{
		pre = pre->next;
	}
	if (pre->next == NULL)
	{
		cout << "�ó˿Ͳ����ڣ�" << endl;
	}
	else
	{
		left_ticket++;
		Passenger* p = pre->next; //p����Ҫɾ���Ľڵ�
		pre->next = pre->next->next;
		delete p;
		find_airline(head, in_num)->count--;
		cout << "ɾ���ɹ���" << endl;
	}

	save(head);
	saveP(head);
}

//�鿴�˿�
void Airline::show_Passenger(Airline* now)
{
	if (now->headp->next == NULL)
	{
		cout << "û�г˿ͣ�" << endl;
	}
	else
	{
		Passenger* p = now->headp->next;

		cout << "����\t" << "֤����\t" << "��λ��" << endl;
		while (p != NULL)
		{
			cout << p->name << "\t" << p->id << "\t" << p->pos << endl;
			p = p->next;
		}
	}
}

//�˳�ϵͳ
void ExitSystem()
{
	cout << "��ӭ�´�ʹ��" << endl;
	system("pause");
	exit(0);; //�˳�����
}

//����Airline����
Airline* creatList(int n)
{
	Airline* head = new Airline;
	Airline* pre = head;

	for (int i = 0; i < n; i++)
	{
		Airline* p = new Airline;
		cout << "�������" << i + 1 << "��������" << endl;
		cin >> p->num;
		cout << "�������" << i + 1 << "�������յ�վ" << endl;
		cin >> p->terminal;
		cout << "�������" << i + 1 << "����������λ��" << endl;
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

//�鿴��ǰ������Ϣ
void show_Airline(Airline* head)
{
	if (head == NULL)
	{
		cout << "��ǰû�к�����Ϣ��" << endl;
	}
	else
	{
		Airline* p = head->next;

		cout << "������\t�յ�վ\t\t��Ʊ��\t\t��Ʊ��" << endl;

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

//���ݺ���Ų��Һ���
void find_Airline1(Airline* head, string in_num)
{
	if (head != NULL)
	{
		Airline* p = head->next;

		int xxx = -1; //�ж��Ƿ��ҵ��ú���

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
			cout << "û���ҵ��ú��࣡" << endl;
		}
	}
	else
	{
		cout << "û���ҵ��ú��࣡" << endl;
	}
}

//�����յ�վ���Һ���
void find_Airline2(Airline* head, string in_terminal)
{
	if (head != NULL)
	{
		Airline* p = head->next;

		int xxx = -1; //�ж��Ƿ��ҵ��ú���

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
			cout << "û���ҵ��ú��࣡" << endl;
		}
	}
	else
	{
		cout << "û���ҵ��ú��࣡" << endl;
	}
}

//���Ӻ��� -- ���������һλ
void add_Airline(Airline* head, string in_num)
{
	Airline* pre = head;
	for (int i = 0; i < airline_num; i++)
	{
		pre = pre->next;
	}

	Airline* newNode = new Airline;
	cout << "�������º�����յ�վ��" << endl;
	cin >> newNode->terminal;
	cout << "�������º��������λ����" << endl;
	cin >> newNode->all_ticket;
	newNode->left_ticket = newNode->all_ticket;
	newNode->num = in_num;
	newNode->next = NULL;
	pre->next = newNode;
	save(head);
	saveP(head);
	cout << "��ӳɹ���" << endl;

	system("pause");
	system("cls");
}

//ɾ������
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
		cout << "�ú��಻���ڣ�" << endl;
	}
	else
	{
		Airline* p = pre->next; //p����Ҫɾ���Ľڵ�
		pre->next = pre->next->next;
		delete p;
		save(head);
		cout << "ɾ���ɹ���" << endl;
	}
	save(head);
	saveP(head);

	system("pause");
	system("cls");
}

//�ҵ������ظú���ĵ�ַ
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

//�޸ĺ���
void cha_Airline(Airline* head, string in_num)
{
	cout << "�������µ��յ�վ��" << endl;
	cin >> find_airline(head, in_num)->terminal;
	cout << "�������µ�����λ����" << endl;
	cin >> find_airline(head, in_num)->all_ticket;
	cout << "�������µ�ʣ��Ʊ����" << endl;
	cin >> find_airline(head, in_num)->left_ticket;
	save(head);
	saveP(head);
	cout << "�޸ĳɹ���" << endl;
}

int main()
{
	int choice = 0; //�����洢�û���ѡ��

	Airline* head = NULL;

	airline_num = get_num(); //��ȡ�ļ��еĺ�������

	read_Airline(airline_num, head); //��ȡ�ļ��е�����

	while (true)
	{
		//����չʾ�˵���Ա����
		show_Menu();

		cout << "����������ѡ��" << endl;

		cin >> choice; //�����û���ѡ��

		switch (choice)
		{
		case 0: //�˳�ϵͳ
			ExitSystem();
			break;
		case 1: //��ʼ������
		{
			int n = 0;
			cout << "�ò���������ԭ�ȵĺ��࣡\n";
			cout << "�����봴���ĺ���������" << endl;
			cin >> n;
			airline_num = n;
			head = creatList(n);
			save(head);
			break;
		}
		case 2: //��ʾ������Ϣ
			show_Airline(head);
			break;
		case 3: //���ݺ���Ų鿴�˿�����
		{
			string in_num;
			cout << "������Ҫ�鿴�ĺ����ţ�" << endl;
			cin >> in_num;

			if (find_airline(head, in_num) == NULL)
			{
				cout << "�ú��಻���ڣ�" << endl;
			}
			else
			{
				if (find_airline(head, in_num)->headp == NULL)
				{
					cout << "û�г˿�!" << endl;
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
		case 4: //���Һ���
		{
			cout << "1.���ݺ���Ų��Һ���" << endl;
			cout << "2.�����յ�վ���Һ���" << endl;
			int n = 0;
			cin >> n;
			switch (n)
			{
			case 1:
			{
				string in_num;
				cout << "������Ҫ���ҵĺ����ţ�" << endl;
				cin >> in_num;

				find_Airline1(head, in_num);
				break;
			}
			case 2:
			{
				string in_terminal;
				cout << "������Ҫ���ҵĺ����ţ�" << endl;
				cin >> in_terminal;

				find_Airline2(head, in_terminal);
				break;
			}
			default:
				cout << "����������" << endl;
				break;
			}
			system("pause");
			system("cls");
			break;
		}
		case 5: //��Ʊ
		{
			string in_num;
			cout << "������ѡ��ĺ����ţ�" << endl;
			cin >> in_num;
			if (find_airline(head, in_num) == NULL)
			{
				cout << "�ú��಻���ڣ�" << endl;
			}
			else
			{
				if (find_airline(head, in_num)->left_ticket > 0)
				{
					find_airline(head, in_num)->addPList(head, find_airline(head, in_num));
				}
				else
				{
					cout << "�˺���������" << endl;
					cout << "������\t\t" << "�յ�վ\t\t" << "��Ʊ��" << endl;
					cout << find_airline(head, in_num)->num << "\t\t"
						<< find_airline(head, in_num)->terminal << "\t\t"
						<< find_airline(head, in_num)->left_ticket << endl;
				}
			}
			system("pause");
			system("cls");
			break;
		}
		case 6: //��Ʊ
		{
			string in_num;
			cout << "������ѡ��ĺ����ţ�" << endl;
			cin >> in_num;
			if (find_airline(head, in_num) == NULL)
			{
				cout << "�ú��಻���ڣ�" << endl;
			}
			else
			{
				string in_id;
				cout << "�������������֤��" << endl;
				cin >> in_id;
				find_airline(head, in_num)->del_Passenger(head, in_num, in_id);

			}
			system("pause");
			system("cls");
			break;
		}
		case 7: //���Ӻ���
		{
			if (head != NULL)
			{
				string in_num;
				cout << "������Ҫ���ӵĺ����ţ�" << endl;
				cin >> in_num;

				add_Airline(head, in_num);
			}
			else
			{
				cout << "���ȳ�ʼ�����࣡" << endl;
				system("pause");
				system("cls");
			}
			break;
		}
		case 8: //ɾ������
		{
			if (head != NULL)
			{
				string in_num;
				cout << "������Ҫɾ���ĺ����ţ�" << endl;
				cin >> in_num;

				del_Airline(head, in_num);
			}
			else
			{
				cout << "���ȳ�ʼ�����࣡" << endl;
				system("pause");
				system("cls");
			}
			break;
		}
		case 9: //�޸ĺ���
		{
			if (head != NULL)
			{
				string in_num;
				cout << "������Ҫ�޸ĵĺ����ţ�" << endl;
				cin >> in_num;
				if (find_airline(head, in_num) == NULL)
				{
					cout << "�ú��಻���ڣ�" << endl;
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
				cout << "���ȳ�ʼ�����࣡" << endl;
				system("pause");
				system("cls");
			}
			break;
		}
		default:
			system("cls"); //����
			break;
		}
	}

	system("pause");

	return 0;
}