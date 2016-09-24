/*
    This Sketch demonstrates the use of the Extended RemoteSwitch Library  with an ESP8266-01 WiFi module
    The setup is as follows:
    Supply module with 3.3 Volt
    Attach  pin GPIO2 with the signal pin of a 433 Mhz transmitter module
    The names you want to attach to the Buttons are defined in  the array "socketnames"
    the number of sockets can be extended. Pages are dynamically generated based on the
    size of the array as calculated by "ARRAY_SIZE"
    The use of  4 different modules is demonstrated:
    ELRO AB440
    Blokker /SelectRemote 1728029
    EverFlourish EMW203
    Eurodomest 972080
    Strongly dapted from Alexbloggt
    23-09-2016 DIY_bloke
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <RemoteSwitch.h> //FuzzyLogic extended lib

/************************************************
            Object Declarations
 ***********************************************/
ElroAb440Switch ab440Switch(2);
BlokkerSwitch3 blokkerTransmitter(2);// SelectRemote 1728029
EverFlourishSwitch everswitch(2);  // EverFlourish EMW203
Ener002Switch enerswitch(2);      // Eurodomest 972080

//  Fill out the base address of YOUR Eurodomest switch
const unsigned long euro  = 823149;  // base adres eurodomest  : 11001000111101101101  Supply the baseaddress of YOUR Eurodomest
#define ElroAAN ab440Switch.sendSignal(29, 'A', true)
#define ElroUIT ab440Switch.sendSignal(29, 'A', false)
#define BlokkerAAN blokkerTransmitter.sendSignal(1, true)
#define BlokkerUIT blokkerTransmitter.sendSignal(1, false)
#define EverFlourishAAN everswitch.sendSignal('A', 1, true)
#define EverFlourishUIT everswitch.sendSignal('A', 1, false)
#define EnerAAN enerswitch.sendSignal(euro, 1, true)
#define EnerUIT enerswitch.sendSignal(euro, 1, false)
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

MDNSResponder mdns;

// Replace with your network credentials
const char* ssid = "YourSSID";
const char* password = "YourPassword";
ESP8266WebServer server(80);

// Define names of your buttons here, number of buttons will be  automatically adapted
char* socketnames[] = {"ELROAB440", "Blokker", "EverFlourish", "ActionSwitch"};

int numofsockets = ARRAY_SIZE (socketnames);

// sample css and html code
String css = "body {background-color:#ffffff; color: #000000; font-family: 'Century Gothic', CenturyGothic, AppleGothic, sans-serif;}h1 {font-size: 2em;}";
String head1 = "<!DOCTYPE html> <html> <head> <title>RemoteSwitch Demo</title> <style>";
String head2 = "</style></head><body><center>";
String header = head1 + css + head2;
String body = "";

String website(String h, String b)
{
	String complete = h + b;
	return complete;
}

void setup ()
{
	// adapt body part of html if necessary
	body = "<h1>RemoteSwitch Demo</h1>";

	// socket names and buttons are created dynamical
	for (int i = 0; i < numofsockets; i++)
	{
		String namesocket = socketnames[i];
		body = body + "<p>" + namesocket + " <a href=\"socket" + String(i) + "On\"><button>ON</button></a>&nbsp;<a href=\"socket" + String(i) + "Off\"><button>OFF</button></a></p>";
	}

	body += "</center></body>";
	delay(1000);

	Serial.begin(115200);
	WiFi.begin(ssid, password);
	Serial.println("");

	// Wait for connection
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}

	// serial output of connection details
	Serial.println("");
	Serial.print("Connected to ");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	if (mdns.begin("esp8266", WiFi.localIP()))
	{
		Serial.println("MDNS responder started");
	}

	// this page is loaded when accessing the root of esp8266´s IP
	server.on("/", []()
	{
		String webPage = website(header, body);
		server.send(200, "text/html", webPage);
	});

	/************************************************
	                      Dynamically creating pages
	 *********************************************** */
	for (int i = 0; i < numofsockets; i++)
	{
		String pathOn = "/socket" + String(i) + "On";
		const char* pathOnChar = pathOn.c_str();
		String pathOff = "/socket" + String(i) + "Off";
		const char* pathOffChar = pathOff.c_str();

		//content ON page
		server.on(pathOnChar, [i]()
		{
			String webPage = website(header, body);
			server.send(200, "text/html", webPage);

			switch (i)
			{
				case 0:
					ElroAAN;
					break;
				case 1:
					BlokkerAAN;
					break;
				case 2:
					EverFlourishAAN;
					break;
				case 3:
					EnerAAN;
					break;
				default:
					break;

			}
			delay(500);
		});

		//content OFF page
		server.on(pathOffChar, [i]()
		{
			String webPage = website(header, body);
			server.send(200, "text/html", webPage);

			switch (i)
			{
				case 0:
					ElroUIT;
					break;
				case 1:
					BlokkerUIT;
					break;
				case 2:
					EverFlourishUIT;
					break;
				case 3:
					EnerUIT;
					break;
				default:
					break;

			}
			delay(500);
		});
	}
	server.begin();
	Serial.println("HTTP server started");
}

void loop(void)
{
	server.handleClient();
}
