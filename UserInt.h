char Dispbuffer[22];  // make sure this is large enough for the largest string it must hold
char* CopyFlashToRam(const char* p){
  //length.Dispbuffer; 
    for (uint8_t i = 0; i < 22; i++) {
      Dispbuffer[i] = 0;
    }
    for (uint8_t i = 0; i < strlen_P(p); i++) {
       char c = pgm_read_byte_near(p + i);
      Dispbuffer[i] = c;
      //Serial.print(c);
    }
   //  Serial.print(F("Dispbuffer : ")); Serial.println(Dispbuffer);
    return Dispbuffer;
}
void UpdateDisplayBuffer(void){  


 
    Display_Line1 = Str_Date + "   ";
    Display_Line1 +=  Str_Time;
   // Display_Line1 =  String(Key.Adc); 
    
  #ifdef SDCARD_EXISTS
        // old sd card
  //  UpdateSD_LogTime();// Line2
 //   UpdateFileSize();// Line3
 //   ConvertFileSize(FileSize.Total);// Line3  
      //#define ARRSIZE 21
    String str;

    /*
    switch(exSD_Status){
      case DEF_FILE_NAME: str = "                     "; break;
      case SDCARD_INIT:   str = " SD Card Initilised  ";break;
      case BINFILE_CREATE:str = " Binary File Created ";break;
      case CSVFILE_CREATE:str = " CSV File Created    ";break;
      case SDCARD_PROBLEM:str = " SD Card Problem !!  ";break;
      case BINFILE_FAIL:  str = "Binary File Failed !!";break;
      case CSVFILE_FAIL:  str = " Csv File Failed !!  ";break;
     default:str = " No Info ??????      ";break;
     break;                            
    }
    */
      switch(exSD_Status){
      case DEF_FILE_NAME: str = CopyFlashToRam(EX_LOG_MESS0);break;
      case SDCARD_INIT:   str = CopyFlashToRam(EX_LOG_MESS1);break;
      case BINFILE_CREATE:str = CopyFlashToRam(EX_LOG_MESS2);break;
      case CSVFILE_CREATE:str = CopyFlashToRam(EX_LOG_MESS3);break;
      case SDCARD_PROBLEM:str = CopyFlashToRam(EX_LOG_MESS4);break;
      case BINFILE_FAIL:  str = CopyFlashToRam(EX_LOG_MESS5);break;
      case CSVFILE_FAIL:  str = CopyFlashToRam(EX_LOG_MESS6);break;
     default:str = CopyFlashToRam(EX_LOG_MESS7);break;                        
    }  
    Display_Line2 =  str;
    Display_Line3 =  String(binName);

  #endif
  
    UpdateProperLine(DispRollIndex[0], 4); // Line 4
    UpdateProperLine(DispRollIndex[1], 5); // Line 5
    UpdateProperLine(DispRollIndex[2], 6); // Line 6
    UpdateProperLine(DispRollIndex[3], 7); // Line 7
    
    // DisplayFullSensors();   
    UpdateDisplayMenu(); // Line8    
}
// Flash // RAM   // Usage
// 61438 // 2856  // UpdateFileSize
// 61926 // 2848 // ConvertFileSize with log
// 61898 // 2848 // ConvertFileSize without log
void ConvertFileSize(uint32_t Number){
  #define MAXDIGIT 10  // max 10 char 9.999.999.999 max 10GB
  #define ARRSIZE 21
    char str[ARRSIZE]; uint8_t i = 0;uint8_t Digit = 1;uint32_t Division=1 ;
    //Serial.print("Number ");Serial.println(Number);
    for( i = 0; i < ARRSIZE; i++)
       str[i] = ' '; //clean all    
              
   for(uint32_t j = Number ; j >= 10; j /= 10){
         Digit++;
         Division *= 10;
   }
    i = MAXDIGIT- Digit; // max 10 char 9.999.999.999 max 10GB 
    for(; Digit != 0; Digit--){
        str[i++] =  (Number /Division) +'0';
        if((Digit == 10) ||  (Digit == 4) || (Digit == 7)){
            str[i++] ='.';
        }
        Number %= Division;
        Division /= 10;
    }
    str[i++] = ' ';   //10 1
    str[i++] = 'B';
    str[i++] = 'y';
    str[i++] = 't';
    str[i++] = 'e';
    str[i++] = 's';  
    for( i = 0; i < ARRSIZE; i++){
      Display_Line3[i] =  str[i];
    }
    /*
   // i= ARRSIZE;
    Display_Line3[i++] = ' ';   //10 1
    Display_Line3[i++] = 'B';
    Display_Line3[i++] = 'y';
    Display_Line3[i++] = 't';
    Display_Line3[i++] = 'e';
    Display_Line3[i++] = 's';  
    */
     // Display_Line3 += " Bytes";
    //  Serial.print("Display_Line3 ");Serial.println(Display_Line3);
   //     Serial.print(str[i]);
}

