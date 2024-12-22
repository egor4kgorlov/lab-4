#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cctype>
#include <locale>

#pragma warning(disable : 4996)
using namespace std;

void preprocessBadChar(const char* pattern, unordered_map<char, int>& badChar) {
    int patternLength = strlen(pattern);
    for (int i = 0; i < patternLength; i++) {
        badChar[pattern[i]] = i;
    }
}


void boyerMooreSearch(const char* text, const char* pattern) {
    int textLength = strlen(text);
    int patternLength = strlen(pattern);
    unordered_map<char, int> badChar;

    preprocessBadChar(pattern, badChar);

    vector<int> foundPositions;
    int shift = 0;
    while (shift <= textLength - patternLength) {
        int j = patternLength - 1;

        while (j >= 0 && pattern[j] == text[shift + j]) {
            j--;
        }

        if (j < 0) {
            foundPositions.push_back(shift);
            shift += (shift + patternLength < textLength) ? patternLength - badChar[text[shift + patternLength]] : 1;
        }
        else {
            shift += max(1, j - badChar[text[shift + j]]);
        }
    }

    if (foundPositions.empty()) {
        cout << "Подстрока не найдена." << endl;
    }
    else {
        cout << "Подстрока найдена в позициях: ";
        for (size_t i = 0; i < foundPositions.size(); ++i) {
            cout << foundPositions[i] << (i == foundPositions.size() - 1 ? "" : ", ");
        }
        cout << endl;
    }

}

void removeExtraSpaces(char* inputText) {
    char temp[1000];
    int j = 0;
    bool inSpace = false;
    for (int i = 0; inputText[i] != '\0'; ++i) {
        if (inputText[i] != ' ') {
            temp[j++] = inputText[i];
            inSpace = false;
        }
        else if (!inSpace) {
            temp[j++] = ' ';
            inSpace = true;
        }
    }
    temp[j] = '\0';
    strcpy(inputText, temp);
}


void removeExtraPunctuation(char* inputText) {
    char temp[1000];
    int j = 0;

    for (int i = 0; inputText[i] != '\0'; ++i) {
        if (ispunct(inputText[i])) {
            if (i == 0 || !ispunct(inputText[i - 1])) {
                temp[j++] = inputText[i];
            }
        }
        else {
            temp[j++] = inputText[i];
        }
    }
    temp[j] = '\0';
    strcpy(inputText, temp);
}

void correctCase(char* inputText) {
    bool newSentence = true;

    for (int i = 0; inputText[i] != '\0'; ++i) {
        if (newSentence && isalpha(inputText[i])) {
            inputText[i] = toupper(inputText[i]);
            newSentence = false;
        }
        else {
            inputText[i] = tolower(inputText[i]);
        }

        if (inputText[i] == '.' || inputText[i] == '!' || inputText[i] == '?') {
            newSentence = true;
        }
    }
}

void editText(char* inputText) {
    removeExtraSpaces(inputText);
    removeExtraPunctuation(inputText);
    correctCase(inputText);
}


void linearSearch(const char* text, const char* pattern) {
    int textLength = strlen(text);
    int patternLength = strlen(pattern);

    if (patternLength == 0) {
        cout << "Подстрока пустая, невозможно выполнить поиск" << endl;
        return;
    }
    vector<int> foundPositions;

    for (int i = 0; i <= textLength - patternLength; i++) {
        int j;
        for (j = 0; j < patternLength; j++) {
            if (text[i + j] != pattern[j]) {
                break;
            }
        }
        if (j == patternLength) {
            foundPositions.push_back(i);
        }
    }

    if (foundPositions.empty()) {
        cout << "Подстрока не найдена." << endl;
    }
    else {
        cout << "Подстрока найдена в позициях: ";
        for (size_t i = 0; i < foundPositions.size(); ++i) {
            cout << foundPositions[i] << (i == foundPositions.size() - 1 ? "" : ", ");
        }
        cout << endl;
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
                cout << "Ошибка чтения из файла" << endl;
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

        cout << "Введите строку для поиска: ";
        if (!cin.getline(text, 1000)) {
            cout << "Ошибка ввода" << endl;
            continue;
        }
        cout << "Введите подстроку для поиска: ";
        if (!cin.getline(pattern, 1000)) {
            cout << "Ошибка ввода" << endl;
            continue;
        }
        cout << "Результаты линейного поиска:" << endl;
        linearSearch(text, pattern);
        cout << "Результаты алгоритма Бойера-Мура:" << endl;
        boyerMooreSearch(text, pattern);
    }

    return 0;
}