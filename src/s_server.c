#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <threads.h>
#include <signal.h>
#include "cs104_slave.h"
#include "hal_thread.h"
#include "hal_time.h"

#define MAX_LENGTH 256

bool running;

int randomNumber;
int IPADDRESS[4];
int PORT;
int ORIGINATOR_ADDRESS;
int COMMON_ADDRESS;
short DATA_LOGS;
int PER_RESPONSE_time;
int ALARM_PERIOD_time;
int FAILURE_EFFECT;
int BREAKER_DELAY;
int MONITOR_TEMP_FREQ;
double INPUT_POWER_FACTOR = 1;
double TRANSFORMER_POWER_FACTOR = 5;
int dayInMinutes=0;
char str_IP[256];
// zkaladni hodnoty gener
double variables[4][4]; // U voltige I corrent frequenc

double Q, P;
float act_load;
bool probihaiciscenar;

// scenare
int scenar = 1;
bool undervoltage, overvoltage;
bool shortCircuit;
int shortCircuitphase;
bool probihaiciscenar;
bool probihaiciscenarI;
int alarm;
int waitshortCircuit;
double holderI;

// obrazovky
double monitorIN[4];  // U, I P Q
double monitorOUT[4]; //
double MONITOR_OUT_objects[4][4];
double MONITOR_TEMPERATURE_object[7];
double MONITOR_FREQ_objects[3];
int cot=0;

int onehundredfourioa[16][2] = {
        {4000, 0},
        {4001, 2},
        {4002, 2},
        {4003, 0},
        {5000, 0},
        {5001, 0},
        {5002, 0},
        {5003, 0},
        {5004, 0},
        {5006, 0},
        {5007, 0},
        {5011, 0},
        {5012, 0},
        {5014, 0},
        {5017, 0},
        {5018, 0}
    };
//srand(time(NULL));


void sleep(int milliseconds) {
    thrd_sleep(
        &(struct timespec){
            .tv_sec = milliseconds / 1000,
            .tv_nsec = (milliseconds % 1000) * 1000,
        },
        NULL);
}

void onehundredfour(){
    onehundredfourioa[15][1];
}
void
sigint_handler(int signalId)
{
    running = false;
}

//získávání datu a času
void
printCP56Time2a(CP56Time2a time)
{
    printf("%02i:%02i:%02i %02i/%02i/%04i", CP56Time2a_getHour(time),
           CP56Time2a_getMinute(time),
           CP56Time2a_getSecond(time),
           CP56Time2a_getDayOfMonth(time),
           CP56Time2a_getMonth(time),
           CP56Time2a_getYear(time) + 2000);
}


static bool
clockSyncHandler (void* parameter, IMasterConnection connection, CS101_ASDU asdu, CP56Time2a newTime)
{
    printf("Process time sync command with time "); printCP56Time2a(newTime); printf("\n");

    uint64_t newSystemTimeInMs = CP56Time2a_toMsTimestamp(newTime);

    /* Set time for ACT_CON message */
    CP56Time2a_setFromMsTimestamp(newTime, Hal_getTimeInMs());

    /* update system time here */

    return true;
}