void UpdateFileSize(){
    String str;unsigned long Remain;
    str = "    "; //4
    if( FileSize.Total == 0) str += " ";//10
    
      if(FileSize.Total < 100000000)str += ' ';   // 100 Mb     
      if(FileSize.Total < 10000000)str += ' ';   // 10 Mb    
      if(FileSize.Total < 1000000)str += ' ';   // 1 Mb
      if(FileSize.Total < 100000)str += ' ';   // 100 kb
      if(FileSize.Total < 10000)str += ' ';   // 10 kb      
      if(FileSize.Total < 1000)str += ' ';   // 1 kb
      if(FileSize.Total < 100)str += ' ';   // 100 byte
      if(FileSize.Total < 10)str += ' ';   // 10  byte  
      if(FileSize.Total >= 1000000){  // 1 mb   // find out the 1millions dot place and remaining leading zeros
        str += String(FileSize.Total / 1000000);  // 9 digit 
        str +='.';  // 9 digit 
        Remain = FileSize.Total % 1000000;        
        if(Remain < 100000)str += '0'; //  _50000
        if(Remain < 10000)str += '0';  //  __5000
        if(Remain < 1000)str += '0';  //  ___500
      
        if(Remain  >= 1000){
          unsigned long Remain2 = Remain % 1000;
          str += String(Remain / 1000);  // 9 digit 
          str +='.';   // 9 digit
          if(Remain2 < 100)str += '0';
          if(Remain2 < 10)str += '0';
          str += String(Remain2);  // 9 digit       
        }  
      }
      else if(FileSize.Total >= 1000){ // find out the 1000s dot place and remaining leading zeros
        Remain = FileSize.Total % 1000;
        str += String(FileSize.Total / 1000);  // 9 digit 
        str +='.';   // 9 digit 
        if(Remain < 100)str += '0';
        if(Remain < 10)str += '0';
        str += String(Remain);  // 9 digit 
      }
      else str += String(FileSize.Total); // less than 1000 
             
      str += String(" Bytes"); //9 digit  total 15 
        
      Display_Line3 = str;//LimitCopyDisplayStr(str,MAX_DISPLAY_CHAR);
}

