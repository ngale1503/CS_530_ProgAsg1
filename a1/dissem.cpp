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
/*                             OpCode To Mnemonic                             */
/* -------------------------------------------------------------------------- */
string opcodeToMnemonic(string opcode)
{
    for (int i = 0; i < 60; i++)
    {
        if (ops[i] == opcode)
        {
            return string(mnemonics[i]);
        }
    }
    cout << "ERROR: OPCODE NOT FOUND! OPCODE WITH THE STRING: " << opcode << " WAS NOT FOUND.\n";
    exit(EXIT_FAILURE);
}

/* -------------------------------------------------------------------------- */
/*                            Operand Type 1,2,3,4                            */
/* -------------------------------------------------------------------------- */
const static int instructionType[] = {
    3, 3, 2, 3, 2, 3,
    3, 2, 3, 3, 2, 1,
    1, 1, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3,
    2, 1, 3, 3, 2, 3,
    2, 2, 1, 3, 3, 3,
    3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 2, 2,
    3, 1, 3, 2, 3};

/* -------------------------------------------------------------------------- */
/*                             Check Type 3 Format                            */
/* -------------------------------------------------------------------------- */
/* -- Checks if an operand instruction is type 3 and return true or false. -- */
bool isType3(string opcode)
{
    for (int i = 0; i < 60; i++)
    {
        if (ops[i] == opcode)
        {
            if (instructionType[i] == 3)
            {
                return true;
            }
            return false;
        }
    }
    cout << "ERROR: OPCODE NOT FOUND!\n OPCODE WITH THE STRING: " << opcode << " WAS NOT FOUND. CAN NOT RETURN INSTRUCTION TYPE\n";
    exit(EXIT_FAILURE);
}

/* -------------------------------------------------------------------------- */
/*              Check if instruction is type 4 [EXTENTED FORMAT]              */
/* -------------------------------------------------------------------------- */
/* --- Given nixbpe it return true is the instruction is type 4 else false -- */
bool isType4(string nixbpe)
{
    char e = nixbpe[5];
    if (e == '1')
        return true;
    if (e == '0')
        return false;
    cout << "Error: Could not tell type of 'e' in 'nixbpe', value of e was read as " << e << "\n";
    exit(EXIT_FAILURE);
}

/* -------------------------------------------------------------------------- */
/*                                 Hex To Int                                 */
/* -------------------------------------------------------------------------- */
/** Convert hexadecimal string to int. */
int hexToInt(string hexStr)
{
    unsigned int number;
    std::stringstream ss;
    ss << std::hex << hexStr;
    ss >> number;
    return static_cast<int>(number);
}

/* -------------------------------------------------------------------------- */
/*                                 Int To Hex                                 */
/* -------------------------------------------------------------------------- */
/** Convert Intreger to Hexadecimal. */
string intToHex(int number)
{
    std::stringstream stream;
    stream << std::hex << number;
    return stream.str();
}

/* -------------------------------------------------------------------------- */
/*                               Hex To Binnary                               */
/* -------------------------------------------------------------------------- */
string hexToBininary(const string &s)
{
    string out;
    for (auto i : s)
    {
        uint8_t n;
        if (i <= '9' and i >= '0')
            n = i - '0';
        else
            n = 10 + i - 'A';
        for (int8_t j = 3; j >= 0; --j)
            out.push_back((n & (1 << j)) ? '1' : '0');
    }
    return out;
}

/* -------------------------------------------------------------------------- */
/*                                Binary To Hex                               */
/* -------------------------------------------------------------------------- */

string binarytohex(const string &s)
{
    string out;
    for (uint i = 0; i < s.size(); i += 4)
    {
        int8_t n = 0;
        for (uint j = i; j < i + 4; ++j)
        {
            n <<= 1;
            if (s[j] == '1')
                n |= 1;
        }

        if (n <= 9)
            out.push_back('0' + n);
        else
            out.push_back('A' + n - 10);
    }

    return out;
}

