#include "CSVReader.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;

vector<string> *exceptions_collector;

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");

	string file_name;

	if (argc > 1){
		file_name = argv[1];
	}
	else {
		//file_name = "t_ex5.csv";
		cout << "Работа программы прекращена по причине:" << endl;
		cout << "Путь к файлу не указан." << endl;
		return 1;
	}

	vector<vector<string>> table;
	vector<vector<bool>> check_pos_map;
	vector<string> row, column_headers;
	vector<bool> row_false_init;
	vector<int>	row_numbers;
	string line, r_head;
	int row_number;
	size_t i, j;

	// Получаем таблицу
	fstream file(file_name, ios::in);
	if (file.is_open())
	{
		// Получаем набор наименований колонок
		getline(file, line);
		ReadLine(line, column_headers, r_head);

		// Проверка на корректность имен колонок
		for(string header:column_headers){
			if (nums.find(header[header.size() - 1]) != string::npos) {
				cout << "Программа завершила работу с ошибкой:" << endl;
				cout << "Некорректное имя колонки: " << header << "." << endl;
				cout << "Имя колонки не должно заканчиваться на цифру." << endl;

				return 2;
			}
		}

		// Считываем значения из таблицы
		while (getline(file, line))
		{
			row.clear();
			row_false_init.clear();

			row_number = ReadLine(line, row, r_head);
			if (row_number == -1)
			{
				cout << "Программа завершила работу с ошибкой:" << endl;
				cout << "Некорректный номер строки: " << r_head << "." <<endl;

				return 3;
			}
			row_numbers.push_back(row_number);
			table.push_back(row);

			// Заполняем карту
			// Карта будет использоваться для пометки о том, что значение в ячейке расчитано
			for (i = 0; i < row.size(); i++)
			{
				row_false_init.push_back(false);
			}
			check_pos_map.push_back(row_false_init);
		}
	}
	exceptions_collector = new vector<string>;

	// Проверка таблицы на "прямоугольность"
	size_t row_size(column_headers.size());
	bool flag_se_b(false), flag_se_s(false);

	for (i = 0; i < table.size(); i++)	
	{	
		if (table[i].size() != row_size)
		{
			exceptions_collector->push_back(
				"Некорректное число элементов в строке: " + to_string(row_numbers[i]) +
				"\nТекущее число:  " + to_string(table[i].size()) + ";" +
				"\nОжидаемое число:" + to_string(row_size) + ".");

			if (table[i].size() < row_size)
			{
				flag_se_s = true;

				// Расширяем строку, заполняя ячейки нулями. Правим карту
				while (table[i].size() < row_size) { 
					table[i].push_back("0");
					check_pos_map[i].push_back(false);
				};
			} else 
			{
				flag_se_b = true;

				// Обрезаем строку до числа известных наименований колонок таблицы
				while (table[i].size() > row_size) { 
					table[i].pop_back(); 
				};
			}
		}
	}
	if (flag_se_b) exceptions_collector->push_back(
		"Таблица была обрезана по ширине основываясь на числе известных имен колонок.");
	if (flag_se_s) exceptions_collector->push_back(
		"Неуказанные в файле элементы строки таблицы были автоматически добавлены на основывании числа известных имен колонок.");


	// Расчет значений в таблице
	for (i = 0; i < table.size(); i++){
		for (j = 0; j < table[0].size(); j++){		
			table[i][j] = GetResultAtCell(j, i, column_headers, row_numbers, table, check_pos_map);
		}
	}

	// Вывод таблицы в консоль
	double res;

	for (i = 0; i < column_headers.size(); i++)
	{
		cout << "," << column_headers[i];
	}
	for (i = 0; i < row_numbers.size(); i++)
	{
		cout << endl << row_numbers[i];

		for (j = 0; j < column_headers.size(); j++){

			// Вывод расчитанных данных
			try 
			{
				res = stod(table[i][j]);

				cout << "," << setprecision(2) << res;
			}
			catch (const exception&)
			{	
				cout << "," << table[i][j];
			}
		}
	}

	// Вывод списка ошибок
	size_t  ec_s = exceptions_collector->size();

	if (ec_s > 0)
	{
		cout << endl << "\nСписок выявленных проблем при работе с таблицей:" << endl;

		for (i = 0; i < ec_s; i++){
			cout << to_string(i + 1) << ") " << exceptions_collector->at(i) << endl;
		}
	}

	delete exceptions_collector;

	return 0;
}
