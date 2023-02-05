#pragma once

#include <vector>
#include <string>

using namespace std;

/// <summary>
/// ������� ���������� �������� � ������
/// </summary>
/// <param name="c_n">������ ������� ������</param>
/// <param name="r_n">������ ������ ������</param>
/// <param name="column_headers">������ ������� �����</param>
/// <param name="row_numbers">������ ���� �������</param>
/// <param name="table">������� ��������</param>
/// <param name="map">����� ��� ������� ����������� �����</param>
/// <param name="exceptions_collector">������� ���������� - �������� � ���� ����������, ���� ������� ���������</param>
/// <returns></returns>
string GetResultAtCell(int c_n, int r_n, vector<string>& column_headers, vector<string>& row_numbers,
	vector<vector<string>>& table, vector<vector<bool>>& map);