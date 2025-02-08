
/*
   Управление кнопкой/сенсором
  - Удержание - яркость
  - 1х тап - переключение эффектов
  - 2х тап - переключение режима на цветные эффекты
*/

// ************************** НАСТРОЙКИ ЛЕНТЫ ***********************
#define CURRENT_LIMIT 3000  // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит
#define AUTOPLAY_TIME 99999999999999999999999999999    // время между сменой режимов в секундах
#define NUM_LEDS 25        // количсетво светодиодов в одном отрезке ленты
#define NUM_STRIPS 1       // количество отрезков ленты (в параллели)
#define LED_PIN 6           // пин ленты
#define BTN_PIN 2           // пин кнопки/сенсора
#define MIN_BRIGHTNESS 5  // минимальная яркость при ручной настройке
#define BRIGHTNESS 255      // начальная яркость
#define FIRE_PALETTE 0      // разные типы огня (0 - 3). Попробуй их все! =)

// ************************** НАСТРОЙКИ ДИОДОВ ***********************
byte bright = BRIGHTNESS; // яркость LED светодиодов
byte hue;     // изменение оттенка LED
byte saturation = 255; // Полная насыщенность
byte value = 255; // Полная яркость
byte thisMode;     // Переменная для хранения текущего режима работы
int tempBrightness;     // Переменная для хранения временного значения яркости

// ************************** НАСТРОЙКИ КНОПКИ ***********************
bool gReverseDirection = false;
boolean loadingFlag = true;
boolean autoplay = true;
boolean powerDirection = true;     // Флаг, указывающий направление управления питанием
boolean powerActive = false;     // Флаг, указывающий, активно ли питание
boolean powerState = true;     // Флаг, указывающий текущее состояние питания
boolean whiteMode = false;     // Отслеживание состояния режима
boolean brightDirection = true;     // Флаг, указывающий направление изменения яркости
boolean wasStep = false;     // Отслеживание состояния

//************************* НАСТРОЙКИ ЭФФЕКТОВ *****************************
bool filling = true; // Флаг для отслеживания состояния заполнения
bool useColor1 = true; // Флаг для определения, какой цвет использовать
bool isOn = false; // Состояние светодиода
bool isForward = true; // Направление эффекта: вперед или назад    
bool isLighting = true; // Флаг, указывающий, идет ли процесс освещения
bool isResetting = false; // Флаг для отслеживания сброса

// ************************ ПОДКЛЮЧЕНИЕ БИБЛИОТЕ К***************************
#include "FastLED.h" // подключаем библиотеку фастлед
#include "GyverButton.h"
#include "GyverTimer.h"

// ********************* НАСТРОЙКИ ЭФФЕКТОВ ************************************
int BOTTOM_INDEX = 0;        // светодиод начала отсчёта
int TOP_INDEX = int(NUM_LEDS / 2);
int EVENODD = NUM_LEDS % 2;
int ledsX[NUM_LEDS][3];     //-ARRAY FOR COPYING WHATS IN THE LED STRIP CURRENTLY (FOR CELL-AUTOMATA, MARCH, ETC)
int thisdelay = 20;          // Задержка между интерациями цикла
int thisstep = 10;           // Изменение параметра яркость/цвет
int thathue; // Цвет для второй половины
int thisindex = 0;
int thisColor[3] = {0, 0, 0}; // {RED, GRN, BLU}
int idex = 0;                //-LED INDEX (0 to LED_COUNT-1
int ibright = 0;             //-BRIGHTNESS (0-255)
int bouncedirection = 0;     // Перключатель направления цвета (0-1)
float tcount = 0.0;          // Отслеживание времени циклов
int lcount = 0;              // Счетчик интераций
int cycleCount = 0; // Счетчик циклов
int currentCycle = 0; // Текущий цикл стробирования
int strobeState = 0; // Текущее состояние стробирования
int explosionStage = 0; // Этап взрыва
int currentPixel = 0; // Текущий светодиод
int currentLED = 0; // Индекс текущего светодиода
int randomHue = 0; // Переменная для хранения случайного цвета
struct ColorParams {
    int hue;
    int saturation;
};
ColorParams primaryColor = {0, 255};
ColorParams secondaryColor;

unsigned long previousMillis = 0; // Хранит предыдущее время
unsigned long lastUpdate = 0; // Переменная для отслеживания времени
int timer = 0; // Таймер для управления обновлениями

// *************************** КОД ***********************************
CRGB leds[NUM_LEDS];
CRGBPalette16 gPal; // Палитра
CRGB color1; // Первый цвет
CRGB color2; // Второй цвет
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
GTimer_ms autoplayTimer((long)AUTOPLAY_TIME * 1000);
GTimer_ms brightTimer(20);

// залить все
void fillAll(CRGB newcolor) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = newcolor;
  }
}

