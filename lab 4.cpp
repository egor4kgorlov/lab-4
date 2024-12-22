#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>

#pragma warning(disable : 4996)
using namespace std;


void editText(char* text) {
    
    for (int i = 0; text[i] != '\0'; ++i) {
        if (!isalpha(text[i]) && !isspace(text[i])) {
            text[i] = ' ';
        }
    }
}


void linearSearch(const char* text, const char* pattern) {
    int n = strlen(text);
    int m = strlen(pattern);

    if (m == 0) {
        cout << "Подстрока пустая, невозможно выполнить поиск" << endl;
        return;
    }

    for (int i = 0; i <= n - m; ++i) {
        int j;
        for (j = 0; j < m; ++j) {
            if (text[i + j] != pattern[j]) {
                break;
            }
        }
        if (j == m) {
            cout << "Подстрока найдена на позиции: " << i << endl;
        }
    }
}

void buildBadCharTable(const char* pattern, int m, int badCharTable[]) {
    for (int i = 0; i < 256; ++i) {
        badCharTable[i] = -1;
    }
    for (int i = 0; i < m; ++i) {
        badCharTable[(int)pattern[i]] = i;
    }
}


void boyerMooreSearch(const char* text, const char* pattern) {
    int n = strlen(text);
    int m = strlen(pattern);

    if (m == 0) {
        cout << "Подстрока пустая, невозможно выполнить поиск" << endl;
        return;
    }

    int badCharTable[256];
    buildBadCharTable(pattern, m, badCharTable);

    int i = 0;
    while (i <= n - m) {
        int j = m - 1;
        while (j >= 0 && pattern[j] == text[i + j]) {
            j--;
        }
        if (j < 0) {
            cout << "Подстрока найдена на позиции: " << i << endl;
            i += (i + m < n) ? m - badCharTable[(int)text[i + m]] : 1;
        }
        else {
            i += max(1, j - badCharTable[(int)text[i + j]]);
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    while (true) {
        cout << "Введите с клавиатуры(1) или с файла(2), или 0 для выхода: ";
        int choice;
        if (!(cin >> choice)) {
            cout << "Неверный ввод. Пожалуйста, введите число.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');


        if (choice == 0) {
            break;
        }

        char inputText[1000] = "";
        vector<string> words;


        if (choice == 1) {
            cout << "Введите текст (может не заканчиваться точкой):\n";
            if (!cin.getline(inputText, sizeof(inputText))) {
                cout << "Ошибка ввода" << endl;
                continue;
            }
        }
        else if (choice == 2) {
            char fileName[100];
            cout << "Введите имя файла:\n";
            if (!cin.getline(fileName, sizeof(fileName))) {
                cout << "Ошибка ввода" << endl;
                continue;
            }

            ifstream inputFile(fileName);
            if (!inputFile.is_open()) {
                cerr << "Ошибка открытия файла!" << endl;
                continue;
            }

            if (!inputFile.getline(inputText, sizeof(inputText))) {
                cout << "Ошибка чтения из файла!" << endl;
                inputFile.close();
                continue;
            }
            inputFile.close();
            cout << "Текст из файла: " << inputText << endl;
        }
        else {
            cerr << "Неверный выбор!" << endl;
            continue;
        }

        editText(inputText);
        cout << "Отредактированный текст: " << inputText << endl;
        stringstream ss(inputText);
        string word;
        while (ss >> word) {
            words.push_back(word);
        }

        cout << "Слова в обратном порядке: ";
        for (int i = words.size() - 1; i >= 0; --i) {
            cout << words[i] << ' ';
        }
        cout << endl;

        cout << "Количество символов в каждом слове:" << endl;
        for (const auto& word : words) {
            cout << word << ": " << word.length() << " символов" << endl;
        }


        char text[1000];
        char pattern[1000];

        cout << "Введите строку для линейного поиска: ";
        if (!cin.getline(text, 1000)) {
            cout << "Ошибка ввода!" << endl;
            continue;
        }
        cout << "Введите подстроку для поиска: ";
        if (!cin.getline(pattern, 1000)) {
            cout << "Ошибка ввода!" << endl;
            continue;
        }
        cout << "Результаты линейного поиска:" << endl;
        linearSearch(text, pattern);

        cout << "Введите строку для поиска Бойера-Мура: ";
        if (!cin.getline(text, 1000)) {
            cout << "Ошибка ввода!" << endl;
            continue;
        }
        cout << "Введите подстроку для поиска: ";
        if (!cin.getline(pattern, 1000)) {
            cout << "Ошибка ввода!" << endl;
            continue;
        }
        cout << "Результаты алгоритма Бойера-Мура:" << endl;
        boyerMooreSearch(text, pattern);
    }

    return 0;
}
  


