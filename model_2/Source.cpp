#include <iostream>
#include <Windows.h>
#include <cmath>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <ctime>
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

//���������� ������� �� �������� �������
void fillOrderTable(vector<vector<int>>& orderTable, vector<vector<int>>& parameters, vector<int>& D0, vector<int>& D1, vector<int>& D10, vector<int>& D2) {
    rule1(parameters, D10, D2, orderTable); //��������� 1 ������� �� 1 �������
    rule2(parameters, D1, D0, D2, orderTable); //��������� 2 ������� �� 2 �������
    rule3(parameters, D1, D0, D2, orderTable); //��������� 3 ������� �� 3 �������
    rule4(parameters, D1, D0, D2, orderTable); //��������� 4 ������� �� 4 �������
}

//������� ������ ������� �� ������������ �������
vector<int> getProcessingOrder(vector<vector<int>>& table, int column) {
    vector<int> order;

    for (const auto& row : table) {
        order.push_back(row[column]);
    }

    return order;
}

//������� ���������������� ������� ������� ��������� ������� �� ��������� �������
vector<vector<int>> reorderTableByOrder(vector<vector<int>>& table, vector<int>& order) {
    vector<vector<int>> newTable;

    for (int index : order) {
        newTable.push_back(table[index - 1]);
    }

    return newTable;
}

//������� ��� ��������� ���� ��������� ������������������� (������������)
vector<vector<int>> generateAllPermutations(int details) {
    vector<vector<int>> allPermutations; //������ ��� �������� ���� ������������
    vector<int> currentOrder; //������ ��� �������� ������� ������������

    for (int i = 1; i <= details; i++) {
        currentOrder.push_back(i);
    }

    do {
        allPermutations.push_back(currentOrder); //��������� ������� ������������ � �������� ������
    } while (next_permutation(currentOrder.begin(), currentOrder.end()));

    return allPermutations;
}

//������� ������� ���������� Tij � �������� Tnm
int calculateTij(vector<vector<int>>& tableTij, vector<vector<int>>& table) {
    int n = tableTij.size();
    int m = tableTij[0].size() - 1;
    for (int i = 0; i < n; i++) {
        tableTij[i][0] = table[i][0];
    }
    //������������ Tij ��� ������ ������ � ������� ������
    for (int i = 0; i < n; i++) {
        for (int j = 1; j <= m; j++) {
            int t_ij = table[i][j];
            //��� ������ ������ � ������� ������, ����� ��������� ����� ������ ����
            if (i == 0 && j == 1) {
                tableTij[i][j] = t_ij;
            }
            //���� ��� ������ ������, ��������� ������ ���������� ����� ��������� �� ���������� ������
            else if (i == 0) {
                tableTij[i][j] = tableTij[i][j - 1] + t_ij;
            }
            //���� ��� ������ ������, ��������� ������ ���������� ����� ��������� �� ���������� ������
            else if (j == 1) {
                tableTij[i][j] = tableTij[i - 1][j] + t_ij;
            }
            //� ����� ������ ��������� �������� �� ������� ��������� ��������� ���������� ������ �� ������� ������ � ������� ������ �� ���������� ������
            else {
                int maxHeightTij = 0, maxLeftTij = 0;
                for (int h = i - 1; h >= 0; h--) {
                    if (tableTij[h][j] > maxHeightTij && table[h][j] != 0) maxHeightTij = tableTij[h][j];
                }
                for (int l = j - 1; l >= 1; l--) {
                    if (tableTij[i][l] > maxLeftTij && table[i][l] != 0) maxLeftTij = tableTij[i][l];
                }
                tableTij[i][j] = t_ij + max(maxHeightTij, maxLeftTij);

            }
        }
    }
    //���������� �������� Tnm - ����� ���������� ��������� ������ �� ��������� ������
    return tableTij[n - 1][m];
}

//������� ��������� ������� �������� �������
vector<int> getToj(vector<vector<int>>& tableTij, vector<vector<int>>& table) {
    vector<int> Toj(tableTij.size() + 1, 0); //������ ��� ������� �������� ������ ������ + ����� ���������
    int n = tableTij.size(); //���������� �������
    int m = tableTij[0].size() - 1; //���������� �������
    int totalToj = 0; //����� ����� ������� ��������

    for (int i = 0; i < n; i++) {
        int sum_t_ij = 0; //����� ������� ��������� t_ij ��� i-�� ������
        //��������� ��� ������� ��������� t_ij ��� i-�� ������ �� ���� �������
        for (int j = 1; j <= m; j++) {
            sum_t_ij += table[i][j]; //���������� ������� � ��������� t_ij
        }
        //������������ ����� �������� ��� i-�� ������
        Toj[i] = tableTij[i][m] - sum_t_ij;
        totalToj += Toj[i];  //��������� � ����� ����� ������� ��������
    }
    Toj[n] = totalToj;

    return Toj;
}

