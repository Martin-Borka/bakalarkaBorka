#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "cs104_slave.h"
#include "hal_thread.h"
#include "hal_time.h"
#include <time.h>
#include <math.h>
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
static bool running = true;

bool serverConnected=0;

char str_INparam[256]="-1";
int INparameter=2404;               //pro test
char strParam[256];

float sharedMatrixPER[32][3];
//float tmpSharedValue;

char str_IP[256]="-1";
int chanPort;
char str_OA[256]="-1";
int originatorAddress=-1;
int commonAddress=0;
bool dataLog=0;

//monitor values
//float monitor_angle;
//float angleCos;
//float angleSin;
int periodicTime=0;

bool monitor_inUsed=0;
int monitor_in[127][5];
float monitor_in_actual[127][4];
int monitor_inSet=0;

bool monitor_outUsed=0;
int monitor_out[127][3];
float monitor_out_actual[127][4];
int monitor_outSet=0;

float act_load;
float input_power_factor = 1;
float transformer_power_factor = 5;

int generating[5]={1,1,1,1,1};
bool reloadedVal=true;
int alarmTime=0;
bool receivedCommand=false;
int overvoltage=0;
int undervoltage=0;
int alarm=0;
int failureEffect=0;
int breaker_delay=0;
bool shortCircuit=false;
bool underOverVoltage=false;
bool offBreaker=false;
bool offDiscon=false;
bool sendRecovery=false;
bool offBreakerRemote=false;
bool offBreakerRemoteDone=false;
bool offDisconRemote=false;
bool offDisconRemoteDone=false;


bool tempFreqeUsed=0;
float temperature[2];
float temperatureRange[2];
float temperature_amb[3];
float monitor_freq[3];

bool monitor4Used=0;
int monitor4[127][2];
int monitor4Set=0;

bool monitor2Used=0;
int monitor2[127][2];
int monitor2Set=0;

int cot=0;

int matrixRow=0;

char strPort[256];
char matrixPath[256];
char logTXPath[256];
char logRXPath[256];

void sharedMatrixPER_clr(){
    for (int j = 0; j < 32; ++j) {
        sharedMatrixPER[j][0]=0;
        sharedMatrixPER[j][1]=0;
        sharedMatrixPER[j][2]=0;
    }
}


void nacteniMC(int param){

    snprintf(strParam, sizeof(strParam), "%d", param);
    strcpy(str_INparam,"MainConfig_server");
    strcat(str_INparam,strParam);
    strcat(str_INparam,".txt");

    FILE* fp;
    //fp = fopen("MainConfig_server2404.txt", "r")
    fp = fopen(str_INparam, "r");
    char * line = NULL;
    size_t len = 0;

    while (getline(&line, &len, fp) != -1) {
        char* pch;
        pch = strtok (line,"=, ;-");
        while (pch != NULL){

            if(!(strcmp(pch,"IPADDRESS"))){         //pro test
                pch = strtok (NULL, "=, ;-");
                for (int j = 0; j < strlen(pch)-2; j++) { //-2 ->-1
                    str_IP[j]=pch[j];
                    str_OA[j]=pch[j];
                }
                str_IP[strlen(pch)+1]='\0';
                str_OA[strlen(pch)+1]='\0';
            }

            else if(!(strcmp(pch,"PORT"))){         //pro test
                pch = strtok (NULL, "=, ;-");
                chanPort=atoi(pch);
            }
            else if(!(strcmp(pch,"SERVER_ID"))){    //pro test
                pch = strtok (NULL, "=, ;-");
                originatorAddress=atoi(pch);
            }
            else if(!(strcmp(pch,"COMMON_ADDRESS"))){
                pch = strtok (NULL, "=, ;-");
                commonAddress=atoi(pch);
            }
            else if(!(strcmp(pch,"DATA_LOGS"))){
                pch = strtok (NULL, "=, ;-");
                if (atoi(pch)==1){
                    dataLog=true;
                }
            }
            else if(!(strcmp(pch,"PER_RESPONSE_time"))){
                pch = strtok (NULL, "=, ;-");
                periodicTime=atoi(pch);
            }

            else if(!(strcmp(pch,"ALARM_PERIOD_time"))){
                pch = strtok (NULL, "=, ;-");
                alarmTime=atoi(pch);
            }
            else if(!(strcmp(pch,"FAILURE_EFFECT"))){
                pch = strtok (NULL, "=, ;-");
                failureEffect=atoi(pch);
            }

            else if(!(strcmp(pch,"BREAKER_DELAY"))){
                pch = strtok (NULL, "=, ;-");
                breaker_delay=atoi(pch);
            }

            //angles
            else if(!(strcmp(pch,"INPUT_POWER_FACTOR"))){
                pch = strtok (NULL, "=, ;-");
                input_power_factor=atof(pch);
            }
            else if(!(strcmp(pch,"TRANSFORMER_POWER_FACTOR"))){
                pch = strtok (NULL, "=, ;-");
                transformer_power_factor=atof(pch);
            }

            //monitor

            //in
            else if(!(strcmp(pch,"MONITOR_IN"))){
                pch = strtok (NULL, "=, ;-");
                if (atoi(pch)==1){
                    monitor_inUsed=true;
                }
            }
            else if(!(strcmp(pch,"MONITOR_IN_objects"))){
                pch = strtok (NULL, "=, ;-");
                int i=0;
                while (pch != NULL){
                    monitor_in[i][0]=atoi(pch);
                    //printf("pch: %s \n",pch);
                    pch = strtok (NULL, "=, ;-");
                    monitor_in[i][1]=atoi(pch);
                    //printf("pch1: %s \n",pch);
                    pch = strtok (NULL, "=, ;-");
                    //printf("pch2: %s \n",pch);
                    monitor_in[i][2]=atoi(pch);
                    pch = strtok (NULL, "=, ;-");
                    //printf("pch3: %s \n",pch);
                    monitor_in[i][3]=atoi(pch);
                    pch = strtok (NULL, "=, ;-");
                    //printf("pch3: %s \n",pch);
                    monitor_in[i][4]=atoi(pch);
                    pch = strtok (NULL, "=, ;-");
                    //printf("pch3: %s \n",pch);
                    i+=1;
                    monitor_inSet+=1;

                }
            }

            //out
            else if(!(strcmp(pch,"MONITOR_OUT"))){
                pch = strtok (NULL, "=, ;-");
                if (atoi(pch)==1){
                    monitor_outUsed=true;
                }
            }
            else if(!(strcmp(pch,"MONITOR_OUT_objects"))){
                pch = strtok (NULL, "=, ;-");
                int i=0;
                while (pch != NULL){
                    monitor_out[i][0]=atoi(pch);
                    pch = strtok (NULL, "=, ;-");
                    monitor_out[i][1]=atoi(pch);
                    pch = strtok (NULL, "=, ;-");
                    monitor_out[i][2]=atoi(pch);
                    pch = strtok (NULL, "=, ;-");
                    //monitor_out[i][3]=atoi(pch);
                    //pch = strtok (NULL, "=, ;-");
                    //monitor_out[i][4]=atoi(pch);
                    //pch = strtok (NULL, "=, ;-");
                    i+=1;
                    monitor_outSet+=1;
                }
            }

            //temperature+frequency
            else if(!(strcmp(pch,"MONITOR_TEMP_FREQ"))){
                pch = strtok (NULL, "=, ;-");
                if (atoi(pch)==1){
                    tempFreqeUsed=true;
                }
            }
            else if(!(strcmp(pch,"MONITOR_TEMPERATURE_object"))){
                pch = strtok (NULL, "=, ;-");
                temperature[0]=atoi(pch);
                pch = strtok (NULL, "=, ;-");
                temperature[1]=atoi(pch);
                pch = strtok (NULL, "=, ;-");
                temperatureRange[0]=atoi(pch);
                pch = strtok (NULL, "=, ;-");
                temperatureRange[1]=atoi(pch);
                pch = strtok (NULL, "=, ;-");
                temperature_amb[0]=atoi(pch);
                pch = strtok (NULL, "=, ;-");
                temperature_amb[1]=atoi(pch);
            }
            else if(!(strcmp(pch,"MONITOR_FREQ_objects"))){
                pch = strtok (NULL, "=, ;-");
                monitor_freq[0]=atoi(pch);
                //printf("pch: %s \n",pch);
                pch = strtok (NULL, "=, ;-");
                monitor_freq[1]=atof(pch);
                //printf("pch1: %s \n",pch);
                pch = strtok (NULL, "=, ;-");
                //printf("pch2: %s \n",pch);
                monitor_freq[2]=atof(pch);
                pch = strtok (NULL, "=, ;-");
                //printf("pch3: %s \n",pch);
            }

            else if(!(strcmp(pch,"MONITOR4"))){
                pch = strtok (NULL, "=, ;-");
                if (atoi(pch)==1){
                    monitor4Used=true;
                }
            }
            else if(!(strcmp(pch,"MONITOR4_objects"))){
                pch = strtok (NULL, "=, ;-");
                int i=0;
                while (pch != NULL){
                    monitor4[i][0]=atoi(pch);
                    pch = strtok (NULL, "=, ;-");
                    monitor4[i][1]=atoi(pch);
                    pch = strtok (NULL, "=, ;-");
                    i+=1;
                    monitor4Set+=1;
                }
            }

            else if(!(strcmp(pch,"MONITOR2"))){
                pch = strtok (NULL, "=, ;-");
                if (atoi(pch)==1){
                    monitor2Used=true;
                }
            }

            else if(!(strcmp(pch,"MONITOR2_objects"))){
                pch = strtok (NULL, "=, ;-");
                int i=0;
                while (pch != NULL){
                    monitor2[i][0]=atoi(pch);
                    pch = strtok (NULL, "=, ;-");
                    monitor2[i][1]=atoi(pch);
                    pch = strtok (NULL, "=, ;-");
                    i+=1;
                    monitor2Set+=1;
                }
            }


            pch = strtok (NULL, "=, ;-");
        }

    }

    fclose(fp);

}

