#define BLYNK_TEMPLATE_ID "TMPL6hAMjEw5a"
#define BLYNK_TEMPLATE_NAME "Arduino robot"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "esp_camera.h"
#include <WebServer.h>

// Blynk & WiFi Credentials
char auth[] = "yK1JCmkljStP-GInAouRYisqj_HkRSrI";
char ssid[] = "Shaswata's S23 FE";
char pass[] = "Spyder_robot";

// ESP32-CAM Pins
#define PWDN_GPIO_NUM  32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM   0
#define SIOD_GPIO_NUM  26
#define SIOC_GPIO_NUM  27
#define Y9_GPIO_NUM    35
#define Y8_GPIO_NUM    34
#define Y7_GPIO_NUM    39
#define Y6_GPIO_NUM    36
#define Y5_GPIO_NUM    21
#define Y4_GPIO_NUM    19
#define Y3_GPIO_NUM    18
#define Y2_GPIO_NUM     5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM  23
#define PCLK_GPIO_NUM  22

// Simple web server for streaming
WebServer server(80);
String localIP = "";

void setup() {
  Serial.begin(115200); // For Nano communication
  //Serial2.begin(115200, SERIAL_8N1, 16, 17);
  
  // Initialize Camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_SVGA; // Smaller for better performance
  config.pixel_format = PIXFORMAT_JPEG;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 20; // Higher compression for better performance
  config.fb_count = 1;
  
  // Initialize camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
  }
  
  // Connect to WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Get IP address
  localIP = WiFi.localIP().toString();
  
  // Print the IP address to serial monitor
  Serial.println("");
  Serial.print("Camera Ready! Use 'http://");
  Serial.print(localIP);
  Serial.println("' to connect");
  
  // Set up a simple streaming server
  server.on("/", HTTP_GET, handleRoot);
  server.on("/stream", HTTP_GET, handleStream);
  server.begin();
  
  Serial.println("HTTP server started");

  // Start Blynk after everything else is set up
  Blynk.begin(auth, ssid, pass);
  
  // Send IP address to Blynk
  Blynk.virtualWrite(V8, "Camera stream at: \nhttp://" + localIP);
}

void handleRoot() {
  String html = "<html><head>";
  html += "<title>ESP32-CAM Robot Stream</title>";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<style>";
  html += "body { text-align: center; font-family: Arial; margin: 0px auto; padding: 0px; }";
  html += "img { width: 100%; height: auto; max-width: 640px; }";
  html += "</style></head><body>";
  html += "<h1>ESP32-CAM Robot Stream</h1>";
  html += "<img src=\"/stream\" id=\"stream\">";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handleStream() {
  WiFiClient client = server.client();
  
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: multipart/x-mixed-replace; boundary=frame");
  client.println();
  
  while (client.connected()) {
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      delay(1);
      continue;
    }
    
    client.println("--frame");
    client.println("Content-Type: image/jpeg");
    client.println("Content-Length: " + String(fb->len));
    client.println();
    client.write(fb->buf, fb->len);
    client.println();
    
    esp_camera_fb_return(fb);
    delay(10);
  }
}

// Blynk Button Handlers (Forward commands to Nano)
BLYNK_WRITE(V0) { if (param.asInt()) Serial.write('F'); } // Forward
BLYNK_WRITE(V1) { if (param.asInt()) Serial.write('B'); } // Back
BLYNK_WRITE(V2) { if (param.asInt()) Serial.write('L'); } // Left
BLYNK_WRITE(V3) { if (param.asInt()) Serial.write('R'); } // Right
BLYNK_WRITE(V4) { if (param.asInt()) Serial.write('W'); } // Wave
BLYNK_WRITE(V5) { if (param.asInt()) Serial.write('D'); } // Dance
BLYNK_WRITE(V6) { if (param.asInt()) Serial.write('S'); } // Stand
BLYNK_WRITE(V7) { if (param.asInt()) Serial.write('C'); } // Sit


// Send IP address to Blynk widget when connected
BLYNK_CONNECTED() {
  // Re-send IP address in case of reconnection
  Blynk.virtualWrite(V8, "Camera stream at: \nhttp://" + localIP);
}

void loop() {
  Blynk.run(); // Handle Blynk
  server.handleClient(); // Handle web streaming requests
}
