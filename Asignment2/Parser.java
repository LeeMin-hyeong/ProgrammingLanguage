import java.util.Scanner;
import java.util.Stack;

class Parser {
    private Stack<Integer> s;
    private String str;
    private int index;
    private final int EQ_OP = 10;
    private final int NEQ_OP = 11;
    private final int LS_OP = 20;
    private final int GR_OP = 21;
    private final int LE_OP = 22;
    private final int GE_OP = 23;
    private final int UNKNOWN = -1;

    public void parse(String input) {
        this.index = 0;
        this.str = input;
        this.s = new Stack<Integer>();
        try{
            expr();
        }
        catch(Exception e){
            System.out.println(e.getMessage());
        }
    }

    private char lookup() {
        if(index >= str.length()) return '\0';
        while(str.charAt(index) == ' ') index++;
        return str.charAt(index);
    }

    private boolean isDigit() {
        if(index >= str.length()) return false;
        return str.charAt(index) >= '0' && str.charAt(index) <= '9';
    }

    private void error() throws Exception {
        // System.out.println("syntax error!!");
        throw new Exception("syntax error!!");
    }

    private void lexeme(char match) throws Exception {
        if (index >= str.length()) error();
        if (str.charAt(index) == match) index++;
        else error();
    }

    private void number() throws Exception {
        // System.out.println("enter number");
        String num = "";
        if (!isDigit()) error();
        else{
            while (isDigit()) {
                num+=str.charAt(index);
                index++;
            }
            // System.out.println(num);
            s.push(Integer.parseInt(num));
        }
    }

    private void factor() throws Exception {
        // System.out.println("enter factor");
        if (lookup() == '(') {
            lexeme('(');
            aexpr();
            lexeme(')');
        } else number();
    }

    private void term() throws Exception {
        // System.out.println("enter term");
        factor();
        while (lookup() == '+' || lookup() == '-') {
            int left = s.pop();
            if(lookup() == '+'){
                lexeme(lookup());
                factor();
                int right = s.pop();
                s.push(left+right);
            }
            else if(lookup() == '-'){
                lexeme(lookup());
                factor();
                int right = s.pop();
                s.push(left-right);
            }
        }
    }

    private void aexpr() throws Exception {
        // System.out.println("enter aexpr");
        term();
        while (lookup() == '*' || lookup() == '/') {
            int left = s.pop();
            if(lookup() == '*'){
                lexeme(lookup());
                term();
                int right = s.pop();
                s.push(left*right);
            }
            else if(lookup() == '/'){
                lexeme(lookup());
                term();
                int right = s.pop();
                if(right == 0) throw new Exception("divide by zero exception");
                s.push(left/right);
            }
        }
    }

    private int relop() throws Exception {
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
            if(str.charAt(index) == '='){
                lexeme('=');
                return GE_OP;
            }
            else return GR_OP;
        }
        else if(lookup() == '<'){
            lexeme('<');
            if(str.charAt(index) == '='){
                lexeme('=');
                return LE_OP;
            }
            else return LS_OP;
        }
        else error();
        return UNKNOWN;
    }

    private void bexpr() throws Exception {
        int left = s.pop();
        int rop = relop();
        aexpr();
        int right = s.pop();
        switch(rop){
            case EQ_OP:
                System.out.println(">> " + (left == right));
                break;
            case NEQ_OP:
                System.out.println(">> " + (left != right));
                break;
            case LS_OP:
                System.out.println(">> " + (left < right));
                break;
            case GR_OP:
                System.out.println(">> " + (left > right));
                break;
            case LE_OP:
                System.out.println(">> " + (left <= right));
                break;
            case GE_OP:
                System.out.println(">> " + (left >= right));
                break;
            default:
                error();
        }
    }

    private void expr() throws Exception {
        // System.out.println("enter expr");
        if (lookup() == '(' || isDigit()) {
            aexpr();
        }
        else error();
        if(lookup() != '\0'){
            bexpr();
        }
        else System.out.printf(">> %d\n", s.pop());
    }

    
    public static void main(String[] args) {
        Parser parser = new Parser();
        String input;
        Scanner sc = new Scanner(System.in);
        while(true){
            input = sc.nextLine();
            if(input.length()==0) break;
            parser.parse(input);
        }
        sc.close();
    }
}
