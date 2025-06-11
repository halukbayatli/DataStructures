#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

typedef struct CircleLinkedNode
{
    char word[100];
    int wordCount;
    struct CircleLinkedNode *prev;
    struct CircleLinkedNode *next;
}CLN;

CLN *WordNodes = NULL;

CLN *findWord(CLN *wordNodes)
{
    FILE *filetext;
    filetext = fopen("text.txt","r");
    if(filetext == NULL)
    {
        perror("Dosya okunamadi...\n");
        return wordNodes;
    }
    char word[50];
    while(fscanf(filetext,"%s",word) != EOF)
    {
        char tempWord[50];
        int j = 0;
        for(int i = 0; word[i] != '\0'; i++)
        {
            word[i] = tolower(word[i]);
            if(isalpha(word[i]))
            {
                tempWord[j++] = word[i];
            }
        }
        tempWord[j] = '\0';
        strcpy(word,tempWord);
        if(wordNodes == NULL)
        {
            wordNodes = (CLN*)malloc(sizeof(CLN));
            strcpy(wordNodes->word,word);
            wordNodes->wordCount = 1;
            wordNodes->prev = wordNodes;
            wordNodes->next = wordNodes;
            continue;
        }
        int control = 0;
        CLN *iter = wordNodes;
        while (1)
        {
            if(strcmp(word,iter->word) == 0)
            {
                iter->wordCount++;
                control = 1;
                break;
            }
            iter = iter->next;
            if(iter == wordNodes)
            {
                break;
            } 
        }
        if(!control)
        {
            CLN *tempWord = (CLN*)malloc(sizeof(CLN));
            strcpy(tempWord->word,word);
            tempWord->wordCount = 1;
            tempWord->prev = wordNodes->prev;
            tempWord->next = wordNodes;
            wordNodes->prev->next = tempWord;
            wordNodes->prev = tempWord;
        }
    }
    fclose(filetext);
    return wordNodes;
}

CLN *wordCountSort(CLN *wordNodes)
{
    if(wordNodes == NULL)
    {
        return wordNodes;
    }
    CLN *head1 = wordNodes;
    CLN *head2;
    int temp;
    char tempWord[50];
    while(1)
    {
        head2 = wordNodes;
        while (1)
        {
            if(head2->next != wordNodes && head2->wordCount < head2->next->wordCount)
            {
                strcpy(tempWord,head2->word);
                strcpy(head2->word,head2->next->word);
                strcpy(head2->next->word,tempWord);
                temp = head2->wordCount;
                head2->wordCount = head2->next->wordCount;
                head2->next->wordCount = temp;
            }
            head2 = head2->next;
            if (head2 == wordNodes)
            {
                break;
            }
        }
        head1 = head1->next;
        if (head1 == wordNodes)
        {
            break;
        }
    }
    return wordNodes;
}

CLN *alphaSort(CLN *wordNodes)
{
    if (wordNodes == NULL)
    {
        return wordNodes;
    }
    CLN *head1 = wordNodes;
    CLN *head2;
    int temp;
    char tempWord[50];
    int control;
    while (1)
    {
        control = 0;
        head2 = wordNodes;
        while (head2->next != wordNodes)
        {
            if(strcmp(head2->word,head2->next->word) > 0)
            {
                strcpy(tempWord,head2->word);
                strcpy(head2->word,head2->next->word);
                strcpy(head2->next->word,tempWord);
                temp = head2->wordCount;
                head2->wordCount = head2->next->wordCount;
                head2->next->wordCount = temp;
                control = 1;
            }
            head2 = head2->next;    
        }
        head1 = head1->next;
        if(head1->next == wordNodes && !control)
        {
            break;
        }
        if(head1->next == wordNodes)
        {
            head1 = wordNodes;
        }
    }
    return wordNodes;
}

