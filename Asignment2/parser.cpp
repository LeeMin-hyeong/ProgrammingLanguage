#include <iostream>
#include <string>
#include <cstdlib>
#include <stack>
#define EQ_OP 10
#define NEQ_OP 11
#define LS_OP 20
#define GR_OP 21
#define LE_OP 22
#define GE_OP 23
#define UNKNOWN -1
using namespace std;

class Parser {
public:
    void parse(string input) {
        index = 0;
        str = input;
        while(!s.empty()) s.pop();
        try{
            expr();
        }
        catch(exception& e){
            cout << e.what();
        }
    }

private:
    stack<int> s;
    string str;
    int index;

    char lookup() {
        if(index >= str.length()) return EOF;
        while(str[index] == ' ') index++;
        return str[index];
    }

    bool is_digit() {
        return str[index] >= '0' && str[index] <= '9';
    }

    void error() {
        throw runtime_error("syntax error!!\n");
    }

    void lexeme(char c) {
        if (str[index] == c) index++;
        else error();
    }

    void number() {
        string num = "";
        if (!is_digit()) error();
        else{
            while (is_digit()) {
                num+=str[index];
                index++;
            }
            s.push(stoi(num));
        }
    }

    void factor() {
        // cout << "enter factor\n";
        if (lookup() == '(') {
            lexeme('(');
            aexpr();
            lexeme(')');
        } else number();
    }

    void term() {
        // cout << "enter term\n";
        factor();
        while (lookup() == '+' || lookup() == '-') {
            int left = s.top();
            s.pop();
            if(lookup() == '+'){
                lexeme(lookup());
                factor();
                int right = s.top();
                s.pop();
                s.push(left+right);
            }
            else if(lookup() == '-'){
                lexeme(lookup());
                factor();
                int right = s.top();
                s.pop();
                s.push(left-right);
            }
        }
    }

    void aexpr() {
        // cout << "enter aexpr\n";
        term();
        while (lookup() == '*' || lookup() == '/') {
            int left = s.top();
            s.pop();
            if(lookup() == '*'){
                lexeme(lookup());
                term();
                int right = s.top();
                s.pop();
                s.push(left*right);
            }
            else if(lookup() == '/'){
                lexeme(lookup());
                term();
                int right = s.top();
                s.pop();
                if(right == 0) throw runtime_error("divide by zero exception\n");
                s.push(left/right);
            }
        }
    }

    int relop() {
        // cout << "enter relop\n";
        if(lookup() == '='){
            lexeme('=');
            lexeme('=');
            return EQ_OP;
        }
        else if(lookup() == '!'){
            lexeme('!');
            lexeme('=');
            return NEQ_OP;
        }
        else if(lookup() == '>'){
            lexeme('>');
            if(str[index] == '='){
                lexeme('=');
                return GE_OP;
            }
            else return GR_OP;
        }
        else if(lookup() == '<'){
            lexeme('<');
            if(str[index] == '='){
                lexeme('=');
                return LE_OP;
            }
            else return LS_OP;
        }
        else error();
        return UNKNOWN;
    }

    void bexpr() {
        // cout << "enter bexpr\n";
        int left = s.top();
        s.pop();
        int rop = relop();
        aexpr();
        int right = s.top();
        s.pop();
        switch(rop){
            case EQ_OP:
                cout << ">> " << (left == right ? "true" : "false") << '\n';
                break;
            case NEQ_OP:
                cout << ">> " << (left != right ? "true" : "false") << '\n';
                break;
            case LS_OP:
                cout << ">> " << (left < right ? "true" : "false") << '\n';
                break;
            case GR_OP:
                cout << ">> " << (left > right ? "true" : "false") << '\n';
                break;
            case LE_OP:
                cout << ">> " << (left <= right ? "true" : "false") << '\n';
                break;
            case GE_OP:
                cout << ">> " << (left >= right ? "true" : "false") << '\n';
                break;
            default:
                error();
        }
    }

    void expr() {
        // cout << "enter expr\n";
        if (lookup() == '(' || is_digit()) {
            aexpr();
        }
        else error();
        if(lookup() != EOF){
            bexpr();
        }
        else cout << ">> " << s.top() << '\n';
    }
};

int main() {
    Parser parser;
    string input;
    while(true){
        getline(cin, input);
        if(input.length() == 0) break;
            parser.parse(input);
    }
}
