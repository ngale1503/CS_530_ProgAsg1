#include <unistd.h>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
std::stringstream ss;

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
    "E0", "F8", "2C", "B8", "DC"};

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
    "STCH", "STF", "STI", "STL", "STS", "STSW",
    "STT", "STX", "SUB", "SUBF", "SUBR", "SVC",
    "TD", "TIO", "TIX", "TIXR", "WD"};

/* -------------------------------------------------------------------------- */
/*                                 Hex To Int                                 */
/* -------------------------------------------------------------------------- */
// Convert hexadecimal string to int.
int hexToInt(string hexStr){
    unsigned int number;   
    std::stringstream ss;
    ss << std::hex << hexStr;
    ss >> number;
    return static_cast<int>(number);
}

/* -------------------------------------------------------------------------- */
/*                                 Int To Hex                                 */
/* -------------------------------------------------------------------------- */
// Convert Intreger to Hexadecimal.
string intToHex(int number){
    std::stringstream stream;
    stream << std::hex << number;
    return stream.str();
}


/* -------------------------------------------------------------------------- */
/*                               Program Counter                              */
/* -------------------------------------------------------------------------- */
/** Keeps count of the position the program is in. */
class COUNTER{
    private:
        string currentPositon;

    public:
        COUNTER();
        string set(string);
        string get();
        string add(string);
        string subtract(string);
};
COUNTER::COUNTER(){
}
string COUNTER::add(string hexNumber){
    int currentPositionInt = hexToInt(COUNTER::currentPositon);
    int summand = hexToInt(hexNumber);
    int newCurrPosition = currentPositionInt + summand;
    currentPositon = intToHex(newCurrPosition);
    return currentPositon;
}
string COUNTER::subtract(string hexNumber){
    int currentPositionInt = hexToInt(COUNTER::currentPositon);
    int subtrahend = hexToInt(hexNumber);
    int newCurrPosition = currentPositionInt - subtrahend;
    currentPositon = intToHex(newCurrPosition);
    return currentPositon;
}
string COUNTER::get(){
    return COUNTER::currentPositon;
}
string COUNTER::set(string hexNumber){
    currentPositon = hexNumber;
    return currentPositon;
}

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
bool parseHeader(string headerString, string &outFile, COUNTER &counter)
{
    string spacer = "     ";
    string name = headerString.substr(1, 6);
    string startingAddress = headerString.substr(7, 12);
    counter.set(startingAddress);
    outFile = "0000" + spacer + name + spacer + spacer + "START" + spacer + "0\n";
    return true;
}

/* -------------------------------------------------------------------------- */
/*                              Parse Text Record                             */
/* -------------------------------------------------------------------------- */
/** Out file is the final string that will be printed. */
bool parseTextRecord(string textLine, string &outFile, COUNTER &counter)
{
    string startingAddress = textLine.substr(1, 6);
    
/* ------------------------------ Error Checker ----------------------------- */
    // Temperoray check to see if we are on the right track.
    // if(counter.get() != startingAddress){
    //     cout << "ERROR POSITION DO NOT MATCH!\n";
    //     cout << "Expected address: " << startingAddress << ". Got address: " << counter.get() << "\n";
    // }
    // return true;
    return true;
}

/* -------------------------------------------------------------------------- */
/*                          Parse Modification Record                         */
/* -------------------------------------------------------------------------- */
bool parseModificationRecord(string modificationLine, string &outFile, COUNTER &counter)
{

    return true;
}

/* -------------------------------------------------------------------------- */
/*                              Parse End Record                              */
/* -------------------------------------------------------------------------- */
bool parseEndRecord(string endLine, string &outFile, COUNTER &counter)
{
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
void mainParser(vector<string> objArray, string &outLstStr, COUNTER &counter)
{
    /** loop through the obj file and read in the translations */
    for (int i = 0; i < objArray.size(); i += 1)
    {
        string line = objArray.at(i);
        char lineType = line[0];
        switch (lineType)
        {
        case 'H':
            parseHeader(line, outLstStr, counter);
            break;
        case 'T':
            /** TODO */
            parseTextRecord(line, outLstStr, counter);
            break;
        case 'M':
            /** TODO */
            parseModificationRecord(line, outLstStr, counter);
            break;
        case 'E':
            /** TODO */
            parseEndRecord(line, outLstStr, counter);
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

    /* ---------------------------- Check Valid Input --------------------------- */

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

    /* ------------------------------ Program Start ----------------------------- */

    /** Read the file from first pram and store each line as array in second file */
    reader(symFileString, symArray);
    reader(objFileString, objArray);

    /** Create a new counter and attach a *counter pointer to it. */
    COUNTER *counter = new COUNTER;

    /** Parse the object codes. */
    mainParser(objArray, outLstStr, *counter);

    cout << outLstStr << "\n";

    writer(outLstStr);
}
