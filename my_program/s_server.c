#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <signal.h>
#include "cs104_slave.h"
#include "hal_thread.h"
#include "hal_time.h"

#define MAX_LENGTH 256

bool running;


int randomNumber;
int IPADDRESS[5];
int PORT;
int ORIGINATOR_ADDRESS;
int COMMON_ADDRESS;
short DATA_LOGS;
int PER_RESPONSE_time;
int ALARM_PERIOD_time;
int FAILURE_EFFECT;
int BREAKER_DELAY;
double INPUT_POWER_FACTOR=1;
double TRANSFORMER_POWER_FACTOR=5;
double dayInMinutes;
//zkaladni hodnoty gener
double variables[4][4]; //U voltige I corrent frequenc
/*
double frequency[4]; //f
double current[4]; //I
double voltage[4]; //U */
double Q, P;
float act_load;
bool probihaiciscenar;


//scenare
int scenar=1;
bool undervoltage, overvoltage;
bool probihaiciscenar;
int alarm;

//obrazovky
double monitorIN[4];  //U, I P Q
double monitorOUT[4]; //


srand(time(NULL));



void LogSTART(){

}

void nacteni(){
    FILE *fp;
    fp = fopen("MainConfig_server2404.txt", "r");
    if (fp == NULL) {
        printf("Chyba při otevírání souboru.\n");
        return 1;
    }

    //
    while (getline(&line, &len, fp) != -1) {
        char* pch;
        pch = strtok (line,"=, ;-");
        while (pch != NULL){


            char ip_address[16];

            if(!(strcmp(pch,"IPADDRESS"))){        
                pch = strtok (NULL, "=, ;-");
                strcpy(ip_address, pch);

                char *token = strtok(ip_address, ".");
                int i = 0;
                while (token != NULL) {
                    IPADDRESS[i] = atoi(token); // Převedení části IP adresy na číslo
                    token = strtok(NULL, "."); 
                    i++;
                }
            }

            if(!(strcmp(pch,"PORT"))){        
                pch = strtok (NULL, "=, ;-");
                PORT=atoi(pch);}

            if(!(strcmp(pch,"ORIGINATOR_ADDRESS"))){        
                pch = strtok (NULL, "=, ;-");
                ORIGINATOR_ADDRESS=atoi(pch);}

            if(!(strcmp(pch,"COMMON_ADDRESS"))){        
                pch = strtok (NULL, "=, ;-");
                COMMON_ADDRESS=atoi(pch);}

            if(!(strcmp(pch,"DATA_LOGS"))){        
                pch = strtok (NULL, "=, ;-");
                DATA_LOGS=atoi(pch);}

            if(!(strcmp(pch,"PER_RESPONSE_time"))){        
                pch = strtok (NULL, "=, ;-");
                PER_RESPONSE_time=atoi(pch);}

            if(!(strcmp(pch,"ALARM_PERIOD_time"))){        
                pch = strtok (NULL, "=, ;-");
                ALARM_PERIOD_time=atoi(pch);}

            if(!(strcmp(pch,"FAILURE_EFFECT"))){        
                pch = strtok (NULL, "=, ;-");
                FAILURE_EFFECT=atoi(pch);}

            if(!(strcmp(pch,"BREAKER_DELAY"))){        
                pch = strtok (NULL, "=, ;-");
                BREAKER_DELAY=atoi(pch);}

            if(!(strcmp(pch,"INPUT_POWER_FACTOR"))){        
                pch = strtok (NULL, "=, ;-");
                INPUT_POWER_FACTOR=atoi(pch);}

            if(!(strcmp(pch,"TRANSFORMER_POWER_FACTOR"))){        
                pch = strtok (NULL, "=, ;-");
                TRANSFORMER_POWER_FACTOR=atoi(pch);
                }
            
            //monitor   VOLTAGE = 220.0; 218-228; 0.01
             //           CURRENT = 0.5; 96-100; 0.01
              //          FREQUENCY = 50.0; 45-55; 0.03

            if(!(strcmp(pch,"VOLTAGE"))){        
                pch = strtok (NULL, "=, ;-");
                    variables[0][0]=atoi(pch);
                    pch = strtok (NULL, "=, ;-");
                    variables[0]=atoi(pch);
                    pch = strtok (NULL, "=, ;-");
                    variables[0]=atoi(pch);
                    pch = strtok (NULL, "=, ;-");
                    variables[0]=atoi(pch);
                }

            if(!(strcmp(pch,"CURRENT"))){        
                pch = strtok (NULL, "=, ;-");
                    variables[1][0]=atoi(pch);
                    pch = strtok (NULL, "=, ;-");
                    variables[1][1]=atoi(pch);
                    pch = strtok (NULL, "=, ;-");
                    variables[1][2]=atoi(pch);
                    pch = strtok (NULL, "=, ;-");
                    variables[1][3]=atoi(pch);
                }

            if(!(strcmp(pch,"FREQUENCY"))){        
                pch = strtok (NULL, "=, ;-");
                    variables[2][0]=atoi(pch);
                    pch = strtok (NULL, "=, ;-");
                    variables[2][1]=atoi(pch);
                    pch = strtok (NULL, "=, ;-");
                    variables[2][2]=atoi(pch);
                    pch = strtok (NULL, "=, ;-");
                    variables[2][3]=atoi(pch);
                }
            

            


        }   
    }
    fclose(fp);

}     