//������� ��������� ������� ������� �������
vector<int> getTpr(vector<vector<int>>& tableTij, vector<vector<int>>& table) {
    vector<int> Tpr(tableTij[0].size(), 0); //������ ��� ������� ������� ������� ������ + ����� ���������
    int n = tableTij.size(); //���������� �������
    int m = tableTij[0].size() - 1; //���������� �������
    int totalTpr = 0; //����� ����� ������� �������

    for (int j = 1; j <= m; j++) {
        int sum_t_ij = 0; //����� ������� ��������� t_ij ��� ���� ������� �� j-�� ������
        //��������� ��� ������� ��������� t_ij ��� j-��� ������
        for (int i = 0; i < n; i++) {
            sum_t_ij += table[i][j];
        }
        //������������ ����� ������� ��� j-��� ������
        Tpr[j - 1] = tableTij[n - 1][j] - sum_t_ij;
        totalTpr += Tpr[j - 1];  //��������� � ����� ����� ������� �������
    }
    Tpr[m] = totalTpr;

    return Tpr;
}

//������� ������ ������� �������� ��������� ��������� �������
void printTijTable(vector<vector<int>>& table, vector<vector<int>>& tableTij, vector<int>& tableToj, vector<int>& tableTpr, int machines) {
    for (int i = -1; i < machines + 1; i++) {
        if (i == -1) cout << setw(3) << static_cast<char>('i') << " ";
        else if (i < machines) cout << setw(7) << static_cast<char>('A' + i) << " ";
        else if (i == machines) cout << setw(7) << "Toj" << " ";
    }
    cout << endl;
    for (int i = 0; i < table.size(); i++) {
        for (int j = 0; j <= machines; j++) {
            if (!j) cout << setw(3) << table[i][j] << " ";
            else if (table[i][j]) cout << setw(3) << table[i][j] << "/" << setw(3) << tableTij[i][j] << " ";
            else if (!table[i][j]) cout << setw(3) << table[i][j] << "/" << setw(4) << " ";
        }
        cout << setw(7) << tableToj[i] << endl;
    }
    cout << setw(3) << "Tpr" << " ";
    for (int i = 0; i < tableTpr.size() - 1; i++) {
        cout << setw(7) << tableTpr[i] << " ";
    }
    cout << setw(5) << tableTpr[tableTpr.size() - 1] << "\\" << tableToj[tableToj.size() - 1] << endl;
}

struct SequenceInfo {
    int index; //������ ������������������
    int Ttc; //����� ���������� ��������� Ttc
    int Toj; //��������� ����� �������� Toj
    int Tpr; //��������� ����� ������� Tpr
};

//������� ��� ������ ����������� ������������������
int findOptimalSequence(int Ttc1, int Ttc2, int Ttc3, int Ttc4, vector<int>& Toj1, vector<int>& Toj2, vector<int>& Toj3, vector<int>& Toj4, vector<int>& Tpr1, vector<int>& Tpr2, vector<int>& Tpr3, vector<int>& Tpr4, vector<vector<int>>& orderTable) {

    vector<SequenceInfo> sequences = {
        {1, Ttc1, Toj1.back(), Tpr1.back()},
        {2, Ttc2, Toj2.back(), Tpr2.back()},
        {3, Ttc3, Toj3.back(), Tpr3.back()},
        {4, Ttc4, Toj4.back(), Tpr4.back()}
    };

    //��������� ������������������ �� ���� ���������:
    //1) ����������� Ttc
    //2) ����������� Toj (���� Ttc ����������)
    //3) ����������� Tpr (���� � Toj ����������)
    auto optimalSeq = *min_element(sequences.begin(), sequences.end(), [](const SequenceInfo& a, const SequenceInfo& b) {
        if (a.Ttc != b.Ttc) {
            return a.Ttc < b.Ttc;
        }
        else if (a.Toj != b.Toj) {
            return a.Toj < b.Toj;
        }
        else {
            return a.Tpr < b.Tpr;
        }
        });

    cout << "����������� ����� ������������������, ���������� �� ������� " << optimalSeq.index << " c T�� = " << optimalSeq.Ttc << " : ";
    vector<int> optimaleRuleOrder = getProcessingOrder(orderTable, optimalSeq.index - 1);
    for_each(optimaleRuleOrder.begin(), optimaleRuleOrder.end(), [](int x) { cout << x << " "; });
    cout << endl << endl;
    return optimalSeq.index;
}

