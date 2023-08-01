// https://en.cppreference.com/w/cpp/types/integer

char binName[] = "Voi_V5_data00.bin";
//char CSV_File[] = "Voi_V5_data00.bin";
uint8_t Log_Status = 0;
uint8_t Log_StatusTimer = 0;
uint8_t exSD_Status = 0;

bool Rate_Fast = OFF;
 
//const size_t ADC_COUNT = 6;


struct data_t {
 // uint16_t adc[ADC_COUNT];
 float Sensor_Arr[8];
 uint16_t  Adc_Arr[2];
};

static uint32_t LogNumber = 0;  
uint32_t Sample_Err_Arr[100];
uint32_t Sample_Err_No = 0;
uint32_t Sample_No = 0;

const uint64_t PREALLOCATE_SIZE = (uint64_t)PREALLOCATE_SIZE_MiB << 20;
const size_t FIFO_DIM = 512 * FIFO_SIZE_SECTORS / sizeof(data_t);



  int32_t delta;  // Jitter in log time.
  int32_t maxDelta = 0;
  uint32_t maxLogMicros = 0;
  uint32_t maxWriteMicros = 0;
  size_t maxFifoUse = 0;
  size_t fifoCount = 0;
  size_t fifoHead = 0;
  size_t fifoTail = 0;
  uint16_t overrun = 0;
  uint16_t maxOverrun = 0;
  uint32_t totalOverrun = 0;
  uint32_t fifoBuf[128 * FIFO_SIZE_SECTORS];

  #define  DEF_FILE_NAME 0
  #define  SDCARD_INIT 1
  #define  BINFILE_CREATE 2
  #define  CSVFILE_CREATE  3
  #define  SDCARD_PROBLEM  7
  #define  BINFILE_FAIL  8
  #define  CSVFILE_FAIL  9  
  
// 0 do nothing
// 1 pre-start logging display and other preparation those functions should se it 2 when done // should set to 1 with enter command 
// 2 start logging // create bin file ans start  
// 3 stop logging // create csv file than bin 2 csv than set to 4  
// 4 log stopped for display and others those functions should set  it 0 when done 


 // 0  
 // 1 sd initilised
 // 2 binary file created 
 // 3 csv  file created 

 // 7 sd failed
 // 8 bin failed 
 // 9 csv failed
  
  
  uint8_t Esc_Count;
  

struct SensorIMU
{
  float x;
  float y; // 27
  float z; //    
};

SensorIMU Accelometer;
SensorIMU Gyro;
SensorIMU Magnetometer;
float Temperature; 

float roll,pitch,rollF,pitchF=0;
float yaw,yawF = 0;

static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };
//char Disp_MENU1[] =    {'L','O','G',' ','S','T','A','R','T',' ','&',' ','S','T','O','P',' ','M','E','N','U','\0'};
static const char Disp_MENU_NULL_LOG[]PROGMEM = "                  ESC";
static const char Disp_MENU_NULL[]  PROGMEM = "ENTER             ESC";
static const char Disp_MENU_ERROR[] PROGMEM = " KEY ERROR !         ";
static const char Disp_MENU1[] PROGMEM =      "LOG START & STOP MENU";
static const char Disp_MENU2[] PROGMEM =      "LOG SAMPLE TIME MENU ";
static const char Disp_MENU3[] PROGMEM =      "DISPLAY STANDBYE MENU";
static const char Disp_MENU4[] PROGMEM =      "INFORMATION MENU";
static const char Disp_MENU5[] PROGMEM =      "DATE & TIME ADJ. MENU";
static const char Disp_MENU6[] PROGMEM =      "ENERGY METER MENU    ";
static const char Disp_MENU7[] PROGMEM =      "LOG FILE MAXSIZE MENU";

static const char Disp_MENU5_SUB7[] PROGMEM = "Date & Time Updated !"; 
static const char Disp_MENU6_SUB1[] PROGMEM = "Start Calib? 60 Secs."; 
static const char Disp_MENU6_SUB2[] PROGMEM = "Sure To Start Calib.?"; 
static const char Disp_MENU6_SUB3[] PROGMEM = "Calibration Started !"; 

