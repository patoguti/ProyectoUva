#include <dht.h>
#include <LowPower.h>
#include <SD.h>
#include <SPI.h>
#include <DS1302.h>


//sensor luminosidad flying fish////////
//rango de valores: 0 - 1023
//puerto A0
//valor 1023 alta luminosidad
int sensorLuminosidad = A0;
String luminosidad = "";
///////////////////////////////////////

/////////////sensor lluvia/////////////
int sensorLluvia = A1;
String lluvia = "";
///////////////////////////////////////

///////////////RTC(reloj)//////////////
#define SCK_PIN 7
#define IO_PIN 6
#define RST_PIN 5
DS1302 rtc(RST_PIN, IO_PIN, SCK_PIN);
String dia;
String fecha;
String hora;
///////////////////////////////////////

/////////////DHT////////////////////
#define DHT11_PIN 2
dht DHT;
String dhtTemp = "";
String dhtHum = "";
////////////////////////////////////

////////////////SD////////////////////
int cs = 10;
File myFile;
const char* archivo = "testall.txt";
//////////////////////////////////////

//cantidad de tiempo a dormir arduino
int tiempoStandBy = 1; // tiempoStandyBy*8 + 4 = total segundos
////////////////////////////////////

void setup() {
  Serial.begin(9600);
  pinMode(cs, OUTPUT);
  pinMode(SS, OUTPUT);
  pinMode(sensorLuminosidad,INPUT);
  while (!SD.begin(cs)) {
    Serial.println("SD no se ha iniciado");   
    delay(1000);
  }
  Serial.println("SD inicializada.");
  delay(1000);
}

void loop() {
  dia = rtc.getDOWStr();
  fecha= rtc.getDateStr();
  hora= rtc.getTimeStr();
  DHT.read11(DHT11_PIN); 
  guardarInfo();
  Serial.flush();
  
  //dormir arduino
  for(int j = 0; j< tiempoStandBy; j++){
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
  delay(4000);
}
//guarda los valores de los sensores en el archivo indicado
void guardarInfo(){
  luminosidad = obtenerLuminosidad();
  dhtTemp = obtenerDhtTemp();
  dhtHum = obtenerDhtHum();
  lluvia = obtenerLluvia();
  myFile = SD.open(archivo, FILE_WRITE);
   if (myFile) {
    Serial.println("dato guardado ");
    myFile.println("Temp:"+dhtTemp+";Hum:"+dhtHum+";Lum:"+luminosidad+";Lluv:"+lluvia+";"+dia+";"+fecha+";"+hora);
    myFile.close();
  } else {
      while (!SD.begin(cs)) {
        Serial.println("SD no se ha iniciado");   
        delay(1000);
    }
    Serial.println("SD inicializada.");
  }
}

//retorna valor luminosidad
String obtenerLuminosidad(){
  Serial.println("luminosidad registrada");
  return String(1023-analogRead(sensorLuminosidad));
}

String obtenerDhtTemp(){
  Serial.println("temperata registrada");
  return String(DHT.temperature);
}

String obtenerDhtHum(){
  Serial.println("humedad registrada");
  return String(DHT.humidity);
}

String obtenerLluvia(){
  Serial.println("lluvia registrada");
  return String (analogRead(sensorLluvia));
}