//��������� ������������ ������� ����� ��� NxM
void GanttGraph(vector<vector<int>>& table, vector<vector<int>>& tableTij, int details, int machines) {
    for (int machine = 0; machine < machines; machine++) {
        cout << setw(2) << static_cast<char>('A' + machine) << "| ";

        int currentTime = 0;

        //�������� �� ���� �������
        for (int detail = 0; detail < details; detail++) {
            int detailNumber = table[detail][0];
            if (table[detail][machine + 1] == 0) continue;
            int processingTime = table[detail][machine + 1]; //����� ��������� ������� ������ �� ���� ������
            int startTime = tableTij[detail][machine + 1] - processingTime; //������ ��������� ���� ������ �� ������
            int endTime = tableTij[detail][machine + 1]; //����� ��������� ��������� ������ �� ������
            //������� ����� ������� ���������
            for (int t = currentTime; t < startTime; t++) {
                cout << "-"; //�������
            }
            //��������� ������
            for (int t = startTime; t < endTime; t++) {
                cout << detailNumber;
            }
            currentTime = endTime;
        }
        cout << endl;
    }
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
    cout << endl;

    calculateParameters(table, parameters, details, machines);
    distributeDetails(parameters, D0, D1, D10, D2);
    vector<vector<int>> orderTable(details, vector<int>(4)); //������� � ��������� �������, ������������ �������� �������� �������
    fillOrderTable(orderTable, parameters, D0, D1, D10, D2);
    cout << "������� � ����������� � ���������� ������������������� �� ������ �������� ������� ��������:" << endl;
    printTableWithParamAndOrder(table, parameters, orderTable, machines);
    cout << endl;
    
    //������� ��������� �������
    vector<int> rule1Order = getProcessingOrder(orderTable, 0);
    vector<int> rule2Order = getProcessingOrder(orderTable, 1);
    vector<int> rule3Order = getProcessingOrder(orderTable, 2);
    vector<int> rule4Order = getProcessingOrder(orderTable, 3);
    
    //������� ��������� ������� �� �������� ��������
    vector<vector<int>> rule1Table = reorderTableByOrder(table, rule1Order);
    vector<vector<int>> rule2Table = reorderTableByOrder(table, rule2Order);
    vector<vector<int>> rule3Table = reorderTableByOrder(table, rule3Order);
    vector<vector<int>> rule4Table = reorderTableByOrder(table, rule4Order);
    
    //������� Tij ��� ������ ��������� ������� �� �������� ��������
    vector<vector<int>> tableTij(details, vector<int>(machines + 1));
    vector<vector<int>> rule1Tij(details, vector<int>(machines + 1));
    vector<vector<int>> rule2Tij(details, vector<int>(machines + 1));
    vector<vector<int>> rule3Tij(details, vector<int>(machines + 1));
    vector<vector<int>> rule4Tij(details, vector<int>(machines + 1));
    
    //����� ���������������� �������� ��� ������ ��������� ������� �� �������� ��������
    int Ttc = calculateTij(tableTij, table);
    int Ttc1 = calculateTij(rule1Tij, rule1Table);
    int Ttc2 = calculateTij(rule2Tij, rule2Table);
    int Ttc3 = calculateTij(rule3Tij, rule3Table);
    int Ttc4 = calculateTij(rule4Tij, rule4Table);
    
    //������� �������� ������� ��� ������ ��������� ������� �� �������� ��������
    vector<int> tableToj = getToj(tableTij, table);
    vector<int> rule1Toj = getToj(rule1Tij, rule1Table);
    vector<int> rule2Toj = getToj(rule2Tij, rule2Table);
    vector<int> rule3Toj = getToj(rule3Tij, rule3Table);
    vector<int> rule4Toj = getToj(rule4Tij, rule4Table);
    
    //������� ������� ������� ��� ������ ��������� ������� �� �������� ��������
    vector<int> tableTpr = getTpr(tableTij, table);
    vector<int> rule1Tpr = getTpr(rule1Tij, rule1Table);
    vector<int> rule2Tpr = getTpr(rule2Tij, rule2Table);
    vector<int> rule3Tpr = getTpr(rule3Tij, rule3Table);
    vector<int> rule4Tpr = getTpr(rule4Tij, rule4Table);
    
    cout << "������� ���������� ������������ ����� ��������� ������� ��� ��������� �������:" << endl;
    printTijTable(table, tableTij, tableToj, tableTpr, machines); cout << endl;
    cout << "��� � �������� ������������������: " << Ttc << endl << endl;
    cout << "������ ����� ��� �������� ������������������:" << endl;
    GanttGraph(table, tableTij, details, machines);
    cout << endl;

    cout << "������� ���������� ������������ ����� ��������� ������� ��� 1-��� �������:" << endl;
    printTijTable(rule1Table, rule1Tij, rule1Toj, rule1Tpr, machines); cout << endl;
    cout << "������� ���������� ������������ ����� ��������� ������� ��� 2-��� �������:" << endl;
    printTijTable(rule2Table, rule2Tij, rule2Toj, rule2Tpr, machines); cout << endl;
    cout << "������� ���������� ������������ ����� ��������� ������� ��� 3-��� �������:" << endl;
    printTijTable(rule3Table, rule3Tij, rule3Toj, rule3Tpr, machines); cout << endl;
    cout << "������� ���������� ������������ ����� ��������� ������� ��� 4-��� �������:" << endl;
    printTijTable(rule4Table, rule4Tij, rule4Toj, rule4Tpr, machines); cout << endl;

    int optimalSequence = findOptimalSequence(Ttc1, Ttc2, Ttc3, Ttc4, rule1Toj, rule2Toj, rule3Toj, rule4Toj, rule1Tpr, rule2Tpr, rule3Tpr, rule4Tpr, orderTable);
    switch (optimalSequence) {
    case 1: {
        cout << "������ ����� ��� �������� ������������������, ���������� �� ������� 1:" << endl;
        GanttGraph(rule1Table, rule1Tij, details, machines);
        break;
    }
    case 2: {
        cout << "������ ����� ��� �������� ������������������, ���������� �� ������� 2:" << endl;
        GanttGraph(rule2Table, rule2Tij, details, machines);
        break;
    }
    case 3: {
        cout << "������ ����� ��� �������� ������������������, ���������� �� ������� 3:" << endl;
        GanttGraph(rule3Table, rule3Tij, details, machines);
        break;
    }
    case 4: {
        cout << "������ ����� ��� �������� ������������������, ���������� �� ������� 4:" << endl;
        GanttGraph(rule4Table, rule4Tij, details, machines);
        break;
    }
    }
    cout << endl;

    int minTtc = Ttc, minT_PermitationsIndex = 0;
    cout << "������� ���������:" << endl;
    vector<vector<int>> allPermutations = generateAllPermutations(details);
    for (int i = 0; i < allPermutations.size(); i++) {
        vector<vector<int>> currentTable = reorderTableByOrder(table, allPermutations[i]);
        vector<vector<int>> currentTableTij(details, vector<int>(machines + 1));
        int currentTtc = calculateTij(currentTableTij, currentTable);
        if (minTtc > currentTtc) {
            minTtc = currentTtc;
            minT_PermitationsIndex = i;
        }
    }
    cout << "����������� �������: ";
    vector<int> optimalOrder = allPermutations[minT_PermitationsIndex];
    for_each(optimalOrder.begin(), optimalOrder.end(), [](int x) { cout << x << " "; });
    cout << endl;
    vector<vector<int>> optimalTable = reorderTableByOrder(table, optimalOrder);
    printTable(optimalTable, machines);
    vector<vector<int>> optimalTableTij(details, vector<int>(machines + 1));
    int optimalTtc = calculateTij(optimalTableTij, optimalTable);
    vector<int> optimalTableToj = getToj(optimalTableTij, optimalTable);
    vector<int> optimalTableTpr = getTpr(optimalTableTij, optimalTable);
    cout << "������� ���������� ������������ ����� ��������� ������� ��� ������������ �������:" << endl;
    printTijTable(optimalTable, optimalTableTij, optimalTableToj, optimalTableTpr, machines); cout << endl;
    cout << "T�� ������������ ������� = " << optimalTtc << endl;
    cout << "������ ����� ��� ������������ �������:" << endl;
    GanttGraph(optimalTable, optimalTableTij, details, machines);
    cout << endl << endl;

    srand(time(nullptr));
    cout << "��������� ������������������: " << endl;
    vector<int> randomOrder = allPermutations[rand() % (allPermutations.size())];
    for_each(randomOrder.begin(), randomOrder.end(), [](int x) { cout << x << " "; });
    cout << endl;
    vector<vector<int>> randomTable = reorderTableByOrder(table, randomOrder);
    vector<vector<int>> randomTableTij(details, vector<int>(machines + 1));
    int randomTtc = calculateTij(randomTableTij, randomTable);
    vector<int> randomTableToj = getToj(randomTableTij, randomTable);
    vector<int> randomTableTpr = getTpr(randomTableTij, randomTable);
    cout << "������� ���������� ������������ ����� ��������� ������� ��� ���������� �������:" << endl;
    printTijTable(randomTable, randomTableTij, randomTableToj, randomTableTpr, machines); cout << endl;
    cout << "T�� ���������� ������� = " << randomTtc << endl;

    cout << endl << endl;
    return 0;
}