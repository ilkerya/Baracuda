
void KeyTimeOutCheck(void) {
  if (Key.BoardTimeOutEnb == ON) {
    if (Key.BoardTimeOut)Key.BoardTimeOut--;
    else {
      Key.BoardTimeOutEnb = OFF;
      MainMenu =  MENU_NULL;
    }
  }
}

void KeyTimeOutStart(void) {
  Key.BoardTimeOutEnb = ON;
  Key.BoardTimeOut  = 24;// 24 sec
}

void DisplayWakeUp(void) {
  if ((Display.OLED_Timer == 0) && (Display.OLED_Init == OFF)) {
    Display.OLED_Init = ON;
  }//else if(OLED_Timer)OLED_Timer += 10; // add additional time every time any key released
}

void  DispExtTimeout(void) {
  if (Display.OLED_Timer <= KEYDISP_TIMER) Display.OLED_Timer = KEYDISP_TIMER;
}
void UpdateInfoQue(uint8_t UpDown){
    #define MAXLINE 9
    #define MINLINE 1
   if (UpDown == DOWNROLL){    // down menu
     DispRollIndex[3] = DispRollIndex[2];
     DispRollIndex[2] = DispRollIndex[1];
     DispRollIndex[1] = DispRollIndex[0];
     DispRollIndex[0]++;
     if (DispRollIndex[0]  > MAXLINE) DispRollIndex[0] = MINLINE;
  } 
  if (UpDown == UPROLL){   
     DispRollIndex[0] = DispRollIndex[1];
     DispRollIndex[1] = DispRollIndex[2];
     DispRollIndex[2] = DispRollIndex[3];
     DispRollIndex[3]--;  
     if (DispRollIndex[3]  < MINLINE) DispRollIndex[3] = MAXLINE; 
   } 
}
void UpdateDispRoll(uint8_t UpDown){
    if(Display.SensorRollTimer){
      Display.SensorRollTimer--;
      return;
    }
    UpdateInfoQue(UpDown);
}
void KeySensonsorRoll(uint8_t UpDown){
      Display.SensorRollTimer = 30; // 2sec x 30 = 60 sec
      UpdateInfoQue(UpDown);      
}
void DispEnable(bool Enable, uint8_t Timer) {
  if (Enable == ON) {
    Display.SleepEnable = ON; //go sleep
    Display.OLED_Timer = Timer;
  }
  else   Display.SleepEnable = OFF;    // no sleep
}
void DispEnable_4SD_Prblm(bool Enable, uint8_t Timer) {
  if (Enable == ON) {
    //Display.SleepEnable = ON; //go sleep
    Display.OLED_Timer = Timer;
  }
  //else   Display.SleepEnable = OFF;    // no sleep
}
/*
void SetResetLog(bool Enable) {
    if(Enable == ON){
      SDCard.LogEnable = ON;
      //NVRam_Write_LogStatus(ON); 
      NVRam_Write(EE_LOGSTATUS,ON);
      
    }
    if(Enable == OFF){
      SDCard.LogEnable = OFF;
     // NVRam_Write_LogStatus(OFF);
      NVRam_Write(EE_LOGSTATUS,OFF);      
    } 
}
*/

void Startup_NV_Ram(){
// #if  defined (ARDUINO_DUE) |  defined (STM32_F407)   
    #ifdef ARDUINO_DUE // 8 bit AVR
      char serial[4] = {'F','5','6','C'}; // DUE is just flash every time compile and right fully cleaned
      char que[2] = {'0','1'};            // give default numbers otherwise need to use serial port to update
      NVRam_Write_QueNo(&que[0]);
      NVRam_Write_SerNo((char*)&serial);
    #endif

    NVRam_Read_SerNo();
    NVRam_Read_QueNo();
    UpdateLogFileNo();
    UpdateLogFileId();

            //switch(NVRam_Read_MaxFileSize()){
        switch(NVRam_Read(EE_FILESIZE)){                  
          case 0: FileSize.MaxSize = KBYTE_500; 
          break;
          case 1: FileSize.MaxSize = MBYTE_1; 
          break;
          case 2: FileSize.MaxSize = MBYTE_2;
          break;
          case 3: FileSize.MaxSize = MBYTE_4;
          break;
          case 4: FileSize.MaxSize = MBYTE_8;
          break;
          case 5:FileSize.MaxSize = MBYTE_16;
          break;           
          case 6:FileSize.MaxSize = MBYTE_32;
          break;  
          default: FileSize.MaxSize = MBYTE_2;
          break;                    
        }
        switch(NVRam_Read(EE_SAMPLE)){
           case TASK_1MSEC: SampleTime = TASK_1MSEC;
            break;           
           case TASK_5MSEC: SampleTime = TASK_5MSEC;
            break;        
           case TASK_10MSEC: SampleTime = TASK_10MSEC;
            break;           
           case TASK_20MSEC: SampleTime = TASK_20MSEC;
            break;            
           case TASK_50MSEC: SampleTime = TASK_50MSEC;
            break;   
           case TASK_100MSEC: SampleTime = TASK_100MSEC; 
            break;         
           case TASK_250MSEC: SampleTime = TASK_250MSEC; 
            break;
          case TASK_500MSEC: SampleTime = TASK_500MSEC; 
            break;
          case TASK_1SEC: SampleTime = TASK_1SEC; 
            break;
          case TASK_2SEC: SampleTime = TASK_2SEC;
            break;
          case TASK_5SEC: SampleTime = TASK_5SEC;
            break;
          case TASK_10SEC: SampleTime = TASK_10SEC;
            break;
          case TASK_20SEC: SampleTime = TASK_20SEC;
            break;           
          case TASK_60SEC: SampleTime = TASK_60SEC;
            break;  
          default: //SampleTime = TASK_1MSEC;
            break;                    
        }
   if(NVRam_Read(EE_LOGSTATUS)== OFF) SDCard.LogEnable = OFF;
   else {
         switch(SampleTime){
           case TASK_10MSEC: 
            break;  
           case TASK_20MSEC: 
            break;            
           case TASK_50MSEC: 
            break;   
           case TASK_100MSEC:  
            break;         
           case TASK_250MSEC:  
            break;
       //   case TASK_500MSEC:  
       //    break;  
          default://SDCard.LogEnable = ON;
          break;
         }      
   }

    if(NVRam_Read(EE_STANDBYE) == OFF)DispEnable(OFF,0);
    else DispEnable(ON,100); // default 
        
          #ifdef ENERGYMETER_EXISTS
    // NVRam_Read_MainsFreq();
      if(NVRam_Read(EE_ADE_FREQ) == HERTZ_60)Mains_Frequency = FREQUENCY_60HZ; //60
      else Mains_Frequency = FREQUENCY_50HZ; // 50
      #endif   
}

