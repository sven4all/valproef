/*
Valproef - Meet de tijd dat het duurt om een metalen kogeltje een bepaalde afstand te laten vallen.
Versie 1.1
*/

// Pin 13 is verbonden met een LED, pin 4 met de schakelaar.
#define LED 13
#define SWITCH 4
#define BUTTON 2

unsigned long starttime;
boolean ballDropped = false;
 
void setup()
{
	//Initializeer de Serial.
	Serial.begin(9600);
	Serial.flush();

	// Initializeer de LED-pin als output en de schakelaar-pin en button-pin als input.
	pinMode(LED, OUTPUT);
        pinMode(BUTTON, INPUT);
	pinMode(SWITCH, INPUT);
}

void loop()
{
	//Als de bal al losgelaten is hoeft de knop niet meer gecheckt te worden, maar dan moet de schakelaar gecheckt worden.
	if (!ballDropped)
	{
		checkButton();
	} else
	{
		checkSwitch();
	}
}

void checkButton()
{
	/*
	Deze code bepaalt of de knop is ingedrukt. De knop zelf moet nog ingebouwd worden.
	
	Als de bal al gevallen is, sla deze stap over.
	Als er op de knop is gedrukt, start de timer. Dit is te bepalen door te kijken of de stroom van de externe voedingskast uit staat.
	Houdt ook een timer bij in het programma zelf.
	*/
	if (digitalRead(BUTTON) == HIGH) /*Button pressed*/
	{
                Serial.println("Button pressed");
		ballDropped = true;
		//TODO Start externe timer.
		starttime = millis(); //Start interne timer. millis() of micros()?
	}
}

void checkSwitch()
{
	if (digitalRead(SWITCH) == LOW)
	{
		digitalWrite(LED, LOW); //Lampjes om te testen.
		switchOpened();
	} else
	{
		digitalWrite(LED, HIGH);
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
