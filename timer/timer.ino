/*
  LiquidCrystal Library - 
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 * Ultrasonic 
 * Trig to digital pin 6
 * Echo to dogotal pin 7
*/

#include <LiquidCrystal.h>
#include <Ultrasonic.h>

//variaveis globais
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Ultrasonic ultrasonic(6,7);
unsigned long tempo=0;
unsigned long timer=0;
unsigned long t1=0;
unsigned long t2=0;
unsigned long tempo2=0;
bool flip=false;
int btn=LOW;
int btn2=LOW;
int ultimo = LOW;
int ultimo2 = LOW;
int contador =0;
int contador2 = 0;
int modo = 0;
unsigned long tempo3=0;
unsigned long tempo4=0;
long maiorT1=0;
long menorT1=0;
long maiorT2=0;
long menorT2=0;
int mT1=0;
int MT1=0;
int mT2=0;
int MT2=0;
bool liberado = true;
unsigned long tLiberado =0;
int menu_3 = 0;
int p=0;
/*
 * setup 
 * configuracao do display lcd
 * configuracao do serial 9600
 * configuracao dos pinos dos botoes
 * inicializacao do meu no modo automatico
 */
void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(10,INPUT);
  pinMode(9,INPUT);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Modo Automatico");
  delay(5000);
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("T1"); 
  lcd.setCursor(3,0);
  lcd.print(getTempo(0));
  lcd.setCursor(0,1);
  lcd.print("T2");
  lcd.setCursor(3,1);
  lcd.print(getTempo(0));
}

/*
 * funcao para controlar a troca de menu utilizando o botao 1
 * para calibrar a leitura e minimizar a possibilidade de leituras falcas foi utilizado um debouce com tempo de 40ms
 */
void func1(){
  int pressionado2 = digitalRead(9);
  if(pressionado2 != btn2){
    tempo2 = millis();
  }
  if(millis()- tempo2 > 40 && liberado == true){
    if(pressionado2 == btn2){
      btn2 = pressionado2;
      if(btn2 != ultimo2){
        liberado = false;
        tLiberado = millis();
        modo ++;
        tempo3 = millis();
        Serial.print("menu trocado ");
        Serial.print(modo);
        Serial.println();
        ultimo = btn;
        return;
      }
    }
  }
  if(millis() - tLiberado>500){
    liberado = true;
    Serial.println("Liberado");
  }
  switch(modo){
    case 0:
      automatico();
    break;
    case 1:
      menu2();
    break;
    case 2:
      menu3();
    break;
    case 3:
     menu1();
     modo = 0;
     timer = millis();
    break;
  }
  btn2 = pressionado2;
}
/*
 * manu 1
 * template vazio do menu automatico
 */
void menu1(){
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("T1"); 
  lcd.setCursor(3,0);
  lcd.print(getTempo(0));
  lcd.setCursor(0,1);
  lcd.print("T2");
  lcd.setCursor(3,1);
  lcd.print(getTempo(0));
}
/*
 * menu 2
 * template e manu para mostrar quantas vezes um objeto entrou ou saiu da frente do sensor
 * objeto entrou soma 1
 * objeto saiu soma 1
 */
void menu2(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("    Ciclos    ");
  lcd.setCursor(5,1);
  lcd.print(contador2);
  delay(50);
}
/*
 * menu 3
 * menu para mostrar maior tempo e menor tempo em que um objeto estteve na frente do sensor
 * sub menu com botao de operacao 
 * botao 2 troca o sub menu
 */
void menu3(){
  if(p == 0 || p>300 || digitalRead(10)==HIGH){
    switch(menu_3){
      case 0:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Maior tempo T1 ");
        lcd.setCursor(0,1);
        lcd.print(getTempo(maiorT1));
        lcd.setCursor(11,1);
        lcd.print(MT1);    
      break;
      case 1:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Menor tempo T1 ");
        lcd.setCursor(0,1);
        lcd.print(getTempo(menorT1));
        lcd.setCursor(11,1);
        lcd.print(mT1);
      break;
      case 2:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Maior tempo T2 ");
        lcd.setCursor(0,1);
        lcd.print(getTempo(maiorT2));
        lcd.setCursor(11,1);
        lcd.print(MT2);
      break;
      case 3:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Menor tempo T2 ");
        lcd.setCursor(0,1);
        lcd.print(getTempo(menorT2));
        lcd.setCursor(11,1);
        lcd.print(mT2);
      break;
      case 4:
      menu_3 = -1;
      break;
    }
    delay(100);
    p=1;
    menu_3++;
  }
  delay(1);
  p++;
}
/*
 * funcao para ver atualizar automaticamente quando um objeto esta na frente do sensor e manu principal
 * para calibrar a leitura e minimizar a possibilidade de leituras falcas foi utilizado um debouce com tempo de 500ms
 * 
 */
void automatico(){
  int pressionado = VeObj();
  if(pressionado != btn){
    tempo = millis();
  }
  if(millis()- tempo > 500){
    if(pressionado == btn){
      btn = pressionado;
      btn = VeObj();
      if(btn != ultimo){
        contador2++;
        if(btn == HIGH){
          long t = millis();
          t2 = t-timer;
          //Serial.println(t);
          timer = t;
        }
        if(btn == LOW){
         long t = millis();
         t1 = t-timer;
         //Serial.println(t);
         timer = t;
        }
        if(t1>=maiorT1){
         maiorT1 = t1;
         MT1 = contador2;
        }
        if(t2>=maiorT2){
         maiorT2=t2;
         MT2 = contador2;
        }
        if(t1<menorT1 || menorT1 == 0){
         menorT1 = t1;
         mT1= contador2;
        }
        if(t2<menorT2 || menorT2 == 0){
         menorT2 = t2;
         mT2 = contador2;
        }
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("T1");
        lcd.setCursor(3,0);
        lcd.print(getTempo(t1));
        lcd.setCursor(0,1);
        lcd.print("T2");
        lcd.setCursor(3,1);
        lcd.print(getTempo(t2));
        ultimo = btn;
      }
    }
  }
  btn = pressionado;
}
/*
 *funcao para pegar se um objeto esta dentro de uma distancia 
 *comparando o valor lido com range
 *entrada void
 *saida int booleano 0 ou 1
 *
 */
int VeObj(){
  int range = 70;
  float distCM;
  long microsec = ultrasonic.timing();
  distCM=ultrasonic.convert(microsec, Ultrasonic::CM);
  if(distCM < range){
    return HIGH;
  }else{
    return LOW;
  }
}

/*
* funcao para converter o tempo em millisegundos para hora minuto e segundo
* seguindo a divisao
*   1Seg = 1000mS
*   1Min = 60Seg = 60000mS
*   1Hr = 60Min = 3600000ms
* entrada long
* saida String formatada  
*/

String getTempo(long t){
  long h ,m , s;
  unsigned long r; 
  h = (long) t/3600000;
  r = (t%3600000);
  m = (long) r/60000;
  r = (r%60000);
  s =(long) r/1000;
  char resp[12];
  String a = "";
  a+= h;
  a+= "h ";
  a+= m;
  a+= "m ";
  a+= s;
  a+= "s";
  return a;
}

/*
 * main loop
 */
void loop() {
  func1();
}

