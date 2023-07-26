

bool serialReadLine(char* str, size_t size) ;

/*
  Change the value of SD_CS_PIN if you are using SPI and
  your hardware does not use the default value, SS.
  Common values are:
  Arduino Ethernet shield: pin 4
  Sparkfun SD shield: pin 8
  Adafruit SD shields and modules: pin 10
*/

// SDCARD_SS_PIN is defined for the built-in SD on some boards.
#ifndef SDCARD_SS_PIN
const uint8_t SD_CS_PIN = 10;
#else   // SDCARD_SS_PIN
// Assume built-in SD is used.
const uint8_t SD_CS_PIN = SDCARD_SS_PIN;
#endif  // SDCARD_SS_PIN
/*
// FIFO SIZE - 512 byte sectors.  Modify for your board.
#ifdef __AVR_ATmega328P__
// Use 512 bytes for 328 boards.
#define FIFO_SIZE_SECTORS 1
#elif defined(__AVR__)
// Use 2 KiB for other AVR boards.
#define FIFO_SIZE_SECTORS 4
#else  // __AVR_ATmega328P__
// Use 8 KiB for non-AVR boards.
#define FIFO_SIZE_SECTORS 16
#endif  // __AVR_ATmega328P__

// Preallocate 1GiB file.
//const uint32_t PREALLOCATE_SIZE_MiB = 1024UL; // max size 1024*1024 = 1.048.576 kb

// Try max SPI clock for an SD. Reduce SPI_CLOCK if errors occur.
#define SPI_CLOCK SD_SCK_MHZ(50)

// Try to select the best SD card configuration.
#if HAS_SDIO_CLASS
#define SD_CONFIG SdioConfig(FIFO_SDIO)
#elif ENABLE_DEDICATED_SPI
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SPI_CLOCK)
#else  // HAS_SDIO_CLASS
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SPI_CLOCK)
#endif  // HAS_SDIO_CLASS

// Save SRAM if 328.
#ifdef __AVR_ATmega328P__
#include "MinimumSerial.h"
MinimumSerial MinSerial;
#define Serial MinSerial
#endif  // __AVR_ATmega328P__
*/



#if SD_FAT_TYPE == 0
typedef SdFat sd_t;
typedef File file_t;
#elif SD_FAT_TYPE == 1
typedef SdFat32 sd_t;
typedef File32 file_t;
#elif SD_FAT_TYPE == 2
typedef SdExFat sd_t;
typedef ExFile file_t;
#elif SD_FAT_TYPE == 3
typedef SdFs sd_t;
typedef FsFile file_t;
#else  // SD_FAT_TYPE
#error Invalid SD_FAT_TYPE
#endif  // SD_FAT_TYPE

sd_t sd;

file_t binFile;
file_t csvFile;




//------------------------------------------------------------------------------

/*
#if USE_RTC
#if USE_RTC == 1
RTC_DS1307 rtc;
#elif USE_RTC == 2
RTC_DS3231 rtc;
#elif USE_RTC == 3
RTC_PCF8523 rtc;
#else  // USE_RTC == type
#error USE_RTC type not implemented.
#endif  // USE_RTC == type
*/
// Call back for file timestamps.  Only called for file create and sync().


#ifdef RTC_ADON_DATALOGGER
void dateTime(uint16_t* date, uint16_t* time, uint8_t* ms10) {
  DateTime now = rtc.now();

  // Return date using FS_DATE macro to format fields.
  *date = FS_DATE(now.year(), now.month(), now.day());

  // Return time using FS_TIME macro to format fields.
  *time = FS_TIME(now.hour(), now.minute(), now.second());

  // Return low time bits in units of 10 ms.
  *ms10 = now.second() & 1 ? 100 : 0;
}
#endif
#ifdef RTC_STM332_NUCLEO44
void dateTime(uint16_t* date, uint16_t* time, uint8_t* ms10) {
 //DateTime now = rtc.now();

  // Return date using FS_DATE macro to format fields.
  *date = FS_DATE(rtc.getYear(), rtc.getMonth(), rtc.getDay());

  // Return time using FS_TIME macro to format fields.
  *time = FS_TIME(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());

  // Return low time bits in units of 10 ms.
  *ms10 = rtc.getSeconds() & 1 ? 100 : 0;
}

