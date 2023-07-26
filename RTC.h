 
#define RTC_ADON_DATALOGGER
//#define RTC_STM332_NUCLEO44
 
//https://github.com/adafruit/Data-Logger-shield
// https://forum.arduino.cc/index.php?topic=523216.0
//#if defined (ARDUINO_MEGA) | defined (ARDUINO_DUE) 
//   RTC_PCF8523 rtc; // I2c Addres 0x68
//#endif
#if defined RTC_ADON_DATALOGGER
   RTC_PCF8523 rtc; // I2c Addres 0x68
#endif


 
#ifdef RTC_STM332_NUCLEO44
  #include <STM32RTC.h>
  STM32RTC& rtc = STM32RTC::getInstance();
#endif 

void SetCompile(void);

void  RTC_Read_2Buffer(void){
  #ifdef RTC_STM332_NUCLEO44
      rtc.setHours(DateTimeBuf.Hour);
      rtc.setMinutes(DateTimeBuf.Minute);
      rtc.setSeconds(DateTimeBuf.Second);
   //   rtc.setWeekDay(DateTimeBuf.weekDay);
      rtc.setDay(DateTimeBuf.Day);
      rtc.setMonth(DateTimeBuf.Month);
      rtc.setYear(DateTimeBuf.Year-2000);
#endif 
#if defined RTC_ADON_DATALOGGER
    rtc.adjust(DateTime(DateTimeBuf.Year, DateTimeBuf.Month, DateTimeBuf.Day, DateTimeBuf.Hour, DateTimeBuf.Minute, DateTimeBuf.Second));
#endif    
}

//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//static const char char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesdy", "Wedns.", "Thurs.", "Friday", "Satur."};

struct Date_Time
{
   uint16_t Year = 2023;
   uint8_t Month = 7;  
   uint8_t WeekDay = 1;
   uint8_t Day = 24;
   uint8_t Hour = 22;
   uint8_t Minute = 52;
   uint8_t Second = 30;    
} ;
Date_Time Compile;
Date_Time Current;