nacteniscenare(){
 FILE *file;
    char line[256]; // Buffer pro načtení řádku

    // Otevření souboru pro čtení
    file = fopen("scenare.txt", "r");
    if (file == NULL) {
        printf("Chyba při otevírání souboru.\n");
        return 1;
    }

    if (fgets(line, sizeof(line), file)) {
        char *ptr = line;
        while (*ptr && !isdigit(*ptr)) {
            ptr++;
        }
        // Převedeme nalezený text na číslo
        if (*ptr) {
            scenar = strtol(ptr, NULL, 10);
            printf("Hodnota proměnné 'scenar' je: %d\n", scenar);
        } else {
            printf("Na řádku nebylo nalezeno žádné číslo.\n");
        }
    } else {
        printf("Nepodařilo se načíst řádek ze souboru.\n");
    }
    fclose(file);
    return 0;
} 

//--------------------------------------------------------------------------------------------------------------
 
 
double nahodnehodnoty(double aktual, double min, double max, double step,int scenar) {
    randomNumber = rand() % 3;

    int n = (int)((max - min) / step) + 1;
    int *size = n;

    // Alokace paměti pro pole
    double* array = (double*)malloc(n * sizeof(double));
    if (array == NULL) {
        printf("Chyba při alokaci paměti!\n");
        exit(1);
    }

    // Generování hodnot
    double current = min;
    for (int i = 0; i < n; i++) {
        array[i] = current;
        current += step;
        // Oprava možné chyby zaokrouhlování
        if (current > max) {
            array[i] = max;
        }
    }


    for (int i = 0; i < size; i++)
        {
            if(aktual==min){
                aktual=array[i+1];
                i=size;
            }
            else if (aktual==max)
            {
                aktual=array[i-1];
                i=size;
            }else if(aktual==array[i] && scenar==0){
                aktual=array[i-1];
                i=size;
            } 
            else if(aktual==array[i] && scenar==2){
                aktual=array[i+1];
                i=size;
            } 
            else if(aktual==array[i]){
                switch (randomNumber) {
                    case 0:
                        break;
                    case 1:
                        aktual=array[i-1];
                        break;
                    case 2:
                        aktual=array[i-1];
                        break;
                    default:
                        break;
                    }
                i=size;
            }   
        }


    return(aktual);
}
//------------------------


