#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define IMPERIAL 1
#define METRIC 2
#define NUMBER 3
#define OPERATOR 4

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

double convert (double a, char *unit) {
    double result;
    char convertedResult[50];

    if (strcmp(unit, "in") == 0) {
        result = a * 25400000;
    } else if (strcmp(unit, "ft") == 0) {
        result = a * 304800000;
    } else if (strcmp(unit, "yd") == 0) {
        result = a * 914400000;
    } else if (strcmp(unit, "mi") == 0) {
        result = a * 1609344000000;
    } else if (strcmp(unit, "mm") == 0) {
        result = a * 1000000;
    } else if (strcmp(unit, "cm") == 0) {
        result = a * 10000000;
    } else if (strcmp(unit, "dm") == 0) {
        result = a * 100000000;
    } else if (strcmp(unit, "m") == 0) {
        result = a * 1000000000;
    } else if (strcmp(unit, "km") == 0) {
        result = a * 10000000000;
    } else {
        result = 0;
    };
    sprintf(convertedResult, "%f", result);
    return result;
};

int main () {
    char input[200];
    char *tokens[100];
    char *holdingStack[100];
    // char *output[200];
    int system;

    struct {
        int dataType;
        union {
            double num;
            char op;
        } value;
    } output[200];

    printf("Enter a string: \n");

    fgets(input, 200, stdin);
    int inputLength = strlen(input);
    if (input[inputLength - 1] == '\n') input[inputLength - 1] = '\0';
    printf("You entered: %s\nLength: %d\n", input, inputLength);

    int tokenCount = 0;
    int tokenLength = 0;
    int holdingCount = 0;
    int outputCount = 0;
    char *token = strtok(input, " ");

    int i = 0;
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
        printf("end: %d\n", *end);
        printf("num: %f\n", num);

        if (*end == 0) {
            // It's a number 
            output[outputCount].value.num = *tokens[tokenCount];
            printf("%.2f added to output\n", output[outputCount].value.num);
            outputCount++;
            tokenCount++;

            if (tokenCount == tokenLength) {
                for (int h = 0; h < holdingCount; h++) {
                    output[outputCount].value.num = *holdingStack[h];
                    printf("%.2f added to output\n", output[outputCount].value.num);
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
                output[outputCount].value.op = *holdingStack[holdingCount - 1];
                printf("%c added to output\n", output[outputCount].value.op);
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
                    output[outputCount].value.op = *holdingStack[holdingCount -1];
                    printf("%c added to output\n", output[outputCount].value.op);
                    outputCount++;
                    holdingCount--;
                }
            };
        } else {
            printf("%s Must be a unit of measurement\n", end);
            if (*end == *"in" || *end == *"ft" || *end == *"yd" || *end == *"mi") {
                system = IMPERIAL;
                printf("System = %d\n", system);
                output[outputCount].value.num = convert(num, end);
                printf("Converted output added to output = %.2f", output[outputCount].value.num);
                outputCount++;
                tokenCount++;
                // if system && system != imperial: system will be user's choice
            };
        };
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
    
    printf("result = %.2f\n", solveStack[0]);

    return 0;
};
