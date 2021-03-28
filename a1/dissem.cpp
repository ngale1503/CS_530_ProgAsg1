#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;.

/* -------------------------------------------------------------------------- */
/*                                  Operands                                  */
/* -------------------------------------------------------------------------- */
const static string ops[] = {
"18", "58", "90", "40", "B4", "28",
"88", "A0", "24", "64", "9C", "C4",
"C0", "F4", "3C", "30", "34", "38",
"48", "00", "68", "50", "70", "08",
"6C", "74", "04", "D0", "20", "60",
"98", "C8", "44", "D8", "AC", "4C",
"A4", "A8", "F0", "EC", "0C", "78",
"54", "80", "D4", "14", "7C", "E8",
"84", "10", "1C", "5C", "94", "B0",
"E0", "F8", "2C", "B8", "DC"
};


/* -------------------------------------------------------------------------- */
/*                             Operand Translation                            */
/* -------------------------------------------------------------------------- */

const static string mnemonics[] = {
"ADD", "ADDF", "ADDR", "AND", "CLEAR", "COMP",
"COMPF", "COMPR", "DIV", "DIVF", "DIVR", "FIX",
"FLOAT", "HIO", "J", "JEQ", "JGT", "JLT",
"JSUB", "LDA", "LDB", "LDCH", "LDF", "LDL",
"LDS", "LDT", "LDX", "LPS", "MUL", "MULF",
"MULR", "NORM", "OR", "RD", "RMO", "RSUB",
"SHIFTL", "SHIFTR", "SIO", "SSK", "STA", "STB",
"STCH", "STF", "STI", "STL","STS", "STSW",
"STT", "STX", "SUB", "SUBF", "SUBR", "SVC",
"TD", "TIO", "TIX", "TIXR", "WD"
};


/* -------------------------------------------------------------------------- */
/*                                   Writer                                   */
/* -------------------------------------------------------------------------- */
bool writer(string input)
{
    fstream outFile;
    outFile.open("out.lst", ios::out);
    if (!outFile)
    {
        return false;
    }
    else
    {
        outFile << input;
        outFile.close();
        return true;
    }
    return false;
}

/* -------------------------------------------------------------------------- */
/*                                   Reader                                   */
/* -------------------------------------------------------------------------- */
/**
 * Take a given file name and a storage vector/array, read the file from file name
 * and store it in the vector/array. Stores it one line per row.
 */
bool reader(string filename, vector<string> &fileStringArray)
{
    /** Read the obj file and store each line as string in strArray */
    std::ifstream fileContainer(filename);
    if (fileContainer.is_open())
    {
        std::string line;
        while (std::getline(fileContainer, line))
        {
            fileStringArray.push_back(line.c_str());
        }
        fileContainer.close();
        return true;
    }
    else
    {
        cout << "error opeing symFile\n";
        exit(EXIT_FAILURE);
    }
}

/* -------------------------------------------------------------------------- */
/*                                Parse Header                                */
/* -------------------------------------------------------------------------- */
/**
 * Take the header record extract the name and add it to the pointer to the outFile
 * Out file is the final string that will be printed.
 */
bool parseHeader(string headerString, string &outFile){
    string spacer = "     ";
    string name = headerString.substr (1,6);
    outFile = "0000" + spacer + name + spacer + spacer + "START"+ spacer + "0\n";
    return true;
}

/* -------------------------------------------------------------------------- */
/*                              Parse Text Record                             */
/* -------------------------------------------------------------------------- */
/** Out file is the final string that will be printed. */
bool parseTextRecord(string headerString, string &outFile){

    return true;
}

/* -------------------------------------------------------------------------- */
/*                          Parse Modification Record                         */
/* -------------------------------------------------------------------------- */
bool parseModificationRecord(string headerString, string &outFile){

    return true;
}


/* -------------------------------------------------------------------------- */
/*                              Parse End Record                              */
/* -------------------------------------------------------------------------- */
bool parseEndRecord(string headerString, string &outFile){
    string spacer = "     ";
    outFile += spacer + spacer + "END" + spacer + "First";
    return true;
}

/* -------------------------------------------------------------------------- */
/*                                 Main Parser                                */
/* -------------------------------------------------------------------------- */
/**
  * takes the object code and splits it into header, text, modification and end record.
  * After spliting it sends it to its individual parsers
  */
void mainParser(vector<string> objArray, string &outLstStr){
    /** loop through the obj file and read in the translations */
    for(int i = 0; i < objArray.size(); i += 1){
        string line = objArray.at(i);
        char lineType = line[0];
        switch (lineType)
        {
        case 'H':
            parseHeader(line, outLstStr);
            break;
        case 'T':
            /** TODO */
            parseTextRecord(line, outLstStr);
            break;
        case 'M':
            /** TODO */
            parseModificationRecord(line, outLstStr);
            break;
        case 'E':
            /** TODO */
            parseEndRecord(line, outLstStr);
            break;
        default:
            cout << "Unsupported type: " << line << "\n";
            exit(EXIT_FAILURE);
        }
    }
}

/* -------------------------------------------------------------------------- */
/*                                    MAIN                                    */
/* -------------------------------------------------------------------------- */

int main(int argc, char const *argv[])
{

/* ------------------------------- DEFINITIONS ------------------------------ */

    /** Stores file name for sym file and obj file. */
    string symFileString;
    string objFileString;

    /*Temp Name Storage*/
    string inputFileName1 = argv[1];
    string inputFileName2 = argv[2];

    /** Storage for the files that have already been read */
    vector<string> objArray;
    vector<string> symArray;

    /** Final string that contains the out.lst */
    string outLstStr;


/* ------------------------------ Program Start ----------------------------- */

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
    /** Read the file from first pram and store each line as array in second file */
    reader(symFileString, symArray);
    reader(objFileString, objArray);

    /** Parse the object codes. */
    mainParser(objArray, outLstStr);

    cout << outLstStr << "\n";

    writer(outLstStr);
}
