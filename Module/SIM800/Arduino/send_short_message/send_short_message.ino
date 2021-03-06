#include <M5Stack.h>
#define RX_PIN      16
#define TX_PIN      17
#define RESET_PIN   5   // pin to the reset pin Sim800L

String _buffer;

String _readSerial(uint32_t timeout)
{

    uint64_t timeOld = millis();

    while (!Serial2.available() && !(millis() > timeOld + timeout))
    {
        delay(13);
    }

    String str;

    while(Serial2.available())
    {
        if (Serial2.available()>0)
        {
            str += (char) Serial2.read();
        }
    }
    Serial.print(str);

    return str;

}

bool _sendSms()
{
    // Can take up to 60 seconds

    Serial2.print (F("AT+CMGF=1\r"));   //Text mode
    _buffer = _readSerial(5000);

    Serial2.print (F("AT+CSCA=\""));    // 信息中心号码，深圳联通
    Serial2.print ("+8618820991250");
    Serial2.print(F("\"\r"));
    _buffer = _readSerial(5000);

    Serial2.print (F("AT+CSCS=\"GSM\"\r"));
    _buffer = _readSerial(5000);

    Serial2.print (F("AT+CMGS=\""));
    Serial2.print ("+8613265764263");        // 对方手机号
    Serial2.print(F("\"\r"));
    _buffer = _readSerial(5000);

    Serial2.print ("Hello");
    Serial2.print ("\r");
    _buffer = _readSerial(5000);

    Serial2.print(0x1A,HEX);
    _buffer = _readSerial(60000);

    if ( (_buffer.indexOf("ER")) == -1)
    {
        return false;
    }
    else return true;
    // Error found, return 1
    // Error NOT found, return 0
}

void setup() {
  M5.begin();
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  pinMode(5, OUTPUT);

  _buffer.reserve(255); // Reserve memory to prevent intern fragmention

  delay(1000);// delay for SIM800L power on

  _sendSms();

}

void loop() {

}
