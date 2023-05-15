#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <string.h>

// Definir as credenciais da rede Wi-Fi
const char *ssid = "RAIO-IOT";
const char *password = "Raio-123456789";

// Definir as credenciais do broker MQTT
const char *mqtt_server = "c2d3f38e6eee4b1597daa92b190781c5.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char *mqtt_username = "hivemq.webclient.1684187502185";
const char *mqtt_password = "2wrpi:1W7Q?0NAxZ%#aH";

const char *root_ca =
    "-----BEGIN CERTIFICATE-----\n"
    "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\n"
    "-----END CERTIFICATE-----\n";

const int relay = 18;

String status = "0";

// Inicializar o cliente WiFi e o cliente MQTT
WiFiClientSecure espClient;
PubSubClient client(espClient);

// Função de callback chamada quando uma mensagem é recebida no tópico inscrito
void callback(char *topic, byte *payload, unsigned int length)
{
  if (strcmp(topic, "Sockets") == 0)
  {
    status = "";
    for (int i = 0; i < length; i++)
    {
      status += (char)payload[i];
    }
    Serial.print("Status recebido no tópico [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.println(status);
    Serial.println();
  }
}

void reconnect()
{
  // Loop até que o cliente MQTT esteja conectado ao broker
  while (!client.connected())
  {
    Serial.print("Conectando ao broker MQTT...");
    // Tente conectar o cliente MQTT
    if (client.connect("ESP32Client", mqtt_username, mqtt_password))
    {
      Serial.println("Conectado");
      // Inscreva-se em um tópico MQTT
      client.subscribe("Sockets");
      client.subscribe("EspResponse");
    }
    else
    {
      Serial.print("Falha na conexão com o broker MQTT, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando se reconectar em 5 segundos...");
      // Aguarde 5 segundos antes de tentar novamente
      delay(5000);
    }
  }
}

void setup()
{
  // Inicializar a porta serial para depuração
  Serial.begin(9600);
  pinMode(relay, OUTPUT);
  // Conectar à rede Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Conectando à rede Wi-Fi...");
  }

  // Exibir informações sobre a rede Wi-Fi conectada
  Serial.println("Conectado à rede Wi-Fi");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Configurar o cliente MQTT
  espClient.setCACert(root_ca); // Onde root_ca é a string do certificado de autoridade de certificação do servidor MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop()
{
  int value = digitalRead(relay);
  if (status == "1")
    digitalWrite(relay, HIGH);
  if (status == "0")
    digitalWrite(relay, LOW);

  // Publicação de mensagem no tópico
  if (client.connected() && value != status.toInt())
  {
    //client.publish("EspResponse", "1");
  }
  else
  {
    // Reconexão ao MQTT caso a conexão seja perdida
    reconnect();
  }
  // Mantenha o cliente MQTT conectado e processe as mensagens recebidas
  client.loop();
}