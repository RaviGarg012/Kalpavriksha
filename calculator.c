#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Helper Functions
int getPrecedence(char operator)
{
  if (operator == '*' || operator == '/')
  {
    return 2;
  }
  if (operator == '+' || operator == '-')
  {
    return 1;
  }
  return 0;
}

int applyOperation(int first, int second, char operator, int *result, int *error)
{
  switch (operator)
  {
  case '+':
    *result = first + second;
    break;
  case '-':
    *result = first - second;
    break;
  case '*':
    *result = first * second;
    break;
  case '/':
    if (second == 0)
    {
      printf("Error: Division by zero.\n");
      *error = 1;
      return 0;
    }
    *result = first / second;
    break;
  }
  return 1;
}

// Evaluation Function
void evaluateExpression(const char *expression)
{
  int valueStack[100];
  char operatorStack[100];
  int valueStackTop = -1;
  int operatorStackTop = -1;
  int error = 0;
  int expressionLength = strlen(expression);

  for (int expressionIndex = 0; expressionIndex < expressionLength; expressionIndex++)
  {
    // Skip whitespace
    if (isspace(expression[expressionIndex]))
    {
      continue;
    }

    // If the character is a digit, parse the full number
    if (isdigit(expression[expressionIndex]))
    {
      int currentValue = 0;
      while (expressionIndex < expressionLength && isdigit(expression[expressionIndex]))
      {
        currentValue = (currentValue * 10) + (expression[expressionIndex] - '0');
        expressionIndex++;
      }
      valueStack[++valueStackTop] = currentValue;
      expressionIndex--; // Correct for the extra increment in the inner loop
    }
    // If the character is an operator
    else if (expression[expressionIndex] == '+' || expression[expressionIndex] == '-' || expression[expressionIndex] == '*' || expression[expressionIndex] == '/')
    {
      while (operatorStackTop != -1 && getPrecedence(operatorStack[operatorStackTop]) >= getPrecedence(expression[expressionIndex]))
      {
        if (valueStackTop < 1)
        {
          error = 1;
          break;
        }
        int secondOperand = valueStack[valueStackTop--];
        int firstOperand = valueStack[valueStackTop--];
        char poppedOperator = operatorStack[operatorStackTop--];
        int operationResult;
        if (!applyOperation(firstOperand, secondOperand, poppedOperator, &operationResult, &error))
        {
          return; // Exit if applyOperation failed (e.g., division by zero)
        }
        valueStack[++valueStackTop] = operationResult;
      }
      if (error)
      {
        break;
      }
      operatorStack[++operatorStackTop] = expression[expressionIndex];
    }
    // If the character is invalid
    else
    {
      error = 1;
      break;
    }
  }

  if (error)
  {
    printf("Error: Invalid expression.\n");
    return;
  }

  // Apply remaining operators to the values in the stack
  while (operatorStackTop != -1)
  {
    if (valueStackTop < 1)
    {
      error = 1;
      break;
    }
    int secondOperand = valueStack[valueStackTop--];
    int firstOperand = valueStack[valueStackTop--];
    char poppedOperator = operatorStack[operatorStackTop--];
    int operationResult;
    if (!applyOperation(firstOperand, secondOperand, poppedOperator, &operationResult, &error))
    {
      return;
    }
    valueStack[++valueStackTop] = operationResult;
  }

  // Final check for a valid expression and print the result
  if (error || valueStackTop != 0 || operatorStackTop != -1)
  {
    printf("Error: Invalid expression.\n");
  }
  else
  {
    printf("%d\n", valueStack[valueStackTop]);
  }
}

int main()
{
  char expression[256];
  // get input

  if (scanf("%[^\n]", expression) == 1)
  {
    // Hand off the expression to be evaluated.
    evaluateExpression(expression);
  }
  else
  {
    printf("Error: Invalid expression.\n");
  }

  return 0;
}