/* -------------------------------------------------------------------------- */
/*                               Program Counter                              */
/* -------------------------------------------------------------------------- */
/** Keeps count of the position the program is in. */
class COUNTER
{
private:
    string positionCounter;
    string relativeCounter;

public:
    COUNTER();
    string set(string);
    string get();
    string add(string);
    string subtract(string);
};
COUNTER::COUNTER()
{
}
string COUNTER::add(string hexNumber)
{
    int currentPosition = hexToInt(COUNTER::positionCounter);
    int summand = hexToInt(hexNumber);
    int newCurrPosition = currentPosition + summand;
    positionCounter = intToHex(newCurrPosition);
    return positionCounter;
}
string COUNTER::subtract(string hexNumber)
{
    int currentPosition = hexToInt(COUNTER::positionCounter);
    int subtrahend = hexToInt(hexNumber);
    int newCurrPosition = currentPosition - subtrahend;
    positionCounter = intToHex(newCurrPosition);
    return positionCounter;
}
string COUNTER::get()
{
    return COUNTER::positionCounter;
}
string COUNTER::set(string hexNumber)
{
    positionCounter = hexNumber;
    return positionCounter;
}

/* -------------------------------------------------------------------------- */
/* ------------------------- MEMORY ADDRESSING CLASS ------------------------ */
/* -------------------------------------------------------------------------- */
/* ------- Hold addressing values of memory and holds the address type ------ */
/**
 * Addressing type: [simple, indirect, immediate]
 * Addressing value: [0007, 700, A, 5]
 */
class MEMORYADDRESS
{
private:
    string type;
    string value;

public:
    string get();
    void setType(string);
    void setValue(string);
};
string MEMORYADDRESS::get()
{
    if (!type.empty() && !value.empty())
    {
        string finalValue;
    }
    return "Error either the type or value was empty for the addressing class\n";
}
void MEMORYADDRESS::setType(string newType)
{
    type = newType;
}
void MEMORYADDRESS::setValue(string newValue)
{
    type = newValue;
}

/* -------------------------------------------------------------------------- */
/*                                  OUT CLASS                                 */
/* -------------------------------------------------------------------------- */
/* ----------------- Stores the final values to be outputed ----------------- */
class OUTPUT
{
private:
    string address;
    string symbol;
    string instruction;
    MEMORYADDRESS location;
    string opcode;
    string nixbpe;

public:
    void setAddress(string);
    void setSymbol(string);
    void setInstruction(string);
    void setLocation(string, string);
    void setOpcode(string);
    void setNixbpe(string);
    void print();
};
void OUTPUT::setAddress(string newValue)
{
    address = newValue;
}
void OUTPUT::setSymbol(string newValue)
{
    symbol = newValue;
}
void OUTPUT::setInstruction(string newValue)
{
    instruction = newValue;
}
void OUTPUT::setLocation(string type, string value)
{
    location;
    location.setType(type);
    location.setValue(value);
}
void OUTPUT::setOpcode(string newValue)
{
    opcode = newValue;
}
void OUTPUT::setNixbpe(string newValue)
{
    nixbpe = newValue;
}

