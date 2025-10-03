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
  int values[100];
  char ops[100];
  int values_top = -1;
  int ops_top = -1;
  int error = 0;
  int len = strlen(expression);
  for (int i = 0; i < len; i++)
  {
    // Skip whitespace
    if (isspace(expression[i]))
    {
      continue;
    }

    if (isdigit(expression[i]))
    {
      int val = 0;
      while (i < len && isdigit(expression[i]))
      {
        val = (val * 10) + (expression[i] - '0');
        i++;
      }
      values[++values_top] = val;
      i--;
    }

    else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/')
    {

      while (ops_top != -1 && getPrecedence(ops[ops_top]) >= getPrecedence(expression[i]))
      {
        if (values_top < 1)
        {
          error = 1;
          break;
        }
        int val2 = values[values_top--];
        int val1 = values[values_top--];
        char op = ops[ops_top--];
        int res;
        if (!applyOperation(val1, val2, op, &res, &error))
        {
          return;
        }
        values[++values_top] = res;
      }
      if (error)
        break;
      ops[++ops_top] = expression[i];
    }
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

  while (ops_top != -1)
  {
    if (values_top < 1)
    {
      error = 1;
      break;
    }
    int val2 = values[values_top--];
    int val1 = values[values_top--];
    char op = ops[ops_top--];
    int res;
    if (!applyOperation(val1, val2, op, &res, &error))
    {
      return;
    }
    values[++values_top] = res;
  }

  if (error || values_top != 0 || ops_top != -1)
  {
    printf("Error: Invalid expression.\n");
  }
  else
  {
    printf("%d\n", values[values_top]);
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
    // Handle cases where input is empty or just a newline
    // which scanf would fail to read.
    printf("Error: Invalid expression.\n");
  }

  return 0;
}