//-------
int readInput() {

   FILE *fp;


    fp = fopen("conf/MainConfig_server2404.txt", "r");
    if (fp == NULL) {
        printf("Chyba při otevírání souboru.\n");
        return -1; // Signalizace chyby
    }
    printf("Nacteni souboru v poradku.\n");
    char line[2048];
    char ip_address[16];
    //char line[1024]; // Pevný buffer pro čtení řádků
    int breakthis=0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        char *pch = strtok(line, "=, ;-");
        while (pch != NULL) {

    //....................................

            if (!(strcmp(pch, "IPADDRESS"))) {
                pch = strtok(NULL, "=, ;-");
                if (pch != NULL) {
                    strcpy(ip_address, pch);
                    char *token = strtok(ip_address, ".");
                    int i = 0;
                    while (token != NULL) {
                        IPADDRESS[i] = atoi(token);
                        token = strtok(NULL, ".");
                        i++;
                    }
                }
                snprintf(str_IP, sizeof(str_IP), "%d.%d.%d.%d",
                     IPADDRESS[0], IPADDRESS[1], IPADDRESS[2], IPADDRESS[3]);
                printf("IP.\n");
            }

            if (!(strcmp(pch, "PORT"))) {
                pch = strtok(NULL, "=, ;-");
                PORT = atoi(pch);
            }

            if (!(strcmp(pch, "ORIGINATOR_ADDRESS"))) {
                pch = strtok(NULL, "=, ;-");
                ORIGINATOR_ADDRESS = atoi(pch);
            }

            if (!(strcmp(pch, "COMMON_ADDRESS"))) {
                pch = strtok(NULL, "=, ;-");
                COMMON_ADDRESS = atoi(pch);
            }

            if (!(strcmp(pch, "DATA_LOGS"))) {
                pch = strtok(NULL, "=, ;-");
                DATA_LOGS = atoi(pch);
            }

            if (!(strcmp(pch, "PER_RESPONSE_time"))) {
                pch = strtok(NULL, "=, ;-");
                PER_RESPONSE_time = atoi(pch);
            }

            if (!(strcmp(pch, "ALARM_PERIOD_time"))) {
                pch = strtok(NULL, "=, ;-");
                ALARM_PERIOD_time = atoi(pch);
            }

            if (!(strcmp(pch, "FAILURE_EFFECT"))) {
                pch = strtok(NULL, "=, ;-");
                FAILURE_EFFECT = atoi(pch);
            }

            if (!(strcmp(pch, "BREAKER_DELAY"))) {
                pch = strtok(NULL, "=, ;-");
                BREAKER_DELAY = atoi(pch);
            }

            if (!(strcmp(pch, "INPUT_POWER_FACTOR"))) {
                pch = strtok(NULL, "=, ;-");
                INPUT_POWER_FACTOR = atof(pch);
                printf("IPF.\n");
            }

            if (!(strcmp(pch, "TRANSFORMER_POWER_FACTOR"))) {
                pch = strtok(NULL, "=, ;-");
                TRANSFORMER_POWER_FACTOR = atof(pch);
            }

            if (!(strcmp(pch, "VOLTAGE"))) {
                for (int j = 0; j < 4; j++) {
                    pch = strtok(NULL, "=, ;-");
                    variables[0][j] = atof(pch);
                }
                printf("voltage.\n");
            }

            if (!(strcmp(pch, "CURRENT"))) {
                for (int j = 0; j < 4; j++) {
                    pch = strtok(NULL, "=, ;-");
                    variables[1][j] = atof(pch);

                }
            }

            if (!(strcmp(pch, "FREQUENCY"))) {
                for (int j = 0; j < 4; j++) {
                    pch = strtok(NULL, "=, ;-");
                    variables[2][j] = atof(pch);
                }
            }

            if (!(strcmp(pch, "MONITOR_OUT_objects"))) {
                for (int j = 0; j < 4; j++) {
                    pch = strtok(NULL, "=, ;-");
                    MONITOR_OUT_objects[0][j] = atof(pch);
                }
            }

            if (!(strcmp(pch, "MONITOR_TEMP_FREQ"))) {
                pch = strtok(NULL, "=, ;-");
                MONITOR_TEMP_FREQ = atoi(pch);
            }

            if (!(strcmp(pch, "MONITOR_TEMPERATURE_object"))) {
                for (int j = 0; j < 6; j++) {
                    pch = strtok(NULL, "=, ;-");
                    if (pch == NULL) {
                        printf("Chybí hodnota pro MONITOR_TEMPERATURE_object[%d]\n", j);
                        break;
                    }
                    MONITOR_TEMPERATURE_object[j] = atof(pch);
                }
                //printf("Monitor T O.\n");
            }

            if (!(strcmp(pch, "MONITOR_FREQ_objects"))) {
                for (int j = 0; j < 3; j++) {
                    pch = strtok(NULL, "=, ;-");
                    MONITOR_FREQ_objects[j] = atof(pch);
                }
                printf("Monitor.\n");
            }


            pch = strtok(NULL, "=, ;-");
        }
    }
    fclose(fp);

    //printf("IPADDRESS: %s.%s.%s.%s\n", IPADDRESS[0], IPADDRESS[1], IPADDRESS[2], IPADDRESS[3]);
    printf("PORT: %d\n", PORT);
    printf("ORIGINATOR_ADDRESS: %d\n", ORIGINATOR_ADDRESS);
    printf("COMMON_ADDRESS: %d\n", COMMON_ADDRESS);
    printf("DATA_LOGS: %d\n", DATA_LOGS);
    printf("PER_RESPONSE_time: %.2f\n", PER_RESPONSE_time);
    printf("ALARM_PERIOD_time: %.2f\n", ALARM_PERIOD_time);
    printf("FAILURE_EFFECT: %d\n", FAILURE_EFFECT);
    printf("BREAKER_DELAY: %d\n", BREAKER_DELAY);
    printf("INPUT_POWER_FACTOR: %.2f\n", INPUT_POWER_FACTOR);
    printf("TRANSFORMER_POWER_FACTOR: %.2f\n", TRANSFORMER_POWER_FACTOR);
    printf("VOLTAGE: %.2f\n", variables[0][0]);
    printf("CURRENT: %.2f\n", variables[1][0]);
    printf("FREQUENCY: %.2f\n", variables[2][0]);
    printf("MONITOR_OUT_objects: %.2f\n", MONITOR_OUT_objects[0][0]);
    printf("MONITOR_TEMP_FREQ: %d\n", MONITOR_TEMP_FREQ);
    printf("MONITOR_TEMPERATURE_object: %.2f\n", MONITOR_TEMPERATURE_object[1]);
    printf("MONITOR_FREQ_objects: %.2f\n", MONITOR_FREQ_objects[1]);


    return 0;
}