#endif
//#endif  // USE_RTC


//------------------------------------------------------------------------------
#define error(s) sd.errorHalt(&Serial, F(s))
#define dbgAssert(e) ((e) ? (void)0 : error("assert " #e))
//-----------------------------------------------------------------------------
// Convert binary file to csv file.
void binaryToCsv() {
  uint8_t lastPct = 0;
  uint32_t t0 = millis();
  data_t binData[FIFO_DIM];

  if (!binFile.seekSet(512)) {
    //error("binFile.seek failed");
    exSD_Status = CSVFILE_FAIL;
  }
  uint32_t tPct = millis();
  printRecord(&csvFile, nullptr);
  while (!Serial.available() && binFile.available()) {
    int nb = binFile.read(binData, sizeof(binData));
    if (nb <= 0) {
      //error("read binFile failed");
      exSD_Status = CSVFILE_FAIL;
    }
    size_t nr = nb / sizeof(data_t);
    for (size_t i = 0; i < nr; i++) {
      printRecord(&csvFile, &binData[i]);  // 
    }
      //Serial.print(F("nr: "));
      // Serial.println(nr);     
      //printRecord(&csvFile, &binData[i]);

      
    if ((millis() - tPct) > 1000) {
      uint8_t pct = binFile.curPosition() / (binFile.fileSize() / 100);
      if (pct != lastPct) {
        tPct = millis();
        lastPct = pct;
        Serial.print(pct, DEC);
        Serial.println('%');
        csvFile.sync();
      }
    }
  //  if (Serial.available()) {
  //    break;
  //  }
  }
  //printRecord(&csvFile, &binData[i]);
  csvFile.close();
  Serial.print(F("Done: "));
  Serial.print(0.001 * (millis() - t0));
  Serial.println(F(" Seconds"));
  exSD_Status = CSVFILE_CREATE;
}