// функция получения цвета пикселя по его номеру
uint32_t getPixColor(int thisPixel) {
  return (((uint32_t)leds[thisPixel].r << 16) | ((long)leds[thisPixel].g << 8 ) | (long)leds[thisPixel].b);
}

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  if (CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT / NUM_STRIPS);
  FastLED.setBrightness(bright);
  FastLED.show();

  randomSeed(analogRead(0)); // Инициализация генератора слчайных чисел
  touch.setTimeout(300);  // Настройка таймаута на удержание ( по умл. 500мс)
  touch.setStepTimeout(50);
  
  // Генерация двух случайных цветов
  color1 = CHSV(random(0, 256), 255, 255);
  color2 = CHSV(random(0, 256), 255, 255);

  if (FIRE_PALETTE == 0) gPal = HeatColors_p;
  else if (FIRE_PALETTE == 1) gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);
  else if (FIRE_PALETTE == 2) gPal = CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White);
  else if (FIRE_PALETTE == 3) gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::White);
  
}

// Функция для установки всех светодиодов в заданный цвет
void one_color_all(byte red, byte green, byte blue) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(red, green, blue);
  }
};

int favoriteEffects[] = {1, 7, 12, 21, 31}; // Индексы "любимых" эффектов
int currentEffectIndex = 0;


void loop() {
touch.tick();

if (touch.hasClicks()) {
    byte clicks = touch.getClicks();
    switch (clicks) {
        case 1:
            if (!whiteMode && !powerActive) {
                currentEffectIndex = (currentEffectIndex + 1) % (sizeof(favoriteEffects) / sizeof(favoriteEffects[0]));
                thisMode = favoriteEffects[currentEffectIndex];
                effectTimer.start(); // Запускаем таймер для эффектов
            } 
            break;
            
      case 2: if (!powerActive) {
          whiteMode = !whiteMode;
          if (whiteMode) {
            effectTimer.stop();
            CHSV randomColor = CHSV(random8(), 255, 255);
            fill_solid(leds, NUM_LEDS, randomColor);
            FastLED.show();
          }else {
            effectTimer.stop();
          }
        }
        break;
            
        default:
            break;
    }
}

if (touch.isRelease()) {
    if (wasStep) {
        wasStep = false;
        brightDirection = !brightDirection;
    }
}

    if (effectTimer.isReady() && powerState) {
        switch (thisMode) {
           case 1: rainbow(); break;  // Радуга
           case 2: disco(); break;  // Диско
           case 3: handleStrobe(); break;  // Стробоскоп (мигалки)
           case 4: colorWipeDiod(); break;  // Заполнение цветом Диода
           case 5: colorWipe(); break;  // Заполнение цветом
           case 6: color(); break;  // Цвет
           case 7: rainbowColor(); break;  // Цвет Полные
           case 8: runningWave(); break;  // Бегущая волна
           case 9: pulses(); break;  // Пульсация
           case 10: randomBlink(); break;  // Случайное мигание
           case 11: chase(); break;  // Светящийся круг
           case 12: rainbowCycle(); break;  // Радуга цикл
           case 13: fadeInOut(); break;  // Затухание
           case 14: starField(); break;  // Звезды
           case 15: raindrop(); break;  // Светящийся дождь
           case 16: colorFade(); break;  // Динамическое изменение цвета
           case 17: twinklingStar(); break;  // Мерцающая звезда
           case 18: trail(); break;  // Световой след
           case 19: funLights(); break;  // Забавные огни
           case 20: pulse_color_all(0); break;  // Пульсация цвета всех диодов
           case 21: rainbow_fade(); break;  // Радуга
           case 22: color_bounce(); break;  // Отскок
           case 23: random_burst(); break;  // Рандомный всплеск
           case 24: ems_lightsONE(); break;  // Аварийные огни 1 диод
           case 25: ems_lightsALL(); break;  // Аварийные огни
           case 26: flicker(); break;  // Мерцание
           case 27: sin_bright_wave(); break;  // Sin. Волна яркости
           case 28: flame(); break;  // Пламя
           case 29: lighter(); break;  // Огонек
           case 30: lightBugs(); break;  // Светлячки
           case 31: colors(); break;  // Цвета
           case 32: sparkles(); break;  // Конфетти
           case 33: fire(); break;  // Огонь
           case 34: discoEffect(); break;  // Диско эффект
           case 35: edgeGlow(); break;  // Светящиеся края
           case 36: goldEffect(); break;  // Золотая лента
           case 37: lightDartsEffect(); break;  // Дартс
           case 38: lightDartsEffectL(); break;  // Дартс с отскоком
           case 39: rocketEffect(); break;  // Ракета (не работает)
           case 40: lightExplosion(); break;  // Взрыв (не работает)
           case 41: lightFountain(); break;  // Фонтан (не работает)
           case 42: vinigret(); break;  // Винигрет (не работает)
           case 43: BREATHING_AURA_SYNC(); break;  // Сердцебиение 
           case 44: STROBING_AURA_SYNC(); break;  // Стробоскоп
           case 45: colorCycle_AURA_SYNC(); break;  // Один цвет
           case 46: RAINBOW_AURA_SYNC(); break;  // Радуга (без переливаний)
           case 47: COMET_AURA_SYNC(); break;  // Комета
           }
        FastLED.show();
    }
}
void changeColor() {
    CHSV randomColor = CHSV(random8(), 255, 255);
    fill_solid(leds, NUM_LEDS, randomColor);
}