int nacteniscenare()
{
    FILE *file;
    char line[256]; // Buffer pro načtení řádku

    // Otevření souboru pro čtení
    file = fopen("conf/scenare.txt", "r");
    if (file == NULL)
    {
        printf("Chyba při otevírání souboru.\n");
        return 1;
    }

    if (fgets(line, sizeof(line), file))
    {
        char *ptr = line;
        while (*ptr && !isdigit(*ptr))
        {
            ptr++;
        }
        // Převedeme nalezený text na číslo
        if (*ptr)
        {
            scenar = strtol(ptr, NULL, 10);
            printf("Hodnota promenne 'scenar' je: %d\n", scenar);
        }
        else
        {
            printf("Na řádku nebylo nalezeno žádné číslo.\n");
        }
    }
    else
    {
        printf("Nepodařilo se načíst řádek ze souboru.\n");
    }
    fclose(file);
    return 0;
}

//--------------------------------------------------------------------------------------------------------------
static bool
interrogationHandler(void* parameter, IMasterConnection connection, CS101_ASDU asdu, uint8_t qoi)
{
    printf("Received interrogation for group %i\n", qoi);
    return true;
}

void LogTXwT(int ioa,float value,CP56Time2a timestamp ){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen ("log/log.txt", "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d IOA: %i value: %f with timestamp: %04i-%02i-%02i %02i:%02i:%02i.%03i \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
            tm.tm_sec,ioa,value,CP56Time2a_getYear(timestamp) + 2000 ,CP56Time2a_getMonth(timestamp),CP56Time2a_getDayOfMonth(timestamp),
            CP56Time2a_getHour(timestamp), CP56Time2a_getMinute(timestamp), CP56Time2a_getSecond(timestamp), CP56Time2a_getMillisecond(timestamp));
    fclose(fp);
}

