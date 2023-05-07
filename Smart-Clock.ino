// https://sunrise-sunset.org/api

// ADD: sunrise, sunset, twilight
// ADD: AQI

// PAGES: Main, Weather, Astro, Settings




// internet-related
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "weather_stuff.h"
#include "sunrise_sunset.h"


//tft-related
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#include <Fonts/OpenSansBold9pt7b.h>
#include <Fonts/OpenSansBold14pt7b.h>
#include <Fonts/OpenSansBold30pt7b.h>
#include <Fonts/Tiny3x3a2pt7b.h>
#include <Fonts/FreeSerifItalic9pt7b.h>
#include <Fonts/FreeSansOblique12pt7b.h>
#include <Fonts/FreeSansBoldOblique12pt7b.h>

// Replace with your network credentials
const char *ssid     = "Wu Tang LAN";
const char *password = "ganja420";

// These are 'flexible' lines that can be changed
#define TFT_CS 17
#define TFT_DC 15
#define TFT_RST -1 // RST can be set to -1 if you tie it to Arduino's reset
#define SCREEN_W 480
// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

// ----- THEMES ----- //

// // Light mode perfect paramss
// int ALT_TXT_COLOR =HX8357_RED;
// int TXT_COLOR =HX8357_BLACK;
// int BG_COLOR =HX8357_WHITE;
// int pwm_lvl=6000;


// Dark mode perfect params
int TXT_COLOR =HX8357_RED;
int ALT_TXT_COLOR =HX8357_YELLOW;
int BG_COLOR =HX8357_BLACK;
int pwm_lvl=4000;


int DATE_TXT_COLOR=TXT_COLOR;




#define TIME_DIFF_HRS -5 //CDT

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "FRIDAY", "SATURDAY"};

//Month names
String months[12]={"January", "February", "March", "04/", "May", "June", "July", "August", "September", "October", "November", "December"};

// ------------------------
// Omly update displays when these values change
String prev_timestr="";
String prev_sec="";
String prev_am_pm="";
// String prev_weekDay="";
String prev_datestr="";
int prev_second=-1;
// ------------------------
String sunrise_t="";
String sunset_t="";
String maxtemp_c="";
String mintemp_c="";
// ------------------------

String BASE_URL = "http://api.weatherapi.com/v1/current.json?";
String API_KEY  = "key=31e4a8ae8c7b43f6a72191923232704";
String ZIP_CODE = "&q=62901";
String AQI      = "&q=62901&aqi=yes";


typedef struct txt_info{
	int16_t x=0;
	int16_t y=0;
	uint16_t w=0;
	uint16_t h=0;
// for storing size and pos of texts
} txt_info;

txt_info T_st;
txt_info S_st;  //
txt_info A_st;  //am_pm string
txt_info D_st;



#define COL1 80
#define ROW1 100
uint16_t col2=COL1+310;
uint16_t row2=ROW1-48;
uint16_t col3=col2;
uint16_t row3=ROW1;

// ----------------------------------------------
void print2(String text, uint16_t col, uint16_t row, int16_t x, int16_t y, uint16_t w, uint16_t h,
			int txt_color,int bg_color, int txt_size, GFXfont font){
	// tft.fillRect(col-3, y-3, w+6, h+6, bg_color);
	tft.setTextSize(txt_size);
	tft.setFont(&font);
	tft.setTextColor(txt_color);
	tft.setCursor(col, row);
	tft.println(text);
}

txt_info new_func(txt_info ST, String text, GFXfont font, uint8_t txt_size, int txt_color,int bg_color,
				 uint16_t col, uint16_t row, uint16_t offset){

	// tft.fillRect(ST.x-2, ST.y-2, ST.w+4, ST.h+4, HX8357_RED); //fill prev text

	ST=get_wh(ST, txt_size, font, col, row, text);

	uint16_t THIS_COL = SCREEN_W - ST.w - 10 - offset;

	print2(text, THIS_COL, row, ST.x, ST.y, ST.w, ST.h, txt_color, bg_color, txt_size, font);

	return ST;
}

txt_info get_wh(txt_info in_struct,
						int txt_size, GFXfont font,
						uint16_t col, uint16_t row,
						String text ){


	int16_t x,y;
	uint16_t w, h;

	tft.setTextSize(txt_size);
	tft.setFont(&font);

	tft.getTextBounds(text, col, row, &x, &y, &w, &h);

	in_struct.x=x;
	in_struct.y=y;
	in_struct.w=w;
	in_struct.h=h;

	return in_struct;
}

