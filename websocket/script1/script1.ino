/// WIFI Settings ///
//char* ssid     = "FIBRA-A066";
//char* password = "0Z48058288";

#include <ArduinoWebsockets.h>

const char* ssid = "FIBRA-A066"; // Nome da rede
const char* password = "0Z48058288"; // Senha da rede
const char* websockets_server_host = "192.168.25.13"; // IP do servidor websocket
const int websockets_server_port = 3333; // Porta de conexão do servidor
const char* IP = "mensagem do ESP!";

// Utilizamos o namespace de websocket para podermos utilizar a classe WebsocketsClient
using namespace websockets;

// Objeto websocket client
WebsocketsClient client;

// Led
const int led = 23;

void setup() 
{
    // Iniciamos a serial com velocidade de 115200
    Serial.begin(115200);

    // Definimos o pino como saída
    pinMode(led, OUTPUT);

    // Conectamos o wifi
    WiFi.begin(ssid, password);

    // Enquanto não conectar printamos um "."
    while(WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
    }

    // Exibimos "WiFi Conectado"
    Serial.println("Connected to Wifi, Connecting to server.");

    // Tentamos conectar com o websockets server
    bool connected = client.connect(websockets_server_host, websockets_server_port, "/");

    // Se foi possível conectar
    if(connected) 
    {
        // Exibimos mensagem de sucesso
        Serial.println("Connected!");
        // Enviamos uma msg "Hello Server" para o servidor
        client.send(IP);
    }   // Se não foi possível conectar
    else 
    {
        // Exibimos mensagem de falha
        Serial.println("Not Connected!");
        return;
    }
    
    // callback onde as mesagens serão recebidas
    client.onMessage([&](WebsocketsMessage message)
    {        
        // Exibimos a mensagem recebida na serial
        Serial.print("O servidor enviou algo: ");
        Serial.println(message.data());

        // Ligamos/Desligamos o led de acordo com o comando
        if(message.data().equalsIgnoreCase("ON"))
            digitalWrite(led, HIGH);
        
        if(message.data().equalsIgnoreCase("OFF"))
            digitalWrite(led, LOW);
    
});

}
void loop() 
{
          
    //  De tempo em tempo, o websockets client checa por novas mensagens recebidas
    if(client.available()) 
       client.poll();
       client.ping();    
    delay(300);
}
