# radar_using_atmega16
this project consists to 2 parts :
first:(embedded system)
this project uses : UART / TIMER1(servo / ultrasonic)
HW: atmega16 , servo motor , ultrasonic , usb to ttl


second:(desktop appliction)
use C# , .net framework

the idea is the servo will rotate with angle(a) and when ultrasonic detect any object with distance (d)
will use UART to send the angle and distance to C# app 

C# app: will convert (a,d) to (x,y)point and draw it
