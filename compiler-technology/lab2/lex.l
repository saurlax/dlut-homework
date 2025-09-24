%{
#include <stdio.h>
int letter_count = 0;
int digit_count = 0;
%}

%%

20232241392     { printf("文浩然"); }
[a-zA-Z]        { letter_count++; ECHO; }
[0-9]           { digit_count++; ECHO; }
.               { ECHO; }

%%

int yywrap() { return 1; }

int main() {
    yyin = fopen("input.txt", "r");
    yylex();
    fclose(yyin);
    printf("\n字母总数: %d\n", letter_count);
    printf("数字总数: %d\n", digit_count);
    return 0;
}