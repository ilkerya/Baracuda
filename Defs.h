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
const uint32_t PREALLOCATE_SIZE_MiB = 1024UL; // max size 1024*1024 = 1.048.576 kb

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


//==============================================================================
//const uint64_t PREALLOCATE_SIZE = (uint64_t)PREALLOCATE_SIZE_MiB << 20;
// Max length of file name including zero byte.
#define FILE_NAME_DIM 40
// Max number of records to buffer while SD is busy.
//const size_t FIFO_DIM = 512 * FIFO_SIZE_SECTORS / sizeof(data_t);

#define SD_FAT_TYPE 3

//uint32_t Log_Interval = 10000; // 2 msec
  
  void clearSerialInput(void);
  void ExLog_Start(void);
  void ExLog_End(void);

  #define TASK_1MSEC     1000
  #define TASK_5MSEC     5000   
  #define TASK_10MSEC   10000
  #define TASK_20MSEC   20000
  #define TASK_50MSEC   50000
  #define TASK_100MSEC 100000
  #define TASK_250MSEC 250000
  #define TASK_500MSEC 500000
  #define TASK_1SEC   1000000
  
  #define TASK_2SEC   2000000
  #define TASK_5SEC   5000000
  #define TASK_10SEC 10000000
  #define TASK_20SEC 200
  #define TASK_60SEC 202 
// 001BC5067010E312
/*
typedef  byte               uint8;
typedef  signed char        int8;
typedef  unsigned char      uint8;
//typedef  signed short       int16;
//typedef  unsigned short     uint16;
typedef  signed int         int16;
typedef  unsigned int       uint16;
typedef  signed long        int32;
typedef  unsigned long      uint32;
typedef  signed long long   int64;
typedef  unsigned long long uint64

C:\Program Files (x86)\Arduino\libraries

*/
//#define FASTLOG_INIT_TIMER 10

#define DOWNROLL 0
#define UPROLL 1
//#define TEMP_HUM_ONBOARD_SENSOR_EXISTS
//#define TEMP_HUM_1_SENSOR_EXISTS
//#define TEMP_HUM_2_SENSOR_EXISTS
//#define TEMP_HUM_3_SENSOR_EXISTS  

//#define PM25_DUST_SENSOR_EXISTS
#define SDCARD_EXISTS
#define OLEDDISPLAY_EXISTS
//#define ENERGYMETER_EXISTS 
//#define PROGRELAY_EXISTS 
#define BATTERY_SCOOTER_EXISTS
#define  SOLAR_PANEL_EXISTS
//#define LIGHT_SENSOR_EXISTS  
//#define BAR_PRES_SENSOR_EXISTS  
#define ACCL_GYRO_SENSOR_EXISTS  
//
//#define  LIDAR_EXISTS
//#define WIND_SENSOR_EXISTS  
//#define LEM_CURRENT_EXISTS
//#define VOLTAGE_MEASURE_EXISTS

//#define KEY_DIGITAL
#define KEY_ANALOG
#define SPI_SPEED 100000     //SPI Speed   max for MEGA 8 MHz
#define AD9153_PROTOTYPE  // AD9153 Power Monitoring IC Related IOs

 // #define DEBUG_SIMULATOR_MODE // For DEbugging As A Simulator
// Select Hardware Type
//#define FIRST_PROTOTYPE  // with LEM current Transdcucer

#define ARM_MATH_CM0PLUS

#define RELAY_OUT_1 23
#define RELAY_OUT_2 53

//#include "SdsDustSensor.h" // https://github.com/lewapek/sds-dust-sensors-arduino-library
/*
 #define SI072_FIRST_SENSOR 7  // multiplexer Channel 7 first blu box prot
 #define SI072_SECOND_SENSOR 1 // first prot  0      0
 #define SI072_THIRD_SENSOR 2 // sec1                2 
*/
/*
 // first ADE9153 prototypes
 #define SI072_FIRST_SENSOR 2  // multiplexer Channel 7 first blu box prot
 #define SI072_SECOND_SENSOR 3 // first prot  0      0
 #define SI072_THIRD_SENSOR 4 // sec1                2 
*/

#define POWERIC_SETUP1 0  // Init
#define POWERIC_SETUP2 1  // Delay
#define POWERIC_SETUP3 2 // End
#define POWERIC_NORMAL 3
#define POWERIC_CALB1  4
#define POWERIC_CALB2  5 
#define POWERIC_CALB3  6
#define POWERIC_CALB4  7
#define POWERIC_CALB5  8
#define POWERIC_CALB6  9
#define POWERIC_CALB7  10
#define POWERIC_CALB8  11
#define POWERIC_CALB9  12
#define POWERIC_CALB10 13

