#include "CSVReader.h"
#include "ExceptionsCollector.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <variant>

using namespace std;

// Функция чтения строки
void ReadLine(string line, vector<string> &row, string &row_number)
{
	// Учет CRLF
	line.erase(remove(line.begin(), line.end(), '\r'),line.end());

	stringstream str(line);
	string value;

	// Отделяем первую ячейку
	getline(str, value, ',');
	row_number = value;

	// Перевести строку в набор элементов
	while (getline(str, value, ','))
	{
		row.push_back(value);
	}

	// Обработка некорректного номера строки
	if (row_number.find_first_not_of(nums) != string::npos)
	{
		throw runtime_error(
			"Некорректный номер строки: "s + row_number + ".\n" +
			"Номер строки должен определяться ТОЛЬКО цифрами.");
	}
}

// Функция поиска индексов ячейки
void FindCellIndexes(string &arg_r, string &arg_c, vector<string> row_numbers, vector<string> column_headers,
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
		if (index_r == -1) ex_mes += ("\nСтрока с номером (" + arg_r + ") не найдена.");
		if (index_c == -1) ex_mes += ("\nКолонка с именем (" + arg_c + ") не найдена.");

		throw runtime_error(ex_mes);
	}
}

// Считает математическую операцию между двумя аргументами
string Order(char op, string arg1, string arg2, string& cell_value)
{
	double arg1_i, arg2_i;

	if((arg1.find_first_not_of(nums + ",.") == string::npos) && (arg2.find_first_not_of(nums + ",.") == string::npos))
	{
		arg1_i = stod(arg1);
		arg2_i = stod(arg2);
	}
	else
	{
		throw runtime_error(
			"Неудалось вычислить выражение (" + cell_value + ") по причине:" +
			"\nОдин из аргументов не является числом." +
			"\nЛевый аргумент:  " + arg1 +
			"\nПравый аргумент: " + arg2);		
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
		return (cell_value);
	}
}

// Функция попытки получения int из string
int TryStoI(string &cell_value, int substr_start_pos, int substr_count) {
	
	string temp = cell_value.substr(substr_start_pos, substr_count);

	try 
	{ 
		return stoi(temp);
	}
	catch (invalid_argument)
	{
		throw runtime_error(
			"Расчет значения в ячейке (" + cell_value + ") был прекращен по причине:" +
			"\nНе удалось преобразовать к числу выражение (" + temp + ").");
	}
}


