#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;
#define SYNTAX_ERROR 2
#define END_OF_INPUT 1
#define PRINT_COMMAND "!"
#define INPUT_COMMAND "?"
#define ASSIGN_COMMAND "="
#define SPACE ' '
void printError(int error, int line);
int findSyntaxErrorLine(vector<string> commands);
bool isNumber(string s);
bool isOperator(char cmd);
bool isOperator(string cmd);
bool isVar(string cmd);
bool isCommand(char cmd);
bool isCommand(string cmd);
int val(string cmd, map<string, int> var);
string trim(string str);
int inputVariable(vector<string> &line, vector<string> inputs, map<string, int> &variables, int &inputNumber);
void printVariable(vector<string> &line, map<string, int> variables);
void assign(vector<string> &line, map<string, int> &variables);
int calc(vector<string> &line, map<string, int> &variables);
vector<string> explode(string line);
void getCode(vector<string> &commands);
void getInputs(vector<string> &commands);
int run(vector<string> commands, vector<string> inputs);
int main() {
    vector<string> commands, inputs;

    getCode(commands);
    getInputs(inputs);

    int syntaxErrorLine = findSyntaxErrorLine(commands);
    if (syntaxErrorLine) {
        printError(SYNTAX_ERROR, syntaxErrorLine);
        return -1;
    }

    return run(commands, inputs);
}
int run(vector<string> commands, vector<string> inputs) {
    string cmd;
    int inputNumber = 0;
    map<string, int> variables;

    for (int l = 0; l < commands.size(); l++) {
        vector<string> line = explode(commands[l]);
        if (line.size() == 0)
            continue;
        cmd = line[0];
        if (cmd == PRINT_COMMAND)
            printVariable(line, variables);

        else if (isVar(cmd))
            assign(line, variables);

        else if (!inputVariable(line, inputs, variables, inputNumber)) {
            printError(END_OF_INPUT, l + 1);
            return -1;
        }
    }
    return 0;
}
void printError(int error, int line) {
    if (error == 1)
        cout << "Unexpected end of input" << endl;
    else if (error == 2)
        cout << "Syntax error at line " << line << endl;
}

int findSyntaxErrorLine(vector<string> commands) {
    vector<string> line(1000, "");
    string command;
    for (int l = 1; l <= commands.size(); l++) {
        line = explode(commands[l - 1]);

        if (line.size() == 0)
            continue;
        if (line.size() < 2)
            return l;
        if ((line[0] == PRINT_COMMAND || line[0] == INPUT_COMMAND) && !isVar(line[1]))
            return l;
        if ((line[0] == PRINT_COMMAND || line[0] == INPUT_COMMAND) && (line.size() > 2))
            return l;
        if (isNumber(line[0]) || line[0] == ASSIGN_COMMAND || isOperator(line[0]))
            return l;
        if (isVar(line[0])) {
            if (line.size() < 3 || line[1] != ASSIGN_COMMAND || line.size() % 2 == 0)
                return l;
            for (int i = 2; i < line.size(); i++) {
                if (isCommand(line[i]))
                    return l;
                if (i % 2 == 0 && !(isNumber(line[i]) || isVar(line[i])))
                    return l;
                if (i % 2 == 1 && !isOperator(line[i]))
                    return l;
            }
        }
    }
    return 0;
}

int inputVariable(vector<string> &line, vector<string> inputs, map<string, int> &variables, int &inputNumber) {
    string var = line[1];
    if (inputNumber >= inputs.size() || !isNumber(inputs[inputNumber]))
        return 0;

    variables[var] = stoi(inputs[inputNumber]);
    inputNumber++;
    return 1;
}

void printVariable(vector<string> &line, map<string, int> variables) {
    string var = line[1];
    cout << variables[var] << endl;
}

void assign(vector<string> &line, map<string, int> &variables) {
    variables[line[0]] = calc(line, variables);
}

int calc(vector<string> &line, map<string, int> &variables) {
    int result = 0;

    for (int i = 2; i < line.size(); i += 2)
        result += val(line[i], variables) * (line[i - 1] == "-" ? -1 : 1);

    return result;
}

bool isNumber(string s) {
    for (int i = 0; i < s.length(); i++)
        if (!isdigit(s[i]))
            return false;

    return true;
}

bool isVar(string cmd) {
    if (!isalpha(cmd[0]))
        return false;
    for (int i = 0; i < cmd.length(); i++)
        if (!(isalpha(cmd[i]) || isdigit(cmd[i])))
            return false;
    return true;
}

string trim(string str) {
    string result;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == SPACE)
            continue;
        result += str[i];
    }
    return result;
}

bool isOperator(char cmd) {
    return cmd == '+' || cmd == '-';
}

bool isOperator(string cmd) {
    return cmd == "+" || cmd == "-";
}

bool isCommand(char cmd) {
    return cmd == INPUT_COMMAND[0] || cmd == PRINT_COMMAND[0] || cmd == ASSIGN_COMMAND[0];
}

bool isCommand(string cmd) {
    return cmd == PRINT_COMMAND || cmd == INPUT_COMMAND || cmd == ASSIGN_COMMAND;
}

int val(string cmd, map<string, int> variables) {
    if (isNumber(cmd))
        return stoi(cmd);
    return variables[cmd];
}

vector<string> explode(string line) {
    vector<string> result;
    string temp;
    if (line.size() == 0)
        return vector<string>(0);
    for (int i = 0; i < line.length(); i++) {
        if (line[i] == SPACE || isOperator(line[i]) || isCommand(line[i])) {
            if (temp != " " && temp.length() > 0)
                result.push_back(trim(temp));
            if (isOperator(line[i]) || isCommand(line[i]))
                result.push_back(string(1, line[i]));
            temp = "";
        } else
            temp += line[i];
    }

    if (temp.length() > 0)
        result.push_back(trim(temp));
    return result;
}
void getCode(vector<string> &commands) {
    string cmd;
    while (getline(cin, cmd) && trim(cmd) != "run")
        commands.push_back(cmd);
}

void getInputs(vector<string> &inputs) {
    string cmd;

    while (getline(cin, cmd))
        if (cmd.length() > 0)
            inputs.push_back(cmd);
}
