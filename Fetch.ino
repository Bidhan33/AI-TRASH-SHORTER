#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define CAMERA_MODEL and include camera_pins.h
#define CAMERA_MODEL_WROVER_KIT
#include "camera_pins.h"

// Define the boundary as a macro for string concatenation
#define BOUNDARY "----WebKitFormBoundary7MA4YWxkTrZu0gW"

// WiFi credentials and API endpoint
const char* ssid_Router = "IT hertz when IP";
const char* password_Router = "wachtwoord";
const char* apiEndpoint = "https://j7mp36xg-3000.euw.devtunnels.ms/api/upload";
const int pins[] = { 12, 33, 32, 2 };
const int NUM_PINS = 4;
const int NUM_LEDS = 7;

const int LED_CONFIGS[7][2] = {
  { 0, 1 },  // LED 1: A to B
  { 0, 2 },  // LED 2: A to C
  { 0, 3 },  // LED 3: A to D
  { 1, 2 },  // LED 4: B to C
  { 1, 3 },  // LED 5: B to D
  { 2, 3 },  // LED 6: C to D
  { 2, 0 }   // LED 7: C to A
};

const int buttonPin = 13;  // Button connected to pin 13
const int I2C_SDA = 14;    // New SDA pin
const int I2C_SCL = 15;    // New SCL pin
bool buttonPressed = false;
bool imageReady = false;

camera_config_t config;
bool wifiConnected = false;

HTTPClient http;

// Define head and tail for multipart/form-data
const char* head = "--" BOUNDARY "\r\n"
                   "Content-Disposition: form-data; name=\"image\"; filename=\"image.jpg\"\r\n"
                   "Content-Type: image/jpeg\r\n\r\n";

const char* tail = "\r\n--" BOUNDARY "--\r\n";

camera_fb_t* fb = NULL;  // Declare fb globally

// LCD setup
LiquidCrystal_I2C lcd(0x27, 20, 4);  // Set the LCD address to 0x27 for a 20 chars and 4 line display

void updateLCD(int line, const char* message) {
  lcd.setCursor(0, line);
  lcd.print("                    ");  // Clear the line (20 spaces)
  lcd.setCursor(0, line);
  lcd.print(message);
}

void updateLeds(int ledIndex, bool state) {
  // If invalid LED index or state is false, turn all LEDs off
  if (ledIndex < 0 || ledIndex >= NUM_LEDS || !state) {
    // Set all pins to input (high impedance) to turn off all LEDs
    for (int i = 0; i < NUM_PINS; i++) {
      pinMode(pins[i], INPUT);
    }
    return;
  }

  // First, set all pins to input mode (high impedance)
  for (int i = 0; i < NUM_PINS; i++) {
    pinMode(pins[i], INPUT);
  }

  // Get source and sink pins for the requested LED
  int sourcePin = pins[LED_CONFIGS[ledIndex][0]];
  int sinkPin = pins[LED_CONFIGS[ledIndex][1]];

  // Configure pins for the selected LED
  pinMode(sourcePin, OUTPUT);
  pinMode(sinkPin, OUTPUT);
  digitalWrite(sourcePin, HIGH);
  digitalWrite(sinkPin, LOW);
}

void camera_init() {
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_VGA;  // Adjust as needed
  config.pixel_format = PIXFORMAT_JPEG;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 15;  // Lower quality for smaller size
  config.fb_count = 1;       // Single framebuffer
}

void connectToWiFi() {
  updateLCD(0, "Connecting WiFi...");
  WiFi.begin(ssid_Router, password_Router);
  int attempts = 0;

  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    updateLCD(0, "WiFi connected");
    delay(1000);
    updateLCD(0, "Ready to scan!");
    wifiConnected = true;
  } else {
    updateLCD(0, "WiFi failed!");
    wifiConnected = false;
  }
}

void setup() {
  Serial.begin(115200);

  // Initialize I2C with new pins
  Wire.begin(I2C_SDA, I2C_SCL);

  lcd.init();
  lcd.backlight();
  updateLCD(0, "Initializing...");

  pinMode(buttonPin, INPUT_PULLUP);

  camera_init();

  esp_err_t err = esp_camera_init(&config);

  if (err != ESP_OK) {
    updateLCD(0, "Camera Init Failed!");
    while (1) {
      delay(1000);
    }
  }

  connectToWiFi();

  for (int i = 0; i < NUM_PINS; i++) {
    pinMode(pins[i], INPUT);
  }
}

