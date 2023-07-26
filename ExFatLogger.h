


void SD_CardLogTask(){ 
  
}



// Avoid IDE problems by defining struct in septate .h file.
// Pad record so size is a power of two for best write performance.
#ifndef ExFatLogger_h
#define ExFatLogger_h

#endif  // ExFatLogger_h

void logRecord(data_t* data, uint16_t overrun) {
  if (overrun) {  // Add one since this record has no adc data. Could add overrun field.
    overrun++;
    //SensorAcccel_GyroRead();
  }
  SensorAcccel_GyroRead();
         data->Adc_Arr[0] = analogRead(BAT_CURRENT); // let adc to rest in between measurements

         data->Sensor_Arr[0] = Accelometer.x;
         data->Sensor_Arr[1] = Accelometer.y;
         data->Sensor_Arr[2] = Accelometer.z;
         data->Sensor_Arr[3] = Gyro.x;
         data->Sensor_Arr[4] = Gyro.y;
         data->Sensor_Arr[5] = Gyro.z;
         data->Sensor_Arr[4] = Gyro.y;
         data->Sensor_Arr[5] = Gyro.z;

         data->Adc_Arr[1] = analogRead(BAT_VOLT) ;

}
void printRecord(Print* pr, data_t* data) {
  static uint32_t nr = 0;
  if (!data) { // header
    pr->print(F("Sample T.,"));
   // pr->print(Log_Interval);
    pr->print(SampleTime);    
    pr->print(F(",  uSec, Max Dev.,"));
    if(Rate_Fast == ON) maxDelta += SampleTime;
    pr->print(maxDelta);     
    pr->print(F(",  uSec,Tot.Overrun:,"));
    pr->println(totalOverrun);       
    
    pr->print(F("Start Log,"));    
    pr->print(Str_DateLog_Init);
    pr->print(F(", "));
    pr->println(Str_TimeLog_Init);  
    pr->print(F("End  Log,"));    
    pr->print(Str_Date);
    pr->print(F(", "));
    pr->print(Str_Time);  
    pr->print(F(",Log End:, "));
    if(Rate_Fast == ON) pr->println(F("Low Sampl. Rate"));
    else pr->println(F("On Purpose")); 

      
    pr->print(F("rec no"));
    /*
    for (size_t i = 0; i < ADC_COUNT; i++) {
      pr->print(F(",adc"));
      pr->print(i);
    }
    */
    pr->print(F(",Acc x, Acc y, Acc z, Gyr x, Gyr y, Gyr z"));
    pr->print(F(",Bat Cur.,Bat Volt"));
    pr->println();
    nr = 0;
    return;
  }
  /*
  if (data->adc[0] & 0X8000) {
    uint16_t n = data->adc[0] & 0X7FFF;
    nr += n;
    pr->print(F("-1,"));
    pr->print(n);
    pr->println(F(",overuns"));
  } else {
    *(
    pr->print(nr++);
    /*
    for (size_t i = 0; i < ADC_COUNT; i++) {
      pr->write(',');
      pr->print(data->adc[i]);
    }
    pr->println();
    */
    pr->print(nr++);

    pr->write(',');pr->print(data->Sensor_Arr[0]);
    pr->write(',');pr->print(data->Sensor_Arr[1]);
    pr->write(',');pr->print(data->Sensor_Arr[2]);
    pr->write(',');pr->print(data->Sensor_Arr[3]);
    pr->write(',');pr->print(data->Sensor_Arr[4]);
    pr->write(',');pr->print(data->Sensor_Arr[5]);
    pr->write(',');pr->print(data->Adc_Arr[0]);
    pr->write(',');pr->print(data->Adc_Arr[1]);

//    pr->write(',');pr->print(analogRead(BAT_CURRENT));
 //   pr->write(',');pr->print(analogRead(BAT_VOLT));
    pr->println();
    /*
    if (End_Footer) { // 
      pr->print(F("End,"));  
      DateTimeBuf.RTC_Update = OFF;
      RTC_TimeClock();
      pr->print(Str_Date);
      pr->print(F(", "));
      pr->println(Str_Time);
    }
    */
}

//------------------------------------------------------------------------------
void testSensor() {
  const uint32_t interval = 200000;
  int32_t diff;
  data_t data;
  clearSerialInput();
  Serial.println(F("\nTesting - type any character to stop\n"));
  delay(1000);
  printRecord(&Serial, nullptr);
  uint32_t m = micros();
  while (!Serial.available()) {
    m += interval;
    do {
      diff = m - micros();
    } while (diff > 0);
    logRecord(&data, 0);
    printRecord(&Serial, &data);
  }
}