void NVRam_Read_QueNo() {
   #ifdef ARDUINO_MEGA // 8 bit AVR 
    File_Que[0] = (char)EEPROM.read(NVRAM_QUE1);
    File_Que[1] = (char)EEPROM.read(NVRAM_QUE2);

  #endif
   #ifdef ARDUINO_DUE // 8 bit AVR
    File_Que[0] = (char)dueFlashStorage.read(NVRAM_QUE1);
    File_Que[1] = (char)dueFlashStorage.read(NVRAM_QUE2);
   #endif
   #ifdef STM32_F407 // 8 bit AVR
    File_Que[0] =  NVRam_Check_Content((char)EEPROM.read(NVRAM_QUE1),NUMBER_ONLY);
    File_Que[1] =  NVRam_Check_Content((char)EEPROM.read(NVRAM_QUE2),NUMBER_ONLY);   
   #endif
   
   Serial.print(F("File_Que: ")) ;
   Serial.println(File_Que);  
}
void NVRam_Write_QueNo(char* p) {
  Serial.print(F("File_Que:"));
  #ifdef ARDUINO_MEGA // 8 bit AVR
          EEPROM.write(NVRAM_QUE1, p[0]);// high byte
          EEPROM.write(NVRAM_QUE2, p[1]);// high byte                         
    #endif
    #ifdef ARDUINO_DUE
          dueFlashStorage.write(NVRAM_QUE1, p[0]);// high byte
          dueFlashStorage.write(NVRAM_QUE2, p[1]);// high byte                 
    #endif
       #ifdef STM32_F407 // 8 bit AVR
          EEPROM.write(NVRAM_QUE1, p[0]);// high byte
          EEPROM.write(NVRAM_QUE2, p[1]);// high byte    
   #endif
         Serial.println(p[0]);Serial.println(p[1]);
}

void NVRam_Read_SerNo() {
   #ifdef ARDUINO_MEGA // 8 bit AVR 
    Device_Id[0] = (char)EEPROM.read(NVRAM_ID1);
    Device_Id[1] = (char)EEPROM.read(NVRAM_ID2);
    Device_Id[2] = (char)EEPROM.read(NVRAM_ID3);
    Device_Id[3] = (char)EEPROM.read(NVRAM_ID4);
  #endif
   #ifdef ARDUINO_DUE // 8 bit AVR
    Device_Id[0] = (char)dueFlashStorage.read(NVRAM_ID1);
    Device_Id[1] = (char)dueFlashStorage.read(NVRAM_ID2);
    Device_Id[2] = (char)dueFlashStorage.read(NVRAM_ID3);
    Device_Id[3] = (char)dueFlashStorage.read(NVRAM_ID4);
   #endif
      #ifdef STM32_F407 // 8 bit AVR
    Device_Id[0] =  NVRam_Check_Content((char)EEPROM.read(NVRAM_ID1),LETTER_NUMBER);
    Device_Id[1] =  NVRam_Check_Content((char)EEPROM.read(NVRAM_ID2),LETTER_NUMBER);
    Device_Id[2] =  NVRam_Check_Content((char)EEPROM.read(NVRAM_ID3),LETTER_NUMBER);
    Device_Id[3] =  NVRam_Check_Content((char)EEPROM.read(NVRAM_ID4),LETTER_NUMBER);   
   #endif
   Serial.print(F("Device_Id: ")) ;
   Serial.println(Device_Id);  
}
void NVRam_Write_SerNo(char* p) {
  Serial.print(F("EECode: "));
  #ifdef ARDUINO_MEGA // 8 bit AVR
          EEPROM.write(NVRAM_ID1, p[0]);// high byte
          EEPROM.write(NVRAM_ID2, p[1]);// high byte                 
          EEPROM.write(NVRAM_ID3, p[2]);// high byte         
          EEPROM.write(NVRAM_ID4, p[3]);// high byte         
    #endif
    #ifdef ARDUINO_DUE
          dueFlashStorage.write(NVRAM_ID1, p[0]);// high byte
          dueFlashStorage.write(NVRAM_ID2, p[1]);// high byte                
          dueFlashStorage.write(NVRAM_ID3, p[2]);// high byte        
          dueFlashStorage.write(NVRAM_ID4, p[3]);// high byte         
    #endif
       #ifdef STM32_F407 // 8 bit AVR
          EEPROM.write(NVRAM_ID1, p[0]);// high byte
          EEPROM.write(NVRAM_ID2, p[1]);// high byte                 
          EEPROM.write(NVRAM_ID3, p[2]);// high byte         
          EEPROM.write(NVRAM_ID4, p[3]);// high byte  
   #endif
         Serial.print(p[0]);Serial.print(p[1]);Serial.print(p[2]);Serial.println(p[3]);
}

