EOF = -1
EQ_OP = 10
NEQ_OP = 11
LS_OP = 20
GR_OP = 21
LE_OP = 22
GE_OP = 23
UNKNOWN = -1

class Parser:
    def parse(self, input:str):
        self.__index = 0
        self.__string = input
        self.__stack = []
        try:
            self.__expr()
        except Exception as e:
            print(f">> {e}")

    def __lookup(self) -> str:
        if self.__index >= len(self.__string): return EOF
        while(self.__string[self.__index] == " "):
            self.__index+=1
        return self.__string[self.__index]

    def __is_digit(self) -> bool: 
        if self.__index >= len(self.__string): return False
        return "0" <= self.__string[self.__index] <= "9"

    def __error(self):
        raise Exception("syntax error!!")

    def __lexeme(self, c:str):
        if self.__index >= len(self.__string): self.__error()
        if self.__string[self.__index] == c:
            self.__index += 1
        else:
            self.__error()

    def __number(self):
        # print("enter __number")
        num = ""
        if not self.__is_digit(): self.__error()
        else:
            while self.__is_digit():
                num += self.__string[self.__index]
                self.__index += 1
            self.__stack.append(int(num))

    def __factor(self):
        # print("enter __factor")
        if self.__lookup() == "(":
            self.__lexeme("(")
            self.__aexpr()
            self.__lexeme(")")
        else:
            self.__number()

    def __term(self):
        # print("enter __term")
        self.__factor()
        while self.__lookup() == "+" or self.__lookup() == "-":
            left = self.__stack.pop()
            if self.__lookup() == "+":
                self.__lexeme(self.__lookup())
                self.__factor()
                right = self.__stack.pop()
                self.__stack.append(left+right)
            elif self.__lookup() == "-":
                self.__lexeme(self.__lookup())
                self.__factor()
                right = self.__stack.pop()
                self.__stack.append(left-right)

    def __aexpr(self):
        # print("enter __aexpr")
        self.__term()
        while self.__lookup() == "*" or self.__lookup() == "/":
            left = self.__stack.pop()
            if self.__lookup() == "*":
                self.__lexeme(self.__lookup())
                self.__term()
                right = self.__stack.pop()
                self.__stack.append(left*right)
            elif self.__lookup() == "/":
                self.__lexeme(self.__lookup())
                self.__term()
                right = self.__stack.pop()
                if(right == 0):
                    raise Exception("divie by zero exception")
                self.__stack.append(left//right)

    def __relop(self) -> int:
        # print("enter __relop")
        if self.__lookup() == "=":
            self.__lexeme("=")
            self.__lexeme("=")
            return EQ_OP
        elif self.__lookup() == "!":
            self.__lexeme("!")
            self.__lexeme("=")
            return NEQ_OP
        elif self.__lookup() == ">":
            self.__lexeme(">")
            if self.__string[self.__index] == "=":
                self.__lexeme("=")
                return GE_OP
            else: return GR_OP
        elif self.__lookup() == "<":
            self.__lexeme("<")
            if self.__string[self.__index] == "=":
                self.__lexeme("=")
                return LE_OP
            else: return LS_OP
        else: self.__error()
        return UNKNOWN

    def __bexpr(self):
        # print("enter __bexpr")
        left = self.__stack.pop()
        rop = self.__relop()
        self.__aexpr()
        right = self.__stack.pop()
        if rop == EQ_OP:
            print(f">> {left == right}")
        elif rop == NEQ_OP:
            print(f">> {left != right}")
        elif rop == LS_OP:
            print(f">> {left < right}")
        elif rop == GR_OP:
            print(f">> {left > right}")
        elif rop == LE_OP:
            print(f">> {left <= right}")
        elif rop == GE_OP:
            print(f">> {left >= right}")

    def __expr(self):
        # print("enter __expr")
        if self.__lookup() == "(" or self.__is_digit():
            self.__aexpr()
        else: self.__error()

        if self.__lookup() != EOF:
            self.__bexpr()
        else:
            print(f">> {self.__stack.pop()}")

parser = Parser()
while(True):
    expr = input()
    if(len(expr) == 0): break
    parser.parse(expr)

