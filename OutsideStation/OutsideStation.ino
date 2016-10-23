#include <dht11.h>
#include <Bridge.h>
#include <Console.h>
#include <FileIO.h>
#include <HttpClient.h>
#include <Mailbox.h>
#include <Process.h>
#include <YunClient.h>
#include <YunServer.h>

YunServer server;
dht11 dht;

int E1 = 2;
int M1 = 3;
int GROUND_0 = 5;
int GROUND_1 = 4;
int DHT11 = 4;

void setup() {
  // put your setup code here, to run once:
  pinMode(M1, OUTPUT);
  //pinMode(GROUND_0, INPUT);

  Serial.begin(9600);  
  Bridge.begin();
  server.noListenOnLocalhost();
  server.begin();
}

void sendFail(YunClient client, int code, String result) {
  client.print(F("{\"code\": "));
  client.print(code);
  client.print(F(", \"result\": \""));
  client.print(result);
  client.println(F("\"}"));
}

void sendIntResult(YunClient client, int code, int result) {
  client.print(F("{\"code\": "));
  client.print(code);
  client.print(F(", \"result\": \""));
  client.print(result);
  client.println(F("\"}"));  
}

void waterPollControl(YunClient client, int number) {
  if (client.read() != '/') {
    sendFail(client, 500, "invalid params");
    return;
  }
  int on = client.parseInt();
  if (on == 1 && client.read() != '/') {
    sendFail(client, 500, "invalid params");
    return;
  }
  int power = client.parseInt();  
  switch (number) {
    case 0:
      digitalWrite(E1, on);
      analogWrite(M1, power);
      sendIntResult(client, 200, on);
      break;
    case 1:
      break;
    default:
      sendFail(client, 500, "no more water poll");
      break;
  }
}

void groundStatus(YunClient client, int number) {
  switch (number) {
    case 0:
      sendIntResult(client, 200, analogRead(GROUND_0));
      break;
      
    case 1:
      sendIntResult(client, 200, analogRead(GROUND_1));
      break;    
    
    default:
      sendFail(client, 500, "no more ground status");
      break;
  }
}

void envStatus(YunClient client, int number) {
  int chk = dht.read(DHT11);
  switch (chk){
  case DHTLIB_OK:  
    break;
    
  case DHTLIB_ERROR_CHECKSUM: 
    sendFail(client, 500, "DHT11 checksum error.");
    return;
    
  case DHTLIB_ERROR_TIMEOUT: 
    sendFail(client, 500, "DHT11 time out.");
    return;

  default: 
    sendFail(client, 500, "DHT11 unknown error.");
    return;
  }

  switch (number) {
    case 0:
      sendIntResult(client, 200, dht.temperature);
      break;
      
    case 1:
      sendIntResult(client, 200, dht.humidity);
      break;      
    
    default:
      sendFail(client, 500, "no more env status");
      break;
  }
}

void process(YunClient client) {
  String command = client.readStringUntil('/');
  Serial.println(command);
  if (command.startsWith("GET")) {
    String method = client.readStringUntil('/');
    if (method.startsWith("ground")) {
      int number = client.parseInt();
      groundStatus(client, number);
    } else if (method.startsWith("env")) {
      int number = client.parseInt();
      envStatus(client, number);
    } else {
      sendFail(client, 404, "method not be support!");
    }
  } else if (command.startsWith("PUT")) {
    String method = client.readStringUntil('/');
    if (method.startsWith("water")) {
      int number = client.parseInt();
      waterPollControl(client, number);
    } else {
      sendFail(client, 404, "method not be support!");
    }    
  } else if (command.startsWith("POST")) {
    
  } else {
    Serial.print(command);
    Serial.println("not be support!");
    sendFail(client, 404, "method not be support!");
  }
}

void loop() {
  YunClient client = server.accept();
  
  if (client) {
    process(client);    
    client.stop(); 
  }
  delay(50);
}
