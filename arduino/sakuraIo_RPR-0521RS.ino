#include <Wire.h>
#include <RPR-0521RS.h>
#include <SakuraIO.h>

SakuraIO_I2C sakuraio;
RPR0521RS rpr0521rs;

uint32_t cnt = 0;

void setup() {
  byte rc;

  Serial.begin(9600);
  rc = rpr0521rs.init();
  Serial.print("Waiting to come online");
  for(;;){
    if( (sakuraio.getConnectionStatus() & 0x80) == 0x80 ) break;
    Serial.print(".");
    delay(1000);
  }
  
}

void loop() {
  cnt++;
  Serial.println(cnt);
  byte rc;
  unsigned short ps_val;
  float als_val;
  byte near_far;
  
  rc = rpr0521rs.get_psalsval(&ps_val, &als_val);
  if (rc == 0) {
    Serial.print(F("RPR-0521RS (Proximity)     = "));
    Serial.print(ps_val);
    Serial.print(F(" [count]"));
    near_far = rpr0521rs.check_near_far(ps_val);
    if (near_far == RPR0521RS_NEAR_VAL) {
      Serial.println(F(" Near"));
    } else {
      Serial.println(F(" Far"));
    }
    
    if (als_val != RPR0521RS_ERROR) {
      Serial.print(F("RPR-0521RS (Ambient Light) = "));
      Serial.print(als_val);
      Serial.println(F(" [lx]"));
      Serial.println();
    }
  }

 float ps_float;
 ps_float = (unsigned short)ps_val;

    if(sakuraio.enqueueTx(0,ps_float) != CMD_ERROR_NONE){
    Serial.println("[ERR] enqueue error");
  }

  if(sakuraio.enqueueTx(1,als_val) != CMD_ERROR_NONE){
    Serial.println("[ERR] enqueue error");
  }
    if(sakuraio.enqueueTx(2,cnt) != CMD_ERROR_NONE){
    Serial.println("[ERR] enqueue error");
  }
  
  sakuraio.send();
  delay(500);

}
