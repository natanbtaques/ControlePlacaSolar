#include <Arduino.h>
// Código exemplo para teste de Micro Servo Motor SG90 
// Movimento do servo através de comandos especificados 
  
#include "Servo.h" // Inclui a Biblioteca Servo.h 
#include <Fuzzy.h>
#define pinLDR A0
Servo meuservo1; // Cria o objeto servo para programação
Servo meuservo2; 
int angulo=15; // Ajusta o ângulo inicial do Servo na parte superior 
int tetaeixo=60;//Ajusta o angulo inicial do servo na base 
float passo=3;

int distanceX = 0;
int distanceY = 0;

Fuzzy *fuzzy = new Fuzzy();

void setup() {
  meuservo1.attach(12); // Declara o pino do servo rotação horizontal
  meuservo2.attach(13); // Declara pino do servo rotação vertical
  Serial.begin(9600);  // declara o serial do LDR
  meuservo1.write(tetaeixo);
  meuservo2.write(angulo);

  // Instantiating a FuzzyInput object
  FuzzyInput *distance = new FuzzyInput(1);
  // Instantiating a FuzzySet object
  FuzzySet *small = new FuzzySet(20, 20, 100, 150);
  // Including the FuzzySet into FuzzyInput
  distance->addFuzzySet(small);
  // Instantiating a FuzzySet object
  FuzzySet *safe = new FuzzySet(100, 150, 150, 220);
  // Including the FuzzySet into FuzzyInput
  distance->addFuzzySet(safe);
  // Instantiating a FuzzySet object
  FuzzySet *big = new FuzzySet(150, 220, 500, 500);
  // Including the FuzzySet into FuzzyInput
  distance->addFuzzySet(big);
  // Including the FuzzyInput into Fuzzy
  fuzzy->addFuzzyInput(distance);

  // Instantiating a FuzzyOutput objects
  FuzzyOutput *speed = new FuzzyOutput(1);
  // Instantiating a FuzzySet object
  FuzzySet *slow = new FuzzySet(1, 1, 3, 5);
  // Including the FuzzySet into FuzzyOutput
  speed->addFuzzySet(slow);
  // Instantiating a FuzzySet object
  FuzzySet *average = new FuzzySet(3, 5, 5, 8);
  // Including the FuzzySet into FuzzyOutput
  speed->addFuzzySet(average);
  // Instantiating a FuzzySet object
  FuzzySet *fast = new FuzzySet(5, 8, 8, 10);
  // Including the FuzzySet into FuzzyOutput
  speed->addFuzzySet(fast);
  // Including the FuzzyOutput into Fuzzy
  fuzzy->addFuzzyOutput(speed);

  // Building FuzzyRule "IF distance = small THEN speed = slow"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *ifDistanceSmall = new FuzzyRuleAntecedent();
  // Creating a FuzzyRuleAntecedent with just a single FuzzySet
  ifDistanceSmall->joinSingle(small);
  // Instantiating a FuzzyRuleConsequent objects
  FuzzyRuleConsequent *thenSpeedSlow = new FuzzyRuleConsequent();
  // Including a FuzzySet to this FuzzyRuleConsequent
  thenSpeedSlow->addOutput(slow);
  // Instantiating a FuzzyRule objects
  FuzzyRule *fuzzyRule01 = new FuzzyRule(1, ifDistanceSmall, thenSpeedSlow);
  // Including the FuzzyRule into Fuzzy
  fuzzy->addFuzzyRule(fuzzyRule01);

  // Building FuzzyRule "IF distance = safe THEN speed = average"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *ifDistanceSafe = new FuzzyRuleAntecedent();
  // Creating a FuzzyRuleAntecedent with just a single FuzzySet
  ifDistanceSafe->joinSingle(safe);
  // Instantiating a FuzzyRuleConsequent objects
  FuzzyRuleConsequent *thenSpeedAverage = new FuzzyRuleConsequent();
  // Including a FuzzySet to this FuzzyRuleConsequent
  thenSpeedAverage->addOutput(average);
  // Instantiating a FuzzyRule objects
  FuzzyRule *fuzzyRule02 = new FuzzyRule(2, ifDistanceSafe, thenSpeedAverage);
  // Including the FuzzyRule into Fuzzy
  fuzzy->addFuzzyRule(fuzzyRule02);

  // Building FuzzyRule "IF distance = big THEN speed = high"
  // Instantiating a FuzzyRuleAntecedent objects
  FuzzyRuleAntecedent *ifDistanceBig = new FuzzyRuleAntecedent();
  // Creating a FuzzyRuleAntecedent with just a single FuzzySet
  ifDistanceBig->joinSingle(big);
  // Instantiating a FuzzyRuleConsequent objects
  FuzzyRuleConsequent *thenSpeedFast = new FuzzyRuleConsequent();
  // Including a FuzzySet to this FuzzyRuleConsequent
  thenSpeedFast->addOutput(fast);
  // Instantiating a FuzzyRule objects
  FuzzyRule *fuzzyRule03 = new FuzzyRule(3, ifDistanceBig, thenSpeedFast);
  // Including the FuzzyRule into Fuzzy
  fuzzy->addFuzzyRule(fuzzyRule03);
}

