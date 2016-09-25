int digitPins[] = {8, 3, 4, 7};
int segmentPins[] = {6, 2, 10, 12, 13, 5, 9};
int inputPins[] = {A0, 11};

int zeroDigitSegments[] = {0, 1, 2, 3, 4, 5};
int oneDigitSegments[] = {1, 2};
int twoDigitSegments[] = {0, 1, 3, 4, 6};
int threeDigitSegments[] = {0, 1, 2, 3, 6};
int fourDigitSegments[] = {1, 2, 5, 6};
int fiveDigitSegments[] = {0, 2, 3, 5, 6};
int sixDigitSegments[] = {0, 2, 3, 4, 5, 6};
int sevenDigitSegments[] = {0, 1, 2};
int eightDigitSegments[] = {0, 1, 2, 3, 4, 5, 6};
int nineDigitSegments[] = {0, 1, 2, 3, 5, 6};

String jsonLeft = "{";
String jsonRight = "}";

int temperatureSensorValue = 0;
int lastReadMs = 0;

void setup() {
  Serial.begin(9600);
  _setPinMode(digitPins, sizeof(digitPins) / sizeof(int), OUTPUT);
  _setPinMode(segmentPins, sizeof(segmentPins) / sizeof(int), OUTPUT);
  _setPinMode(inputPins, sizeof(inputPins) / sizeof(int), INPUT); 
}

void loop() {
  _doDelayed();
  _writeBigNumber(temperatureSensorValue);
}

void _doDelayed() {
  int nowMs = millis();
  boolean shouldRead = (nowMs - lastReadMs) > 350;
  if (shouldRead) {
    lastReadMs = nowMs;
    temperatureSensorValue = _getTemperatureSensorValue();
    _printTemperature(temperatureSensorValue);
    _checkButton();
  }
}

void _checkButton() {
  if (digitalRead(11) == HIGH) {
    _printButtonPressed();
  }
}

void _printButtonPressed() {
  String result = jsonLeft + "\"type\":\"button\", \"value\":\"pressed\"" + jsonRight;
  Serial.println(result);
}

void _printTemperature(int temperatureValue) {
  String result = jsonLeft + "\"type\":\"temperature\", \"value\":\"" + temperatureValue + "\"" + jsonRight;
  Serial.println(result);
}

int _getTemperatureSensorValue () {
  return analogRead(A0) / 0.666;
}

int* _getDigitSegments (int digit) {
  switch(digit) {
    case 0:
      return zeroDigitSegments;
    case 1:
      return oneDigitSegments;
    case 2:
      return twoDigitSegments;
    case 3:
      return threeDigitSegments;
    case 4:
      return fourDigitSegments;
    case 5:
      return fiveDigitSegments;
    case 6:
      return sixDigitSegments;
    case 7:
      return sevenDigitSegments;
    case 8:
      return eightDigitSegments;
    case 9:
      return nineDigitSegments;
  }
}

int _getDigitSegmentsLength (int digit) {
  switch(digit) {
    case 0:
      return sizeof(zeroDigitSegments) / sizeof(int);
    case 1:
      return sizeof(oneDigitSegments) / sizeof(int);
    case 2:
      return sizeof(twoDigitSegments) / sizeof(int);
    case 3:
      return sizeof(threeDigitSegments) / sizeof(int);
    case 4:
      return sizeof(fourDigitSegments) / sizeof(int);
    case 5:
      return sizeof(fiveDigitSegments) / sizeof(int);
    case 6:
      return sizeof(sixDigitSegments) / sizeof(int);
    case 7:
      return sizeof(sevenDigitSegments) / sizeof(int);
    case 8:
      return sizeof(eightDigitSegments) / sizeof(int);
    case 9:
      return sizeof(nineDigitSegments) / sizeof(int);
  }
}

void _write (int pins[], int pinsLength, int indexes[], int indexesLength, boolean setValue) {
  for (int i = 0; i < pinsLength; i++) {
    if (_isInArray(indexes, indexesLength, i)) {
      digitalWrite(pins[i], setValue);   
    }
  }
}

boolean _isInArray(int someArray[], int someArrayLength, int value) {
  boolean result = false;
  for (int i = 0; i < someArrayLength; i++) {
    if (someArray[i] == value) {
      result = true;
      break;
    }
  }
  return result;
}

void _writeValue (int digitIndex, int digitSegments[], int digitSegmentsLength) {
  int digitIndexes[] = {digitIndex};
  _write(digitPins, sizeof(digitPins) / sizeof(int), digitIndexes, sizeof(digitIndexes) / sizeof(int), LOW);
  _write(segmentPins, sizeof(segmentPins) / sizeof(int), digitSegments, digitSegmentsLength, HIGH);
}

void _writeDigit (int digitIndex, int digit) {
  _writeValue(digitIndex, _getDigitSegments(digit), _getDigitSegmentsLength(digit));
}

void _setPinMode (int pins[], int pinsLength, boolean pinModeValue) {
  for (int i = 0; i < pinsLength; i++) {
    pinMode(pins[i], pinModeValue);
  }
}

void _resetValues (int pins[], int pinsLength, boolean initValue) {
  for (int i = 0; i < pinsLength; i++) {
    digitalWrite(pins[i], initValue);
  }
}

void _resetAllValues () {
  _resetValues(digitPins, sizeof(digitPins) / sizeof(int), HIGH);
  _resetValues(segmentPins, sizeof(segmentPins) / sizeof(int), LOW);
}

void _writeNumber(int digitIndex, int value) {
  _resetAllValues();
  _writeDigit(digitIndex, value);
  delayMicroseconds(900);
}

void _writeNumbers(int one, int two, int three, int four) {
  _writeNumber(0, four);
  _writeNumber(1, three);
  _writeNumber(2, two);
  _writeNumber(3, one);
}

void _writeBigNumber(int bigNumber) {
  int thousands = bigNumber / 1000;
  int thousandsRemainder = bigNumber % 1000;
  int hundreds = thousandsRemainder / 100;
  int hundredsRemainder = thousandsRemainder % 100;
  int tenths = hundredsRemainder / 10;
  int tenthsRemainder = hundredsRemainder % 10;
  _writeNumbers(thousands, hundreds, tenths, tenthsRemainder);  
}

