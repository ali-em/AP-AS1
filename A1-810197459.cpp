#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;
#define SYNTAX_ERROR 2
#define END_OF_INPUT 1

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
int inputVariable(vector<string> &line, vector<string> inputs, map<string, int> &variables, int &input_number);
void printVariable(vector<string> &line, map<string, int> variables);
void assign(vector<string> &line, map<string, int> &variables);
int calc(vector<string> &line, map<string, int> &variables);
vector<string> explode(string line);
void getCode(vector<string> &commands);
void getInputs(vector<string> &commands);
int main() {
    map<string, int> variables;
    vector<string> commands, line(1000, ""), inputs;
    int input_number = 0;
    string cmd;

    getCode(commands);
    getInputs(inputs);

    int syntax_error_line = findSyntaxErrorLine(commands);
    if (syntax_error_line) {
        printError(SYNTAX_ERROR, syntax_error_line);
        return -1;
    }

    for (int l = 0; l < commands.size(); l++) {
        line = explode(commands[l]);
        if (line.size() == 0)
            continue;
        cmd = line[0];
        if (cmd == "!")
            printVariable(line, variables);

        else if (isVar(cmd))
            assign(line, variables);

        else if (!inputVariable(line, inputs, variables, input_number)) {
            printError(END_OF_INPUT, l + 1);
            return -1;
        }
    }
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
        if ((line[0] == "!" || line[0] == "?") && !isVar(line[1]))
            return l;
        if ((line[0] == "!" || line[0] == "?") && (line.size() > 2))
            return l;
        if (isNumber(line[0]) || line[0] == "=" || isOperator(line[0]))
            return l;
        if (isVar(line[0])) {
            if (line.size() < 3 || line[1] != "=" || line.size() % 2 == 0)
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

int inputVariable(vector<string> &line, vector<string> inputs, map<string, int> &variables, int &input_number) {
    string var = line[1];
    if (input_number >= inputs.size() || !isNumber(inputs[input_number]))
        return 0;

    variables[var] = stoi(inputs[input_number]);
    input_number++;
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
        if (str[i] == ' ')
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
    return cmd == '!' || cmd == '?' || cmd == '=';
}

bool isCommand(string cmd) {
    return cmd == "!" || cmd == "?" || cmd == "=";
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
        if (line[i] == ' ' || isOperator(line[i]) || isCommand(line[i])) {
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
