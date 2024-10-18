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
        cerr << "Ошибка открытия файла!" << endl;
        return;
    }
    int rows, cols;
    File >> cols >> rows;
    table.resize(rows, vector<int>(cols+1));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <= cols; j++) {
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
    for (int i = -1; i < machines; i++) {
        if (i == -1) cout << setw(3) << static_cast<char>('i') << " ";
        else if (i < machines) cout << setw(3) << static_cast<char>('A' + i) << " ";
    }
    cout << endl;
    for (const auto& row : table) {
        for (const auto& col : row) {
            cout << setw(3) << col << " ";
        }
        cout << endl;
    }
}

//Функция вывода таблицы значений обработки деталей
void printTableWithParam(vector<vector<int>>& table, vector<vector<int>>& parameters, int machines) {
    for (int i = -1; i < machines + 3; i++) {
        if (i == -1) cout << setw(3) << static_cast<char>('i') << " ";
        else if (i < machines) cout << setw(3) << static_cast<char>('A' + i) << " ";
        else if (i == machines) cout << setw(3) << "P1" << " ";
        else if (i == machines + 1) cout << setw(3) << "P2" << " ";
        else cout << "Lmb" << " ";
    }
    cout << endl;
    for (int i = 0; i < table.size(); i++) {
        for (int j = 0; j < table[i].size(); j++) {
            cout << setw(3) << table[i][j] << " ";
        }
        for (int j = 1; j < 4; j++) {
            cout << setw(3) << parameters[i][j] << " ";
        }
        cout << endl;
    }
}

//Функция вывода таблицы значений обработки деталей с параметрами и последовательностями обработки
void printTableWithParamAndOrder(vector<vector<int>>& table, vector<vector<int>>& parameters, vector<vector<int>>& orderTable, int machines) {
    for (int i = -1; i < machines + 7; i++) {
        if (i == -1) cout << setw(3) << static_cast<char>('i') << " ";
        else if (i < machines) cout << setw(3) << static_cast<char>('A' + i) << " ";
        else if (i == machines) cout << setw(3) << "P1" << " ";
        else if (i == machines + 1) cout << setw(3) << "P2" << " ";
        else if (i == machines + 2) cout << "Lmb" << " ";
        else if (i == machines + 3) cout << "R1" << " ";
        else if (i == machines + 4) cout << "R2" << " ";
        else if (i == machines + 5) cout << "R3" << " ";
        else if (i == machines + 6) cout << "R4" << " ";
    }
    cout << endl;

    for (int i = 0; i < table.size(); i++) {
        for (int j = 0; j < table[i].size(); j++) {
            cout << setw(3) << table[i][j] << " ";
        }
        for (int j = 1; j < 4; j++) {
            cout << setw(3) << parameters[i][j] << " ";
        }
        for (int k = 0; k < 4; k++) {
            cout << setw(3) << orderTable[i][k] << " ";
        }
        cout << endl;
    }
}

//Функция расчета параметров Pi1, Pi2 и lambda для каждой детали
void calculateParameters(vector<vector<int>>& table, vector<vector<int>>& parameters, int rows, int cols) {
    parameters.resize(rows, vector<int>(4));
    for (int i = 0; i < rows; i++) {
        int P1 = 0, P2 = 0;
        int half = ((cols) / 2) + (cols) % 2; //Половина станков (учитывает нечётное m)

        //Рассчитываем P1 (сумма времени на первой половине станков)
        for (int j = 1; j <= half; j++) {
            P1 += table[i][j];
        }

        //Рассчитываем P2 (сумма времени на второй половине станков)
        for (int j = half; j <= cols; j++) {
            P2 += table[i][j];
        }

        int lambda = P2 - P1;

        //Записываем результаты в таблицу параметров
        parameters[i][0] = i + 1; //Номер детали
        parameters[i][1] = P1; //P1
        parameters[i][2] = P2; //P2
        parameters[i][3] = lambda;
    }
}