//-------------------------------------------------------------------------------
bool createBinFile() {
  binFile.close();
  while (sd.exists(binName)) {
    char* p = strchr(binName, '.');
    if (!p) {
     // error("no dot in filename");
      return 0;
    }
    while (true) {
      p--;
      if (p < binName || *p < '0' || *p > '9') {
        error("Can't create file name");
      }
      if (p[0] != '9') {
        p[0]++;
        break;
      }
      p[0] = '0';
    }
  }
  if (!binFile.open(binName, O_RDWR | O_CREAT)) {
    //error("open binName failed");
    exSD_Status = BINFILE_FAIL;
    return 0;
  }
  Serial.print("bin File Created : "); 
//  Serial.println(binName);
    exSD_Status = BINFILE_CREATE;
  if (!binFile.preAllocate(PREALLOCATE_SIZE)) {
    //error("preAllocate failed");
    return 0;
  }

  Serial.print(F("preAllocated: "));
  Serial.print(PREALLOCATE_SIZE_MiB);
  Serial.println(F(" MiB"));
  return 1;
}
//-------------------------------------------------------------------------------
bool createCsvFile() {
  char csvName[FILE_NAME_DIM];
  if (!binFile.isOpen()) {
    exSD_Status = BINFILE_FAIL;
    Serial.println(F("No current binary file"));
    return false;
  }

  // Create a new csvFile.
  binFile.getName(csvName, sizeof(csvName));
  char* dot = strchr(csvName, '.');
  if (!dot) {
    //error("no dot in filename");
    exSD_Status = CSVFILE_FAIL;
    Serial.println(F("no dot in filename"));   
    return false;
  }
  strcpy(dot + 1, "csv");
  if (!csvFile.open(csvName, O_WRONLY | O_CREAT | O_TRUNC)) {
    //error("open csvFile failed");
    exSD_Status = CSVFILE_FAIL;
    Serial.println(F("open csvFile failed"));   
    return false;  
  }
  //clearSerialInput();
  Serial.print(F("Writing: "));
  Serial.print(csvName);
  Serial.println(F(" - type any character to stop"));
  exSD_Status = CSVFILE_CREATE;
  return true;
}


  
//-------------------------------------------------------------------------------
bool logData(void) {

  
  data_t* fifoData = (data_t*)fifoBuf;

  // Write dummy sector to start multi-block write.

  dbgAssert(sizeof(fifoBuf) >= 512);
  memset(fifoBuf, 0, sizeof(fifoBuf));
  if (binFile.write(fifoBuf, 512) != 512) {
    //error("write first sector failed");
    Serial.println("write first sector failed");
    return OFF;
  }
 // clearSerialInput();
  //Serial.println(F("Type any character to stop"));
  Serial.println(F("Type Esc 2 stop"));
  // Wait until SD is not busy.
  
  while (sd.card()->isBusy()) {}

    Str_DateLog_Init = Str_Date;
    Str_TimeLog_Init = Str_Time;
    Sample_No = 0;
    Sample_Err_No = 0;

  // Start time for log file.
  uint32_t m = millis();

  // Time to log next record.
  uint32_t logTime = micros();
  while (true) {
    // Time for next data record.
 //   logTime += Log_Interval;
    logTime += SampleTime;
    // Wait until time to log data.
    delta = micros() - logTime;
    Rate_Fast = OFF;
    if (delta > 0) {
    //  Serial.print(F("delta: "));
    //  Serial.println(delta);
    //  Serial.println("Rate too fast");
     // error("Rate too fast");
      //return OFF;
      Rate_Fast = ON;
    }
    while (delta < 0) {  // wait until logtime is achieved for sampling 
      delta = micros() - logTime;
    }

    Sample_No++;

    if (fifoCount < FIFO_DIM) {
      uint32_t m = micros();
      logRecord(fifoData + fifoHead, overrun);
      m = micros() - m;
      if (m > maxLogMicros) {
        maxLogMicros = m;
      }
      fifoHead = fifoHead < (FIFO_DIM - 1) ? fifoHead + 1 : 0;
      fifoCount++;
      if (overrun) {
        if (overrun > maxOverrun) {
          maxOverrun = overrun;
        }
        overrun = 0;
      }
    } else {
      totalOverrun++;
      overrun++;
      if (overrun > 0XFFF) {
        //error("too many overruns");
          Serial.println("too many overruns");
         return OFF;
      }
     // if (ERROR_LED_PIN >= 0) {digitalWrite(ERROR_LED_PIN, HIGH);}
    }
    // Save max jitter.
    if (delta > maxDelta) {
      maxDelta = delta;
    }
    // Write data if SD is not busy.
    if (!sd.card()->isBusy()) {
      size_t nw = fifoHead > fifoTail ? fifoCount : FIFO_DIM - fifoTail;
      // Limit write time by not writing more than 512 bytes.
      const size_t MAX_WRITE = 512 / sizeof(data_t);
      if (nw > MAX_WRITE) nw = MAX_WRITE;
      size_t nb = nw * sizeof(data_t);
      uint32_t usec = micros();
      if (nb != binFile.write(fifoData + fifoTail, nb)) {
       // error("write binFile failed");
         Serial.println("write binFile failed");
         return OFF;
      }
      usec = micros() - usec;
      if (usec > maxWriteMicros) {
        maxWriteMicros = usec;
      }
      fifoTail = (fifoTail + nw) < FIFO_DIM ? fifoTail + nw : 0;
      if (fifoCount > maxFifoUse) {
        maxFifoUse = fifoCount;
      }
      fifoCount -= nw;
      /*
      if (Serial.available()) {
        break;
      }
      */
      if(Log_Escape())break;
      
     // if(Rate_Fast == ON)break;
      if(Rate_Fast == ON){
        Sample_Err_Arr[Sample_Err_No] = Sample_No;
        Sample_Err_No++;
        if(!(Sample_Err_No < 100)){
          break;
        }
      }
    }
  } //while(1)
  Serial.print(F("\nLog time: "));
  Serial.print(0.001 * (millis() - m));
  Serial.println(F(" Seconds"));
  binFile.truncate();
  binFile.sync();
  Serial.print(("File size: "));
  // Warning cast used for print since fileSize is uint64_t.
  Serial.print((uint32_t)binFile.fileSize());
  Serial.println(F(" bytes"));
  Serial.print(F("totalOverrun: "));
  Serial.println(totalOverrun);
  Serial.print(F("FIFO_DIM: "));
  Serial.println(FIFO_DIM);
  Serial.print(F("maxFifoUse: "));
  Serial.println(maxFifoUse);
  Serial.print(F("maxLogMicros: "));
  Serial.println(maxLogMicros);
  Serial.print(F("maxWriteMicros: "));
  Serial.println(maxWriteMicros);
  Serial.print(F("Log interval: "));
  //Serial.print(Log_Interval);
   Serial.print(SampleTime); 
  Serial.print(F(" micros\nmaxDelta: "));
  Serial.print(maxDelta);
  Serial.println(F(" micros"));
 
  Serial.print(F("Sample_Err_No : "));
  Serial.println(Sample_Err_No);
  Serial.println(F(" Error Lines"));  
  if(!(Sample_Err_No < 100)) Sample_Err_No = 99;  
  for(uint32_t i = 0; i <= Sample_Err_No ; i++){
    Serial.print(i);Serial.print(F(": "));
    Serial.println(Sample_Err_Arr[i]);
  }
  
  Serial.println(F(" "));   
  return ON;
}
//------------------------------------------------------------------------------
/*
void openBinFile() {
  char name[FILE_NAME_DIM];
  clearSerialInput();
  Serial.println(F("Enter file name"));
  if (!serialReadLine(name, sizeof(name))) {
    return;
  }
  if (!sd.exists(name)) {
    Serial.println(name);
    Serial.println(F("File does not exist"));
    return;
  }
  binFile.close();
  if (!binFile.open(name, O_RDONLY)) {
    Serial.println(name);
    Serial.println(F("open failed"));
    return;
  }
  Serial.println(F("File opened"));
}
*/
//-----------------------------------------------------------------------------
void printData() {
  if (!binFile.isOpen()) {
    Serial.println(F("No current binary file"));
    return;
  }
  // Skip first dummy sector.
  if (!binFile.seekSet(512)) {
    error("seek failed");
  }
  clearSerialInput();
  Serial.println(F("type any character to stop\n"));
  delay(1000);
  printRecord(&Serial, nullptr);
  while (binFile.available() && !Serial.available()) {
    data_t record;
    if (binFile.read(&record, sizeof(data_t)) != sizeof(data_t)) {
      error("read binFile failed");
    }
    printRecord(&Serial, &record);
  }
}
//------------------------------------------------------------------------------
void printUnusedStack() {
#if HAS_UNUSED_STACK
  Serial.print(F("\nUnused stack: "));
  Serial.println(UnusedStack());
#endif  // HAS_UNUSED_STACK
}
//------------------------------------------------------------------------------
bool serialReadLine(char* str, size_t size) {
  size_t n = 0;
  while (!Serial.available()) {
    yield();
  }
  while (true) {
    int c = Serial.read();
    if (c < ' ') break;
    str[n++] = c;
    if (n >= size) {
      Serial.println(F("input too long"));
      return false;
    }
    uint32_t m = millis();
    while (!Serial.available() && (millis() - m) < 100) {
    }
    if (!Serial.available()) break;
  }
  str[n] = 0;
  return true;
}

