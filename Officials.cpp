#include <fstream>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;


struct Node // структура каждого чиновника
{
	int id; // id чиновника
	string name; // фамилия чиновника
	Node* employer; // указатель на руководителя
	vector <Node*> employees; // вектор с подчиненными
	double bribe; // взятка
	bool flag; // переменная, которая показывает, что нужно дать этому человеку взятку
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
	// в каждой строке должно быть четыре значения, разделенные тремя пробелами
	int flag_count = 0;
	bool sub_flag = false;
	for (int j = 0; j < (victim.length() - 1); j++)
	{
		if (victim[j] != ' ') // первый символ не равные пробелу
			sub_flag = true;
		// если текущий символ "пробел", до этого уже были не только "пробелы" и следующий символ тоже не "пробел"
		if ((victim[j] == ' ') and (victim[j + 1] != ' ') and (sub_flag == true))
		{
			flag_count += 1;
		}
	}
	if (flag_count != 3) // если количество ключевых пробелов не равно трем
		return false;
	else
		return true;
}

/*
	Добавление чиновника в дерево с предварительной проверкой, которая включает все себя функцию выше.
	Логическая, т.к. возвращает false при ошибке в заполнении
*/ 
bool AddToTree(PNode& Tree, string entry, PNode* officials, int i) 
{
	stringstream init(entry);
	string id, name, boss, bribe;
	init >> id >> name >> boss >> bribe;
	char numbers[10] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
	int count = 0;
	
	if (numb_check(id)==false) // проверка правильности заполнения id
	{
		cout << "Ошибка в id, строка " << i + 2 << endl << endl;
		
		return false;
	}

	if (i != (stoi(id) - 1)) // если id идут не по порядку
	{
		cout << "Ошибка в id (неверный порядок чиновников), строка " << i + 2 << endl << endl;
		
		return false;
	}
		

	if (numb_check(boss) == false) // проверка правильности заполнения boss
	{
		cout << "Ошибка в id руководителя, строка " << i + 2 << endl << endl;
		
		return false;
	}

	// если в векторе нет такого чиновника и при этом текущий чиновник не является главным
	if ((stoi(boss)-1>=i) and (stoi(id)!=1)) 
	{
		cout << "Ошибка в id руководителя (такого руководителя нет), строка " << i + 2 << endl << endl;
		
		return false;
	}
	
	if (numb_check(bribe) == false) // проверка правильности заполнения взятки
	{
		cout << "Ошибка в написании взятки, строка " << i + 2 << endl<<endl;
		
		return false;
	}


	if (stoi(id) == 1) // создание главного чиновника
	{
		Tree = new Node;
		Tree->id = stoi(id);
		Tree->name = name;
		Tree->flag = true;
		Tree->employer = NULL; // т.к. у главного нет руководителя
		Tree->bribe = stoi(bribe);
		officials[stoi(id) - 1] = Tree; // добавление в вектор со всеми чиновниками
	}
	else // остальные чиновники
	{
		PNode New = new Node;
		New->id = stoi(id);
		New->name = name;
		New->employer = officials[stoi(boss) - 1]; // добавление указателя на руководителя
		officials[stoi(boss) - 1]->employees.push_back(New); // добавление данного чиновника в массив с подчиненными его руководителя
		New->bribe = stoi(bribe);
		officials[stoi(id) - 1] = New; // добавление в вектор со всеми чиновниками
	}
	return true; // если все хорошо



}

