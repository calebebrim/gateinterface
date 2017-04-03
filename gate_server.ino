              
#include "tonedef.h"
#include <SPI.h>
#include <Ethernet.h>



// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

IPAddress ip(192, 168, 25, 100);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);
int pinpulse = 8;
int buzz = 9;
int count_reset = 1000;
//int melody[] = {
//  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
//};

//int noteDurations[] = {
//  4, 8, 8, 4, 4, 4, 4, 4
//};

int melody[] = {
  NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, 0, NOTE_D4, NOTE_FS4, NOTE_G4, NOTE_D5
};


// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  5, 8, 8, 8, 12, 5, 8, 8, 8
};

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  crescent();

  pinMode(pinpulse, OUTPUT);
  pinMode(buzz,OUTPUT);

  digitalWrite(pinpulse, LOW);


  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  //  digitalWrite(pinpulse,HIGH);
  
}


void loop() {
  // listen for incoming clients

  EthernetClient client = server.available();
  if (client) {

    Serial.println("New Client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    int count = 0;
    String header = "";
    while (client.connected()) {
      count++;
      if (client.available()) {
        char c = client.read();
//        Serial.write(c);
        header.concat(c);

        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          Serial.println(header);
          // send a standard http response header
          int action = 1;
          Serial.println(">>>>>");
          
          int getindex = header.indexOf("GET");
          int httpindex = header.indexOf("HTTP/1.1",getindex);
          String params = header.substring(getindex+3,httpindex);
          if(params.indexOf("open=1")>0){
            action = 2;
          }
          Serial.println(getindex);
          Serial.println(httpindex);
          Serial.println(params+"<<<");
          
           
          switch (action){
            case 1:
              printBody(client);
              break;
            case 2:
              Serial.println("Liga controle");
              digitalWrite(pinpulse, HIGH);
          
              delay(500);
          
              Serial.println("Desliga controle");
              digitalWrite(pinpulse, LOW);
              client.println("ok");
            case 3:
            break; 
          }
        
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        } else if(count >= count_reset){
          break;
        }
        

      }
    }
    // give the web browser time to receive the data

    
    
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
    crescent();

  }
}

void crescent() {
  Serial.println("-- Tone Start --");
//  int limit = sizeof(noteDurations);
  int limit = 4;
  for (int thisNote = 0; thisNote < limit; thisNote++) {
    Serial.print("Tone -");
    Serial.println(thisNote);
    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzz, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(buzz);
  }
  Serial.println("Quit Tone");
  

}



void printBody(EthernetClient client){
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");  // the connection will be closed after completion of the response
  //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<link rel='icon' href='data:;base64,iVBORw0KGgo='>");
  client.println("<title>Controle Do Portão</title>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"stylesheet\"  type=\"text/css\" href=\"https://goo.gl/sMujMB\">");//jquery.mobile-1.2.1.min.css

  client.println("<script src=\"https://goo.gl/DYDulT\"></script>"); //jquery.min.js
  client.println("<script src=\"https://goo.gl/LPnBLk\"></script>"); //jquery.mobile.min.js

  client.println("</head>");
  client.println("<body>");
  client.println("<div data-role=\"page\" data-title=\"Controle do portao\">");
  
  client.println("<div data-role=\"header\"><h1>Controle do portao</h1></div>");
  
  client.println("<div data-role=\"content\">");
  client.println("<button class=\"ui-btn\" onclick=\"$.mobile.loading( 'show'); $.get(`open=1`,()=>{$.mobile.loading('hide')})\">Open or Close</button>");
  
  client.println("</div>");
  
//  client.println("<div data-role=\"footer\"><h2>Controle do Portao</h2></div>"); 
  
  client.println("</div>");//page

//  client.println("<div style=\"width: 100%; height: 100%\">");
//  client.println("<div style=\"margin-left: 50%; height: 100%\">");
//  client.print("<form action=\"http://");
//  client.print(Ethernet.localIP());
//  client.println("/\" >");
//  client.println("<input type=\"submit\" value=\"Abrir/Fechar\" style=\"position: relative; left: -30px; top: 100px; font-size: larger;\">");
//  client.println("</form>");
//  
//  client.println("</div>");
//  client.println("</div>");
  client.println("</body>");
  client.println("</html>");  
}


