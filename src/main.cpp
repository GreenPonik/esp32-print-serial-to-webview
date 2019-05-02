#include "Arduino.h"
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

const char *ssid = "<your_ssid>";
const char *password = "<your_password>";

AsyncWebServer server_http(80);

void log(String message)
{
	Serial.print("incomming message : ");
	Serial.println(message);
	File file = SPIFFS.open("/logs.txt", FILE_APPEND);
	if (!file)
	{
		Serial.println("There was an error opening the file for writing");
		return;
	}

	if (message.length() > 0)
	{
		if (!file.println(message))
		{
			Serial.println("File write failed");
		}
		file.close();
	}
}

#define DEBUGPRINT0(x) \
	log(x);            \
	Serial.print(x);
#define DEBUGPRINTLN0(x) \
	log(x);              \
	Serial.println(x);

const char index_html[] PROGMEM = "<!DOCTYPE html><html><head><meta charset='utf-8'></head><body><h1>Choose Action</h1><ul><li><a href='/logs'>Show logs</a></li><li><a href='/delete'>Delete logs</a></li></ul></body></html>";

static bool logsRemoved = false;
void setup()
{
	Serial.begin(115200);

	if (!SPIFFS.begin(true))
	{
		DEBUGPRINTLN0("An Error has occurred while mounting SPIFFS");
		return;
	}

	if (SPIFFS.exists("/logs.txt"))
	{
		SPIFFS.remove("/logs.txt");
	}
	log("start writing logs");

	WiFi.begin(ssid, password);
	while (!WiFi.isConnected())
	{
		delay(1000);
		DEBUGPRINTLN0("Connecting to WiFi..");
	}

	DEBUGPRINTLN0("Connected to the WiFi network");
	DEBUGPRINTLN0(WiFi.localIP());

	server_http.onNotFound([](AsyncWebServerRequest *request) {
		request->send(400);
	});

	server_http.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "text/html", index_html);
	});
	server_http.on("/logs", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response;
		String ip = request->client()->remoteIP().toString();

		long i = 0;
		String fileLine;
		String fileContent;
		File file = SPIFFS.open("/logs.txt", FILE_READ);
		while (file.available())
		{
			String indexLine = String(i) + ". ";
			fileLine = file.readStringUntil('\n');
			fileContent += indexLine + fileLine;

			i++;
		}
		file.close();

		String logs_html = "<!DOCTYPE html><html><head><meta charset='utf-8'><meta http-equiv='refresh' content='3'/></head><body><h1>Client connected : <strong>" + ip + "</strong></h1><br><a href='/'>Return Home</a><pre style='max-width: 90%;background-color: black; color: white; padding: 20px;'>" + fileContent + "</pre></body></html>";
		response = request->beginResponse(200, "text/html", logs_html);
		request->send(response);
	});
	server_http.on("/delete", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response;
		String ip = request->client()->remoteIP().toString();

		String delete_logs_html = "<!DOCTYPE html><html><head><meta charset='utf-8'></head><body><h1>Client connected : <strong>" + ip + "</strong></h1><br><a href='/'>Return Home</a><form action='/logs/delete' method='post'><button type='submit'>Delete logs</button></form></body></html>";
		response = request->beginResponse(200, "text/html", delete_logs_html);
		request->send(response);
	});
	server_http.on("/delete", HTTP_POST, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = request->beginResponse(200, "text/html", logsRemoved ? "<h1><strong>Logs file removed</strong></h1>" : "<h1><strong>Logs file remove failed</strong></h1>");
		response->addHeader("Connection", "close");
		request->send(response); }, [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {}, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
			if (SPIFFS.exists("/logs.txt"))
			{
				logsRemoved = SPIFFS.remove("/logs.txt");
			} });

	server_http.begin();
}

void loop() {}