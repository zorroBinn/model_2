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
void printTable(vector<vector<int>>& table, int machines) {
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

//������� ������ ������� �������� ��������� ������� � ����������� � �������������������� ���������
void printTableWithParamAndOrder(vector<vector<int>>& table, vector<vector<int>>& parameters, vector<vector<int>>& orderTable, int machines) {
    for (int i = -1; i < machines + 7; i++) {
        if (i == -1) cout << setw(3) << static_cast<char>('i') << " ";
        else if (i < machines) cout << setw(3) << static_cast<char>('A' + i) << " ";
        else if (i == machines) cout << setw(3) << "P1" << " ";
        else if (i == machines + 1) cout << setw(3) << "P2" << " ";
        else if (i == machines + 2) cout << setw(3) << "Lmb" << " ";
        else if (i == machines + 3) cout << setw(3) << "R1" << " ";
        else if (i == machines + 4) cout << setw(3) << "R2" << " ";
        else if (i == machines + 5) cout << setw(3) << "R3" << " ";
        else if (i == machines + 6) cout << setw(3) << "R4" << " ";
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
void distributeDetails(vector<vector<int>>& parameters, vector<int>& D0, vector<int>& D1, vector<int>& D10, vector<int>& D2) {
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
void printSets(vector<int>& D0, vector<int>& D1, vector<int>& D10, vector<int>& D2) {
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

//������� ��� ���������� ������� ������� �� ������� ������� �������
void rule1(vector<vector<int>>& parameters, vector<int>& D10, vector<int>& D2, vector<vector<int>>& orderTable) {
    //��������� ������ � D10 �� ����������� Pi1, ��� ��������� Pi1 - �� �������� lambda
    vector<int> sortedD10 = D10;
    sort(sortedD10.begin(), sortedD10.end(), [&](int a, int b) {
        if (parameters[a - 1][1] != parameters[b - 1][1])
            return parameters[a - 1][1] < parameters[b - 1][1];
        return parameters[a - 1][3] > parameters[b - 1][3];
        });

    //��������� ������ � D2 �� �������� Pi2, ��� ��������� Pi2 - �� �������� lambda
    vector<int> sortedD2 = D2;
    sort(sortedD2.begin(), sortedD2.end(), [&](int a, int b) {
        if (parameters[a - 1][2] != parameters[b - 1][2])
            return parameters[a - 1][2] > parameters[b - 1][2];
        return parameters[a - 1][3] > parameters[b - 1][3];
        });

    int index = 0;
    for (int detail : sortedD10) {
        orderTable[index++][0] = detail;
    }
    for (int detail : sortedD2) {
        orderTable[index++][0] = detail;
    }
}

//������� ��� ���������� ������� ������� �� ������� ������� �������
void rule2(vector<vector<int>>& parameters, vector<int>& D1, vector<int>& D0, vector<int>& D2, vector<vector<int>>& orderTable) {
    //�������� ��� ������ � ���� ������
    vector<int> allDetails;
    allDetails.insert(allDetails.end(), D1.begin(), D1.end());
    allDetails.insert(allDetails.end(), D0.begin(), D0.end());
    allDetails.insert(allDetails.end(), D2.begin(), D2.end());

    //��������� ��� ������ �� �������� lambda
    sort(allDetails.begin(), allDetails.end(), [&](int a, int b) {
        return parameters[a - 1][3] > parameters[b - 1][3];
        });

    int index = 0;
    for (int i = 0; i < allDetails.size(); ) {
        int currentLambda = parameters[allDetails[i] - 1][3];
        vector<int> sameLambdaDetails;

        //�������� ��� ������ � ���������� ��������� lambda
        while (i < allDetails.size() && parameters[allDetails[i] - 1][3] == currentLambda) {
            sameLambdaDetails.push_back(allDetails[i]);
            i++;
        }
        //���� � ���� ������ ����� ����� ������, ��������� �� �������� �������
        if (sameLambdaDetails.size() > 1) {
            //��������� ������� ��� ���������� �� ������������� D1, D0, D2
            vector<int> tempD1, tempD0, tempD2;

            //������������ ������ �� ������������� D1, D0, D2
            for (int detail : sameLambdaDetails) {
                if (find(D1.begin(), D1.end(), detail) != D1.end()) {
                    tempD1.push_back(detail);
                }
                else if (find(D0.begin(), D0.end(), detail) != D0.end()) {
                    tempD0.push_back(detail);
                }
                else if (find(D2.begin(), D2.end(), detail) != D2.end()) {
                    tempD2.push_back(detail);
                }
            }

            //��������� ������ � D1 �� ����������� Pi1
            sort(tempD1.begin(), tempD1.end(), [&](int a, int b) {
                return parameters[a - 1][1] < parameters[b - 1][1];
                });

            //��������� ������ � D0 �� ����������� Pi1
            sort(tempD0.begin(), tempD0.end(), [&](int a, int b) {
                return parameters[a - 1][1] < parameters[b - 1][1];
                });

            //��������� ������ � D2 �� �������� Pi2
            sort(tempD2.begin(), tempD2.end(), [&](int a, int b) {
                return parameters[a - 1][2] > parameters[b - 1][2];
                });

            //��������� ��������������� ������ � orderTable
            for (int detail : tempD1) {
                orderTable[index++][1] = detail;
            }
            for (int detail : tempD0) {
                orderTable[index++][1] = detail;
            }
            for (int detail : tempD2) {
                orderTable[index++][1] = detail;
            }
        }
        else {
            //���� � ������ ����� ���� ������, ������ ��������� � � orderTable
            orderTable[index++][1] = sameLambdaDetails[0];
        }
    }
}

//������� ��� ���������� �������� ������� �� �������� ������� �������
void rule3(vector<vector<int>>& parameters, vector<int>& D1, vector<int>& D0, vector<int>& D2, vector<vector<int>>& orderTable) {
    //��������� ������ � D1 �� ����������� P1
    vector<int> sortedD1 = D1;
    sort(sortedD1.begin(), sortedD1.end(), [&](int a, int b) {
        return parameters[a - 1][1] < parameters[b - 1][1];
        });

    //��������� ������ � D0 �� ����������� P1
    vector<int> sortedD0 = D0;
    sort(sortedD0.begin(), sortedD0.end(), [&](int a, int b) {
        return parameters[a - 1][1] < parameters[b - 1][1];
        });

    //��������� ������ � D2 �� �������� P2
    vector<int> sortedD2 = D2;
    sort(sortedD2.begin(), sortedD2.end(), [&](int a, int b) {
        return parameters[a - 1][2] > parameters[b - 1][2];
        });

    int index = 0;
    for (int detail : sortedD1) {
        orderTable[index++][2] = detail;
    }
    for (int detail : sortedD0) {
        orderTable[index++][2] = detail;
    }
    for (int detail : sortedD2) {
        orderTable[index++][2] = detail;
    }
}

//������� ��� ���������� ��������� ������� �� ��������� ������� �������
void rule4(vector<vector<int>>& parameters, vector<int>& D1, vector<int>& D0, vector<int>& D2, vector<vector<int>>& orderTable) {
    vector<pair<int, int>> pairsD1, pairsD0, pairsD2;
    vector<int> tempD0 = D0;
    vector<int> tempD2 = D2;
    int leftoverD1 = -1, leftoverD0 = -1, leftoverD2 = -1;

    if (!D1.empty()) {
        vector<int> sortedD1_byP2 = D1;
        vector<int> sortedD1_byP1 = D1;

        //��������� D1 �� �������� P2
        sort(sortedD1_byP2.begin(), sortedD1_byP2.end(), [&](int a, int b) {
            return parameters[a - 1][2] > parameters[b - 1][2];
            });

        //��������� D1 �� ����������� P1
        sort(sortedD1_byP1.begin(), sortedD1_byP1.end(), [&](int a, int b) {
            return parameters[a - 1][1] < parameters[b - 1][1];
            });

        //������ ���� � D1
        int sizeD1 = D1.size();
        for (int i = 0; i < sizeD1 / 2; i++) {
            int first = sortedD1_byP2.front(); //������ ������� ���� � �� ����������� P2
            sortedD1_byP2.erase(sortedD1_byP2.begin());
            sortedD1_byP1.erase(find(sortedD1_byP1.begin(), sortedD1_byP1.end(), first));
            int second = sortedD1_byP1.front(); //������ ������� ���� � �� ����������� P1
            sortedD1_byP1.erase(sortedD1_byP1.begin());
            sortedD1_byP2.erase(find(sortedD1_byP2.begin(), sortedD1_byP2.end(), second));
            pairsD1.push_back({ first, second }); //��������� ����
        }

        //���� ���������� ������� � D1 ��������, ��������� ���������� ������ � ��������� �� D0 ��� D2
        if (sizeD1 % 2 != 0) {
            leftoverD1 = sortedD1_byP2.front(); //�������� ������

            //�������� ����� ���� �� D0 �� �������� P1
            if (!tempD0.empty()) {
                //��������� D0 �� ����������� P1
                sort(tempD0.begin(), tempD0.end(), [&](int a, int b) {
                    return parameters[a - 1][1] < parameters[b - 1][1];
                    });
                pairsD1.push_back({ leftoverD1, tempD0[0] }); //���� � �������� ������� �� D1
                tempD0.erase(tempD0.begin()); //������� �������������� ������ �� D0
            }
            //���� D0 �����, ���� � D2
            else if (!tempD2.empty()) {
                //��������� D2 �� ����������� P1
                sort(tempD2.begin(), tempD2.end(), [&](int a, int b) {
                    return parameters[a - 1][1] < parameters[b - 1][1];
                    });
                pairsD1.push_back({ leftoverD1, tempD2[0] }); //���� � �������� ������� �� D1
                tempD2.erase(tempD2.begin()); //������� �������������� ������ �� D2
            }
        }
        if (pairsD1.size() % 2 == 0) {
            //������������� ���� D1 �� �������� �������� (P2 - P1)
            sort(pairsD1.begin(), pairsD1.end(), [&](pair<int, int> a, pair<int, int> b) {
                int diffA = parameters[a.first - 1][2] - parameters[a.second - 1][1]; //�������� P2 - P1 ��� ������ ����
                int diffB = parameters[b.first - 1][2] - parameters[b.second - 1][1]; //�������� P2 - P1 ��� ������ ����
                return diffA > diffB; //�� �������� ��������
                });
        }
    }

    if (!tempD0.empty()) {
        vector<int> sortedD0_byP2 = tempD0;
        vector<int> sortedD0_byP1 = tempD0;

        //��������� D0 �� �������� P2
        sort(sortedD0_byP2.begin(), sortedD0_byP2.end(), [&](int a, int b) {
            return parameters[a - 1][2] > parameters[b - 1][2];
            });

        //��������� D0 �� ����������� P1
        sort(sortedD0_byP1.begin(), sortedD0_byP1.end(), [&](int a, int b) {
            return parameters[a - 1][1] < parameters[b - 1][1];
            });

        //������ ���� � D0
        int sizeD0 = tempD0.size();
        for (int i = 0; i < sizeD0 / 2; i++) {
            int first = sortedD0_byP2.front(); //������ ������� ���� � �� ����������� P2
            sortedD0_byP2.erase(sortedD0_byP2.begin());
            sortedD0_byP1.erase(find(sortedD0_byP1.begin(), sortedD0_byP1.end(), first));
            int second = sortedD0_byP1.front(); //������ ������� ���� � �� ����������� P1
            sortedD0_byP1.erase(sortedD0_byP1.begin());
            sortedD0_byP2.erase(find(sortedD0_byP2.begin(), sortedD0_byP2.end(), second));
            pairsD0.push_back({ first, second }); //��������� ����
        }

        //���� ���������� ������� � D0 ��������, ��������� ���������� ������ � ��������� �� D2, ���� ��� ����
        if (sizeD0 % 2 != 0) {
            leftoverD0 = sortedD0_byP2.front(); //�������� ������

            //�������� ����� ���� �� D2
            if (!tempD2.empty()) {
                //��������� D2 �� ����������� P1
                sort(tempD2.begin(), tempD2.end(), [&](int a, int b) {
                    return parameters[a - 1][1] < parameters[b - 1][1];
                    });
                pairsD0.push_back({ leftoverD0, tempD2[0] }); //���� � �������� ������� �� D1
                tempD2.erase(tempD2.begin()); //������� �������������� ������ �� D2
            }
        }
        if (pairsD0.size() % 2 == 0) {
            //������������� ���� D0 �� �������� �������� (P2 - P1)
            sort(pairsD0.begin(), pairsD0.end(), [&](pair<int, int> a, pair<int, int> b) {
                int diffA = parameters[a.first - 1][2] - parameters[a.second - 1][1]; //�������� P2 - P1 ��� ������ ����
                int diffB = parameters[b.first - 1][2] - parameters[b.second - 1][1]; //�������� P2 - P1 ��� ������ ����
                return diffA > diffB; //�� �������� ��������
                });
        }
    }

    if (!tempD2.empty()) {
        vector<int> sortedD2_byP2 = tempD2;
        vector<int> sortedD2_byP1 = tempD2;

        //��������� D2 �� �������� P2
        sort(sortedD2_byP2.begin(), sortedD2_byP2.end(), [&](int a, int b) {
            return parameters[a - 1][2] > parameters[b - 1][2];
            });

        //��������� D2 �� ����������� P1
        sort(sortedD2_byP1.begin(), sortedD2_byP1.end(), [&](int a, int b) {
            return parameters[a - 1][1] < parameters[b - 1][1];
            });

        //������ ���� � D2
        int sizeD2 = tempD2.size();
        for (int i = 0; i < sizeD2 / 2; i++) {
            int first = sortedD2_byP2.front(); //������ ������� ���� � �� ����������� P2
            sortedD2_byP2.erase(sortedD2_byP2.begin());
            sortedD2_byP1.erase(find(sortedD2_byP1.begin(), sortedD2_byP1.end(), first));
            int second = sortedD2_byP1.front(); //������ ������� ���� � �� ����������� P1
            sortedD2_byP1.erase(sortedD2_byP1.begin());
            sortedD2_byP2.erase(find(sortedD2_byP2.begin(), sortedD2_byP2.end(), second));
            pairsD2.push_back({ first, second }); //��������� ����
        }

        //���� ���������� ������� � D2 ��������
        if (sizeD2 % 2 != 0) {
            leftoverD2 = sortedD2_byP2.front(); //�������� ������
        }

        //������������� ���� D2 �� �������� �������� (P2 - P1)
        sort(pairsD2.begin(), pairsD2.end(), [&](pair<int, int> a, pair<int, int> b) {
            int diffA = parameters[a.first - 1][2] - parameters[a.second - 1][1]; //�������� P2 - P1 ��� ������ ����
            int diffB = parameters[b.first - 1][2] - parameters[b.second - 1][1]; //�������� P2 - P1 ��� ������ ����
            return diffA > diffB; //�� �������� ��������
            });
    }

    //��������� orderTable ������������
    int index = 0;
    for (auto& p : pairsD1) {
        orderTable[index++][3] = p.first;
        orderTable[index++][3] = p.second;
    }
    for (auto& p : pairsD0) {
        orderTable[index++][3] = p.first;
        orderTable[index++][3] = p.second;
    }
    for (auto& p : pairsD2) {
        orderTable[index++][3] = p.first;
        orderTable[index++][3] = p.second;
    }

    //���� �������� �������� ������
    if (leftoverD1 != -1 || leftoverD0 != -1 || leftoverD2 != -1) {
        int leftover;
        if (leftoverD1 != -1) leftover = leftoverD1;
        if (leftoverD0 != -1) leftover = leftoverD0;
        if (leftoverD2 != -1) leftover = leftoverD2;
        int lmbX = parameters[leftover - 1][3];
        bool isInsert = false;
        for (int i = 0; i < parameters.size() - 3; i+=2) {
            int lmb1 = parameters[orderTable[i][3] - 1][3]; //lambda ��� i-�� ��������
            int lmb2 = parameters[orderTable[i + 1][3] - 1][3]; //lambda ��� (i+1)-�� ��������
            int lmb3 = parameters[orderTable[i + 2][3] - 1][3]; //lambda ��� (i+2)-�� ��������
            int lmb4 = parameters[orderTable[i + 3][3] - 1][3]; //lambda ��� (i+3)-�� ��������

            //��������� ������� �������
            if (max(lmb1, lmb2) >= lmbX && lmbX >= min(lmb3, lmb4)) {
                for (int j = parameters.size() - 1; j > i + 2; j--) {
                    orderTable[j][3] = orderTable[j - 1][3];
                }
                orderTable[i + 2][3] = leftover;
                isInsert = true;
                break;
            }  
        }
        if (!isInsert) {
            if (lmbX <= max(parameters[orderTable[parameters.size()-3][3] - 1][3], parameters[orderTable[parameters.size() - 2][3] - 1][3])) {
                //��������� �������� ������ � ����� 4-�� �������
                orderTable[parameters.size() - 1][3] = leftover;
                isInsert = true;
            }
        }
        if (!isInsert) {
            //�������� ��� �������� 4-�� ������� �� ���� ������� ����
            for (int i = parameters.size() - 1; i > 0; i--) {
                orderTable[i][3] = orderTable[i - 1][3];
            }
            //��������� �������� ������ � ������ 4-�� �������
            orderTable[0][3] = leftover;
        }
    }
}


void fillOrderTable(vector<vector<int>>& orderTable, vector<vector<int>>& parameters, vector<int>& D0, vector<int>& D1, vector<int>& D10, vector<int>& D2) {
    rule1(parameters, D10, D2, orderTable); //��������� 1 ������� �� 1 �������
    rule2(parameters, D1, D0, D2, orderTable); //��������� 2 ������� �� 2 �������
    rule3(parameters, D1, D0, D2, orderTable); //��������� 3 ������� �� 3 �������
    rule4(parameters, D1, D0, D2, orderTable); //��������� 4 ������� �� 4 �������
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

    vector<vector<int>> orderTable(details, vector<int>(4)); //������� � ��������� �������, ������������ �������� �������� �������
    fillOrderTable(orderTable, parameters, D0, D1, D10, D2);
    printTableWithParamAndOrder(table, parameters, orderTable, machines);

    cout << endl << endl;
    return 0;
}