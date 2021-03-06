
#define LEFT_EYE_PIN 6
#define RIGHT_EYE_PIN 9

#define BUTTON_PIN 2
#define POTI_PIN A1

const int poti_min = 39;
const int poti_max = 625;
const float poti_to_intensity = 255.0/(poti_max - poti_min);

const float iter_step_max = 10;
const float poti_to_iter_step = iter_step_max/(poti_max - poti_min);

uint8_t power;
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

  Serial.begin(9600);
}

void loop()
{
  poti = analogRead(POTI_PIN);

  oldButtonState = buttonState;
  buttonState = digitalRead(BUTTON_PIN);

  if(buttonState == HIGH && oldButtonState == LOW) // when the button is pressed
    mode = (Mode)((mode+1) % NUM_MODES);

  switch(mode)
  {
    case MODE_DIM_POWER:
      Serial.println("Mode DIM POWER");
      power = poti*poti_to_intensity; // poti range to 0-255
      dim_iter = (dim_iter+uint8_t(speed*poti_to_iter_step))% power;
      analogWrite(LEFT_EYE_PIN, dim_iter);
      analogWrite(RIGHT_EYE_PIN, dim_iter);
      break;
    case MODE_DIM_SPEED:
      Serial.println("Mode DIM SPEED");
      speed = poti;
      dim_iter = (dim_iter+uint8_t(speed*poti_to_iter_step))% power;
      analogWrite(LEFT_EYE_PIN, dim_iter);
      analogWrite(RIGHT_EYE_PIN, dim_iter);
      break;
    case MODE_ALTERNATE:
      Serial.println("Mode ALTERNATE");
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
      Serial.println("Mode FIXED");
      analogWrite(LEFT_EYE_PIN, power);
      analogWrite(RIGHT_EYE_PIN, power);
      power = poti*poti_to_intensity; // poti range to 0-255
      break;
    default:
      Serial.print("Mode unknown: ");
      Serial.println(mode);
  }

  Serial.print("Power: ");
  Serial.print(power);
  Serial.print(", speed: ");
  Serial.println(speed);
 
  delay(20);
}
