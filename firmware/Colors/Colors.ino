

// *************************** НАСТРОЙКИ ЛЕНТЫ ***********************
#define CURRENT_LIMIT 3000  // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит
#define NUM_LEDS 25        // количсетво светодиодов в одном отрезке ленты
#define NUM_STRIPS 1       // количество отрезков ленты (в параллели)
#define LED_PIN 6           // пин ленты
#define BTN_PIN 2           // пин кнопки/сенсора
#define MIN_BRIGHTNESS 5  // минимальная яркость при ручной настройке
#define BRIGHTNESS 255      // начальная яркость

// *************************** НАСТРОЙКИ ДИОДОВ ***********************

byte bright = BRIGHTNESS; // яркость LED светодиодов
byte hue;     // изменение оттенка LED
byte thisMode;     // Переменная для хранения текущего режима работы
int tempBrightness;     // Переменная для хранения временного значения яркости

// *************************** НАСТРОЙКИ КНОПКИ ***********************

boolean powerDirection = true;     // Флаг, указывающий направление управления питанием
boolean powerActive = false;     // Флаг, указывающий, активно ли питание
boolean powerState = true;     // Флаг, указывающий текущее состояние питания
boolean whiteMode = false;     // Отслеживание состояния режима
boolean brightDirection = true;     // Флаг, указывающий направление изменения яркости
boolean wasStep = false;     // Отслеживание состояния

// *************************** ПОДКЛЮЧЕНИЕ БИБЛИОТЕК ***********************
#include "FastLED.h" // подключаем библиотеку фастлед
#include "GyverButton.h"
#include "GyverTimer.h"

// *************************** КОД ***********************
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

  randomSeed(analogRead(0)); // Инициализация генератора слчайных чисел
  touch.setTimeout(300);  // Настройка таймаута на удержание ( по умл. 500мс)
  touch.setStepTimeout(50);
  
  }

int favoriteEffects[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55}; // Индексы "любимых" эффектов
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
            case 1: LightRed(); break;   // Светло-красный
            case 2: Red(); break;        // Красный
            case 3: DarkRed(); break;    // Темно-красный        
            case 4: LightGreen(); break; // Светло-зеленый
            case 5: Green(); break;      // Зеленый
            case 6: DarkGreen(); break;  // Темно-зеленый        
            case 7: LightBlue(); break;  // Светло-синий
            case 8: Blue(); break;       // Синий
            case 9: DarkBlue(); break;   // Темно-синий        
            case 10: LightOrange(); break; // Светло-оранжевый
            case 11: Orange(); break;      // Оранжевый
        case 12: DarkOrange(); break;  // Темно-оранжевый       
        case 13: LightYellow(); break; // Светло-желтый
        case 14: Yellow(); break;      // Желтый
        case 15: DarkYellow(); break;  // Темно-желтый        
        case 16: LightPurple(); break; // Светло-фиолетовый
        case 17: Purple(); break;      // Фиолетовый
        case 18: DarkPurple(); break;  // Темно-фиолетовый      
        case 19: LightCyan(); break;   // Светло-циановый
        case 20: Cyan(); break;        // Циан
        case 21: DarkCyan(); break;    // Темно-циановый      
        case 22: LightPink(); break;   // Светло-розовый
        case 23: Pink(); break;        // Розовый
        case 24: DarkPink(); break;    // Темно-розовый      
        case 25: LightTurquoise(); break; // Светло-бирюзовый
        case 26: Turquoise(); break;      // Бирюзовый
        case 27: DarkTurquoise(); break;  // Темно-бирюзовый   
        case 28: LightGrey(); break;       // Светло-серый
        case 29: Grey(); break;            // Серый
        case 30: DarkGrey(); break;        // Темно-серый       
        case 31: LightBrown(); break;      // Светло-коричневый
        case 32: Brown(); break;           // Коричневый
        case 33: DarkBrown(); break;       // Темно-коричневый        
        case 34: Golden(); break;          // Золотой        
        case 35: LightLavender(); break;   // Светло-лавандовый
        case 36: Lavender(); break;        // Лавандовый
        case 37: DarkLavender(); break;    // Темно-лавандовый        
        case 38: LightPeach(); break;      // Светло-персиковый
        case 39: Peach(); break;           // Персиковый      
        case 40: LightEmerald(); break;    // Светло-изумрудный
        case 41: Emerald(); break;         // Изумрудный        
        case 42: LightSkyBlue(); break;    // Светло-небесно-голубой
        case 43: SkyBlue(); break;         // Небесно-голубой        
        case 44: Salatovy(); break;        // Салатовый
        case 45: Citric(); break;          // Лимонный
        case 46: Mint(); break;            // Мятный
        case 47: LightSilver(); break;     // Светло-серебристый
        case 48: Magenta(); break;         // Пурпурный
        case 49: LightBlueGreen(); break;  // Светло-синий-зеленый
        case 50: LightOlive(); break;      // Светло-оливковый
        case 51: SoftPink(); break;        // Нежно-розовый
        case 52: LightPurpleBlue(); break; // Светло-фиолетово-синий
        case 53: LightPinkPeach(); break;   // Светло-розовато-персиковый
        case 54: Maroon(); break;           // Темно-бордовый
        case 55: Shortbread(); break;       // Песочный
           }
        FastLED.show();
    }
}