/*
#define KBYTE_500 524288
#define MBYTE_1 1048576
#define MBYTE_2 2097152
#define MBYTE_4 4194304
#define MBYTE_10 10485760 
#define MBYTE_20 20971520
*/
#define FILE_500KBYTE 524288
#define KBYTE_500 1
#define MBYTE_1 2
#define MBYTE_2 4
#define MBYTE_4 8
#define MBYTE_8 16
#define MBYTE_16 32 
#define MBYTE_32 64



 #define SI072_ONBOARD_SENSOR_ADDR 1  // multiplexer Channel 7 first blu box prot
 #define SI072_FIRST_SENSOR_ADDR 7  // multiplexer Channel 7 first blu box prot
 #define SI072_SECOND_SENSOR_ADDR 3 // first prot  0      0
 #define SI072_THIRD_SENSOR_ADDR 2 // sec1                2 

#define NO_IC2_MULTIPLEXER 16
#define DEBUG_KEY

#define ON 1 //
#define OFF 0 //

#ifdef FIRST_PROTOTYPE
  //  const int chipSelect = 10; // mega SS for SD Card  
  #define KEY_UP 5 // 12//4 //RED
  #define KEY_RIGHT 2//12//2 //
  NN
  #define LED_GREEN 3// 11//3 // GREEN
  #define LED_RED 4 // 12//4 //RED
  #define KEY_MID 5// 11//5 //
  #define KEY_LEFT 6//13//6 // ok
#endif



#if defined (STM32_F407) 
/*
  #define TDI         A7 //9 Atmel ice JTAG  // 0 2 GND
  #define TDO         A6 //3 Atmel ice JTAG  // 4 Vdd -> 5V mega 3V3 Due
  #define TMS         A5 //5 Atmel ice JTAG  // 6 Reset
  #define TCK         A4 //1 Atmel ice JTAG
  */
  #define SOL_VOLT      A4  
  #define SOL_CURRENT     A3
  #define BAT_VOLT        A2
  #define BAT_CURRENT     A1 
  uint16_t KEY_ANALOG_IN = A0;  



  
    #define DEBUG_OUT_1          9 
    #define DEBUG_OUT_2          8 
    #define DEBUG_OUT_3          7 
  


          
    #if  defined KEY_DIGITAL
  #define KEY_RIGHT       13  gg hh
  #define KEY_DOWN        12
  #define KEY_LEFT        11
  #define KEY_UP          12    
    #endif
   //   const int chipSelect = 10;
   //   const int SD_CS_PINOUT = 10;
      #define SD_CS_PINOUT    10  
#endif

#if defined (ARDUINO_MEGA)  | defined (ARDUINO_DUE) 
  #define TDI         A7 //9 Atmel ice JTAG  // 0 2 GND
  #define TDO         A6 //3 Atmel ice JTAG  // 4 Vdd -> 5V mega 3V3 Due
  #define TMS         A5 //5 Atmel ice JTAG  // 6 Reset
  #define TCK         A4 //1 Atmel ice JTAG

 #define DEBUG_OUT_1   8
 #define DEBUG_OUT_2   9
 #define DEBUG_OUT_3   7

   
 // #define DEBUG_OUT      A4  
  #define ANALOG3        A3
  #define BAT_VOLT        A2
  #define BAT_CURRENT     A1
  #define KEY_ANALOG_IN   A0  
     
    #if  defined KEY_DIGITAL
  #define KEY_RIGHT       13
  #define KEY_DOWN        12
  #define KEY_LEFT        11
  #define KEY_UP          12    
    #endif
   //   const int chipSelect = 10;
   //   const int SD_CS_PINOUT = 10;
      #define SD_CS_PINOUT    10  
        #define LED_RED        9  // L1
          #define  LED_GREEN         7 //L2
#endif


    #ifdef ENERGYMETER_EXISTS
  #define ADE9153A_CS_PIN  8 
      #endif      

#ifdef ENERGYMETER_EXISTS 
  #define ADE9153A_RED_LED 6                 //On-board LED pin 
  #define ADE9153A_CALB_BUTTON   5         
  #define ADE9153A_RESET_PIN     4  
  #define ADE9153A_IRQ_PIN       3
  #define ADE9153A_ZX_DREADY_PIN 2
