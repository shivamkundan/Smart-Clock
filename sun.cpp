

// if (00:00 < curr_t < 04:00){
// 	diff = first_light - curr_t;
// }

// if (04:00 < curr_t < sun rise) {
// 	diff = sunrise - curr_t;
// }

// if (sunrise < curr_t < 16:00) {
// 	diff = sunset - curr_t;
// }

// if (sunset < curr_t < last_light) {
// 	diff = last_light - curr_t;
// }

// if (last_light < curr_t ) {
// 	diff = first_light+1 - curr_t
// }




// 00:00 to [first_light]

// 00:00 to 00:06 -> same date sunrise

// 00:06 to


// // ===========================================
// typedef struct datetime_info{
// 	uint8_t Month;
// 	uint8_t Day;
// 	uint16_t Year;
// 	unsigned long  FirstLight;
// 	unsigned long  Sunrise;
// 	unsigned long  Sunset;
// 	unsigned long  LastLight;
// 	datetime_info prev_day;
// 	datetime_info next_day;
// } datetime_info;



// datetime_info TODAY;




// if (00:00 < curr_t < 04:00){
// 	diff = TODAY[FirstLight] - curr_t;
// }

// if (04:00 < curr_t < TODAY[Sunrise]) {
// 	diff = TODAY[Sunrise] - curr_t;
// }

// if (TODAY[Sunrise] < curr_t < 16:00) {
// 	diff = TODAY[Sunset] - curr_t;
// }

// if (TODAY[Sunset] < curr_t < TODAY[LastLight]) {
// 	diff = TODAY[LastLight] - curr_t;
// }

// if (TODAY[LastLight] < curr_t < 00:00) {
// 	diff = TODAY[next_day][FirstLight] - curr_t
// }

// // ===========================================

// datetime_info TODAY;
// TOMORROW=TODAY[next_day];
// YESTERDAY=TODAY[prev_day];

// if (YESTERDAY[LastLight] < curr_t < TODAY[FirstLight]){
// 	diff = TODAY[FirstLight] - curr_t;
// }

// if (TODAY[FirstLight] < curr_t < TODAY[Sunrise]) {
// 	diff = TODAY[Sunrise] - curr_t;
// }

// if (TODAY[Sunrise] < curr_t < TODAY[Sunset]) {
// 	diff = TODAY[Sunset] - curr_t;
// }

// if (TODAY[Sunset] < curr_t < TODAY[LastLight]) {
// 	diff = TODAY[LastLight] - curr_t;
// }

// if (TODAY[LastLight] < curr_t < TOMORROW[FirstLight]) {
// 	diff = TODAY[next_day][FirstLight] - curr_t
// }


// /*
//   Rui Santos
//   Complete project details at Complete project details at https://RandomNerdTutorials.com/esp32-http-get-post-arduino/

//   Permission is hereby granted, free of charge, to any person obtaining a copy
//   of this software and associated documentation files.

//   The above copyright notice and this permission notice shall be included in all
//   copies or substantial portions of the Software.
// */

// #include <WiFi.h>
// #include <HTTPClient.h>
// #include <Arduino_JSON.h>

// const char* ssid = "REPLACE_WITH_YOUR_SSID";
// const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// //Your Domain name with URL path or IP address with path
// const char* serverName = "http://192.168.1.106:1880/get-sensor";

// // the following variables are unsigned longs because the time, measured in
// // milliseconds, will quickly become a bigger number than can be stored in an int.
// unsigned long lastTime = 0;
// // Timer set to 10 minutes (600000)
// //unsigned long timerDelay = 600000;
// // Set timer to 5 seconds (5000)
// unsigned long timerDelay = 5000;

// String sensorReadings;
// float sensorReadingsArr[3];

// void setup() {
//   Serial.begin(115200);

//   WiFi.begin(ssid, password);
//   Serial.println("Connecting");
//   while(WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("");
//   Serial.print("Connected to WiFi network with IP Address: ");
//   Serial.println(WiFi.localIP());

//   Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
// }

// void loop() {
//   //Send an HTTP POST request every 10 minutes
//   if ((millis() - lastTime) > timerDelay) {
//     //Check WiFi connection status
//     if(WiFi.status()== WL_CONNECTED){

//       sensorReadings = httpGETRequest(serverName);
//       Serial.println(sensorReadings);
//       JSONVar myObject = JSON.parse(sensorReadings);

//       // JSON.typeof(jsonVar) can be used to get the type of the var
//       if (JSON.typeof(myObject) == "undefined") {
//         Serial.println("Parsing input failed!");
//         return;
//       }

//       Serial.print("JSON object = ");
//       Serial.println(myObject);

//       // myObject.keys() can be used to get an array of all the keys in the object
//       JSONVar keys = myObject.keys();

//       for (int i = 0; i < keys.length(); i++) {
//         JSONVar value = myObject[keys[i]];
//         Serial.print(keys[i]);
//         Serial.print(" = ");
//         Serial.println(value);
//         sensorReadingsArr[i] = double(value);
//       }
//       Serial.print("1 = ");
//       Serial.println(sensorReadingsArr[0]);
//       Serial.print("2 = ");
//       Serial.println(sensorReadingsArr[1]);
//       Serial.print("3 = ");
//       Serial.println(sensorReadingsArr[2]);
//     }
//     else {
//       Serial.println("WiFi Disconnected");
//     }
//     lastTime = millis();
//   }
// }

// String httpGETRequest(const char* serverName) {
//   WiFiClient client;
//   HTTPClient http;

//   // Your Domain name with URL path or IP address with path
//   http.begin(client, serverName);

//   // If you need Node-RED/server authentication, insert user and password below
//   //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

//   // Send HTTP POST request
//   int httpResponseCode = http.GET();

//   String payload = "{}";

//   if (httpResponseCode>0) {
//     Serial.print("HTTP Response code: ");
//     Serial.println(httpResponseCode);
//     payload = http.getString();
//   }
//   else {
//     Serial.print("Error code: ");
//     Serial.println(httpResponseCode);
//   }
//   // Free resources
//   http.end();

//   return payload;
// }
