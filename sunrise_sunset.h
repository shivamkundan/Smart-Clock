
// internet-related
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

#include <TimeLib.h>

#define STR_SIZE 20
#define SEC_PER_DAY 86400
#define TIME_DIFF_HRS -5 //CDT
// WiFiUDP ntpUDP;
// NTPClient timeClient(ntpUDP, "pool.ntp.org");


String API_LINK="http://api.sunrise-sunset.org/json?lat=37.72045996994535&lon=-89.21522622808743&date=";
// String jsonBuffer;

#define DST_OFFSET 1 	// hours in -1,0,1, etc.

// used for formatting
int H,M;
char *AP;

String prev_date="";

// Store information for 3 days
typedef struct solar_info{
// saves all vals as unix epochs for each date
	unsigned long  FirstLight;
	unsigned long  Sunrise;
	unsigned long  Sunset;
	unsigned long  LastLight;
	solar_info *prev_day;
	solar_info *next_day;
} solar_info;

solar_info TODAY;
solar_info YESTERDAY;
solar_info TOMORROW;


// --------------------------------------------------- //
void print_time_from_epoch(unsigned long epoch){
	char buff[32];
	sprintf(buff, "%02d.%02d.%02d %02d:%02d:%02d", month(epoch), day(epoch), year(epoch), hour(epoch), minute(epoch), second(epoch));
	Serial.println(buff);
}

void make_date_string(char* datestr, unsigned long date) {
	sprintf(datestr, "%4d-%02d-%02d", year(date), month(date), day(date)  );
}

String time_to_str(unsigned long date) {
	char out[STR_SIZE];
	sprintf(out, "%4d-%02d-%02d", year(date), month(date), day(date)  );
	return out;
}

tmElements_t str_to_time(String s){

	tmElements_t T1;
	char am_pm=s[s.length()-2];
	char sunrise_t[s.length()+1];
	for (int i=0;i<s.length()+1;i++)
		sunrise_t[i]=s[i];
	sscanf(sunrise_t, "%d:%d: %s", &H, &M, &AP);

	// convert to 24-hr format
	if (am_pm=='P')
		H+=12;

	H+=DST_OFFSET;

	T1.Hour=H;
	T1.Minute=M;
	return T1;
}

unsigned long convert_to_epoch(unsigned long epochTime, String in_time){
	tmElements_t TS=str_to_time(in_time);
	TS.Day = day(epochTime);
	TS.Month = month(epochTime);
	TS.Year = year(epochTime) - 1970; // because Year is offset from 1970
	time_t TSsec = makeTime( TS );
	// Serial.println(TSsec);
	return TSsec;
}

solar_info init_solar_info(solar_info DAY, time_t epochTime, int offset_days){

	unsigned long t_unix_today = (unsigned long) epochTime;

	// increment/decrement days
	t_unix_today=t_unix_today+(offset_days*SEC_PER_DAY);

	// construct http query string
	String serverPath=API_LINK+time_to_str(t_unix_today);
	// Serial.print("serverPath: ");
	// Serial.println(serverPath);

	// fetch data
	jsonBuffer = httpGETRequest(serverPath.c_str());
	JSONVar myObject = JSON.parse(jsonBuffer);

	// assign to struct as unix epochs
	DAY.FirstLight=convert_to_epoch(t_unix_today,myObject["results"]["civil_twilight_begin"]);
	DAY.Sunrise=   convert_to_epoch(t_unix_today,myObject["results"]["sunrise"]);
	DAY.Sunset=    convert_to_epoch(t_unix_today,myObject["results"]["sunset"]);
	DAY.LastLight= convert_to_epoch(t_unix_today,myObject["results"]["civil_twilight_end"]);

	// Serial.print("civil_twilight_begin: ");
	// Serial.println(myObject["results"]["civil_twilight_begin"]);

	return DAY;
}

void reinit_all_days(time_t epochTime){
	// timeClient.update();
	// time_t epochTime = timeClient.getEpochTime();
	// Get astro stats for each day
	YESTERDAY=init_solar_info(YESTERDAY,epochTime,-1);
	TODAY=init_solar_info(TODAY,epochTime,0);
	TOMORROW=init_solar_info(TODAY,epochTime,1);
}

String short_time(unsigned long date){
	String suffix="";
	char buff[32];
	sprintf(buff, "%02d:%02d", hour(date), minute(date));
	// sprintf(buff, "%02d:%02d:%02d", hour(date), minute(date), second(date));
	// sprintf(buff, "%02d.%02d.%02d %02d:%02d:%02d", month(date), day(date), year(date), hour(date), minute(date), second(date));
	suffix+=" [";
	suffix+=buff;
	suffix+="]";
	return suffix;
}

// --------------------------------------------------- //
String calculate_time_diff(time_t epochTime){

	uint32_t diff=0;
	epochTime = (unsigned long) epochTime;
	String suffix;
	String outstr="";

	if (epochTime > YESTERDAY.LastLight && epochTime  < TODAY.FirstLight){
		diff = TODAY.FirstLight - epochTime;
		suffix=" till first light (1)" + short_time(TODAY.FirstLight);
	}

	if (epochTime > TODAY.FirstLight && epochTime  < TODAY.Sunrise) {
		diff = TODAY.Sunrise - epochTime;
		suffix=" till sunrise" + short_time(TODAY.Sunrise);
	}

	if (epochTime > TODAY.Sunrise && epochTime < TODAY.Sunset) {
		diff = TODAY.Sunset - epochTime;
		suffix=" till sunset" + short_time(TODAY.Sunset);
	}

	if (epochTime > TODAY.Sunset && epochTime < TODAY.LastLight) {
		diff = TODAY.LastLight - epochTime;
		suffix=" till last light" + short_time(TODAY.LastLight);
	}

	if (epochTime > TODAY.LastLight && epochTime<TOMORROW.FirstLight) {
		diff = TOMORROW.FirstLight - epochTime;
		suffix=" till first light (2)" + short_time(TOMORROW.FirstLight);
	}

	uint32_t hours = diff / 3600;
	uint8_t minutes = (diff / 60) % 60;
	uint8_t seconds = diff % 60;

	outstr+=String(hours)+":"+String(minutes);//+":"+String(seconds);
	outstr+=suffix;
	return outstr;
	// Serial.print(String(hours)+":"+String(minutes)+":"+String(seconds)+" ");
	// Serial.println(suffix);
}

// --------------------------------------------------- //
void testPrint(time_t epochTime){
	Serial.print("now: ");
	print_time_from_epoch(epochTime);
	Serial.println();
	print_time_from_epoch(YESTERDAY.FirstLight);
	print_time_from_epoch(YESTERDAY.Sunrise);
	print_time_from_epoch(YESTERDAY.Sunset);
	print_time_from_epoch(YESTERDAY.LastLight);
	Serial.println();
	print_time_from_epoch(TODAY.FirstLight);
	print_time_from_epoch(TODAY.Sunrise);
	print_time_from_epoch(TODAY.Sunset);
	print_time_from_epoch(TODAY.LastLight);
	Serial.println();
	print_time_from_epoch(TOMORROW.FirstLight);
	print_time_from_epoch(TOMORROW.Sunrise);
	print_time_from_epoch(TOMORROW.Sunset);
	print_time_from_epoch(TOMORROW.LastLight);
}