// ----------------------------------------------
void get_current_weather(){
	String serverPath = BASE_URL+API_KEY+ZIP_CODE+AQI;

	jsonBuffer = httpGETRequest(serverPath.c_str());
	Serial.println("current weather:");
	Serial.println(jsonBuffer);
	Serial.println();
	// Serial.println(serverPath);
	JSONVar myObject = JSON.parse(jsonBuffer);



	tft.setTextSize(2);
	tft.setFont(&OpenSansBold14pt7b);
	tft.setTextColor(TXT_COLOR);
	// tft.getTextBounds(sec, col2, row2, &x2, &y2, &w2, &h2);
	// tft.fillRect(x2-2, y2-2, w2+4, h2+4, BG_COLOR);

	int col=8;
	tft.setCursor(col, 200);
	tft.print(myObject["current"]["temp_c"]);

	tft.setTextSize(1);
	int _x = tft.getCursorY();
	int _y = tft.getCursorX();


	tft.setTextSize(3);
	tft.setFont(&Tiny3x3a2pt7b);
	tft.setCursor(_y+3, _x-30);
	tft.println("O");

	tft.setTextSize(1);
	tft.setFont(&OpenSansBold14pt7b);
	tft.setCursor(_y+12, _x-18);
	tft.println("C");

	tft.setTextSize(1);
	tft.setFont(&FreeSansBoldOblique12pt7b);
	// tft.setTextSize(1);
	tft.setCursor(col, 235);
	tft.print("Feels: ");
	tft.print(myObject["current"]["feelslike_c"]);
	tft.println("°C");

	tft.setTextSize(1.2);
	tft.setFont(&OpenSansBold14pt7b);
	tft.setCursor(col, 270);
	tft.println(String(myObject["current"]["condition"]["text"]));



	uint16_t start_col=200;
	uint16_t start_row=200;
	uint16_t gap=35;

	tft.setTextSize(2);
	tft.setFont(&OpenSansBold14pt7b);

	tft.setCursor(start_col, start_row);
	tft.print(myObject["current"]["humidity"]);
	tft.println("%");


	tft.setTextSize(1);
	start_col=200;

	tft.setCursor(start_col, 235);
	tft.print(myObject["current"]["pressure_mb"]);
	tft.println("mb");

	start_col=350;
	start_row=190;

	tft.setCursor(start_col, start_row);
	tft.print(myObject["current"]["wind_mph"]);
	tft.println("mph");



	// tft.setCursor(start_col, start_row+gap*3);
	// tft.print("uvi: ");
	// tft.println(myObject["current"]["uv"]);

	// // tft.println(myObject["current"]["temp_c"]);
	// // tft.println(myObject["current"]["condition"]["text"]);


	// // Serial.println(myObject["current"]["wind_mph"]);
	// Serial.println(myObject["current"]["wind_degree"]);
	// Serial.println(myObject["current"]["wind_dir"]);
	// // Serial.println(myObject["current"]["pressure_mb"]);
	// Serial.println(myObject["current"]["precip_mm"]);
	// // Serial.println(myObject["current"]["humidity"]);

	// Serial.println(myObject["current"]["cloud"]);
	// // Serial.println(myObject["current"]["feelslike_c"]);
	// Serial.println(myObject["current"]["vis_miles"]);
	// // Serial.println(myObject["current"]["uv"]);
	// Serial.println(myObject["current"]["gust_mph"]);
}

