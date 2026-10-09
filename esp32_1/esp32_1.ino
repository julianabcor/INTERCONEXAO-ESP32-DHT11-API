/*
  ESP32 #1 - PAINEL (sensor local "sala-1" + vários ESP32 remotos)

  Arquivos do sketch: este .ino + pagina.h (HTML/CSS/JS do painel)

  Bibliotecas (Gerenciador de Bibliotecas da Arduino IDE):
    - "DHT sensor library" (Adafruit)
    - "Adafruit Unified Sensor"
  Placa: ESP32 (pacote "esp32" by Espressif Systems)

  Rotas:
    GET  /                       painel (visão geral ou um sensor: /?s=sala-2)
    GET  /sala2                  redireciona para /?s=sala-2
    GET  /api/dados              leitura atual de todos os sensores
    GET  /api/historico?id=X     histórico de um sensor
    POST /api/sensor             recebe {"id":"sala-2","temperatura":24.0,"umidade":61.0}
*/

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DHT.h>

// =====================================================
// CONFIGURAÇÃO
// =====================================================
// true  -> o ESP32 cria a PRÓPRIA rede Wi-Fi (celular e ESP32 remotos se conectam a ela)
//          Endereço do painel: http://192.168.4.1
// false -> o ESP32 entra em uma rede existente (ex.: hotspot do celular)
const bool MODO_AP = true;

// Rede criada pelo ESP32 (MODO_AP = true). A senha deve ter no mínimo 8 caracteres.
const char* AP_SSID  = "Monitor-ESP32";
const char* AP_SENHA = "12345678";

// Rede existente (MODO_AP = false). O ESP32 só funciona em redes de 2,4 GHz.
const char* WIFI_SSID  = "NOME_DA_REDE";
const char* WIFI_SENHA = "SENHA_DA_REDE";

#define PINO_DHT 14          // pino de dados do sensor local
#define TIPO_DHT DHT11       // deve coincidir com o sensor físico (DHT22 ou DHT11)

const char* ID_LOCAL = "sala-1";               // nome do sensor local no painel (reservado)
const unsigned long INTERVALO_LEITURA = 2000;  // ms (o DHT22 suporta no mínimo 2 s)

const int MAX_SENSORES = 7;                    // 1 local + 6 remotos
const int HIST_SENS = 300;                     // leituras guardadas por sensor

// =====================================================
// ESTADO
// =====================================================
DHT dht(PINO_DHT, TIPO_DHT);
WebServer servidor(80);

struct Medidor {
  char id[24];
  bool local;
  bool valido;                 // já recebeu ao menos uma leitura válida
  float t;
  float u;
  unsigned long ultima;        // millis() da última leitura
  unsigned long total;         // total de leituras recebidas
  float hT[HIST_SENS];
  float hU[HIST_SENS];
  unsigned long hMs[HIST_SENS];
  int n;
  int pos;
};
Medidor medidores[MAX_SENSORES];
int numMedidores = 0;

bool leituraOkLocal = false;
unsigned long errosLocal = 0;
unsigned long ultimaLeitura = 0;
unsigned long ultimaTentativaWifi = 0;
bool modoApAtivo = false;

int buscarMedidor(const char* id) {
  for (int i = 0; i < numMedidores; i++) {
    if (strcmp(medidores[i].id, id) == 0) return i;
  }
  return -1;
}

int criarMedidor(const char* id, bool local) {
  if (numMedidores >= MAX_SENSORES) return -1;
  Medidor& m = medidores[numMedidores];
  memset(&m, 0, sizeof(Medidor));
  strncpy(m.id, id, sizeof(m.id) - 1);
  m.local = local;
  return numMedidores++;
}

// Guarda uma leitura na lista (cria o sensor se ainda não existir).
// Retorna false se a lista estiver cheia.
bool registrarLeitura(const char* id, float t, float u) {
  int i = buscarMedidor(id);
  if (i < 0) i = criarMedidor(id, false);
  if (i < 0) return false;
  Medidor& m = medidores[i];
  unsigned long agora = millis();
  m.t = t;
  m.u = u;
  m.ultima = agora;
  m.valido = true;
  m.total++;
  m.hT[m.pos] = t;
  m.hU[m.pos] = u;
  m.hMs[m.pos] = agora;
  m.pos = (m.pos + 1) % HIST_SENS;
  if (m.n < HIST_SENS) m.n++;
  return true;
}