static const char Disp_MENU6_SUB4[] PROGMEM = "EU/US Sel. Active:";
static const char Disp_MENU6_SUB42[] PROGMEM =                   " EU"; 
static const char Disp_MENU6_SUB43[] PROGMEM =                   " US"; 
static const char Disp_MENU6_SUB5[] PROGMEM = "EU  230V / 50 Hz  ?  "; 
static const char Disp_MENU6_SUB6[] PROGMEM = "US  110V / 60 Hz  ?  ";
static const char Disp_MENU6_SUB7[] PROGMEM = " Updated To";
static const char Disp_MENU6_SUB71[] PROGMEM =            " 50 Hz    ";
static const char Disp_MENU6_SUB72[] PROGMEM =            " 60 Hz    ";



//static const char Disp_MENU7_SUB1[] PROGMEM =      "Active-> "; //9
static const char Disp_MENU7_SUB1[] PROGMEM =      "500 Kbytes  ";
static const char Disp_MENU7_SUB2[] PROGMEM =      "1 Mbytes    ";
static const char Disp_MENU7_SUB3[] PROGMEM =      "2 Mbytes    ";
static const char Disp_MENU7_SUB4[] PROGMEM =      "4 Mbytes    ";
static const char Disp_MENU7_SUB5[] PROGMEM =      "8 Mbytes    ";
static const char Disp_MENU7_SUB6[] PROGMEM =      "16 Mbytes   ";
static const char Disp_MENU7_SUB7[] PROGMEM =      "32 Mbytes   ";

static const char Disp_MENU_Enter[] PROGMEM =     "Enter -> ";
static const char Disp_MENU_Activ[] PROGMEM =     "Active.. ";
static const char Disp_MENU7_SUB8[] PROGMEM =      " Sure?  If Yes Enter ";
static const char Disp_MENU7_SUB9[] PROGMEM =      "File Size Updated !  ";
/*
static const char Disp_MENU7_SUB12[] PROGMEM =      " 500 KBytes "; //12
static const char Disp_MENU7_SUB13[] PROGMEM =      "  1 MBytes  "; //9
static const char Disp_MENU7_SUB14[] PROGMEM =      "  2 MBytes  "; //9
static const char Disp_MENU7_SUB15[] PROGMEM =      "  4 MBytes  "; //9
static const char Disp_MENU7_SUB16[] PROGMEM =      " 10 MBytes  "; //9
static const char Disp_MENU7_SUB17[] PROGMEM =      " 20 MBytes  "; //9
*/
static const char Disp_MENU1_SUB1[] PROGMEM = "LOG START            "; 
static const char Disp_MENU1_SUB2[] PROGMEM = "LOG STOP             "; 
static const char Disp_MENU1_SUB3[] PROGMEM = "LOG Started !        ";
static const char Disp_MENU1_SUB3_FAST[] PROGMEM = "LOG Starting !       ";
static const char Disp_MENU1_SUB4[] PROGMEM = "LOG Stopped !        "; 

static const char Disp_MENU2_SUB[] PROGMEM = "Enter -> ";  //9

  static const char LOG_1MSEC[]   PROGMEM = "  1 mS"; //12
  static const char LOG_5MSEC[]   PROGMEM = "  5 mS"; //12  
  static const char LOG_10MSEC[]  PROGMEM = " 10 mS"; //12
  static const char LOG_20MSEC[]  PROGMEM = " 20 mS"; //12  
  static const char LOG_50MSEC[]  PROGMEM = " 50 mS"; //12
  static const char LOG_100MSEC[] PROGMEM = "100 mS"; //12
  static const char LOG_250MSEC[] PROGMEM = "250 mS"; //12
  static const char LOG_500MSEC[] PROGMEM = "500 mS"; //12  
  static const char LOG_1SEC[]    PROGMEM = " 1 Sec"; //12
  static const char LOG_2SEC[]    PROGMEM = " 2 Sec"; //12  
  static const char LOG_5SEC[]    PROGMEM = " 5 Sec"; //12
  static const char LOG_10SEC[]   PROGMEM = " 10Sec"; //12
  static const char LOG_20SEC[]   PROGMEM = " 20Sec"; //12
  static const char LOG_60SEC[]   PROGMEM = " 60Sec"; //12   
  
