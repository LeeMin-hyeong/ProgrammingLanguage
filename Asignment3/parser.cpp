#include <iostream>
#include <string>
#include <unordered_map>
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
        // 초기화
        str = input;
        index = 0;
        m.clear();
        try{
            program();
        }
        // Syntax error catch
        catch(exception& e){
            cout << e.what();
        }
    }

private:
    unordered_map<char, int> m;
    string str;
    int index;

    // 다음 문자 확인
    char lookup(){
        if(index >= str.length()) return EOF;
        return str[index];
    }

    // 공백 제거
    void remove_space(){
        while(lookup() == ' ') index++;
    }

    // 다음 문자가 숫자인지 확인
    bool is_digit(){
        return lookup() >= '0' && lookup() <= '9';
    }

    // 다음 문자가 알파벳인지 확인
    bool is_alpha(){
        return lookup() >= 'a' && lookup() <= 'z';
    }

    // 에러가 발생했을 때 예외를 발생시켜 파싱을 중단
    void error(string msg = "Syntax Error!!\n"){
        throw runtime_error(msg);
    }

    // 예상되는 문자를 소비
    void lexeme(char expected_char) {
        if (lookup() == expected_char) index++;
        else error();
    }

    // 코드의 정수 리터럴을 정수로 변환
    int number() {
        string num = "";
        if (!is_digit()) error();
        else while (is_digit()) num+=str[index++];
        return stoi(num);
    }

    // 변수 타입 정의
    void type(){
        lexeme('i'); lexeme('n'); lexeme('t'); lexeme(' '); // 필수 공백 소비
        remove_space();
    }

    // 변수의 값을 리턴
    // 초기화되지 않은 변수는 0의 값을 가짐
    int var(){
        if(!is_alpha()) error();
        char v = lookup();
        lexeme(v);
        remove_space();
        return m[v];
    }

    // parse term
    int term(){
        int ret;
        remove_space();
        if(is_digit()) ret = number();
        else if(is_alpha()) ret = var();
        else error();
        remove_space();
        return ret;
    }

    // parse aexpr
    int aexpr() {
        int ret = term();
        while (lookup() == '+' || lookup() == '-') {
            if(lookup() == '+'){
                lexeme('+');
                ret += term();
            }
            else if(lookup() == '-'){
                lexeme('-');
                ret -= term();
            }
        }
        return ret;
    }

    // 비교 연산자의 종류 파악
    int relop() {
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
            if(lookup() == '='){
                lexeme('=');
                return GE_OP;
            }
            else return GR_OP;
        }
        else if(lookup() == '<'){
            lexeme('<');
            if(lookup() == '='){
                lexeme('=');
                return LE_OP;
            }
            else return LS_OP;
        }
        else error();
        return UNKNOWN;
    }

    // 비교 연산 스트링을 입력받아 해당 스트링의 연산 결과를 리턴
    bool bexpr(string bool_expr) {

        // 현재 파싱중인 코드 임시 저장 
        string cur_str = str;
        int cur_index = index;

        // 비교 연산 코드를 파싱 코드로 전환
        str = bool_expr;
        index = 0;

        int left = aexpr();
        int rop = relop();
        int right = aexpr();

        // 임시 저장된 코드를 다시 불러옴
        str = cur_str;
        index = cur_index;

        switch(rop){
            case EQ_OP:
                return left == right;
            case NEQ_OP:
                return left != right;
            case LS_OP:
                return left < right;
            case GR_OP:
                return left > right;
            case LE_OP:
                return left <= right;
            case GE_OP:
                return left >= right;
            default:
                error();
                return false;
        }
    }

    void declaration(){
        type();
        var();
        lexeme(';');
    }

    void statement(){
        char v = lookup();
        lexeme(v);
        // print
        if(v=='p' && lookup()=='r'){
            lexeme('r'); lexeme('i'); lexeme('n'); lexeme('t'); lexeme(' ');
            int result = aexpr();
            lexeme(';');
            cout << result << '\n';
        }
        // while
        else if(v=='w' && lookup()=='h'){
            lexeme('h'); lexeme('i'); lexeme('l'); lexeme('e');
            remove_space();
            lexeme('(');
            remove_space();
            // 코드로부터 비교 연산 코드 추출
            string bool_expr = "";
            while(lookup() != ')') bool_expr += str[index++];
            lexeme(')');
            remove_space();
            lexeme('{');
            remove_space();
            
            // 코드로부터 while문의 statements 추출
            string while_statement = "";
            int brace = 0;
            while(lookup() != EOF){
                // while 중첩에 대한 중괄호 처리
                if(lookup() == '{') brace++;
                else if(lookup() == '}'){
                    if(brace>0) brace--;
                    else break;
                }
                while_statement += str[index++];
            }
            lexeme('}');

            // 현재 파싱 중인 코드 임시 저장
            string cur_str = str;
            int cur_index = index;

            // 현재 시스템 상태가 조건을 만족하는 지 확인
            while(bexpr(bool_expr)){
                // 조건을 만족한다면 while statement 반복 실행
                str = while_statement;
                index = 0;
                while(lookup() != EOF){
                    statement();
                    remove_space();
                }
            }

            // 임시 저장된 코드를 다시 불러옴
            str = cur_str;
            index = cur_index;
        }
        // assign
        else{
            remove_space();
            lexeme('=');
            m[v] = aexpr();
            lexeme(';');
        }
    }

    void program() {
        remove_space();
        // declaration 반복
        while(lookup() != EOF){
            if(!is_alpha()) error();
            char v = lookup();
            index++;
            // 첫 문자가 'i' 바로 다음 위치에 알파벳이 있다면 declaration
            if(v == 'i' && is_alpha()){
                index--;
                declaration();
            }
            else{
                index--;
                break;
            }
            remove_space();
        }
        // statement 반복
        while(lookup() != EOF){
            if(!is_alpha()) error();
            statement();
            remove_space();
        }
    }
};

int main(){
    Parser parser;
    string input;
    bool prev_input_empty = false;
    while(true){
        getline(cin, input);
        if(input.length() == 0){
            if(prev_input_empty) break;
            else prev_input_empty = true;
        }
        else{
            prev_input_empty = false;
            parser.parse(input);
        }
    }
}