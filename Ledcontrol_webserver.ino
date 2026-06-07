  #include <WiFi.h>
  // Replace with your network credentials
  const char* ssid = "Vivo Y30";
  const char* password = "123456789";
  // Set web server port number to 80
  WiFiServer server(80);
  // Variable to store the HTTP request
  String header;
  // Auxiliary variables to store the current output state
  String led1_state = "off";
  String led2_state = "off";
  String led3_state = "off";
  String led4_state = "off";
  // Assign output variables to GPIO pins
  const int led1 = 27;
  const int led2 = 14;
  const int led3 = 12;
  const int led4 = 13;
  // Current time
  unsigned long currentTime = millis();
  // Previous time
  unsigned long previousTime = 0;
  // Define timeout time in milliseconds (example: 2000ms = 2s)
  const long timeoutTime = 2000;

  void setup() {
    Serial.begin(115200);
    delay(1000);
   
    pinMode(led1, OUTPUT);     // Initialize the output variables as outputs
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(led4, OUTPUT);
    
    digitalWrite(led1, LOW);    // Set outputs to LOW
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
   
    Serial.print("Connecting to ");  // Connect to Wi-Fi network with SSID and password
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED && attempt < 20) {
      delay(500);
      Serial.print(".");
      attempt++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
      // Print local IP address and start web server
      Serial.println("");
      Serial.println("WiFi connected.");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      server.begin();
    } else {
      Serial.println("");
      Serial.println("Failed to connect to WiFi.");
    }
  }
  
  void loop() {
    WiFiClient client = server.available();   // Listen for incoming clients

    if (client) {                             // If a new client connects,
      currentTime = millis();
      previousTime = currentTime;
      Serial.println("New Client.");          // Print a message out in the serial port
      String currentLine = "";                // Make a String to hold incoming data from the client
      while (client.connected() && currentTime - previousTime <= timeoutTime) {  // Loop while the client's connected
        currentTime = millis();
        if (client.available()) {             // If there's bytes to read from the client,
          char c = client.read();             // Read a byte, then
          Serial.write(c);                    // Print it out the serial monitor
          header += c;
          if (c == '\n') {                    // If the byte is a newline character
            // If the current line is blank, you got two newline characters in a row.
            // That's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              
              // Turns the GPIOs on and off
              if (header.indexOf("GET /led1/on") >= 0) {
                Serial.println("LED 1 on");
                led1_state = "on";
                digitalWrite(led1, HIGH);
              } else if (header.indexOf("GET /led1/off") >= 0) {
                Serial.println("LED 1 off");
                led1_state = "off";
                digitalWrite(led1, LOW);
              } else if (header.indexOf("GET /led2/on") >= 0) {
                Serial.println("LED 2 on");
                led2_state = "on";
                digitalWrite(led2, HIGH);
              } else if (header.indexOf("GET /led2/off") >= 0) {
                Serial.println("LED 2 off");
                led2_state = "off";
                digitalWrite(led2, LOW);
              } else if (header.indexOf("GET /led3/on") >= 0) {
                Serial.println("LED 3 on");
                led3_state = "on";
                digitalWrite(led3, HIGH);
              } else if (header.indexOf("GET /led3/off") >= 0) {
                Serial.println("LED 3 off");
                led3_state = "off";
                digitalWrite(led3, LOW);
              } else if (header.indexOf("GET /led4/on") >= 0) {
                Serial.println("LED 4 on");
                led4_state = "on";
                digitalWrite(led4, HIGH);
              } else if (header.indexOf("GET /led4/off") >= 0) {
                Serial.println("LED 4 off");
                led4_state = "off";
                digitalWrite(led4, LOW);
              }
              
              // Display the HTML web page
              client.println("<!DOCTYPE html><html>");
              client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<link rel=\"icon\" href=\"data:,\">");
              // CSS to style the on/off buttons
              // Feel free to change the background-color and font-size attributes to fit your preferences
              client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
              client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
              client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
              client.println(".button2 {background-color: #555555;}</style></head>");
              
              // Web Page Heading
              client.println("<body><h1>ESP32 Web Server</h1>");
              
              // Display current state, and ON/OFF buttons for each LED
              client.println("<p>LED 1 - State " + led1_state + "</p>");
              if (led1_state == "off") {
                client.println("<p><a href=\"/led1/on\"><button class=\"button\">ON</button></a></p>");
              } else {
                client.println("<p><a href=\"/led1/off\"><button class=\"button button2\">OFF</button></a></p>");
              }
              
              client.println("<p>LED 2 - State " + led2_state + "</p>");
              if (led2_state == "off") {
                client.println("<p><a href=\"/led2/on\"><button class=\"button\">ON</button></a></p>");
              } else {
                client.println("<p><a href=\"/led2/off\"><button class=\"button button2\">OFF</button></a></p>");
              }
              
              client.println("<p>LED 3 - State " + led3_state + "</p>");
              if (led3_state == "off") {
                client.println("<p><a href=\"/led3/on\"><button class=\"button\">ON</button></a></p>");
              } else {
                client.println("<p><a href=\"/led3/off\"><button class=\"button button2\">OFF</button></a></p>");
              }
              
              client.println("<p>LED 4 - State " + led4_state + "</p>");
              if (led4_state == "off") {
                client.println("<p><a href=\"/led4/on\"><button class=\"button\">ON</button></a></p>");
              } else {
                client.println("<p><a href=\"/led4/off\"><button class=\"button button2\">OFF</button></a></p>");
              }
              
              client.println("</body></html>");
              
              // The HTTP response ends with another blank line
              client.println();
              // Break out of the while loop
              break;
            } else { // If you got a newline, then clear currentLine
              currentLine = "";
            }
          } else if (c != '\r') {  // If you got anything else but a carriage return character,
            currentLine += c;      // Add it to the end of the currentLine
          }
        }
      }
      
      // Clear the header variable
      header = "";
      // Close the connection
      client.stop();
      Serial.println("Client disconnected.");
      Serial.println("");
    }
  }