#if defined (ARDUINO_MEGA) | defined (ARDUINO_DUE) 


  static const char Disp_MENU2_SUB01[] PROGMEM = "  1 mSec    "; //12
  static const char Disp_MENU2_SUB02[] PROGMEM = "  5 mSec    "; //12
  static const char Disp_MENU2_SUB03[] PROGMEM = " 10 mSec    "; //12
  static const char Disp_MENU2_SUB04[] PROGMEM = " 20 mSec    "; //12
  static const char Disp_MENU2_SUB05[] PROGMEM = " 50 mSec    "; //12
  static const char Disp_MENU2_SUB06[] PROGMEM = " 100 mSec   "; //12
  static const char Disp_MENU2_SUB07[] PROGMEM = " 250 mSec   "; //12
  static const char Disp_MENU2_SUB08[] PROGMEM = " 500 mSec   "; //12
  static const char Disp_MENU2_SUB09[] PROGMEM = " 1 Sec      "; //12
  static const char Disp_MENU2_SUB10[] PROGMEM =" 2 Sec      "; //12
  static const char Disp_MENU2_SUB11[] PROGMEM =" 5 Sec      "; //12
  static const char Disp_MENU2_SUB12[] PROGMEM =" 10 Sec     "; //12   
  static const char Disp_MENU2_SUB13[] PROGMEM =" 20 Sec     "; //12
  static const char Disp_MENU2_SUB14[] PROGMEM =" 60 Sec     "; //12    
  
#endif
#ifdef STM32_F407 // 10msec
  static const char Disp_MENU2_SUB01[] PROGMEM = "  1 mSec    "; //12
  static const char Disp_MENU2_SUB02[] PROGMEM = "  5 mSec    "; //12
  static const char Disp_MENU2_SUB03[] PROGMEM = " 10 mSec    "; //12
  static const char Disp_MENU2_SUB04[] PROGMEM = " 20 mSec    "; //12
  static const char Disp_MENU2_SUB05[] PROGMEM = " 50 mSec    "; //12
  static const char Disp_MENU2_SUB06[] PROGMEM = " 100 mSec   "; //12
  static const char Disp_MENU2_SUB07[] PROGMEM = " 250 mSec   "; //12
  static const char Disp_MENU2_SUB08[] PROGMEM = " 500 mSec   "; //12
  static const char Disp_MENU2_SUB09[] PROGMEM = " 1 Sec      "; //12  // max for STM32
  
  static const char Disp_MENU2_SUB10[] PROGMEM =" 2 Sec      "; //12
  static const char Disp_MENU2_SUB11[] PROGMEM =" 5 Sec      "; //12
  static const char Disp_MENU2_SUB12[] PROGMEM =" 10 Sec     "; //12   
//  static const char Disp_MENU2_SUB13[] PROGMEM =" 20 Sec     "; //12
//  static const char Disp_MENU2_SUB14[] PROGMEM =" 60 Sec     "; //12    
#endif  
static const char Disp_MENU2_SUB13[] PROGMEM = " Sure?  If Yes Enter ";
static const char Disp_MENU2_SUB14[] PROGMEM = "Sample Time Updated !";

static const char Disp_MENU3_SUBMAIN[] PROGMEM = "STANDBYE ";  
static const char Disp_MENU3_SUB1[] PROGMEM = "Enable "; 
static const char Disp_MENU3_SUB2[] PROGMEM = "Disable";  
static const char Disp_MENU3_SUB3[] PROGMEM = "Updated! On";
static const char Disp_MENU3_SUB4[] PROGMEM = "Updated! Off";
                                           // "LOG START & STOP MENU";
//uId 756E6B77.6F000C05 ->17
//static const char Disp_MENU4_SUB3[] PROGMEM = "Term:115200 8n1 NL&CR";
static const char Disp_MENU4_SUB3_1[] PROGMEM = "uId ";
static const char Disp_MENU4_SUB4[] PROGMEM = "Term:115200 8n1 NL&CR";
static const char SD1_CARD[] PROGMEM = "SD1 Card "; 
static const char SD2_CARD[] PROGMEM = "SD2 Card ";
static const char SDHC_CARD[] PROGMEM ="SDHC Card "; 
static const char NO_FAT[]  PROGMEM   ="Fat Problem    !";
static const char SD_CARD_ERR[] PROGMEM ="Card Problem    !";

static const char SETTINGUP[] PROGMEM     = "SettingUp"; //21-3
static const char  CALIBRATING[] PROGMEM  = "Calibrating";
static const char  ICERROR[]   PROGMEM    = " IC Error";

static const char  FASTLOG_LINE3[] PROGMEM = "                     "; 
static const char  FASTLOG_LINE4[] PROGMEM = "                     ";
static const char  FASTLOG_LINE5[] PROGMEM = "   DISPLAY FROZEN !  ";
static const char  FASTLOG_LINE6[] PROGMEM = "   ESC KEY TO STOP   ";
//static const char  FASTLOG_LINE7[] PROGMEM = "   5 TIMES TO STOP!  ";  

