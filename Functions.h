
 

#if defined (ARDUINO_MEGA) | defined (ARDUINO_DUE) 

  #define TIME_20MSEC 1 
  #define TIME_50MSEC 2
  #define TIME_100MSEC 5
  #define TIME_250MSEC 12
  #define TIME_500MSEC 25
  #define TIME_1_SEC 50
  #define TIME_2_SEC 100
  #define TIME_5_SEC 250
  #define TIME_10_SEC 500
  #define TIME_20_SEC 1000
  #define TIME_60_SEC 3000
#endif
#ifdef STM32_F407 // 10msec

  #define TIME_10MSEC 1
  #define TIME_20MSEC 2 
  #define TIME_50MSEC 5
  #define TIME_100MSEC 10
  #define TIME_250MSEC 25
  #define TIME_500MSEC 50
  #define TIME_1_SEC 100
  #define TIME_2_SEC 200
  #define TIME_5_SEC 500
  #define TIME_10_SEC 1000
  #define TIME_20_SEC 2000
  #define TIME_60_SEC 6000
#endif


// interrupt vector
  #ifdef STM32_F407 
  void STM32_IntVectorT1(){
  //  digitalWrite(LED_BLUE, !digitalRead(LED_BLUE));
  #endif

    #ifdef ARDUINO_MEGA
ISR(TIMER1_OVF_vect){        // interrupt service routine that wraps a user defined function supplied by attachInterrupt
 //   TCNT1 = 34286;            // preload timer for 500mSec
       TCNT1 = 64286;            // preload timer for 20mSec
    #endif
    
    #ifdef ARDUINO_DUE
void TC3_Handler(){
        TC_GetStatus(TC1, 0);
    #endif

     #ifdef ARDUINO_MKRZERO
void TC5_Handler (void){
      #endif 
    
     #ifdef CHIPKIT_MAX32
  //  void __ISR(_TIMER_2_VECTOR, i p l 3 ) myISR function ( void ){
void JustAFunction(){
    //  ISR(TIMER1_OVF_vect){
    #endif  

          digitalWrite(DEBUG_OUT_1, !digitalRead(DEBUG_OUT_1));
          
 /*   
    #if defined (ARDUINO_MEGA)  | defined (ARDUINO_DUE)//  | defined (ARDUINO_MKRZERO)
          digitalWrite(DEBUG_OUT, digitalRead(DEBUG_OUT) ^ 1);   
    #endif
    #if defined (ARDUINO_MKRZERO) | defined (CHIPKIT_MAX32)
          digitalWrite(DEBUG_OUT, digitalRead(DEBUG_OUT) ^ 1);   
    #endif     
*/

  //if((Log_Status == LOG_LOOP_IN_ACTION)  || (Log_Status == LOG_BIN_CREATE_POST))return;
  if(Log_Status == LOG_LOOP_IN_ACTION)return; // only in this mode !!!
  

  #ifdef STM32_F407 // 10msec int
    Loop.Task_10msec = ON; 
    Loop.IntTimer_20m ++;       
    Loop.IntTimer_50m ++;   
    Loop.IntTimer_100m ++;  
    Loop.IntTimer_250m ++;
    Loop.IntTimer_500m ++;   
    Loop.IntTimer_1 ++;
    Loop.IntTimer_2 ++;
    Loop.IntTimer_5 ++;
  //    Loop.IntTimer_10 ++;
 //   Loop.IntTimer_20 ++;   
 //   Loop.IntTimer_60 ++;   
 #endif
   
#if defined (ARDUINO_MEGA) | defined (ARDUINO_DUE) 
    Loop.Task_10msec = OFF; 
    Loop.IntTimer_20m = TIME_20MSEC;       
    Loop.IntTimer_50m ++;   
    Loop.IntTimer_100m ++;  
    Loop.IntTimer_250m ++;
    Loop.IntTimer_500m ++;   
    Loop.IntTimer_1 ++;
    Loop.IntTimer_2 ++;
    Loop.IntTimer_5 ++;
    Loop.IntTimer_10 ++;
    Loop.IntTimer_20 ++;   
    Loop.IntTimer_60 ++;   
#endif


    if(Loop.IntTimer_20m >= TIME_20MSEC){
      Loop.IntTimer_20m = 0;
      Loop.Task_20msec = ON;    
          #if  defined KEY_DIGITAL
      Key_Functions_Digital();
          #endif
          #if  defined KEY_ANALOG      
      Key_Functions_Analog(analogRead(KEY_ANALOG_IN));   
          #endif 
    }
    if(Loop.IntTimer_50m >= TIME_50MSEC){
      Loop.IntTimer_50m = 0;
      Loop.Task_50msec = ON;     
    }
    if(Loop.IntTimer_100m >= TIME_100MSEC){
      Loop.IntTimer_100m = 0;
      Loop.Task_100msec = ON;
      
       #ifdef BATTERY_SCOOTER_EXISTS
     
      #ifdef ANALOG_RES_12BIT
          float ADC_BIT = 0.8056640625; //mA
          #define  ADC_OFFSET 78
      #endif            
      #ifdef ANALOG_RES_10BIT
            float ADC_BIT = 3.22265625; // mV
            #define  ADC_OFFSET 19
      #endif             

      #define  CURRENT_OFFSET 89
                
      Values.Bat_Current_Adc = analogRead(BAT_CURRENT);  
      Values.Battery_Current_IO_Volt = (float)(ADC_BIT * Values.Bat_Current_Adc + ADC_OFFSET);
      
      Values.Battery_Current_Eq_Volt = 2500 - Values.Battery_Current_IO_Volt + CURRENT_OFFSET; 
      Values.Battery_Current_LEM = (15/2500)*Values.Battery_Current_Eq_Volt;
         
      
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");

      uint32_t  RES_DIVIDER_HIGH = 61600; 
      uint32_t  RES_DIVIDER_LOW  = 3300;   
      #define   VOLTAGE_OFFSET 1
           
      Values.Bat_Voltage_Adc = analogRead(BAT_VOLT);  
      Values.Battery_Voltage_IO_Volt = (float)(ADC_BIT * Values.Bat_Voltage_Adc + ADC_OFFSET);
      Values.Battery_Voltage_Real =  ((RES_DIVIDER_HIGH+ RES_DIVIDER_LOW)/ RES_DIVIDER_LOW)*Values.Battery_Voltage_IO_Volt + VOLTAGE_OFFSET;
      

      #endif     // end of  #ifdef BATTERY_SCOOTER_EXISTS
        
    }
    if(Loop.IntTimer_250m >= TIME_250MSEC){
      Loop.IntTimer_250m = 0;
      Loop.Task_250msec = ON;
   
    }
    if(Loop.IntTimer_500m >= TIME_500MSEC){ // 500 msec
      Loop.IntTimer_500m = 0;
      Loop.Task_500msec = ON;
      
    }
    if(Loop.IntTimer_1 >= TIME_1_SEC){  // 1 sec
      Loop.IntTimer_1 = 0;
      Loop.Task_1Sec = ON;

    #if defined (ARDUINO_MEGA)  | defined (ARDUINO_DUE)       
      digitalWrite(LED_GREEN, digitalRead(LED_GREEN) ^ 1); 
    #endif     
    #ifdef STM32_F407
      digitalWrite(LED_RED, !digitalRead(LED_RED));
    #endif


       //   digitalWrite(DEBUG_OUT_2, !digitalRead(DEBUG_OUT_2));
     //     digitalWrite(DEBUG_OUT_3, !digitalRead(DEBUG_OUT_3));     
               
      /*
      if(Display.SleepEnable == ON){
        if(Display.OLED_Timer) Display.OLED_Timer--;   // sleep active
      }
      else Display.OLED_Timer = 32768; // no sleep    
      if(Display.InitDelay == OFF)Display.InitDelay = ON;   
      */
    }
       
    if(Loop.IntTimer_2 >= TIME_2_SEC){ // 2 sec
      Loop.IntTimer_2 = 0;
      Loop.Task_2Sec = ON;
      //PrintDisplayBuffer();
    }
/*
   
    if(Loop.IntTimer_5 >= TIME_5_SEC){  // 5 sec
      Loop.IntTimer_5 = 0;
      Loop.Task_5Sec = ON;
      CounterSync_Main = 0;
    }
    if(Loop.IntTimer_10 >= TIME_10_SEC){  // 10 sec
      Loop.IntTimer_10 = 0;
      Loop.Task_10Sec = ON;
    }
  
    if(Loop.IntTimer_20 >= TIME_20_SEC){  // 20 sec
      Loop.IntTimer_20 = 0;
      Loop.Task_20Sec = ON;
     
    }
    if(Loop.IntTimer_60 >= TIME_60_SEC){  // 60 sec
      Loop.IntTimer_60 = 0;
      Loop.Task_60Sec = ON;
    }        
*/
       //  digitalWrite(LED_RED, digitalRead(LED_RED) ^ 1);
     //   if(!digitalRead(KEY_LEFT) || !digitalRead(KEY_MID) || !digitalRead(KEY_RIGHT))

}
// https://www.onlinegdb.com/edit/Hkmlxi_08


/*
void Start_Cycle_Count(uint32_t Start_C){
    SD_Counter.StartCount = micros(); 
}
void End_Cycle_Count(uint32_t Start_C){
    SD_Counter.EndCount = micros()- SD_Counter.StartCount; 
}
 Start_Cycle_Count(SD_Counter.StartCount);
  */

void Log_Data_Write_SD(){
    #ifdef SDCARD_EXISTS
    //  SD_Counter.StartCount  = micros();  
      digitalWrite(DEBUG_OUT_2,HIGH);
      SD_CardLogTask(); // Data 2 SD card Write
      digitalWrite(DEBUG_OUT_2,LOW);
   //   SD_Counter.EndCount = micros() - SD_Counter.StartCount;
    #endif 
}


void Common_Loop(){
  if (Loop.Task_10msec) {
    Loop.Task_10msec = OFF;
    if (SampleTime == TASK_10MSEC) Log_Data_Write_SD();
  }
  if (Loop.Task_20msec) {
    Loop.Task_20msec = OFF;
    if (SampleTime == TASK_20MSEC)  Log_Data_Write_SD();  
  }
  if (Loop.Task_50msec) {
    Loop.Task_50msec = OFF;
    if (SampleTime == TASK_50MSEC) Log_Data_Write_SD();
  }
  if (Loop.Task_100msec) {
    Loop.Task_100msec = OFF;
    if (SampleTime == TASK_100MSEC) Log_Data_Write_SD();    
  }
  if (Loop.Task_250msec) {
    Loop.Task_250msec = OFF;
    if (SampleTime == TASK_250MSEC) Log_Data_Write_SD();
    

	      #ifdef OLEDDISPLAY_EXISTS
        // One time after wake up form sleep
        if (Display.OLED_Init == ON) {
          Display_ReInit_Start(20);
          Display.OLED_Init = OFF;
        }
        if (Display.OLED_Timer) {
          if(Log_Status == LOG_OFF) DisplayScreen();
        }
        else {
          Display_SwitchOff();
        }
        Display_ReInit_End();
        #endif
        #ifdef LEM_CURRENT_EXISTS
            AnalogValRead();
        #endif
      
  }
  if (Loop.Task_500msec) {
    Loop.Task_500msec = OFF;
    if (SampleTime == TASK_500MSEC){Log_Data_Write_SD();}
 
    if(SDCard.LogEnable){
      ExFat_Logger_Loop();   
    }
     
  //  SD_Card_Log_NV_Menu(SDCard.LogEnable);
      
  }
  if (Loop.Task_1Sec) {
    Loop.Task_1Sec = OFF;
    #ifndef DEBUG_SIMULATOR_MODE

    SerialPortRx();
    RTC_TimeClock();
    

  #ifdef WIND_SENSOR_EXISTS
    WindSensorRead();
  #endif
  #ifdef TEMP_HUM_ONBOARD_SENSOR_EXISTS
    SensorRead_Si072(SI072_ONBOARD_SENSOR_ADDR); // MULTIPLEXER NO  
  //  Serial.print(F("Temperature_OnBoard "));Serial.println(Values.Temperature_OnBoard);
   // Serial.print(F("Humidity_OnBoard "));Serial.println(Values.Humidity_OnBoard);
    
  #endif
  #ifdef TEMP_HUM_1_SENSOR_EXISTS
    SensorRead_Si072(SI072_FIRST_SENSOR_ADDR); // MULTIPLEXER NO  
  #endif
  #ifdef TEMP_HUM_2_SENSOR_EXISTS  
    SensorRead_Si072(SI072_SECOND_SENSOR_ADDR); // MULTIPLEXER NO  
  #endif
  #ifdef TEMP_HUM_3_SENSOR_EXISTS 
    SensorRead_Si072(SI072_THIRD_SENSOR_ADDR); // MULTIPLEXER NO
  #endif
  #ifdef ENERGYMETER_EXISTS
     EnergymeterCalbLed( );
  #endif

  #ifdef  BAR_PRES_SENSOR_EXISTS
    SensorAlt_Read();
  #endif
  #ifdef LIGHT_SENSOR_EXISTS
    SensorLight_Read();
  #endif
  #ifdef ACCL_GYRO_SENSOR_EXISTS
    SensorAcccel_GyroRead();
  #endif
  #ifdef LIDAR_EXISTS
    Sensor_LidarRead();
  #endif
  
  #endif // end of  #ifndef DEBUG_SIMULATOR_MODE

    KeyTimeOutCheck();

      if(Display.SleepEnable == ON){
        if(Display.OLED_Timer) Display.OLED_Timer--;   // sleep active
      }
      else Display.OLED_Timer = 32768; // no sleep    
      if(Display.InitDelay == OFF)Display.InitDelay = ON;   
      

    if (SampleTime == TASK_1SEC)Log_Data_Write_SD(); 
    
          if((MainMenu == MENU5_SUB7)||(MainMenu == MENU2_SUB14)||(MainMenu == MENU3_SUB3) ||
                                  (MainMenu == MENU3_SUB4)||(MainMenu == MENU1_SUB3)||
                                  (MainMenu == MENU1_SUB4)||(MainMenu == MENU6_SUB3) 
                                  ||(MainMenu == MENU7_SUB9)){
          Display.MenuTimeout++;
          if(Display.MenuTimeout > 4){
            Display.MenuTimeout = 0;
            MainMenu = MENU_NULL;
         }
      }
      if(MainMenu == MENU6_SUB7) {  //  Mains Updated !   
        Display.MenuTimeout++;
        if(Display.MenuTimeout > 2){
          Display.MenuTimeout = 2;
          MainMenu = MENU6_SUB3;  // Calibration Started !
          #ifdef ENERGYMETER_EXISTS
            EnergyMeterIC.Mode = POWERIC_CALB1; 
          #endif
        }
      }     
  }
  if (Loop.Task_2Sec) {
    Loop.Task_2Sec = OFF;
  //  if (SampleTime == TASK_2SEC) Log_Data_Write_SD();


        PrintDisplayBuffer();
        #ifdef DISPLAY_ROLL
        UpdateDispRoll(DOWNROLL); 
        #endif

      if(SDCard.PauseTimer){
         SDCard.PauseTimer--;    
      }
        #ifdef ENERGYMETER_EXISTS   
             EnergyMeterIC_Operation();
        #endif
     
  }
  if (Loop.Task_5Sec) {
    Loop.Task_5Sec = OFF;
 //   if (SampleTime == TASK_5SEC) Log_Data_Write_SD();
      #ifdef PM25_DUST_SENSOR_EXISTS  
      
      #endif

  }
  #if defined (ARDUINO_MEGA) | defined (ARDUINO_DUE) 
  if (Loop.Task_10Sec) {
    Loop.Task_10Sec = OFF;
    if (SampleTime == TASK_10SEC) Log_Data_Write_SD();
  }
  if (Loop.Task_20Sec) {
    Loop.Task_20Sec = OFF;
    if (SampleTime == TASK_20SEC) Log_Data_Write_SD();
  }
  if (Loop.Task_60Sec) {
    Loop.Task_60Sec = OFF;
    if (SampleTime == TASK_60SEC) Log_Data_Write_SD();
  }
  #endif
} // end of comon loop 

void IO_Settings() {


#ifdef STM32_F407
    
  #define LED_BUILTIN       PB0               // Pin 33/PB0 control on-board LED_GREEN on F767ZI
  #define LED_BLUE          PB7               // Pin 73/PB7 control on-board LED_BLUE on F767ZI
  #define LED_RED           PB14              // Pin 74/PB14 control on-board LED_BLUE on F767ZI
  
  pinMode(LED_BUILTIN,  OUTPUT);
  pinMode(LED_BLUE,     OUTPUT);
  pinMode(LED_RED,      OUTPUT);

  pinMode(DEBUG_OUT_1, OUTPUT); 
  pinMode(DEBUG_OUT_2, OUTPUT); 
  pinMode(DEBUG_OUT_3, OUTPUT);   

  digitalWrite(DEBUG_OUT_2,LOW);
 
#endif 

  #ifdef ANALOG_RES_12BIT
    analogReadResolution(12);
  #endif 
  #ifdef ANALOG_RES_10BIT
    analogReadResolution(10);
  #endif 



  #ifdef ARDUINO_DUE
    // default i2c pin is for Mega not Due so set pins to input
    pinMode(70, INPUT);  // 
    pinMode(71, INPUT);  // 

      pinMode(DEBUG_OUT_1, OUTPUT); 
  pinMode(DEBUG_OUT_2, OUTPUT); 
  pinMode(DEBUG_OUT_3, OUTPUT); 
  #endif

  #ifdef ARDUINO_MKRZERO
    digitalWrite(OLED_GND, LOW);
    pinMode(OLED_GND, OUTPUT);  // 

    pinMode(A4, INPUT);  
    digitalWrite(OLED_CS, LOW);
    pinMode(OLED_CS, OUTPUT);  //
    digitalWrite(OLED_RESET, LOW);
    pinMode(OLED_RESET, OUTPUT);  // 

    digitalWrite(OLED_DC, LOW);
    pinMode(OLED_DC, OUTPUT);  // 
    digitalWrite(OLED_CLK, LOW);
    pinMode(OLED_CLK, OUTPUT);  // 
    digitalWrite(OLED_MOSI, LOW);
    pinMode(OLED_MOSI, OUTPUT);  // 

    digitalWrite(OLED_POWER, HIGH);
    pinMode(OLED_POWER, OUTPUT);  //



  
  #endif

  #ifdef ENERGYMETER_EXISTS
  /*
  pinMode(A5, INPUT);  // 
  pinMode(A4, INPUT);  // 
  pinMode(A3, INPUT);  // 
  pinMode(A2, INPUT);  // 
  pinMode(A1, INPUT);  // 
  digitalWrite(A4, HIGH);
  digitalWrite(A3, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A1, HIGH);

  pinMode(A5, INPUT_PULLUP);  //
  pinMode(A4, INPUT_PULLUP);  //
  pinMode(A3, INPUT_PULLUP);  //
  pinMode(A2, INPUT_PULLUP);  //
  pinMode(A1, INPUT_PULLUP);  //  
  */
  

  pinMode(ADE9153A_RESET_PIN, OUTPUT);  // ADE9153A_RESET_PIN
  digitalWrite(ADE9153A_RESET_PIN, HIGH);
  pinMode(ADE9153A_CS_PIN, OUTPUT);  // ADE9153A_SPI_SS_PIN
  digitalWrite(ADE9153A_CS_PIN, HIGH);
  pinMode(ADE9153A_ZX_DREADY_PIN, INPUT);  // ADE9153A_ZX_DREADY_PIN
  pinMode(ADE9153A_IRQ_PIN, INPUT);  // ADE9153A_IRQ_PIN
  pinMode(ADE9153A_CALB_BUTTON, INPUT);  // ADE9153A_USER_BUTTON 
  
  pinMode(ADE9153A_ZX_DREADY_PIN, INPUT_PULLUP);
  pinMode(ADE9153A_IRQ_PIN, INPUT_PULLUP);
  pinMode(ADE9153A_CALB_BUTTON, INPUT_PULLUP);
   
  #endif
  digitalWrite(RELAY_OUT_1, LOW);
  pinMode(RELAY_OUT_1, OUTPUT);  // SS Pin high to avoid miscommunication

  digitalWrite(RELAY_OUT_2, LOW);
  pinMode(RELAY_OUT_2, OUTPUT);  // SS Pin high to avoid miscommunication

  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  
  #if defined (ARDUINO_MEGA)  | defined (ARDUINO_DUE) 
  
  pinMode(LED_GREEN, OUTPUT);           // set pin to input
  digitalWrite(LED_GREEN, LOW);       // turn on pullup resistors
  pinMode(LED_RED, OUTPUT);           // set pin to input
  digitalWrite(LED_RED, LOW);       // turn on pullup resistors


         
   #endif 
/*
  pinMode(KEY_LEFT, INPUT);           // set pin to input
  pinMode(KEY_LEFT, INPUT_PULLUP);

  pinMode(KEY_UP, INPUT);           // set pin to input
  pinMode(KEY_UP, INPUT_PULLUP);

  pinMode(KEY_DOWN, INPUT);           // set pin to input
  pinMode(KEY_DOWN, INPUT_PULLUP);

  pinMode(KEY_RIGHT, INPUT);           // set pin to input
  pinMode(KEY_RIGHT, INPUT_PULLUP);
*/

//  pinMode(KEY_ANALOG, INPUT);  // 
}


void MicroInit() {

  FileSize.MaxSize = MBYTE_1; // MAXLOGFILESIZE
   
  Serial.begin(115200);

  
  // uint16_t temp = SPI_FULL_SPEED;
  //    Serial.print("SPI_FULL_SPEED");
 
//    Serial.println(temp);
  IO_Settings();
  #ifdef OLEDDISPLAY_EXISTS
  DisplaySetPowerIO();
  #endif
  
  ResetCasePrint();
  
   // SDCard.LogStatus = 0;      // default start with log off;
 
    /*  
  uint8_t Mode = NVRam_Read(EE_SAMPLE);
    if((Mode== TASK_500MSEC)||(Mode== TASK_1SEC)||(Mode== TASK_2SEC)||(Mode== TASK_5SEC)||(Mode== TASK_10SEC)||(Mode== TASK_20SEC)||(Mode== TASK_60SEC)){
      SDCard.LogEnable = ON; 
      SampleTime =  Mode;
    }
    else SampleTime =  TASK_2SEC; // default
  */


 
  #if defined (ARDUINO_MEGA)  | defined (ARDUINO_DUE) 
 // UniqueID8dump(Serial);
  // Serial.print("UniqueID: ");
  for (size_t i = 0; i < 8; i++)
  {
    if (UniqueID8[i] < 0x10)
      Serial.print("0");
    Serial.print(UniqueID8[i], HEX);
    Serial.print(".");
  }
  Serial.println();
  #endif

  #ifdef ARDUINO_MEGA
  wdt_reset();
  wdt_enable(WDTO_8S);   //wdt_enable(WDT0_1S);
  //https://www.nongnu.org/avr-libc/user-manual/group__avr__watchdog.html
  #endif

  #ifdef ARDUINO_DUE
  wdt_reset();
  wdt_enable();   
  //https://forum.arduino.cc/t/due-watchdog-timer-not-working/525122/2
  #endif

  #ifdef ARDUINO_MEGA
//  ADCSRA &= ~ (1 << ADEN);            // turn off ADC to save power ,, enable when needed and turn off again
    ADCSRA |= (1 << ADEN); // enable adc
  #endif
  Serial.print(F("Compiled: "));
  Serial.println( __DATE__ ", " __TIME__ ", " __VERSION__); 
 // Serial.println( F("Compiled: ") __DATE__ ", " __TIME__ ", " __VERSION__);
  //Compiled: Jul 21 2020 15:55:39 7.3.0
  //  ShowSerialCode();

  Startup_NV_Ram();
  SDCard.LogBootInit = 0;  // put the header of the csv file

  Serial.print(F("SDCard.LogEnable: "));
  Serial.print(SDCard.LogEnable);
  Serial.print(F("    SampleTime: "));
  Serial.println(SampleTime);
  Serial.print(F("    DisplaySleep: "));
  Serial.println(Display.SleepEnable);

 
//#ifndef DEBUG_SIMULATOR_MODE
   #ifdef SDCARD_EXISTS
 //   SD_Card_Info();
  //  SD_Card_Init();
  //  GetFileSize();
  //  ReadConfigFile(); // 2020.12.25
  #endif  
    Sensors_PeripInit();
    DateTimeBuf.Init = ON;

    exSdCard_Init();

    
  #ifdef OLEDDISPLAY_EXISTS
    DisplayInit();   
 // #endif
  #endif

  #ifdef STM32_F407 
    Serial.println(STM32_TIMER_INTERRUPT_VERSION);
    Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));
    // initialize timer1      // Interval in microsecs
   if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 100, TimerHandler))
    {
      Serial.print(F("Starting ITimer OK, millis() = ")); Serial.println(millis());
    }
    else
      Serial.println(F("Can't set ITimer. Select another freq. or timer"));      
    ISR_Timer.setInterval(TIMER_INTERVAL_10_MS,STM32_IntVectorT1);
  #endif
    
  #ifdef ARDUINO_MEGA
  // initialize timer1
    noInterrupts();           // disable all interrupts
    TCCR1A = 0;
    TCCR1B = 0;
    //    TCNT1 = 34286;            // preload timer 65536-16MHz/256/2Hz 500mS
    TCNT1 = 64286;            // preload timer 65536-16MHz/256/50Hz 20 ms
    TCCR1B |= (1 << CS12);    // 256 prescaler
    TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
    interrupts();
  #endif

  #ifdef ARDUINO_DUE
    #define    ARM_M3_INTERRUPT_FREQ 50
        // 20 -> 50ms   1000/20 = 50
        // 32 -> 32ms       
        // 50 -> 20ms
        // 51 -> 19.62 m2
        // 52 -> 19.22 ms      
        // 54 -> 18.63
        // 64 -> 15.68 ms
        // 100 -> 10 ms
    
    startTimer(TC1, 0, TC3_IRQn, ARM_M3_INTERRUPT_FREQ); //TC1 channel 0, the IRQ for that channel and the desired frequency 64

  #endif
  #ifdef ARDUINO_MKRZERO // ARM Cortex M0
    tcConfigure(50000); //configure the timer to run at <sampleRate>Hertz
    tcStartCounter(); //starts the timer
  #endif


}

