

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define MQ135_PIN 26  
#define GAS_THRESHOLD 300 
#define LED_PIN 25      
#define BUZZER_PIN 27   

float temp, humidity;
int gasValue;

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  dht.begin();
  Wire.begin();  


  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("System Init...");
  display.display();
  delay(1000);
}


void loop() {
  temp = dht.readTemperature();
  humidity = dht.readHumidity();
  gasValue = analogRead(MQ135_PIN);

  bool gasAlert = gasValue > GAS_THRESHOLD;
  bool tempAlert = temp > 35.0;

  
  if (gasAlert || tempAlert) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }

  
  if (gasAlert || tempAlert) {
    Serial.print("ALERT! Temp: "); Serial.print(temp);
    Serial.print("  Gas: "); Serial.println(gasValue);
  }

  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("T: "); display.print(temp); display.println(" C");
  display.print("H: "); display.print(humidity); display.println(" %");
  display.print("Gas: "); display.println(gasValue);
  display.display();

  delay(2000);
}
