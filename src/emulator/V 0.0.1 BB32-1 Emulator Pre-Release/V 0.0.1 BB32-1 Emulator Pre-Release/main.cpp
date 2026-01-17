#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <thread>

namespace fs = std::filesystem;
using namespace std;
using namespace chrono_literals;
bool emulating = false;
bool select_file = true;

int addr = 0;
int registers[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int RAM[16777216];
string ROM[16777216];
bool input[16];
bool output[16];
string contents;
int ind;
int reg_block = 1;
bool carry;
bool zero[2];
bool ecl_data[36];

void ecl_send_data(string data) {
    int i = 0;
    while (i < 32){
    if (data[i] == '1'){input[0] = true;}
    else {input[0] = false;}
    input[1] = true;
    this_thread::sleep_for(1ms);
    input[1] = false;
    this_thread::sleep_for(1ms);
    i++;

}
}


void ecl_shift(bool new_value){
    for (int i = 35; i > 0; --i) {
        ecl_data[i] = ecl_data[i - 1];
    }
    ecl_data[0] = new_value;
}

void zero_shift(bool newValue) {
    zero[1] = zero[0];
    zero[0] = newValue;
}

 int char_index(char target, int n, string s) {
    int count = 0;
    ind = 0;
    if (n != 0){
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == target) {
            count++;
            if (count == n) {
                ind = i;
                //cout << "target found";
                break;         // stop after finding nth occurrence
            }
        }
    }
}
else {
    ind = 0;
}
return ind;
}

void load_ROM(){
    int total = count(contents.begin(), contents.end(), ';');
    int i = 0;
    string line;
    while (i < total){
    line = contents.substr(char_index(';',i, contents), char_index(';',i + 1, contents) - char_index(';',i, contents));
    line.erase(
    remove(line.begin(), line.end(), ';'),
    line.end()
    );
    ROM[i] = line;
    //cout << ROM[i];
    i++;

}
}

