/*
  ESP32 #2 - SENSOR REMOTO (DHT11)
  Conecta-se à rede criada pelo ESP32 #1 e envia as leituras por HTTP POST.

  Bibliotecas: "DHT sensor library" (Adafruit) e "Adafruit Unified Sensor".
  HTTPClient e WiFi já acompanham o pacote "esp32".
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// Mesma rede/senha definidas no ESP32 #1 (AP_SSID / AP_SENHA)
const char* WIFI_SSID  = "Monitor-ESP32";
const char* WIFI_SENHA = "12345678";

// IP fixo do ESP32 #1 quando ele funciona como ponto de acesso (MODO_AP = true)
const char* URL_PAINEL = "http://192.168.4.1/api/sensor";
const char* ID_SENSOR  = "sala-2";   // até 23 caracteres

#define PINO_DHT 14
#define TIPO_DHT DHT11

const unsigned long INTERVALO_ENVIO = 3000;  // ms

DHT dht(PINO_DHT, TIPO_DHT);
unsigned long ultimoEnvio = 0;

void conectarWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_SENHA);
  Serial.print("Conectando a ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado. IP: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  dht.begin();
  conectarWifi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) conectarWifi();

  if (millis() - ultimoEnvio < INTERVALO_ENVIO) return;
  ultimoEnvio = millis();

  float t = dht.readTemperature();
  float u = dht.readHumidity();
  if (isnan(t) || isnan(u)) {
    Serial.println("Erro de leitura do DHT");
    return;
  }

  char corpo[96];
  snprintf(corpo, sizeof(corpo),
           "{\"id\":\"%s\",\"temperatura\":%.1f,\"umidade\":%.1f}",
           ID_SENSOR, t, u);

  HTTPClient http;
  http.begin(URL_PAINEL);
  http.addHeader("Content-Type", "application/json");  // obrigatório para o ESP32 #1 ler o corpo
  int codigo = http.POST(corpo);
  Serial.print("Enviado: ");
  Serial.print(corpo);
  Serial.print(" -> HTTP ");
  Serial.println(codigo);   // 200 = recebido; -1 = sem conexão; 400 = dados inválidos
  http.end();
}