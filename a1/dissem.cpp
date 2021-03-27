#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

/* -------------------------------------------------------------------------- */
/*                                   Writer                                   */
/* -------------------------------------------------------------------------- */
bool writer(string input){
    fstream outFile;
	outFile.open("out.lst", ios::out);
	if (!outFile) {
		return false;
	}
	else {
        outFile << input;
		outFile.close(); 
        return true;
	}
	return false;
}


/* -------------------------------------------------------------------------- */
/*                                    MAIN                                    */
/* -------------------------------------------------------------------------- */

int main(int argc, char const *argv[])
{
    //DEFINITIONS

    /** Stores file name for sym file and obj file. */
    string symFileString;
    string objFileString;

    /*Temp Name Storage*/
    string inputFileName1 = argv[1];
    string inputFileName2 = argv[2];

    /** Storage for the files that have already been read */
    vector<string> objArray;
    vector<string> symArray;

    /** Check if the user entered both files */
    if (argc < 2)
    {
        cout << "Please enter missing input symFile/s\n";
        exit(EXIT_FAILURE);
    }

    /** Check file order and put them in correct symFileString and objFileString */
    if (inputFileName1[inputFileName1.length() - 1] == 'j' && inputFileName2[inputFileName2.length() - 1] == 'm')
    {
        objFileString = argv[1];
        symFileString = argv[2];
    }
    else if (inputFileName1[inputFileName1.length() - 1] == 'm' && inputFileName2[inputFileName2.length() - 1] == 'j')
    {
        objFileString = argv[2];
        symFileString = argv[1];
    }
    else
    {
        cout << "Please check symFile extention and try again. usage: ./dissem symFile.sym symFile.obj\n";
        exit(EXIT_FAILURE);
    }

    /** Read the obj file and store each line as string in strArray */
    std::ifstream symFile(symFileString);
    if (symFile.is_open())
    {
        std::string line;
        while (std::getline(symFile, line))
        {
            symArray.push_back(line.c_str());
            cout << "\n";
        }
        symFile.close();
    }
    else
    {
        cout << "error opeing symFile\n";
    }


    /** Read the obj file and store each line as string in objArray */
    std::ifstream objFile(objFileString);
    if (objFile.is_open())
    {
        std::string line;
        while (std::getline(objFile, line))
        {
            printf("%s", line.c_str());
            objArray.push_back(line.c_str());
            cout << "\n";
        }
        objFile.close();
    }
    else
    {
        cout << "error opeing symFile\n";
    }

    writer(symArray.at(0));
}

