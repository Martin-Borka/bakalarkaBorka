#include "cs104_connection.h"
#include "hal_time.h"
#include "hal_thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

uint16_t port = IEC_60870_5_104_DEFAULT_PORT;


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
rawMessageHandler (void* parameter, uint8_t* msg, int msgSize, bool sent)
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

//event logs
void LogSTART(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen ("EventLog.txt", "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d Client started \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
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

void LogCONREQ(const char* ipAddress){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen ("EventLog.txt", "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d New connection request from %s: \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,ipAddress);
    fclose(fp);
}

void LogCONEST(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen ("EventLog.txt", "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d Connection established\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
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
void LogCONREF(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen ("EventLog.txt", "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d Connections refreshed \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fclose(fp);
}
void LogCONSTARTTD(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen ("EventLog.txt", "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d Received START_CON \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fclose(fp);
}
void LogCONSTOPTD(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen ("EventLog.txt", "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d Received STOPDT_CON \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fclose(fp);
}

//data logs

void LogRX(int type,int elements){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen ("LogRX.txt", "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d type(%i) elements: %i\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,type,elements);
    fclose(fp);
}

void LogRXwoT(int ioa,float value){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen ("LogRX.txt", "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d IOA: %i value: %f\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
            tm.tm_sec,ioa,value);
    fclose(fp);
}


void LogRXwT(int ioa,float value,CP56Time2a timestamp ){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen ("LogRX.txt", "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d IOA: %i value: %f with timestamp: %04i-%02i-%02i %02i:%02i:%02i.%03i \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
            tm.tm_sec,ioa,value,CP56Time2a_getYear(timestamp) + 2000 ,CP56Time2a_getMonth(timestamp),CP56Time2a_getDayOfMonth(timestamp),
            CP56Time2a_getHour(timestamp), CP56Time2a_getMinute(timestamp), CP56Time2a_getSecond(timestamp), CP56Time2a_getMillisecond(timestamp));
    fclose(fp);
}

void LogRXwT24(int ioa,float value,CP24Time2a timestamp ){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen ("LogRX.txt", "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d IOA: %i value: %f with timestamp: %02i:%02i.%03i\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
            tm.tm_sec,ioa,value,CP24Time2a_getMinute(timestamp), CP24Time2a_getSecond(timestamp), CP24Time2a_getMillisecond(timestamp));
    fclose(fp);
}



void LogTX(int type,int elements){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen ("LogTX.txt", "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d type(%i) elements: %i\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,type,elements);
    fclose(fp);
}

void LogTXwoT(int ioa,float value){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen ("LogTX.txt", "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d IOA: %i value: %f\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
            tm.tm_sec,ioa,value);
    fclose(fp);
}
void LogTXwT(int ioa,float value,CP56Time2a timestamp ){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE * fp;
    fp = fopen ("LogTX.txt", "a");
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d IOA: %i value: %f with timestamp: %04i-%02i-%02i %02i:%02i:%02i.%03i \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
            tm.tm_sec,ioa,value,CP56Time2a_getYear(timestamp) + 2000 ,CP56Time2a_getMonth(timestamp),CP56Time2a_getDayOfMonth(timestamp),
            CP56Time2a_getHour(timestamp), CP56Time2a_getMinute(timestamp), CP56Time2a_getSecond(timestamp), CP56Time2a_getMillisecond(timestamp));
    fclose(fp);
}

/* Connection event handler */
static void
connectionHandler (void* parameter, CS104_Connection connection, CS104_ConnectionEvent event)
{
    switch (event) {
    case CS104_CONNECTION_OPENED:
        printf("Connection established\n");
        LogCONEST();
        break;
    case CS104_CONNECTION_CLOSED:
        printf("Connection closed\n");
        LogCONCLOSED();
        break;
    case CS104_CONNECTION_STARTDT_CON_RECEIVED:
        printf("Received STARTDT_CON\n");
        LogCONSTARTTD();
        break;
    case CS104_CONNECTION_STOPDT_CON_RECEIVED:
        printf("Received STOPDT_CON\n");
        LogCONSTOPTD();
        break;
    }
}

/*
 * CS101_ASDUReceivedHandler implementation
 *
 * For CS104 the address parameter has to be ignored
 */


//float sharedMatrix[32][3];
//int ps=-1;

char serversIp[127][256];
int serversSet=0;
bool dataLog=false;
uint16_t chanPort = 2404;

bool req45Used=false;
int req45Objects[127][2];
int req45Time=0;
int req45Set=0;


bool req46Used=false;
int req46Objects[127][2];
int req46Time=0;
int req46Set=0;

void nacteniMC(){

    printf("done \n");
    FILE* fp;
    fp = fopen("MainConfig_client.txt", "r");
    char * line = NULL;
    size_t len = 0;

    while (getline(&line, &len, fp) != -1) {
        char* pch;
        pch = strtok (line,"=, ");
        while (pch != NULL){

            if(!(strcmp(pch,"SERVERS_IP"))){
                pch = strtok (NULL, "=, ");
                int i=0;
                while (pch != NULL){
                    if(strlen(pch)>6){
                        for (int j = 0; j < strlen(pch); j++) {
                            serversIp[i][j]=pch[j];
                        }
                        serversIp[i][strlen(pch)+1]='\0';
                        serversSet+=1;
                    }
                    i+=1;
                    pch = strtok (NULL, "=, ");
                }
            }

            else if(!(strcmp(pch,"DATA_LOGS"))){
                pch = strtok (NULL, "=, ");
                if (atoi(pch)==1){
                    dataLog=true;
                }
            }
            //45
            /*
            else if(!(strcmp(pch,"REQUEST45"))){
                pch = strtok (NULL, "=, ");
                if (atoi(pch)==1){
                    req45Used=true;
                }
            }
            else if(!(strcmp(pch,"REQUEST45_time"))){
                pch = strtok (NULL, "=, ");
                req45Time=atoi(pch);
            }


            else if(!(strcmp(pch,"REQUEST45_objects"))){
                pch = strtok (NULL, "=, ;");
                int i=0;
                while (pch != NULL){
                    req45Objects[i][0]=atoi(pch);
                    pch = strtok (NULL, "=, ;");
                    req45Objects[i][1]=atoi(pch);
                    pch = strtok (NULL, "=, ;");
                    i+=1;
                    req45Set+=1;
                }
            }

            //46
            else if(!(strcmp(pch,"REQUEST46"))){
                pch = strtok (NULL, "=, ");
                if (atoi(pch)==1){
                    req46Used=true;
                }
            }
            else if(!(strcmp(pch,"REQUEST46_time"))){
                pch = strtok (NULL, "=, ");
                req46Time=atoi(pch);
            }


            else if(!(strcmp(pch,"REQUEST46_objects"))){
                pch = strtok (NULL, "=, ;");
                //printf("pch2: %s \n",pch);
                int i=0;
                while (pch != NULL){
                    req46Objects[i][0]=atoi(pch);
                    pch = strtok (NULL, "=, ;");
                    req46Objects[i][1]=atoi(pch);
                    pch = strtok (NULL, "=, ;");
                    i+=1;
                    req46Set+=1;
                }
            }*/
            pch = strtok (NULL, "=, ");
        }
    }
    fclose(fp);

}

//bool cmdRowUsed=0;
int cmdRow[127][3];
int cmdRowSet=0;

void commandCheck(){
    printf("CHECK\n");
    for (int j = 0; j < cmdRowSet; ++j) {
        cmdRow[j][0]=0;
        cmdRow[j][1]=0;
        cmdRow[j][2]=0;

    }
    cmdRowSet=0;

    printf(" pred: -%i- -%i- -%i-\n", cmdRow[0][0],cmdRow[0][1],cmdRow[0][2]);
    printf(" pred: -%i- -%i- -%i-\n", cmdRow[1][0],cmdRow[1][1],cmdRow[1][2]);
    printf(" pred: -%i- -%i- -%i-\n", cmdRow[2][0],cmdRow[2][1],cmdRow[2][2]);
    printf("set je: %i\n",cmdRowSet);


    FILE* fp;
    fp = fopen("CommandBuffer.txt", "r+");
    char * line = NULL;
    size_t len = 0;
    int i=0;
    while (getline(&line, &len, fp) != -1) {
        char* pch;
        pch = strtok (line,"=, ;-");
        cmdRow[i][0]=atoi(pch);
        //printf("pch je: -%s- \n", pch);

        pch = strtok(NULL, "=, ;-");
        cmdRow[i][1]=atoi(pch);
        //printf("pch2 je: -%s- \n", pch);

        pch = strtok(NULL, "=, ;-");
        cmdRow[i][2]=atoi(pch);
        //printf("pch3 je: -%s- \n", pch);

        if (cmdRow[i][0]!=0 || cmdRow[i][1]!=0){
            printf("row je: -%i- -%i- -%i-\n", cmdRow[i][0],cmdRow[i][1],cmdRow[i][2]);
            cmdRowSet+=1;
            i+=1;

        }else{
            printf("Invalid Command\n");
            cmdRow[i][0]=0;
            cmdRow[i][1]=0;
            cmdRow[i][2]=0;
            //i-=1;
        }



    }
    printf("Konec cteni\n");
    printf("row: -%i- -%i- -%i-\n", cmdRow[0][0],cmdRow[0][1],cmdRow[0][2]);
    printf("row: -%i- -%i- -%i-\n", cmdRow[1][0],cmdRow[1][1],cmdRow[1][2]);
    printf("row: -%i- -%i- -%i-\n", cmdRow[2][0],cmdRow[2][1],cmdRow[2][2]);
    printf("set je: %i\n",cmdRowSet);

    fclose(fp);

    FILE* f;
    f = fopen("CommandBuffer.txt", "w");
    fprintf (f, "0 0 0 \n");
    fclose(f);

}


void SendREQCommand(CS104_Connection con,CS101_AppLayerParameters alParams,int cot,int objNum, int ioa , int value){

    CS101_ASDU REQAsdu = CS101_ASDU_create(alParams, 0, cot, 9876, 1 , false, false);
    if (dataLog){
        LogTX(objNum,1);
    }
    bool tmpBool=0;

    if(objNum==45){
        if(value==1){
            tmpBool=true;
        } else{
            tmpBool=false;
        }

        InformationObject io = (InformationObject) SingleCommand_create(NULL,ioa,tmpBool,1,0);
        CS101_ASDU_addInformationObject(REQAsdu, io);
        CS104_Connection_sendASDU(con,REQAsdu);
        printf("SENDED Command type: %i with ioa: %i and value: %i\n",objNum,ioa,SingleCommand_getState((SingleCommand)io));
        if (dataLog){
            LogTXwoT(ioa,SingleCommand_getState((SingleCommand) io));
        }
        InformationObject_destroy(io);
        CS101_ASDU_destroy(REQAsdu);
    }

    if(objNum==46){
        InformationObject io = (InformationObject) DoubleCommand_create(NULL,ioa,value,1,0);
        CS101_ASDU_addInformationObject(REQAsdu, io);
        CS104_Connection_sendASDU(con,REQAsdu);
        printf("SENDED Command type: %i with ioa: %i and value: %i\n",objNum,ioa,DoubleCommand_getState((DoubleCommand)io));
        if (dataLog){
            LogTXwoT(ioa,DoubleCommand_getState((DoubleCommand) io));
        }
        InformationObject_destroy(io);
        CS101_ASDU_destroy(REQAsdu);
    }
}



static bool
asduReceivedHandler (void* parameter, int address, CS101_ASDU asdu)
{
    printf("RECVD ASDU type: %s(%i) elements: %i \n",
            TypeID_toString(CS101_ASDU_getTypeID(asdu)),
            CS101_ASDU_getTypeID(asdu),
            CS101_ASDU_getNumberOfElements(asdu));

    if (dataLog && CS101_ASDU_getTypeID(asdu)<45){
        LogRX(CS101_ASDU_getTypeID(asdu),CS101_ASDU_getNumberOfElements(asdu));
    }
/*
    if (CS101_ASDU_getTypeID(asdu)<45){
        ps+=1;
        sharedMatrix[ps][0]=CS101_ASDU_getTypeID(asdu);
        sharedMatrix[ps][1]=CS101_ASDU_getNumberOfElements(asdu);
    }*/

    if (CS101_ASDU_getTypeID(asdu) == M_SP_TA_1) { //2
        int i;
        for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {
            SinglePointWithCP24Time2a io = (SinglePointWithCP24Time2a) CS101_ASDU_getElement(asdu, i);
            printf("    IOA: %i value: %i in time: ",
                    InformationObject_getObjectAddress((InformationObject) io),
                    SinglePointInformation_getValue((SinglePointInformation) io));
                    printCP24Time2a(SinglePointWithCP24Time2a_getTimestamp(io));
            if (dataLog){
                LogRXwT24(InformationObject_getObjectAddress((InformationObject) io),SinglePointInformation_getValue((SinglePointInformation) io),
                          SinglePointWithCP24Time2a_getTimestamp(io));
            }
            //sharedMatrix[ps][2]=SinglePointInformation_getValue((SinglePointInformation) io);
            SinglePointWithCP24Time2a_destroy(io);
        }
    }

    else if (CS101_ASDU_getTypeID(asdu) == M_DP_TA_1) { //4
        int i;
        for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {
            DoublePointWithCP24Time2a io = (DoublePointWithCP24Time2a) CS101_ASDU_getElement(asdu, i);
            printf("    IOA: %i value: %i in time: ",
                   InformationObject_getObjectAddress((InformationObject) io),
                   DoublePointInformation_getValue((DoublePointInformation) io));
            printCP24Time2a(DoublePointWithCP24Time2a_getTimestamp(io));
            if (dataLog){
                LogRXwT24(InformationObject_getObjectAddress((InformationObject) io),DoublePointInformation_getValue((DoublePointInformation) io),
                          DoublePointWithCP24Time2a_getTimestamp(io));
            }
            //sharedMatrix[ps][2]=DoublePointInformation_getValue((DoublePointInformation) io);
            DoublePointWithCP24Time2a_destroy(io);
        }
    }

    else if (CS101_ASDU_getTypeID(asdu) == M_ME_TC_1) { //14
        int i;
        for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {
            MeasuredValueShortWithCP24Time2a io = (MeasuredValueShortWithCP24Time2a) CS101_ASDU_getElement(asdu, i);
            printf("    IOA: %i value: %f\n",
                   InformationObject_getObjectAddress((InformationObject) io),
                   MeasuredValueShort_getValue((MeasuredValueShort) io));
            if (dataLog){
                LogRXwoT(InformationObject_getObjectAddress((InformationObject) io),MeasuredValueShort_getValue((MeasuredValueShort) io));
            }
            //sharedMatrix[ps][2]=MeasuredValueShort_getValue((MeasuredValueShort) io);
            MeasuredValueShortWithCP24Time2a_destroy(io);
        }
    }

    else if (CS101_ASDU_getTypeID(asdu) == M_ME_TF_1) { //36
        int i;
        for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {
            MeasuredValueShortWithCP56Time2a io = (MeasuredValueShortWithCP56Time2a) CS101_ASDU_getElement(asdu, i);
            printf("    IOA: %i value: %.2f time: ",
                   InformationObject_getObjectAddress((InformationObject) io),
                   MeasuredValueShort_getValue((MeasuredValueShort) io));
            printCP56Time2a(MeasuredValueShortWithCP56Time2a_getTimestamp(io));
            if (dataLog){
                LogRXwT(InformationObject_getObjectAddress((InformationObject) io),MeasuredValueShort_getValue((MeasuredValueShort) io),
                        MeasuredValueShortWithCP56Time2a_getTimestamp(io));
            }
            //sharedMatrix[ps][0]=CS101_ASDU_getTypeID(asdu);
            //sharedMatrix[ps][1]=InformationObject_getObjectAddress((InformationObject) io);
            //sharedMatrix[ps][2]=MeasuredValueShort_getValue((MeasuredValueShort) io);
            MeasuredValueShortWithCP56Time2a_destroy(io);
            printf("\n");
            //ps+=1;
        }
    }


    else {
        printf("Received ASDU type: (%d)\n",CS101_ASDU_getTypeID(asdu));
    }


    return true;
}


int
main(int argc, char** argv)
{
    LogSTART();

    int connected=0;

    nacteniMC();

    for (int j = 0; j < 5; ++j) {
        printf("IP address %i: %s \n",j,serversIp[j]);
    }
    printf("Serveru definovano %i\n",serversSet);
    printf("PORT: %i\n",chanPort);
    printf("DATA_LOGS: %i\n",dataLog);


    printf("REQ45: %i\n",req45Used);
    printf("REQUEST45_time: %i\n",req45Time);
    printf("REQUEST45_OBJECTS: %i-%i %i-%i %i-%i .... %i-%i\n",req45Objects[0][0],req45Objects[0][1],
           req45Objects[1][0],req45Objects[1][1],req45Objects[2][0],req45Objects[2][1],req45Objects[4][0],req45Objects[4][1]);

    printf("REQ46: %i\n",req46Used);
    printf("REQUEST46_time: %i\n",req46Time);
    printf("REQUEST46_OBJECTS: %i-%i %i-%i %i-%i .... %i-%i\n",req46Objects[0][0],req46Objects[0][1],
           req46Objects[1][0],req46Objects[1][1],req46Objects[2][0],req46Objects[2][1],req46Objects[4][0],req46Objects[4][1]);

    printf("\n");


    bool running=true;

    CS104_Connection conList[serversSet];
    CS101_AppLayerParameters alParamsList[serversSet];

    LogSETUP_G();

    for (int i = 0; i < serversSet; ++i) {
        conList[i]=CS104_Connection_create(serversIp[i], port);
        CS104_Connection_setConnectionHandler(conList[i], connectionHandler, NULL);
        CS104_Connection_setASDUReceivedHandler(conList[i], asduReceivedHandler, NULL);
        alParamsList[i]=CS104_Connection_getAppLayerParameters(conList[i]);

        if(CS104_Connection_connect(conList[i])){
            Thread_sleep(100);
            printf("CONNECTION: Server %s connected.\n",serversIp[i]);
            CS104_Connection_sendStartDT(conList[i]);
            struct sCP56Time2a newTime;
            CP56Time2a_createFromMsTimestamp(&newTime, Hal_getTimeInMs());
            printf("Send time sync command\n");
            Thread_sleep(100);
            CS104_Connection_sendClockSyncCommand(conList[i], 1, &newTime);
            connected+=1;
        }else{
            printf("CONNECTION: Server %s offline.\n",serversIp[i]);
        }

    }

    printf("Client running...\n");

    int seconds;
    int time_tmp=-1;
    int time_now;

    int time_REQ45timer=0;
    int time_REQ46timer=0;

    int time_con_refresh=0;

    int tmp=0;
    int mtmp=0;

    int a=0;


    while (running){

        seconds = time(NULL)%60;
        time_now=seconds;

        if(time_now!=time_tmp){

            time_REQ45timer+=1;
            time_REQ46timer+=1;
            time_tmp=time_now;
            time_con_refresh+=1;
            mtmp+=1;

            commandCheck();
            for (int i = 0; i < cmdRowSet; ++i) {
                //type=cmdRow[i][0];
                //ioa=cmdRow[i][1];
                //value=cmdRow[i][2];
                SendREQCommand(conList[0],alParamsList[0],5,cmdRow[i][0],cmdRow[i][1],cmdRow[i][2]);
            }

            printf("vterina \n");



        }

        //Thread_sleep(1);
        /*
        if(mtmp>=1){
            FILE *fp;
            fp = fopen ("RXmatrix.txt", "w");
            printf("Matrix \n");
            for (int l = 0; l < 13; ++l) { //5->32
                printf("   [%f %f %f] \n", sharedMatrix[l][0],sharedMatrix[l][1],sharedMatrix[l][2]);
                fprintf(fp,"%f %f %f \n", sharedMatrix[l][0],sharedMatrix[l][1],sharedMatrix[l][2]);
            }
            fclose(fp);
            ps=-1;
            mtmp=0;
        }*/
        Thread_sleep(1);

        if(time_con_refresh>=60){
            for (int i = 0; i < serversSet; ++i) {
                CS104_Connection_close(conList[i]);
                connected=0;
            }
            for (int i = 0; i < serversSet; ++i) {
                CS104_Connection_connect(conList[i]);
                CS104_Connection_sendStartDT(conList[i]);
                //CS104_Connection_createSecure("0.0.0.0",-1,TLSConfiguration_create());
                connected+=1;
            }
            printf("CONs refreshed.\n");
            LogCONREF();
            time_con_refresh=0;

        }/*
        if(a>10 && a<20){
            SendREQCommand(conList[0],alParamsList[0],5,46,4000,2);
            //SendREQCommand(conList[0],alParamsList[0],5,46,4001,2);
        }
        if(a>=20){
            SendREQCommand(conList[0],alParamsList[0],5,46,4000,1);
            //SendREQCommand(conList[0],alParamsList[0],5,46,4001,2);
            a=0;
        }*/


        a+=1;
        Thread_sleep(500);


    /*
        Thread_sleep(1);

        if(time_REQ45timer>=req45Time && req45Used==true && connected>0){

            printf("REQ45 budou zde. V case %i\n",seconds);

            for (int i = 0; i < serversSet; ++i) {
                for (int j = 0; j < req45Set; ++j) {
                    SendREQCommand(conList[i],alParamsList[i],5,45,req45Objects[j][0],req45Objects[j][1]);
                }
            }
            time_REQ45timer=0;
        }
        else if(time_REQ46timer>=req46Time && req46Used==true && connected>0){

            printf("REQ46 budou zde. V case %i\n",seconds);

            for (int i = 0; i < serversSet; ++i) {
                for (int j = 0; j < req46Set; ++j) {
                    SendREQCommand(conList[i],alParamsList[i],5,46,req46Objects[j][0],req46Objects[j][1]);
                }
            }
            time_REQ46timer=0;
            }*/
        }

    CS104_Connection_destroy(conList[0]);
    CS104_Connection_destroy(conList[1]);
    Thread_sleep(1000);
    printf("exit\n");
}


