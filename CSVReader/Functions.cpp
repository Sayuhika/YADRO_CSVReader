#include "CSVReader.h"
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

extern vector<string> *exceptions_collector;

// Функция чтения строки
int ReadLine(string line, vector<string>& row, string& r_n)
{
	// Учет CRLF
	line.erase(remove(line.begin(), line.end(), '\r'),line.end());

	stringstream str(line);
	string value;

	getline(str, value, ',');
	int row_number = -1;
	try
	{
		row_number = stoi(value);
	}
	catch (const exception&)
	{
		r_n = value;
	}

	while (getline(str, value, ','))
	{
		row.push_back(value);
	}

	return row_number;
}

// Функция поиска индексов ячейки
bool FindCellIndexes(int &arg_r, string &arg_c, vector<int> row_numbers, vector<string> column_headers, 
	size_t &index_r, size_t&index_c, string &cell_value)
{
	for (size_t i = 0; i < row_numbers.size(); i++) 
	{
		index_r = -1;

		if (arg_r == row_numbers[i]) {
			index_r = i;
			break;
		}
	}
	// Столбец
	for (size_t i = 0; i < column_headers.size(); i++) 
	{
		index_c = -1;

		if (arg_c == column_headers[i]) {
			index_c = i;
			break;
		}
	}
	if ((index_c == -1) || (index_r == -1)) 
	{
		string ex_mes = ("Неудалось вычислить выражение (" + cell_value + ") по причине:");
		if (index_r == -1) ex_mes += ("\nСтрока с номером (" + to_string(arg_r) + ") не найдена.");
		if (index_c == -1) ex_mes += ("\nКолонка с именем (" + arg_c + ") не найдена.");
		exceptions_collector->push_back(ex_mes);
		return false;
	}
	return true;
}

// Считает математическую операцию между двумя аргументами
string Order(char op, string arg1, string arg2, string& cell_value)
{
	double arg1_i, arg2_i;

	try {
		arg1_i = stod(arg1);
		arg2_i = stod(arg2);
	}
	catch (const exception&)
	{
		exceptions_collector->push_back(
			"Неудалось вычислить выражение (" + cell_value + ") по причине:" +
			"\nОдин из аргументов не является числом." +
			"\nЛевый аргумент:  " + arg1 +
			"\nПравый аргумент: " + arg2);

		return (arg1 + arg1);
	};

	switch (op) {
	case '+':
		return to_string(arg1_i + arg2_i);
	case '-':
		return to_string(arg1_i - arg2_i);
	case '*':
		return to_string(arg1_i * arg2_i);
	case '/':
	{
		if (arg2_i == 0)
			return "NaN";
		else 
			return to_string(arg1_i / arg2_i);
	}
	default:
		return (arg1 + arg1);
	}
}

// Функция попытки получения int из string
bool TryStoI(int &arg, string &cell_value, int substr_start_pos, int substr_count) {
	
	string temp = cell_value.substr(substr_start_pos, substr_count);

	try 
	{ 
		arg = stoi(temp);
	}
	catch (invalid_argument)
	{
		exceptions_collector->push_back(
			"Расчет значения в ячейке (" + cell_value + ") был прекращен по причине:" +
			"\nНе удалось преобразовать к числу выражение (" + temp + ").");
		return false;
	}
	return true;
}

// Функция вычисления значения в ячейке
string GetResultAtCell(int c_n, int r_n, vector<string>& column_headers, vector<int>& row_numbers, 
	vector<vector<string>>& table, vector<vector<bool>>& map)
{
	string cell_value = table[r_n][c_n];

	if (cell_value[0] == '=')
	{
		// Поиск оператора
		char op_chr = '\0';
		size_t op_pos = cell_value.find_first_of("+-*/", 1);
		if (op_pos != string::npos) op_chr = cell_value[op_pos];
		else {
			// Случай одного аргумента "=A1"
			string arg_s;
			int arg_i, argr_i;
			size_t index_r, index_c;

			size_t eos_i = cell_value.find_last_not_of(nums);			
			if (!TryStoI(arg_i, cell_value, eos_i + 1, cell_value.size() - eos_i)) {
				map[r_n][c_n] = true;
				return table[r_n][c_n] = cell_value;
			}
			arg_s = cell_value.substr(1, eos_i);

			// Попытка получения индексов ячейки
			if (!FindCellIndexes(arg_i, arg_s, row_numbers, column_headers, index_r, index_c, cell_value))
			{
				map[r_n][c_n] = true;
				return table[r_n][c_n] = cell_value;
			}

			map[r_n][c_n] = true; 
			table[r_n][c_n] = cell_value = GetResultAtCell(index_c, index_r, column_headers, row_numbers, table, map);
			return cell_value;
		}

		// Случай двух аргументов "=A1+A2"
		// Блок выделения аргументов из ячейки // Начало // ========================================== //
		string arg1_s, arg2_s;
		int arg1_i, arg2_i;
		size_t eos_i = 0;

		// Находим имя колонки и номер строки правого аргумента
		eos_i = cell_value.find_last_not_of(nums);
		if (!TryStoI(arg2_i, cell_value, eos_i + 1, cell_value.size() - eos_i)) {
			map[r_n][c_n] = true;
			return table[r_n][c_n] = cell_value;
		}
		arg2_s = cell_value.substr(op_pos + 1, eos_i - op_pos);

		// Находим имя колонки и номер строки левого аргумента
		eos_i = cell_value.find_last_not_of(nums, op_pos - 1);
		if (!TryStoI(arg1_i, cell_value, eos_i + 1, op_pos - eos_i)) {
			map[r_n][c_n] = true;
			return table[r_n][c_n] = cell_value;
		}
		arg1_s = cell_value.substr(1, eos_i);

		// Ищем непосредственные номера ячеек в таблице
		size_t index1_r, index2_r, index1_c, index2_c;

		// Аргумент 1
		// Попытка получения индексов ячейки
		if (!FindCellIndexes(arg1_i, arg1_s, row_numbers, column_headers, index1_r, index1_c, cell_value))
		{
			map[r_n][c_n] = true;
			return table[r_n][c_n] = cell_value;
		}

		// Аргумент 2
		// Попытка получения индексов ячейки
		if (!FindCellIndexes(arg2_i, arg2_s, row_numbers, column_headers, index2_r, index2_c, cell_value)) 
		{
			map[r_n][c_n] = true;
			return table[r_n][c_n] = cell_value;
		}

		// Расчет значения в ячейке
		string g_arg1 = GetResultAtCell(index1_c, index1_r, column_headers, row_numbers, table, map);
		string g_arg2 = GetResultAtCell(index2_c, index2_r, column_headers, row_numbers, table, map);
		cell_value = Order(op_chr, g_arg1, g_arg2, cell_value);

		// Блок выделения аргументов из ячейки // Конец // ========================================== //
	}

	map[r_n][c_n] = true;
	return table[r_n][c_n] = cell_value;
}