void captureImage() {
  updateLCD(0, "Scanning...");
  updateLCD(1, "");
  updateLeds(-1, false);  // Turn off all LEDs when starting new scan

  if (fb) {
    esp_camera_fb_return(fb);
  }

  fb = esp_camera_fb_get();
  if (!fb) {
    updateLCD(0, "Capture failed!");
    updateLCD(1, "");
    delay(2000);
    updateLCD(0, "Ready to scan!");
    updateLCD(1, "");
    return;
  }

  imageReady = true;
}

// Helper function to map category to LED index
int getCategoryLedIndex(const char* category) {
  if (strcmp(category, "plastic") == 0) return 0;
  if (strcmp(category, "paper") == 0) return 1;
  if (strcmp(category, "cardboard") == 0) return 2;
  if (strcmp(category, "bio_waste") == 0) return 3;
  if (strcmp(category, "mixed_waste") == 0) return 4;
  if (strcmp(category, "metal") == 0) return 5;
  if (strcmp(category, "glass") == 0) return 6;
  return -1;  // Category not found or return_to_store
}

void uploadImage() {
  if (!fb) {
    updateLCD(0, "No image!");
    updateLCD(1, "");
    return;
  }

  updateLCD(0, "Scan complete!");
  updateLCD(1, "Thinking...");

  size_t headLen = strlen(head);
  size_t tailLen = strlen(tail);
  size_t totalLen = headLen + fb->len + tailLen;

  uint8_t* body = (uint8_t*)malloc(totalLen);
  if (!body) {
    updateLCD(0, "Memory error!");
    updateLCD(1, "");
    updateLeds(-1, false);
    return;
  }

  memcpy(body, head, headLen);
  memcpy(body + headLen, fb->buf, fb->len);
  memcpy(body + headLen + fb->len, tail, tailLen);

  if (http.begin(apiEndpoint)) {
    http.addHeader("Content-Type", "multipart/form-data; boundary=" BOUNDARY);
    http.addHeader("Content-Length", String(totalLen));

    int httpCode = http.POST(body, totalLen);
    free(body);

    if (httpCode > 0) {
      String payload = http.getString();
      StaticJsonDocument<1024> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        // Clear display before showing results
        for (int i = 0; i < 4; i++) {
          updateLCD(i, "");
        }

        // Display object
        updateLCD(0, "Object:");
        if (doc.containsKey("object")) {
          String object = doc["object"].as<String>();
          updateLCD(1, object.c_str());
        } else {
          updateLCD(1, "Unknown");
        }

        // Display category
        updateLCD(2, "Bin:");
        const char* keys[] = { "plastic", "paper", "cardboard", "bio_waste",
                               "mixed_waste", "metal", "glass", "return_to_store" };
        const char* displayNames[] = { "Plastic", "Paper", "Cardboard", "Bio waste",
                                       "Mixed waste", "Metal", "Glass", "Return to store" };
        bool categoryFound = false;

        // Turn off all LEDs first
        updateLeds(-1, false);

        for (int i = 0; i < 8; i++) {
          if (doc.containsKey(keys[i]) && doc[keys[i]].as<bool>()) {
            updateLCD(3, displayNames[i]);
            int ledIndex = getCategoryLedIndex(keys[i]);
            if (ledIndex >= 0) {  // Only light LED if it's not return_to_store
              updateLeds(ledIndex, true);
            }
            categoryFound = true;
            break;
          }
        }

        if (!categoryFound) {
          updateLCD(3, "Check packaging");
          updateLeds(-1, false);  // Turn off all LEDs
        }
      } else {
        updateLCD(0, "Parse error!");
        updateLCD(1, "");
        updateLeds(-1, false);
      }
    } else {
      updateLCD(0, "Upload failed!");
      updateLCD(1, "");
      updateLeds(-1, false);
    }
    http.end();
  } else {
    updateLCD(0, "Connection failed!");
    updateLCD(1, "");
    updateLeds(-1, false);
  }

  delay(5000);  // Show result for 5 seconds
  updateLCD(0, "Ready to scan!");
  for (int i = 1; i < 4; i++) {
    updateLCD(i, "");
  }
  updateLeds(-1, false);  // Turn off LEDs after showing result

  imageReady = false;
}

void loop() {
  if (!wifiConnected) {
    connectToWiFi();
  }

  if (wifiConnected) {
    if (digitalRead(buttonPin) == LOW && !buttonPressed) {
      buttonPressed = true;
      captureImage();
    } else if (digitalRead(buttonPin) == HIGH) {
      buttonPressed = false;
    }

    if (imageReady) {
      uploadImage();
    }

    if (WiFi.status() != WL_CONNECTED) {
      wifiConnected = false;
      updateLCD(0, "WiFi disconnected");
      updateLCD(1, "");
      updateLeds(-1, false);  // Turn off LEDs on WiFi disconnect
    }
  }

  delay(100);
}