#include <Servo.h>

// SERVOLARIN TƏYİNİ 
Servo qarajServosu;
Servo evServosu;
Servo paltarServosu;

// PİNLƏRİN TƏYİNİ 
// Qaraj Sistemi
const int qarajTrig = 9;
const int qarajEcho = 10;
const int qarajServoPin = 11;

// Ev Qapısı Sistemi
const int evTrig = 4;
const int evEcho = 3;
const int evServoPin = 7;

// Yağış və Paltar Sistemi
const int yagisPin = 6; // Yağış sensorunun DO (Digital Output) pini
const int paltarServoPin = 5;

void setup() {
  // Serial Monitoru başladırıq (Yoxlamaq üçün)
  Serial.begin(9600);

  // Servoları pinlərə bağlayırıq
  qarajServosu.attach(qarajServoPin);
  evServosu.attach(evServoPin);
  paltarServosu.attach(paltarServoPin);

  // Sensor pinlərinin giriş/çıxış nizamlamaları
  pinMode(qarajTrig, OUTPUT);
  pinMode(qarajEcho, INPUT);
  
  pinMode(evTrig, OUTPUT);
  pinMode(evEcho, INPUT);
  
  pinMode(yagisPin, INPUT); // Yağış sensoru girişdir

  // BAŞLANGIC VƏZİYYƏTİ (Proqram başlayanda hər şey bağlı/təhlükəsiz olsun)
  qarajServosu.write(0); // Qaraj qapısı bağlı
  evServosu.write(0); // Ev qapısı bağlı
  paltarServosu.write(0); // Paltarlar çöldə (yağış yoxdur)
}

void loop() {
  // 1. QARAJ QAPISI SİSTEMİ (Məsafə ölçümü və idarəetmə)
  long qarajMuddet, qarajMesafe;
  digitalWrite(qarajTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(qarajTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(qarajTrig, LOW);
  qarajMuddet = pulseIn(qarajEcho, HIGH);
  qarajMesafe = qarajMuddet * 0.034 / 2;

  if (qarajMesafe > 0 && qarajMesafe <= 25) { // Maşın 25 sm-dən yaxındırsa
    qarajServosu.write(90); // Qapını aç
    delay(3000);
  } else {
    qarajServosu.write(0); // Yoxdursa bağla
  }

  // 2. EV QAPISI SİSTEMİ (Məsafə ölçümü və idarəetmə)
  long evMuddet, evMesafe;
  digitalWrite(evTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(evTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(evTrig, LOW);
  evMuddet = pulseIn(evEcho, HIGH);
  evMesafe = evMuddet * 0.034 / 2;

  if (evMesafe > 0 && evMesafe <= 15) { // İnsan 15 sm-dən yaxındırsa
    evServosu.write(90); // Ev qapısını aç
    delay(3000);
  } else {
    evServosu.write(0); // Yoxdursa bağla
  }

  // 3. YAĞIŞ VƏ PALTARASQISI SİSTEMİ
  int yagisVeziyyeti = digitalRead(yagisPin);

  // Diqqət: Yağış sensorları adətən su dəyəndə LOW (0) siqnalı verir
  if (yagisVeziyyeti == LOW) { 
    paltarServosu.write(90); // Yağış yağırsa, asqını içəri çək (90 dərəcə)
    Serial.println("Yagis yagir! Paltarlar iceri cekildi.");
  } else {
    paltarServosu.write(0); // Yağış kəsilsə, asqını yenidən çölə çıxart (0 dərəcə)
  }

  // Sistemi yormamaq və stabil işləmək üçün kiçik fasilə
  delay(100);
}