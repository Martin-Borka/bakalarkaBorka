#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define MAX_CONFIG_SECTIONS 10
#define MAX_CONFIG_VALUES 100

typedef struct {
    char name[MAX_LINE_LENGTH];
    int value;
    char objects[MAX_LINE_LENGTH];
}
ConfigValue;

typedef struct {
    char name[MAX_LINE_LENGTH];
    ConfigValue values[MAX_CONFIG_VALUES];
    int value_count;
}
ConfigSection;



bool running = true;
double Q, U, I, f;

void getPromene()
{
    FILE* soubor;
    soubor = fopen("promene.txt", "r");
    if (soubor == NULL)
    {
        printf("Chyba: Soubor se nepodařilo otevřít.\n");
        return 1;
    }
    // Načtení proměnných ze souboru
    fscanf(soubor, "%lf %lf %lf %lf", &Q, &U, &I, &f);
    fclose(soubor);

    // Výpis načtených proměnných
    printf("Načtené proměnné:\n");
    printf("Q: %.2lf\n", Q);
    printf("U: %.2lf\n", U);
    printf("I: %.2lf\n", I);
    printf("f: %.2lf\n", f);

}

void LogSTART()
{

}

void nacteni()
{
    FILE* fp;
    char line[MAX_LINE_LENGTH];
    char* token;
    ConfigSection sections[MAX_CONFIG_SECTIONS];
    int section_count = 0;
    int value_count = 0;

    fp = fopen("MainConfig_server2404.txt", "r");
    if (fp == NULL)
    {
        printf("Chyba při otevírání souboru.\n");
        return 1;
    }

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL)
    {
        line[strcspn(line, "\n")] = 0; // Odstraní znak konce řádku

        if (strstr(line, "---") == line)
        { // Začátek sekce
            strcpy(sections[section_count].name, line + 3); // Uloží název sekce
            sections[section_count].value_count = 0;
            section_count++;
        }
        else if (strstr(line, "=") != NULL)
        { // Klíč-hodnota
            token = strtok(line, "=");
            strcpy(sections[section_count - 1].values[value_count].name, token); // Uloží název klíče

            token = strtok(NULL, "=");
            if (isdigit(token[0]))
            { // Číselná hodnota
                sections[section_count - 1].values[value_count].value = atoi(token);
            }
            else
            { // Textová hodnota
                strcpy(sections[section_count - 1].values[value_count].objects, token);
            }
            value_count++;
        }
    }

    fclose(fp);

    // Vypíše načtená data
    for (int i = 0; i < section_count; i++)
    {
        printf("Sekce: %s\n", sections[i].name);
        for (int j = 0; j < sections[i].value_count; j++)
        {
            printf("  %s = ", sections[i].values[j].name);
            if (sections[i].values[j].value != 0)
            {
                printf("%d\n", sections[i].values[j].value);
            }
            else
            {
                printf("%s\n", sections[i].values[j].objects);
            }
        }
    }

    return 0;
}



//start
int main()
{

    LogSTART();
    nacteni();


    //promene
    time_t rawtime;
  struct tm *timeinfo;

time(&rawtime); // Získání aktuálního času v sekundách
timeinfo = localtime(&rawtime); // Převod na strukturu tm

printf("Aktuální čas: %s\n", asctime(timeinfo)); // Zobrazení času


while (running)
{
    //časovač



    running = false;
}




return 0;
}


