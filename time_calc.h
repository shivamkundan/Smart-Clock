// #include <TimeLib.h>

// #include <Arduino.h>




// void setup()
// {
//   Serial.begin(115200);

//   tmElements_t T1;
//   tmElements_t T2;

//   T1.Hour = 22;
//   T1.Minute = 49;
//   T1.Second = 59;
//   T1.Day = 24;
//   T1.Month = 12;
//   T1.Year = 2015 - 1970; // because Year is offset from 1970

//   T2.Hour = 0;
//   T2.Minute = 0;
//   T2.Second = 0;
//   T2.Day = 25;
//   T2.Month = 12;
//   T2.Year = 2015 - 1970; // because Year is offset from 1970

//   // convert T1 and T2 to seconds since 1/1/1970
//   time_t T1sec = makeTime( T1 );
//   time_t T2sec = makeTime( T2 );
//   Serial.print( "T1 in seconds since 1970: " );
//   Serial.println( T1sec );
//   Serial.print( "T2 in seconds since 1970: " );
//   Serial.println( T2sec );

//   // differences in seconds
//   int32_t diff = T2sec - T1sec;
//   Serial.print( "Difference between T1 and T2 in seconds: " );
//   Serial.println( diff );

//   // show difference in hours, minutes and seconds
//   uint32_t hours = diff / 3600;
//   uint8_t minutes = (diff / 60) % 60;
//   uint8_t seconds = diff % 60;
//   Serial.print( "T1 was " );
//   Serial.print( hours );
//   Serial.print( " hour(s), " );
//   Serial.print( minutes );
//   Serial.print( " minute(s) and " );
//   Serial.print( seconds );
//   Serial.println( " second(s) before T2." );
// }

// void loop()
// {
// }