//Функция для распределения деталей по подмножествам D0, D1, D10, D2
void distributeDetails(vector<vector<int>>& parameters, vector<int>& D0, vector<int>& D1, vector<int>& D10, vector<int>& D2) {
    for (const auto& param : parameters) {
        int lambda = param[3];
        int detailNumber = param[0];

        if (lambda > 0) {
            D1.push_back(detailNumber);
            D10.push_back(detailNumber); //Так как lmb >= 0, то также добавляем в D10
        }
        else if (lambda == 0) {
            D0.push_back(detailNumber);
            D10.push_back(detailNumber); //Так как lmb = 0, то также добавляем в D10
        }
        else {
            D2.push_back(detailNumber);
        }
    }
    sort(D0.begin(), D0.end());
    sort(D1.begin(), D1.end());
    sort(D10.begin(), D10.end());
    sort(D2.begin(), D2.end());
}

//Функция для вывода множеств деталей
void printSets(vector<int>& D0, vector<int>& D1, const vector<int>& D10, vector<int>& D2) {
    cout << "D0 (lmb = 0): ";
    for (int detail : D0) cout << detail << " ";
    cout << endl;

    cout << "D1 (lmb > 0): ";
    for (int detail : D1) cout << detail << " ";
    cout << endl;

    cout << "D10 (lmb >= 0): ";
    for (int detail : D10) cout << detail << " ";
    cout << endl;

    cout << "D2 (lmb < 0): ";
    for (int detail : D2) cout << detail << " ";
    cout << endl;
}

//Функция для заполнения первого столбца по первому правилу Петрова
void rule1(vector<vector<int>>& parameters, vector<int>& D10, vector<int>& D2, vector<int>& orderColumn) {
    //Сортируем детали в D10 по возрастанию Pi1, при равенстве Pi1 - по убыванию lambda
    vector<int> sortedD10 = D10;
    sort(sortedD10.begin(), sortedD10.end(), [&](int a, int b) {
        if (parameters[a - 1][1] != parameters[b - 1][1])
            return parameters[a - 1][1] < parameters[b - 1][1];
        return parameters[a - 1][3] > parameters[b - 1][3];
        });

    //Сортируем детали в D2 по убыванию Pi2, при равенстве Pi2 - по убыванию lambda
    vector<int> sortedD2 = D2;
    sort(sortedD2.begin(), sortedD2.end(), [&](int a, int b) {
        if (parameters[a - 1][2] != parameters[b - 1][2])
            return parameters[a - 1][2] > parameters[b - 1][2];
        return parameters[a - 1][3] > parameters[b - 1][3];
        });

    //Заполняем порядок обработки: сначала элементы из sortedD10, затем из sortedD2
    int index = 0;
    for (int detail : sortedD10) {
        orderColumn[index++] = detail;
    }
    for (int detail : sortedD2) {
        orderColumn[index++] = detail;
    }
}

void fillOrderTable(vector<vector<int>>& orderTable, vector<vector<int>>& parameters, vector<int>& D0, vector<int>& D1, vector<int>& D10, vector<int>& D2) {
    rule1(parameters, D10, D2, orderTable[0]); //Заполняем 1 столбец по 1 правилу
    //rule2(parameters, orderTable[1]);          // Заполняем 2 столбец
    //rule3(parameters, D1, D0, D2, orderTable[2]); // Заполняем 3 столбец
    //rule4(parameters, D1, D0, D2, orderTable[3]); // Заполняем 4 столбец
}


int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    vector<vector<int>> table; //Исходная таблица значений
    vector<vector<int>> parameters; //Таблица с параметрами P1, P2, lambda
    
    vector<int> D0, D1, D10, D2; //Подмножества D0, D1, D10, D2
    int machines, details; //Кол-во станков и деталей

    readDataFromFile("input.txt", table, details, machines);

    cout << "Исходная таблица значений:" << endl;
    printTable(table, machines);
    cout << endl << endl;

    calculateParameters(table, parameters, details, machines);
    cout << "Исходная таблица значений c параметрами:" << endl;
    printTableWithParam(table, parameters, machines);
    
    distributeDetails(parameters, D0, D1, D10, D2);
    cout << "Подмножества деталей:" << endl;
    printSets(D0, D1, D10, D2);
    cout << endl << endl;

    vector<vector<int>> orderTable(details, vector<int>(4)); //Таблица с порядками деталей, определёнными согласно правилам Петрова
    //fillOrderTable(orderTable, parameters, D0, D1, D10, D2);
    //printTableWithParamAndOrder(table, parameters, orderTable, machines);

    cout << endl << endl;
    return 0;
}