void Parse_FileString(){
  int DelimCount=0;
  int j = 0;
  int index;
  for (unsigned int i = 0; i < Config_Str.length(); i++) {
    if (Config_Str.substring(i, i+1) == ",") {
      switch(DelimCount){
        case 0:Relay1str = Config_Str.substring(j, i);
        break;
        case 1:RlStr2 = Config_Str.substring(j,i);
        break;
        case 2:RLlVal = Config_Str.substring(j,i);
        break;
        case 3:RlStr4 = Config_Str.substring(j,i);
        break;
        case 4:Relay2str = Config_Str.substring(j,i);
        break;
        case 5:RlStr6 = Config_Str.substring(j,i);
        break; 
        case 6:RL2Val = Config_Str.substring(j,i);
             j = i+1;
             RlStr8 = Config_Str.substring(j);
        break; 
        default:
        break;   
      }
      j = i+1;
      DelimCount++;
    }
  }
 
    index = ELEMENTS;
    Relay1str.trim();//remove leadig & last space characters
    if(Relay1str == "Relay1"){
     // index == 0; ?????????????????
      RLlVal.trim();  // Temperature // Current // PM25
      for( index = 0; index < ELEMENTS; index++){
        if(RLlVal == KeyWords[index]){      
          RlStr2.trim();
          RlStr4.trim();       
          RL1Min =RlStr2.toFloat();
          RL1Max= RlStr4.toFloat();
          
          break;  
        }  
      } 
   }
   if(index == ELEMENTS) {
        Relay1str = "---";     
        RLlVal = "Nan";   
        RlStr2 = "----";
        RlStr4 = "----";
    }
    index = ELEMENTS;
    Relay2str.trim();//remove leadig & last space characters
    if(Relay2str == "Relay2"){
      index = 0;
      RL2Val.trim();  // Temperature // Current // PM25
      for( index = 0; index < ELEMENTS; index++){
        if(RL2Val == KeyWords[index]){      
          RlStr6.trim();
          RlStr8.trim();       
          RL2Min= RlStr6.toFloat();
          RL2Max= RlStr8.toFloat();
          break;  
        }  
      }     
   }
    if(index == ELEMENTS) {
        Relay2str = "---";     
        RL2Val = "Nan";   
        RlStr6 = "----";
        RlStr8 = "----";
    }
   // Serial.println("'''''''''''''''''");
  Serial.println(Relay1str);
  Serial.println(RlStr2);
  Serial.println(RLlVal);
  Serial.println(RlStr4);
  Serial.println(Relay2str);
  Serial.println(RlStr6);
  Serial.println(RL2Val);
  Serial.println(RlStr8);
}
#define Relay1_Val 8
#define Relay2_Val 4

