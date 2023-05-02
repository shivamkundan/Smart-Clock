#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <TimeLib.h>

String jsonBuffer;

String httpGETRequest(const char* serverName) {
	WiFiClient client;
	HTTPClient http;

	// Your Domain name with URL path or IP address with path
	http.begin(client, serverName);

	// Send HTTP POST request
	int httpResponseCode = http.GET();

	String payload = "{}";

	if (httpResponseCode>0) {
	// Serial.print("HTTP Response code: ");
	// Serial.println(httpResponseCode);
	payload = http.getString();
	}
	else {
	Serial.print("Error code: ");
	Serial.println(httpResponseCode);
	}
	// Free resources
	http.end();

	return payload;
}