#endif
  #define TX_OUTPUT_PIN          1 //ON BOARD PROGRAMMING & DEBUG RESERVED
  #define RX_INPUT_PIN           0  //ON BOARD PROGRAMMING & DEBUG RESERVED

  #define OUT_PINOUT 2 // Out pin of the sensor
  #define RV_PINOUT 1 // RV output of the sensor
  #define TMP_PINOUT 0 // analogRead(TMP_PINOUT);

#define MAX_DISPLAY_CHAR 21
#define  MAXSHOWLINE 6  // define how many lines for sensorts to show including fw info line 

#define DISPSHOWLINE4
#define DISPSHOWLINE3
#define DISPSHOWLINE2
#define DISPSHOWLINE1

#define DISPROLL_LINE0 0
#define DISPROLL_LINE1 1
#define DISPROLL_LINE2 2
#define DISPROLL_LINE3 3
#define DISPROLL_LINE4 4
#define DISPROLL_LINE5 5
#define DISPROLL_LINE6 6
#define DISPROLL_LINE7 7
#define DISPROLL_LINE8 8
#define DISPROLL_LINE9 9

#define MENU_NULL 0
#define MENU1   16
#define MENU2   32
#define MENU3   48
#define MENU4   64
#define MENU5   80
#define MENU6   96
#define MENU7   112

#define MENU1_SUB1 17 // +=4  // Log Start
#define MENU1_SUB2 18    // / Log Stop
#define MENU1_SUB3 19 // +=4
#define MENU1_SUB4 20

#define MENU2_SUB01  33  // +=4
#define MENU2_SUB02  34
#define MENU2_SUB03  35
#define MENU2_SUB04  36
#define MENU2_SUB05  37
#define MENU2_SUB06  38
#define MENU2_SUB07  39
#define MENU2_SUB08  40
#define MENU2_SUB09  41
#define MENU2_SUB10  42
#define MENU2_SUB11  43
#define MENU2_SUB12  44
#define MENU2_SUB13  45
#define MENU2_SUB14  46

#define MENU3_SUB1  49 // +=4
#define MENU3_SUB2  50
#define MENU3_SUB3  51 // +=4
#define MENU3_SUB4  52

#define MENU4_SUB1 65
#define MENU4_SUB2 66
#define MENU4_SUB3 67
#define MENU4_SUB4 68

#define MENU5_SUB1 81
#define MENU5_SUB2 82
#define MENU5_SUB3 83
#define MENU5_SUB4 84
#define MENU5_SUB5 85
#define MENU5_SUB6 86
#define MENU5_SUB7 87
#define MENU5_SUB8 88

#define MENU6_SUB1 97
#define MENU6_SUB2 98
#define MENU6_SUB3 99
#define MENU6_SUB4 100
#define MENU6_SUB5 101
#define MENU6_SUB6 102
#define MENU6_SUB7 103

#define MENU7_SUB1 113
#define MENU7_SUB2 114
#define MENU7_SUB3 115
#define MENU7_SUB4 116
#define MENU7_SUB5 117
#define MENU7_SUB6 118
#define MENU7_SUB7 119
#define MENU7_SUB8 120
#define MENU7_SUB9 121

#define KEYDISP_TIMER 40

#define SD_NOT_Present 0
#define SD1_TYPE 1
#define SD2_TYPE 2
#define SDHC_TYPE 3
#define UNKNOWN_TYPE 4

#define LETTER_NUMBER 0
#define NUMBER_ONLY 1
/*
#if defined (ARDUINO_MEGA)  & defined (ARDUINO_DUE) 
    #error Select Only One Platform-> ARDUINO_MEGA or ARDUINO_DUE
#endif

 #if !(!defined (ARDUINO_MEGA) ^ !defined (ARDUINO_DUE)) 
    #error Select At Least One Platform -> ARDUINO_MEGA or ARDUINO_DUE
#endif
*/
#if defined (KEY_DIGITAL)  & defined (KEY_ANALOG) 
    #error Select Only One Type -> KEY_DIGITAL or KEY_ANALOG
#endif

 #if !(!defined (KEY_DIGITAL) ^ !defined (KEY_ANALOG)) 
    #error Select At Least One Type -> KEY_DIGITAL or KEY_ANALOG
#endif

