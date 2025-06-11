#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>

typedef struct Tree
{
    int data;
    struct Tree *left;
    struct Tree *right;
    int height;
} AVL;

AVL *AVLTree_1 = NULL;
AVL *AVLTree_2 = NULL;

void generateGraphImage(const char *dotFile, const char *outputImage)
{
    char command[256];
    snprintf(command, sizeof(command), "dot -Tpng %s -o %s", dotFile, outputImage);
    system(command);
}

void writeDot(AVL *AVL_Tree, FILE *file)
{
    if (AVL_Tree == NULL)
        return;
    if (AVL_Tree->left != NULL)
        fprintf(file, "    %d -> %d;\n", AVL_Tree->data, AVL_Tree->left->data);
    if (AVL_Tree->right != NULL)
        fprintf(file, "    %d -> %d;\n", AVL_Tree->data, AVL_Tree->right->data);
    writeDot(AVL_Tree->left, file);
    writeDot(AVL_Tree->right, file);
}

void exportToDot(AVL *AVL_Tree, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("File error!\n");
        return;
    }
    fprintf(file, "digraph AVLTree {\n");
    fprintf(file, "    node [fontname=\"Arial\", shape=circle];\n");
    if (AVL_Tree == NULL)
        fprintf(file, "\n");
    else
        writeDot(AVL_Tree, file);
    fprintf(file, "}\n");
    fclose(file);
}

int getHeight(AVL *tree)
{
    if (tree == NULL)
        return 0;
    return tree->height;
}

int getBalance(AVL *tree)
{
    if(tree == NULL)
        return 0;
    return getHeight(tree->left) - getHeight(tree->right);
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

AVL *RightRotate(AVL *y)
{
    AVL *x = y->left;
    AVL *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->left),getHeight(y->right))+1;
    x->height = max(getHeight(x->left),getHeight(x->right))+1;

    return x;
}

AVL *LeftRotate(AVL *x)
{
    AVL *y = x->right;
    AVL *T2 = y->left;

    y->left = x;
    x->right = T2;

    y->height = max(getHeight(y->left),getHeight(y->right))+1;
    x->height = max(getHeight(x->left),getHeight(x->right))+1;

    return y;
}

AVL *insert(AVL *AVL_Tree, int value)
{
    if (AVL_Tree == NULL)
    {
        AVL_Tree = (AVL *)malloc(sizeof(AVL));
        AVL_Tree->data = value;
        AVL_Tree->left = NULL;
        AVL_Tree->right = NULL;
        AVL_Tree->height = 1;
        return AVL_Tree;
    }
    else if (AVL_Tree->data > value)
        AVL_Tree->left = insert(AVL_Tree->left,value);
    else if (AVL_Tree->data < value)
        AVL_Tree->right = insert(AVL_Tree->right,value);
    AVL_Tree->height = max(getHeight(AVL_Tree->left),getHeight(AVL_Tree->right))+1;
    int AVL_Balance = getBalance(AVL_Tree);
    if (AVL_Balance > 1)
    {
        if (value < AVL_Tree->left->data)
            return RightRotate(AVL_Tree);
        else if(value > AVL_Tree->left->data)
        {
            AVL_Tree->left = LeftRotate(AVL_Tree->left);
            return RightRotate(AVL_Tree);
        }
    }
    else if (AVL_Balance < -1)
    {
        if (value > AVL_Tree->right->data)
            return LeftRotate(AVL_Tree);
        else if (value < AVL_Tree->right->data)
        {
            AVL_Tree->right = RightRotate(AVL_Tree->right);
            return LeftRotate(AVL_Tree);
        }
    }
    return AVL_Tree;
}