void PrintDisplayBuffer(void){
/*
      Serial.println(Display_Line1);
      Serial.println(Display_Line2);
      Serial.println(Display_Line3);
      Serial.println(Display_Line4);
      Serial.println(Display_Line5);
      Serial.println(Display_Line6);
      Serial.println(Display_Line7);
      Serial.println(Display_Line8);
      Serial.println();
 

    // Additionals
    Serial.print(F("DevId: "));Serial.print(Device_Id);
    
    Serial.print(F(" SensorOnBrd: "));Serial.print(SensorId.OnBoard,HEX);   
    Serial.print(F("    Sensor1: "));Serial.print(SensorId.No1,HEX);
    Serial.print(F("    Sensor2: "));Serial.print(SensorId.No2,HEX);
    Serial.print(F("    Sensor3: "));Serial.print(SensorId.No3,HEX);
    */ 
    //Compiled: Jul 21 2020 15:55:39 7.3.0

    /*
    Serial.print(F("   Version: ")); 
    Serial.println(F(__VERSION__));  // 11 1 8  
  //  Serial.print(F("File: ")); 
  //  Serial.println(F(__FILE__));  // 11 1 8 
 
  //  Serial.print(F("Cpp Standart: ")); 
 //   Serial.print(__cplusplus);  // 11 1      
   // Serial.print(F("Line: ")); 
   // Serial.println(__LINE__);  // 11 1 8   

    Serial.print(F("  DATE:")); 
    Serial.print(__DATE__);  // 11 1 8        
    Serial.print(F("  TIME:")); 
    Serial.println(__TIME__);  // 11 1 8 
    */ 
     /*   
    Serial.print(F("Repository: "));  

    for (byte k = 0; k < strlen_P(Repository); k++){
     char c = pgm_read_byte_near(Repository + k);
      Serial.print(c);
    } 
    */
/*
    Serial.print(F("MainMenu No: ")); Serial.println(MainMenu);  // 11 1 8  
    Serial.print(F("Key.Adc: ")); Serial.println(Key.Adc);  // 11 1 8  
    Serial.print(F("Display.OLED_Timer: ")); Serial.println(Display.OLED_Timer);  // 11 1 8  
    Serial.print(F("Log_Status  : ")); Serial.println(Log_Status);  // 11 1 8     
    Serial.print(F("SDCard.LogEnable : ")); Serial.println(SDCard.LogEnable);  // 11 1 8       
    Serial.print(F("SampleTime : ")); Serial.println(SampleTime);  // 11 1 8        
        Serial.println(); 

    */    
   #ifdef ARDUINO_MEGA         
    char buffer[75];
    for (int i = 0; i < 5; i++) {
      strcpy_P(buffer, (char *)pgm_read_word(&(Terminal_table[i])));  // Necessary casts and dereferencing, just copy.
      Serial.println(buffer);
    } 
    #endif
/*
    int result = system(NULL);  
    Serial.print(F("int result = system(NULL): ")); 
    Serial.println(result);  // 11 1 8   
  */      
  //  Serial.println();         
  //  Serial.println();   
    
//61164
//2890
    
  /*  
     Serial.print(F("GNUC: ")); 
    Serial.println(__GNUC__ );  
     Serial.print(F("GNUC_MINOR: ")); 
    Serial.println(__GNUC_MINOR__ );  
     Serial.print(F("GNUC_PATCHLEVEL: ")); 
    Serial.println(__GNUC_PATCHLEVEL__  );  
*/
   // Serial.print(F("RELAY 1:"));Serial.println(digitalRead(RELAY_OUT_1));
   // Serial.print(F("RELAY 2:"));Serial.println(digitalRead(RELAY_OUT_2));   
     
  //  Serial.println();
  //  Serial.println();     
}
 /*
  String s = "geeksforgeeks";   
    int n = s.length();  
    // declaring character array 
    char char_array[n + 1]; 
    // copying the contents of the 
    // string to char array 
    strcpy(char_array, s.c_str()); 
*/

