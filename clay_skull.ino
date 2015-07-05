
#define LEFT_EYE_PIN 6
#define RIGHT_EYE_PIN 9

#define BUTTON_PIN 2
#define POTI_PIN A1

const int poti_min = 39;
const int poti_max = 625;

int power;
int speed;
int poti;

typedef enum mode_enum
{
  MODE_DIM_POWER = 0,
  MODE_DIM_SPEED = 1,
  MODE_ALTERNATE = 2,
  MODE_FIXED = 3
} Mode;
const int NUM_MODES = 4;

Mode mode;
bool left_eye_on;
uint8_t dim_iter;
int buttonState = LOW;
int oldButtonState = LOW;

void setup()
{
  pinMode(LEFT_EYE_PIN, OUTPUT);
  pinMode(RIGHT_EYE_PIN, OUTPUT);

  pinMode(BUTTON_PIN, INPUT);
  pinMode(POTI_PIN, INPUT);

  power = 255;
  speed = 1023;
  mode = MODE_DIM_POWER;
}

void loop()
{
  poti = analogRead(POTI_PIN);

  oldButtonState = buttonState;
  buttonState = digitalRead(BUTTON_PIN);

  if(buttonState == HIGH && oldButtonState == LOW) // when the button is pressed
    mode = (Mode)(mode+1 % NUM_MODES);

  switch(mode)
  {
    case MODE_DIM_POWER:
      power = poti/4; // 0-1023 to 0-255
      dim_iter = dim_iter+1 % power;
      analogWrite(LEFT_EYE_PIN, power);
      analogWrite(RIGHT_EYE_PIN, power);
      break;
    case MODE_DIM_SPEED:
      speed = poti;
      break;
    case MODE_ALTERNATE:
      if(left_eye_on)
      {
        analogWrite(LEFT_EYE_PIN, power);
        analogWrite(RIGHT_EYE_PIN, 0);
      }
      else
      {
        analogWrite(LEFT_EYE_PIN, 0);
        analogWrite(RIGHT_EYE_PIN, power);
      }
      left_eye_on = !left_eye_on;
      speed = poti;
      break;
    break;
    case MODE_FIXED:
      analogWrite(LEFT_EYE_PIN, power);
      analogWrite(RIGHT_EYE_PIN, power);
      power = poti/4; // 0-1023 to 0-255
      break;
  }

  delay(20);
}