static const char  EX_LOG_MESS0[] PROGMEM = "                     "; 
static const char  EX_LOG_MESS1[] PROGMEM = " SD Card Initilised  ";
static const char  EX_LOG_MESS2[] PROGMEM = " Binary File Created ";
static const char  EX_LOG_MESS3[] PROGMEM = " CSV File Created    ";
static const char  EX_LOG_MESS4[] PROGMEM = " SD Card Problem !!  ";  
static const char  EX_LOG_MESS5[] PROGMEM = "Binary File Failed !!";
static const char  EX_LOG_MESS6[] PROGMEM = " Csv File Failed !!  ";
static const char  EX_LOG_MESS7[] PROGMEM = "  ??????????????     ";
static const char  EX_LOG_MESS8[] PROGMEM = "   5 TIMES TO STOP!  ";  

static const char  MESSAGE_LOG_START[]    PROGMEM = " Log Started! "; 
static const char  MESSAGE_LOGBINCREATE[] PROGMEM = "Bin File Done!"; 
static const char  MESSAGE_LOOPINACTION[] PROGMEM = "On Going Log! ";
static const char  MESSAGE_LOOPENDSCUCC[] PROGMEM = "Log Completed ";
static const char  MESSAGE_LOOPENDFAIL_[] PROGMEM = "SessionFailed!";
static const char  MESSAGE_LOGT2CSVCRET[] PROGMEM = "Convert ->CSV!"; 

static const char  MESSAGE_LOG_BIN_FAIL[] PROGMEM = "Bin File Fail!";
static const char  MESSAGE_LOG_REC_FAIL[] PROGMEM = "Log Rec. Fail!";
static const char  MESSAGE_LOG_2CSV_FAIL[]PROGMEM = "CSV File Fail!"; 




static const char  Repository[] PROGMEM = "github.com/ilkerya/AlphaCent";
static const char  Terminal_1[] PROGMEM = "For Adjusting date&time send as below format";
static const char  Terminal_2[] PROGMEM = "Year,Month,Date,Hour,Minute;Second";
static const char  Terminal_3[] PROGMEM = "2020,05,27,21,14,23";
static const char  Terminal_4[] PROGMEM = "For Adjusting serialID send as below format";
static const char  Terminal_5[] PROGMEM = "EEEExxxx as4 digit HEX format";
const char *const Terminal_table[] PROGMEM = {Terminal_1, Terminal_2, Terminal_3, Terminal_4, Terminal_5};

struct CycleCounter
{
  uint32_t StartCount = 0;
  uint32_t EndCount = 0;
};
CycleCounter SD_Counter ;
CycleCounter Int_Counter ;



#ifdef ENERGYMETER_EXISTS
struct EnergyIcVariables
{
  uint8_t Mode = 0;
  uint8_t Timer = 0;
  bool Error = 0;
  bool commscheck;  
};
EnergyIcVariables EnergyMeterIC ;

#define FREQUENCY_50HZ 0x0000 
#define FREQUENCY_60HZ 0x0010 

uint16_t  Mains_Frequency = FREQUENCY_50HZ;

#endif  

      uint32_t PR_Counter_Current  = 0; 
       uint32_t PR_Counter_Prev  = 0;  

// so variables
                     
String Display_Line1 ="Display.........Line1"; 
String Display_Line2 ="Display........Line2."; 
String Display_Line3 ="Display.......Line3.."; 
String Display_Line4 ="Display......Line4..."; 
String Display_Line5 ="Display.....Line5....";
String Display_Line6 ="Display....Line6.....";
String Display_Line7 ="Display...Line7......";
String Display_Line8 ="Display..Line8.......";


uint8_t MainMenu =0;
uint8_t DispRollIndex[4] = {1,0,0,0};

//2 1 0 0
//3 2 1 0
//4 3 2 1
//5 4 3 2
//6 5 4 3
//7 6 5 4
//8 7 6 5
//9 8 7 6
//1 9 8 7
//2 1 9 8 
//3 2 1 9  
//4 3 2 1  
float RL1Min, RL1Max, RL2Min,RL2Max,CompValue;
String RLlVal, RL2Val;
String Relay1str, RlStr2, RlStr4, Relay2str, RlStr6,  RlStr8;
#define ELEMENTS 12
 const String KeyWords[12]   = {"Tmp1","Tmp2","Tmp3","Hum1","Hum2","Hum3","PM25","PM10","Cur","Volt","Powr","PF",};

struct Various{
  uint8_t MaxFileSizeSure;
  uint32_t SampleTime;
};
Various KeySupport;

