#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char const *argv[])
{
    // Check if the user entered both files
    if (argc < 2)
    {
        cout << "Please enter missing input symFile/s\n";
        exit(EXIT_FAILURE);
    }

    string symFileString;
    string objFileString;
    string file1 = argv[1];
    string file2 = argv[2];

    if (file1[file1.length() - 1] == 'j' && file2[file2.length() - 1] == 'm')
    {
        objFileString = argv[1];
        symFileString = argv[2];
    }
    else if (file1[file1.length() - 1] == 'm' && file2[file2.length() - 1] == 'j')
    {
        objFileString = argv[2];
        symFileString = argv[1];
    }
    else
    {
        cout << "Please check symFile extention and try again. usage: ./dissem symFile.sym symFile.obj\n";
        exit(EXIT_FAILURE);
    }


    std::ifstream symFile(symFileString);
    if (symFile.is_open())
    {
        std::string line;
        while (std::getline(symFile, line))
        {
            printf("%s", line.c_str());
            cout << "\n";
        }
        symFile.close();
    }else
    {
        cout << "error opeing symFile\n";
    }

    std::ifstream objFile(objFileString);
    if (objFile.is_open())
    {
        std::string line;
        while (std::getline(objFile, line))
        {
            printf("%s", line.c_str());
            cout << "\n";
        }
        objFile.close();
    }else
    {
        cout << "error opeing symFile\n";
    }
}
