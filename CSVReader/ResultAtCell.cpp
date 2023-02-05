#include "ResultAtCell.h"
#include <stdexcept>
#include "ExceptionsCollector.h"
#include "CSVReader.h"

using namespace std;

// ������� ���������� �������� � ������
string GetResultAtCell(int c_n, int r_n, vector<string>& column_headers, vector<string>& row_numbers,
	vector<vector<string>>& table, vector<vector<bool>>& map)
{
	string cell_value = table[r_n][c_n];
	if (map[r_n][c_n]) return cell_value;

	if (cell_value[0] == '=')
	{
		// ����� ���������
		char op_chr = '\0';
		size_t op_pos = cell_value.find_first_of("+-*/", 1);
		if (op_pos != string::npos)
			op_chr = cell_value[op_pos];
		else
		{
			// ������ ������ ��������� "=A1"
			string arg_s, arg_i;
			size_t index_r, index_c;

			size_t eos_i = cell_value.find_last_not_of(nums);
			try
			{
				arg_i = TryStoI(cell_value, eos_i + 1, cell_value.size() - eos_i);
			}
			catch (runtime_error e)
			{
				ExceptionsCollector::getInstance()->add(e.what());

				map[r_n][c_n] = true;
				return table[r_n][c_n] = cell_value;
			}
			arg_s = cell_value.substr(1, eos_i);

			// ������� ��������� �������� ������
			if (arg_s.empty())
			{
				table[r_n][c_n] = arg_i;
			}
			else
			{
				try
				{
					FindCellIndexes(arg_i, arg_s, row_numbers, column_headers, index_r, index_c, cell_value);
					table[r_n][c_n] = GetResultAtCell(index_c, index_r, column_headers, row_numbers, table, map);
				}
				catch (runtime_error e)
				{
					ExceptionsCollector::getInstance()->add(e.what());

					table[r_n][c_n] = cell_value;
				}

			}

			map[r_n][c_n] = true;
			return table[r_n][c_n];
		}

		// ������ ���� ���������� "=A1+A2"
		// ���� ��������� ���������� �� ������ // ������ // ========================================== //
		string arg1_s, arg2_s, arg1_i, arg2_i;
		size_t eos_i = 0;

		// ������� ��� ������� � ����� ������ ������� ���������
		eos_i = cell_value.find_last_not_of(nums);
		try
		{
			arg2_i = TryStoI(cell_value, eos_i + 1, cell_value.size() - eos_i);
		}
		catch (runtime_error e)
		{
			ExceptionsCollector::getInstance()->add(e.what());

			map[r_n][c_n] = true;
			return table[r_n][c_n] = cell_value;
		}
		arg2_s = cell_value.substr(op_pos + 1, eos_i - op_pos);

		// ������� ��� ������� � ����� ������ ������ ���������
		eos_i = cell_value.find_last_not_of(nums, op_pos - 1);
		try
		{
			arg1_i = TryStoI(cell_value, eos_i + 1, op_pos - eos_i);
		}
		catch (runtime_error e)
		{
			ExceptionsCollector::getInstance()->add(e.what());

			map[r_n][c_n] = true;
			return table[r_n][c_n] = cell_value;
		}
		arg1_s = cell_value.substr(1, eos_i);

		// ���� ���������������� ������ ����� � �������
		size_t index1_r, index2_r, index1_c, index2_c;
		string g_arg1, g_arg2;

		// �������� 1
		// ������� ��������� �������� ������
		if (arg1_s.empty())
		{
			g_arg1 = arg1_i;
		}
		else
		{
			try
			{
				FindCellIndexes(arg1_i, arg1_s, row_numbers, column_headers, index1_r, index1_c, cell_value);
				g_arg1 = GetResultAtCell(index1_c, index1_r, column_headers, row_numbers, table, map);
			}
			catch (runtime_error e)
			{
				ExceptionsCollector::getInstance()->add(e.what());

				map[r_n][c_n] = true;
				return table[r_n][c_n] = cell_value;
			}

		}

		// �������� 2
		// ������� ��������� �������� ������
		if (arg2_s.empty())
		{
			g_arg2 = arg2_i;
		}
		else
		{
			try
			{
				FindCellIndexes(arg2_i, arg2_s, row_numbers, column_headers, index2_r, index2_c, cell_value);
				g_arg2 = GetResultAtCell(index2_c, index2_r, column_headers, row_numbers, table, map);
			}
			catch (runtime_error e)
			{
				ExceptionsCollector::getInstance()->add(e.what());

				map[r_n][c_n] = true;
				return table[r_n][c_n] = cell_value;
			}

		}

		// ������ �������� � ������
		try
		{
			cell_value = Order(op_chr, g_arg1, g_arg2, cell_value);
		}
		catch (runtime_error e)
		{
			ExceptionsCollector::getInstance()->add(e.what());
		}

		// ���� ��������� ���������� �� ������ // ����� // ========================================== //
	}

	map[r_n][c_n] = true;
	return table[r_n][c_n] = cell_value;
}