// function prototypes
void Common_Loop(); 
void ResetCasePrint();
void IO_Settings();
void MicroInit(void);
void Display_ReInit_Start(uint8_t Timer);
void Display_ReInit_End(void);

void SD_Card_Info(void);
void SD_Card_Init(void);
void SD_Card_Data_Preparation(void);
void SD_Card_Header_Preparation(void);

void RTC_Init();
void SensorInit_Si072(uint8_t);
void SensorAlt_Init();
void SensorLight_Init();
void SensorACccel_GyroInit();
void Sensor_LidarInit(void);
void Sensors_PeripInit();

void CurrentVolt_Read(void);
void AdcRead(void);
void WindSensorRead(void);
void SensorRead_Si072(unsigned char);
void SensorAlt_Read(void);
void SensorLight_Read(void);
void SensorAcccel_GyroRead(void);
void Sensor_LidarRead(void);
void SDS_DustSensor(void);
void UpdateSensorInfo(void);

void UpdateInfoLine();
void UpdateDisplayMenu();
void UpdateSD_LogTime();
void UpdateFileSize();
void ConvertFileSize(uint32_t);// Line3  
void UpdateProperLine(uint8_t Index, uint8_t Line);

void  RTC_TimeClock(void);

void EscMenuKey(void);
void EnterMenuKey(void);
void DownMenuKey(void);
void UpMenuKey(void);
void SetSampling(uint16_t Time);
void DispEnable(bool Enable, uint8_t Timer);
void DispEnable_4SD_Prblm(bool Enable, uint8_t Timer);

void  DispExtTimeout(void);
void   DisplayMenu(void);
void KeyTimeOutCheck(void);
void SD_CardLogTask(void);
void SD_Log_File(void);
void SD_Info_Only(void);
void DisplayFullSensors(void);
void DisplayTestDevices(void);
void SerialPortRx(void);
void UpdateDispRoll(uint8_t);
void Log_Data_Write_SD(void);

void Parse_FileString(void);
void Relay_loop(void) ;
float GetValue(uint8_t Relay);
String LimitCopyDisplayStr(String str, uint8_t MaxNumber);
void EnergyMeterIC_Operation(void);
void I2_ACK_Reset(void);

void SetResetLog(bool Enable);
void NVRam_Write_LogStatus(bool Mode);
uint8_t NVRam_Read(uint8_t Address);
void NVRam_Write(uint8_t Address, uint8_t Sample); // EE_SAMPLE
void NVRam_Read_Standbye(void);
void NVRam_Write_Standbye(bool Mode);
void NVRam_Read_MainsFreq(void);
void NVRam_Write_MainsFreq(bool Mode);
void NVRam_Write_MaxFileSize(uint8_t Size);
uint8_t NVRam_Read_MaxFileSize(void);

void NVRam_Read_SerNo(void);
void NVRam_Write_SerNo(char* p);
void NVRam_Read_QueNo(void);
void NVRam_Write_QueNo(char* p);
char NVRam_Check_Content(char Content, bool Number);

void UpdateLogFileId(void);
char* CopyFlashToRam(const char* );

void Due_Memory();
void Print_ARM_SPI_Regs(void);

void UpdateLogFileNo(void);
void UpdateLogFileId(void);
void Startup_NV_Ram(void);

void EEProm_Update_FileNo(void);
void EEProm_Update_DevId(void);
void EEProm_Update_Debug(bool);

bool Log_Escape(void);

/*
C:\Users\ilker\Documents\Atmel Studio\7.0\ArduinoSketch6\ArduinoSketch6\ArduinoCore\src\libraries\SD\utility\Sd2Card.cpp 
 // send command and return error code.  Return zero for OK
uint8_t Sd2Card::cardCommand(uint8_t cmd, uint32_t arg) {
  chipSelectLow();


  static uint8_t chip_select_asserted = 0;

void Sd2Card::chipSelectHigh(void) {
  digitalWrite(chipSelectPin_, HIGH);
  #ifdef USE_SPI_LIB
  if (chip_select_asserted) {
    chip_select_asserted = 0;
    SDCARD_SPI.endTransaction();
  }
  #endif
}
//------------------------------------------------------------------------------
void Sd2Card::chipSelectLow(void) {
  #ifdef USE_SPI_LIB
  if (!chip_select_asserted) {
    chip_select_asserted = 1;
    SDCARD_SPI.beginTransaction(settings);
  }
  #endif
  digitalWrite(chipSelectPin_, LOW);
}


 * /
 */
