#pragma once

#include <string>
#include <vector>

using namespace std;

const string nums = "0123456789";

/// <summary>
/// Функция чтения строки
/// </summary>
/// <param name="line">Строка для сегментации по запятым</param>
/// <param name="row">Вектор из элементов сегментированной строки</param>
/// <returns></returns>
void ReadLine(string line, vector<string>& row, string &row_number);

/// <summary>
/// Функция поиска индексов ячейки
/// </summary>
/// <param name="arg_r">Искомое значение номера строки</param>
/// <param name="arg_c">Искомое значение имени колонки</param>
/// <param name="row_numbers">Список номеров строк</param>
/// <param name="column_headers">Список имен колонок</param>
/// <param name="index_r">Найденый индекс номера строки ячейки в таблице</param>
/// <param name="index_c">Найденый индекс имени колонки ячейки в таблице</param>
/// <param name="exceptions_collector">Сборщик исключений - собирает в себя исключения, если таковые возникнут</param>
/// <param name="cell_value">Исходное значение дробящейся ячейки. Требуется для создания сообщения в случаи исключения</param>
/// <returns></returns>
void FindCellIndexes(string& arg_r, string& arg_c, vector<string> row_numbers, vector<string> column_headers,
	size_t& index_r, size_t& index_c, string& cell_value);

/// <summary>
/// Считает математическую операцию между двумя аргументами
/// </summary>
/// <param name="op">Математический оператор</param>
/// <param name="arg1">Левый аргумент математического выражения</param>
/// <param name="arg2">Правый аргумент математического выражения</param>
/// <returns></returns>
string Order(char op, string arg1, string arg2, string& cell_value);

/// <summary>
/// Функция попытки получения int из string
/// </summary>
/// <param name="cell_value">Исходная ячейка</param>
/// <param name="substr_start_pos">Позиция с которой мы пытаемся срезать число из исходной ячейки</param>
/// <param name="substr_count">Число срезаемых символов</param>
/// <returns></returns>
int TryStoI(string& cell_value, int substr_start_pos, int substr_count);