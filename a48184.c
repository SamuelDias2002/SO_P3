#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

#define FILE_MODE (S_IRUSR | S_IWUSR)

void print_output(int start, int end, char *filename)
{
    int num;
    int file = open("ints.bin", O_RDONLY);
    if (file < 0)
    {
        perror("Erro ao abrir o ficheiro");
        exit(1);
    }

    for (int i = start; i < end; i++)
    {
        if (read(file, &num, sizeof(int)) < sizeof(int))
        {
            perror("Erro ao ler o ficheiro");
            exit(1);
        } 
        else
        {
        printf("%d ", num);
        }
    }
    printf("\n");
}

void print_file(int start, int end, char *filename)
{
    int file = open("ints.bin", O_RDONLY);
    if (file < 0)
    {
        perror("Erro ao abrir o ficheiro");
        exit(1);
    }

    int num;
    for (int i = 0; i < start; i++)
    {
        if (read(file, &num, sizeof(int)) < sizeof(int))
        {
            perror("Erro ao ler o ficheiro");
            exit(1);
        }
    }

    for (int i = start; i < end; i++)
    {
        if (read(file, &num, sizeof(int)) < sizeof(int))
        {
            perror("Erro ao ler o ficheiro");
            exit(1);
        }
    }

    if (file != 0)
    {
        int file_output = open(filename, FILE_MODE);
        if (file_output < 0)
        {
            perror("Erro ao criar o ficheiro");
            exit(1);
        }

        lseek(file, start * sizeof(int), SEEK_SET);
        for (int i = start; i < end; i++)
        {
            if (read(file, &num, sizeof(int)) < sizeof(int))
            {
                perror("Erro ao ler o ficheiro");
                exit(1);
            }
            if (write(file_output, &num, sizeof(int)) < sizeof(int))
            {
                perror("Erro ao escrever no ficheiro");
                exit(1);
            }
        }

        close(file_output);
    }
    printf("\n");
    close(file);
}

int main()
{
    while (1)
    {
        char linha[200];
        char s1[3] = "", s2[100] = "";
        int a, b;
        fgets(linha, 200, stdin);
        sscanf(linha, "%s %d %d %s", s1, &a, &b, s2);

        if (strcmp(s1, "EX") == 0)
        {
            exit(0);
        }
        else if (strcmp(s1, "NG") == 0)
        {
            int pid = fork();
            if (pid < 0)
            {
                perror("Erro ao criar processo filho");
                exit(1);
            }
            else if (pid == 0)
            {
                print_file(a, b, s2);
                exit(0);
            }
        }
        else if (strcmp(s1, "NE") == 0)
        {
            int pid = fork();
            if (pid < 0)
            {
                perror("Erro ao criar processo filho");
                exit(1);
            }
            else if (pid == 0)
            {
                print_output(a, b, s2);
                exit(0);
            }
            else
            {
                wait(NULL);
            }
        }
        else
        {
            perror("Protocolo Desconhecido\n");
        }
    }

    return 0;
}