AVL *deleteItemTree(AVL *AVL_Tree, int value)
{
    if (AVL_Tree == NULL)
        return NULL;
    if (AVL_Tree->data > value)
        AVL_Tree->left = deleteItemTree(AVL_Tree->left, value);
    else if (AVL_Tree->data < value)
        AVL_Tree->right = deleteItemTree(AVL_Tree->right, value);
    else
    {
        if (AVL_Tree->left == NULL || AVL_Tree->right == NULL)
        {
            AVL *temp;
            if (AVL_Tree->left != NULL)
                temp = AVL_Tree->left;
            else
                temp = AVL_Tree->right;
            if (temp == NULL)
            {
                temp = AVL_Tree;
                AVL_Tree = NULL;
            }
            else
                *AVL_Tree = *temp;
            free(temp);
        }
        else
        {
            AVL *temp = AVL_Tree->right;
            while (temp->left != NULL)
                temp = temp->left;
            AVL_Tree->data = temp->data;
            AVL_Tree->right = deleteItemTree(AVL_Tree->right, temp->data);
        }
    }
    if (AVL_Tree == NULL)
        return AVL_Tree;
    AVL_Tree->height = max(getHeight(AVL_Tree->left), getHeight(AVL_Tree->right)) + 1;
    int balance = getBalance(AVL_Tree);
    if (balance > 1)
    {
        if (getBalance(AVL_Tree->left) >= 0)
            return RightRotate(AVL_Tree);
        else
        {
            AVL_Tree->left = LeftRotate(AVL_Tree->left);
            return RightRotate(AVL_Tree);
        }
    }
    if (balance < -1)
    {
        if (getBalance(AVL_Tree->right) <= 0)
            return LeftRotate(AVL_Tree);
        else
        {
            AVL_Tree->right = RightRotate(AVL_Tree->right);
            return LeftRotate(AVL_Tree);
        }
    }
    return AVL_Tree;
}

int searchValue(AVL *AVLTree,int value)
{
    if(AVLTree == NULL)
        return -1;
    if(AVLTree->data > value)
        return searchValue(AVLTree->left,value);
    else if(AVLTree->data < value)
        return searchValue(AVLTree->right,value);
    else if(AVLTree->data == value)
        return AVLTree->data;
}

void inorder(AVL *AVL_Tree)
{
    if(AVL_Tree != NULL)
    {
        inorder(AVL_Tree->left);
        printf("%d\t",AVL_Tree->data);
        inorder(AVL_Tree->right);
    }
}

void fileWrite(AVL *AVL_Tree,FILE *File)
{
    if(AVL_Tree != NULL)
    {
        fprintf(File,"%d,",AVL_Tree->data);
        fileWrite(AVL_Tree->left,File);
        fileWrite(AVL_Tree->right,File);
    }
}

AVL* fileRead(AVL *AVL_Tree,FILE *File)
{
    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), File) != NULL)
    {
        char *strNumber = strtok(buffer, ",");
        while (strNumber != NULL)
        {
            int value = atoi(strNumber);
            if (value != 0 || strNumber[0] == '0')
            {
                AVL_Tree = insert(AVL_Tree, value);
            }
            strNumber = strtok(NULL, ",");
        }
    }
    return AVL_Tree;
}

int main()
{
    FILE *F = fopen("numbers.txt", "r");
    if (F != NULL)
    {
        AVLTree_1 = fileRead(AVLTree_1,F);
        AVLTree_2 = fileRead(AVLTree_2,F);
        fclose(F);
    }
    else
    {
        perror("Dosya okunamadi...");
        exit(1);
    }
    srand(time(NULL));
    int control = 1;
    int choose;
    while(control == 1)
    {
        printf("1.Ekleme\n");
        printf("2.Listeleme\n");
        printf("3.Silme\n");
        printf("4.Graph Grafigi Olusturma\n");
        printf("5.Cikis\n");
        scanf("%d",&choose);
        if(choose == 1)
        {
            int randNum = rand()%90+10;
            printf("Value: %d",randNum);
            AVLTree_1 = insert(AVLTree_1,randNum);
        }
        else if(choose == 2)
        {
            printf("AVL TREE: ");
            inorder(AVLTree_1);
            printf("\n");
            printf("AVL TREE OF DELETED ITEM: ");
            inorder(AVLTree_2);
        }
        else if(choose == 3)
        {
            int value;
            printf("Value: ");
            scanf("%d",&value);
            if(searchValue(AVLTree_1,value) != -1)
            {
                AVLTree_2 = insert(AVLTree_2,value);
                AVLTree_1 = deleteItemTree(AVLTree_1,value);
            }
        }
        else if (choose == 4)
        {
            exportToDot(AVLTree_1, "23100011030AVLTree.dot");
            generateGraphImage("23100011030AVLTree.dot", "23100011030AVLTree.png");
        }
        else if (choose == 5)
        {
            FILE *F = fopen("numbers.txt","w");
            if(F != NULL)
            {
                fileWrite(AVLTree_1,F);
                fprintf(F,"\n");
                fileWrite(AVLTree_2,F);
                fclose(F);
            }
            control = choose;
        }
        getch();
        system("cls");
    }
    return 0;
}