void OUTPUT::print()
{
    cout << "Address: " << address << "\n symbol: " << symbol << "\n instruction: " << instruction << "\nlocation: " << location.get() << " opcode: " << opcode << "\n nixbpe: " << nixbpe << "\n";
    cout << "-------------------------------------------------\n";
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
/*                     Convert OP Code to Instruction Type                    */
/* -------------------------------------------------------------------------- */
/* ---- Given an op code it analyses it and returns the instruction type 2,3,4 ---- */
int opCodeToType(string opCode)
{
    for (int i = 0; i < 60; i++)
    {
        if (opCode == ops[i])
        {
            return instructionType[i];
        }
    }
    cout << "ERROR: Instruction type not found.\n";
    exit(EXIT_FAILURE);
}

/* -------------------------------------------------------------------------- */
/*                Addressing Type [Simple, Immediate, Indirect]               */
/* -------------------------------------------------------------------------- */
/**
 * input: nixbpe
 * output [Simple, Immediate, Indirect]
 */
string addressingType(string nixbpe)
{
    char i = nixbpe[1];
    char n = nixbpe[0];
    if (i == '0')
    {
        if (n == '0')
            return "simple";
        if (n == '1')
            return "indirect";
    }
    if (i == '1')
    {
        if (n == '0')
            return "simple";
        if (n == '1')
            return "immediate";
    }
    cout << "Error: Could not find addressing type. '" << nixbpe << "' is not a valid value\n";
    exit(EXIT_FAILURE);
}

/* -------------------------------------------------------------------------- */
/*                         Convert OP Code To Mnemonic                        */
/* -------------------------------------------------------------------------- */
/* ---------- Given an op code convert that to the correct mnemonic --------- */
string opCodeToMnemonic(string opCode)
{
    for (int i = 0; i < 59; i++)
    {
        if (opCode == ops[i])
        {
            return mnemonics[i];
        }
    }
    cout << "ERROR: Mnemonic not found.\n";
    exit(EXIT_FAILURE);
}

/* -------------------------------------------------------------------------- */
/*                                Parse Opcodes                               */
/* -------------------------------------------------------------------------- */
vector<OUTPUT> parseOpCodes(string opcodes, vector<OUTPUT> &opcodesArray, string startingAddress)
{
    OUTPUT outputValue;
    COUNTER textLocationCounter;


    if (opcodes.length() < 4)
    {
        return opcodesArray;
    }

    textLocationCounter.set(startingAddress);
    /** store the current address to be printed out later */
    outputValue.setAddress(textLocationCounter.get());
    /**
     * take the first two letters of the opcode and convert to binary
     * 6910083E174000024000 -> 69 -> 01101001
     */
    string opCodeAsBinary = hexToBininary(opcodes.substr(0, 2));

    /**
     * first 6 bits are opcode 011010 and the last 2 bits are ni of nixbpe
     * remove ni and get opcode as 01101000
     */
    string opCodeRemovedExtras = opCodeAsBinary.substr(0, 6) + "00";

    /** convert the binary value to hex 01101000 -> 68 */
    string opCodeAsHex = binarytohex(opCodeRemovedExtras);

    if (isType3(opCodeAsHex))
    {

        /**
        * look up 68 as menmonic 68 -> LDB
        */
        string mnemonic = opcodeToMnemonic(opCodeAsHex);
        /** store mnemonic in the output class */
        outputValue.setInstruction(mnemonic);

        /** Find nixbpe value */
        string ni = opCodeAsBinary.substr(6, 2);
        string xbpe = hexToBininary(opcodes.substr(2, 1));
        string nixbpe = ni + xbpe;

        /**
         * Set the full opcode value from the opcodes string.
         * This includes all 6 values for type 3,
         * and 8 values for type 4.
         */
        string fullOpcode = opcodes.substr(0, 6);
        outputValue.setOpcode(fullOpcode);

        /** store nixbpe in the output class for final output. */
        outputValue.setNixbpe(nixbpe);
        /** Check if instruction is extended format. */
        if (isType4(nixbpe))
        {
            /** If it is extended format update the mnemonic previously stored. */
            mnemonic = "+" + mnemonic;
            outputValue.setInstruction(mnemonic);
            cout <<"TYPE4 MNWMONIX " << mnemonic << "\n";
            /**
             * If extended format update opcode from 6 to 8 values.
             * EG. 691008 -> 6910083E
             */
            string fullType4Opcode = opcodes.substr(0, 8);
            outputValue.setOpcode(fullType4Opcode);

            /** TYPE4: recursively recall the function but remove the address that was just analysed */
            string newOpcodes = opcodes.substr(8, opcodes.length() - 8);
            /** store the current address to be printed out later */
            textLocationCounter.add("4");
            outputValue.print();
            parseOpCodes(newOpcodes, opcodesArray, textLocationCounter.get());
        }
        else
        {
            /** TYPE3: recursively recall the function but remove the address that was just analysed */
            string newOpcodes = opcodes.substr(6, opcodes.length() - 6);
            textLocationCounter.add("3");
            outputValue.print();
            parseOpCodes(opcodes.substr(6, opcodes.length()), opcodesArray, textLocationCounter.get());
        }
    }
    else
    {
        /** TYPE2: recursively recall the function but remove the address that was just analysed */
        string newOpcodes = opcodes.substr(4, opcodes.length() - 4);
        string mnemonic = opcodeToMnemonic(opCodeAsHex);
        outputValue.setInstruction(mnemonic);
        textLocationCounter.add("2");
        outputValue.print();
        parseOpCodes(opcodes.substr(4, opcodes.length()), opcodesArray, textLocationCounter.get());
    }
    
    return opcodesArray;
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

    string startingAddress = headerString.substr(7, 6);
    string endingAddress = headerString.substr(13, 6);

    int startingAddressAsInt = hexToInt(startingAddress);
    int endingAddressAsInt = hexToInt(endingAddress);

    string programSizeInBytes = intToHex(endingAddressAsInt - startingAddressAsInt + 1);

    outFile = "0000" + spacer + name + spacer + spacer + "START" + spacer + "0\n";
    return true;
}

/* -------------------------------------------------------------------------- */
/*                              Parse Text Record                             */
/* -------------------------------------------------------------------------- */
/** Out file is the final string that will be printed. */
bool parseTextRecord(string textLine, string &outFile, COUNTER &counter, vector<vector<int>> modificationsArray)
{

    string startingAddress = textLine.substr(1, 6);

    int opCodeLengthAsInt = hexToInt(textLine.substr(7, 2));

    string allOpCodes = textLine.substr(9, opCodeLengthAsInt * 2);

    vector<OUTPUT> opcodesArray;

    parseOpCodes(allOpCodes, opcodesArray, startingAddress);
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
/*            Filter Modification lines and extract type 4 location           */
/* -------------------------------------------------------------------------- */
/**
 * input: [H**,T**,M00000105,M00085905, E**]
 * output: [[M000001, 05],[000859, 05]]
 */
vector<vector<int>> extractModificationRecords(vector<string> &objArray)
{
    vector<vector<int>> modificationsArray;
    int modificationRecordsCount = 0;
    /** loop through the obj file and read in the translations */
    for (int i = 0; i < objArray.size(); i += 1)
    {
        string line = objArray.at(i);
        char lineType = line[0];
        if (lineType == 'M')
        {

            int location = stoi(line.substr(1, 6));
            int modificationLength = stoi(line.substr(7, 2));

            /** Create an array that stores location of type 4 and the length to be modified. */
            vector<int> newModificationRecord;
            newModificationRecord.push_back(location);
            newModificationRecord.push_back(modificationLength);

            /** push the new array on to all type 4 arrays */
            modificationsArray.push_back(newModificationRecord);

            modificationRecordsCount++;
        }
    }
    return modificationsArray;
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
    /**
     * 2D array contains extracted modification records [M 000001 05] [M 000859 05]:
     *      [[where to modify, length of address field to be modified in half bytes], [..], ..]
     *      EX: [[M000001, 05],[000859, 05]]
     */
    vector<vector<int>> modificationsArray = extractModificationRecords(objArray);

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
            parseTextRecord(line, outLstStr, counter, modificationsArray);
            break;
        case 'E':
            /** TODO */
            parseEndRecord(line, outLstStr, counter);
            break;
        case 'M':
            /** Do Nothing */
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

    /** Check file order and put them in correct order symFileString and objFileString */
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

    writer(outLstStr);
}
