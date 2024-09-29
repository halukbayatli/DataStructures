#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct Lesson
{
    struct Lesson *nextLesson;
    char lessonName[50];
    int midtermGrade;
    int finalGrade;
} StuLesson;

typedef struct Student
{
    struct Student *nextStu;
    char name[50];
    char surname[50];
    int stuNumber;
    StuLesson *lessons;
} Stu;

Stu *Students = NULL;

Stu *fileRead(const char *filename) //Dosya Okuma Fonksiyonu
{
    FILE *fileStudents;
    fileStudents = fopen(filename, "r");
    if (fileStudents == NULL)
    {
        return NULL;
    }
    Stu *students = NULL, *tempStudents = NULL;
    StuLesson *lessons = NULL, *tempLessons = NULL;

    char bufferSize[256];
    while (fgets(bufferSize, sizeof(bufferSize), fileStudents))
    {
        if (strcmp(bufferSize, "\n") == 0)
        {
            continue;
        }
        else
        {
            Stu *newStudent = (Stu *)calloc(1, sizeof(Stu));
            sscanf(bufferSize, "%s %s %d", newStudent->name, newStudent->surname, &newStudent->stuNumber);
            newStudent->lessons = NULL;
            newStudent->nextStu = NULL;
            while (fgets(bufferSize, sizeof(bufferSize), fileStudents) && bufferSize[0] != '\n')
            {
                StuLesson *newLesson = (StuLesson *)calloc(1, sizeof(StuLesson));
                sscanf(bufferSize, "%s %d %d", newLesson->lessonName, &newLesson->midtermGrade, &newLesson->finalGrade);
                newLesson->nextLesson = NULL;
                if (newStudent->lessons == NULL)
                {
                    newStudent->lessons = newLesson;
                    tempLessons = newLesson;
                }
                else
                {
                    tempLessons->nextLesson = newLesson;
                    tempLessons = newLesson;
                }
            }
            if (students == NULL)
            {
                students = newStudent;
                tempStudents = newStudent;
            }
            else
            {
                tempStudents->nextStu = newStudent;
                tempStudents = newStudent;
            }
        }
    }
    fclose(fileStudents);
    return students;
}

void fileWrite(Stu *students) //Dosya Yazma Fonksiyonu
{
    FILE *fileStudents;
    fileStudents = fopen("students.txt", "w");
    if (fileStudents == NULL)
    {
        perror("Dosya okunamadi...\n");
        return;
    }
    Stu *headStudents = students;
    while (headStudents != NULL)
    {
        fprintf(fileStudents, "%s %s %d\n", headStudents->name, headStudents->surname, headStudents->stuNumber);
        StuLesson *headLessons = headStudents->lessons;
        while (headLessons != NULL)
        {
            fprintf(fileStudents, "%s %d %d\n", headLessons->lessonName, headLessons->midtermGrade, headLessons->finalGrade);
            headLessons = headLessons->nextLesson;
        }
        fprintf(fileStudents, "\n");
        headStudents = headStudents->nextStu;
    }
    fclose(fileStudents);
}

Stu *sortStudent(Stu *students, Stu *newStudent) //Sıralama Fonksiyonu
{
    if (Students == NULL || students->stuNumber > newStudent->stuNumber)
    {
        newStudent->nextStu = students;
        return newStudent;
    }
    else
    {
        Stu *currentStudents = students;
        while (currentStudents->nextStu != NULL && currentStudents->nextStu->stuNumber < newStudent->stuNumber)
        {
            currentStudents = currentStudents->nextStu;
        }
        newStudent->nextStu = currentStudents->nextStu;
        currentStudents->nextStu = newStudent;
    }
    return students;
}

Stu *stuAddition(Stu *students) //Ogrenci Ekleme
{
    Stu *newStudent = (Stu *)calloc(1, sizeof(Stu));
    printf("\n");
    printf("Ad: ");
    scanf(" %[^\n]", newStudent->name);
    printf("Soyad: ");
    scanf("%s", newStudent->surname);
    printf("Numara: ");
    scanf("%d", &newStudent->stuNumber);
    newStudent->lessons = NULL;
    Stu *temp = students;
    while (temp)
    {
        if (temp->stuNumber == newStudent->stuNumber)
        {
            printf("Bu numarali ogrenci mevcut lutfen tekrar deneyiniz...\n");
            free(newStudent);
            return students;
        }
        temp = temp->nextStu;
    }
    return sortStudent(students, newStudent);
}

Stu *lessonAddition(Stu *students) //Ders Ekleme
{
    Stu *headStudents = students;
    int number, lessonNumber;    
    printf("Ogrenci Numarasi: ");
    scanf("%d", &number);
    Stu *iterStudents = students;
    while (iterStudents != NULL && iterStudents->stuNumber != number)
    {
        iterStudents = iterStudents->nextStu;
    }
    if (iterStudents != NULL) 
    {
        printf("Ders sayisi: ");
        scanf("%d", &lessonNumber);    
        StuLesson *iterlessons = iterStudents->lessons;
        for (int i = 0; i < lessonNumber; i++) 
        {
            StuLesson *newLesson = (StuLesson*)calloc(1, sizeof(StuLesson));
            printf("Ders: ");
            scanf("%s", newLesson->lessonName);
            printf("Vize: ");
            scanf("%d", &newLesson->midtermGrade);
            printf("Final: ");
            scanf("%d", &newLesson->finalGrade);
            newLesson->nextLesson = NULL;
            if (iterlessons == NULL) 
            {
                iterStudents->lessons = newLesson;
            } 
            else 
            {
                while (iterlessons->nextLesson != NULL) 
                {
                    iterlessons = iterlessons->nextLesson;
                }
                iterlessons->nextLesson = newLesson;
            }
            iterlessons = newLesson;
        }
    } 
    else 
    {
        printf("Ogrenci numarasi bulunamadi... Lutfen tekrar deneyiniz\n");
    }
    return headStudents;
}

