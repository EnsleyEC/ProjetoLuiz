/*  Código de teste para interface serial
 * 
 * Definições de mensagens:
 *  "Erro: "  serão mostradas em um JOptionPane 
 *  "Estado: " serão mostradas em um JLabel 
 *  "Delay: " será o tempo de espera pra reiniciar o sistema
 *  "Tempo: " contagem de tempo para reiniciar o sistema
 * 
 */

#include <Servo.h>
#include <LiquidCrystal.h>

#define SERVO 6
#define LED1 8
#define LED2 36
#define BUTTON 50
#define BUZZER 7


int contadorErro = 0;

String senha = "1234";
Servo s;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
  
void setup() { 
 pinMode(LED1, OUTPUT); 
 pinMode(LED2, OUTPUT);
 pinMode(BUTTON, INPUT);
 pinMode(BUZZER, OUTPUT);
 
 Serial.begin(9600);
 
 s.attach(SERVO);
 s.write(0);
 
 lcd.begin(16, 4);
}
  
/**
 * Função que lê uma string da Serial
 * e retorna-a
 */
String leStringSerial(){
  String conteudo = "";
  char caractere;
  
  // Enquanto receber algo pela serial
  while(Serial.available() > 0) {
    // Lê byte da serial
    caractere = Serial.read();
    // Ignora caractere de quebra de linha
    if (caractere != '\n'){
      // Concatena valores
      conteudo.concat(caractere);
    }
    // Aguarda buffer serial ler próximo caractere
    delay(10);
  }
  
  String myString = String(caractere);

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Digite a senha");

  if(conteudo.equals("Fechar")){
    Serial.println("Estado: Fechado");
    for(int pos = 90; pos >= 0; pos--){
      s.write(pos);
    }
  
  }else if(conteudo.equals(senha)){ //Senha correta
    for(int pos = 0; pos < 90; pos++){
      s.write(pos);
    }

    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Aberto!");
    
    Serial.println("Estado: Aberto");
    digitalWrite(LED1, HIGH);
    delay(2000);
    digitalWrite(LED1, LOW);
    
    contadorErro = 0;
  }else{
    Serial.println("Erro: Senha incorreta");
    erro();
  }
    
  return conteudo;
}


void erro(){

  contadorErro ++;  
  int delayErro = 200;

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Senha errada");

  digitalWrite(LED1, LOW);
  
  digitalWrite(LED2, HIGH);
  delay(delayErro);
  digitalWrite(LED2, LOW);
  delay(delayErro);
  digitalWrite(LED2, HIGH);
  delay(delayErro);
  digitalWrite(LED2, LOW);
  delay(delayErro);
  digitalWrite(LED2, HIGH);
  delay(delayErro);
  digitalWrite(LED2, LOW);

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Digite a senha");

  if(contadorErro == 3) bloquearUsuario();
  
}

void bloquearUsuario(){
  int delayBlock = 5;

  tone(BUZZER, 800, 1000); 
  delay(100);
  
  Serial.println("Delay: "+String(delayBlock));

  for(int i = 0; i < delayBlock; i++){
    Serial.println("Tempo: "+String(i+1));

    lcd.clear();
    lcd.print("Reiniciando...  ("+String(i+1)+")");
    
    delay(1000);
  }

  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  
  delay(2000);
  
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  Serial.println("Erro: Sistema Reiniciado");

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Digite a senha");
  
  contadorErro = 0;
}
  
void loop() {
  // Se receber algo pela serial

  if(digitalRead(BUTTON) == HIGH){
    delay(300);
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Fechado!"); 

    Serial.println("Estado: Fechado");

    for(int pos = 90; pos >= 0; pos--){
      s.write(pos);
    }
  }
  
  if (Serial.available() > 0){
    // Lê toda string recebida
    String recebido = leStringSerial();     
  }
}
