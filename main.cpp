#include <iostream>
#include <fstream>
#include <filesystem>

#define ChunkSize 4096

using namespace std;
namespace fs = std::filesystem;

bool GetFirstAnswer() {
    string Answer;
    cout << "Encrypt? (y/n) ";
    cin >> Answer;
    if (Answer == "y" || Answer == "Y") {
        return true;
    } else if (Answer == "n" || Answer == "N") {
        return false;
    } else {
        cout << "Wrong\n";
        return GetFirstAnswer();
    }
}

void CheckAndMakeFile(string FileName) {
    fs::path filePath = FileName;
    if (!fs::exists(filePath)) {
        ofstream NewEncrypt(FileName);
        NewEncrypt.close();
        if (FileName != "encrypt") {
            cout << "Failed: File does not exist, Making a new one named: ";
            cout << FileName;
            cout << "\n";
        }
    }
}

void EncryptFunc(int key, string FlieName) {
    char c;
    fstream fin, fout;
    fs::path Path = FlieName;
    if (!fs::exists(Path)) {
        cout << "Failed: File does not exist";
        cout << "\n";
        return;
    }
    fin.open(FlieName, fstream::in);
    fout.open("encrypt", fstream::out);
    
    int pre = 0;
    while (fin >> noskipws >> c) {
        int AsciiKey = (c + key + pre); //qip
        fout << (char)AsciiKey;
        pre = c;
    }

    fin.close();
    fout.close();
}

void DecryptFunc(int key, string FlieName) {
    char c;
    fstream fin, fout;

    CheckAndMakeFile(FlieName);
    fout.open("encrypt", fstream::in); 
    fin.open(FlieName, fstream::out);

    int pre = 0;
    while (fout >> noskipws >> c) {
        int AsciiKey = (c - key - pre);
        fin << (char)AsciiKey;
        pre = (char)AsciiKey;
    }

    fin.close();
    fout.close(); 
}

int main() {
    bool Encrypt = GetFirstAnswer();
    if (Encrypt) cout << "File to Encrypt: ";
    else if (!Encrypt) cout << "File to Decrypt to: ";
    string FlieName;
    cin >> FlieName;

    std::string Password;
    cout << "Password: ";
    cin >> Password;
    char *Pass = Password.data();

    int key = 1;
    for (int i = 0; i < sizeof(Pass) / sizeof(Pass[0]); i++) key += (int)Pass[i];
    key *= sizeof(Pass);

    CheckAndMakeFile("encrypt");

    if (Encrypt) EncryptFunc(key, FlieName);
    else DecryptFunc(key, FlieName);

    cout << "Finished!\n";
    return 0;
}