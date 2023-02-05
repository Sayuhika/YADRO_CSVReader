#pragma once

#include <vector>
#include <string>

using namespace std;

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
string GetResultAtCell(int c_n, int r_n, vector<string>& column_headers, vector<string>& row_numbers,
	vector<vector<string>>& table, vector<vector<bool>>& map);