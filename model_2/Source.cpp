#include <iostream>
#include <Windows.h>
#include <cmath>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
using namespace std;

//������� ������ �������� ������� �������� �� �����
void readDataFromFile(const string& filename, vector<vector<int>>& table, int& row, int& col) {
    ifstream File(filename);
    if (!File.is_open()) {
        cerr << "������ �������� �����!" << endl;
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

//������� ������ ������� �������� ��������� �������
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

//������� ������ ������� �������� ��������� �������
void printTableWithParam(const vector<vector<int>>& table, vector<vector<int>>& parameters, int machines) {
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

//������� ������� ���������� Pi1, Pi2 � lambda ��� ������ ������
void calculateParameters(vector<vector<int>>& table, vector<vector<int>>& parameters, int rows, int cols) {
    parameters.resize(rows, vector<int>(4));
    for (int i = 0; i < rows; i++) {
        int P1 = 0, P2 = 0;
        int half = ((cols) / 2) + (cols) % 2; //�������� ������� (��������� �������� m)

        //������������ P1 (����� ������� �� ������ �������� �������)
        for (int j = 1; j <= half; j++) {
            P1 += table[i][j];
        }

        //������������ P2 (����� ������� �� ������ �������� �������)
        for (int j = half; j <= cols; j++) {
            P2 += table[i][j];
        }

        int lambda = P2 - P1;

        //���������� ���������� � ������� ����������
        parameters[i][0] = i + 1; //����� ������
        parameters[i][1] = P1; //P1
        parameters[i][2] = P2; //P2
        parameters[i][3] = lambda;
    }
}

//������� ��� ������������� ������� �� ������������� D0, D1, D10, D2
void distributeDetails(const vector<vector<int>>& parameters, vector<int>& D0, vector<int>& D1, vector<int>& D10, vector<int>& D2) {
    for (const auto& param : parameters) {
        int lambda = param[3];
        int detailNumber = param[0];

        if (lambda > 0) {
            D1.push_back(detailNumber);
            D10.push_back(detailNumber); //��� ��� lmb >= 0, �� ����� ��������� � D10
        }
        else if (lambda == 0) {
            D0.push_back(detailNumber);
            D10.push_back(detailNumber); //��� ��� lmb = 0, �� ����� ��������� � D10
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

//������� ��� ������ �������� �������
void printSets(const vector<int>& D0, const vector<int>& D1, const vector<int>& D10, const vector<int>& D2) {
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

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    vector<vector<int>> table; //�������� ������� ��������
    vector<vector<int>> parameters; //������� � ����������� P1, P2, lambda
    vector<int> D0, D1, D10, D2; //������������ D0, D1, D10, D2
    int machines, details; //���-�� ������� � �������

    readDataFromFile("input.txt", table, details, machines);

    cout << "�������� ������� ��������:" << endl;
    printTable(table, machines);
    cout << endl << endl;

    calculateParameters(table, parameters, details, machines);
    cout << "�������� ������� �������� c �����������:" << endl;
    printTableWithParam(table, parameters, machines);
    
    distributeDetails(parameters, D0, D1, D10, D2);
    cout << "������������ �������:" << endl;
    printSets(D0, D1, D10, D2);

    cout << endl << endl;
    return 0;
}