CLN *deleteWord(CLN *wordNodes)
{
    if (wordNodes == NULL)
    {
        return wordNodes;
    }
    char letter[50];
    printf("Karakter giriniz: ");
    scanf("%s", letter);
    CLN *iter = wordNodes;
    CLN *temp = NULL;
    int len = strlen(letter);
    int control = 1;
    int count = 0;
    while (control)
    {
        control = 0;
        iter = wordNodes;
        do
        {
            char word[50];
            strcpy(word, iter->word);
            if (strncmp(word, letter, len) == 0)
            {
                printf("Silinen kelime: %s\n", word);
                control = 1;
                if (iter->prev == iter && iter->next == iter)
                {
                    free(iter);
                    return NULL;
                    count++;
                }
                if (iter == wordNodes)
                {
                    wordNodes->prev->next = wordNodes->next;
                    wordNodes->next->prev = wordNodes->prev;
                    temp = wordNodes->next;
                    free(wordNodes);
                    wordNodes = temp;
                    iter = wordNodes;
                    count++;
                }
                else
                {
                    iter->prev->next = iter->next;
                    iter->next->prev = iter->prev;
                    temp = iter->next;
                    free(iter);
                    iter = temp;
                    count++;
                }
            }
            iter = iter->next;

        } while (iter != wordNodes);
    }
    if(count == 0)
    {
        printf("Girdiginiz karaktere basalayan kelimeler bulunamadi...\n");
    }
    return wordNodes;
}

void wordList(CLN *wordNodes)
{
    int choose;
    if(wordNodes == NULL)
    {
        WordNodes = NULL; 
    }
    else
    {
        printf("Normal yazdirma icin    ---> 1\n");
        printf("Ters yazdirma icin      ---> 2\n");
        printf("Islem secimi: ");
        scanf("%d",&choose);
        system("cls");
        if (choose == 1)
        {
            CLN *iter = wordNodes;
            while (1)
            {
                printf("Kelime: %s  --  Sayi: %d\n",iter->word,iter->wordCount);
                iter = iter->next;
                if(iter == wordNodes)
                {
                    break;
                }
            }
        }
        else if(choose == 2)
        {
            CLN *iter = wordNodes;
            while (iter->next != wordNodes) 
            {
                iter = iter->next;
            }
            while (1)
            {
                printf("Kelime: %s  --  Sayi: %d\n", iter->word, iter->wordCount);
                iter = iter->prev;
                if (iter == wordNodes)
                {
                    printf("Kelime: %s  --  Sayi: %d\n", iter->word, iter->wordCount);
                    break;
                }
            }
        }
    }
}

void main()
{
    int choose;
    while(1)
    {
        printf("Kelime sayisi bulmak icin   ---> 1\n");
        printf("Siralama yapmak icin        ---> 2\n");
        printf("Silmek icin                 ---> 3\n");
        printf("Listelemek icin             ---> 4\n");
        printf("Cikmak icin                 ---> 0\n");
        printf("Islem secimi: ");
        scanf("%d",&choose);
        system("cls");
        if (choose == 1)
        {
            WordNodes = findWord(WordNodes);
        }
        else if (choose == 2)
        {
            printf("Kelime sayisina gore siralama icin  ---> 1\n");
            printf("Alfabetik olarak siralama icin      ---> 2\n");
            printf("Islem secimi: ");
            scanf("%d",&choose);
            system("cls");
            if (choose == 1)
            {
                WordNodes = wordCountSort(WordNodes);
            }
            else if (choose == 2)
            {
                WordNodes = alphaSort(WordNodes);
            }
        }
        else if (choose == 3)
        {
            WordNodes = deleteWord(WordNodes);
        }
        else if (choose == 4)
        {
            wordList(WordNodes);
        }
        else if (choose == 0)
        {
            CLN *temp, *iter;
            iter = WordNodes;
            while(1)
            {
                temp = iter;
                iter = iter->next;
                free(temp);
                if(iter == WordNodes)
                {
                    break;
                }  
            }
            exit(1);
        }
        getch();
        system("cls");
    }
}