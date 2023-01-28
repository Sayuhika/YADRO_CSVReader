#pragma once

#include <string>
#include <vector>

using namespace std;

const string numbers = "0123456789";

/// <summary>
/// Функция чтения строки
/// </summary>
/// <param name="line">Строка для сегментации по запятым</param>
/// <param name="row">Вектор из элементов сегментированной строки</param>
/// <param name="r_n">Служит для хранения значения вызывающего исключение</param>
/// <returns></returns>
int ReadLine(string line, vector<string>& row, string& r_n);

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
bool FindCellIndexes(int& arg_r, string& arg_c, vector<int> row_numbers, vector<string> column_headers,
	size_t& index_r, size_t& index_c, string& cell_value);

/// <summary>
/// Считает математическую операцию между двумя аргументами
/// </summary>
/// <param name="op">Математический оператор</param>
/// <param name="arg1">Левый аргумент математического выражения</param>
/// <param name="arg2">Правый аргумент математического выражения</param>
/// <returns></returns>
string Order(int mod, string arg1, string arg2, string& cell_value);

/// <summary>
/// Функция вычисления значения в ячейке
/// </summary>
/// <param name="c_n">Индекс колонки ячейки</param>
/// <param name="r_n">Индекс строки ячейки</param>
/// <param name="column_headers">Список номеров строк</param>
/// <param name="row_numbers">Список имен колонок</param>
/// <param name="table">Таблица значений</param>
/// <param name="map">Карта для пометки вычисленных ячеек</param>
/// <param name="exceptions_collector">Сборщик исключений - собирает в себя исключения, если таковые возникнут</param>
/// <returns></returns>
string GetResultAtCell(int c_n, int r_n, vector<string>& column_headers, vector<int>& row_numbers, 
	vector<vector<string>>& table, vector<vector<bool>>& map);