void getStatus(){
    FILE* fp;
    fp = fopen("ServerStatus.txt", "r");
    char * line = NULL;
    size_t len = 0;
    int i=0;
    while (getline(&line, &len, fp) != -1) {
        char* pch;
        pch = strtok (line,"=, ;-");
        //printf("pch je: -%s- \n", pch);
        pch = strtok(NULL, "=, ;-");
        pch[strlen(pch)-2]='\0'; //-2 ->-1
        //printf("pch2 je: -%s- \n", pch);
        generating[i]=atoi(pch);
        i+=1;
    }
    fclose(fp);
}



void getTraficLoad(int act_min){
    FILE* fp;
    fp = fopen("LoadDiagram.txt", "r");
    char* line = NULL;
    size_t len = 0;
    bool readDiagram=true;

    if ((getline(&line, &len, fp) != -1)){
        char* pch;
        pch = strtok (line," ");
        pch = strtok(NULL, " ");
        pch[strlen(pch)-2]='\0'; //-2 ->-1
        if (atoi(pch) != -1){
            readDiagram=false;
            act_load = atof(pch);
        }
    }

    while (getline(&line, &len, fp) != -1 && readDiagram) {
        char* pch;
        pch = strtok (line," ");
        if (atoi(pch) == act_min){
            //printf("pch1 je: -%s- \n", pch);
            pch = strtok(NULL, " ");
            pch[strlen(pch)-2]='\0'; //-2 ->-1
            //printf("pch2 je: -%s- \n", pch);
            act_load = atof(pch);
            //printf("pch final je: -%f- \n", act_load);
        }
    }
    fclose(fp);

}


void
sigint_handler(int signalId)
{
    running = false;
}

float getRandomVal(float min, float max){
    return ((float)rand()/(float)(RAND_MAX)) * (max-min) + min;
}

float getTemperature(float temp){
    if (temp>temperatureRange[1]){
        return temp-1;
    }
    if (temp<temperatureRange[0]){
        return temp+1;
    }
    return temp-1+((float)rand()/(float)(RAND_MAX))*2;
}

float getTemperatureAmb(){
    return ((float)rand()/(float)(RAND_MAX)) * (temperature_amb[1]-temperature_amb[0]) + temperature_amb[0];
}



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

void
printCP24Time2a(CP24Time2a time)
{
    printf("%02i:%02i.%03i \n",
           CP24Time2a_getMinute(time),
           CP24Time2a_getSecond(time),
           CP24Time2a_getMillisecond(time));
}

