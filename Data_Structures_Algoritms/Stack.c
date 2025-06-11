#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Stack
{
    int value;
    struct Stack *next;
} Stack;

int isEmpty(Stack *S)
{
    return S == NULL ? 1 : 0;
}

Stack *pushStack(Stack *S, int number)
{
    Stack *newStack = (Stack *)malloc(sizeof(Stack));
    newStack->value = number;
    newStack->next = S;
    return newStack;
}

int popStack(Stack **S)
{
    if (isEmpty(*S))
        return -1;
    else
    {
        Stack *tempStack = *S;
        (*S) = (*S)->next;
        int popValue = tempStack->value;
        free(tempStack);
        return popValue;
    }
}

void printTopStack(Stack *S)
{
    if (isEmpty(S))
        printf("Main\n");
    else
        printf("Top element: %d\n", S->value);
}

int main()
{
    Stack *F_Stack = NULL;
    Stack *B_Stack = NULL;

    char control = 'c';
    char choose;
    int item;
    while (control == 'c')
    {
        srand(time(0));
        printf(" <<--       -->> \n");
        printf("Enter command: ");
        scanf(" %c", &choose);
        if (choose == '>')
        {
            if (isEmpty(B_Stack))
                item = rand() % 100 + 1;
            else
                item = popStack(&B_Stack);
            F_Stack = pushStack(F_Stack, item);
            printTopStack(F_Stack);
        }
        else if (choose == '<')
        {
            item = popStack(&F_Stack);
            if (item != -1)
                B_Stack = pushStack(B_Stack, item);
            printTopStack(F_Stack);
        }
        if (choose == 'q')
            control = 'q';
        getch();
        system("cls");
    }
    free(F_Stack);
    free(B_Stack);
    return 0;
}