void listLesson(Stu *students) //Derse Gore Listeleme
{
    Stu *iterStudents = students;
    char lessonName[100];
    printf("Listelemek icin ders adi giriniz: ");
    scanf("%s", lessonName);
    while (iterStudents != NULL)
    {
        StuLesson *iterLesson = iterStudents->lessons;
        while (iterLesson != NULL)
        {
            if (strcmp(lessonName, iterLesson->lessonName) == 0)
            {
                printf("\n");
                printf("        Ad: %s\n", iterStudents->name);
                printf("        Soyad: %s\n", iterStudents->surname);
                printf("        Numara: %d\n", iterStudents->stuNumber);
                printf("        Vize:  %d\t", iterLesson->midtermGrade);
                printf("        Final: %d\t", iterLesson->finalGrade);
                float averageGrade = ((iterLesson->midtermGrade) + (iterLesson->finalGrade)) * 0.5;
                printf("        Ortalama: %.2f\n\n", averageGrade);
            }
            iterLesson = iterLesson->nextLesson;
        }
        iterStudents = iterStudents->nextStu;
    }
    printf("\n");
}

void listStudent(Stu *students) //Ogrenci Listeleme Fonkisyonu
{
    Stu *iterStudents = students;
    while (iterStudents != NULL)
    {
        printf("\n");
        printf("    Ad: %s\n", iterStudents->name);
        printf("    Soyad: %s\n", iterStudents->surname);
        printf("    Numara: %d\n", iterStudents->stuNumber);
        printf("    Dersler:\n");
        StuLesson *iterLesson = iterStudents->lessons;
        while (iterLesson != NULL)
        {
            printf("        Ders: %s\n", iterLesson->lessonName);
            printf("        Vize:  %d\t", iterLesson->midtermGrade);
            printf("        Final: %d\t", iterLesson->finalGrade);
            float averageGrade = ((iterLesson->midtermGrade) + (iterLesson->finalGrade)) * 0.5;
            printf("        Ortalama: %.2f\n\n", averageGrade);
            iterLesson = iterLesson->nextLesson;
        }
        iterStudents = iterStudents->nextStu;
    }
    printf("\n");
}

Stu *deletionStudent(Stu *students) //Silme Fonksiyonu
{
    Stu *iterStudents = students;
    Stu *prevStudent = NULL;
    char userSurname[100];
    printf("Soyadi giriniz: ");
    scanf("%s", userSurname);
    while (iterStudents != NULL)
    {
        if (strcmp(userSurname, iterStudents->surname) == 0)
        {
            Stu *tempStudent = iterStudents;
            if (prevStudent == NULL)
            {
                students = iterStudents->nextStu;
                iterStudents = students;
            }
            else
            {
                prevStudent->nextStu = iterStudents->nextStu;
                iterStudents = prevStudent->nextStu;
            }
            free(tempStudent);
        }
        else
        {
            prevStudent = iterStudents;
            iterStudents = iterStudents->nextStu;
        }
    }
    return students;
}

void freeMemory(Stu *students) //Bellek Serbet Bırakma Fonksiyonu
{
    StuLesson *tempLesson;
    while (students->lessons->nextLesson != NULL)
    {
        tempLesson = students->lessons;
        students->lessons = students->lessons->nextLesson;
        free(tempLesson);
    }
    Stu *tempStudent;
    while (students->nextStu != NULL)
    {
        tempStudent = students;
        students = students->nextStu;
        free(tempStudent);
    }
}

void main()
{
    Students = fileRead("students.txt");
    int choose;
    while (1)
    {
        printf("\n");
        printf("        Ogrenci Bilgi Sistemi\n");
        printf("    Ekleme Islemleri        ---> 1\n");
        printf("    Listeleme Islemleri     ---> 2\n");
        printf("    Silme Islemleri         ---> 3\n");
        printf("    Cikmak icin             ---> 4\n");
        printf("Islem secimi: ");
        scanf("%d", &choose);
        system("cls");
        if (choose == 1)
        {
            printf("\n");
            printf("    Ogrenci eklemek icin    ---> 1\n");
            printf("    Ders eklemek icin       ---> 2\n");
            printf("Islem secimi: ");
            scanf("%d", &choose);
            system("cls");
            if (choose == 1)
            {
                Students = stuAddition(Students);
            }
            else if (choose == 2)
            {
                Students = lessonAddition(Students);
            }
            getch();
        }
        else if (choose == 2)
        {
            printf("\n");
            printf("    Tum ogrencileri listelemek icin         ---> 1\n");
            printf("    Derse gore ogrenci listelemek icin      ---> 2\n");
            printf("Islem secimi: ");
            scanf("%d", &choose);
            system("cls");
            if (choose == 1)
            {
                listStudent(Students);
            }
            else
            {
                listLesson(Students);
            }
            getch();
        }
        else if (choose == 3)
        {
            Students = deletionStudent(Students);
        }
        else if (choose == 4)
        {
            fileWrite(Students);
            freeMemory(Students);
            break;
        }
        system("cls");
    }
}