#include "SPI.h"
#include <EEPROM.h>
#include <Adafruit_GFX_AS.h>   // http://rcl-radio.ru/wp-content/uploads/2020/06/Adafruit_GFX.zip
#include <Adafruit_ILI9341_STM.h>
#include <STM32ADC.h>
#include <HardwareTimer.h> 
 STM32ADC myADC(ADC1);
 uint8 pin = PA0;
 volatile static bool dma1_ch1_Active;
 #define maxSamples 1000
 uint16_t buffer[maxSamples]; 
 uint16 dataPoints[maxSamples]; 
 
#define TFT_CS         PB1                 
#define TFT_DC         PB10            
#define TFT_RST        PB11
  Adafruit_ILI9341_STM tft = Adafruit_ILI9341_STM(TFT_CS, TFT_DC, TFT_RST); // Mosi - PA7, SCK - PA5
 
byte data[1000],data_old[1000];
int setting,hold_set,i,x,y,i2,u_max,u_min,minn,u_sinh,mn=2,raz,per,razv,sinhro;
int u1,u2,t1,t2,zap,ux=1,uxx=1,fun;
long h0,h1;
long times,times1,hhh,times2,times3,tim;
byte hold,www,w=1,w1=1,w2=1,w3=1,w4=1,link,w5=1,b1=1,b2=1;
String raz_x;
float k1,k2,del=1;
 
int data1[1000];
 
 
void setup() {
 Serial.begin(115200);
 EEPROM.init(0x801F000,0x801F800,0x400);// 1024 byte
 pinMode(PA0, INPUT_ANALOG);
 pinMode(PB12,INPUT_PULLUP);// HOLD
 pinMode(PB13,INPUT_PULLUP);//+
 pinMode(PB14,INPUT_PULLUP);//-
 pinMode(PB15,INPUT_PULLUP);// SET
 tft.begin();tft.setRotation(1);
 tft.fillScreen(ILI9341_BLACK); 
 razv=EEPROM.read(0);
 sinhro=EEPROM.read(1);
 
 
ADC1->regs->CR1 = 0; // Обнулить регистр управления
ADC1->regs->SQR1 = 0; // Обнулить регистр SQR1
ADC1->regs->CR2 |= ADC_CR2_CAL; // Пуск калибровки
while (!(ADC1->regs->CR2 & ADC_CR2_CAL)){}; 
 
}
 
