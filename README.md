# Interconexão ESP32 + DHT11 + API

Projeto de interconexão entre placas **ESP32** para aquisição de temperatura e umidade com o sensor **DHT11** e disponibilização dos dados por meio de uma **API**.

> **Nota:** os trechos marcados com `[PREENCHER]` dependem de detalhes do código e devem ser ajustados antes da publicação.

---

## Sumário

- [Visão geral](#visão-geral)
- [Arquitetura](#arquitetura)
- [Estrutura do repositório](#estrutura-do-repositório)
- [Materiais necessários](#materiais-necessários)
- [Esquema de ligação](#esquema-de-ligação)
- [Configuração do ambiente](#configuração-do-ambiente)
- [Como executar](#como-executar)
- [Endpoints da API](#endpoints-da-api)
- [Exemplo de resposta](#exemplo-de-resposta)
- [Solução de problemas](#solução-de-problemas)
- [Melhorias futuras](#melhorias-futuras)
- [Autoria](#autoria)

---

## Visão geral

O objetivo do projeto é demonstrar a comunicação entre dois dispositivos ESP32 e a exposição das leituras do sensor DHT11 por meio de uma API, permitindo que outras aplicações (dashboards, aplicativos web, planilhas) consumam os dados.

**Funcionalidades principais:**

- Leitura de temperatura (°C) e umidade relativa (%) com o DHT11.
- Comunicação entre as duas placas ESP32 via `[PREENCHER: HTTP / ESP-NOW / MQTT / Serial]`.
- Disponibilização dos dados por meio de endpoints `[PREENCHER: HTTP/JSON]`.

## Arquitetura

```
┌──────────────┐   [PREENCHER: protocolo]   ┌──────────────┐    HTTP/JSON    ┌───────────────┐
│   ESP32 #1   │ ─────────────────────────▶ │   ESP32 #2   │ ──────────────▶ │ Cliente / App │
│  + DHT11     │                            │  (API/Wi-Fi) │                 │ (navegador,   │
│  (sensor)    │                            │              │                 │  dashboard)   │
└──────────────┘                            └──────────────┘                 └───────────────┘
```

`[PREENCHER: ajustar o diagrama conforme o funcionamento real — por exemplo, se o ESP32 #1 já expõe a API diretamente.]`

## Estrutura do repositório

```
INTERCONEXAO-ESP32-DHT11-API/
├── esp32_1/     # Firmware do primeiro ESP32  [PREENCHER: função, ex.: leitura do DHT11]
├── esp32-2/     # Firmware do segundo ESP32   [PREENCHER: função, ex.: servidor da API]
└── README.md    # Documentação do projeto
```

## Materiais necessários

| Qtde | Componente                          |
| ---- | ----------------------------------- |
| 2    | Placas ESP32 (DevKit)               |
| 1    | Sensor DHT11 (ou módulo com resistor pull-up integrado) |
| 1    | Resistor de 10 kΩ (apenas se o DHT11 não for em módulo) |
| —    | Protoboard e jumpers                |
| 2    | Cabos micro-USB / USB-C (conforme a placa) |
| —    | Rede Wi-Fi 2,4 GHz                  |

## Esquema de ligação

Ligação padrão do DHT11 ao ESP32 (`[PREENCHER: confirmar o pino utilizado no código]`):

| DHT11  | ESP32                |
| ------ | -------------------- |
| VCC    | 3V3                  |
| GND    | GND                  |
| DATA   | GPIO `[PREENCHER]`   |

> O DHT11 requer um resistor pull-up de 10 kΩ entre DATA e VCC, caso a versão utilizada não seja em módulo.

## Configuração do ambiente

### Pré-requisitos

- [Arduino IDE](https://www.arduino.cc/en/software) 2.x **ou** [PlatformIO](https://platformio.org/) `[PREENCHER: ferramenta utilizada]`
- Suporte à placa ESP32 instalado (Gerenciador de Placas: *esp32 by Espressif Systems*)
- Bibliotecas:
  - `DHT sensor library` (Adafruit)
  - `Adafruit Unified Sensor`
  - `[PREENCHER: WiFi, WebServer / ESPAsyncWebServer, ArduinoJson, etc.]`

### Credenciais de rede

Antes de gravar o firmware, ajuste as credenciais de Wi-Fi nos arquivos de cada projeto:

```cpp
const char* ssid     = "NOME_DA_REDE";
const char* password = "SENHA_DA_REDE";
```

> **Atenção:** não publique credenciais reais no repositório.

## Como executar

1. Clone o repositório:
   ```bash
   git clone https://github.com/julianabcor/INTERCONEXAO-ESP32-DHT11-API.git
   cd INTERCONEXAO-ESP32-DHT11-API
   ```
2. Abra o projeto da pasta `esp32_1` na IDE, selecione a placa (*ESP32 Dev Module*) e a porta serial, e grave o firmware.
3. Repita o procedimento para a pasta `esp32-2` na segunda placa.
4. Abra o Monitor Serial (`115200` baud, `[PREENCHER: confirmar]`) e verifique o endereço IP obtido.
5. Acesse a API a partir de um navegador ou cliente HTTP, na mesma rede:
   ```
   http://<IP_DO_ESP32>/[PREENCHER: rota]
   ```

## Endpoints da API

| Método | Rota                | Descrição                                   |
| ------ | ------------------- | ------------------------------------------- |
| GET    | `[PREENCHER: /dados]` | Retorna temperatura e umidade atuais      |
| GET    | `[PREENCHER]`       | `[PREENCHER]`                               |

## Exemplo de resposta

```json
{
  "temperatura": 25.0,
  "umidade": 60.0
}
```

`[PREENCHER: ajustar os nomes dos campos e as unidades conforme o código.]`

Exemplo de consumo com `curl`:

```bash
curl http://<IP_DO_ESP32>/[PREENCHER: rota]
```

## Solução de problemas

| Sintoma                                   | Possível causa / ação                                              |
| ----------------------------------------- | ------------------------------------------------------------------ |
| Leitura `NaN` do DHT11                    | Verificar ligação, pino de dados e resistor pull-up                |
| ESP32 não conecta ao Wi-Fi                | Confirmar SSID/senha e que a rede é de 2,4 GHz                     |
| API inacessível                           | Verificar se o computador está na mesma rede e o IP no Monitor Serial |
| Falha na gravação do firmware             | Segurar o botão BOOT durante o upload; testar outro cabo USB       |
| Dados não chegam ao segundo ESP32         | Conferir o protocolo de comunicação e as configurações de ambas as placas |

## Melhorias futuras

- Substituição do DHT11 por um sensor de maior precisão (DHT22, BME280).
- Armazenamento histórico das leituras (banco de dados ou planilha).
- Dashboard para visualização em tempo real.
- Autenticação nos endpoints da API.

## Autoria

Amanda de Souza Miranda
Ana Clara Rodrigues Abreu
Juliana Branco Cordeiro
Luíza da Silva

## CentroWEG - MA 78

`[PREENCHER: adicionar colegas de equipe, instituição/curso e licença, se aplicável.]`
