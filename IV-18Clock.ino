
#define dataPin 13
#define clockPin 12
#define loadPin 11
/*
 _
|_|
|_|

  7
2   6
  1 
3   5
  4
  
*/
const byte digits[] = {
    0b01111110, //0
    0b0110000,  //1
    0b01101101, //2
    0b01111001, //3
    0b00110011, //4
    0b01011011, //5
    0b01011111, //6
    0b01110000, //7
    0b01111111, //8
    0b01110011  //9
};
const int digitOffset = 4;

const int positionOffset = 12;

byte CurrentlyDisplayed[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
byte TargetDisplay[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

void setup()
{
  // put your setup code here, to run once:
  pinMode(9, INPUT_PULLUP);
  pinMode(loadPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  Serial.begin(9600);
  CurrentlyDisplayed[0] = digits[0];
  TargetDisplay[0] = digits[1];
}

void loop()
{
  static int counter = 1;
  static unsigned long lastExecuted = 0;

  if (millis() - lastExecuted >= 1000)
  {
    lastExecuted = millis();
    counter++;
    if (counter >= 10)
    {
      counter = 0;
    }
    for(int i = 0; i< sizeof(TargetDisplay); i++){
      TargetDisplay[i] = digits[random(0, 9)];
    }
    //TargetDisplay[0] = digits[counter];
  }
  digitalWrite(dataPin, HIGH);
  StepTransition();
  UpdateDisplay();
}

void StepTransition()
{
  static unsigned long lastExecuted = 0;

  if (millis() - lastExecuted < 100)
  {
    //lastExecuted = millis();
    return;
  }
  lastExecuted = millis();
  for (int i = 0; i < sizeof(CurrentlyDisplayed); i++)
  {
    if (CurrentlyDisplayed[i] != TargetDisplay[i])
    {
      for (int x = 0; x < 8; x++)
      {
        if (bitRead(CurrentlyDisplayed[i], x) != bitRead(TargetDisplay[i], x))
        {
          bitWrite(CurrentlyDisplayed[i], x, bitRead(TargetDisplay[i], x));
          break;
        }
      }
    }
  }
}

void UpdateDisplay()
{
  unsigned long data = 0;
  for (int i = 0; i < sizeof(CurrentlyDisplayed); i++)
  {
    bitWrite(data, i + positionOffset, 1);
    data |= (unsigned long)CurrentlyDisplayed[i] << digitOffset;
    WriteToThing(data);
    data = 0;
  }
}

void WriteToThing(unsigned long dat)
{
  digitalWrite(dataPin, LOW);
  digitalWrite(clockPin, LOW);

  digitalWrite(loadPin, LOW);
  for (byte i = 0; i < 20; i++)
  {
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, bitRead(dat, i));
    digitalWrite(clockPin, HIGH);
    digitalWrite(dataPin, LOW);
  }
  digitalWrite(loadPin, HIGH);
}