void RTC_TimeClock(){
    if(DateTimeBuf.RTC_Update == ON){
      DateTimeBuf.RTC_Update = OFF;
      RTC_Read_2Buffer();
      return;
    }  
  
    #ifdef RTC_STM332_NUCLEO44
    
    Year = rtc.getYear();
    Month = rtc.getMonth();  
    Day = rtc.getDay();
    Hour = rtc.getHours();
    Minute = rtc.getMinutes();
    Second = rtc.getSeconds(); 
    Year += 2000;    
  // if(Year < 2020)Year = 2020;
  // if(Year > 2030)Year = 2030;
     Str_Date = String(Year);   
    Str_Date += '-';    
    if(Month<10)Str_Date += '0';  
     Str_Date += String(Month);
     Str_Date += '-';       
    if(Day<10)Str_Date += '0';  
     Str_Date += String(Day);
     
     Str_Time = "";
    if(Hour<10)Str_Time += '0';
     Str_Time += String(Hour);
      Str_Time += ':';    
    if(Minute<10)Str_Time += '0';   
     Str_Time += String(Minute);
    Str_Time += ':';   
    if(Second<10)Str_Time += '0';   
     Str_Time += String(Second);

      if(DateTimeBuf.Init == ON){
        DateTimeBuf.Init = OFF;
        DateTimeBuf.Year = Year;
        DateTimeBuf.Month = Month;
        DateTimeBuf.Day = Day;
        DateTimeBuf.Hour = Hour;
        DateTimeBuf.Minute = Minute;
        DateTimeBuf.Second = Second;
    }
   #endif 
  #ifdef RTC_ADON_DATALOGGER
    DateTime now = rtc.now();

    Str_Date = String(now.year(),DEC);   
    Str_Date += '-';    
    if(now.month()<10)Str_Date += '0';  
     Str_Date += String(now.month(),DEC);
     Str_Date += '-';       
    if(now.day()<10)Str_Date += '0';  
     Str_Date += String(now.day(),DEC);
     
     Str_Time = "";
    if(now.hour()<10)Str_Time += '0';
     Str_Time += String(now.hour(),DEC);
      Str_Time += ':';    
    if(now.minute()<10)Str_Time += '0';   
     Str_Time += String(now.minute(),DEC);
    Str_Time += ':';   
    if(now.second()<10)Str_Time += '0';   
     Str_Time += String(now.second(),DEC);
   
    if(DateTimeBuf.Init == ON){
      DateTimeBuf.Init = OFF;
      DateTimeBuf.Year = now.year();
      DateTimeBuf.Month = now.month();
      DateTimeBuf.Day = now.day();
      DateTimeBuf.Hour = now.hour();
      DateTimeBuf.Minute = now.minute();
      DateTimeBuf.Second = now.second();
    }
   #endif 
}
static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}
    static const char* mydate = __DATE__;
    static const char* mytime = __TIME__;
  /*  
    static byte comp_seconds = 0;
    static byte comp_minutes = 0;
    static byte comp_hours = 0;
    static byte comp_day = 0;
    static byte comp_weekDay = 1;
    static byte comp_month = 0;
    static uint16_t comp_year = 0;
*/
void RTC_Default_Start(){
  // get rtc content and compare with compile data&time use the most updated one

   #ifdef RTC_STM332_NUCLEO44 
   Current.Year = rtc.getYear();
   Current.Month  = rtc.getMonth();  
   Current.Day  = rtc.getDay();
   Current.Hour  = rtc.getHours();
   Current.Minute  = rtc.getMinutes();
   Current.Second  = rtc.getSeconds(); 
   #endif 
  #ifdef RTC_ADON_DATALOGGER
    DateTime now = rtc.now();
   Current.Year  = now.year();
   Current.Month  = now.month();  
   Current.Day  = now.day();
   Current.Hour  = now.hour();
   Current.Minute  = now.minute();
   Current.Second  = now.second();    
   #endif 
                
  Serial.print("RTC Current :");
  Serial.print(Current.Year );
  Serial.print(":");Serial.print(Current.Month ); 
  Serial.print(":");Serial.print(Current.Day );
  Serial.print(":");Serial.print(Current.Hour );
  Serial.print(":");Serial.print(Current.Minute ); 
  Serial.print(":");Serial.println(Current.Second );

   
   Compile.Year  = conv2d(mydate + 9);
    #ifdef RTC_ADON_DATALOGGER
      Compile.Year  += 2000;
    #endif
         
  // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
  switch (mydate[0]) {
    case 'J': Compile.Month = (mydate[1] == 'a') ? 1 : ((mydate[2] == 'n') ? 6 : 7); break;
    case 'F': Compile.Month = 2; break;
    case 'A': Compile.Month = mydate[2] == 'r' ? 4 : 8; break;
    case 'M': Compile.Month = mydate[2] == 'r' ? 3 : 5; break;
    case 'S': Compile.Month = 9; break;
    case 'O': Compile.Month = 10; break;
    case 'N': Compile.Month = 11; break;
    case 'D': Compile.Month = 12; break;
  }
  Compile.Day = conv2d(mydate + 4);
  Compile.Hour = conv2d(mytime);
  //if (hourFormat == rtc.HOUR_12) {
  //  period = hours >= 12 ? rtc.PM : rtc.AM;
  //  hours = hours >= 13 ? hours - 12 : (hours < 1 ? hours + 12 : hours);
  // }
  Compile.Minute = conv2d(mytime + 3);
  Compile.Second= conv2d(mytime + 6);

  Serial.print("RTC Compile :");
  Serial.print(Compile.Year);
  Serial.print(":");Serial.print(Compile.Month); 
  Serial.print(":");Serial.print(Compile.Day);
  Serial.print(":");Serial.print(Compile.Hour);
  Serial.print(":");Serial.print(Compile.Minute); 
  Serial.print(":");Serial.println(Compile.Second);

  if(Current.Year < Compile.Year ){SetCompile();return;} //2023 RTC year  < compile year  2022
  if(Current.Year > Compile.Year )return;
  if(Current.Month  < Compile.Month ){SetCompile();return;}
  if(Current.Month  > Compile.Month )return;  
  if(Current.Day < Compile.Day ){SetCompile();return;} 
  if(Current.Day > Compile.Day)return;   
  if(Current.Hour < Compile.Hour ){SetCompile();return;}
  if(Current.Hour > Compile.Hour )return;  
  if(Current.Minute < Compile.Minute ){SetCompile();return;}
  if(Current.Minute > Compile.Minute )return;// 19>20 
  if(Current.Second  < Compile.Second ){SetCompile();return;}
  if(Current.Second  > Compile.Second )return; 
}
void SetCompile(void){
  Serial.println("compile date is newer than RTC current date !");
    #ifdef RTC_STM332_NUCLEO44
      rtc.setTime(Compile.Hour, Compile.Minute, Compile.Second);
      rtc.setDate(Compile.WeekDay, Compile.Day, Compile.Month, Compile.Year);
    #endif 
  
    #ifdef RTC_ADON_DATALOGGER
      rtc.adjust(DateTime(Compile.Year, Compile.Month, Compile.Day, Compile.Hour, Compile.Minute, Compile.Second)) ;

    #endif 
}
void  RTC_Init(){
  #ifdef RTC_STM332_NUCLEO44
  // Set the time
  rtc.begin(); // initialize RTC 24H format
   RTC_Default_Start();
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   // Set the time

   #endif 
    #ifdef RTC_ADON_DATALOGGER
  delay(10);
  if (! rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
    //while (1);
  }
  else {
    if (! rtc.initialized()) {
      Serial.println(F("RTC is NOT running!"));
      // following line sets the RTC to the date & time this sketch was compiled
     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      //   RTC_Test = 18;
      // This line sets the RTC with an explicit date & time, for example to set
      // January 21, 2014 at 3am you would call:
      // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }
    else{
      Serial.println(F("RTC is Running!"));
    } 

    if(Serial) { // if serial port connected adjust it from computer
    //  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); 
    //  Serial.println("RTC Time Adjusted!");
    //  RTC_Test = 18;
    }
  }
     RTC_Default_Start();
  #endif
}


 void SerialPortRx() {
  //   #if defined (ARDUINO_MEGA) | defined (ARDUINO_DUE) 
     
    static uint8_t ndx = 0;
    char endMarker = '\n';
    uint8_t Counter = 0;

        // if (Serial.available() > 0) {
     while (Serial.available() > 0 && newData == false) {
        char rc = Serial.read();
        Counter++; // 2020,05,27,21,14,23 19 characters + \0' // in total 21

        if (rc != endMarker) {
            rxarr[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
              ndx = numChars - 1;
          }
        }
        else {
          rxarr[ndx] = '\0'; // terminate the string
          ndx = 0;
          newData = true;
      }
   }
  //////////////////// end of while loop 
   if (newData == true) {
        //Serial.print(F("This just in .................................. "));
          Serial.println(rxarr);
          newData = false;            
          Serial.print(F("Counter:"));  
          Serial.println(Counter);           
          if((Counter == 21) && (rxarr[4] == ',' )&&(rxarr[7] == ',') && (rxarr[10] == ',')&&  (rxarr[13] == ',' )&& (rxarr[16] == ',' )){
              for(int i = 0; i<32; i ++ ){ // ascii 2 byte
                  rxarr[i] -= 48;          
              }         
            // uint16_t Year=0; uint8_t Month=0; uint8_t Day=0; uint8_t Hour=0; uint8_t Minute=0; uint8_t Second=0;             
              uint16_t Year  = 1000 * rxarr[0];
                Year  += 100 * rxarr[1]; 
                Year  += 10 * rxarr[2];   
                Year  += rxarr[3];         
              uint8_t Month  = 10 * rxarr[5];   Month  += rxarr[6];       
              uint8_t Day  = 10 * rxarr[8];     Day  += rxarr[9];               
              uint8_t Hour  = 10 * rxarr[11];   Hour  += rxarr[12];    
              uint8_t Minute  = 10 * rxarr[14]; Minute  += rxarr[15];       
              uint8_t Second  = 10 * rxarr[17]; Second  += rxarr[18];      

              #ifdef RTC_STM332_NUCLEO44
              rtc.setHours(Hour);
              rtc.setMinutes(Minute);
              rtc.setSeconds(Second);
          //   rtc.setWeekDay(weekDay);
              rtc.setDay(Day);
              rtc.setMonth(Month);
              rtc.setYear(Year-2000);
                 #endif 
                  #ifdef RTC_ADON_DATALOGGER
              rtc.adjust(DateTime(Year, Month, Day, Hour, Minute, Second));
                #endif       
              MainMenu = MENU5_SUB7;
              DispExtTimeout();
        }
        // FILEQUE
        if((Counter == 11) && (rxarr[0] == 'F' )&&(rxarr[1] == 'I') && (rxarr[2] == 'L')&&  (rxarr[3] == 'E' )
                                                &&(rxarr[4] == 'Q') && (rxarr[5] == 'U')&&  (rxarr[6] == 'E' )){
            EEProm_Update_FileNo();  
            return;                   
        }  
        if((Counter == 11) && (rxarr[0] == 'D' )&&(rxarr[1] == 'E') && (rxarr[2] == 'V')&& (rxarr[3] == 'I' ) &&(rxarr[4] == 'D' )){   
             EEProm_Update_DevId();                                              
             return;                      
        } 
        // DebugOff
        if((Counter == 9) && (rxarr[0] == 'D' )&&(rxarr[1] == 'e') && (rxarr[2] == 'b')&& (rxarr[3] == 'u' ) &&(rxarr[4] == 'g' )
                          &&(rxarr[5] == 'O') && (rxarr[6] == 'f')&&  (rxarr[7] == 'f' )){     
             EEProm_Update_Debug(OFF); 
             Serial.println(F("SERIAL PORT DEBUG OFF "));
             return; 
 
         }                                
         // DebugOn
        if((Counter == 8) && (rxarr[0] == 'D' )&&(rxarr[1] == 'e') && (rxarr[2] == 'b')&& (rxarr[3] == 'u' ) &&(rxarr[4] == 'g' )
                          &&(rxarr[5] == 'O') && (rxarr[6] == 'n')){     
             EEProm_Update_Debug(ON); 
             Serial.println(F("SERIAL PORT DEBUG ON "));             
             return;  
         }                            
    }
 //  #endif
 }
 void EEProm_Update_FileNo(void){
          NVRam_Write_QueNo(&rxarr[7]);                
          delay(10);
          NVRam_Read_QueNo();
          UpdateLogFileId(); 
          SDCard.LogBootInit = 0; // restart logging from 0           
}
void EEProm_Update_DevId(void){
            NVRam_Write_SerNo(&rxarr[5]); 
            delay(10);
            NVRam_Read_SerNo();
            UpdateLogFileId(); 
            SDCard.LogBootInit = 0; // restart logging from 0  
}
bool DebugState = OFF;
void EEProm_Update_Debug(bool State){
  if(State == ON)DebugState = ON;
  else DebugState = OFF;
}
 