struct TaskOrg
{
  uint8_t  FastLog_Counter =0;
  uint8_t  FastLog_DelayTimer = 0;
  uint16_t  IntTimer_10m = 0; 
  uint16_t  IntTimer_20m = 0;  
  uint16_t  IntTimer_50m = 0;
  uint16_t  IntTimer_100m = 0;  
  uint16_t  IntTimer_250m = 0;
  uint16_t  IntTimer_500m = 0;
  uint16_t  IntTimer_1 = 0;
  uint16_t  IntTimer_2 = 0;
  uint16_t  IntTimer_5 = 0;
  uint16_t  IntTimer_10 = 0;
  uint16_t  IntTimer_20 = 0;
  uint16_t  IntTimer_60 = 0;
  bool Task_10msec =0; 
  bool Task_20msec =0; 
  bool Task_50msec =0;
  bool Task_100msec =0;
  bool Task_250msec =0;
  bool Task_500msec =0;
  bool Task_1Sec =0;
  bool Task_2Sec =0;
  bool Task_5Sec =0;
  bool Task_10Sec =0;
  bool Task_20Sec =0;
  bool Task_60Sec =0;

    
};

 TaskOrg Loop;




uint32_t SampleTime = TASK_10MSEC; // 250msec 1 // 500 2 // 1Sec 4 // 2sec 8 // 5sec 16 // 10sec 32 // 20sec 64  

// the logging file
//File logfile;
String dataString = "";
//char LOG_FILE[] =  "LOG_xxxx.csv";
char LOG_FILE[] =  "Lxxxx_01.csv";
const char ConfigFile[] PROGMEM =  "RLConfig.txt";
char Device_Id[5] = {'0','0','0','0','\0'}; 
char File_Que[3] = {'0','0','\0'}; 
String Config_Str = "";

struct SDCard_Variables
{
  bool LogEnable;  // log on or off  eski LogStatus
  bool LogBootInit; // log on  init case  eski LogStatusInit
  bool Pause;  // At SD Problem Try Again
  uint16_t PauseTimer;
  uint16_t PauseCount;  
  uint8_t Status; 
  bool FatError;
  float Volume;
};

SDCard_Variables SDCard;

struct FileSize_Variables
{
  uint32_t Total;  // log on or off
  uint32_t MaxSize;
  uint8_t Byte; // log on  init case
  uint8_t Kbyte;
  uint8_t Mbyte;
};

FileSize_Variables FileSize;


uint16_t CounterSync_Main = 0;
//uint16_t CounterSync_Int = 0;
//uint16_t  CounterDev_Int = 0;


struct Key_Variables{
  uint32_t Adc = 0;
  uint16_t Logger=0;
  uint8_t BoardTimeOut = 0;
  bool BoardTimeOutEnb = 0;  
  bool Left_Press = 0;
  bool Down_Press = 0;
  bool Right_Press = 0;
  bool Up_Press = 0; 
  bool Left_Rel = 0;
  bool Down_Rel = 0;
  bool Right_Rel = 0;
  bool Up_Rel = 0;  
  bool Error = 0;  
  bool Released = 0;
  uint8_t TimeOut = 0;
  bool TimeOutEnb = 0;
};

Key_Variables Key;

struct  Sensors{
  uint32_t OnBoard;
  uint32_t No1;
  uint32_t No2;
  uint32_t No3;    
} ;

Sensors SensorId;
//2874

//String Sensor_Info_SDS= "";

//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesdy", "Wedns.", "Thurs.", "Friday", "Satur."};
//static const char SD_Type[4][14] = {"SD1 Card " , "SD2 Card " , "SDHC Card " , "Card Problem    !"};


const uint8_t numChars = 32;
char rxarr[numChars]; // an array to store the received data
boolean newData = false;

#define BUF_LENGTH 60
uint8_t PMBuffer[BUF_LENGTH];

struct
{
  float Humidity_OnBoard;
  float Temperature_OnBoard; // 27  
  float Humidity_Ch1;
  float Temperature_Ch1; // 27
  float Humidity_Ch2;
  float Temperature_Ch2; // 27
  float Humidity_Ch3;
  float Temperature_Ch3; // 27  
  float Current;
  float Voltage;
  float PowerFactor; 
  float ActivePower;
  float Frequency;
  float Pressure; //  
  float TemperatureBMP; //  
  float Altitude; //
  float PM25=0; //
  float PM10=0; //   
  float WindRaw;   // 35        
  uint16_t WindMPH;    // 31
  uint16_t WindTemp;   // 35
  uint16_t Luminosity;  
  uint16_t Battery_Current;  
  uint16_t Battery_Voltage; 
  uint16_t Bat_Current_Adc;  
  uint16_t Bat_Voltage_Adc;   
  float Solar_Current;  
  uint16_t Solar_Voltage; 
  