CS101_ASDU asdu2Send;
CS101_ASDU CreateMONITOR_IN_ASDU(CS101_AppLayerParameters alParams, int cot, CP56Time2a timestamp){

    asdu2Send = CS101_ASDU_create(alParams, false, cot, ORIGINATOR_ADDRESS, COMMON_ADDRESS, false, false);


        for (int j = 0; j < 4; ++j) {
            InformationObject io = (InformationObject) MeasuredValueShortWithCP56Time2a_create(NULL,NULL, monitorIN[j], IEC60870_QUALITY_GOOD,timestamp);
            CS101_ASDU_addInformationObject(asdu2Send, io);
            if (DATA_LOGS){
                LogTXwT(InformationObject_getObjectAddress((InformationObject) io),MeasuredValueShort_getValue((MeasuredValueShort) io), timestamp);
            }
            //tmpSharedValue=MeasuredValueShort_getValue((MeasuredValueShort) io);
            printf("    IOA: %i with value: %f \n",1000,MeasuredValueShort_getValue((MeasuredValueShort)io));

            InformationObject_destroy(io);
        }


    return asdu2Send;

}
//---------------------------------------------------------------------------------------
double nahodnehodnoty(double aktual, double min, double max, double step, int scenare)
{
    randomNumber = rand() % 2; //3;
    randomNumber +=1;
    printf("random = %d\n", randomNumber);
    printf("aktual = %lf \n", aktual);
    printf("min = %lf\n", min);
    printf("max = %lf\n", max);
    printf("step = %lf\n", step);
    printf("scenar = %d\n", scenare);

    if (step <= 0) {
        step=0.001;
    }

    int n = (int)((max - min) / step) + 1;
    int size = n;
    if (n <= 0) {
        printf("Chybny rozsah nebo krok!\n");
        return aktual;
    }


        if ((aktual <= min) && (scenare != 0))
        {
            aktual += step;

        }
        else if ((aktual >= max) && (scenare != 2))
        {
            aktual -= step;

        }
        else if(scenare == 0)
        {
            aktual -= step* 10;
        }
        else if (scenare == 2)
        {
            aktual += step* 10;
        }
        else
        {
            switch (randomNumber)
            {
                case 0:
                    break;
                case 1:
                    aktual = aktual -= step;
                break;
                case 2:
                    aktual = aktual += step;
                break;
                default:
                    break;
            }

        }


    printf("aktual = %lf\n-----\n", aktual);

    return (aktual);
}

//------------------------





void getTraficLoad(int act_min) {
    FILE* fp = fopen("conf/LoadDiagram.txt", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }
    char line[1024];
    bool readDiagram = true;

    // Čtení prvního řádku souboru
    if (fgets(line, sizeof(line), fp) != NULL) {
        char* pch = strtok(line, " ");
        if (pch != NULL) {
            char* value = strtok(NULL, " ");
            if (value != NULL) {
                value[strcspn(value, "\r\n")] = '\0';
                if (strcmp(value, "-1") == 0) {
                    readDiagram = true; // Pokud hodnota je -1, čte diagram
                } else {
                    act_load = atof(value); // Pokud není -1, nastaví aktuální hodnotu
                    readDiagram = false;
                }
            }
        }
    }
    printf("getTraficLoad readDiagrm:%i \n", readDiagram);


    while (fgets(line, sizeof(line), fp) != NULL && readDiagram) {
        char* minute = strtok(line, " ");
        char* load = strtok(NULL, " ");
        if (minute != NULL && load != NULL) {
            minute[strcspn(minute, "\r\n")] = '\0';
            load[strcspn(load, "\r\n")] = '\0';

            if (atoi(minute) == act_min) {
                act_load = atof(load);
                break; // Hodnota nalezena
            }
        }
    }/**/
    fclose(fp);
}

void zapisDoSouboru(const char *text)
{
    FILE *soubor = fopen("log/log.txt", "a");
    if (soubor == NULL)
    {
        perror("Chyba při otevírání souboru");
        exit(1);
    }

    fprintf(soubor, "%s\n", text);
    fclose(soubor);
}

void printonehundredfour(){
    printf("104 ioa:\n");
    for (int i = 0; i < 15; i++) {
        printf("%d -> %d\n", onehundredfourioa[i][0], onehundredfourioa[i][1]);
    }

}

// Log
//data logs