void getTraficLoad(int act_min) {
    FILE* fp;
    fp = fopen("LoadDiagram.txt", "r");
    char* line = NULL;
    size_t len = 0;
    bool readDiagram = true;

    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    // Čtení prvního řádku souboru
    if (getline(&line, &len, fp) != -1) {
        char* pch = strtok(line, " ");
        if (pch != NULL) {
            char* value = strtok(NULL, " ");
            if (value != NULL) {
                value[strcspn(value, "\r\n")] = '\0';
                if (strcmp(value, "-1") == 0) {
                    readDiagram = true; //-1 čte soubor
                } else {
                    act_load = atof(value); // Pokud není -1
                    readDiagram = false;
                }
            }
        }
    }
    while (getline(&line, &len, fp) != -1 && readDiagram) {
        char* minute = strtok(line, " ");
        char* load = strtok(NULL, " ");

        if (minute != NULL && load != NULL) {
            minute[strcspn(minute, "\r\n")] = '\0';
            load[strcspn(load, "\r\n")] = '\0';

            if (atoi(minute) == act_min) {
                act_load = atof(load);
                break; //nalezena
            }
        }
    }
    free(line);
    fclose(fp);

}

void zapisDoSouboru(const char *text) {
    FILE *soubor = fopen("log.txt", "a");
    if (soubor == NULL) {
        perror("Chyba při otevírání souboru");
        exit(1);
    }

    fprintf(soubor, "%s\n", text);
    fclose(soubor);
}