void loop() {
 
if(digitalRead(PB15)==LOW&&hold==0){w=1;w1=1;b1=1;b2=0;setting++;if(setting>2){setting=0;}delay(300);}
if(digitalRead(PB15)==LOW&&hold==1){w=1;w2=1;w4=1;hold_set++;if(hold_set>4){hold_set=0;}delay(300); }
 
//////  TFT  //////////////////////////////////////////
  tft.setCursor(295, 0);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
  //////////////////// HOLD ON OFF ///////////////////////////////////////////
 if(digitalRead(PB12)==LOW&&hold==0){hold=1;w2=1;w4=1;link=1;setting=0;delay(300);}  
 if(digitalRead(PB12)==LOW&&hold==1){hold=0;www=1;b2=0;hold_set=0;delay(300);}
 
 if(link==1){link=0;tft.fillRect(170,0,100,30,ILI9341_BLACK);}
 if(hold==0){tft.setCursor(170, 0);tft.print("OSCILLOSCOPE");tft.setCursor(170, 10);tft.print("VERSION 0.1");tft.setCursor(170, 20);tft.print("RCL-RADIO.RU");}
 
razmer();
 if(hold==0){
 if(setting==0){
  if(digitalRead(PB14)==LOW&&hold==0){razv++;if(razv>12){razv=12;}EEPROM.update(0,razv);delay(300);b2=1;w3=1;w=1;w1=1;u1=0;u2=0;t1=0;t2=0;}
  if(digitalRead(PB13)==LOW&&hold==0){razv--;if(razv<0){razv=0;}EEPROM.update(0,razv);delay(300);b2=1;w3=1;w=1;w1=1;w5=1;}
  razmer();
  if(b2<5){b2++;tft.fillRect(80,0,65,8,ILI9341_RED);tft.setCursor(90, 0);tft.print((float)times3/10/mn,1);tft.print(" uS");}
  }
 
 if(setting==1){
  if(digitalRead(PB14)==LOW&&hold==0){uxx++;if(uxx>5){uxx=5;}del=1;ux=uxx;delay(300);w=1;w1=1;u1=0;u2=0;t1=0;t2=0;}
  if(digitalRead(PB13)==LOW&&hold==0){uxx--;ux=uxx;if(uxx<=0){del=2;uxx=0;ux=1;}delay(300);w=1;w1=1;}
  if(w1==1){w1=0; tft.fillRect(85,10,50,8,ILI9341_RED);tft.setCursor(90, 10);tft.print("U x ");if(uxx==0){tft.print(0.5,1);}else{tft.print(uxx);}}
  }
 
 if(setting==2){
  if(digitalRead(PB13)==LOW&&hold==0){sinhro++;if(sinhro>200){sinhro=200;}EEPROM.update(1,sinhro);delay(10);w=1;w1=1;}
  if(digitalRead(PB14)==LOW&&hold==0){sinhro--;if(sinhro<0){sinhro=0;}EEPROM.update(1,sinhro);delay(10);w=1;w1=1;}
    tft.fillRect(0, 230-sinhro+3, 3, 3, ILI9341_BLACK);tft.fillRect(0, 230-sinhro, 3, 3, 0xFFFFDD);tft.fillRect(0, 230-sinhro-3, 3, 3, ILI9341_BLACK);
 if(w1==1){w1=0;tft.fillRect(85,20,50,8,ILI9341_RED);tft.setCursor(90, 20);tft.print("SINH");}
 }
 }
 
  if(hold==0&&w==1){w=0;
  if(setting!=0){tft.fillRect(80,0,65,8,ILI9341_BLACK);tft.setCursor(90, 0);tft.print((float)times3/10/mn,1);tft.print(" uS");}
  if(setting!=1){tft.fillRect(70,10,65,8,ILI9341_BLACK);tft.setCursor(90, 10);tft.print("U x ");if(uxx==0){tft.print(0.5,1);}else{tft.print(uxx);}}
  if(setting!=2){tft.fillRect(70,20,65,8,ILI9341_BLACK);tft.setCursor(90, 20);tft.print("SINH");
  tft.fillRect(0, 230-sinhro+3, 3, 3, ILI9341_BLACK);tft.fillRect(0, 230-sinhro, 3, 3, 0x333333);tft.fillRect(0, 230-sinhro-3, 3, 3, ILI9341_BLACK);
  }}
 
 
if(uxx==0){pinMode(PA2,OUTPUT);digitalWrite(PA2,LOW);}
if(uxx>0){pinMode(PA2,INPUT);analogRead(PA2);}
 
 
//////////////// HOLD SET ////////////////////////////////////////////////////////////////////////////////
 if(hold==1){
  if(w4==1){w4=0;
  tft.fillRect(80,0,65,8,ILI9341_BLUE);tft.setCursor(90, 0);tft.print((float)times3/10/mn,1);;tft.print(" uS");tft.fillRect(70,10,65,8,ILI9341_BLACK);
  tft.fillRect(0, 230-sinhro, 3, 3, 0x000000);tft.setCursor(90, 10);tft.print("U x ");tft.fillRect(70,20,65,8,ILI9341_BLACK);if(uxx==0){tft.print(0.5,1);}else{tft.print(uxx);}}
 
  tft.setCursor(295, 0);
  if(digitalRead(PB14)==LOW&&hold_set==0){i2+=2;if(i2>290){i2=290;}delay(50);
  tft.drawLine(0, u1+30,320, u1+30,  0x000000);
  tft.drawLine(0, 230-u2,320, 230-u2,  0x000000);
  tft.drawLine(t1,   230, t1, 30,  0x000000);
  tft.drawLine(318-t2,   230, 318-t2, 30,  0x000000);
 
  u1=0;u2=0;t1=0;t2=0;}
  if(digitalRead(PB13)==LOW&&hold_set==0){i2-=2;if(i2<0){i2=0;}delay(50);
  tft.drawLine(0, u1+30,320, u1+30, 0x000000);
  tft.drawLine(0, 230-u2,320, 230-u2,  0x000000);
  tft.drawLine(t1,   230, t1, 30,  0x000000);
  tft.drawLine(318-t2,   230, 318-t2, 30,  0x000000);
 
  u1=0;u2=0;t1=0;t2=0;}
  if(hold==1){tft.fillRect(i2,235,4,3,ILI9341_BLACK);tft.fillRect(i2+2,235,30,3,ILI9341_GREEN);tft.fillRect(i2+32,235,4,3,ILI9341_BLACK);}
  if(hold==0){tft.fillRect(295,0,25,8,ILI9341_RED);tft.fillRect(0,235,240,3,ILI9341_BLACK);}else{tft.print("HOLD");}
 
  if(digitalRead(PB14)==LOW&&hold==1&&hold_set==1){if(u1<199-u2){u1++;}w2=1;if(u1>200){u1=200;}delay(30);w=1;w1=1;}
  if(digitalRead(PB13)==LOW&&hold==1&&hold_set==1){if(u1<201-u2){u1--;}w2=1;if(u1<0){u1=0;}delay(30);w=1;w1=1;}
  if(digitalRead(PB13)==LOW&&hold==1&&hold_set==2){if(u2<199-u1){u2++;}w2=1;if(u2>200){u2=200;}delay(30);w=1;w1=1;}
  if(digitalRead(PB14)==LOW&&hold==1&&hold_set==2){if(u2<201-u1){u2--;}w2=1;if(u2<0){u2=0;}delay(30);w=1;w1=1;}
 
  if(digitalRead(PB14)==LOW&&hold==1&&hold_set==3){if(t1<319-t2){t1++;};w2=1;if(t1>320){t1=320;}delay(30);w=1;w1=1;}
  if(digitalRead(PB13)==LOW&&hold==1&&hold_set==3){if(t1<321-t2){t1--;}w2=1;if(t1<0){t1=0;}delay(30);w=1;w1=1;}
  if(digitalRead(PB13)==LOW&&hold==1&&hold_set==4){if(t2<319-t1){t2++;}w2=1;if(t2>320){t2=320;}delay(30);w=1;w1=1;}
  if(digitalRead(PB14)==LOW&&hold==1&&hold_set==4){if(t2<321-t1){t2--;}w2=1;if(t2<0){t2=0;}delay(30);w=1;w1=1;}
 
 if(w2==1){w2=0;
   tft.drawLine(0, u1-1+30,320, u1-1+30,  ILI9341_BLACK);if(hold_set==1){tft.drawLine(0, u1+30,320, u1+30,  0xFFDAB9);}else{tft.drawLine(0, u1+30,320, u1+30,  0x222222);} tft.drawLine(0, u1+1+30,320, u1+1+30,  ILI9341_BLACK);
   tft.drawLine(0, 230-u2-1,320, 230-u2-1,  ILI9341_BLACK);if(hold_set==2){tft.drawLine(0, 230-u2,320, 230-u2,  0xFFDAB9);}else{tft.drawLine(0, 230-u2,320, 230-u2,  0x222222);} tft.drawLine(0, 230-u2+1,320, 230-u2+1,  ILI9341_BLACK);
   tft.drawLine(t1-1, 230, t1-1, 30,  ILI9341_BLACK);if(hold_set==3){tft.drawLine(t1,   230, t1, 30,  0xFFDAB9);}else{tft.drawLine(t1,   230, t1, 30,  0x222222);}tft.drawLine(t1+1, 230, t1+1, 30,  ILI9341_BLACK);
   tft.drawLine(318-t2-1, 230, 318-t2-1, 30,  ILI9341_BLACK);if(hold_set==4){tft.drawLine(318-t2,   230, 318-t2, 30,  0xFFDAB9);}else{{tft.drawLine(318-t2,   230, 318-t2, 30,  0x222222);}}tft.drawLine(318-t2+1, 230, 318-t2+1, 30,  ILI9341_BLACK);
 
   if(hold_set==1){tft.fillRect(150,0,55,8,ILI9341_RED);}else{tft.fillRect(150,0,55,8,ILI9341_BLACK);}
   if(hold_set==2){tft.fillRect(150,10,55,8,ILI9341_RED);}else{tft.fillRect(150,10,55,8,ILI9341_BLACK);}
   if(hold_set==3){tft.fillRect(220,0,60,8,ILI9341_RED);}else{tft.fillRect(220,0,60,8,ILI9341_BLACK);}
   if(hold_set==4){tft.fillRect(220,10,60,8,ILI9341_RED);}else{tft.fillRect(220,10,60,8,ILI9341_BLACK);}
 
 
    tft.fillRect(150,20,55,8,ILI9341_BLACK);
    tft.setCursor(150, 0);tft.print("U1 = ");tft.print((3.3-u1*0.0165)/ux*del,2);
    tft.setCursor(150, 10);tft.print("U2 = ");tft.print((abs(u2*0.0165))/ux*del,2);
    tft.setCursor(150, 20);tft.print("U  = ");tft.print(((3.3-u1*0.0165)-abs(u2*0.0165))/ux*del,2);
 
    tft.fillRect(220,20,65,8,ILI9341_BLACK);
    if(razv<6){zap=1;}else{zap=0;}
    tft.setCursor(220, 0);tft.print("T1 = ");tft.print(t1*(float)times3/1000/mn,zap);
    tft.setCursor(220, 10);tft.print("T2 = ");tft.print(((float)times3/3.125-t2*(float)times3/1000)/mn,zap);
    tft.setCursor(220, 20);tft.print("T  = ");tft.print(((((float)times3/3.125-t2*(float)times3/1000))/mn-((t1*(float)times3/1000))/mn),zap);
    tft.fillRect(280,20,30,8,ILI9341_BLACK);tft.print(" uS");
 }} 
////////// END HOLD SET ///////////////////////////////////////////////////////////////////////////////////
  setka();
 
  if(hold==0){DMA();if(dma1_ch1_Active == 0){for(int x=0; x<maxSamples; x++){data[x]=map(buffer[x],0,4095/ux,0,200);}}u1=0;u2=0;t1=0;t2=0;}
  arr();
 
 if(hold==0){i2=0;for(i=1;i<1000;i++){if(data[i+5]>sinhro&&data[i+3]<sinhro){fun=i;if(fun>680){fun=0;}i=2000;}}
  for(i=0;i<1000-fun;i++){data1[i]=data[fun+i];}i=0;}
 
 while(i<320){ 
 tft.drawLine(i*mn, 230-data_old[i],i*mn+mn-1, 230-data_old[i+1], ILI9341_BLACK);
 tft.drawLine(i*mn, 230-data1[i+i2],i*mn+mn-1, 230-data1[i+1+i2],  ILI9341_RED);i++;}i=0;
 while(i<639){data_old[i]=data1[i+i2];i++;}i=0; 
 if((mn>1&&w1==1&&hold==0&&setting==0)||www==1){www=0;w=1;w1=0;tft.fillScreen(ILI9341_BLACK);}
 if(razv==2&&w5==1){w5=0;w=1;w1=0;tft.fillScreen(ILI9341_BLACK);}
}
 