#define MAXNOCHAR 4
void UpdateProperLine(uint8_t Index, uint8_t Line){
    #define TEMPERATURE_DEGREE_ASCII 247  // FOR OLED DISPLAY https://en.wikipedia.org/wiki/Code_page_437
    String CelsiusDegree = "";  
    CelsiusDegree = (char)TEMPERATURE_DEGREE_ASCII;
    CelsiusDegree +=  "C";
    
    String str = String(Index)+ ".";    
    switch(Index){
      case DISPROLL_LINE0: 
            str = "";//// show nothing                         
      break; 
        case DISPROLL_LINE1:  
             #ifdef BATTERY_SCOOTER_EXISTS 
                str += "B_C." + String(Values.Battery_Current) + " B_V "+ String(Values.Battery_Voltage);
             #endif  
                
              #ifdef VOLTAGE_MEASURE_EXISTS
               // str += " " + String(Mains_Volt) + "V ";  
                str += " " + String(Mains_Volt) + "V ";  
             #endif
             #ifdef LEM_CURRENT_EXISTS               
                str += String(Current_Mains_Rms) + "A ";  
            #endif

           /* 
          if (!isnan(Values.Temperature_OnBoard)) {
            str += String(Values.Temperature_OnBoard,1);
            str += CelsiusDegree;
            //str += F(" C");  Display.ExpSensOnb = ON;                           
          }
          else  str += F("------");       
          if (!isnan(Values.Humidity_OnBoard)) {
            str +=F(" %");
            //str += String((int)Values.Humidity_OnBoard); // 10...
            str += String(Values.Humidity_OnBoard,1); // 10...
          }
          else   str +=F("----"); 
       //   str += ' ' + String(SensorId.OnBoard, HEX);
            str += F(" Internal Internal Internal Internal ");
            */
      break;        
      case DISPROLL_LINE2:          
            str += F("Ax:");  
            str += String(Accelometer.x,1);//
            str += F(" y:");  
            str += String(Accelometer.y,1);//  
            str += F(" z:");  
            str += String(Accelometer.z,1);//  
                
      /*
          if (!isnan(Values.Temperature_Ch1)) {
            str += String(Values.Temperature_Ch1,1);
            str += CelsiusDegree;
            //str += F(" C");  Display.ExpSens1 = ON;                           
          }
          else  str += F("------");       
          if (!isnan(Values.Humidity_Ch1)) {
            str +=F(" %");
            str += String((int)Values.Humidity_Ch1); // 10...
          }
          else   str +=F("----"); 
          str += ' ' + String(SensorId.No1, HEX);
          */
     break;
     case DISPROLL_LINE3:
     /*
          //str = "4."; // temp sensor2
          if (!isnan(Values.Temperature_Ch2)) {
            str += String(Values.Temperature_Ch2,1);
            str += CelsiusDegree;
           // str += F(" C");   Display.ExpSens2 = ON;               //  str += '°'; 
          }
          else  str += F("------");        
          if (!isnan(Values.Humidity_Ch2)) {
            str +=F(" %");
            str += String((int)Values.Humidity_Ch2); // 
          }
          else   str +=F("----");
          str += ' ' + String(SensorId.No2, HEX);  
         
         if (!isnan(Accelometer.x)) {
            str += F("x:");  
            str += String(Accelometer.x,1);//7
            str += F("y:");             //9
            str += String(Accelometer.y,1);//14
            str += F("z:");     //16
            str += String(Accelometer.z,1);//21
                      
        }
        else  str += F("------");     
    */ 

            str += F("Gx:");  
            str += String(Gyro.x,1);//
            str += F(" y:");  
            str += String(Gyro.y,1);//  
            str += F(" z:");  
            str += String(Gyro.z,1);//  

              
   
     break;
     case DISPROLL_LINE4:   
     /*
        //str = "5."; // temp sensor3
         if (!isnan(Values.Temperature_Ch3)) {
            str += String(Values.Temperature_Ch3,1);
            str += CelsiusDegree;
            //str += F(" C");    Display.ExpSens3 = ON;
        }
        else  str += F("------");  
        if (!isnan(Values.Humidity_Ch3)) {
          str +=F(" %");
          str += String((int)Values.Humidity_Ch3); // 
        }
        else   str +=F("----");   // 10 lines
        str += ' ' + String(SensorId.No3, HEX); 
           
         if (!isnan(Gyro.x)) {
            str += F("x:");  //2
            str += String(Gyro.x,1);//6
            str += F(" y:");      //9           
            str += String(Gyro.y,1); //13
            str += F(" z:");       // 16
            str += String(Gyro.z,1); //20                     
        }
        else  str += F("------"); 
     */ 
            str += F("Az:");  
            str += String(Accelometer.z,1);//
            str += F(" Gz:");  
            str += String(Gyro.z,1);//                  
     break;
     case DISPROLL_LINE5:   
        //str += Display_LineTry;

             #ifdef BATTERY_SCOOTER_EXISTS 
                str += "B_C." + String(Values.Battery_Current) + " B_V "+ String(Values.Battery_Voltage);
             #endif  
                
        
              #ifdef VOLTAGE_MEASURE_EXISTS
               // str += " " + String(Mains_Volt) + "V ";  
                str += " " + String(Mains_Volt) + "V ";  
             #endif
             #ifdef LEM_CURRENT_EXISTS               
                str += String(Current_Mains_Rms) + "A ";  
            #endif

           #ifdef ENERGYMETER_EXISTS 
             if(EnergyMeterIC.Error){
                //str += ICERROR;
                 str += CopyFlashToRam(ICERROR);
             }              
             else if(EnergyMeterIC.Mode == POWERIC_NORMAL){            
                str +=String(Values.Current)+ "A "; // 3/4/2 = 9
                str +=String((uint16_t)Values.Voltage)+ "V "; //4/2  =15
                str += String(Values.Frequency)+ "Hz"; //4 = 22               
             }        
             else if((EnergyMeterIC.Mode == POWERIC_SETUP1) || (EnergyMeterIC.Mode == POWERIC_SETUP2) || (EnergyMeterIC.Mode == POWERIC_SETUP3)){
                   //str += SETTINGUP; // 3/4/2
                   str += CopyFlashToRam(SETTINGUP);    
             }
             else   //str += CALIBRATING; // 3/4/2   
                      str += CopyFlashToRam(CALIBRATING);        
          #endif          
     break;      
     case DISPROLL_LINE6:       
             #ifdef ENERGYMETER_EXISTS 
              if(EnergyMeterIC.Error){
                  //  str += ICERROR;
                    str += CopyFlashToRam(ICERROR); 
              }             
              else if(EnergyMeterIC.Mode == POWERIC_NORMAL){       
                str += F(" ");
                str += String(Values.ActivePower); // 3/4/3
                str += F(" W PF ");                   
              //  str += String(powerVals.ActivePowerValue/1000);  // 3/4/1        
                str += String(Values.PowerFactor);  // 3/4/1                       
              }
              else if((EnergyMeterIC.Mode == POWERIC_SETUP1) || (EnergyMeterIC.Mode == POWERIC_SETUP2) || (EnergyMeterIC.Mode == POWERIC_SETUP3)){
                  //str += SETTINGUP; // 3/4/2
                  str += CopyFlashToRam(SETTINGUP); 
             }    
             else   //str += CALIBRATING;//str += "Calibrating!";  // 3/4/2  
                      str += CopyFlashToRam(CALIBRATING);               
            #endif 
        #ifdef  LIDAR_EXISTS
              str += F("Lidar ");
              str += String(Values.Lidar,5);

        #endif        

             #ifdef SOLAR_PANEL_EXISTS 
                str += "Sol " + String(Values.Solar_Current) + "mA  "+ String(Values.Solar_Voltage)+ "V";
             #endif  
                               
     break; 
     case DISPROLL_LINE7:
            #ifdef PM25_DUST_SENSOR_EXISTS         
              str += F(" PM2.5: ");
              //  str += "7. PM2.5: ";
                  if(Values.PM25 < 100.00)str +=  String(Values.PM25,1);
                  else str += String(Values.PM25,0);
            #endif               
     break;  
     case DISPROLL_LINE8:    
          str += "R1:";
          str += String(digitalRead(RELAY_OUT_1)); //7 + 1   
          str += " ";  
         // str +=  String((unsigned int)RL1Min) +   RLlVal  +  String((unsigned int)RL1Max);
          for(int i = 0; i < MAXNOCHAR; i++){
            str += RlStr2[i];  // Limit Str length to 4 20.4 // 124. // 1378
          }      
          str += " " ;   
          str += RLlVal ; 
          str += " ";           
          for(int i = 0; i < MAXNOCHAR; i++){
            str += RlStr4[i];
          }        
      break;  
      case DISPROLL_LINE9:       
          str += "R2:" +String(digitalRead(RELAY_OUT_2))+ " "; //7 + 1
          for(int i = 0; i < MAXNOCHAR; i++){
            str += RlStr6[i];  // Limit Str length to 4 20.4 // 124. // 1378
          }      
          /*
          str += " " +  RL2Val + " "; 
          for(int i = 0; i < MAXNOCHAR; i++){
            str += RlStr8[i];
          }   
          */     
      break;                
      default: str = F("default");
      break; 
    }
    uint16_t Length = str.length();
    if(Length > MAX_DISPLAY_CHAR){ // 34 > 21 //Limit the string to display size
      str.remove(MAX_DISPLAY_CHAR, (Length - MAX_DISPLAY_CHAR));// 21, (34-21) 13
      //str =  String(Index) +  "..." + String(Line);
      //str +=  F(".error");
    }
    if(Length < MAX_DISPLAY_CHAR){// Fill the string to reach full 
      for(uint16_t i = 0; i < (MAX_DISPLAY_CHAR - Length);i++){
        str.setCharAt(Length+i, 'x');
      }    
    }
    
    switch(Line){
      case 4:    
          Display_Line4 = str;//LimitCopyDisplayStr(str,MAX_DISPLAY_CHAR);                  
      break; 
      case 5:    
          Display_Line5 = str;//LimitCopyDisplayStr(str,MAX_DISPLAY_CHAR);             
      break;
      case 6:   
          Display_Line6 = str;//LimitCopyDisplayStr(str,MAX_DISPLAY_CHAR);
      break;
      case 7:
          Display_Line7 = str;   
      break;
      default:              // 
          Display_Line4 = "";Display_Line5 = "";Display_Line6 = "";Display_Line7 = "";
      break;
    }        
}