void get_weather_forecast(){
	String serverPath = "http://api.weatherapi.com/v1/forecast.json?key=31e4a8ae8c7b43f6a72191923232704&q=62901&days=1&aqi=no&alerts=no";

	jsonBuffer = httpGETRequest(serverPath.c_str());
	// Serial.println(jsonBuffer);
	// Serial.println(serverPath);
	JSONVar myObject = JSON.parse(jsonBuffer);
	// Serial.println("--------------");

	Serial.print("curr time:");
	Serial.println(String(myObject["location"]["localtime"]));

	Serial.print("Forecast: ");
	Serial.println(myObject);

	sunrise_t=String(myObject["forecast"]["forecastday"][0]["astro"]["sunrise"]);
	// Serial.println(sunrise);

	sunset_t=String(myObject["forecast"]["forecastday"][0]["astro"]["sunset"]);


	// String moon_phase=String(myObject["forecast"]["forecastday"]["moon_phase"]);



	Serial.print("current from forecast: ");
	Serial.println(myObject["current"]);
	Serial.print("max: ");
	Serial.println(myObject["forecast"]["forecastday"][0]["day"]["maxtemp_c"]);
	Serial.print("min: ");
	Serial.println(myObject["forecast"]["forecastday"][0]["day"]["mintemp_c"]);

	maxtemp_c= String((const char*) myObject["forecast"]["forecastday"][0]["day"]["maxtemp_c"]);
	mintemp_c= String((const char*) myObject["forecast"]["forecastday"][0]["day"]["mintemp_c"]);




	Serial.println();

	tft.setTextSize(1);
	tft.setFont(&FreeSansOblique12pt7b);
	// tft.setFont(&FreeSansBoldOblique12pt7b);
	// String text="6:45 until first light";
	String text=sunrise_t+"  "+sunset_t + "    ";
	// int16_t x,y;
	// uint16_t w, h;
	// tft.getTextBounds(text, 0, 0, &x, &y, &w, &h);
	// tft.setCursor(240-w/2, 140);
	tft.fillRect(0, 280, 480, 35, BG_COLOR);
	tft.setCursor(30, 300);

	tft.print(text);



	tft.setTextColor(HX8357_RED);

	tft.print(myObject["forecast"]["forecastday"][0]["day"]["maxtemp_c"]);
	// tft.setTextColor(HX8357_BLUE);
	tft.print("  ");
	tft.print(myObject["forecast"]["forecastday"][0]["day"]["mintemp_c"]);

	tft.setTextColor(TXT_COLOR);
	// tft.print("  ");
	int start_col=200;
	tft.setCursor(350, 235);
	tft.setFont(&OpenSansBold14pt7b);
	tft.setTextSize(1);
	tft.print(myObject["forecast"]["forecastday"][0]["day"]["daily_chance_of_rain"]);
	tft.print("% rain");
	// 	// +"  "+mintemp_c);


	// tft.print("  ");
	// tft.println(sunrise_t+"  "+sunset_t);

	Serial.print("sunrise:");
	Serial.println(sunrise_t);

	Serial.print("sunset: ");
	Serial.println(sunset_t);

	// Serial.println(myObject["forecast"]["forecastday"][0]["astro"]["moonrise"]);
	// Serial.println(myObject["forecast"]["forecastday"][0]["astro"]["moonset"]);
	// Serial.println(myObject["forecast"]["forecastday"][0]["astro"]["moon_phase"]);
	// Serial.println(myObject["forecast"]["forecastday"][0]["astro"]["moon_illumination"]);

	// Serial.println("--------------");
}

// ----------------------------------------------
String format_hour(int currentHour){
	String hr="";
	if (currentHour>12){
			currentHour=currentHour-12;
			hr=String(currentHour);}
	else
		hr=String(currentHour);

	if (currentHour==0)
		hr=12;

	return hr;
}

String format_minute (int currentMinute) {
	String min="";
	if (currentMinute<10)
		min="0"+String(currentMinute);
	else
		min=String(currentMinute);
	return min;
}

String format_second(int currentSecond){
	String sec="";
	if (currentSecond<10)
		sec="0"+String(currentSecond);
	else
		sec=String(currentSecond);
	// return ("  "+sec);
	return (sec);
}

String format_am_pm(int currentHour){
	String am_pm="";
	if (currentHour<12)
		am_pm+="AM";
	else
		am_pm+="PM";

	return am_pm;
}

String format_date(tm *ptm){
	int monthDay = ptm->tm_mday;
	int currentMonth = ptm->tm_mon+1;
	String datestr=months[currentMonth-1] + String(monthDay); // + ", " +String(currentYear);
	return datestr;
}
// ----------------------------------------------





