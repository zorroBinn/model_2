#include <iostream>
#include <Windows.h>
#include <cmath>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
using namespace std;

//Функция чтения исходной таблицы значений из файла
void readDataFromFile(const string& filename, vector<vector<int>>& table, int& row, int& col) {
    ifstream File(filename);
    if (!File.is_open()) {
        std::cerr << "Ошибка открытия файла!" << std::endl;
        return;
    }
    int rows, cols;
    File >> cols >> rows;
    table.resize(rows, vector<int>(++cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (!j) table[i][j] = i + 1;
            else File >> table[i][j];
        }
    }
    row = rows;
    col = cols;
    File.close();
}

//Функция вывода таблицы значений обработки деталей
void printTable(const vector<vector<int>>& table, int machines) {
    for (int i = -1; i < machines - 1; i++) {
        if (i == -1) cout << setw(3) << static_cast<char>('i') << " ";
        else cout << setw(3) << static_cast<char>('A' + i) << " ";
    }
    cout << endl;
    for (const auto& row : table) {
        for (const auto& col : row) {
            cout << setw(3) << col << " ";
        }
        cout << endl;
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    vector<vector<int>> table; //Исходная таблица значений
    int machines, details; //Кол-во станков и деталей

    readDataFromFile("input.txt", table, details, machines);

    cout << "Исходная таблица значений:" << endl;
    printTable(table, machines);

    cout << endl << endl;
    return 0;
}