#include <fstream>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;


struct Node // каждый чиновник
{
	int id;
	string name;
	Node* employer;
	vector <Node*> employees;
	double bribe;
	bool flag;
};

typedef Node* PNode; // укзатель на чиновника

bool numb_check(string unchecked) // проверяется, что строка состоит только из цифр
{
	char numbers[10] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
	int count = 0;
	for (int n = 0; n < unchecked.length(); n++)
	{
		for (int p = 0; p < 10; p++)
		{
			if (unchecked[n] == numbers[p])
				count++;
		}
	}
	if (count != unchecked.length())
	{
		return false;
	}
	else
		return true;

}

bool space_check(string victim) // проверка на то, что каждая строка соответствует ожидаемой
{
	int flag_count = 0;
	bool sub_flag = false;
	for (int j = 0; j < (victim.length() - 1); j++)
	{
		if (victim[j] != ' ')
			sub_flag = true;
		if ((victim[j] == ' ') and (victim[j + 1] != ' ') and (sub_flag == true))
		{
			flag_count += 1;
		}
	}
	if (flag_count != 3)
		return false;
	else
		return true;
}

// добавление чиновника в дерево с предварительной проверкой, которая включает все себя функцию выше
bool AddToTree(PNode& Tree, string entry, PNode* officials, int i) 
{
	stringstream init(entry);
	string id, name, boss, bribe;
	init >> id >> name >> boss >> bribe;
	char numbers[10] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
	int count = 0;
	
	if (numb_check(id)==false)
	{
		cout << "Ошибка в id, строка " << i + 2;
		return false;
	}

	if (i != (stoi(id) - 1))
	{
		cout << "Ошибка в id (неверный порядок чиновников), строка " << i + 2;
		return false;
	}
		

	if (numb_check(boss) == false)
	{
		cout << "Ошибка в id руководителя, строка " << i + 2;
		return false;
	}

	if ((officials[stoi(boss)-1]==nullptr) and (stoi(id)!=1))
	{
		cout << "Ошибка в id руководителя (такого руководителя нет), строка " << i + 2;
		return false;
	}
	
	if (numb_check(bribe) == false)
	{
		cout << "Ошибка в написании взятки, строка " << i + 2;
		return false;
	}


	if (stoi(id) == 1) // создание главного чиновника
	{
		Tree = new Node;
		Tree->id = stoi(id);
		Tree->name = name;
		Tree->employer = NULL;
		Tree->bribe = stoi(bribe);
		officials[stoi(id) - 1] = Tree;
	}
	else // остальные чиновники
	{
		PNode New = new Node;
		New->id = stoi(id);
		New->name = name;
		New->employer = officials[stoi(boss) - 1];
		officials[stoi(boss) - 1]->employees.push_back(New);
		New->bribe = stoi(bribe);
		officials[stoi(id) - 1] = New;
	}
	return true;



}

void officials_print(PNode Tree, int k) // вывод иерархии чиновников
{
	char symb = '-';
	if (Tree->employer == NULL)
	{
		cout << Tree->name << ' ' << Tree->bribe << endl;
	}
	else		
		cout << string(k * (4 + 1), symb) << Tree->name << ' ' << Tree->bribe << endl;
	for (int j = 0; j < Tree->employees.size(); j++)
	{
		officials_print(Tree->employees[j], k + 1);
	}
}

void minim_bribe(PNode Tree) // нахождение минимальной суммы взятки
{
	
	double minimum;
	int id;
	if (Tree->employees.empty() == false)
	{

		for (int i = 0; i < Tree->employees.size(); i++)
			minim_bribe(Tree->employees[i]);
		minimum = Tree->employees[0]->bribe;
		id = 0;

		for (int i = 0; i < Tree->employees.size(); i++)
		{
			if (minimum > Tree->employees[i]->bribe)
			{
				minimum = Tree->employees[i]->bribe;
				id = i;
			}
		}
		for (int i = 0; i < Tree->employees.size(); i++)
		{
			if (minimum == Tree->employees[i]->bribe)
			{
				Tree->employees[i]->flag = true;
				
			}
		}
		Tree->employees[id]->flag = true;
		Tree->bribe += minimum;
	}
	if (Tree->employer == NULL)
		cout <<endl<<"Минимальная сумма взятки: " << Tree->bribe<<endl<<endl;
	
}

void bribe_path(PNode Tree,string s) // вывод способов получения подписи
{
	string res = s;
	if (Tree->employees.empty() == false)
	{
		for (int i = 0; i < Tree->employees.size(); i++)
			if (Tree->employees[i]->flag == true)
				bribe_path(Tree->employees[i], res + " <--- " +Tree->employees[i]->name);
	}
	else
	{
		if (Tree->flag == true)
			cout << res<<endl;
	}
} 

void main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	cout << "Добро пожаловать в программу 'Чиновники'!\nФайл с входными находится в папке проекта!\n";
	cout << "\nФайл должен содержать:\n\n";
	cout<<"- количество чиновников\n- id чиновника, его имя, id руководителя и размер взятки каждого (если взятки нет, то написать 0)\n\n\n";
	ifstream f;
	f.open("input.txt");
	string off;
	bool check = false;
	if (getline(f, off))
	{
		if ((numb_check(off) == false) or (off==""))
		{
			cout << "Неверно введено количество чиновников\n\n";
			return;
		}
		
		
	}
	else
	{
		cout << "Файл пуст!";
		return;
		check = true;
	}
	int n = stoi(off);
	string* inputs = new string[n];
	int i = 1;
	while (getline(f, off))
	{
		
		if (off == "")
			check = true;

		else
		{
			if (space_check(off) == false)
				check = true;
		}

		if((i-1)<n)
			inputs[i-1] = off;
		if (i == n and check==false)
			break;
		if (i > n)
			check = true;
		i++;
	}

	if ((i) != n)
		check = true;
	
	
	if (check == true)
		cout << "Error";
	else
	{
		PNode* officials = new PNode[n]{ nullptr };

		PNode Head = NULL;
		PNode TempH = Head;

		bool fill = true;


		for (int i = 0; i < n; i++)
		{
			fill = AddToTree(Head, inputs[i], officials, i);
			if (i == 0)
			{
				TempH = Head;
			}
			if (fill == false)
				break;
		}

		if (fill == true)
		{
			cout << "Получившаяся иерархия министерства:\n\n";
			officials_print(Head, 0);
			cout << endl<<string(20, '-');
			cout << endl << endl;
			minim_bribe(Head);
			cout << "Возможный порядок получения подписей:\n\n";
			bribe_path(Head,Head->name);
			cout << "\n\n\n\n";
		}
		
	}


}