void DMA(){
 
     switch(per){
       case 0: rcc_set_prescaler(RCC_PRESCALER_ADC, RCC_ADCPRE_PCLK_DIV_2); break;
       case 1: rcc_set_prescaler(RCC_PRESCALER_ADC, RCC_ADCPRE_PCLK_DIV_4); break;
       case 2: rcc_set_prescaler(RCC_PRESCALER_ADC, RCC_ADCPRE_PCLK_DIV_6); break;
       case 3: rcc_set_prescaler(RCC_PRESCALER_ADC, RCC_ADCPRE_PCLK_DIV_8); break;
      }
     switch (raz) {
      case 0: adc_set_sample_rate(ADC1, ADC_SMPR_1_5); break;
      case 1: adc_set_sample_rate(ADC1, ADC_SMPR_7_5); break;
      case 2: adc_set_sample_rate(ADC1, ADC_SMPR_13_5); break;
      case 3: adc_set_sample_rate(ADC1, ADC_SMPR_28_5); break;
      case 4: adc_set_sample_rate(ADC1, ADC_SMPR_41_5); break;
      case 5: adc_set_sample_rate(ADC1, ADC_SMPR_55_5); break;
      case 6: adc_set_sample_rate(ADC1, ADC_SMPR_71_5); break;
      case 7: adc_set_sample_rate(ADC1, ADC_SMPR_239_5); break;
       } 
 
  adc_set_reg_seqlen(ADC1, 1);
  ADC1->regs->SQR3 = PIN_MAP[pin].adc_channel;      
  ADC1->regs->CR2 |= ADC_CR2_CONT;
  ADC1->regs->CR2 |= ADC_CR2_EXTSEL;   
  ADC1->regs->CR2 |= ADC_CR2_SWSTART; 
 
  dma_init(DMA1);  
  dma_attach_interrupt(DMA1, DMA_CH1, DMA1_CH1_Event);
  myADC.setDMA(buffer, maxSamples, (DMA_MINC_MODE | DMA_TRNS_CMPLT), DMA1_CH1_Event);
  dma1_ch1_Active = 1;
 
 
  times3 = micros();
  dma_enable(DMA1, DMA_CH1); 
  while (dma1_ch1_Active == 1);
  dma_disable(DMA1, DMA_CH1); 
  times3 = micros() - times3;
}
 
 
void setka(){ 
 for(y=30;y<240;y=y+50){for(x=10;x<320;x=x+5){tft.drawPixel(x, y, ILI9341_DARKGREY);}}  
 for(x=0;x<320;x=x+64){for(y=40;y<240;y=y+10){tft.drawPixel(x, y, ILI9341_DARKGREY);}}}
 