void do_function () {
    if (emulating){
    string line = ROM[addr];
    line.erase(remove(line.begin(), line.end(), '\n'), line.end());
    int space1 = char_index(' ', 1, line);
    int comma1 = char_index(',', 1, line);

    string func = line.substr(0, space1);
    string op_1 = line.substr(space1 + 1, comma1 - space1 - 1);
    string op_2 = line.substr(comma1 + 1);
    int var_temp;
    int var2_temp;
    //cout << "|" << op_2 <<  "|\n";


    if (func == "SET") {
        if (op_2 == "A") {var_temp = registers[(reg_block * 3) - 3];}
        else if (op_2 == "B") {var_temp = registers[(reg_block * 3) - 2];}
        else if (op_2 == "C") {var_temp = registers[(reg_block * 3) - 1];}
        else {var_temp = stoi(op_2);}

        if (op_1 == "A") {registers[(reg_block * 3) - 3] = var_temp;}
        else if (op_1 == "B") {registers[(reg_block * 3) - 2] = var_temp;}
        else if (op_1 == "C") {registers[(reg_block * 3) - 1] = var_temp;}
    }

    if (func == "ADD") {
        if (op_2 == "A") {var_temp = registers[(reg_block * 3) - 3];}
        else if (op_2 == "B") {var_temp = registers[(reg_block * 3) - 2];}
        else if (op_2 == "C") {var_temp = registers[(reg_block * 3) - 1];}
        else {var_temp = stoi(op_2);}

        if (op_1 == "A") {registers[(reg_block * 3) - 3] += var_temp; var2_temp = 3;}
        else if (op_1 == "B") {registers[(reg_block * 3)- 2] += var_temp; var2_temp = 2;}
        else if (op_1 == "C") {registers[(reg_block * 3) - 1] += var_temp; var2_temp = 1;}

        if (registers[(reg_block * 3) - var2_temp] > 16777215) {registers[(reg_block * 3) - var2_temp] -= 16777216; carry = true;} //24 bit remap
        else {carry = false;}
        if (registers[(reg_block * 3) - var2_temp] == 0) {zero_shift(true);} else {zero_shift(false);}//flags
        //cout << "adding\n";
    }

    if (func == "SUB") {
        if (op_2 == "A") {var_temp = registers[(reg_block * 3) - 3];}
        else if (op_2 == "B") {var_temp = registers[(reg_block * 3) - 2];}
        else if (op_2 == "C") {var_temp = registers[(reg_block * 3) - 1];}
        else {var_temp = stoi(op_2);}

        if (op_1 == "A") {registers[(reg_block * 3) - 3] -= var_temp; var2_temp = 3;}
        else if (op_1 == "B") {registers[(reg_block * 3)- 2] -= var_temp; var2_temp = 2;}
        else if (op_1 == "C") {registers[(reg_block * 3) - 1] -= var_temp; var2_temp = 1;}

        if (registers[(reg_block * 3) - var2_temp] < 0) {registers[(reg_block * 3) - var2_temp] += 16777216; carry = true;} //24 bit remap
        else {carry = false;}
        if (registers[(reg_block * 3) - var2_temp] == 0) {zero_shift(true);} else {zero_shift(false);}//flags
    }

     if (func == "MUT") {
        if (op_2 == "A") {var_temp = registers[(reg_block * 3) - 3];}
        else if (op_2 == "B") {var_temp = registers[(reg_block * 3) - 2];}
        else if (op_2 == "C") {var_temp = registers[(reg_block * 3) - 1];}
        else {var_temp = stoi(op_2);}

        if (op_1 == "A") {registers[(reg_block * 3) - 3] *= var_temp; var2_temp = 3;}
        else if (op_1 == "B") {registers[(reg_block * 3)- 2] *= var_temp; var2_temp = 2;}
        else if (op_1 == "C") {registers[(reg_block * 3) - 1] *= var_temp; var2_temp = 1;}

        carry = false;
        while (registers[(reg_block * 3) - var2_temp] > 16777215) {registers[(reg_block * 3) - var2_temp] -= 16777216; carry = true;} //24 bit remap
        if (registers[(reg_block * 3) - var2_temp] == 0) {zero_shift(true);} else {zero_shift(false);} //flags
    }
      if (func == "DIV") {
        if (op_2 == "A") {var_temp = registers[(reg_block * 3) - 3];}
        else if (op_2 == "B") {var_temp = registers[(reg_block * 3) - 2];}
        else if (op_2 == "C") {var_temp = registers[(reg_block * 3) - 1];}
        else {var_temp = stoi(op_2);}

        if (op_1 == "A") {registers[(reg_block * 3) - 3] /= var_temp; var2_temp = 3;}
        else if (op_1 == "B") {registers[(reg_block * 3)- 2] /= var_temp; var2_temp = 2;}
        else if (op_1 == "C") {registers[(reg_block * 3) - 1] /= var_temp; var2_temp = 1;}

        zero_shift(false);
        carry = false;
    }
    if (func == "MOD") {
        if (op_2 == "A") { var_temp = registers[(reg_block * 3) - 3]; }
        else if (op_2 == "B") { var_temp = registers[(reg_block * 3) - 2]; }
        else if (op_2 == "C") { var_temp = registers[(reg_block * 3) - 1]; }
        else { var_temp = stoi(op_2); }

        if (op_1 == "A") { registers[(reg_block * 3) - 3] %= var_temp; var2_temp = 3; }
        else if (op_1 == "B") { registers[(reg_block * 3) - 2] %= var_temp; var2_temp = 2; }
        else if (op_1 == "C") { registers[(reg_block * 3) - 1] %= var_temp; var2_temp = 1; }

        zero_shift(false);
        carry = true;
}
     if (func == "WRITE") {
        if (op_2 == "A") {var_temp = registers[(reg_block * 3) - 3];}
        else if (op_2 == "B") {var_temp = registers[(reg_block * 3) - 2];}
        else if (op_2 == "C") {var_temp = registers[(reg_block * 3) - 1];}
        else {var_temp = stoi(op_2);}

        if (op_1 == "A") {var2_temp = registers[(reg_block * 3) - 3];}
        else if (op_1 == "B") {var2_temp = registers[(reg_block * 3) - 2];}
        else if (op_1 == "C") {var2_temp = registers[(reg_block * 3) - 1];}
        else {var2_temp = stoi(op_2);}

        RAM[var2_temp] = var_temp;
    }
    if (func == "READ") {
        if (op_2 == "A") {var_temp = registers[(reg_block * 3) - 3];}
        else if (op_2 == "B") {var_temp = registers[(reg_block * 3) - 2];}
        else if (op_2 == "C") {var_temp = registers[(reg_block * 3) - 1];}
        else {var_temp = stoi(op_2);}

        if (op_1 == "A") {registers[(reg_block * 3) - 3] = RAM[var_temp];}
        else if (op_1 == "B") {registers[(reg_block * 3) - 2] = RAM[var_temp];}
        else if (op_1 == "C") {registers[(reg_block * 3) - 1] = RAM[var_temp];}
        else {var2_temp = stoi(op_2);}


    }
    if (func == "IF") {
        bool statement = false;
       if (op_1 == "A = B" or op_1 == "B = A" ) { if (registers[(reg_block * 3) - 3] == registers[(reg_block * 3) - 2]){statement = true;};} //equals
    if (op_1 == "A = C" or op_1 == "C = A" ) { if (registers[(reg_block * 3) - 3] == registers[(reg_block * 3) - 1]){statement = true;};}
    if (op_1 == "C = B" or op_1 == "B = C" ) { if (registers[(reg_block * 3) - 1] == registers[(reg_block * 3) - 2]){statement = true;};}

    if (op_1 == "A > B" or op_1 == "B < A" ) { if (registers[(reg_block * 3) - 3] > registers[(reg_block * 3) - 2]){statement = true;};} //greater than and lesser than
    if (op_1 == "A > C" or op_1 == "C < A" ) { if (registers[(reg_block * 3) - 3] > registers[(reg_block * 3) - 1]){statement = true;};}
    if (op_1 == "C > B" or op_1 == "B < C" ) { if (registers[(reg_block * 3) - 1] > registers[(reg_block * 3) - 2]){statement = true;};}

    if (op_1 == "AND") { if (zero[0] == true && zero[1] == true){statement = true;};} //logic gates
    if (op_1 == "OR") { if (zero[0] == true or zero[1] == true){statement = true;};}
    if (op_1 == "NOT") { if (zero[0] == false){statement = true;};}
    if (op_1 == "XOR") { if (zero[0] ^ zero[1]){statement = true;};}

    if (op_2 == "A") {registers[(reg_block * 3) - 3] = RAM[var_temp];}
    else if (op_2 == "B") {registers[(reg_block * 3) - 2] = RAM[var_temp];}
    else if (op_2 == "C") {registers[(reg_block * 3) - 1] = RAM[var_temp];}
    else {var_temp = stoi(op_2);}

        if (statement) {addr = var_temp;}


    }
     if (func == "JMP") {
        if (op_1 == "A") {var_temp = registers[(reg_block * 3) - 3];}
        else if (op_1 == "B") {var_temp = registers[(reg_block * 3) - 2];}
        else if (op_1 == "C") {var_temp = registers[(reg_block * 3) - 1];}
        else {var_temp = stoi(op_2);}
        //cout << "jumping to: " << var_temp;
        addr = var_temp;
    }

     if (func == "REG") {
        if (op_1 == "A") {registers[(reg_block * 3) - 3] = RAM[var_temp];}
        else if (op_1 == "B") {registers[(reg_block * 3) - 2] = RAM[var_temp];}
        else if (op_1 == "C") {registers[(reg_block * 3) - 1] = RAM[var_temp];}
        else {var_temp = stoi(op_2);}

        reg_block = var_temp + 1;
    }


    if (func == "HLT") {
        emulating = false;
        cout << "halt\n";
}
    if (func == "OUT"){
        if (op_1 == "A") {var_temp = registers[(reg_block * 3) - 3];}
        else if (op_1 == "B") {var_temp = registers[(reg_block * 3) - 2];}
        else if (op_1 == "C") {var_temp = registers[(reg_block * 3) - 1];}
        else {var_temp = stoi(op_2);}

        if (op_2 == "A") {var2_temp = registers[(reg_block * 3) - 3];}
        else if (op_2 == "B") {var2_temp = registers[(reg_block * 3) - 2];}
        else if (op_2 == "C") {var2_temp = registers[(reg_block * 3) - 1];}
        else {var2_temp = stoi(op_2);}

        if (var2_temp > 0) {output[var_temp] = 1;} else {output[var_temp] = 0;}
}

    if (func == "IN") {
        if (op_2 == "A") {var2_temp = registers[(reg_block * 3) - 3];}
        else if (op_2 == "B") {var2_temp = registers[(reg_block * 3) - 2];}
        else if (op_2 == "C") {var2_temp = registers[(reg_block * 3) - 1];}
        else {var2_temp = stoi(op_2);}

        if (op_1 == "A") {registers[(reg_block * 3) - 3] = input[var2_temp];}
        else if (op_1 == "B") {registers[(reg_block * 3) - 2] = input[var2_temp];;}
        else if (op_1 == "C") {registers[(reg_block * 3) - 1] = input[var2_temp];;}
}
}


}

int main() {
    bool actioning = false;
    string path;
    cout << "Welcome to the open-source emulator for the homebrew BB32-1 32-bit CPU! Please insert path to ROM file.\nFor more information go the the GitHub repo: https://github.com/Dabigboi2000/BB32-1\n";


    while (select_file){

    cout << "Path to file: ";
    getline(cin, path);

    ifstream file(path);
    if (!file) {
        cout << "Invalid file\n";
    }

    else{
    actioning = true;
    while (actioning){
    stringstream buffer;
    buffer << file.rdbuf();
    contents = buffer.str();
    cout << "\n";
    cout << contents << endl;
    cout << "\n";
    cout << "What would you like to do with this file?\n-Emulate    E\n-Exit       B\n"; //<< "\n-Assemble   A\n";
    string run;
    cin >> run;
    if (run == "E" or run == "e"){select_file = false;}
    if (run == "B" or run == "b"){actioning = false;}
    else {cout << "Unrecognised input, try again\n";}
    }
    }
    }

    load_ROM();
    emulating = true;
    while (emulating){
    //cout << registers[0] << '\n';
    do_function();
    addr++;
    //std::this_thread::sleep_for(100ms);
    }
return 0;
}