char NVRam_Check_Content(char Content, bool Number){
   char AsciiTable[] =  {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','a','b','c','d','e','f'};
   #define NUMBER_SIZE 10
   #define LETTER_NUMBER_SIZE 22   
   uint8_t Size;
   if (Number == LETTER_NUMBER)Size = LETTER_NUMBER_SIZE;// number and letter 
   if (Number == NUMBER_ONLY) Size = NUMBER_SIZE; // only numbers 
   for(uint8_t i=0; i<Size; i++){
       if(Content == AsciiTable[i])return Content;
   } 
   //return '0'; 
   if (Number == LETTER_NUMBER)  return AsciiTable[random(LETTER_NUMBER_SIZE)];
   if (Number == NUMBER_ONLY)    return AsciiTable[random(NUMBER_SIZE)];   
}

/*
void NVRam_Write_LogStatus(bool Mode) { 
    #ifdef ARDUINO_MEGA
      if (Mode == OFF)EEPROM.write(EE_LOGSTATUS, OFF); // OFF
      else EEPROM.write(EE_LOGSTATUS, ON);// ON
    #endif
    #ifdef ARDUINO_DUE
      if (Mode == OFF)dueFlashStorage.write(EE_LOGSTATUS, OFF); // OFF
      else dueFlashStorage.write(EE_LOGSTATUS, ON);// ON
    #endif    
    #ifdef CHIPKIT_MAX32 // 32 bit ARM
      Mode = 0;
    #endif 
      
}
*/

/*
#define EE_MAINS_FREQ 32 // eepROM ADRESS
#define HERTZ_50 ON
#define HERTZ_60 OFF
*/
/*
void NVRam_Write_MainsFreq(bool Mode) {
      if(Mode == HERTZ_60)Mains_Frequency = FREQUENCY_60HZ; //60
      if(Mode == HERTZ_50)Mains_Frequency = FREQUENCY_50HZ; // 50
    #ifdef ARDUINO_MEGA
      if (Mode == HERTZ_60)EEPROM.write(EE_ADE_FREQ, HERTZ_60); // OFF
      else EEPROM.write(EE_ADE_FREQ, HERTZ_50);// ON
    #endif
    #ifdef ARDUINO_DUE
      if (Mode == HERTZ_60)dueFlashStorage.write(EE_ADE_FREQ, HERTZ_60); // OFF
      else dueFlashStorage.write(EE_ADE_FREQ, HERTZ_50);// ON
    #endif 
    #ifdef CHIPKIT_MAX32 // 32 bit ARM
      Mode = 0;
    #endif       
}

void NVRam_Read_MainsFreq(void) {
    #ifdef ARDUINO_MEGA
        uint8_t Read = EEPROM.read(EE_ADE_FREQ);// OFF
    #endif
     #ifdef ARDUINO_DUE
      uint8_t Read =dueFlashStorage.read(EE_ADE_FREQ);
    #endif
     #ifdef ARDUINO_MKRZERO
        uint8_t Read = 0;  
      #endif
       #ifdef CHIPKIT_MAX32 // 32 bit ARM
       uint8_t Read = 0;
    #endif 
      if(Read == HERTZ_60)Mains_Frequency = FREQUENCY_60HZ; //60
      if(Read == HERTZ_50)Mains_Frequency = FREQUENCY_50HZ; // 50
}
*/
  
/*
void NVRam_Write_MaxFileSize(uint8_t Size) {

    #ifdef ARDUINO_MEGA
        EEPROM.write(EE_FILESIZE, Size);
    #endif
    #ifdef ARDUINO_DUE
      dueFlashStorage.write(EE_FILESIZE, Size); // OFF
    #endif 
    #ifdef CHIPKIT_MAX32 // 32 bit ARM
      Mode = 0;
    #endif       
}
uint8_t NVRam_Read_MaxFileSize(void) {
    #ifdef ARDUINO_MEGA
        uint8_t Read = EEPROM.read(EE_FILESIZE);// OFF
    #endif
     #ifdef ARDUINO_DUE
      uint8_t Read =dueFlashStorage.read(EE_FILESIZE);
    #endif
     #ifdef ARDUINO_MKRZERO
        uint8_t Read = 0;  
      #endif
       #ifdef CHIPKIT_MAX32 // 32 bit ARM
       uint8_t Read = 0;
    #endif 
    return Read;
}


void NVRam_Write_Standbye(bool Mode) {
    #ifdef ARDUINO_MEGA
      if (Mode == OFF)EEPROM.write(SLEEP_LOG, OFF); // OFF
      else EEPROM.write(SLEEP_LOG, ON);// ON
    #endif
    #ifdef ARDUINO_DUE
      if (Mode == OFF)dueFlashStorage.write(SLEEP_LOG, OFF); // OFF
      else dueFlashStorage.write(SLEEP_LOG, ON);// ON
    #endif 
    #ifdef CHIPKIT_MAX32 // 32 bit ARM
      Mode = 0;
    #endif       
}

void NVRam_Read_Standbye(void) {
    #ifdef ARDUINO_MEGA
        uint8_t Read = EEPROM.read(SLEEP_LOG);// OFF
    #endif
     #ifdef ARDUINO_DUE
      uint8_t Read =dueFlashStorage.read(SLEEP_LOG);
    #endif
     #ifdef ARDUINO_MKRZERO
        uint8_t Read = 0;  
      #endif
       #ifdef CHIPKIT_MAX32 // 32 bit ARM
       uint8_t Read = 0;
    #endif 
      if(Read == OFF)DispEnable(OFF,0);
      if(Read == ON)DispEnable(ON,100); // default 
}
*/
void NVRam_Write(uint8_t Address, uint8_t Sample){
  #ifdef ARDUINO_MEGA
      EEPROM.write(Address, Sample);// ON  
  #endif
     #ifdef ARDUINO_DUE 
    dueFlashStorage.write(Address,Sample); 
  #endif
     #ifdef CHIPKIT_MAX32 // 32 bit ARM
       
    #endif  
    #ifdef STM32_F407  
     EEPROM.write(Address, Sample);
    #endif        
}
uint8_t NVRam_Read(uint8_t Address) {  //
   #ifdef ARDUINO_MEGA // 8 bit AVR
    return EEPROM.read(Address);// OFF
    #endif
   #ifdef ARDUINO_DUE // 32 bit ARM
       return dueFlashStorage.read(Address);
    #endif
     #ifdef ARDUINO_MKRZERO
       // uint8_t Mode = 0;  
      #endif    
   #ifdef CHIPKIT_MAX32 // 32 bit ARM
      // uint8_t Mode = 0;
    #endif   
      #ifdef STM32_F407  
      return EEPROM.read(Address);// OFF
    #endif    
    /* 
    if((Mode== TASK_500MSEC)||(Mode== TASK_1SEC)||(Mode== TASK_2SEC)||(Mode== TASK_5SEC)||(Mode== TASK_10SEC)||(Mode== TASK_20SEC)||(Mode== TASK_60SEC)){
      SDCard.LogEnable = ON; 
      SampleTime =  Mode;
    }
    else{
      //SDCard.LogEnable = OFF; 
      SampleTime =  TASK_2SEC; // default
    }  
    */     
}

void UpdateLogFileNo(void){
  uint16_t No = (File_Que[0]-48)*10;
  No += (File_Que[1]-48);

  Serial.print(F("    No: "));
  Serial.println(No);
  if(No >= 100){
      File_Que[0] =  '0';
      File_Que[1] =  '1';  
      NVRam_Write_QueNo(&File_Que[0]);
  }
}

void UpdateLogFileId(void){
    LOG_FILE[1] = Device_Id[0];
    LOG_FILE[2] = Device_Id[1];
    LOG_FILE[3] = Device_Id[2];
    LOG_FILE[4] = Device_Id[3];
    
    LOG_FILE[6] =  File_Que[0];   
    LOG_FILE[7] =  File_Que[1];  
    
    Serial.print(F("LOG_FILE: ")) ;
    Serial.println(LOG_FILE);
}
void ResetCasePrint() {
#ifdef ARDUINO_MEGA // 8 bit AVR 
    
  uint8_t RESET_CASE = MCUSR;
  Serial.print(F("MCUSR: "));
  Serial.println(RESET_CASE);
  if (MCUSR & WDRF) {
    Serial.print(F("Rebooting from a Watchdog Reset"));
  }
  else if (MCUSR & BORF) {
    Serial.print(F("Rebooting from a Brown-out Reset"));
  }
  else if (MCUSR & EXTRF) {
    Serial.print(F("Rebooting from an External Reset"));
  }
  else if (MCUSR & PORF) {
    Serial.print(F("Rebooting from a Power On Reset"));
  }
  if (MCUSR & (1 << WDRF)) {
    // a watchdog reset occurred
    Serial.print(F("Rebooting from a Watchdog Reset"));
  }
  if (MCUSR & (1 << BORF)) {
    Serial.print(F("Rebooting from a Brown-out Reset"));
  }
  //  a brownout reset occurred
  if (MCUSR & (1 << EXTRF)) {
    //  an external reset occurred
    Serial.print(F("Rebooting from an External Reset"));
  }
  if (MCUSR & (1 << PORF)) {
    //  a power on reset occurred
    Serial.print(F("Rebooting from a Power On Reset"));
  }
  //Clear register
  MCUSR = 0x00;

    #endif
}
uint8_t KeyCounterFast = 0;

bool FastLog_Terminate(void){
      if(Key.Up_Press || (Key.Down_Press))KeyCounterFast++;
      if((Key.Left_Press) || (Key.Right_Press)) KeyCounterFast = 0;
 
      if(KeyCounterFast > 4){   
        KeyCounterFast = 0;    
     
        SDCard.LogEnable = OFF;
        NVRam_Write(EE_LOGSTATUS,SDCard.LogEnable);    
        MainMenu =  MENU1_SUB4;//MENU1
        
        return OFF;
      }
      else return ON;

}

#ifdef KEY_ANALOG
void Key_Functions_Analog(uint32_t Adc) {
    Key.Adc = Adc;
 
#ifdef ARDUINO_DUE // 
      Key.Adc >>= 2; 
    //  if(Key.Adc < 990) {
        Key.Adc *= 66;
        Key.Adc /= 100;        
   //   }
#endif

#if defined (ARDUINO_MEGA)  | defined (ARDUINO_DUE) 

    #define KEY_DOWN_ADC_HIGH 588
    #define KEY_DOWN_ADC_LOW 564   

    #define KEY_LEFT_ADC_HIGH 478
    #define KEY_LEFT_ADC_LOW 440

    #define KEY_RELEASED_ADC_HIGH 404
    #define KEY_RELEASED_ADC_LOW 384   

    #define KEY_UP_ADC_HIGH 358
    #define KEY_UP_ADC_LOW 334   

    #define KEY_RIGHT_ADC_HIGH 110
    #define KEY_RIGHT_ADC_LOW 84   

#endif

#if defined (STM32_F407)

    #define KEY_DOWN_ADC_HIGH 767 //757
    #define KEY_DOWN_ADC_LOW 747   
    #define KEY_LEFT_ADC_HIGH 612  //602
    #define KEY_LEFT_ADC_LOW 592
    #define KEY_RELEASED_ADC_HIGH 521 //511
    #define KEY_RELEASED_ADC_LOW 501   
    #define KEY_UP_ADC_HIGH 465 //455
    #define KEY_UP_ADC_LOW 445   
    #define KEY_RIGHT_ADC_HIGH 155 //145
    #define KEY_RIGHT_ADC_LOW 135   
#endif      
    Key.Down_Press = OFF;
    Key.Right_Press = OFF;
    Key.Up_Press = OFF;
    Key.Left_Press = OFF;
    if((Key.Adc < KEY_RELEASED_ADC_HIGH) &&(Key.Adc > KEY_RELEASED_ADC_LOW)){
      Key.Released = ON;
      Key.Error = OFF;
      return;  
    }
    else if((Key.Adc < KEY_RIGHT_ADC_HIGH) && (Key.Adc > KEY_RIGHT_ADC_LOW))Key.Right_Press = ON;
    else if((Key.Adc < KEY_UP_ADC_HIGH) && (Key.Adc > KEY_UP_ADC_LOW))Key.Up_Press = ON;
    else if((Key.Adc < KEY_DOWN_ADC_HIGH) && (Key.Adc > KEY_DOWN_ADC_LOW))Key.Down_Press = ON;
    else if((Key.Adc < KEY_LEFT_ADC_HIGH) && (Key.Adc > KEY_LEFT_ADC_LOW))Key.Left_Press = ON;   
    else  {
      Key.Error = ON;
      return;
    }
    Key.Error = OFF;
    
  if (!Key.Released)return;
  Key.Released = OFF;
  DisplayWakeUp();

 // if(!FastLog_Terminate()){
  /* //TEST
   if (Key.Left_Press) Serial.println(F("EnterMenuKey"));
   if (Key.Down_Press) Serial.println(F("DownMenuKey"));
   if (Key.Right_Press) Serial.println(F("EscMenuKey"));
   if (Key.Up_Press) Serial.println(F("UpMenuKey"));          
    */
    if (Key.Left_Press) EnterMenuKey();
    if (Key.Down_Press) DownMenuKey();
    if (Key.Right_Press) EscMenuKey();
    if (Key.Up_Press)   UpMenuKey();
 // }  
    KeyTimeOutStart();
}
#endif

#ifdef KEY_DIGITAL
void Key_Functions_Digital(void) {

  if (!Key.Left_Rel && !Key.Down_Rel && !Key.Right_Rel&& !Key.Up_Rel) {
    Key.Released = ON;
  }
  if (Key.Released && !digitalRead(KEY_LEFT)) {
    Key.Left_Rel = ON;
    return;
  }
  if (Key.Left_Rel  &&  digitalRead(KEY_LEFT)) {
    Key.Left_Rel = OFF;
    DisplayWakeUp();
    EnterMenuKey();
    KeyTimeOutStart();
    return;
  }
  if (Key.Released && !digitalRead(KEY_DOWN)) {
    Key.Down_Rel = ON;
    return;
  }
  if (Key.Down_Rel  &&  digitalRead(KEY_DOWN)) {
    Key.Down_Rel = OFF;
    DisplayWakeUp();
    DownMenuKey(); 
    KeyTimeOutStart();
    return;
  }
  if (Key.Released && !digitalRead(KEY_UP)) {
    Key.Up_Rel = ON;
    return;
  }
  if (Key.Up_Rel  &&  digitalRead(KEY_UP)) {
    Key.Up_Rel = OFF;
    DisplayWakeUp();
    UpMenuKey();   
    KeyTimeOutStart();
    return;
  }
  
  if (Key.Released && !digitalRead(KEY_RIGHT)) {
    Key.Right_Rel = ON;
    return;
  }
  if (Key.Right_Rel  &&  digitalRead(KEY_RIGHT)) {
    DisplayWakeUp();
    EscMenuKey();
    KeyTimeOutStart();
    return;
  }
}
#endif
void DownMenuKey(void) {
  if (Display.OLED_Timer == 0) return;
  DispExtTimeout();
  switch (MainMenu) {
    case MENU_NULL :// Menu = MENU_NULL;
            KeySensonsorRoll(DOWNROLL);
      break;
    case MENU1 : MainMenu = MENU7; //
      break;
    case MENU2 : MainMenu = MENU1;
      break;
    case MENU3 : MainMenu = MENU2;
      break;
    case MENU4 : MainMenu = MENU3;
      break;
     case MENU5 : MainMenu = MENU4;
      break;  
     case MENU6 : MainMenu = MENU5;
      break;  
     case MENU7 : MainMenu = MENU6;
      break;  
         
    case MENU1_SUB1 :  MainMenu =  MENU1_SUB2;//
      break;
    case MENU1_SUB2 : MainMenu =  MENU1_SUB1;//
      break;
    case MENU1_SUB3 :       
      break;
    case MENU1_SUB4 :     
      break;    
    case MENU2_SUB01 :  
          #if defined (ARDUINO_MEGA) | defined (ARDUINO_DUE) 
              MainMenu =  MENU2_SUB12;// 
          #endif   
          #ifdef STM32_F407 //
              MainMenu =  MENU2_SUB09;// 5
          #endif           
      break;
    case MENU2_SUB02 :  MainMenu =  MENU2_SUB01;//
      break;
    case MENU2_SUB03 :  MainMenu =  MENU2_SUB02;//
      break;
    case MENU2_SUB04 :  MainMenu =  MENU2_SUB03;//
      break;
    case MENU2_SUB05 :  MainMenu =  MENU2_SUB04;//
      break;
    case MENU2_SUB06 :  MainMenu =  MENU2_SUB05;//
      break;
    case MENU2_SUB07 : MainMenu =  MENU2_SUB06;// 
      break;
   case MENU2_SUB08 :  MainMenu =  MENU2_SUB07;//
      break;
    case MENU2_SUB09 : MainMenu =  MENU2_SUB08;// 5   
      break;     
   case MENU2_SUB10 : MainMenu =  MENU2_SUB09;// 10
      break;
    case MENU2_SUB11 : MainMenu =  MENU2_SUB10;//  20
      break;  
    case MENU2_SUB12 : MainMenu =  MENU2_SUB11;//  60
      break;  
    case MENU2_SUB13 :  
      break;  
    case MENU2_SUB14 :  
      break; 
      
    case MENU3_SUB1 :  MainMenu =  MENU3_SUB2;//
      break;
    case MENU3_SUB2 :  MainMenu =  MENU3_SUB1;//
      break;
    case MENU3_SUB3 : 
      break;
    case MENU3_SUB4 : 
      break; 
    case MENU4_SUB1 :   MainMenu = MENU4_SUB2; 
      break;  
    case MENU4_SUB2 :   MainMenu = MENU4_SUB3; 
      break;   
    case MENU4_SUB3 :   MainMenu = MENU4_SUB4; 
      break; 
    case MENU4_SUB4 :   MainMenu = MENU4_SUB1; 
      break; 
             
    case MENU5_SUB1 :  
        DateTimeBuf.Year--;
        if (DateTimeBuf.Year < 2021)DateTimeBuf.Year = 2040;
      break;
    case MENU5_SUB2 :
         DateTimeBuf.Month--;
        if (DateTimeBuf.Month > 12)DateTimeBuf.Month = 1;         
        if (DateTimeBuf.Month < 1)DateTimeBuf.Month = 12;
      break;
    case MENU5_SUB3 :  
        DateTimeBuf.Day --; 
         if (DateTimeBuf.Day > 31)DateTimeBuf.Day = 1;       
        if (DateTimeBuf.Day < 1)DateTimeBuf.Day = 31;
      break;
    case MENU5_SUB4 :
        if (DateTimeBuf.Hour == 0 )DateTimeBuf.Hour = 23;  
        else DateTimeBuf.Hour --;
        if (DateTimeBuf.Hour > 23)DateTimeBuf.Hour = 0; 
    //    if (DateTimeBuf.Hour == 0 )DateTimeBuf.Hour = 23;                  
    //    if (DateTimeBuf.Hour < 1)DateTimeBuf.Hour = 23;  
      break;
    case MENU5_SUB5 :  
        DateTimeBuf.Minute --;
        if (DateTimeBuf.Minute > 59)DateTimeBuf.Minute = 59;         
      //  if (DateTimeBuf.Minute < 1)DateTimeBuf.Minute = 59;  
      break;
    case MENU5_SUB6 :  
         DateTimeBuf.Second --;
        if (DateTimeBuf.Second > 59)DateTimeBuf.Second = 59;             
     //   if (DateTimeBuf.Second < 1)DateTimeBuf.Second = 59;    
      break;   
     case MENU5_SUB7 :      
      break;
      
      case MENU6_SUB1:  MainMenu = MENU6_SUB4; // Calib Start?  ..60sec   
        break;
      case MENU6_SUB2:  //       
        break;        
      case MENU6_SUB3:      
        break;
      case MENU6_SUB4:  MainMenu = MENU6_SUB1; // Calib Start?  ..60sec        
        break;
      case MENU6_SUB5: MainMenu = MENU6_SUB6; // Calib Start?  ..60sec    //       
        break;        
      case MENU6_SUB6:  MainMenu = MENU6_SUB5; // Calib Start?  ..60sec       
        break;    
      case MENU6_SUB7:      
        break; 
        
      case MENU7_SUB1 : MainMenu =  MENU7_SUB7;//
        break;
      case MENU7_SUB2 : MainMenu =  MENU7_SUB1;//
        break;
      case MENU7_SUB3 : MainMenu =  MENU7_SUB2;//
        break;
      case MENU7_SUB4 : MainMenu =  MENU7_SUB3;//
        break;
      case MENU7_SUB5 : MainMenu =  MENU7_SUB4;//
        break;
      case MENU7_SUB6 : MainMenu =  MENU7_SUB5;//
        break;
      case MENU7_SUB7 :MainMenu =  MENU7_SUB6;//
        break;   
      case MENU7_SUB8 :
        break;            
      case MENU7_SUB9 :
        break; 
        default: MainMenu = MENU_NULL;
     break;

  }
} // DownMenuKey

void UpMenuKey(void) {
  if (Display.OLED_Timer == 0) return;
  DispExtTimeout();
  switch (MainMenu) {
    case MENU_NULL : MainMenu = MENU_NULL;
          KeySensonsorRoll(UPROLL);
      break;
    case MENU1 : MainMenu = MENU2; //
      break;
    case MENU2 : MainMenu = MENU3;
      break;
    case MENU3 : MainMenu = MENU4;
      break;
    case MENU4 : MainMenu = MENU5;
      break;
     case MENU5 : MainMenu = MENU6;
      break;     
     case MENU6 : MainMenu = MENU7;
      break; 
     case MENU7 : MainMenu = MENU1;
      break; 
      
    case MENU1_SUB1 :  MainMenu =  MENU1_SUB2;//
      break;
    case MENU1_SUB2 : MainMenu =  MENU1_SUB1;//
      break;
    case MENU1_SUB3 :       
      break;
    case MENU1_SUB4 :     
      break;
    case MENU2_SUB01 :  MainMenu =  MENU2_SUB02;//
      break;
    case MENU2_SUB02 :  MainMenu =  MENU2_SUB03;//
      break;
    case MENU2_SUB03 :  MainMenu =  MENU2_SUB04;//
      break;
    case MENU2_SUB04 :  MainMenu =  MENU2_SUB05;//
      break;
    case MENU2_SUB05 :  MainMenu =  MENU2_SUB06;//
      break;
    case MENU2_SUB06 :  MainMenu =  MENU2_SUB07;//
      break;
    case MENU2_SUB07 :  MainMenu =  MENU2_SUB08;//
      break;
    case MENU2_SUB08 :  MainMenu =  MENU2_SUB09;
      break;
     case MENU2_SUB09 :  
      #if defined (ARDUINO_MEGA) | defined (ARDUINO_DUE)     
            MainMenu =  MENU2_SUB10;
      #endif
      #ifdef STM32_F407 // 10msec
            MainMenu =  MENU2_SUB01;//
       #endif    
      break;  
    case MENU2_SUB10 :  MainMenu =  MENU2_SUB11;
      break;
     case MENU2_SUB11 :  MainMenu =  MENU2_SUB12;
      break;  
     case MENU2_SUB12 :  MainMenu =  MENU2_SUB01;//
      break; 
     case MENU2_SUB13 :  
      break;  
     case MENU2_SUB14 :  
      break; 
         
    case MENU3_SUB1 :  MainMenu =  MENU3_SUB2;//
      break;
    case MENU3_SUB2 :  MainMenu =  MENU3_SUB1;//
      break;
    case MENU3_SUB3 : 
      break;
    case MENU3_SUB4 : 
      break;    
    case MENU4_SUB1 :   MainMenu = MENU4_SUB2; 
      break;  
    case MENU4_SUB2 :   MainMenu = MENU4_SUB3; 
      break;   
    case MENU4_SUB3 :   MainMenu = MENU4_SUB4; 
      break;  
    case MENU4_SUB4 :   MainMenu = MENU4_SUB1; 
      break;        
    case MENU5_SUB1 :  
        DateTimeBuf.Year++;
        if (DateTimeBuf.Year > 2040)DateTimeBuf.Year = 2021;
      //  if (DateTimeBuf.Year < 2020)DateTimeBuf.Year = 2020;
      break;
    case MENU5_SUB2 :
         DateTimeBuf.Month++;
        if (DateTimeBuf.Month > 12)DateTimeBuf.Month = 1;
        if (DateTimeBuf.Month < 1)DateTimeBuf.Month = 1;
    
      break;
    case MENU5_SUB3 :  
          DateTimeBuf.Day ++;
        if (DateTimeBuf.Day > 31)DateTimeBuf.Day = 1;
        if (DateTimeBuf.Day < 1)DateTimeBuf.Day = 1;
      break;
    case MENU5_SUB4 :
        DateTimeBuf.Hour ++;
        if (DateTimeBuf.Hour > 23)DateTimeBuf.Hour = 0;
       // if (DateTimeBuf.Hour < 1)DateTimeBuf.Hour = 1;  
      break;
    case MENU5_SUB5 :  
              DateTimeBuf.Minute ++;
        if (DateTimeBuf.Minute > 59)DateTimeBuf.Minute = 0;
      //  if (DateTimeBuf.Minute < 1)DateTimeBuf.Minute = 1;  
      break;
    case MENU5_SUB6 :  
         DateTimeBuf.Second ++;
        if (DateTimeBuf.Second > 59)DateTimeBuf.Second = 0;    
      break;
     case MENU5_SUB7 :      
      break;
     case MENU6_SUB1: MainMenu = MENU6_SUB4; // Calib Start?  ..60sec     
      break;
      case MENU6_SUB2:  //       
      break;        
       case MENU6_SUB3:      
      break;
       case MENU6_SUB4: MainMenu = MENU6_SUB1;  //   Calibration Started !  
      break;
      case MENU6_SUB5: MainMenu = MENU6_SUB6; //       
      break;        
       case MENU6_SUB6: MainMenu = MENU6_SUB5;     
      break;    
        case MENU6_SUB7:      //   Mains Updated !   
      break; 
      
      case MENU7_SUB1 : MainMenu =  MENU7_SUB2;
        break;
      case MENU7_SUB2 : MainMenu =  MENU7_SUB3;//
        break;
      case MENU7_SUB3 : MainMenu =  MENU7_SUB4;//
        break;
      case MENU7_SUB4 : MainMenu =  MENU7_SUB5;//
        break;
      case MENU7_SUB5 : MainMenu =  MENU7_SUB6;//
        break;
      case MENU7_SUB6 : MainMenu =  MENU7_SUB7;//
        break;
      case MENU7_SUB7 :MainMenu =  MENU7_SUB1;//
        break;   
      case MENU7_SUB8 :
        break;      
      case MENU7_SUB9 :
        break;   
    default: MainMenu = MENU_NULL;
      break;
  }
}


void EscMenuKey(void) {   
  if (Display.OLED_Timer == 0) return;
  DispExtTimeout();
  switch (MainMenu) {
    case MENU_NULL : MainMenu = MENU_NULL;
      break;
    case MENU1 : MainMenu = MENU_NULL; //
      break;
    case MENU2 : MainMenu = MENU_NULL;
      break;
    case MENU3 : MainMenu = MENU_NULL;
      break;
    case MENU4 : MainMenu = MENU_NULL;
      break;        
    case MENU5 : MainMenu = MENU_NULL;
      break;
    case MENU6:   MainMenu = MENU_NULL; //     
      break;
    case MENU7:   MainMenu = MENU_NULL; //     
      break;
           
    case MENU1_SUB1 :  MainMenu =  MENU1;//
      break;
    case MENU1_SUB2 : MainMenu =  MENU1;//
      break;
    case MENU1_SUB3 :     // LOG Started  
      break;
    case MENU1_SUB4 :     // LOG Stoped
      break;
    case MENU2_SUB01 :  MainMenu =  MENU2;//
      break;
    case MENU2_SUB02 : MainMenu =  MENU2;//
      break;
    case MENU2_SUB03 :  MainMenu =  MENU2;//
      break;
    case MENU2_SUB04 : MainMenu =  MENU2;//
      break;
    case MENU2_SUB05 :  MainMenu =  MENU2;//
      break;
    case MENU2_SUB06 :  MainMenu =  MENU2;//
      break;
    case MENU2_SUB07 :  MainMenu =  MENU2;//
      break;
    case MENU2_SUB08 :  MainMenu =  MENU2;//
      break;
    case MENU2_SUB09 :  MainMenu =  MENU2;//
      break;
    case MENU2_SUB10 :  MainMenu =  MENU2;//
      break;
    case MENU2_SUB11 :  MainMenu =  MENU2;//
      break;
     case MENU2_SUB12 :  MainMenu =  MENU2;//
      break;     
      case MENU2_SUB13 : 
        switch(KeySupport.SampleTime){     
          case TASK_1MSEC: MainMenu =  MENU2_SUB01;//
          break;          
          case TASK_5MSEC: MainMenu =  MENU2_SUB02;//
          break;           
          case TASK_10MSEC: MainMenu =  MENU2_SUB03;//
          break;          
          case TASK_20MSEC: MainMenu =  MENU2_SUB04;//
          break;         
          case TASK_50MSEC: MainMenu =  MENU2_SUB05;//
          break;          
          case TASK_100MSEC: MainMenu =  MENU2_SUB06;//
          break;
          case TASK_250MSEC: MainMenu =  MENU2_SUB07;//
          break;
          case TASK_500MSEC: MainMenu =  MENU2_SUB08;//
          break;
          case TASK_1SEC: MainMenu =  MENU2_SUB09;//
          break;
          case TASK_2SEC: MainMenu =  MENU2_SUB10;//
          break;
          case TASK_5SEC: MainMenu =  MENU2_SUB11;//
          break;           
          case TASK_10SEC: MainMenu =  MENU2_SUB12;//
          break;  
          default: MainMenu =  MENU_NULL; 
          break;                    
        }
      break;
      case MENU2_SUB14 : // Sample Time Updated !
      break;

      
    case MENU3_SUB1 :  MainMenu =  MENU3;//
      break;
    case MENU3_SUB2 :  MainMenu =  MENU3;//
      break;
    case MENU3_SUB3 :   // STANDBYE Updated ! On
      break;
    case MENU3_SUB4 :  // STANDBYE Updated ! Off
      break;
      
     case MENU4_SUB1 :   MainMenu = MENU4; 
      break;  
    case MENU4_SUB2 :   MainMenu = MENU4; 
      break;   
    case MENU4_SUB3 :   MainMenu = MENU4; 
      break;  
     case MENU4_SUB4 :   MainMenu = MENU4; 
      break;             
    case MENU5_SUB1 :  MainMenu = MENU5;
      break;
    case MENU5_SUB2 :  MainMenu = MENU5_SUB1;
      break;
    case MENU5_SUB3 :  MainMenu = MENU5_SUB2;
      break;
    case MENU5_SUB4 :  MainMenu = MENU5_SUB3;
      break;
    case MENU5_SUB5 :  MainMenu = MENU5_SUB4;
      break;
    case MENU5_SUB6 :  MainMenu = MENU5_SUB5;
      break;
    case MENU5_SUB7 :      // Date & Time Updated !
      break;
      

    case MENU6_SUB1: MainMenu = MENU6; //     
      break;
    case MENU6_SUB2: MainMenu = MENU6; //       
      break;        
    case MENU6_SUB3:     //  Calibration Started !     
      break;  
    case MENU6_SUB4: MainMenu = MENU6; //     //  Mains EU/US Select 
      break;
     case MENU6_SUB5:  MainMenu = MENU6;//       
      break;        
     case MENU6_SUB6: MainMenu = MENU6;      
      break;    
     case MENU6_SUB7:       //   Mains Updated !   
      break;  
      
      case MENU7_SUB1 : MainMenu =  MENU7;//
        break;
      case MENU7_SUB2 : MainMenu =  MENU7;//
        break;
      case MENU7_SUB3 : MainMenu =  MENU7;//
        break;
      case MENU7_SUB4 : MainMenu =  MENU7;//
        break;
      case MENU7_SUB5 : MainMenu =  MENU7;//
        break;
      case MENU7_SUB6 : MainMenu =  MENU7;//
        break;
      case MENU7_SUB7 : MainMenu =  MENU7;//
        break; 
      case MENU7_SUB8 :  
        switch(KeySupport.MaxFileSizeSure){
          case 0: MainMenu =  MENU7_SUB1;//
          break;
          case 1: MainMenu =  MENU7_SUB2;//
          break;
          case 2: MainMenu =  MENU7_SUB3;//
          break;
          case 3: MainMenu =  MENU7_SUB4;//
          break;
          case 4: MainMenu =  MENU7_SUB5;//
          break;
          case 5:MainMenu =  MENU7_SUB6;//
          break;           
          case 6:MainMenu =  MENU7_SUB7;//
          break;  
          default: MainMenu =  MENU_NULL; 
          break;                    
        }
      
        break; 
       case MENU7_SUB9 :  // File Size Updated !
        break;                      
    default: MainMenu = MENU_NULL;
    break;
  }
}
void SD_Card_Log_NV_Menu(bool LogStatus){
    NVRam_Write(EE_LOGSTATUS,LogStatus);
    if(LogStatus == ON)MainMenu =  MENU1_SUB3;//MENU1
    else MainMenu =  MENU1_SUB4;
     
}
void EnterMenuKey(void){
  if (Display.OLED_Timer == 0) return;
  DispExtTimeout();
  switch (MainMenu) {
    case MENU_NULL : MainMenu = MENU1;
        break;
    case MENU1 : //Menu = MENU1MIN; // go to sub menu  // sd kart log on
        if (SDCard.LogEnable == ON) MainMenu = MENU1_SUB2; //already logging
        else  MainMenu = MENU1_SUB1;
      break;
   case MENU2 : // Menu = MENU2MIN; // call the right menu according to current one
      switch (SampleTime) {  
        case TASK_1MSEC: MainMenu = MENU2_SUB01;
          break; 
        case TASK_5MSEC: MainMenu = MENU2_SUB02;
          break;        
        case TASK_10MSEC: MainMenu = MENU2_SUB03;
          break;      
        case TASK_20MSEC: MainMenu = MENU2_SUB04;
          break;      
         case TASK_50MSEC: MainMenu = MENU2_SUB05;
          break;       
        case TASK_100MSEC: MainMenu = MENU2_SUB06;
          break;
        case TASK_250MSEC : MainMenu = MENU2_SUB07;
          break;
        case TASK_500MSEC : MainMenu = MENU2_SUB08;
          break;
        case TASK_1SEC : MainMenu = MENU2_SUB09;
          break;
        case TASK_2SEC : MainMenu = MENU2_SUB10;
          break;
        case TASK_5SEC : MainMenu = MENU2_SUB11;
          break;
        case TASK_10SEC : MainMenu = MENU2_SUB12;
          break;      
        default:
          break;
        }
      break;  
    case MENU3 : // Menu = MENU3MIN;
        if (Display.SleepEnable == ON) MainMenu = MENU3_SUB2; //already logging
        else  MainMenu = MENU3_SUB1;
      break;
    case MENU4 :   MainMenu = MENU4_SUB1;       
      break;
    case MENU5 :  MainMenu = MENU5_SUB1;
        DateTimeBuf.Init = ON;       
      break;
    case MENU6:   MainMenu = MENU6_SUB1; //     
      break;  
                 
    case MENU1_SUB1 :  //SetResetLog(ON); 
        switch(SampleTime){
          case TASK_10MSEC:  
          break;          
          case TASK_20MSEC: 
          break;      
          case TASK_50MSEC:  
          break;          
          case TASK_100MSEC:  
          break;
          case TASK_250MSEC: 
          break;    
          /*                
          case TASK_500MSEC:  
          break;
          case TASK_1SEC: SampleTime = TASK_1SEC;  
          break;
          case TASK_2SEC: SampleTime = TASK_2SEC;
          break;
          case TASK_5SEC: SampleTime = TASK_5SEC;
          break;
          case TASK_10SEC: SampleTime = TASK_10SEC;
          break;
          case TASK_20SEC: SampleTime = TASK_20SEC;
          break;           
          case TASK_60SEC: 
          break;  
          */
          default: 
          break;                    
        }
        ExLog_Start(); // only if not fast mode
        //SD_Card_Log_NV_Menu(SDCard.LogEnable);
        NVRam_Write(EE_LOGSTATUS,SDCard.LogEnable);
        MainMenu =  MENU1_SUB3;//MENU1
      break;
    case MENU1_SUB2 : // SetResetLog(OFF);

        
        SDCard.LogEnable = OFF;
        //SD_Card_Log_NV_Menu(SDCard.LogEnable);
        NVRam_Write(EE_LOGSTATUS,SDCard.LogEnable);    
        MainMenu =  MENU1_SUB4;//MENU1
      break;
    case MENU1_SUB3 :  
      break;
    case MENU1_SUB4 :   
      break;
    case MENU2_SUB01 :  if(SampleTime != TASK_1MSEC) {KeySupport.SampleTime = TASK_1MSEC; MainMenu =  MENU2_SUB13;}  
      break;
    case MENU2_SUB02 :  if(SampleTime != TASK_5MSEC) {KeySupport.SampleTime = TASK_5MSEC; MainMenu =  MENU2_SUB13;}  
      break;
    case MENU2_SUB03 :  if(SampleTime != TASK_10MSEC) {KeySupport.SampleTime = TASK_10MSEC; MainMenu =  MENU2_SUB13;}  
      break;
    case MENU2_SUB04 :  if(SampleTime != TASK_20MSEC) {KeySupport.SampleTime = TASK_20MSEC; MainMenu =  MENU2_SUB13;}  
      break;
    case MENU2_SUB05 : if(SampleTime != TASK_50MSEC) {KeySupport.SampleTime = TASK_50MSEC; MainMenu =  MENU2_SUB13;}
      break;
    case MENU2_SUB06 :  if(SampleTime != TASK_100MSEC) {KeySupport.SampleTime = TASK_100MSEC; MainMenu =  MENU2_SUB13;}
      break;
    case MENU2_SUB07 : if(SampleTime != TASK_250MSEC) {KeySupport.SampleTime = TASK_250MSEC; MainMenu =  MENU2_SUB13;}
      break;
    case MENU2_SUB08 : if(SampleTime != TASK_500MSEC) {KeySupport.SampleTime = TASK_500MSEC; MainMenu =  MENU2_SUB13;} 
      break;
    case MENU2_SUB09 :  if(SampleTime != TASK_1SEC) {KeySupport.SampleTime = TASK_1SEC; MainMenu =  MENU2_SUB13;}
      break;
    case MENU2_SUB10 : if(SampleTime != TASK_2SEC) {KeySupport.SampleTime = TASK_2SEC; MainMenu =  MENU2_SUB13;} 
      break;    
    case MENU2_SUB11 :  if(SampleTime != TASK_5SEC) {KeySupport.SampleTime = TASK_5SEC; MainMenu =  MENU2_SUB13;}
      break;
    case MENU2_SUB12 : if(SampleTime != TASK_10SEC) {KeySupport.SampleTime = TASK_10SEC; MainMenu =  MENU2_SUB13;} 
      break;
       
    case MENU2_SUB13 :  
        switch(KeySupport.SampleTime){
           case TASK_1MSEC: SampleTime = TASK_1MSEC;
          break;           
          case TASK_5MSEC: SampleTime = TASK_5MSEC;
          break;           
          case TASK_10MSEC: SampleTime = TASK_10MSEC;// 
          break;          
          case TASK_20MSEC: SampleTime = TASK_20MSEC;//
          break;          
          case TASK_50MSEC: SampleTime = TASK_50MSEC;// 
          break;          
          case TASK_100MSEC: SampleTime = TASK_100MSEC;// 
          break;
          case TASK_250MSEC: SampleTime = TASK_250MSEC; //
          break;          
          case TASK_500MSEC: SampleTime = TASK_500MSEC; //
          break;
          case TASK_1SEC: SampleTime = TASK_1SEC;  
          break;
          case TASK_2SEC: SampleTime = TASK_2SEC;
          break;
          case TASK_5SEC: SampleTime = TASK_5SEC;
          break;
          case TASK_10SEC: SampleTime = TASK_10SEC;
          break;
          case TASK_20SEC: SampleTime = TASK_20SEC;
          break;           
          case TASK_60SEC: SampleTime = TASK_60SEC;
          break;  
          default: MainMenu =  MENU_NULL; 
          break;                    
        }
        NVRam_Write(EE_SAMPLE, SampleTime);
        MainMenu =  MENU2_SUB14;     
      break;
      case MENU2_SUB14 :  
      
      break;  
    case MENU3_SUB1 :  DispEnable(ON,40);
      //NVRam_Write_Standbye(ON);
      NVRam_Write(EE_STANDBYE, ON);
      MainMenu =  MENU3_SUB3;//MENU3
      break;
    case MENU3_SUB2 :  DispEnable(OFF,0);
      //NVRam_Write_Standbye(OFF); 
      NVRam_Write(EE_STANDBYE, OFF);
      MainMenu =  MENU3_SUB4;//MENU3
      break;
    case MENU3_SUB3 :  
      
      break;
    case MENU3_SUB4 :     
      break; 
        

    case MENU4_SUB1 :   MainMenu = MENU4_SUB2; 
      break;  
    case MENU4_SUB2 :   MainMenu = MENU4_SUB3; 
      break;    
    case MENU4_SUB3 :   MainMenu = MENU4_SUB4; 
      break;  
    case MENU4_SUB4 :   MainMenu = MENU4_SUB1; 
      break;               
      
     case MENU5_SUB1 :  MainMenu = MENU5_SUB2;   //Year  
      break;     
     case MENU5_SUB2 :  MainMenu = MENU5_SUB3; // Month
      break;        
     case MENU5_SUB3 :  MainMenu = MENU5_SUB4; // Day
      break;  
     case MENU5_SUB4 :  MainMenu = MENU5_SUB5; // Hour
      break;  
     case MENU5_SUB5 :  MainMenu = MENU5_SUB6; // Minute
      break;
     case MENU5_SUB6 :  MainMenu = MENU5_SUB7; //Second
          DateTimeBuf.RTC_Update = ON;    
      break;
     case MENU5_SUB7 :      
      break;
          
     case MENU6_SUB1: MainMenu = MENU6_SUB2; //     
        break;
     case MENU6_SUB2: MainMenu = MENU6_SUB3; //
        #ifdef ENERGYMETER_EXISTS
          EnergyMeterIC.Mode = POWERIC_CALB1; 
        #endif      
      break;        
     case MENU6_SUB3:      // Calibration Started !
        break;  
     case MENU6_SUB4:   MainMenu = MENU6_SUB5; //  Mains EU/US Select    
        break;
     case MENU6_SUB5:  MainMenu = MENU6_SUB7;  
        #ifdef ENERGYMETER_EXISTS         
          //NVRam_Write_MainsFreq(HERTZ_50);
          Mains_Frequency = FREQUENCY_50HZ;
          NVRam_Write(EE_ADE_FREQ,HERTZ_50); 
        #endif        
      break;        
     case MENU6_SUB6: MainMenu = MENU6_SUB7;  
         #ifdef ENERGYMETER_EXISTS          
          //NVRam_Write_MainsFreq(HERTZ_60);
          Mains_Frequency = FREQUENCY_60HZ;   
          NVRam_Write(EE_ADE_FREQ,HERTZ_60);            
        #endif             
      break;    
     case MENU6_SUB7:    //   Mains Updated !     
      break;    

    case MENU7:     
          switch(FileSize.MaxSize){
            case  KBYTE_500 :  MainMenu = MENU7_SUB1; // 
              break;
            case MBYTE_1 :     MainMenu = MENU7_SUB2; // 
              break;
            case MBYTE_2 :     MainMenu = MENU7_SUB3; // 
              break;
            case MBYTE_4 :     MainMenu = MENU7_SUB4; // 
              break;
            case MBYTE_8 :    MainMenu = MENU7_SUB5; // 
              break;    
            case MBYTE_16 :   MainMenu = MENU7_SUB6; // 
              break;  
            case MBYTE_32 :   MainMenu = MENU7_SUB7; // 
              break;                
            default:
              break;                                                  
          }
        break;  
      case MENU7_SUB1 :  if(FileSize.MaxSize != KBYTE_500) {KeySupport.MaxFileSizeSure = 0; MainMenu =  MENU7_SUB8;}//
        break;
      case MENU7_SUB2 : if(FileSize.MaxSize != MBYTE_1){KeySupport.MaxFileSizeSure = 1; MainMenu =  MENU7_SUB8;}//
        break;
      case MENU7_SUB3 : if(FileSize.MaxSize != MBYTE_2){KeySupport.MaxFileSizeSure = 2; MainMenu =  MENU7_SUB8;}//
        break;
      case MENU7_SUB4 : if(FileSize.MaxSize != MBYTE_4){KeySupport.MaxFileSizeSure = 3;  MainMenu =  MENU7_SUB8;}//
        break;
      case MENU7_SUB5 : if(FileSize.MaxSize != MBYTE_8){KeySupport.MaxFileSizeSure = 4; MainMenu =  MENU7_SUB8;}//
        break;
      case MENU7_SUB6 :  if(FileSize.MaxSize != MBYTE_16){KeySupport.MaxFileSizeSure = 5; MainMenu =  MENU7_SUB8;}//
        break; 
      case MENU7_SUB7 :  if(FileSize.MaxSize != MBYTE_32){KeySupport.MaxFileSizeSure = 6; MainMenu =  MENU7_SUB8;}//
        break; 
        
      case MENU7_SUB8 :  // Sure If Yes Enter
        uint8_t i ;
        switch(KeySupport.MaxFileSizeSure){
          case 0: FileSize.MaxSize = KBYTE_500; i = 0;
          break;
          case 1: FileSize.MaxSize = MBYTE_1;  i = 1; 
          break;
          case 2: FileSize.MaxSize = MBYTE_2;i = 2;
          break;
          case 3: FileSize.MaxSize = MBYTE_4;i = 3;
          break;
          case 4: FileSize.MaxSize = MBYTE_8;i = 4;
          break;
          case 5:FileSize.MaxSize = MBYTE_16;i = 5;
          break;           
          case 6:FileSize.MaxSize = MBYTE_32;i = 6;
          break;  
          default: MainMenu =  MENU_NULL; i = 8;
          break;                    
        }
        NVRam_Write(EE_FILESIZE,i);
        //NVRam_Write_MaxFileSize(i);
        MainMenu =  MENU7_SUB9;
        break; 
       case MENU7_SUB9 :  // File Size Updated !
        break;        
    default: MainMenu = MENU_NULL; 
    break;
  }
}