//Log
//event logs
void LogSTART(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen ("EventLog.txt", "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d Server stared \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fclose(fp);
}
void LogSETUP_G(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen ("EventLog.txt", "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d Setup OK \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fclose(fp);
}
void LogSETUP_F(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen ("EventLog.txt", "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d Setup failed \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fclose(fp);
}

void LogCONREQ(const char* ipAddress){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen ("EventLog.txt", "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d New connection request from %s \n",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, ipAddress);
    fclose(fp);
}

void LogCONOPEN(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen ("EventLog.txt", "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d Connection opened \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fclose(fp);
}

void LogCONCLOSED(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen ("EventLog.txt", "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d Connection closed \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fclose(fp);
}
void LogCONACT(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen ("EventLog.txt", "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d Connection activated \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fclose(fp);
}
void LogCONDEACT(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen ("EventLog.txt", "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d Connection deactivated \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fclose(fp);
}
//data logs

//start
int main() {  

    bool prestart = true;

    srand(time(NULL));//generovani
    char text[MAX_LENGTH];
    running = true;

    LogSTART();
    nacteni();

    

    //pr
    int seconds, time_now, time_tmp, time_monitorTimer;
    double underovervoltageValue=0, underovervoltageValuePlus =1 ;
    int underovervoltagecounter;

    while (running)
    {
        

        seconds = time(NULL)%60;
        time_now=seconds;

        //při změně času
        if(time_now!=time_tmp){
            //time_monitorTimer+=1;
            time_tmp=time_now;

            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            //printf("Time now: %02d:%02d:%02d \n", tm.tm_hour, tm.tm_min, tm.tm_sec);
            dayInMinutes = (tm.tm_hour * 60) + tm.tm_min;
            printf("Time now: %i \n",dayInMinutes);
            Thread_sleep(10); //uspání na 10 mls
            getTraficLoad(dayInMinutes); //nacteni act_load ze souboru
            printf("act_load je: %f \n", act_load);
            

         //scénáře
            nacteniscenare();
            if (prestart==true)
            {

                underovervoltageValue=0;
                underovervoltageValuePlus =1;
                underovervoltagecounter = 0;



                undervoltage=false;
                overvoltage=false;
                alarm=0;
                probihaiciscenar=false;
                prestart = false;
            }
            
            //podpětí
            else if(scenar==0 && (underovervoltagecounter < BREAKER_DELAY)){
                
                if(undervoltage==false){
                    underovervoltageValue = variables[0][0];
                }
                
                underovervoltageValue = nahodnehodnoty(underovervoltageValue, variables[0][1], variables[0][2],variables[0][3],0); //U
                variables[0][0] = underovervoltageValue;

                //variables[0][0]+=FAILURE_EFFECT;



                underovervoltagecounter +=1; //čeká na t=5
                undervoltage=true;
                probihaiciscenar=true;
            }



            //prepeti
            if(scenar==2 && (underovervoltagecounter < BREAKER_DELAY)){



                if(undervoltage==false){
                    underovervoltageValue = variables[0][0];
                }
                
                underovervoltageValue = nahodnehodnoty(underovervoltageValue, variables[0][1], variables[0][2],variables[0][3],2); //U
                variables[0][0] = underovervoltageValue;

                //variables[0][0]+=FAILURE_EFFECT;



                underovervoltagecounter +=1; //čeká na t=5

                overvoltage = true;
                probihaiciscenar=true;
            }


            //podpeti prepeti
            if (((overvoltage == true)||(undervoltage==true)) && (underovervoltagecounter >= BREAKER_DELAY))
            {
                
                
                if ((underovervoltageValuePlus != 0.9)&& undervoltage==true)
                {//under
                    underovervoltageValuePlus -=0.005;
                    underovervoltageValue = nahodnehodnoty(underovervoltageValue, variables[0][1], variables[0][2],variables[0][3],0); //U
                }else if ((underovervoltageValuePlus != 1.1)&& overvoltage==true)
                {//over
                    underovervoltageValuePlus +=0.005;
                    underovervoltageValue = nahodnehodnoty(underovervoltageValue, variables[0][1], variables[0][2],variables[0][3],2); //U
                }


                
                


                variables[0][0] = underovervoltageValue * underovervoltageValuePlus;
                /*
                U>420kV	LD0.VMMXU1.HiWrn.stVal
                U>440kV	LD0.VMMXU1.HiAlm.stVal
                U<380kV	LD0.VMMXU1.LoWrn.stVal
                U<360kV	LD0.VMMXU1.LoAlm.stVal
                */

                if(variables[0][0]<(((variables[0][1]+variables[0][2])/2)*0.95)){

                } else if(variables[0][0]<(((variables[0][1]+variables[0][2])/2)*0.9)){

                } else if(variables[0][0]<(((variables[0][1]+variables[0][2])/2)*1.05)){

                }else if(variables[0][0]<(((variables[0][1]+variables[0][2])/2)*1.1)){

                }

                
                alarm+=1;
            
            }
            


            //zkrat 1
            if(scenar==3 && probihaiciscenar==false){






                probihaiciscenar=true;
            }

            //zkrat 2
            if(scenar==4 && probihaiciscenar==false){






                probihaiciscenar=true;
            }







            //monitory a ukládání
            //změna proměnných


            if(probihaiciscenar==false){
                //double aktual, double min, double max, double step
                variables[0][0] = nahodnehodnoty(variables[0][0], variables[0][1], variables[0][2],variables[0][3],1); //U
                variables[1][0] = nahodnehodnoty(variables[1][0], variables[1][1], variables[1][2], variables[1][3],1)*act_load; //I
                variables[2][0] = nahodnehodnoty(variables[2][0], variables[2][1], variables[2][2], variables[2][3],1); //frequency
                
                

                //u, i, p, q
                monitorIN[0] = variables[0][0];
                monitorIN[1] =variables[1][0];

                monitorIN[2] = variables[0][0]*variables[1][0]*(float)sin(INPUT_POWER_FACTOR);    //Q_in =U*I*sin(alfa) alfa=10°
                monitorIN[3] = variables[0][0]*variables[1][0]*(float)cos(INPUT_POWER_FACTOR);    //P_in =U*I*cos(alfa) alfa=10°

                //uložení do souboru
                snprintf(text, MAX_LENGTH, "Hodnoty jsou: %.5f, %.5f, %.5f, %.5f, ", monitorIN[0], monitorIN[0], monitorIN[0], monitorIN[0]);
                zapisDoSouboru(text);
            }
            
            monitorOUT;

            /*    
            printf("%f-%f-%f-%f\n", monitor_in_actual[j][0], monitor_in_actual[j][1], monitor_in_actual[j][2], monitor_in_actual[j][3]);
            */
        }


        


        //změna dat, UIQ

        
        if (scenar==6)
        {
            running = false;
        }
    }   
    Thread_sleep(1000);
  return 0;
}