static void DMA1_CH1_Event(){dma1_ch1_Active = 0;}
 
void arr(){
   ///// U max max
 if(millis()-times>500){u_max=0;u_min=4100;w3=1;
 tft.fillRect(40,0,30,28,ILI9341_BLACK);
  for(int mmm=0;mmm<640;mmm++){u_min=min(u_min,buffer[mmm]);u_max=max(u_max,buffer[mmm]);}
 tft.setCursor(0, 0);tft.print("Vmax = ");tft.print(u_max*3.3/4095*del,2);
 tft.setCursor(0, 10);tft.print("Vmin = ");tft.print(u_min*3.3/4095*del,2);
 u_sinh = u_max-u_min;
 tft.setCursor(0, 20);tft.print("Vpp  = ");tft.print(u_sinh*3.3/4095*del,2);
 times=millis();
 if(u_max*3.3/4095>=3.3){uxx=0;ux=1;del=2;w3=1;tft.fillRect(70,10,65,8,ILI9341_BLACK);tft.setCursor(90, 10);tft.print("U x ");tft.print(0.5,1);}}
  }
 
void razmer(){
     switch(razv){
    case 0: mn=4;      per=0;raz=0;;break;  
    case 1: mn=2;      per=0;raz=0;;break; 
    case 2: mn=1;      per=0;raz=0;break; 
    case 3: mn=1;      per=0;raz=1;break;
    case 4: mn=1;      per=1;raz=1;break; 
    case 5: mn=1;      per=2;raz=1;break; 
    case 6: mn=1;      per=3;raz=1;break; 
    case 7: mn=1;      per=3;raz=2;break; 
    case 8: mn=1;      per=3;raz=3;break; 
    case 9: mn=1;      per=3;raz=4;break; 
    case 10: mn=1;     per=3;raz=5;break;
    case 11: mn=1;     per=3;raz=6;break;
    case 12: mn=1;     per=3;raz=7;break;
    }
  }
