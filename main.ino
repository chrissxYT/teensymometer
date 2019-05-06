#include <OneWire.h>

OneWire ds(10);
byte i;
byte present = 0;
byte data[12];
byte addr[8];
int led = LED_BUILTIN;
int bc = 0; //blink code
int bcc = 0; //bc counter
#define BC_NO_MORE_ADDR 1
#define BC_CRC_INVALID 2

// the setup routine runs once when you press reset:
void setup() {                
  pinMode(led, OUTPUT);
}

void blnk(byte b, int d)
{
  digitalWrite(led, b ? HIGH : LOW);
  delay(d);
}

// the loop routine runs over and over again forever:
void loop() {
  if(bc)
  {
    if(!bcc) {bc = 0; return;}
    if(bc == BC_NO_MORE_ADDR) digitalWrite(led, bcc % 2 ? HIGH : LOW);
    if(bc == BC_CRC_INVALID) digitalWrite(led, bcc % 5 ? HIGH : LOW);
    bcc--;
    delay(100);
    return;
  }
  if ( !ds.search(addr)) {
    bc = BC_NO_MORE_ADDR;
    ds.reset_search();
    bcc = 25;
    return;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      bc = BC_CRC_INVALID;
      bcc = 25;
      return;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);

  delay(1000);

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);

  for ( i = 0; i < 9; i++)
    data[i] = ds.read();

  for(int j = 0; j < 9; j++)
    for(i = 0; i < 8; i++)
      blnk(data[j] & (1 >> i), 100);
}