  uint16_t Lidar;     
}Values;

#ifdef SOLAR_PANEL_EXISTS

  //  float sensitivity = 100.0  / 500.0; //100mA per 500mV = 0.2
     float sensitivity = 100.0  / 2500.0; //100mA per 2500mV = 0.04   
    float Vref = 2500; // Output voltage with no current: ~ 2500mV or 2.5V
    int Sol_sensorCounter = 0;
     int Sol_CurAdc = 0;    
#endif
/*
#ifdef   ACCL_GYRO_SENSOR_EXISTS 
struct
{
  float x;
  float y; // 27
  float z; //    
}Accelometer;
#endif

struct
{
  float x;
  float y; // 27
  float z; //    
}Gyro;

*/

// EEPROM ADDRESSES 


#define NVRAM_ID1 4
#define NVRAM_ID2 5
#define NVRAM_ID3 6
#define NVRAM_ID4 7
#define EE_LOGSTATUS 8
#define EE_SAMPLE 10

#define NVRAM_QUE1 14
#define NVRAM_QUE2 15
#define EE_STANDBYE 16

#define EE_FILESIZE 20
#define EE_MAINSFREQ 22

#define EE_ADE_FREQ 32 // eepROM ADRESS

#define HERTZ_50 ON
#define HERTZ_60 OFF


String Str_Time="";
String Str_Date="";
String Str_TimeLog_Init="";
String Str_DateLog_Init="";

struct
{
  uint16_t Year=0; 
  uint8_t Month=0; 
  uint8_t Day=0;                       
  uint8_t Hour=0; 
  uint8_t Minute=0; 
  uint8_t Second=0;
  uint8_t weekDay = 0;
  bool Init=0; 
  bool RTC_Update=0;   
  uint16_t Disp_UpdateTimer=0; 
  uint16_t Disp_Flash=0;     
}DateTimeBuf;


struct
{
  bool RTC_Update=0; 
  bool OLED_Init = 0 ; 
  uint8_t MenuTimeout=0; 
  uint8_t Flash=0; 
  uint16_t OLED_Timer = 0; 
  bool InitDelay = 0;
  bool SleepEnable = 0;
  uint8_t ValueTimer = 0; 
  bool ExpSensOnb =0;  
  bool ExpSens1 =0;
  bool ExpSens2 =0;
  bool ExpSens3 =0;
  uint8_t SensorRollTimer = 0;
  uint8_t ReInit_Timer = 2;
  bool ReInit_Enable = OFF;
}Display;


/*
unsigned int AD_Value1;
unsigned int AD_Value2;
unsigned int AD_Value3;

unsigned int Current_Mains_Raw;
unsigned int Current_Mains_Raw_Trim;
float Current_Mains;
float Current_MainsAverage;
float    Current_Mains_Rms ;
float    Current_Mains_Av ;
unsigned int CurrentArray[10];
byte CurrentIndexer = 0;
unsigned int TrimpotAdc ;
unsigned int Mains_Volt_Raw ;
unsigned int Mains_Volt ;
*/

/*
Display.
// for wind sensor
float zeroWindAdjustment = 0.1;             // individual adjustment parameter for wind sensor (standard = 0.1)
const double F_A_temp = 1.00857557;       // constants for calibration factors of Y=A*X+B equation for air temperature, relative humidity, and globe temperature
const double F_B_temp = -0.872682478;      // based on own calibration data - see reference on authors' website
const double F_A_hum = 1.087101959;
const double F_B_hum = -7.173821971;
const double F_A_globe = 0.946524358;
const double F_B_globe = 0.698608493;

float RV_ADunits;                       // variable for analog signal readings of wind sensor
float RV_ADunits_dev1000;               // variable for adjusted analog signal readings of wind sensor
const float T_rev_wind = 21.9;          // constants for wind speed calculation - based on own calibration data - see reference on authors' website
const float v_calc_F1 = 15.36921;
const float v_calc_F2 = 0.01788;
const float v_calc_F3 = -29.04560;
const float v_calc_F4 = 1.01536;
double velReading = 0;
double tempReading = 25;   //default     // variables for sensor readings' temporary storage


*/


  
