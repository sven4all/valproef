/*
Valproef - Meet de tijd dat het duurt om een metalen kogeltje een bepaalde afstand te laten vallen.
Versie 1.1
*/

// Pin 13 is verbonden met een LED, pin 4 met de schakelaar.
#define LED 13	//De LED die aangeeft of de opstelling klaar is voor een meting.
#define FET 5		//De FET. Door deze aan of uit te zetten kan de spoel wel of niet onder spanning worden gezet.
#define SWITCH 4	//De schakelaar waar het kogeltje op valt.
#define BUTTON 2	//De drukknop die gebruikt wordt voor het resetten en het starten van de meting.
#define ALERT 12	//Deze LED is het waarschuwingslampje dat aan gaat als de schakelaar open is.
#define TIMER 8	//De externe timer die gebruikt wordt om de valtijd op te meten.

unsigned long starttime; //De tijd in millisecondes op het moment dat de meting begint en het kogeltje wordt losgelaten.
unsigned long lastResetTime; //De tijd in millisecondes dat de opstelling voor het laatst is gereset.
boolean ballDropped = false; //De vlag die aangeeft of het kogeltje aan het vallen is.
boolean isSet = false; //De vlag die aangeeft of de opstelling klaar is voor een meting.
 
void setup()
{
  // Initialiseer de Serial met een snelheid van 9600 baud.
  Serial.begin(9600);
  Serial.flush();

  // Initialiseer de output en input pins.
  pinMode(LED, OUTPUT);
  pinMode(FET, OUTPUT);
  pinMode(TIMER, OUTPUT);
  pinMode(BUTTON, INPUT);
  pinMode(SWITCH, INPUT);
  pinMode(ALERT, OUTPUT);

  digitalWrite(FET, LOW); //Stuur een laag signaal naar de FET zodat de spoel niet aan staat.
}



void loop()
{
  if (millis() - lastResetTime > 10000)
  {
    unset();
  }
  // Als de bal al losgelaten is hoeft de knop niet meer gecheckt te worden, maar dan moet de schakelaar gecheckt worden.
  checkButton();
  checkSwitch();
}

void checkButton()
{
  /*
  Als de bal aan het vallen is wordt de functie vroegtijdig afgebroken.
  Als de knop niet is ingedrukt gebeurt er eveneens niets.
  */
  if (ballDropped)
  {
    return;
  }

  if (digitalRead(BUTTON) == HIGH)
  {
    if (isSet)
    {
      dropBall();
    } else
    {
      reset();
    }
  }
}

void dropBall()
{
  Serial.println("Meting gestart");
  ballDropped = true;
  starttime = millis(); //Start interne timer.
  digitalWrite(TIMER, HIGH); //Start externe timer.
  unset();
}



void checkSwitch()
{
  if (digitalRead(SWITCH) == LOW)
  {
    if (ballDropped)
    {
      digitalWrite(TIMER, LOW); // Stop de externe timer.
      unsigned int delta = millis() - starttime; // Meet de interne verlopen tijd.
      ballDropped = false;
      Serial.print("Millisecondes: "); // Zend de waarde van de interne timer naar de Serial.
      Serial.println(delta);
      float seconds = float(delta)/1000.0; //De millisecondes worden omgerekend naar hele secondes.
      Serial.print("Gravitatieversnelling: ");
      Serial.print(((2*0.335)/(seconds*seconds))); //Hier wordt de gravitatieversnelling uitgerekend met de formule. 0.335 is de afstand die het kogeltje heeft afgelegd.
      Serial.println(" m/s^2");
    }
    digitalWrite(ALERT, HIGH); //De rode LED wordt aangezet.
    unset();	//De opstelling wordt gereset.
  } else
  {
    digitalWrite(ALERT, LOW);
  }
}

void switchOpened()
{
  ballDropped = false; //Dit reset de opstelling, de knop wordt weer gecheckt en de schakelaar niet meer.
  //TODO Stop externe timer.
  unsigned int delta = millis() - starttime;
  Serial.print("Milliseconds passed: "); //Zend de waarde van de interne timer naar de Serial.
  Serial.println(delta);
}

void reset()
{
  if (digitalRead(SWITCH) == LOW)
  {
    Serial.println("Sluit de schakelaar");
  } else
  {
    Serial.println("Reset");
    digitalWrite(LED, HIGH);
    digitalWrite(FET, HIGH);
    lastResetTime = millis();
    isSet = true;
  }
  delay(1000);
}


void unset()
{
  digitalWrite(LED, LOW);
  digitalWrite(FET, LOW);
  isSet = false;
}
