#include <Servo.h> 
#include <Wii.h>
#include <usbhub.h>


// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif


#define PINCAMERAY 2 
#define PINCAMERAZ 3
#define PINSERVO 4
#define PINMOTOR 5
#define STARTPWM 116


/*
 * Wiimote code
 */
USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
WII Wii(&Btd, PAIR); // This will start an inquiry and then pair with your Wiimote - you only have to do this once
//WII Wii(&Btd); // After that you can simply create the instance like so and then press any button on the Wiimote

bool printAngle=false;

/*
 * Servo code
 * For servos items we have to use PIN: 2, 3, 4, 5, 6, 7, 8
 */

int pulsoMotor=103; //Initial valor to initialice the ESC, if you are using a different esc, this valor maybe change.
int pulsoDireccion=90; // Initial valor for the direction servo

int direccion=0;

Servo servoY,servoZ,servoD, motor;

byte recibiendoByte ;
boolean iniciado = false;

int x,y, mov, dire;


/*
 * 
 */

void setup() {
  Serial.begin(115200);
  /*
   * Setup servos
   */

  servoY.attach(PINCAMERAY);
  servoZ.attach(PINCAMERAZ);
  servoD.attach(PINSERVO);
  motor.attach(PINMOTOR);


  while ( iniciado==false ){
          motor.write(0);   // Armado
          servoD.write(pulsoDireccion); 
          
          iniciado=true;
            
  } 
  motor.write(pulsoMotor);

  /*
   * Setup Wii
   */
  //Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  
  Serial.print(F("\r\nWiimote Bluetooth Library Started"));

}
void loop() {
  Usb.Task();
  if (Wii.wiimoteConnected) {
    
      if(Wii.getButtonClick(A)){
        printAngle=!printAngle;
      }

      if(printAngle){
        Serial.println(Wii.getPitch());
        mov=Wii.getPitch();
        mov=map(Wii.getPitch(),0,360,0,180);
        
        if(mov > 117)
          motor.write(117);
        else
          if(mov <100)
            motor.write(100);
          else
            motor.write(mov);
        
        Serial.println(Wii.getRoll());
        dire=Wii.getRoll();
        //dire=map(Wii.getRoll(),0,360,0,180);
        if(dire <170)
          dire=0;
        else
          if(dire > 190)
            dire=180;
          else
            dire=90;
        
        servoD.write(dire);
      }

      if (Wii.nunchuckConnected) {
        if (Wii.getAnalogHat(HatX) > 137 ||  Wii.getAnalogHat(HatX) < 117 || Wii.getAnalogHat(HatY) > 137 || Wii.getAnalogHat(HatY) < 117) {
          
          x=map(Wii.getAnalogHat(HatX),34,239,0,180);
          y=map(Wii.getAnalogHat(HatY),30,244,0,180);
          
          servoY.write(x);
          servoZ.write(y+40);
        }
        
      }
      if(!printAngle){
        servoD.write(90);
        motor.write(102);
      }
    
 }
  
}