#ifdef SDCARD_EXISTS

void UpdateSD_LogTime(){
    String str;
    if(SDCard.PauseTimer){
      if(SDCard.Status == SD_NOT_Present)str = F("SD Error!  Retry-> "); //18
      if(SDCard.FatError == ON)          str = F("FAT Error! Retry-> "); //18      
      str += String(SDCard.PauseTimer);//13
      Display_Line2 = str;//LimitCopyDisplayStr(str,MAX_DISPLAY_CHAR); 
      return;     
  }
     if(SDCard.Status != SD_NOT_Present){
    //if((SDCard.Status != SD_NOT_Present) || (SDCard.Status != SD_NO_FAT)){
         switch(Display.ValueTimer){
            case 0:
            case 2:
            case 4:       
                if(SDCard.Status == SD1_TYPE)      str = F("SD1 ");         
                else if(SDCard.Status == SD2_TYPE) str = F("SD2 ");           
                else if(SDCard.Status == SDHC_TYPE)str = F("SDH ");
                str += String(SDCard.Volume);   // 4+5+3
                str +=  F("Gb ") ;   // 4+5+3
                
         break;
          case 1:
          case 3: 
                str = String(LOG_FILE) ; // 12
         break;
   //   case 4:
   //      break;
         default:
         break;  
    }           
      str += "   "; // 3   

      switch(SampleTime){
       case TASK_1MSEC:str += CopyFlashToRam(LOG_1MSEC); //5 
          break; 
       case TASK_5MSEC:str += CopyFlashToRam(LOG_5MSEC); //5 
          break;     
       case TASK_10MSEC:str += CopyFlashToRam(LOG_10MSEC); //5 
          break; 
       case TASK_20MSEC:str += CopyFlashToRam(LOG_20MSEC); //5 
          break;       
       case TASK_50MSEC:str += CopyFlashToRam(LOG_50MSEC); //5 
          break; 
       case TASK_100MSEC:str += CopyFlashToRam(LOG_100MSEC); //5 
          break;           
       case TASK_250MSEC:str += CopyFlashToRam(LOG_250MSEC); //5 
          break;          
       case TASK_500MSEC:str += CopyFlashToRam(LOG_500MSEC); //5 
          break;
          default:        
        case TASK_1SEC : str += CopyFlashToRam(LOG_1SEC);//5
          break; 
        case TASK_2SEC : str += CopyFlashToRam(LOG_2SEC);
          break;        
        case TASK_5SEC : str += CopyFlashToRam(LOG_5SEC);
          break;  
        case TASK_10SEC :str += CopyFlashToRam(LOG_10SEC);
          break; 
        case TASK_20SEC :str += CopyFlashToRam(LOG_20SEC);
          break;            
        case TASK_60SEC :str += CopyFlashToRam(LOG_60SEC);
          break;     
      } 
       /*
      switch(SampleTime){
       case TASK_50MSEC:str += F(" 50 mS"); //5 
          break; 
       case TASK_100MSEC:str += F("100 mS"); //5 
          break;           
       case TASK_250MSEC:str += F("250 mS"); //5 
          break;          
       case TASK_500MSEC:str += F("500 mS"); //5 
          break;        
        case TASK_1SEC : str += F("  1Sec");//5
          break; 
        case TASK_2SEC : str += F("  2Sec");
          break;        
        case TASK_5SEC : str += F("  5Sec");
          break;  
        case TASK_10SEC :str += F(" 10Sec");
          break; 
        case TASK_20SEC :str += F(" 20Sec");
          break;            
        case TASK_60SEC :str += F(" 60Sec");
          break;     
      }  
          */   
    }

    else str = F("SD Error             ");
    Display_Line2 = str;//LimitCopyDisplayStr(str,MAX_DISPLAY_CHAR); 
}
#endif
