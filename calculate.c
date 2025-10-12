#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

double operate(double a, char operator, double b) {
    double result;
    switch(operator) {
        case '+':
            result = a + b;
            break;
        case '-':
            result = a - b;
            break;
        case '*':
            result = a * b;
            break;
        case '/':
            result = a / b;
            break;
        default: 
            printf("Unknown operator: %c\n", operator);
            result = 0;
            break;
    }; 
    return result;
};

/* 
double convert (double a, char unit) {
    switch (unit) {
        case '"'
    };
};
*/
int main () {
    char input[200];
    char *tokens[100];
    char *holdingStack[100];
    char *output[200];

    printf("Enter a string: \n");

    fgets(input, 200, stdin);
    int inputLength = strlen(input);
    if (input[inputLength - 1] == '\n') input[inputLength - 1] = '\0';
    printf("You entered: %s\nLength: %d\n", input, inputLength);

    int i = 0;
    int tokenCount = 0;
    int tokenLength = 0;
    int holdingCount = 0;
    int outputCount = 0;
    char *token = strtok(input, " ");

    while (token != NULL && i < 100) {
        tokens[i] = token;
        token = strtok(NULL, " ");
        tokenLength++;
        i++;
    };

    printf("tokenLength: %i\n", tokenLength);

    while (tokenCount < tokenLength ) {
        char *end;
        double num = strtod(tokens[tokenCount], &end);
        printf("Token: %s\n", tokens[tokenCount]);
        printf("end: %s\n", end);

        if (isdigit(tokens[tokenCount][0])) {
            // It's a number 
            output[outputCount] = tokens[tokenCount];
            printf("%s added to output\n", tokens[tokenCount]);
            outputCount++;
            tokenCount++;

            if (tokenCount == tokenLength) {
                for (int h = 0; h < holdingCount; h++) {
                    output[outputCount] = holdingStack[h];
                    printf("%s added to output\n", holdingStack[h]);
                    outputCount++;
                };
            };

        } else if (end == tokens[tokenCount]) {
            // It's an operator
            if (holdingCount == 0) {
                holdingStack[holdingCount] = tokens[tokenCount];
                printf("%s added to holdingStack\n", holdingStack[holdingCount]);
                holdingCount++;
                tokenCount++;

            } else if (*end == '+' || *end == '-') {
                output[outputCount] = holdingStack[holdingCount - 1];
                printf("%s added to output\n", output[outputCount]);
                holdingCount--;
                outputCount++;

            } else if (*end == '*' || *end == '/') {
                if (
                    *holdingStack[holdingCount - 1] == '+' 
                    || 
                    *holdingStack[holdingCount - 1] == '-'
                ) {
                    holdingStack[holdingCount] = tokens[tokenCount];
                    printf("%s added to holdingStack\n", holdingStack[holdingCount]);
                    holdingCount++;
                    tokenCount++;

                } else if (
                    *holdingStack[holdingCount - 1] == '*' 
                    || 
                    *holdingStack[holdingCount - 1] == '/'
                ) {
                    //c
                    output[outputCount] = holdingStack[holdingCount -1];
                    printf("%s added to output\n", output[outputCount]);
                    outputCount++;
                    holdingCount--;
                }
            };
        } else printf("Must be a unit of measurement\n");
    };

    int solveCount = 0;
    double solveStack[200];
    int outputlength = outputCount;
    outputCount = 0;
    while (outputCount < outputlength) {
        if (isdigit(output[outputCount][0])) {
            // it's a number 
            char *end;
            double num = strtod(output[outputCount], &end);
            solveStack[solveCount] = num;
            printf("%.2f added to solveStack\n", solveStack[solveCount]);
            outputCount++;
            solveCount++;

        } else {
            double result = operate(
                                solveStack[solveCount - 2], 
                                *output[outputCount], 
                                solveStack[solveCount - 1]
                            );
            solveStack[solveCount - 2] = result;
            printf("added %.2f to solveStack\n", solveStack[solveCount - 2]);
            solveCount--;
            outputCount++;
        };
    };
    
    printf("result = %.2f", solveStack[0]);

    return 0;
};