/* Callback handler to log sent or received messages (optional) */
static void
rawMessageHandler(void* parameter, IMasterConnection conneciton, uint8_t* msg, int msgSize, bool sent)
{
    if (sent)
        printf("SEND: ");
    else
        printf("RCVD: ");

    int i;
    for (i = 0; i < msgSize; i++) {
        printf("%02x ", msg[i]);
    }

    printf("\n");
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


static bool
interrogationHandler(void* parameter, IMasterConnection connection, CS101_ASDU asdu, uint8_t qoi)
{
    printf("Received interrogation for group %i\n", qoi);
    return true;
}

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

void LogTX(int type,int elements){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;

    fp = fopen (logTXPath, "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d type(%i) elements: %i\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,type,elements);
    fclose(fp);
}

void LogTXwoT(int ioa,float value){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen (logTXPath, "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d IOA: %i value: %f\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
            tm.tm_sec,ioa,value);
    fclose(fp);
}


void LogTXwT(int ioa,float value,CP56Time2a timestamp ){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen (logTXPath, "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d IOA: %i value: %f with timestamp: %04i-%02i-%02i %02i:%02i:%02i.%03i \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
            tm.tm_sec,ioa,value,CP56Time2a_getYear(timestamp) + 2000 ,CP56Time2a_getMonth(timestamp),CP56Time2a_getDayOfMonth(timestamp),
            CP56Time2a_getHour(timestamp), CP56Time2a_getMinute(timestamp), CP56Time2a_getSecond(timestamp), CP56Time2a_getMillisecond(timestamp));
    fclose(fp);
}

void LogTXwT24(int ioa,float value,CP24Time2a timestamp ){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen (logTXPath, "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d IOA: %i value: %f with timestamp: %02i:%02i.%03i \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
            tm.tm_sec,ioa,value, CP24Time2a_getMinute(timestamp), CP24Time2a_getSecond(timestamp), CP24Time2a_getMillisecond(timestamp));
    fclose(fp);
}


//data logs

void LogRX(int type,int elements){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen (logRXPath, "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d type(%i) elements: %i\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,type,elements);
    fclose(fp);
}

void LogRXwoT(int ioa,float value){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen (logRXPath, "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d IOA: %i value: %f\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
            tm.tm_sec,ioa,value);
    fclose(fp);
}
void LogRXwT(int ioa,float value,CP56Time2a timestamp ){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen (logRXPath, "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d IOA: %i value: %f with timestamp: %04i-%02i-%02i %02i:%02i:%02i.%03i \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
            tm.tm_sec,ioa,value,CP56Time2a_getYear(timestamp) + 2000 ,CP56Time2a_getMonth(timestamp),CP56Time2a_getDayOfMonth(timestamp),
            CP56Time2a_getHour(timestamp), CP56Time2a_getMinute(timestamp), CP56Time2a_getSecond(timestamp), CP56Time2a_getMillisecond(timestamp));
    fclose(fp);
}
void LogRXC(int ioa,float value){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen (logRXPath, "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d IOA: %i qoi: %f\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
            tm.tm_sec,ioa,value);
    fclose(fp);
}
void LogRXT(int ioa,CP56Time2a timestamp){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen (logRXPath, "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d IOA: %i with timestamp: %04i-%02i-%02i %02i:%02i:%02i.%03i \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
            tm.tm_sec,ioa,CP56Time2a_getYear(timestamp) + 2000 ,CP56Time2a_getMonth(timestamp),CP56Time2a_getDayOfMonth(timestamp),
            CP56Time2a_getHour(timestamp), CP56Time2a_getMinute(timestamp), CP56Time2a_getSecond(timestamp), CP56Time2a_getMillisecond(timestamp));
    fclose(fp);
}

static bool
asduHandler(void* parameter, IMasterConnection connection, CS101_ASDU asdu)
{
    printf("RECVD ASDU type: %s(%i) elements: %i from klient\n",
           TypeID_toString(CS101_ASDU_getTypeID(asdu)),
           CS101_ASDU_getTypeID(asdu),
           CS101_ASDU_getNumberOfElements(asdu)
    );
    if (dataLog){
        LogRX(CS101_ASDU_getTypeID(asdu),CS101_ASDU_getNumberOfElements(asdu));
    }

    if (CS101_ASDU_getTypeID(asdu) == C_SC_NA_1) {  //45
        int i;
        receivedCommand=true;
        for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {
            SingleCommand io = (SingleCommand) CS101_ASDU_getElement(asdu,i);
            printf("    IOA: %i value: %i\n", InformationObject_getObjectAddress((InformationObject) io), SingleCommand_getState((SingleCommand) io));
            if (dataLog){
                LogRXwoT(InformationObject_getObjectAddress((InformationObject) io), SingleCommand_getState((SingleCommand) io));
            }
            /*for (int j = 0; j < monitor2Set; ++j) {
                if(monitor2[j][0]==InformationObject_getObjectAddress((InformationObject) io)){
                    monitor2[j][1]=SingleCommand_getState((SingleCommand) io);
                }

            }*/
            SingleCommand_destroy(io);
        }
    }
    else if (CS101_ASDU_getTypeID(asdu) == C_DC_NA_1) {  //46
        int i;

        for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {
            DoubleCommand io = (DoubleCommand) CS101_ASDU_getElement(asdu,i);
            printf("    IOA: %i value: %i\n", InformationObject_getObjectAddress((InformationObject) io), DoubleCommand_getState((DoubleCommand)io));

            if (dataLog){
                LogRXwoT(InformationObject_getObjectAddress((InformationObject) io), DoubleCommand_getState((DoubleCommand)io));
            }
            if(InformationObject_getObjectAddress((InformationObject) io)==4000 || InformationObject_getObjectAddress((InformationObject) io)==4001){
                receivedCommand=true;
                if (InformationObject_getObjectAddress((InformationObject) io)==4000 && DoubleCommand_getState((DoubleCommand)io)==2){
                    offBreakerRemote=true;
                }else{
                    offBreakerRemote=false;
                }
                if (InformationObject_getObjectAddress((InformationObject) io)==4001 && DoubleCommand_getState((DoubleCommand)io)==1){
                    offDisconRemote=true;
                }else{
                    offDisconRemote=false;
                }
            }

            DoubleCommand_destroy(io);
        }
    }

    else if (CS101_ASDU_getTypeID(asdu) == C_IC_NA_1) { //100
        int i;
        for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {
            InterrogationCommand io = (InterrogationCommand) CS101_ASDU_getElement(asdu,i);
            if (dataLog){
                LogRXC(InformationObject_getObjectAddress((InformationObject) io),InterrogationCommand_getQOI((InterrogationCommand)io));
            }

            if (InterrogationCommand_getQOI((InterrogationCommand)io)==21){
                printf("Command pro qoi %i\n", InterrogationCommand_getQOI((InterrogationCommand)io));
                //sendResponse=true;

            }
            else{
                printf("    IOA: %i qoi %i\n", InformationObject_getObjectAddress((InformationObject) io),InterrogationCommand_getQOI((InterrogationCommand)io));
            }

            InterrogationCommand_destroy(io);
        }
    }

    else if (CS101_ASDU_getTypeID(asdu) == C_CS_NA_1) {  //103
        int i;
        for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {
            ClockSynchronizationCommand io = (ClockSynchronizationCommand) CS101_ASDU_getElement(asdu,i);
            printf("    IOA: %i with time: ", InformationObject_getObjectAddress((InformationObject) io));
            printCP56Time2a(ClockSynchronizationCommand_getTime(io));printf("\n");
            if (dataLog){
                LogRXT(InformationObject_getObjectAddress((InformationObject) io),
                        ClockSynchronizationCommand_getTime((ClockSynchronizationCommand)io));
            }
            ClockSynchronizationCommand_destroy(io);
        }
    }



    return false;
}

static bool
connectionRequestHandler(void* parameter, const char* ipAddress)
{
    printf("New connection request from %s\n", ipAddress);
    LogCONREQ(ipAddress);

#if 0
    if (strcmp(ipAddress, "127.0.0.1") == 0) {
        printf("Accept connection\n");
        return true;
    }
    else {
        printf("Deny connection\n");
        return false;
    }
#else
    return true;
#endif
}

static void
connectionEventHandler(void* parameter, IMasterConnection con, CS104_PeerConnectionEvent event)
{
    if (event == CS104_CON_EVENT_CONNECTION_OPENED) {
        printf("Connection opened (%p)\n", con);
        serverConnected=true;
        LogCONOPEN();
    }
    else if (event == CS104_CON_EVENT_CONNECTION_CLOSED) {
        printf("Connection closed (%p)\n", con);
        serverConnected=false;
        LogCONCLOSED();
    }
    else if (event == CS104_CON_EVENT_ACTIVATED) {
        printf("Connection activated (%p)\n", con);
        LogCONACT();
    }
    else if (event == CS104_CON_EVENT_DEACTIVATED) {
        printf("Connection deactivated (%p)\n", con);
        LogCONDEACT();
    }
}

CS101_ASDU asdu2Send;

CS101_ASDU CreateMONITOR_IN_ASDU(CS101_AppLayerParameters alParams, int cot, CP56Time2a timestamp){

    asdu2Send = CS101_ASDU_create(alParams, false, cot, originatorAddress, commonAddress, false, false);

    for (int i = 0; i < monitor_inSet; ++i) {
        for (int j = 0; j < 4; ++j) {
            InformationObject io = (InformationObject) MeasuredValueShortWithCP56Time2a_create(NULL,monitor_in[i][0]+j,
                    monitor_in_actual[i][j],IEC60870_QUALITY_GOOD,timestamp);
            CS101_ASDU_addInformationObject(asdu2Send, io);
            if (dataLog){
                LogTXwT(InformationObject_getObjectAddress((InformationObject) io),MeasuredValueShort_getValue((MeasuredValueShort) io), timestamp);
            }
            //tmpSharedValue=MeasuredValueShort_getValue((MeasuredValueShort) io);
            printf("    IOA: %i with value: %f \n",monitor_in[i][0]+j,MeasuredValueShort_getValue((MeasuredValueShort)io));

            InformationObject_destroy(io);
        }
    }

    return asdu2Send;

}

CS101_ASDU CreateMONITOR_OUT_ASDU(CS101_AppLayerParameters alParams, int cot, CP56Time2a timestamp){

    asdu2Send = CS101_ASDU_create(alParams, false, cot, originatorAddress, commonAddress, false, false);

    for (int i = 0; i < monitor_outSet; ++i) {
        for (int j = 0; j < 4; ++j) {
            InformationObject io = (InformationObject) MeasuredValueShortWithCP56Time2a_create(NULL,monitor_out[i][0]+j,
                    monitor_out_actual[i][j],IEC60870_QUALITY_GOOD,timestamp);
            CS101_ASDU_addInformationObject(asdu2Send, io);
            if (dataLog){
                LogTXwT(InformationObject_getObjectAddress((InformationObject) io),MeasuredValueShort_getValue((MeasuredValueShort) io), timestamp);
            }
            //tmpSharedValue=MeasuredValueShort_getValue((MeasuredValueShort) io);
            printf("    IOA: %i with value: %f \n",monitor_out[i][0]+j,MeasuredValueShort_getValue((MeasuredValueShort)io));

            InformationObject_destroy(io);
        }
    }

    return asdu2Send;

}


CS101_ASDU CreateTEMPERATURE_ASDU(CS101_AppLayerParameters alParams, int cot, CP56Time2a timestamp){

    asdu2Send = CS101_ASDU_create(alParams, false, cot, originatorAddress, commonAddress, false, false);

    //actual temperature
    InformationObject io = (InformationObject) MeasuredValueShortWithCP56Time2a_create(NULL,(int)temperature[0],temperature[1],IEC60870_QUALITY_GOOD,timestamp);
    CS101_ASDU_addInformationObject(asdu2Send, io);
    if (dataLog){
        LogTXwT(InformationObject_getObjectAddress((InformationObject) io),MeasuredValueShort_getValue((MeasuredValueShort) io), timestamp);
    }
    printf("    IOA: %i with value: %f \n",(int)temperature[0],MeasuredValueShort_getValue((MeasuredValueShort)io));

    //temperature ranges
    for (int i = 0; i < 2; ++i) {
        io = (InformationObject) MeasuredValueShortWithCP56Time2a_create(NULL,(int)temperature[0]+1+i,temperatureRange[i],IEC60870_QUALITY_GOOD,timestamp);

        CS101_ASDU_addInformationObject(asdu2Send, io);
        if (dataLog){
            LogTXwT(InformationObject_getObjectAddress((InformationObject) io),MeasuredValueShort_getValue((MeasuredValueShort) io), timestamp);
        }
        printf("    IOA: %i with value: %f \n",(int)temperature[0]+1+i,MeasuredValueShort_getValue((MeasuredValueShort)io));
    }

    //temperature amb
    io = (InformationObject) MeasuredValueShortWithCP56Time2a_create(NULL,(int)temperature[0]+3,temperature_amb[2],IEC60870_QUALITY_GOOD,timestamp);
    CS101_ASDU_addInformationObject(asdu2Send, io);
    if (dataLog){
        LogTXwT(InformationObject_getObjectAddress((InformationObject) io),MeasuredValueShort_getValue((MeasuredValueShort) io), timestamp);
    }
    printf("    IOA: %i with value: %f \n",(int)temperature[0]+3,MeasuredValueShort_getValue((MeasuredValueShort)io));
    InformationObject_destroy(io);

    return asdu2Send;

}

CS101_ASDU CreateMONITOR_FREQ_ASDU(CS101_AppLayerParameters alParams, int cot, CP56Time2a timestamp){

    //asdu2Send = CS101_ASDU_create(alParams, false, cot, originatorAddress, commonAddress, false, false);

    InformationObject io = (InformationObject) MeasuredValueShortWithCP56Time2a_create(NULL,(int)monitor_freq[0],
            getRandomVal(monitor_freq[1],monitor_freq[2]),IEC60870_QUALITY_GOOD,timestamp);

    CS101_ASDU_addInformationObject(asdu2Send, io);
    if (dataLog){
        LogTXwT(InformationObject_getObjectAddress((InformationObject) io),MeasuredValueShort_getValue((MeasuredValueShort) io), timestamp);
    }
    //tmpSharedValue=MeasuredValueShort_getValue((MeasuredValueShort) io);
    printf("    IOA: %i with value: %f \n",(int)monitor_freq[0],MeasuredValueShort_getValue((MeasuredValueShort)io));

    InformationObject_destroy(io);

    return asdu2Send;

}

CS101_ASDU CreateMONITOR4_ASDU(CS101_AppLayerParameters alParams,int cot, CP56Time2a timestamp){

    asdu2Send = CS101_ASDU_create(alParams, false, cot, originatorAddress, commonAddress, false, false);

    for (int i = 0; i < monitor4Set; ++i) {

        InformationObject io = (InformationObject) DoublePointWithCP24Time2a_create(NULL,monitor4[i][0],monitor4[i][1],
                                                                                    IEC60870_QUALITY_GOOD,(CP24Time2a) timestamp);

        CS101_ASDU_addInformationObject(asdu2Send, io);
        if (dataLog){
            LogTXwT24(InformationObject_getObjectAddress((InformationObject) io),
                      DoublePointInformation_getValue((DoublePointInformation)io),(CP24Time2a) timestamp);
        }
        //tmpSharedValue=DoublePointInformation_getValue((DoublePointInformation) io);
        printf("    IOA: %i with value: %i in time: ",monitor4[i][0],DoublePointInformation_getValue((DoublePointInformation)io));
        printCP24Time2a((CP24Time2a) timestamp);

        InformationObject_destroy(io);
    }
    return asdu2Send;

}

CS101_ASDU CreateMONITOR2_ASDU(CS101_AppLayerParameters alParams,int cot, CP56Time2a timestamp){

    asdu2Send = CS101_ASDU_create(alParams, false, cot, originatorAddress, commonAddress, false, false);
    bool tmpBool=0;
    for (int i = 0; i < monitor2Set; ++i) {

        if(monitor2[i][1]==1){
            tmpBool=true;
        } else{
            tmpBool=false;
        }

        InformationObject io = (InformationObject) SinglePointWithCP24Time2a_create(NULL,monitor2[i][0],
                tmpBool,IEC60870_QUALITY_GOOD,(CP24Time2a) timestamp);

        CS101_ASDU_addInformationObject(asdu2Send, io);
        if (dataLog){
            LogTXwT24(InformationObject_getObjectAddress((InformationObject) io),SinglePointInformation_getValue((SinglePointInformation)io),(CP24Time2a) timestamp);
        }
        //tmpSharedValue=SinglePointInformation_getValue((SinglePointInformation) io);
        printf("    IOA: %i with value: %i in time: ",monitor2[i][0],SinglePointInformation_getValue((SinglePointInformation)io));
        printCP24Time2a((CP24Time2a) timestamp);

        InformationObject_destroy(io);
    }
    return asdu2Send;

}
CS101_ASDU CreateEvent4_ASDU(CS101_AppLayerParameters alParams, int ioa,int value, CP56Time2a timestamp){

    asdu2Send = CS101_ASDU_create(alParams, false, 3, originatorAddress, commonAddress, false, false);

    InformationObject io = (InformationObject) DoublePointWithCP24Time2a_create(NULL,ioa,value,IEC60870_QUALITY_GOOD,(CP24Time2a) timestamp);
    CS101_ASDU_addInformationObject(asdu2Send, io);
    if (dataLog){
        LogTXwT24(InformationObject_getObjectAddress((InformationObject) io),DoublePointInformation_getValue((DoublePointInformation)io),(CP24Time2a) timestamp);
    }
    printf("    IOA: %i with value: %i \n",ioa,DoublePointInformation_getValue((DoublePointInformation)io));

    InformationObject_destroy(io);

    return asdu2Send;

}

CS101_ASDU CreateEvent2_ASDU(CS101_AppLayerParameters alParams, int ioa, bool value, CP56Time2a timestamp){

    asdu2Send = CS101_ASDU_create(alParams, false, 3, originatorAddress, commonAddress, false, false);
    InformationObject io = (InformationObject) SinglePointWithCP24Time2a_create(NULL,ioa,value,IEC60870_QUALITY_GOOD,(CP24Time2a) timestamp);
    CS101_ASDU_addInformationObject(asdu2Send, io);
    if (dataLog){
        LogTXwT24(InformationObject_getObjectAddress((InformationObject) io),SinglePointInformation_getValue((SinglePointInformation)io),(CP24Time2a) timestamp);
    }
    printf("    IOA: %i with value: %i \n",ioa,SinglePointInformation_getValue((SinglePointInformation)io));

    InformationObject_destroy(io);

    return asdu2Send;

}



int
main(int argc, char** argv)
{
    //chanPort = atoi(argv[2]);         //pro spusteni

    /*angleCos=cosf(0.951); //phi=18°
    angleSin=sinf(0.309);

    double ucinik = cos(power_factor*M_PI/180);

    printf("uhel jeeeeeeeeeeeeeeeeeeeeee %f ucinik je %f\n",power_factor,ucinik);*/


    LogSTART();
    getStatus();
    nacteniMC(INparameter);             //nacteniMC(chanPort); pro spusteni

    /*                                  pro spusteni
    //printf("Parameter1: %s\n",argv[1]);
    //printf("Paremater2: %s\n",argv[2]);
    for (int j = 0; j < strlen(argv[1]); j++) { //-2 ->-1
        str_IP[j]=argv[1][j];
        str_OA[j]=argv[1][j];
    }
    str_IP[strlen(argv[1])+1]='\0';
    str_OA[strlen(argv[1])+1]='\0';*/

    char* pch=str_OA;
    pch = strtok (pch,".");
    pch = strtok(NULL, ".");
    pch = strtok(NULL, ".");
    pch = strtok(NULL, ".");
    originatorAddress=atoi(pch);

    snprintf(strPort, sizeof(strPort), "%d", chanPort);

    strcpy(matrixPath,"TXmatrix");
    strcat(matrixPath,strPort);
    strcat(matrixPath,".txt");

    strcpy(logTXPath,"LogTX");
    strcat(logTXPath,strPort);
    strcat(logTXPath,".txt");

    strcpy(logRXPath,"LogRX");
    strcat(logRXPath,strPort);
    strcat(logRXPath,".txt");


    printf("IPADDRESS: %s\n",str_IP);
    printf("PORT: %i\n",chanPort);
    printf("ORIGINATOR_ADDRESS: %i\n",originatorAddress);
    printf("COMMON_ADDRESS: %i\n",commonAddress);
    printf("DATA_LOGS: %i\n",dataLog);

    printf("PER_RESPONSE_time: %i\n",periodicTime);
    printf("ALARM_PERIOD_time: %i\n",alarmTime);
    printf("FAILURE_EFFECT: %i\n",failureEffect);
    printf("BREAKER_DELAY: %i\n",breaker_delay);

    printf("INPUT_POWER_FACTOR: phi=%f cos(phi)=%f sin(phi)=%f \n",input_power_factor,cos(input_power_factor*M_PI/180),sin(input_power_factor*M_PI/180));
    printf("TRANSFORMER_POWER_FACTOR: phi=%f cos(phi)=%f sin(phi)=%f \n",transformer_power_factor,cos(transformer_power_factor*M_PI/180),sin(transformer_power_factor*M_PI/180));

    printf("IN: %i\n",monitor_inUsed);
    printf("MONITOR_IN_set: %i\n",monitor_inSet);
    printf("MONITOR_IN:%i;%i-%i;%i-%i %i;%i-%i;%i-%i %i;%i-%i;%i-%i\n",monitor_in[0][0],monitor_in[0][1],monitor_in[0][2],monitor_in[0][3],monitor_in[0][4],
            monitor_in[1][0],monitor_in[1][1],monitor_in[1][2],monitor_in[1][3],monitor_in[1][4],monitor_in[2][0],monitor_in[2][1],
            monitor_in[2][2],monitor_in[2][3],monitor_in[2][4]);

    printf("OUT: %i\n",monitor_outUsed);
    printf("MONITOR_OUT_set: %i\n",monitor_outSet);
    printf("MONITOR_OUT:%i;%i-%i, %i;%i-%i, %i;%i-%i\n",monitor_out[0][0],monitor_out[0][1],monitor_out[0][2],
           monitor_out[1][0],monitor_out[1][1],monitor_out[1][2],monitor_out[2][0],monitor_out[2][1],
           monitor_out[2][2]);

    printf("TEMPERATURE: %i\n",tempFreqeUsed);
    printf("TEMPERATURE_object:%f;%f;%f-%f;%f-%f\n",temperature[0],temperature[1],temperatureRange[0],temperatureRange[1],temperature_amb[0],temperature_amb[1]);
    printf("MONITOR_FREQ:%f;%f-%f\n",monitor_freq[0],monitor_freq[1],monitor_freq[2]);

    printf("4: %i\n",monitor4Used);
    printf("MONITOR4:%i;%i %i;%i .... %i;%i\n",monitor4[0][0],monitor4[0][1],monitor4[1][0],
           monitor4[1][1],monitor4[2][0],monitor4[2][1]);

    printf("2: %i\n",monitor2Used);
    printf("MONITOR2:%i;%i %i;%i .... %i;%i\n",monitor2[0][0],monitor2[0][1],monitor2[1][0],
           monitor2[1][1],monitor2[2][0],monitor2[2][1]);

    printf("\n");


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
    CS104_Slave_setASDUHandler(slave, asduHandler, NULL);

    /* set comunication port */
    CS104_Slave_setLocalPort(slave,chanPort);
    //CS104_Slave_createSecure(1,255,TLSConfiguration_create());

    /* set handler to handle connection requests (optional) */
    CS104_Slave_setConnectionRequestHandler(slave, connectionRequestHandler, NULL);

    /* set handler to track connection events (optional) */
    CS104_Slave_setConnectionEventHandler(slave, connectionEventHandler, NULL);

    /* uncomment to log messages */
    //CS104_Slave_setRawMessageHandler(slave, rawMessageHandler, NULL);

    CS104_Slave_start(slave);

    if (CS104_Slave_isRunning(slave) == false) {
        printf("Starting server failed!\n");
        LogSETUP_F();
        goto exit_program;

    } else{
        printf("Server running...\n");
        LogSETUP_G();

    }

    int seconds;
    int time_tmp=-1;
    int time_now;
    int time_monitorTimer=0;
    FILE *fp;

    int dayInMinutes=-1;

    while (running) {
        seconds = time(NULL)%60;
        time_now=seconds;

        //every second do
        if(time_now!=time_tmp){
            time_monitorTimer+=1;
            time_tmp=time_now;

            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            //printf("Time now: %02d:%02d:%02d \n", tm.tm_hour, tm.tm_min, tm.tm_sec);
            dayInMinutes = (tm.tm_hour * 60) + tm.tm_min;
            printf("Time now: %i \n",dayInMinutes);
            Thread_sleep(10);
            getTraficLoad(dayInMinutes);
            printf("act_load je: %f \n", act_load);


            getStatus();
            printf("Status: %i %i %i %i %i\n",generating[0],generating[1],generating[2],generating[3],generating[4]);
            if(generating[(chanPort-2404)]==1 && reloadedVal==false && receivedCommand==false ){
                alarm=0;
                undervoltage=0;
                overvoltage=0;
                monitor_inSet=0;
                monitor_outSet=0;
                monitor4Set=0;
                monitor2Set=0;
                matrixRow=0;
                nacteniMC(INparameter);         //nacteniMC(chanPort); pro spusteni
                reloadedVal=true;
                shortCircuit=false;
                underOverVoltage=false;
                offBreaker=false;
                offDiscon=false;
                sendRecovery=true;
                offBreakerRemote=false;
                offBreakerRemoteDone=false;
                offDisconRemote=false;
                offDisconRemoteDone=false;

            }

            //podpeti
            if(generating[(chanPort-2404)]==0 && undervoltage<breaker_delay){
                for (int i = 0; i < monitor_inSet; ++i) {
                    for (int j = 1; j < 5; ++j) {
                        monitor_in[i][j]-=failureEffect;
                    }
                }
                for (int i = 0; i < monitor_outSet; ++i) {              //nebude potreba
                    for (int j = 1; j < 5; ++j) {
                        monitor_out[i][j]-=failureEffect;
                    }
                }
                undervoltage+=1;
                reloadedVal=false;
            }
            //prepeti
            else if(generating[(chanPort-2404)]==2 && overvoltage<breaker_delay){
                for (int i = 0; i < monitor_inSet; ++i) {
                    for (int j = 1; j < 5; ++j) {
                        monitor_in[i][j]+=failureEffect;
                    }
                }
                for (int i = 0; i < monitor_outSet; ++i) {              //nebude potreba
                    for (int j = 1; j < 5; ++j) {
                        monitor_out[i][j]+=failureEffect;
                    }
                }
                overvoltage+=1;
                reloadedVal=false;
            }
            //zkrat
            else if(generating[(chanPort-2404)]==3 && shortCircuit==false){
                for (int i = 0; i < monitor_inSet; ++i) {
                    for (int j = 1; j < 5; ++j) {
                        monitor_in[i][j]=0;
                    }
                }
                for (int i = 0; i < monitor_outSet; ++i) {              //nebude potreba
                    for (int j = 1; j < 5; ++j) {
                        monitor_out[i][j]=0;
                    }
                }
                //5002 5000 4000 4004 + 5003
                monitor2[2][1]=1;monitor2[0][1]=1;monitor4[0][1]=2;monitor4[4][1]=2;monitor2[3][1]=1;

                CS101_ASDU newAsdu = CreateEvent2_ASDU(alParams,monitor2[2][0],(bool)monitor2[2][1],CP56Time2a_createFromMsTimestamp(NULL, Hal_getTimeInMs()));
                CS104_Slave_enqueueASDU(slave, newAsdu);
                newAsdu = CreateEvent2_ASDU(alParams,monitor2[0][0],(bool)monitor2[0][1],CP56Time2a_createFromMsTimestamp(NULL, Hal_getTimeInMs()));
                CS104_Slave_enqueueASDU(slave, newAsdu);
                newAsdu = CreateEvent4_ASDU(alParams,monitor4[0][0],monitor4[0][1],CP56Time2a_createFromMsTimestamp(NULL, Hal_getTimeInMs()));
                CS104_Slave_enqueueASDU(slave, newAsdu);
                newAsdu = CreateEvent4_ASDU(alParams,monitor4[4][0],monitor4[4][1],CP56Time2a_createFromMsTimestamp(NULL, Hal_getTimeInMs()));
                CS104_Slave_enqueueASDU(slave, newAsdu);
                newAsdu = CreateEvent2_ASDU(alParams,monitor2[3][0],(bool)monitor2[3][1],CP56Time2a_createFromMsTimestamp(NULL, Hal_getTimeInMs()));
                CS104_Slave_enqueueASDU(slave, newAsdu);
                CS101_ASDU_destroy(newAsdu);

                reloadedVal=false;
                shortCircuit=true;
            }
            //offHJ
            else if((generating[(chanPort-2404)]==4 && offBreaker==false) || (offBreakerRemote==true && offBreakerRemoteDone==false)){
                //5000 4000
                monitor2[0][1]=1;monitor4[0][1]=2;

                CS101_ASDU newAsdu = CreateEvent2_ASDU(alParams,monitor2[0][0],(bool)monitor2[0][1],CP56Time2a_createFromMsTimestamp(NULL, Hal_getTimeInMs()));
                CS104_Slave_enqueueASDU(slave, newAsdu);
                newAsdu = CreateEvent4_ASDU(alParams,monitor4[0][0],monitor4[0][1],CP56Time2a_createFromMsTimestamp(NULL, Hal_getTimeInMs()));
                CS104_Slave_enqueueASDU(slave, newAsdu);

                if (generating[(chanPort-2404)]==4 && offBreaker==false){
                    offBreaker=true;
                }
                if (offBreakerRemote==true && offBreakerRemoteDone==false){
                    offBreakerRemoteDone=true;
                }
                reloadedVal=false;

            }
            //offOD
            else if((generating[(chanPort-2404)]==5 && offDiscon==false) || (offDisconRemote==true && offDisconRemoteDone==false)){
                //5001 4001
                monitor2[1][1]=1;monitor4[1][1]=1;

                CS101_ASDU newAsdu = CreateEvent2_ASDU(alParams,monitor2[1][0],(bool)monitor2[1][1],CP56Time2a_createFromMsTimestamp(NULL, Hal_getTimeInMs()));
                CS104_Slave_enqueueASDU(slave, newAsdu);
                newAsdu = CreateEvent4_ASDU(alParams,monitor4[1][0],monitor4[1][1],CP56Time2a_createFromMsTimestamp(NULL, Hal_getTimeInMs()));
                CS104_Slave_enqueueASDU(slave, newAsdu);

                if (generating[(chanPort-2404)]==5 && offDiscon==false){
                    offDiscon=true;
                }

                if (offDisconRemote==true && offDisconRemoteDone==false){
                    offDisconRemoteDone=true;
                }
                reloadedVal=false;

            }


            if(offDisconRemote==false && receivedCommand==true && offDisconRemoteDone==true){
                receivedCommand=false;
            }

            if(offBreakerRemote==false && receivedCommand==true && offBreakerRemoteDone==true){
                receivedCommand=false;
            }


            if ((undervoltage>=breaker_delay || overvoltage>=breaker_delay) && underOverVoltage==false){
                //(5002?) 5000->1 4000->2 (4002/4003)->2 + 5003->1
                monitor2[0][1]=1;monitor4[0][1]=2;monitor2[3][1]=1;
                if (undervoltage>=breaker_delay){
                    monitor4[3][1]=2;
                } else{
                    monitor4[2][1]=2;
                }

                CS101_ASDU newAsdu = CreateEvent2_ASDU(alParams,monitor2[0][0],(bool)monitor2[0][1],CP56Time2a_createFromMsTimestamp(NULL, Hal_getTimeInMs()));
                CS104_Slave_enqueueASDU(slave, newAsdu);
                newAsdu = CreateEvent4_ASDU(alParams,monitor4[0][0],monitor4[0][1],CP56Time2a_createFromMsTimestamp(NULL, Hal_getTimeInMs()));
                CS104_Slave_enqueueASDU(slave, newAsdu);
                if (undervoltage>=breaker_delay){
                    newAsdu = CreateEvent4_ASDU(alParams,monitor4[3][0],monitor4[3][1],CP56Time2a_createFromMsTimestamp(NULL, Hal_getTimeInMs()));
                    CS104_Slave_enqueueASDU(slave, newAsdu);
                } else{
                    newAsdu = CreateEvent4_ASDU(alParams,monitor4[2][0],monitor4[2][1],CP56Time2a_createFromMsTimestamp(NULL, Hal_getTimeInMs()));
                    CS104_Slave_enqueueASDU(slave, newAsdu);
                }
                newAsdu = CreateEvent2_ASDU(alParams,monitor2[3][0],(bool)monitor2[3][1],CP56Time2a_createFromMsTimestamp(NULL, Hal_getTimeInMs()));
                CS104_Slave_enqueueASDU(slave, newAsdu);
                CS101_ASDU_destroy(newAsdu);

                underOverVoltage=true;
            }

            if(underOverVoltage==true || shortCircuit==true){
                alarm+=1;
            }

            if (monitor4[0][1]==2 || monitor4[1][1]==1){
                for (int i = 0; i < monitor_inSet; ++i) {
                    for (int j = 1; j < 5; ++j) {
                        monitor_in[i][j]=0;
                    }
                }
                for (int i = 0; i < monitor_outSet; ++i) {              //nebude potreba
                    for (int j = 1; j < 5; ++j) {
                        monitor_out[i][j]=0;
                    }
                }
                monitor_freq[1]=0;
                monitor_freq[2]=0;
            }

            sharedMatrixPER_clr();
            matrixRow=0;

            sharedMatrixPER[matrixRow][0]=0;
            sharedMatrixPER[matrixRow][1]=serverConnected;
            //sharedMatrixPER[matrixRow][2]=generating[(chanPort-2404)];
            if (generating[(chanPort-2404)]==0 || monitor4[0][1]==2 || monitor4[1][1]==1){
                sharedMatrixPER[matrixRow][2]=0;
            }else{
                sharedMatrixPER[matrixRow][2]=1;
            }
            matrixRow+=1;


            //generovani
            for (int j = 0; j < monitor_inSet; ++j) {                                               //P=U_in *(I_in*load) z textaku => 440*I*act_load
                monitor_in_actual[j][0]=getRandomVal(monitor_in[j][1],monitor_in[j][2]);            //U_in
                monitor_in_actual[j][1]=getRandomVal(monitor_in[j][3],monitor_in[j][4])*act_load;   //I_in =I*act_load
                monitor_in_actual[j][2]=monitor_in_actual[j][0]*monitor_in_actual[j][1]*(float)cos(input_power_factor*M_PI/180);   //P_in =U*I*cos(alfa) alfa=10°
                monitor_in_actual[j][3]=monitor_in_actual[j][0]*monitor_in_actual[j][1]*(float)sin(input_power_factor*M_PI/180);   //Q_in =U*I*sin(alfa) alfa=10°
                printf("%f-%f-%f-%f\n", monitor_in_actual[j][0], monitor_in_actual[j][1], monitor_in_actual[j][2], monitor_in_actual[j][3]);

                sharedMatrixPER[matrixRow][0]=36;
                sharedMatrixPER[matrixRow][1]=monitor_in[j][0];
                sharedMatrixPER[matrixRow][2]=monitor_in_actual[j][0];
                matrixRow+=1;
            }

            for (int j = 0; j < monitor_outSet; ++j) {  //P=U_out * I_out * cos(phi) uz nebude z textaku =>
                monitor_out_actual[j][0]=getRandomVal(monitor_out[j][1],monitor_out[j][2]);                         //U_out
                if (monitor_out_actual[j][0]==0){
                    monitor_out_actual[j][1]=0;                                                                     //I_out = 0
                } else{
                    monitor_out_actual[j][1]=(monitor_in_actual[j][0]*monitor_in_actual[j][1])/monitor_out_actual[j][0];//I_out = (U_in*I_in)/U_out
                }
                monitor_out_actual[j][2]=monitor_out_actual[j][0]*monitor_out_actual[j][1]*(float)cos(transformer_power_factor*M_PI/180);                //P=U*I*cos(alfa)
                monitor_out_actual[j][3]=monitor_out_actual[j][0]*monitor_out_actual[j][1]*(float)sin(transformer_power_factor*M_PI/180);                //Q=U*I*sin(alfa)
                printf("%f-%f-%f-%f\n", monitor_out_actual[j][0], monitor_out_actual[j][1], monitor_out_actual[j][2], monitor_out_actual[j][3]);

                sharedMatrixPER[matrixRow][0]=36;
                sharedMatrixPER[matrixRow][1]=monitor_out[j][0];
                sharedMatrixPER[matrixRow][2]=monitor_out_actual[j][0];
                matrixRow+=1;
            }
            temperature[1]=getTemperature(temperature[1]);
            temperature_amb[2]=getTemperatureAmb();
            //printf("%f-%f-%f-%f-%f-%f\n",temperature[1],temperatureRange[0],temperatureRange[1],temperature_amb[0],temperature_amb[1],temperature_amb[2]);

            sharedMatrixPER[matrixRow][0]=36;
            sharedMatrixPER[matrixRow][1]=temperature[0];
            sharedMatrixPER[matrixRow][2]=temperature[1];
            matrixRow+=1;

            fp = fopen (matrixPath, "w");
            printf("Monitor matrix \n");
            for (int l = 0; l < matrixRow; ++l) {//10->32
                printf("   [%f %f %f]\n", sharedMatrixPER[l][0],sharedMatrixPER[l][1],sharedMatrixPER[l][2]);
                fprintf(fp,"%f %f %f\n", sharedMatrixPER[l][0],sharedMatrixPER[l][1],sharedMatrixPER[l][2]);
            }
            fclose(fp);




        }
        Thread_sleep(1);

        //in response time
        if(time_monitorTimer>=periodicTime || alarm>=alarmTime || sendRecovery==true){

            if (alarm>=alarmTime || sendRecovery==true){
                alarm=0;
                sendRecovery=false;
                cot=3;
            }else{
                cot=1;
            }

            //in values
            if(monitor_inUsed==1){
                printf("SENDED ASDU type: 36 elements: %i \n", monitor_inSet*4);
                if (dataLog){
                    LogTX(36,monitor_inSet*4);
                }
                CS101_ASDU newAsdu = CreateMONITOR_IN_ASDU(alParams, cot,CP56Time2a_createFromMsTimestamp(NULL, Hal_getTimeInMs()));
                CS104_Slave_enqueueASDU(slave, newAsdu);
                CS101_ASDU_destroy(newAsdu);
            }
            //out values
            if(monitor_outUsed==1){
                printf("SENDED ASDU type: 36 elements: %i \n", monitor_outSet*4);
                if (dataLog){
                    LogTX(36,monitor_outSet*4);
                }
                CS101_ASDU newAsdu = CreateMONITOR_OUT_ASDU(alParams, cot,CP56Time2a_createFromMsTimestamp(NULL, Hal_getTimeInMs()));
                CS104_Slave_enqueueASDU(slave, newAsdu);
                CS101_ASDU_destroy(newAsdu);
            }
            //temperature+frequency
            if(tempFreqeUsed==1){
                printf("SENDED ASDU type: 36 elements: %i \n", 5);
                if (dataLog){
                    LogTX(36,5);
                }
                CS101_ASDU newAsdu = CreateTEMPERATURE_ASDU(alParams, cot,CP56Time2a_createFromMsTimestamp(NULL, Hal_getTimeInMs()));
                //CS104_Slave_enqueueASDU(slave, newAsdu);
                newAsdu = CreateMONITOR_FREQ_ASDU(alParams, 1,CP56Time2a_createFromMsTimestamp(NULL, Hal_getTimeInMs()));
                CS104_Slave_enqueueASDU(slave, newAsdu);
                CS101_ASDU_destroy(newAsdu);
            }

            //double values
            if(monitor4Used==1){
                printf("SENDED ASDU type: 4 elements: %i \n", monitor4Set);
                if (dataLog){
                    LogTX(4,monitor4Set);
                }
                CS101_ASDU newAsdu = CreateMONITOR4_ASDU(alParams,cot,CP56Time2a_createFromMsTimestamp(NULL, Hal_getTimeInMs()));
                CS104_Slave_enqueueASDU(slave, newAsdu);
                CS101_ASDU_destroy(newAsdu);
            }
            //single values
            if(monitor2Used==1){
                printf("SENDED ASDU type: 2 elements: %i \n", monitor2Set);
                if (dataLog){
                    LogTX(2,monitor2Set);
                }
                CS101_ASDU newAsdu = CreateMONITOR2_ASDU(alParams,cot,CP56Time2a_createFromMsTimestamp(NULL, Hal_getTimeInMs()));
                CS104_Slave_enqueueASDU(slave, newAsdu);
                CS101_ASDU_destroy(newAsdu);
            }
            time_monitorTimer=0;

        }
    }
    CS104_Slave_stop(slave);
    exit_program:
    CS104_Slave_destroy(slave);
    Thread_sleep(2000);
}

