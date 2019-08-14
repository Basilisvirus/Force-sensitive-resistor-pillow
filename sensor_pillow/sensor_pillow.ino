// ensor in A0.
//Led 13 turns on when ur sitting too long.
// if you get up for within Rest_Time seconds its like you did noit get up at all.
//it warns you if you sit for final_R hours


int Set_Rest_Time=40; //How long will it take to take a rest after the long sitt.
int final_R= 1; //If you are sitting for more than final_R minutes, you will need to get up and rest

int Body_State=0; // 0=no_sit_yet, 1=sitting_first_time, 2=sitting, 3=not_Sitting 
int sec=0;
int minute=0;
int i=0;
int Min_Save=0;
int Sec_Save=0;
int Hour_Save=0;
int hour=0;
float x=0;
int Hour_Save_3=0;
int Min_Save_3=0;
int Sec_Save_3=0;
int y=0;
int Sec_Rest=0;

void Show_Time(){
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(sec);

}

void setup(){

  pinMode(13, OUTPUT);
cli();//stop interrupts

Serial.begin(9600);
//set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0. [Timer interrupt mode to Normal]
  TCCR1B = 0;// same for TCCR1B [Timer interrupt mode to Normal]
  TCNT1  = 0;//initialize counter value to 0. The counter value (TCNT0) increases until a compare match occurs between TCNT1 and OCR1A
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

sei();//allow interrupts

}//end setup



void loop(){
  
x= analogRead(A0);
//Serial.print("from sensor= ");
//Serial.println(x);
delay(300);

//No sit yet 0
if (Body_State==0 && x<800){
Serial.println("Please Sit");  
}

//If first time sit 1
if(x>=800 && Body_State==0){
  Body_State=1;
  hour=0;
  minute=0;
  sec=0;
  Serial.println("You sat first time");
}

//If sitting 2
if(Body_State==1 || x>800){
  Body_State=2;
  Serial.println("You are sitting");

  if(minute>=final_R){
    Serial.println("You are sitting for too long, take a rest");
    
    do{
      digitalWrite(13,1); //turn on the led that indicates that you are sitting too long
      y=1;
    }while(y=0);
  }
}



if((x<800) && (Body_State==2)){
  Body_State=3;
  Hour_Save_3=hour;
  Min_Save_3=minute;
  Sec_Save_3=sec;
    Serial.println("You are not sitting");
    Sec_Rest=0;
}

if (Body_State==3)
{
    
    if (Sec_Rest>= Set_Rest_Time)
    {
    
    Serial.println("You got rest");
    Body_State=0;
      Serial.println("You have left");
      if (y==0){
        digitalWrite(13,0); //turn off the led and let the user know that he/she has rest can sit again
      }
    }
  }
}



  ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
sec= sec +1;
Sec_Rest++;
Show_Time();
if (sec==60){
  sec=0;
  minute= minute +1;
Show_Time();
}
if(minute==60){
  minute=0;
  hour= hour+1;
Show_Time();
}
}