//  Log_Status Phases
#define LOG_OFF 0
#define LOG_START 1
#define LOG_BIN_CREATE 2
#define LOG_BIN_CREATE_POST 3
#define LOG_LOOP_IN_ACTION 4
#define LOG_LOOP_ENDED_SUCCESS 5
#define LOG_LOOP_ENDED_FAIL 8
 
#define LOG_2CSV_CREATE 6
#define LOG_2CSV_CREATE_POST 7

// Timers 
#define LOG_START_TIMER 3
#define LOG_LOOP_ENDED_SUCCESS_TIMER 3
#define LOG_2CSV_CREATE_POST_TIMER 3
#define LOG_BIN_CREATE_POST_TIMER 3
 




void ExFat_Logger_Loop(){
    if( Log_Status == LOG_START){  
      Log_StatusTimer++;
      if(Log_StatusTimer > LOG_START_TIMER){
        Log_StatusTimer = 0;
        Log_Status = LOG_BIN_CREATE;
      }
      /*
        char c = tolower(Serial.read());
      if (c == 'b') {
        openBinFile();
      }else if (c == 'c') {
      if (createCsvFile()) {
        binaryToCsv();
       }
      }else if (c == 'l') {
        Serial.println(F("ls:"));
        sd.ls(&Serial, LS_DATE | LS_SIZE);
        } else if (c == 'p') {
        printData();
      } else if (c == 'r') {
        createBinFile();
        logData();
      } else if (c == 't') {
        testSensor();
      } else if (c == 's') {
       // PrintValues = !PrintValues;   
      } else {
      //Serial.println(F("Invalid entry"));
      }
      */
    }
    if( Log_Status == LOG_BIN_CREATE){  
      if(createBinFile()){
         Serial.println(binName);  
      }
      else{    
        Serial.println("File Failed");  Log_Status = LOG_LOOP_ENDED_FAIL; 
      }
      /*
      Serial.println(binName);        
       if (!binFile.preAllocate(PREALLOCATE_SIZE)) {
           Serial.println("preAllocate failed");        
      }
      */
      Log_Status = LOG_BIN_CREATE_POST;
    }
    if( Log_Status == LOG_BIN_CREATE_POST){  
      Log_StatusTimer++;
      if(Log_StatusTimer > LOG_BIN_CREATE_POST_TIMER){
        Log_StatusTimer = 0;
        Log_Status = LOG_LOOP_IN_ACTION;
      }
    }   
    if( Log_Status == LOG_LOOP_IN_ACTION){  
     //   noInterrupts(); //dont do also disables tim0 int which is millis 
        if(logData()){; // data logging loop, disables interrupt and stuck in the loop unless esc pressed
          Log_Status = LOG_LOOP_ENDED_SUCCESS; // let it convert to csv in main loop
        }
        else {
           Serial.println("Log Failed");
           Log_Status = LOG_LOOP_ENDED_FAIL;
        }
        // MainMenu = MENU1_SUB2;// log stop menu
        // SDCard.LogEnable = OFF;
     //   interrupts();
    }
    if( Log_Status == LOG_LOOP_ENDED_SUCCESS){  
      Log_StatusTimer++;
      if(Log_StatusTimer > LOG_LOOP_ENDED_SUCCESS_TIMER){
        Log_StatusTimer = 0;
        Log_Status = LOG_2CSV_CREATE;
      }
    }  
    
    if( Log_Status == LOG_2CSV_CREATE){
       if (createCsvFile()) {
          binaryToCsv();      
       }
       else{
            Serial.println(" CSV file Create Error !");
       } 
       Log_Status = LOG_2CSV_CREATE_POST;      
    }
    if( Log_Status == LOG_2CSV_CREATE_POST){  
      Log_StatusTimer++;
      if(Log_StatusTimer > LOG_2CSV_CREATE_POST_TIMER){
        Log_StatusTimer = 0;
        Log_Status = LOG_LOOP_ENDED_FAIL;
      }
    }  
    
     if( Log_Status == LOG_LOOP_ENDED_FAIL){
      ExLog_End();  
      Rate_Fast = OFF;
     }
}
void exSdCard_Init(){
  FsDateTime::setCallback(dateTime);
  FillStack();
  Serial.print(FIFO_DIM);
  Serial.println(F(" FIFO entries will be used."));

  // Initialize SD.
  if (!sd.begin(SD_CONFIG)) {
    //sd.initErrorHalt(&Serial)
    Serial.println(F(" SD Card Failed "));
    exSD_Status = SDCARD_PROBLEM;
  }
  else{
    exSD_Status = SDCARD_INIT;
    Serial.println(F(" SD Card Init"));
  }
  
  printUnusedStack();
}