void loop() { 
  int LEFT = analogRead(A0);
  int UP = analogRead(A1);
  int DOWN = analogRead(A2);
  int RIGHT = analogRead(A3);
  
  ////////////////EIXO INFERIRO DO PROJETO 

  if (abs(RIGHT-LEFT)<40){
    Serial.println("Eixo X OK");
    // cont1 = false;
    // delay(200);
  }
  else if((RIGHT>LEFT) && (tetaeixo>30)){
    // Serial.print("Diferença: ");
    // Serial.println(RIGHT - LEFT);
    distanceX = RIGHT - LEFT;
    fuzzy->setInput(1, RIGHT - LEFT);
    fuzzy->fuzzify();
    passo = fuzzy->defuzzify(1);
    tetaeixo -= passo;
    // Serial.print("DIREITA: ");
    // Serial.println(passo);
    meuservo1.write(tetaeixo);
    // delay(25);
  }
  else if((LEFT>RIGHT)  && (tetaeixo<100)){
    // Serial.print("Diferença: ");
    // Serial.println(LEFT - RIGHT);
    distanceX = RIGHT - LEFT;
    fuzzy->setInput(1, LEFT - RIGHT);
    fuzzy->fuzzify();
    passo = fuzzy->defuzzify(1);
    tetaeixo += passo;
    // Serial.print("ESQUERDA: ");
    // Serial.println(passo);
    meuservo1.write(tetaeixo);
    // delay(25);
  }

  /////////////EIXO SUPERIOR RESPONSAVEL PELO ANGULO

  if (abs(UP-DOWN)<20){
    // cont2 = false;
    // delay(200);
    Serial.println("Eixo Y OK");
  }
  else if((UP>DOWN) && (angulo<55)){
    // Serial.print("Diferença: ");
    // Serial.println(UP - DOWN);
    distanceY = UP - DOWN;
    fuzzy->setInput(1, UP - DOWN);
    fuzzy->fuzzify();
    passo = fuzzy->defuzzify(1);
    angulo += passo;
    // Serial.print("UP: ");
    // Serial.println(passo);
    meuservo2.write(angulo);
    // delay(25);
  }
  else if((DOWN>UP) && (angulo>2)){
    // Serial.print("Diferença: ");
    // Serial.println(DOWN - UP);
    distanceY = UP - DOWN;
    fuzzy->setInput(1, DOWN - UP);
    fuzzy->fuzzify();
    passo = fuzzy->defuzzify(1);
    angulo -= passo;
    // Serial.print("DOWN: ");
    // Serial.println(passo);
    meuservo2.write(angulo);
    // delay(25);
  }

  Serial.print(distanceX);
  Serial.print(", ");
  Serial.println(distanceY);
  delay(50);
}  