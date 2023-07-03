#define BLYNK_TEMPLATE_ID "TMPL2PbaYo_Qu"
#define BLYNK_TEMPLATE_NAME "FinisGOL"
#define BLYNK_AUTH_TOKEN "bWVVgzfUVxggAGKzswGmHjF0ynVwYLwM"


#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <Stepper.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define IN1 14
#define IN2 27
#define IN3 26
#define IN4 25


//Pinos do Led
#define LED_VERDE_PIN 13
#define LED_AMARELO_PIN 12
#define LED_VERMELHO_PIN 33


//Pinos do Sensor 
#define trigger_pin 5
#define Echo_pin 18


#define BOTAO_DESLIGAR_PIN 15
#define BOTAO_LIGAR_PIN_1 2


long duration;
int distance;

// Task handles
TaskHandle_t sensorTaskHandle;
TaskHandle_t motor1TaskHandle;
TaskHandle_t motor2TaskHandle;


char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "LucasF";
char pass[] = "";

bool systemon = true;

// Define o range máximo e mínimo do sensor ultrassônico
int RANGE_MAXIMO = 408;
const int steps_per_rev = 200;  // Set to 200 for NIMA 17

BlynkTimer timer;
int value0, value1, value2, value3;

Stepper motor(200, 14, 27, 26, 25);


BLYNK_WRITE(V1)
{
  value0 = param.asInt();
  RANGE_MAXIMO = value0;
  Blynk.virtualWrite(V0,RANGE_MAXIMO);
}


void setup() {
  pinMode(trigger_pin, OUTPUT);  // configure the trigger_pin(D9) as an Output
  pinMode(Echo_pin, INPUT);
  pinMode(LED_VERDE_PIN, OUTPUT);
  pinMode(LED_AMARELO_PIN, OUTPUT);
  pinMode(LED_VERMELHO_PIN, OUTPUT);

  motor.setSpeed(50);
  Serial.begin(115200);
  //Blynk.begin(auth, ssid, pass);   


  // Create tasks
 // xTaskCreatePinnedToCore(controllerTask, "ControllerTask", 2048, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(sensorTask, "SensorTask", 2048, NULL, 1, &sensorTaskHandle, 0);
  
  digitalWrite(LED_VERDE_PIN, HIGH);
  digitalWrite(LED_AMARELO_PIN, LOW);
  digitalWrite(LED_VERMELHO_PIN, LOW);

  systemon = true;
}

void loop() {

}
void sensorTask(void *pvParameters) {
      int teste = 200;

  while (1) {
    
    digitalWrite(trigger_pin, LOW);  // set trigger signal low for 2us
    delayMicroseconds(2);

    /*send 10 microsecond pulse to trigger pin of HC-SR04 */
    digitalWrite(trigger_pin, HIGH);  // make trigger pin active high
    delayMicroseconds(10);            // wait for 10 microseconds
    digitalWrite(trigger_pin, LOW);   // make trigger pin active low

    /*Measure the Echo output signal duration or pulse width */
    duration = pulseIn(Echo_pin, HIGH);  // save time duration value in "duration" variable
    distance = duration * 0.034 / 2;     // Convert pulse duration into distance
    
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance <= 11) {
     Serial.print("Entre 5 e 13 ");
      digitalWrite(LED_VERDE_PIN, LOW);
      digitalWrite(LED_AMARELO_PIN, HIGH);
      digitalWrite(LED_VERMELHO_PIN, LOW);
      motor.step(-50);

       delay(1000);
      digitalWrite(LED_VERDE_PIN, HIGH);
      digitalWrite(LED_AMARELO_PIN, LOW);
      digitalWrite(LED_VERMELHO_PIN, LOW);


    }
    else if (distance <= 20){
      digitalWrite(LED_VERDE_PIN, LOW);
      digitalWrite(LED_AMARELO_PIN, HIGH);
      digitalWrite(LED_VERMELHO_PIN, LOW);
      motor.step(50);

       Serial.print("Entre 13 e 30 ");
       delay(1000);
      digitalWrite(LED_VERDE_PIN, HIGH);
      digitalWrite(LED_AMARELO_PIN, LOW);
      digitalWrite(LED_VERMELHO_PIN, LOW);

    }  
    delay(80);


}  
}

void controllerTask(void *pvParameters) {
  while (1) {
    // Ler o estado dos botões
    int desligar = digitalRead(BOTAO_DESLIGAR_PIN);
    int ligar = digitalRead(BOTAO_LIGAR_PIN_1);
    
    // Desligar o sistema se o botão de desligar for pressionado
    if (desligar == HIGH) {
           Serial.print("bUTÃO 1");

    }
    
    // Ligar os pinos 35 e 34 se o botão de ligar for pressionado
    if (ligar == HIGH) {
      
           Serial.print("bUTÃO 2");
    }
    
    // Pequeno atraso para evitar leituras falsas
    delay(100);
  }
}