void LogTX(int type,int elements){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;

    fp = fopen ("log/log.txt", "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d type(%i) elements: %i\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,type,elements);
    fclose(fp);
}

// event logs
void LogSTART()
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE *fp;
    fp = fopen("log/EventLog.txt", "a");
    fprintf(fp, "%d-%02d-%02d %02d:%02d:%02d Server stared \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fclose(fp);
}
void LogSETUP_G()
{ // nepokazilo se
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE *fp;
    fp = fopen("log/EventLog.txt", "a");
    fprintf(fp, "%d-%02d-%02d %02d:%02d:%02d Setup OK \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fclose(fp);
}
void LogSETUP_F()
{ // pokazilo se
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE *fp;
    fp = fopen("log/EventLog.txt", "a");
    fprintf(fp, "%d-%02d-%02d %02d:%02d:%02d Setup failed \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fclose(fp);
}

// data logs

// undervoltage overvoltage
void LogUnder005()
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE *fp;
    fp = fopen("log/EventLog.txt", "a");
    fprintf(fp, "%d-%02d-%02d %02d:%02d:%02d LD0.VMMXU1.LoWrn.stVal \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fclose(fp);
}

void LogUnder01()
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE *fp;
    fp = fopen("log/EventLog.txt", "a");
    fprintf(fp, "%d-%02d-%02d %02d:%02d:%02d LD0.VMMXU1.LoAlm.stVal \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fclose(fp);
}
void LogOver005()
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE *fp;
    fp = fopen("log/EventLog.txt", "a");
    fprintf(fp, "%d-%02d-%02d %02d:%02d:%02d LD0.VMMXU1.HiWrn.stVal \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fclose(fp);
}

void LogOver01()
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE *fp;
    fp = fopen("log/EventLog.txt", "a");
    fprintf(fp, "%d-%02d-%02d %02d:%02d:%02d LD0.VMMXU1.HiAlm.stVal \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fclose(fp);
}
// zkrat

void LogshortCircuit()
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE *fp;
    fp = fopen("log/EventLog.txt", "a");
    fprintf(fp, "%d-%02d-%02d %02d:%02d:%02d LD0.VMMXU1.HiAlm.stVal \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fclose(fp);
}

// start
int main()
{

    /* Add Ctrl-C handler */
    signal(SIGINT, sigint_handler);

    /* create a new slave/server instance with default connection parameters and
     * default message queue size */
    CS104_Slave slave = CS104_Slave_create(100, 100);

    //CS104_Slave_setLocalAddress(slave, "0.0.0.0"); // "192.168.126.138" "0.0.0.0" "192.168.100.111" "192.168.100.101"
    CS104_Slave_setLocalAddress(slave, str_IP);

    /* Set mode to a single redundancy group
     * NOTE: library has to be compiled with CONFIG_CS104_SUPPORT_SERVER_MODE_SINGLE_REDUNDANCY_GROUP enabled (=1)
     */
    CS104_Slave_setServerMode(slave, CS104_MODE_SINGLE_REDUNDANCY_GROUP);

    /* get the connection parameters - we need them to create correct ASDUs */
    CS101_AppLayerParameters alParams = CS104_Slave_getAppLayerParameters(slave);

    /* set the callback handler for the clock synchronization command */
    CS104_Slave_setClockSyncHandler(slave, clockSyncHandler, NULL);

    /* set the callback handler for the interrogation command */
    CS104_Slave_setInterrogationHandler(slave, interrogationHandler, NULL);

    /* set handler for other message types */
    //CS104_Slave_setASDUHandler(slave, asduHandler, NULL);

    /* set comunication port */
    CS104_Slave_setLocalPort(slave,PORT);
    //CS104_Slave_createSecure(1,255,TLSConfiguration_create());

    /* set handler to handle connection requests (optional)
    CS104_Slave_setConnectionRequestHandler(slave, connectionRequestHandler, NULL);*/

    /* set handler to track connection events (optional)
    CS104_Slave_setConnectionEventHandler(slave, connectionEventHandler, NULL);*/

    /* uncomment to log messages */
    //CS104_Slave_setRawMessageHandler(slave, rawMessageHandler, NULL);

    CS104_Slave_start(slave);


    bool prestart = true;
    // pr
    int time_monitorTimer;
    int seconds;
    int time_tmp=-1;
    int time_now;
    double underovervoltageValue = 0, underovervoltageValuePlus = 1;
    int underovervoltagecounter;
    int underovervoltageValueFlag;
    bool sendRecovery;

    srand(time(NULL)); // generovani
    char text[MAX_LENGTH];
    running = true;

    // log
    onehundredfour();
    LogSTART();
    printf("nacitani\n");
    readInput();
    LogSETUP_G();
    printf("Vse nacteno\n");
    while (running)
    {

        seconds = time(NULL)%60;
        time_now = seconds;



        // při změně času
        if (time_now != time_tmp)
        {
            time_monitorTimer+=1;
            time_tmp = time_now;


            time_t t = time(NULL); // Získání aktuálního času v sekundách od epochy
            struct tm tm = *localtime(&t); // Převedení na lokální čas

            printf("Aktualni cas a datum: %02d-%02d-%04d %02d:%02d:%02d\n",
                   tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
                   tm.tm_hour, tm.tm_min, tm.tm_sec);

            dayInMinutes = (tm.tm_hour * 60) + tm.tm_min;
            printf("Time now: %i \n", dayInMinutes);
            sleep(20);
            getTraficLoad(dayInMinutes); // nacteni act_load ze souboru
            printf("act_load je: %f \n", act_load);

            // scénáře
            nacteniscenare();
            if (prestart == true)
            {

                underovervoltageValue = 0;
                underovervoltageValuePlus = 1;
                underovervoltageValueFlag=0;
                underovervoltagecounter = 0;
                shortCircuitphase = 0;
                holderI = 0;
                waitshortCircuit = 0;

                probihaiciscenarI = false;
                shortCircuit = false;
                undervoltage = false;
                overvoltage = false;
                alarm = 0;
                probihaiciscenar = false;
                prestart = false;
                sendRecovery=true;
            }



            // podpětí
            else if (scenar == 0 && (underovervoltagecounter < BREAKER_DELAY))
            {
                if (undervoltage == false)
                {
                    underovervoltageValue = variables[0][0];
                }

                underovervoltageValue = nahodnehodnoty(underovervoltageValue, variables[0][1], variables[0][2], variables[0][3], 0); // U
                variables[0][0] = underovervoltageValue;

                // variables[0][0]+=FAILURE_EFFECT;

                underovervoltagecounter += 1; // čeká na t=5
                undervoltage = true;
                probihaiciscenar = true;
            }

            // prepeti
            else if (scenar == 2 && (underovervoltagecounter < BREAKER_DELAY))
            {

                if (overvoltage == false)
                {
                    underovervoltageValue = variables[0][0];
                }

                underovervoltageValue = nahodnehodnoty(underovervoltageValue, variables[0][1], variables[0][2], variables[0][3], 2); // U
                variables[0][0] = underovervoltageValue;

                // variables[0][0]+=FAILURE_EFFECT;

                underovervoltagecounter += 1; // čeká na t=5

                overvoltage = true;
                probihaiciscenar = true;
            }

            // podpeti prepeti
            else if (((overvoltage == true) || (undervoltage == true)) && (underovervoltagecounter >= BREAKER_DELAY))
            {

                if ((underovervoltageValuePlus != 0.9) && undervoltage == true)
                { // under
                    underovervoltageValuePlus -= 0.005;
                    underovervoltageValue = nahodnehodnoty(underovervoltageValue, variables[0][1], variables[0][2], variables[0][3], 0); // U
                }
                else if ((underovervoltageValuePlus != 1.1) && overvoltage == true)
                { // over
                    underovervoltageValuePlus += 0.005;
                    underovervoltageValue = nahodnehodnoty(underovervoltageValue, variables[0][1], variables[0][2], variables[0][3], 2); // U
                }

                variables[0][0] = underovervoltageValue * underovervoltageValuePlus;


                if (variables[0][0] < (((variables[0][1] + variables[0][2]) / 2) * 0.9))
                {
                    if (underovervoltageValueFlag==0)
                    {
                        LogUnder01();
                        onehundredfourioa[8][1]= 1;//5004 = 1
                        onehundredfourioa[11][1]= 1;//5011 = 1;


                        printf("%f > %f", ((variables[0][1] + variables[0][2]) / 2),variables[0][0]);
                        underovervoltageValueFlag+=1;

                        scenar = 6;
                    }
                }
                else if (variables[0][0] < (((variables[0][1] + variables[0][2]) / 2) * 0.95))
                {
                    if (underovervoltageValueFlag==1)
                    {
                    LogUnder005();
                    onehundredfourioa[14][1]= 1;//5017 = 1;
                    printf("%f > %f", ((variables[0][1] + variables[0][2]) / 2),variables[0][0]);
                    underovervoltageValueFlag+=1;
                    }
                }
                else if (variables[0][0] > (((variables[0][1] + variables[0][2]) / 2) * 1.1))
                {
                    if (underovervoltageValueFlag==2)
                    {
                    LogOver01();
                    onehundredfourioa[8][1]= 1;
                    onehundredfourioa[12][1]= 1;
                    //zapsani 5004 = 0; 50012=0;
                    printf("%f < %f", ((variables[0][1] + variables[0][2]) / 2),variables[0][0]);
                    underovervoltageValueFlag+=1;
                        scenar = 6;
                    }
                }
                else if (variables[0][0] > (((variables[0][1] + variables[0][2]) / 2) * 1.05))
                {
                    if (underovervoltageValueFlag==3)
                    {
                    LogOver005();
                    onehundredfourioa[15][1]= 1;
                    //5018 = 0;
                    printf("%f < %f", ((variables[0][1] + variables[0][2]) / 2),variables[0][0]);
                    underovervoltageValueFlag+=1;
                    }
                }

                alarm += 1;
            }
/*
        {4000, 0}, 0
        {4001, 0},1
        {4002, 0},2
        {4003, 0},3
        {5000, 0},4
        {5001, 0},5
        {5002, 0},6
        {5003, 0},7
        {5004, 0},8
        {5006, 0},9
        {5007, 0},10
        {5011, 0},11
        {5012, 0},12
        {5014, 0},13
        {5017, 0},14
        {5018, 0}  15*/
            /*
             5001, 5004, 5006=0->1	aktivace zkratové ochrany
            4001=2->1	působení zkratové ochrany
            ??? - jde do scady	aktivace master tripu - povel k vypnutí vypínače
            5002=0->1	povel k vypnutí vypínače od bloku, který řídí opětovné zapínání
            -	Povel k blokování opětovného zapnutí ochraně na druhém konci vedení
            4002=2->0/3	vypínač není sepnut (ale ještě není ani rozepnut)
            4002=0/3->1	vypínač je rozepnut (nyní je měřený proud 0 A)
            -	odpadnutí oc i tripu po rozepnutí vypínače
            5001=1->0
            5002=1->0
            5004, 5006=1->0
            5001=0->1	pous o opětovné zapnutí (pulz 150ms)
            5002=0->1
            4002=1->0/3	vypínač není rozepnut (ale ještě není sepnut)
            4002=0/3->2	vypínač je sepnut
            5001,5002=1->0	Pouze pokud je pokus o opětovné zapnutí úspěšný. Jinak následuje finální vypnutí


            */

            // zkrat 1
            // zkrat 2
            else if ((scenar == 3 || scenar == 4))
            {

                // odpočet prvních 5
                if (waitshortCircuit < BREAKER_DELAY)
                {
                    waitshortCircuit += 1;
                }

                if (waitshortCircuit > BREAKER_DELAY)
                {
                    switch (shortCircuitphase)
                    {
                    case 0:
                        printf("Short Circuit");
                        onehundredfourioa[5][1]= 1;
                        onehundredfourioa[8][1]= 1;
                        onehundredfourioa[9][1]= 1;
                        onehundredfourioa[1][1]= 1;//4001
                        onehundredfourioa[6][1]= 1;
                        waitshortCircuit += 1;
                        shortCircuitphase = 1;
                        break;

                    case 1:
                        shortCircuit = true;
                        probihaiciscenarI = true;
                        holderI = variables[1][0];
                        variables[1][0] *= 4;
                        shortCircuitphase += 1;
                        onehundredfourioa[2][1]= 0;//4002
                        break;

                    case 2:
                        variables[1][0] = 0;
                        shortCircuitphase += 1;
                        onehundredfourioa[2][1]= 1;//4002
                        printf("Short Circuit: I=0");

                        onehundredfourioa[5][1]= 0;
                        onehundredfourioa[6][1]= 0;
                        onehundredfourioa[8][1]= 0;
                        onehundredfourioa[9][1]= 0;
                        break;

                    case 3:
                        onehundredfourioa[5][1]= 1;
                        onehundredfourioa[6][1]= 1;
                        onehundredfourioa[2][1]= 0;
                        onehundredfourioa[2][1]= 2;
                        if (scenar == 3)
                        {
                            variables[1][0] = holderI;
                            probihaiciscenarI = false;
                            onehundredfourioa[5][1]= 0;
                            onehundredfourioa[6][1]= 0;
                            printf("short circuit recovery ");
                        }
                        else if (scenar == 4)
                        {
                            variables[1][0] = 0;
                            variables[1][4] = 0;
                            printf("Short Circuit: I=0");
                        }
                        shortCircuitphase += 1;
                        break;

                        default:
                        break;
                    }
                    printonehundredfour();
                    printf("Short Circuit I=%f", variables[1][0]);

                    if (scenar == 4 && shortCircuitphase > 3)
                    {

                        variables[1][0] = 0;
                        variables[1][4] = 0;
                    }
                }
            }


            // monitory a ukládání
            // změna proměnných

            if (probihaiciscenar == false)
            {
                printf("pocitani promennych U \n");
                // double aktual, double min, double max, double step
                variables[0][0] = nahodnehodnoty(variables[0][0], variables[0][1], variables[0][2], variables[0][3], 1); // U
            }
            if (probihaiciscenarI == false)
            {
                variables[1][0] = nahodnehodnoty(variables[1][0], variables[1][1], variables[1][2], variables[1][3], 1) ; // I

            }
            variables[1][4] = variables[1][0]* act_load;
            variables[2][0] = nahodnehodnoty(variables[2][0], variables[2][1], variables[2][2], variables[2][3], 1); // frequency

            // u, i, p, q
            monitorIN[0] = variables[0][0];
            monitorIN[1] = variables[1][4];

            monitorIN[2] = variables[0][0] * variables[1][4] * sin(INPUT_POWER_FACTOR); // Q_in =U*I*sin(alfa) alfa=10°
            monitorIN[3] = variables[0][0] * variables[1][4] * cos(INPUT_POWER_FACTOR); // P_in =U*I*cos(alfa) alfa=10°

            printf("U %f; I: %f; Q: %f; P: %f, f: %f \n", monitorIN[0], monitorIN[1], monitorIN[2], monitorIN[3], variables[2][0]);

            // uložení do souboru
            snprintf(text, MAX_LENGTH, "Hodnoty jsou: %.5f, %.5f, %.5f, %.5f, ", monitorIN[0], monitorIN[0], monitorIN[0], monitorIN[0]);
            zapisDoSouboru(text);

            // monitorOUT[]
            if (probihaiciscenar == false)
            { // P=U_out * I_out * cos(phi) uz nebude z textaku =>
                // U_out
                MONITOR_OUT_objects[0][0] = nahodnehodnoty(MONITOR_OUT_objects[0][0], MONITOR_OUT_objects[0][1], MONITOR_OUT_objects[0][2], MONITOR_OUT_objects[0][3], 1);
                monitorOUT[0] = MONITOR_OUT_objects[0][0];
            }

            if (monitorOUT[0] == 0)
            {
                monitorOUT[1] = 0; // I_out = 0
            }
            else
            {
                monitorOUT[1] = (monitorIN[0] * monitorIN[1]) / monitorOUT[0]; // I_out = (U_in*I_in)/U_out
            }
            monitorOUT[2] = monitorOUT[0] * monitorOUT[1] * cos(TRANSFORMER_POWER_FACTOR * M_PI / 180); // P=U*I*cos(alfa)
            monitorOUT[3] = monitorOUT[0] * monitorOUT[1] * sin(TRANSFORMER_POWER_FACTOR * M_PI / 180); // Q=U*I*sin(alfa)
            printf("%f; %f; %f; %f\n", monitorOUT[0], monitorOUT[1], monitorOUT[2], monitorOUT[3]);


        }

        // změna dat, UIQ

        //in response time
        if(time_monitorTimer>=PER_RESPONSE_time || alarm>=ALARM_PERIOD_time || sendRecovery==true) {
            if (alarm>=ALARM_PERIOD_time || sendRecovery==true){
                alarm=0;
                sendRecovery=false;
                printf("Alarm\n");
                cot=3;
            }else{
                cot=1;
            }

            //single values

            //time_monitorTimer=0;

            //in values

            printf("SENDED ASDU type: 36 elements: %i \n", 4);
            if (DATA_LOGS){
                LogTX(36,4);
            }
            CS101_ASDU newAsdu = CreateMONITOR_IN_ASDU(alParams, cot,CP56Time2a_createFromMsTimestamp(NULL, Hal_getTimeInMs()));
            CS104_Slave_enqueueASDU(slave, newAsdu);
            CS101_ASDU_destroy(newAsdu);
            time_monitorTimer=0;
        }
            //dočasne
            //scenar=6;
        if (scenar == 6)
        {
            running = false;
        }

    }



    sleep(200);
    return 0;
}