void blit_updated_time (int currentHour, int currentMinute, int currentSecond,tm *ptm) {

	String hr=format_hour(currentHour);
	String min=format_minute(currentMinute);
	String sec=format_second(currentSecond);
	String am_pm=format_am_pm(currentHour);

	// am_pm="AM";

	String weekDay = weekDays[timeClient.getDay()];
	String datestr=format_date(ptm);

	String timestr=hr+":"+min;

	// -------------------------------------------------

	// am_pm="AM";
	if (am_pm!=prev_am_pm){
		prev_am_pm=am_pm;
		tft.fillRect(385, 60, 95, 48, BG_COLOR);
		A_st=new_func(A_st, am_pm, OpenSansBold14pt7b, 2, TXT_COLOR, BG_COLOR, col3, row3, 0  );
	}

	if (sec!=prev_sec){
		prev_sec=sec;
		tft.fillRect(398, 5, 82, 55, BG_COLOR);
		S_st=new_func(S_st, sec, OpenSansBold14pt7b, 2, TXT_COLOR, BG_COLOR, col2, row2, 0  );
	}


	if (timestr!=prev_timestr){
		prev_timestr=timestr;

		if (am_pm=="AM"){
			tft.fillRect(70, 5, 314, 105, BG_COLOR);
			T_st=new_func(T_st, timestr, OpenSansBold30pt7b, 2, TXT_COLOR, BG_COLOR, COL1, ROW1,102  );}
		else{
			tft.fillRect(70, 5, 330, 105, BG_COLOR);
			T_st=new_func(T_st, timestr, OpenSansBold30pt7b, 2, TXT_COLOR, BG_COLOR, COL1, ROW1,85  );}


		tft.fillRect(0, 150, 480, 150,BG_COLOR);

		Serial.println("----------------------");
		get_current_weather();
		get_weather_forecast();

		timeClient.update();
		time_t epochTime = timeClient.getEpochTime();
		reinit_all_days(epochTime);
		// testPrint(epochTime);
		tft.fillRect(0, 115, 480, 45, BG_COLOR);
		tft.setTextSize(1);
		tft.setCursor(30, 140);
		tft.println(calculate_time_diff(epochTime));
		// Serial.println(calculate_time_diff(epochTime));
		// Serial.println();
		// drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
	}


	if (datestr!=prev_datestr){
		prev_datestr=datestr;

		tft.setTextSize(1);
		tft.setFont(&OpenSansBold9pt7b);
		tft.setTextColor(DATE_TXT_COLOR, BG_COLOR);

		uint16_t col=5;
		uint16_t row=37;
		int16_t x1, y1;
		uint16_t w, h;
		tft.getTextBounds(datestr, col, row, &x1, &y1, &w, &h);


		tft.setCursor(col, row);
		tft.print(String(weekDay));
		tft.setCursor(col, row+25);
		tft.println(String(datestr));



	}
	// tft.drawLine(0, 160, 480, 160, HX8357_RED);

	// tft.drawLine(240, 0, 240, 320, HX8357_MAGENTA);
}
// ----------------------------------------------

void get_time() {
	timeClient.update();
	// String formattedTime = timeClient.getFormattedTime();
	//Get a time structure
	time_t epochTime = timeClient.getEpochTime();
	struct tm *ptm = gmtime ((time_t *)&epochTime);
	int currentHour = timeClient.getHours();
	int currentMinute = timeClient.getMinutes();
	int currentSecond = timeClient.getSeconds();

	if (currentSecond!=prev_second){
		prev_second=currentSecond;
		blit_updated_time(currentHour, currentMinute, currentSecond,ptm);
	}
}

void setup() {
	// Initialize Serial Monitor
	Serial.begin(115200);

	// Connect to Wi-Fi
	Serial.print("Connecting to ");
	Serial.println(ssid);


	// setup pwm
	gpio_set_function(0, GPIO_FUNC_PWM);
	uint slice_num = pwm_gpio_to_slice_num(0);
	pwm_set_wrap(slice_num, 65535);
	pwm_set_chan_level(slice_num, PWM_CHAN_A, pwm_lvl);
	pwm_set_enabled(slice_num, true);

	tft.begin();
	tft.setRotation(1);
	tft.setTextSize(4);
	tft.fillScreen(BG_COLOR);
	tft.setTextColor(ALT_TXT_COLOR, BG_COLOR);
	tft.setCursor(30, 100);
	tft.println("Connecting...");


	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		tft.fillScreen(BG_COLOR);
		tft.setCursor(10, 100);
		tft.println("Still connecting...");
		// Serial.print(".");
		delay(500);
	}

	tft.setTextSize(2);

	// tft.fillScreen(HX8357_WHITE);
	tft.fillScreen(BG_COLOR);

// Initialize a NTPClient to get time
	timeClient.begin();
	// Set offset time in seconds to adjust for your timezone, for example:
	// GMT +1 = 3600
	// GMT +8 = 28800
	// GMT -1 = -3600
	// GMT 0 = 0
	timeClient.setTimeOffset(TIME_DIFF_HRS*3600);

	// timeClient.update();

	// time_t epochTime = timeClient.getEpochTime();

	// reinit_all_days(epochTime);
	// testPrint(epochTime);
}

void loop() {

	// tft.fillScreen(BG_COLOR);



	get_time();


	delay(300);
}