// =====================================================
// PÁGINA (HTML/CSS/JS) - definida em pagina.h
// =====================================================
#include "pagina.h"

// =====================================================
// ROTAS HTTP
// =====================================================
void enviarJson(int codigo, const String& corpo) {
  servidor.sendHeader("Cache-Control", "no-store");
  servidor.sendHeader("Access-Control-Allow-Origin", "*");
  servidor.send(codigo, "application/json", corpo);
}

void rotaRaiz() {
  Serial.println("GET / recebido");
  servidor.sendHeader("Cache-Control", "no-store");
  servidor.send_P(200, "text/html; charset=utf-8", PAGINA, sizeof(PAGINA) - 1);
}

// Atalho antigo: /sala2 abre o painel já no sensor "sala-2"
void rotaSala2() {
  servidor.sendHeader("Location", "/?s=sala-2");
  servidor.send(302, "text/plain", "");
}

// Leitura atual de todos os sensores
void rotaDados() {
  unsigned long agora = millis();
  String j;
  j.reserve(160 + numMedidores * 130);
  j += "{\"intervalo_ms\":" + String(INTERVALO_LEITURA);
  j += ",\"uptime_s\":" + String(agora / 1000);
  j += ",\"sensores\":[";
  for (int i = 0; i < numMedidores; i++) {
    Medidor& m = medidores[i];
    if (i) j += ",";
    j += "{\"id\":\"" + String(m.id) + "\"";
    j += ",\"local\":" + String(m.local ? "true" : "false");
    if (m.valido) {
      j += ",\"t\":" + String(m.t, 1);
      j += ",\"u\":" + String(m.u, 1);
      j += ",\"idade_ms\":" + String(agora - m.ultima);
    } else {
      j += ",\"t\":null,\"u\":null,\"idade_ms\":null";
    }
    j += ",\"n\":" + String(m.total);
    if (m.local) {
      j += ",\"ok\":" + String(leituraOkLocal ? "true" : "false");
      j += ",\"erros\":" + String(errosLocal);
    }
    j += "}";
  }
  j += "]}";
  enviarJson(200, j);
}

// Histórico de um sensor: /api/historico?id=sala-2 (sem id = sensor local)
void rotaHistorico() {
  int i = 0;
  if (servidor.hasArg("id")) {
    String q = servidor.arg("id");
    i = buscarMedidor(q.c_str());
  }
  if (i < 0) {
    enviarJson(404, "{\"erro\":\"sensor desconhecido\"}");
    return;
  }
  Medidor& m = medidores[i];
  unsigned long agora = millis();
  int ini = (m.pos - m.n + HIST_SENS) % HIST_SENS;

  String j;
  j.reserve(m.n * 22 + 120);
  j += "{\"id\":\"" + String(m.id) + "\",\"intervalo_ms\":" + String(INTERVALO_LEITURA) + ",\"t\":[";
  for (int k = 0; k < m.n; k++) {
    if (k) j += ",";
    j += String(m.hT[(ini + k) % HIST_SENS], 1);
  }
  j += "],\"u\":[";
  for (int k = 0; k < m.n; k++) {
    if (k) j += ",";
    j += String(m.hU[(ini + k) % HIST_SENS], 1);
  }
  j += "],\"idade_ms\":[";
  for (int k = 0; k < m.n; k++) {
    if (k) j += ",";
    j += String(agora - m.hMs[(ini + k) % HIST_SENS]);
  }
  j += "]}";
  enviarJson(200, j);
}

// Extrai um número de um JSON simples (sem biblioteca externa)
float extrairNumero(const String& json, const char* chave) {
  String busca = String("\"") + chave + "\"";
  int i = json.indexOf(busca);
  if (i < 0) return NAN;
  i = json.indexOf(':', i);
  if (i < 0) return NAN;
  return json.substring(i + 1).toFloat();
}

// Extrai um texto entre aspas de um JSON simples
String extrairTexto(const String& json, const char* chave) {
  String busca = String("\"") + chave + "\"";
  int i = json.indexOf(busca);
  if (i < 0) return "";
  i = json.indexOf(':', i);
  if (i < 0) return "";
  int a = json.indexOf('"', i + 1);
  if (a < 0) return "";
  int b = json.indexOf('"', a + 1);
  if (b < 0) return "";
  return json.substring(a + 1, b);
}

