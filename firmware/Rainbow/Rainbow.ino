
// ************************** НАСТРОЙКА ЛЕНТЫ ***********************

#define CURRENT_LIMIT 2000  // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит

#define NUM_LEDS 25         // количсетво светодиодов в одном отрезке ленты
#define NUM_STRIPS 1       // количество отрезков ленты (в параллели)
#define LED_PIN 6           // пин ленты
#define BTN_PIN 2           // пин кнопки/сенсора
#define MIN_BRIGHTNESS 5  // минимальная яркость при ручной настройке
#define BRIGHTNESS 255      // начальная яркость
#define MODES_AMOUNT 1      // Кол-во эффектов

// ************************** НАСТРОЙКА ДИОДОВ ***********************

int tempBrightness;     // Переменная для хранения временного значения яркости
byte bright = BRIGHTNESS;     // Яркость LED светодиодов
byte hue;     // Изменение оттенка LED
byte thisMode;     // Переменная для хранения текущего режима работы
boolean loadingFlag = true;     // Флаг, указывающий, находится ли система в процессе загрузки
boolean powerDirection = true;     // Флаг, указывающий направление управления питанием
boolean powerActive = false;     // Флаг, указывающий, активно ли питание
boolean powerState = true;     // Флаг, указывающий текущее состояние питания
boolean brightDirection = true;     // Флаг, указывающий направление изменения яркости

// ************************ ПОДКЛЮЧЕНИЕ БИБЛИОТЕК ***************************

#include "FastLED.h" // подключаем библиотеку фастлед
#include "GyverButton.h"
#include "GyverTimer.h"


// ************************** КОД ***********************

CRGB leds[NUM_LEDS];
GButton touch(BTN_PIN, LOW_PULL, NORM_OPEN);  
// Варианты инициализации:
// GButton btn;               // без привязки к пину (виртуальная кнопка) и без указания типа (по умолч. HIGH_PULL и NORM_OPEN)
// GButton btn(пин);          // с привязкой к пину и без указания типа (по умолч. HIGH_PULL и NORM_OPEN)
// GButton btn(пин, тип подключ.);    // с привязкой к пину и указанием типа подключения (HIGH_PULL / LOW_PULL) и без указания типа кнопки (по умолч. NORM_OPEN)
// GButton btn(пин, тип подключ., тип кнопки);      // с привязкой к пину и указанием типа подключения (HIGH_PULL / LOW_PULL) и типа кнопки (NORM_OPEN / NORM_CLOSE)
// GButton btn(BTN_NO_BTN_PIN, тип подключ., тип кнопки); // без привязки к пину и указанием типа подключения (HIGH_PULL / LOW_PULL) и типа кнопки (NORM_OPEN / NORM_CLOSE)
  // HIGH_PULL - кнопка подключена к GND, пин подтянут к VCC (BTN_PIN --- КНОПКА --- GND)
  // LOW_PULL  - кнопка подключена к VCC, пин подтянут к GND
  // по умолчанию стоит HIGH_PULL
  // NORM_OPEN - нормально-разомкнутая кнопка
  // NORM_CLOSE - нормально-замкнутая кнопка
  // по умолчанию стоит NORM_OPEN

GTimer_ms effectTimer(60);
GTimer_ms brightTimer(20);

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  if (CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT / NUM_STRIPS);
  FastLED.setBrightness(bright);
  FastLED.show();

  randomSeed(analogRead(0));
  touch.setTimeout(300);  // Настройка таймаута на удержание ( по умл. 500мс)
  touch.setStepTimeout(50);
  
}

void loop() {
  touch.tick();
  if (touch.hasClicks()) {
    byte clicks = touch.getClicks();
    switch (clicks) {
      case 1:
        powerDirection = !powerDirection;
        powerActive = true;
        tempBrightness = bright * !powerDirection;
        break;
      default:
        break;
    }
  }

  if (effectTimer.isReady() && powerState) {
    switch (thisMode) {
        case 0: rainbow();
        break;
    }
    FastLED.show();
  }

  brightnessTick();
}

void nextMode() {
  thisMode++;
  if (thisMode >= MODES_AMOUNT) thisMode = 0;
  loadingFlag = true;
  FastLED.clear();
}

void brightnessTick() {
  if (powerActive) {
    if (brightTimer.isReady()) {
      if (powerDirection) {
        powerState = true;
        tempBrightness += 10;
        if (tempBrightness > bright) {
          tempBrightness = bright;
          powerActive = false;
        }
        FastLED.setBrightness(tempBrightness);
        FastLED.show();
      } else {
        tempBrightness -= 10;
        if (tempBrightness < 0) {
          tempBrightness = 0;
          powerActive = false;
          powerState = false;
        }
        FastLED.setBrightness(tempBrightness);
        FastLED.show();
      }
    }
  }
}