float GetValue(uint8_t Relay){
  String Val = "";
  if(Relay == Relay1_Val) Val =  RLlVal;
  else if(Relay == Relay2_Val) Val =  RL2Val;
  if(Val == KeyWords[0])return Values.Temperature_Ch1;
  if(Val == KeyWords[1])return Values.Temperature_Ch2;
  if(Val == KeyWords[2])return  Values.Temperature_Ch3;
  if(Val == KeyWords[3])return Values.Humidity_Ch1;
  if(Val == KeyWords[4])return  Values.Humidity_Ch2;
  if(Val == KeyWords[5])return  Values.Humidity_Ch3;
  if(Val == KeyWords[6])return  Values.PM25;
  if(Val == KeyWords[7])return  Values.PM10;
  if(Val == KeyWords[8]) return  Values.Current;
  if(Val == KeyWords[9]) return  Values.Voltage;
  if(Val == KeyWords[10]) return  Values.ActivePower;  
  if(Val == KeyWords[11]) return  Values.PowerFactor;     
  return 0;
}
void Relay_loop() {
 // Parse_FileString();
  CompValue = 0;
  if( RLlVal != "Nan"){
    CompValue = GetValue(Relay1_Val);
    if(CompValue > RL1Max && !digitalRead(RELAY_OUT_1) ) digitalWrite(RELAY_OUT_1,HIGH);
    if(CompValue < RL1Min &&  digitalRead(RELAY_OUT_1) ) digitalWrite(RELAY_OUT_1,LOW); 
  }
  else{
      digitalWrite(RELAY_OUT_1,LOW);
  }
  Serial.print(F("RL1Min: "));Serial.println(RL1Min);
  Serial.print(RLlVal+":  ");Serial.print(CompValue); 
  Serial.print(F("     RELAY1: "));Serial.println(RELAY_OUT_1); 
  Serial.print(F("RL1Max: "));Serial.println(RL1Max); 
  CompValue = 0;
  if( RL2Val != "Nan"){
    CompValue = GetValue(Relay2_Val);
    //if((CompValue < RL2Min)&& RELAY2) RELAY2 = 0;
    //if((CompValue > RL2Max)&& !RELAY2) RELAY2 = 1;  
    if(CompValue > RL2Max && !digitalRead(RELAY_OUT_2) ) digitalWrite(RELAY_OUT_2,HIGH);
    if(CompValue < RL2Min &&  digitalRead(RELAY_OUT_2) ) digitalWrite(RELAY_OUT_2,LOW);  
  }
  else{
       digitalWrite(RELAY_OUT_2,LOW);  
  }
  Serial.print(F("RL2Min: "));Serial.println(RL2Min);
  Serial.print(RL2Val+":  ");Serial.print(CompValue);
  Serial.print(F("      RELAY2: "));Serial.println(RELAY_OUT_2);
  Serial.print(F("RL2Max: "));Serial.println(RL2Max);  
}
void PrintSensor(void){

      Serial.println();

      Serial.print("Ax: ");   //3
      Serial.println(Accelometer.x);   //5

      Serial.print("Ay: ");   //3
      Serial.println(Accelometer.y);   //5

      Serial.print("Az: ");   //3
      Serial.println(Accelometer.z);   //5

      Serial.print("Temp: ");   //3
      Serial.println(Temperature);   //5

      Serial.print("roll: ");   //3
      Serial.println(roll);   //5

      Serial.print("rollF: ");   //3
      Serial.println(rollF);   //5

      Serial.print("pitch: ");   //3
      Serial.println(pitch);   //5

      Serial.print("pitchF: ");   //3
      Serial.println(pitchF);   //5      

      Serial.print("yaw: ");   //3
      Serial.println(yaw);   //5

      Serial.print("yawF: ");   //3
      Serial.println(yawF);   //5     

      Serial.print("Gx: ");   //3
      Serial.println(Gyro.x);   //5

      Serial.print("Gy: ");   //3
      Serial.println(Gyro.y);   //5

      Serial.print("Gz: ");   //3
      Serial.println(Gyro.z);   //5


      Serial.print("roll_2: ");   //3
      Serial.println(roll_2);   //5

      Serial.print("pitch 2: ");   //3
      Serial.println(pitch_2);   //5 

      Serial.print("yaw 2: ");   //3
      Serial.println(yaw_2);   //5 
      

} 
//------------------------------------------------------------------------------
void clearSerialInput() {
  uint32_t m = micros();
  do {
    if (Serial.read() >= 0) {
      m = micros();
    }
  } while (micros() - m < 10000);
}
bool Log_Escape(void){
    uint16_t Esc_Buton = (uint16_t )analogRead(KEY_ANALOG_IN);
    if((Esc_Buton < KEY_RIGHT_ADC_HIGH) && (Esc_Buton > KEY_RIGHT_ADC_LOW)){
        Esc_Count++;
        if(Esc_Count > 4)return ON;    
    } 
    return OFF;    
}
void ExLog_Start(void){
  SDCard.LogEnable = ON; 
  Log_Status = LOG_START; 
}
void ExLog_End(void){
  SDCard.LogEnable = OFF; 
  Log_Status = LOG_OFF; 
  MainMenu =  MENU1_SUB4;//MENU1
}
