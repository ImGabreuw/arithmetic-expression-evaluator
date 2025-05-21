/**
 * Integrantes:
 * Gabriel Ken Kazama Geronazzo - 10418247
 * Lucas Pires de Camargo Sarai - 10418013
 */

/**
 * Gramática sem recursão à esquerda:
 * E -> T {(+T | -T)}
 * T -> F {(*F | /F | %F)}
 * F -> num | (E)
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_EXPRESSION_LENGTH 100
#define MAX_POSTFIX_LENGTH 100

#define OP_ADD '+'
#define OP_SUBTRACT '-'
#define OP_MULTIPLY '*'
#define OP_DIVIDE '/'
#define OP_REST '%'
#define LEFT_PAREN '('
#define RIGHT_PAREN ')'

char inputExpression[MAX_EXPRESSION_LENGTH];
char postfixExpression[MAX_POSTFIX_LENGTH];
int currentPosition = 0;
int postfixPosition = 0;
int syntaxError = 0;

// Funções para os símbolos não-terminais
void E();
void T();
void F();

char peekNextToken();
char consumeToken();
void addToPostfix(char token);
int evaluatePostfixExpression();
void resetParser();
int isOperator(char c);

/**
 * Retorna o próximo token sem consumi-lo
 * @return O próximo caractere na expressão
 */
char peekNextToken()
{
    int i = currentPosition;
    while (inputExpression[i] == ' ')
    {
        i++;
    }
    return inputExpression[i];
}

/**
 * Consome e retorna o próximo token
 * @return O token consumido
 */
char consumeToken()
{
    while (inputExpression[currentPosition] == ' ')
    {
        currentPosition++;
    }
    return inputExpression[currentPosition++];
}

/**
 * Adiciona um token à expressão pós-fixa
 * @param token O token a ser adicionado
 */
void addToPostfix(char token)
{
    postfixExpression[postfixPosition++] = token;
}

/**
 * Verifica se um caractere é um operador aritmético
 * @param c O caractere a ser verificado
 * @return 1 se for operador, 0 caso contrário
 */
int isOperator(char c)
{
    return (c == OP_ADD || c == OP_SUBTRACT || c == OP_MULTIPLY || c == OP_DIVIDE || c == OP_REST);
}

// E -> T {+T | -T | e}
void E()
{
    // Primeiro T
    T();

    // Em seguida, zero ou mais ocorrências de +T ou -T
    while (peekNextToken() == OP_ADD || peekNextToken() == OP_SUBTRACT)
    {
        char operator = consumeToken();
        T();
        addToPostfix(operator);
    }
}

// T -> F {*F | /F | %F | e}
void T()
{
    // Primeiro F
    F();

    // Em seguida, zero ou mais ocorrências de *F, /F ou %F
    while (peekNextToken() == OP_MULTIPLY ||
           peekNextToken() == OP_DIVIDE ||
           peekNextToken() == OP_REST)
    {
        char operator = consumeToken();
        F();
        addToPostfix(operator);
    }
}

// F -> num | (E)
void F()
{
    char nextToken = peekNextToken();

    if (isdigit(nextToken))
    {
        // F -> num
        char digit = consumeToken();
        addToPostfix(digit);
    }
    else if (nextToken == LEFT_PAREN)
    {
        // F -> (E)
        consumeToken(); // Consome '('
        E();            // Processa E

        if (peekNextToken() == RIGHT_PAREN)
        {
            consumeToken(); // Consome ')'
        }
        else
        {
            syntaxError = 1; // Erro sintático: esperado ')'
        }
    }
    else
    {
        syntaxError = 1; // Erro sintático: símbolo inesperado
    }
}

/**
 * Avalia a expressão pós-fixa
 * @return O resultado da avaliação
 */
int evaluatePostfixExpression()
{
    int stack[MAX_POSTFIX_LENGTH];
    int top = -1;

    for (int i = 0; i < postfixPosition; i++)
    {
        char token = postfixExpression[i];

        if (isdigit(token))
        {
            stack[++top] = token - '0';
        }
        else if (isOperator(token))
        {
            if (top < 1)
            {
                // Erro: operandos insuficientes
                return 0;
            }

            int operand2 = stack[top--];
            int operand1 = stack[top--];
            int result;

            switch (token)
            {
            case OP_ADD:
                result = operand1 + operand2;
                break;
            case OP_SUBTRACT:
                result = operand1 - operand2;
                break;
            case OP_MULTIPLY:
                result = operand1 * operand2;
                break;
            case OP_DIVIDE:
                result = operand1 / operand2;
                break;
            case OP_REST:
                result = operand1 % operand2;
                break;
            default:
                return 0; // Operador inválido
            }

            stack[++top] = result;
        }
    }

    return (top >= 0) ? stack[top] : 0;
}

void resetParser()
{
    currentPosition = 0;
    postfixPosition = 0;
    syntaxError = 0;
}

int main()
{
    printf("Enter arithmetic expression: ");
    fgets(inputExpression, MAX_EXPRESSION_LENGTH, stdin);
    inputExpression[strcspn(inputExpression, "\n")] = '\0';

    resetParser();
    E();

    postfixExpression[postfixPosition] = '\0';

    if (!syntaxError && currentPosition == strlen(inputExpression))
    {
        int result = evaluatePostfixExpression();
        printf("%s => %s=%d\n", inputExpression, postfixExpression, result);
    }
    else
    {
        printf("%s => Erro Sintático\n", inputExpression);
    }

    return 0;
}
