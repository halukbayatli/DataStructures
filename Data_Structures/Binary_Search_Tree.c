#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef struct Tree
{
    int data;
    struct Tree *left;
    struct Tree *right;
    struct Tree *parent;
}Tree;

void printTree(Tree *rootTree)
{
    if(rootTree != NULL)
    {
        printTree(rootTree->left);
        printf("%d -> ",rootTree->data);
        printTree(rootTree->right);
    }
}

void printTreeToRoot(Tree *rootTree)
{
    if(rootTree != NULL)
    {
        printf("%d -> ",rootTree->data);
        printTreeToRoot(rootTree->parent);
    }
}

int min(Tree *rootTree)
{
    while(rootTree->left != NULL)
        rootTree = rootTree->left;
    return rootTree->data;
}

int max(Tree *root_tree)
{
   while(root_tree->right != NULL)
       root_tree = root_tree->right;
   return root_tree->data;
}

Tree *deleteTree(Tree *rootTree,int value)
{
    if(rootTree == NULL)
        return NULL;
    if(rootTree->data < value)
        rootTree->right = deleteTree(rootTree->right,value);
    else if(rootTree->data > value)
        rootTree->left = deleteTree(rootTree->left,value);
    else if(rootTree->data == value)
    {
        if(rootTree->left == NULL && rootTree->right == NULL)
        {
            free(rootTree);
            return NULL;
        }
        else if(rootTree->left == NULL)
        {
            Tree *temp = rootTree->right;
            temp->parent = rootTree->parent;
            free(rootTree);
            return temp;
        }
        else if(rootTree->right == NULL)
        {
            Tree *temp = rootTree->left;
            temp->parent = rootTree->parent;
            free(rootTree);
            return temp;
        }
        else
        {
            rootTree->data = min(rootTree->left);
            rootTree->left = deleteTree(rootTree->left,rootTree->data);
        }
    }
    return rootTree;
}

Tree *searchValue(Tree *rootTree,int value)
{
    if(rootTree == NULL)
        return NULL;
    if(rootTree->data == value)
        return rootTree;
    else if(rootTree->data > value)
        searchValue(rootTree->left,value);
    else if(rootTree->data < value)
        searchValue(rootTree->right,value);
}

Tree *createNode(Tree *node,int value)
{
    node = (Tree*)malloc(sizeof(Tree));
    node->data = value;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

Tree *insert(Tree *rootTree,int value)
{
    if(rootTree == NULL)
    {
        rootTree = createNode(rootTree,value);
        return rootTree;
    }
    if(rootTree->data < value)
    {
        rootTree->right = insert(rootTree->right,value);
        rootTree->right->parent = rootTree;
    }
    else if(rootTree->data > value)
    {
        rootTree->left = insert(rootTree->left,value);
        rootTree->left->parent = rootTree;
    }
    return rootTree;
}

Tree *fileRead(Tree *rootTree)
{
    FILE *F = fopen("numbers.txt","r");
    if(F == NULL)
    {
        perror("Dosya acilamadi...");
        exit(0);
    }
    int value;
    while(fscanf(F,"%d\n",&value) != EOF)
    {
        rootTree = insert(rootTree,value);
    }
    fclose(F);
    return rootTree;
}

void fileWrite(Tree *rootTree,FILE *File)
{
    if(rootTree != NULL)
    {
        fprintf(File,"%d\n",rootTree->data);
        fileWrite(rootTree->left,File);
        fileWrite(rootTree->right,File);
    }
}

int main()
{
    Tree *RootTree = NULL;
    RootTree = fileRead(RootTree);
    int choose;
    int value;
    int control = 1;
    while(control)
    {
        printf("1-Ekleme\n");
        printf("2-Arama\n");
        printf("3-Silme\n");
        printf("4-Cikis\n");
        printf("Secim: ");
        scanf("%d",&choose);
        printf("\n\n");
        switch(choose)
        {
            case 1:
                printf("Value: ");
                scanf("%d",&value);
                RootTree = insert(RootTree,value);
                printTree(RootTree);
                break;
            case 2:
                printTree(RootTree);
                printf("Value: ");
                scanf("%d",&value);
                Tree *searchTree = searchValue(RootTree,value);
                printTreeToRoot(searchTree);
                break;
            case 3:
                printf("Value: ");
                scanf("%d",&value);
                RootTree = deleteTree(RootTree,value);
                break;
            case 4:
                FILE *F = fopen("numbers.txt","w");
                if(F != NULL)
                {
                    fileWrite(RootTree,F);
                    printf("Veriler dosyaya yazildi...");
                }
                control = 0;
                break;
            default:
                printf("Gecerli islem deneyiniz...");
        }
        getch();
        system("cls");
    }
    return 0;
}