// Recebe a leitura enviada por um ESP32 remoto (POST /api/sensor)
void rotaSensor() {
  if (!servidor.hasArg("plain")) {
    enviarJson(400, "{\"erro\":\"corpo JSON ausente\"}");
    return;
  }
  String corpo = servidor.arg("plain");
  Serial.print("Recebido: ");
  Serial.println(corpo);

  float t = extrairNumero(corpo, "temperatura");
  float u = extrairNumero(corpo, "umidade");
  if (isnan(t) || isnan(u) || t < -40 || t > 80 || u < 0 || u > 100) {
    enviarJson(400, "{\"erro\":\"valores invalidos\"}");
    return;
  }

  String idTxt = extrairTexto(corpo, "id");
  if (idTxt.length() == 0) idTxt = "remoto";
  char id[24];
  idTxt.toCharArray(id, sizeof(id));
  for (unsigned i = 0; id[i]; i++) {
    char ch = id[i];
    bool valido = (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') ||
                  (ch >= 'A' && ch <= 'Z') || ch == '-' || ch == '_';
    if (!valido) id[i] = '_';
  }

  if (strcmp(id, ID_LOCAL) == 0) {
    enviarJson(409, "{\"erro\":\"id reservado ao sensor local\"}");
    return;
  }
  if (!registrarLeitura(id, t, u)) {
    enviarJson(507, "{\"erro\":\"limite de sensores atingido\"}");
    return;
  }
  enviarJson(200, "{\"ok\":true}");
}

void rotaNaoEncontrada() {
  servidor.send(404, "text/plain", "Rota nao encontrada");
}

// =====================================================
// WI-FI
// =====================================================
void iniciarAp() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_SENHA, 1, 0, 8);   // canal 1, visível, até 8 conexões simultâneas
  modoApAtivo = true;
  Serial.print("Rede criada: ");
  Serial.println(AP_SSID);
  Serial.print("Abra no celular: http://");
  Serial.println(WiFi.softAPIP());
}

void iniciarEstacao() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_SENHA);
  Serial.print("Conectando ao Wi-Fi");
  unsigned long inicio = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - inicio < 20000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Conectado. Abra no celular: http://");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Falha ao conectar. Criando rede propria como alternativa.");
    iniciarAp();
  }
}

// =====================================================
// SETUP / LOOP
// =====================================================
void setup() {
  Serial.begin(115200);
  delay(500);
  dht.begin();

  criarMedidor(ID_LOCAL, true);   // o sensor local é sempre o primeiro da lista

  if (MODO_AP) iniciarAp();
  else iniciarEstacao();

  if (MDNS.begin("monitor")) {
    Serial.println("Tambem disponivel em: http://monitor.local");
  }

  servidor.on("/", HTTP_GET, rotaRaiz);
  servidor.on("/sala2", HTTP_GET, rotaSala2);
  servidor.on("/api/dados", HTTP_GET, rotaDados);
  servidor.on("/api/historico", HTTP_GET, rotaHistorico);
  servidor.on("/api/sensor", HTTP_POST, rotaSensor);
  servidor.onNotFound(rotaNaoEncontrada);
  servidor.begin();
  Serial.println("Servidor HTTP iniciado.");
}

void loop() {
  servidor.handleClient();

  // Reconexão automática (modo rede existente)
  if (!MODO_AP && !modoApAtivo && WiFi.status() != WL_CONNECTED &&
      millis() - ultimaTentativaWifi > 10000) {
    ultimaTentativaWifi = millis();
    WiFi.reconnect();
  }

  // Leitura periódica do sensor local
  if (millis() - ultimaLeitura >= INTERVALO_LEITURA) {
    ultimaLeitura = millis();
    float t = dht.readTemperature();
    float u = dht.readHumidity();

    if (isnan(t) || isnan(u)) {
      errosLocal++;
      leituraOkLocal = false;
      Serial.println("erro");
    } else {
      leituraOkLocal = true;
      registrarLeitura(ID_LOCAL, t, u);   // entra na lista como "sala-1"

      Serial.print(t, 1);       // formato "temperatura,umidade"
      Serial.print(",");
      Serial.println(u, 1);
    }
  }
}