void officials_print(PNode Tree, int k) // вывод иерархии чиновников
{
	char symb = '-';
	if (Tree->employer == NULL) // вывод руквоводителя
	{
		cout << Tree->name << ' ' << Tree->bribe << endl;
	}
	else		
		//вывод других чиновников переменная 'k' отражает уровень
		cout << string(k * (5), symb) << Tree->name << ' ' << Tree->bribe << endl;
	for (int j = 0; j < Tree->employees.size(); j++) // вывоз функции для вывод подчиненных
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
		minimum = Tree->employees[0]->bribe; // минимальной взяткой ставится первая из взяток подчиненных
		id = 0; // id первого подчиненного

		for (int i = 0; i < Tree->employees.size(); i++)
		{
			if (minimum > Tree->employees[i]->bribe) //если текущая сумма взяток меньше предыдущих
			{
				minimum = Tree->employees[i]->bribe;
				id = i;
			}
		}
		for (int i = 0; i < Tree->employees.size(); i++) // проверка на наличие других подходящих чиновников
		{
			if (minimum == Tree->employees[i]->bribe)
			{
				Tree->employees[i]->flag = true;
				
			}
		}
		Tree->employees[id]->flag = true;
		Tree->bribe += minimum;
	}
	if (Tree->employer == NULL) // вывод найденной суммы
		cout <<endl<<"Минимальная сумма взятки: " << Tree->bribe<<endl<<endl;
	
}

void bribe_path(PNode Tree,string s) // вывод способов получения подписи
{
	string res = s;
	if (Tree->employees.empty() == false)
	{
		for (int i = 0; i < Tree->employees.size(); i++) // проверка всех подчиненных
			// если получение минимальной взятки происходит с участием этого человека
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
	cout << "\n\nДобро пожаловать в программу 'Чиновники'!\nФайл с входными находится в папке проекта!\n";
	cout << "\nФайл должен содержать:\n\n";
	cout<<"- количество чиновников\n- id чиновника, его имя, id руководителя и размер взятки каждого (если взятки нет, то написать 0)\n\n\n";
	system("pause");
	cout << endl;
	ifstream f;
	f.open("input.txt");
	string off;
	bool check = false; // равна false, пока нет ошибок в файле
	if (getline(f, off)) // если файл не пустой
	{
		if ((numb_check(off) == false) or (off=="")) // проверка на то, что  впервой строке находится число (кол-во чиновников)
		{
			cout << "Неверно введено количество чиновников\n\n";
			system("pause");
			return;
		}
		
		
	}
	else //если файл пустой
	{
		cout << "Файл пуст!";
		system("pause");
		return;
		check = true;
	}
	int n = stoi(off);
	string* inputs = new string[n]; // все чиновники (их данные в строках)
	int i = 1;
	while (getline(f, off))  // чтение из файла
	{
		
		if (off == "") // если строка пустая
			check = true;

		else
		{
			if (space_check(off) == false) // проверка на соблюдение шаблона данных
				check = true;
		}

		if((i-1)<n)
			inputs[i-1] = off; // занесение в массив с чиновниками (строками данных)

		if (i == n and check==false) // если было прочтено нужное количество чиновников и ошибок не было
			break;

		if (i > n)
			check = true;
		i++;
	}

	if ((i) != n) // если было введено не нужное количество чиновников
		check = true;
	
	
	if (check == true) // если где-то были ошибки, то выполнение программы заканчивается здесь
	{
		cout << "Ошибка в заполнении файла\n\n";
		system("pause");
	}
	else
	{
		// создание динамического массива с указателями на чиновников, с пустыми указателями по умолчанию

		PNode* officials = new PNode[n]{ nullptr }; 
		PNode Head = NULL; // указатель на главного чиновника

		bool fill = true; // true пока дереов заполняется верно


		for (int i = 0; i < n; i++)
		{
			fill = AddToTree(Head, inputs[i], officials, i);

			if (fill == false) // если была ошибка при создании дерева
			{
				break;
				return;
			}
			
		}

		if (fill == true) // если ошибок при создании не было
		{
			cout << "Получившаяся иерархия министерства:\n\n";
			officials_print(Head, 0); // вывод дерева чиновников
			cout << endl<<string(20, '-');
			cout << endl << endl;
			minim_bribe(Head); // поиск способов дачи минимальной взятки
			cout << "Возможный порядок получения подписей:\n\n";
			bribe_path(Head,Head->name); // вывод путей получения взятки
			cout << "\n\n\n\n";
		}
		
	}

	system("pause");
	cout << endl;


}
