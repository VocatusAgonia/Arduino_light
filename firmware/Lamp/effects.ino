#define TRACK_STEP 50
//***************************РАДУГА***************
void rainbow() {
  hue += 2;
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CHSV((byte)(hue + i * float(255 / NUM_LEDS)), 255, 255);
};

//***************************ДИСКО***************
void disco() {
  // Генерируем случайные цвета для каждого светодиода
  for(int i = 0; i < NUM_LEDS; i++) {
    int redValue = random(0, 256);
    int greenValue = random(0, 256);
    int blueValue = random(0, 256);
    leds[i] = CRGB(redValue, greenValue, blueValue);
  }
};


//***************************СТРОБОСКОП***************
void handleStrobe() {
  
  int thathue = (hue + 160) % 255;
  unsigned long currentMillis = millis();

  // Если стробирование еще не началось, инициализируем его
  if (strobeState == 0) {
    thathue = (hue + 160) % 255; // Вычисляем второй цвет
    strobeState = 1; // Переходим к первому циклу
    previousMillis = currentMillis; // Сбрасываем таймер
  }

  // Обрабатываем стробирование
  if (strobeState == 1) {
    if (currentCycle < 5) {
      // Включаем светодиоды для первой половины
      for (int x = 0 ; x < 5; x++ ) {
      for (int i = 0; i < TOP_INDEX; i++) {
        leds[i] = CHSV(hue, saturation, 255); // Полная яркость
      }
      FastLED.show();

      // Проверяем, прошло ли время для выключения
      if (currentMillis - previousMillis >= thisdelay) {
        previousMillis = currentMillis; // Обновляем таймер
        one_color_all(0, 0, 0); // Выключаем светодиоды
        FastLED.show();
        currentCycle++; // Увеличиваем счетчик циклов
      }
      }
    } else {
      // Переходим ко второму циклу
      currentCycle = 0; // Сброс счетчика циклов
      strobeState = 2; // Переходим ко второму состоянию
      previousMillis = currentMillis; // Сбрасываем таймер
    }
  } else if (strobeState == 2) {
    if (currentCycle < 5) {
      for (int x = 0 ; x < 5; x++ ) {
      // Включаем светодиоды для второй половины с максимальной яркостью
      for (int i = TOP_INDEX; i < NUM_LEDS; i++) {
        leds[i] = CHSV(thathue, saturation, 255); // Максимальная яркость
      }
      FastLED.show();

      // Проверяем, прошло ли время для выключения
      if (currentMillis - previousMillis >= thisdelay) {
        previousMillis = currentMillis; // Обновляем таймер
        one_color_all(0, 0, 0); // Выключаем светодиоды
        FastLED.show();
        currentCycle++; // Увеличиваем счетчик циклов
      }
      }
    } else {
      // Завершаем стробирование
      strobeState = 0; // Сброс состояния
      currentCycle = 0; // Сброс счетчика циклов
    }
  }
};

//***************************ЗАПОЛНЕНИЕ ЦВЕТОМ ДИОДА***************
void colorWipeDiod() {

    const long interval = 50; // Интервал обновления (в миллисекундах)
    unsigned long currentMillis = millis(); // Получаем текущее время

    // Проверяем, прошло ли достаточно времени для обновления
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis; // Сохраняем текущее время

        // Генерация случайных значений для hue, saturation и value
        int hue = random(0, 256); // Значение от 0 до 255
        int saturation = random(0, 256); // Значение от 0 до 255
        int value = random(0, 256); // Значение от 0 до 255

        // Установка цвета через CHSV
        leds[currentLED] = CHSV(hue, saturation, value);
        FastLED.show(); // Обновление светодиодов

        // Переход к следующему светодиоду
        currentLED++;
        if (currentLED >= NUM_LEDS) {
            currentLED = 0; // Сброс индекса, если достигнут конец
        }
    }
};
//***************************ЗАПОЛНЕНИЕ ЦВЕТОМ***************
void colorWipe() {

    const long interval = 50; // Интервал обновления (в миллисекундах)
    unsigned long currentMillis = millis(); // Получаем текущее время

    // Проверяем, прошло ли достаточно времени для обновления
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis; // Сохраняем текущее время

        if (filling) {
            // Генерация случайных значений для hue, saturation и value
            int hue = random(0, 256); // Значение от 0 до 255
            int saturation = random(0, 256); // Значение от 0 до 255
            int value = random(0, 256); // Значение от 0 до 255

            // Установка цвета через CHSV
            leds[currentLED] = CHSV(hue, saturation, value);
            FastLED.show(); // Обновление светодиодов

            // Переход к следующему светодиоду
            currentLED++;
            if (currentLED >= NUM_LEDS) {
                currentLED = 0; // Сброс индекса, если достигнут конец
                filling = false; // Завершение заполнения
            }
        } else {
            // После заполнения меняем цвет всех светодиодов
            int newHue = random(0, 256);
            for (int i = 0; i < NUM_LEDS; i++) {
                leds[i] = CHSV(newHue, 255, 255); // Установка нового цвета
            }
            FastLED.show(); // Обновление светодиодов
            filling = true; // Сброс состояния заполнения
        }
    }
};
//***************************ЦВЕТА***************
void color() {

    const long interval = 50; // Интервал обновления (в миллисекундах)
    unsigned long currentMillis = millis(); // Получаем текущее время

    // Проверяем, прошло ли достаточно времени для обновления
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis; // Сохраняем текущее время

        if (filling) {
            // Установка цвета текущему светодиоду
            leds[currentLED] = useColor1 ? color1 : color2;
            FastLED.show(); // Обновление светодиодов

            // Переход к следующему светодиоду
            currentLED++;
            if (currentLED >= NUM_LEDS) {
                currentLED = 0; // Сброс индекса, если достигнут конец
                filling = false; // Завершение заполнения
                useColor1 = !useColor1; // Переключение цвета
            }
        } else {
            // Заполнение светодиодов новым цветом
            for (int i = 0; i < NUM_LEDS; i++) {
                leds[i] = useColor1 ? color1 : color2; // Установка нового цвета
            }
            FastLED.show(); // Обновление светодиодов
            filling = true; // Сброс состояния заполнения
            currentLED = 0; // Сброс индекса для следующего заполнения
            // Генерация новых случайных цветов
            color1 = CHSV(random(0, 256), 255, 255);
            color2 = CHSV(random(0, 256), 255, 255);
        }
    }
};
//***************************ЦВЕТА ПОЛНЫЕ***************
void rainbowColor() {

    const long interval = 50; // Интервал обновления (в миллисекундах)
    unsigned long currentMillis = millis(); // Получаем текущее время

    // Проверяем, прошло ли достаточно времени для обновления
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis; // Сохраняем текущее время

        if (filling) {
            // Заполнение светодиодов радугой
            leds[currentLED] = CHSV(hue, 255, 255);
            FastLED.show(); // Обновление светодиодов

            // Переход к следующему светодиоду
            currentLED++;
            if (currentLED >= NUM_LEDS) {
                currentLED = 0; // Сброс индекса, если достигнут конец
                filling = false; // Завершение заполнения
            }
        } else {
            // Плавное изменение цвета
            for (int i = 0; i < NUM_LEDS; i++) {
                leds[i] = CHSV(hue, 255, 255); // Установка нового цвета
            }
            FastLED.show(); // Обновление светодиодов

            // Изменение оттенка для следующего цикла
            hue += 1; // Увеличение оттенка
            if (hue >= 256) {
                hue = 0; // Сброс оттенка
            }
        }
    }
};
//***************************БЕГУЩАЯ ВОЛНА***************
void runningWave() {
    static uint8_t waveIndex = 0;
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV((i + waveIndex) % 256, 255, 255);
    }
    FastLED.show();
    waveIndex++;
};
//***************************ПУЛЬСАЦИЯ***************
void pulses() {
    static uint8_t brightness = 0;
    static int8_t fadeAmount = 5; // Изменение яркости
    static uint8_t randomHue = random(0, 256); // Генерация случайного цвета (Hue)

    // Устанавливаем цвет светодиодов с текущей яркостью
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(randomHue, 255, brightness); // Устанавливаем цвет с текущей яркостью
    }
    FastLED.show();

    brightness += fadeAmount;

    // Проверяем, достигли ли мы пределов яркости
    if (brightness <= 0) {
        fadeAmount = -fadeAmount; // Изменение направления
        // Генерируем новый случайный цвет (Hue) при затухании
        randomHue = random(0, 256);
    } else if (brightness >= 255) {
        fadeAmount = -fadeAmount; // Изменение направления
    }
}
//***************************СЛУЧАЙНОЕ МИГАНИЕ***************
void randomBlink() {
    for (int i = 0; i < NUM_LEDS; i++) {
        if (random(0, 10) > 5) {
            leds[i] = CRGB::White; // Включаем светодиод
        } else {
            leds[i] = CRGB::Black; // Выключаем светодиод
        }
    }
    FastLED.show();
};
//***************************СВЕТЯЩИЙСЯ КРУГ***************
void chase() {
    static int pos = 0;
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = (i == pos) ? CRGB::Red : CRGB::Black; // Цвет светящегося светодиода
    }
    FastLED.show();
    pos = (pos + 1) % NUM_LEDS; // Переход к следующему светодиоду
};
//***************************РАДУГА ЦИКЛ***************
void rainbowCycle() {
    static uint8_t hue = 0;
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV((i + hue) % 256, 255, 255);
    }
    FastLED.show();
    hue++;
};
//***************************ЗАТУХАНИЕ***************
void fadeInOut() {
    static uint8_t brightness = 0;
    static int8_t fadeAmount = 5; // Изменение яркости

    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(160, 255, brightness); // Цвет
    }
    FastLED.show();

    brightness += fadeAmount;
    if (brightness <= 0 || brightness >= 255) {
        fadeAmount = -fadeAmount; // Изменение направления
    }
};
//***************************ЗВЕЗДЫ***************
void starField() {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = (random(0, 10) > 8) ? CRGB::White : CRGB::Black; // Включаем случайные звезды
    }
    FastLED.show();
};
//***************************СЕТЯЩИЙСЯ ДОЖДЬ***************
void raindrop() {
    static uint8_t dropPosition = 0;

    for (int i = 0; i < NUM_LEDS; i++) {
        if (i == dropPosition) {
            leds[i] = CRGB::Blue; // Цвет капли
        } else {
            leds[i] = CRGB::Black; // Выключаем остальные
        }
    }
    FastLED.show();

    dropPosition = (dropPosition + 1) % NUM_LEDS; // Переход к следующей позиции
};
//***************************ДИНАМИЧЕСКОЕ ИЗМЕНЕНИЕ ЦВЕТА***************
void colorFade() {
    static uint8_t hue = 0;
    static int8_t fadeDirection = 1;

    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(hue, 255, 255);
    }
    FastLED.show();

    hue += fadeDirection;
    if (hue >= 255 || hue <= 0) {
        fadeDirection = -fadeDirection; // Изменяем направление
    }
};
//***************************МЕРЦАЮЩАЯ ЗВЕЗДА***************
void twinklingStar() {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = (random(0, 10) > 8) ? CRGB::White : CRGB::Black; // Случайное мерцание
    }
    FastLED.show();
};
//***************************СВЕТОВОЙ СЛЕД***************
void trail() {
    static uint8_t trailPosition = 0;
    static uint8_t fadeAmount = 5;

    for (int i = 0; i < NUM_LEDS; i++) {
        if (i < trailPosition) {
            leds[i] = CHSV(160, 255, 255); // Цвет следа
        } else {
            leds[i] = CRGB::Black; // Выключаем остальные
        }
    }
    FastLED.show();

    if (trailPosition < NUM_LEDS) {
        trailPosition++; // Увеличиваем позицию следа
    } else {
        trailPosition = 0; // Сбрасываем
    }
};
//***************************ЗАБАВНЫЕ ОГНИ***************
void funLights() {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(random(0, 256), 255, 255); // Случайный цвет
    }
    FastLED.show();
};
//***************************ПУЛЬСАЦИЯ ЦВЕТА ВСЕХ ДИОДОВ***************
void pulse_color_all(int type) {
  
    const long interval = 50; // Интервал обновления (в миллисекундах)
    unsigned long currentMillis = millis(); // Получаем текущее время
    
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        if (type == 0) { // Пульсация яркости
            ibright += (bouncedirection == 0) ? 1 : -1;
            if (ibright >= 255 || ibright <= 1) {
                bouncedirection = !bouncedirection;
            }
            for (int i = 0; i < NUM_LEDS; i++) {
                leds[i] = CHSV(hue, 255, ibright);
            }
        } else if (type == 1) { // Пульсация насыщенности
            saturation += (bouncedirection == 0) ? 1 : -1;
            if (saturation >= 255 || saturation <= 1) {
                bouncedirection = !bouncedirection;
            }
            for (int i = 0; i < NUM_LEDS; i++) {
                leds[i] = CHSV(hue, saturation, 255);
            }
        }
        FastLED.show();
    }
}

//***************************ПЛАВНЫЙ ПЕРЕХОД***************
void rainbow_fade() {

    const long interval = 50; // Интервал обновления (в миллисекундах)
    unsigned long currentMillis = millis(); // Получаем текущее время

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        hue++;
        if (hue > 255) hue = 0;

        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CHSV(hue + (i * 256 / NUM_LEDS), 255, 255);
        }
        FastLED.show();
    }
}

//***************************ОТСКОК***************
void color_bounce() {
    const long interval = 50; // Интервал обновления (в миллисекундах)
    unsigned long currentMillis = millis(); // Получаем текущее время

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        if (bouncedirection == 0) {
            idex++;
            if (idex >= NUM_LEDS) {
                bouncedirection = 1;
                idex = NUM_LEDS - 1;
                hue = random(0, 256); // Генерируем случайный цвет при отскоке
            }
        } else {
            idex--;
            if (idex < 0) {
                bouncedirection = 0;
                hue = random(0, 256); // Генерируем случайный цвет при отскоке
            }
        }
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = (i == idex) ? CHSV(hue, 255, 255) : CHSV(0, 0, 0); // Устанавливаем цвет для текущего светодиода
        }
        FastLED.show();
    }
}
//***************************РАНДОМНЫЙ ВСПЛЕСК***************
void random_burst() {
  
    const long interval = 50; // Интервал обновления (в миллисекундах)
    unsigned long currentMillis = millis(); // Получаем текущее время

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        int randomIndex = random(NUM_LEDS);
        leds[randomIndex] = CHSV(random(256), 255, 255);
        FastLED.show();
    }
}

//***************************АВАРИЙНЫЕ ОГНИ ОДИН ДИОД***************
void ems_lightsONE() {
  
    const long interval = 50; // Интервал обновления (в миллисекундах)
    unsigned long currentMillis = millis(); // Получаем текущее время

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CRGB::Black;
        }
        leds[random(NUM_LEDS)] = CHSV(0, 255, 255); // Красный
        FastLED.show();
    }
}

//***************************АВАРИЙНЫЕ ОГНИ***************
void ems_lightsALL() {
  
    const long interval = 50; // Интервал обновления (в миллисекундах)
    unsigned long currentMillis = millis(); // Получаем текущее время

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = (i % 2 == 0) ? CHSV(0, 255, 255) : CHSV(0, 0, 0); // Красный и черный
        }
        FastLED.show();
    }
}

//***************************МЕРЦАНИЕ***************
void flicker() {


    const long interval = 50; // Интервал обновления (в миллисекундах)
    unsigned long currentMillis = millis(); // Получаем текущее время

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        for (int i = 0; i < NUM_LEDS; i++) {
            // Используем только CHSV
            leds[i] = (random(10) < 3) ? CHSV(0, 255, 255) : CHSV(0, 0, 0); // Красный и черный
        }
        FastLED.show();
    }
}

//***************************СИНУСОИДАЛЬНАЯ ВОЛНА ЯРКОСТИ***************
void sin_bright_wave() {
  
    const long interval = 50; // Интервал обновления (в миллисекундах)
    unsigned long currentMillis = millis(); // Получаем текущее время

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CHSV(hue, 255, (sin(i + hue * 0.1) + 1) * 127); // Синусоидальная яркость
        }
        hue++;
        FastLED.show();
    }
}

//***************************ПЛАМЯ***************
void flame() {

    const long interval = 50; // Интервал обновления (в миллисекундах)
    unsigned long currentMillis = millis(); // Получаем текущее время

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        // Генерация "пламени"
        for (int i = 0; i < NUM_LEDS; i++) {
            // Случайный цвет в диапазоне от 0 до 15 (желтые и оранжевые оттенки)
            uint8_t hue = random(0, 15);
            // Случайная яркость в диапазоне от 100 до 255
            uint8_t brightness = random(100, 255);
            leds[i] = CHSV(hue, 255, brightness);
        }

        FastLED.show();
    }
}

// ****************************** ОГОНЁК ******************************
int16_t position;
boolean direction;

void lighter() {
  FastLED.clear();
  if (direction) {
    position++;
    if (position > NUM_LEDS - 2) {
      direction = false;
    }
  } else {
    position--;
    if (position < 1) {
      direction = true;
    }
  }
  leds[position] = CRGB::White;
}

// ****************************** СВЕТЛЯЧКИ ******************************
#define MAX_SPEED 30
#define BUGS_AMOUNT 3
int16_t speed[BUGS_AMOUNT];
int16_t pos[BUGS_AMOUNT];
CRGB bugColors[BUGS_AMOUNT];

void lightBugs() {
  if (loadingFlag) {
    loadingFlag = false;
    for (int i = 0; i < BUGS_AMOUNT; i++) {
      bugColors[i] = CHSV(random(0, 9) * 28, 255, 255);
      pos[i] = random(0, NUM_LEDS);
      speed[i] += random(-5, 6);
    }
  }
  FastLED.clear();
  for (int i = 0; i < BUGS_AMOUNT; i++) {
    speed[i] += random(-5, 6);
    if (speed[i] == 0) speed[i] += (-5, 6);

    if (abs(speed[i]) > MAX_SPEED) speed[i] = 0;
    pos[i] += speed[i] / 10;
    if (pos[i] < 0) {
      pos[i] = 0;
      speed[i] = -speed[i];
    }
    if (pos[i] > NUM_LEDS - 1) {
      pos[i] = NUM_LEDS - 1;
      speed[i] = -speed[i];
    }
    leds[pos[i]] = bugColors[i];
  }
}

// ****************************** ЦВЕТА ******************************
void colors() {
  hue += 2;
  CRGB thisColor = CHSV(hue, 255, 255);
  fillAll(CHSV(hue, 255, 255));
}

// ****************************** КОНФЕТТИ ******************************
void sparkles() {
  byte thisNum = random(0, NUM_LEDS);
  if (getPixColor(thisNum) == 0)
    leds[thisNum] = CHSV(random(0, 255), 255, 255);
  fade();
}

// ****************************** ОГОНЬ ******************************
#define COOLING  55
// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120

void fire() {
  random16_add_entropy( random());
  Fire2012WithPalette(); // run simulation frame, using palette colors
}

void Fire2012WithPalette()
{
  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  // Step 4.  Map from heat cells to LED colors
  for ( int j = 0; j < NUM_LEDS; j++) {
    // Scale the heat value from 0-255 down to 0-240
    // for best results with color palettes.
    byte colorindex = scale8( heat[j], 240);
    CRGB color = ColorFromPalette( gPal, colorindex);
    int pixelnumber;
    if ( gReverseDirection ) {
      pixelnumber = (NUM_LEDS - 1) - j;
    } else {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
  }
}
// *************** ДИСКО ЭФФЕКТ ***************
void discoEffect() {
    for (int i = 0; i < NUM_LEDS; i++) {
        // Генерируем случайный оттенок от 0 до 255
        uint8_t hue = random(0, 256);
        // Устанавливаем максимальную насыщенность и яркость
        leds[i] = CHSV(hue, 255, 255); // Случайный цвет
    }
}

// *************** СВЕТЯЩИЕСЯ КРАЯ ***************
void edgeGlow() {
    static uint8_t currentLED = 0; // Текущий пиксель
    static int8_t direction = 1; // Направление движения (1 - вперед, -1 - назад)
    static uint32_t previousMillis = 0; // Время последнего обновления
    const int interval = 50; // Интервал обновления (в миллисекундах)
    uint32_t currentMillis = millis();

    // Проверяем, прошло ли достаточно времени
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis; // Обновляем время последнего обновления

        // Очищаем светодиоды
        fill_solid(leds, NUM_LEDS, CRGB::Black);

        // Создаем эффект "edge glow"
        leds[currentLED] = CHSV((currentLED * 255 / NUM_LEDS), 255, 255); // Цвет в формате CHSV

        // Переходим к следующему пикселю
        currentLED += direction;

        // Проверяем, достигли ли мы конца или начала светодиодной ленты
        if (currentLED >= NUM_LEDS || currentLED < 0) {
            direction = -direction; // Изменяем направление
            currentLED += direction; // Корректируем текущий пиксель, чтобы не застрять
        }

        // Обновляем светодиоды
        FastLED.show();
    }
}
// *************** ЗОЛОТАЯ ЛЕНТА ***************
void goldEffect() {
  
    unsigned long currentMillis = millis(); // Получаем текущее время

    // Проверяем, прошло ли 100 мс
    if (currentMillis - lastUpdate >= 100) {
        lastUpdate = currentMillis; // Обновляем время

        if (cycleCount < 10) { // Проверяем, сколько циклов уже выполнено
            // Генерируем случайный цвет (Hue) при каждом включении
            randomHue = random(0, 256); // Генерируем случайный цвет (Hue)

            // Включаем светодиоды случайным цветом (CHSV)
            for (int i = 0; i < NUM_LEDS; i++) {
                leds[i] = CHSV(randomHue, 255, 255); // Устанавливаем случайный цвет
            }
        } else {
            // Выключаем светодиоды
            for (int i = 0; i < NUM_LEDS; i++) {
                leds[i] = CRGB::Black; // Выключаем
            }
        }

        FastLED.show(); // Обновляем светодиоды

        // Увеличиваем счетчик циклов
        cycleCount++;
        if (cycleCount >= 20) { // 10 включений и 10 выключений
            cycleCount = 0; // Сбрасываем счетчик
        }
    }
}
// *************** ДАРТС ***************
void lightDartsEffect() {
  
    unsigned long currentMillis = millis(); // Получаем текущее время

    // Проверяем, прошло ли 100 мс
    if (currentMillis - lastUpdate >= 100) {
        lastUpdate = currentMillis; // Обновляем время

        if (currentLED == 0 && !isOn) {
            // Генерируем новый случайный цвет при начале нового цикла
            randomHue = random(0, 256); // Генерируем случайный цвет (Hue)
        }

        if (currentLED < NUM_LEDS) {
            // Включаем текущий светодиод
            if (!isOn) {
                leds[currentLED] = CHSV(randomHue, 255, 255); // Устанавливаем случайный цвет
                FastLED.show();
                isOn = true; // Устанавливаем состояние включенным
            } else {
                // Выключаем текущий светодиод
                leds[currentLED] = CRGB::Black; // Выключаем
                FastLED.show();
                isOn = false; // Устанавливаем состояние выключенным
                currentLED++; // Переходим к следующему светодиоду
            }
        } else {
            // Сбрасываем состояние после завершения эффекта
            currentLED = 0; // Сбрасываем текущий светодиод
            isOn = false; // Сбрасываем состояние
        }
    }
}
// *************** ДАРТС С ОТСКОКОМ ***************
void lightDartsEffectL() {

  static uint8_t currentLED = 0; // Текущий светодиод
  static int8_t direction = 1; // Направление движения (1 - вперед, -1 - назад)
  static unsigned long lastUpdate = 0; // Время последнего обновления
  static uint8_t randomHue = 0; // Случайный цвет (Hue)
    unsigned long currentMillis = millis(); // Получаем текущее время

    // Проверяем, прошло ли 100 мс
    if (currentMillis - lastUpdate >= 100) {
        lastUpdate = currentMillis; // Обновляем время

        // Генерируем новый случайный цвет при начале нового цикла
        if (currentLED == 0 && !isOn) {
            randomHue = random(0, 256); // Генерируем случайный цвет (Hue)
        }

        // Включаем или выключаем текущий светодиод
        if (isOn) {
            leds[currentLED] = CRGB::Black; // Выключаем
            isOn = false; // Устанавливаем состояние выключенным
        } else {
            leds[currentLED] = CHSV(randomHue, 255, 255); // Устанавливаем случайный цвет
            isOn = true; // Устанавливаем состояние включенным
        }

        FastLED.show(); // Обновляем светодиоды

        // Переходим к следующему светодиоду
        currentLED += direction;

        // Проверяем, достигли ли мы конца или начала светодиодной ленты
        if (currentLED >= NUM_LEDS || currentLED < 0) {
            direction = -direction; // Изменяем направление
            currentLED += direction; // Корректируем текущий светодиод, чтобы не застрять
        }
    }
}

// *************** РАКЕТА ***************  НЕ РАБОТАЕТ
void rocketEffect() {
  
    // Обновляем эффекты в зависимости от таймера
    timer++;
    if (timer >= 10) {
        timer = 0; // Сбрасываем таймер

        if (isForward) {
            // Включаем текущий светодиод
            if (currentLED < NUM_LEDS) {
                leds[currentLED] = CHSV(0, 255, 0); // Устанавливаем цвет
                FastLED.show();
                currentLED++; // Переходим к следующему светодиоду
            } else {
                // Достигли конца, меняем направление на обратное
                isForward = false; 
                currentLED--; // Снижаем индекс, чтобы выключить последний светодиод
            }
        } else {
            // Выключаем текущий светодиод
            if (currentLED >= 0) {
                leds[currentLED] = CRGB::Black; // Выключаем текущий светодиод
                FastLED.show();
                currentLED--; // Переходим к предыдущему светодиоду
            } else {
                // Достигли начала, меняем направление на прямое
                isForward = true; 
                currentLED++; // Увеличиваем индекс, чтобы включить первый светодиод
            }
        }
    }
}
// *************** ВЗРЫВ ***************  НЕ РАБОТАЕТ
void lightExplosion() {

  const unsigned long updateDelay = 100; // Задержка для обновления
  unsigned long currentMillis = millis(); // Получаем текущее время

  // Проверяем, прошло ли время задержки
  if (currentMillis - lastUpdate >= updateDelay) {
    lastUpdate = currentMillis; // Обновляем время

    if (explosionStage == 0) {
      // Включаем все светодиоды
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(160, 255, 255); // Устанавливаем цвет
      }
      FastLED.show();
      explosionStage = 1; // Переходим к следующему этапу
    } else if (explosionStage == 1) {
      // Выключаем все светодиоды
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black; // Выключаем
      }
      FastLED.show();
      explosionStage = 0; // Сбрасываем этап взрыва
    }
  }
}
// *************** ФОНТАН ***************  НЕ РАБОТАЕТ
void lightFountain() {

  const unsigned long lightingDelay = 0; // Задержка для освещения
  const unsigned long resetDelay = 10; // Задержка для сброса
  unsigned long currentMillis = millis(); // Получаем текущее время

  // Если идет освещение
  if (isLighting) {
    // Проверяем, прошло ли время задержки
    if (currentMillis - lastUpdate >= lightingDelay) {
      lastUpdate = currentMillis; // Обновляем время

      // Включаем текущий светодиод
      if (currentLED < NUM_LEDS) {
        leds[currentLED] = CHSV(160, 255, 255); // Устанавливаем цвет
        FastLED.show();
        currentLED++; // Переходим к следующему светодиоду
      } else {
        // После того как все светодиоды включены, переходим к сбросу
        isLighting = false;
        currentLED = 0; // Сбрасываем текущий светодиод
      }
    }
  } else {
    // Проверяем, прошло ли время сброса
    if (currentMillis - lastUpdate >= resetDelay) {
      lastUpdate = currentMillis; // Обновляем время

      // Выключаем текущий светодиод
      if (currentLED < NUM_LEDS) {
        leds[currentLED] = CRGB::Black; // Выключаем
        FastLED.show();
        currentLED++; // Переходим к следующему светодиоду
      } else {
        // Завершили сброс
        isLighting = true; // Возвращаемся к освещению
      }
    }
  }
}
// *************** ВИНИГРЕТ ***************  НЕ РАБОТАЕТ
void vinigret() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if ((uint32_t)getPixColor(i) == 0) {
      
    }
  }
}

// ****************** BREATHING AURA SYNC *******************
void BREATHING_AURA_SYNC() {
  
    static uint8_t brightness = 0;
    static int8_t fadeAmount = 5;
    EVERY_N_MILLISECONDS(30) {
        brightness += fadeAmount;
        if (brightness == 0 || brightness == 255) {
            fadeAmount = -fadeAmount;
        }
        fill_solid(leds, NUM_LEDS, CHSV(0, 255, brightness));
        FastLED.show();
    }
}

// ****************** STROBING AURA SYNC *******************
void STROBING_AURA_SYNC() {
    EVERY_N_MILLISECONDS(100) {
        isOn = !isOn;
        fill_solid(leds, NUM_LEDS, isOn ? CRGB::Blue : CRGB::Black);
        FastLED.show();
    }
}

// ****************** COLOR CYCLE AURA SYNC *******************
void colorCycle_AURA_SYNC() {

  uint8_t hue = 0;
    EVERY_N_MILLISECONDS(10) {
        fill_solid(leds, NUM_LEDS, CHSV(hue++, 255, 255));
        FastLED.show();
    }
}

// ****************** RAINBOW AURA SYNC *******************
void RAINBOW_AURA_SYNC() {
  
  uint8_t hue = 0;
    EVERY_N_MILLISECONDS(20) {
        fill_rainbow(leds, NUM_LEDS, hue++, 7);
        FastLED.show();
    }
}
// ****************** COMET AURA SYNC *******************
void COMET_AURA_SYNC() {

 static uint8_t pos = 0;
    EVERY_N_MILLISECONDS(50) {
        leds[pos] = CRGB::White;
        FastLED.show();
        leds[pos] = CRGB::Black;
        pos = (pos + 1) % NUM_LEDS;
    }
}

// ****************** СЛУЖЕБНЫЕ ФУНКЦИИ *******************
void fade() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if ((uint32_t)getPixColor(i) == 0) continue;
    leds[i].fadeToBlackBy(TRACK_STEP);

    /*// измеряяем цвет текущего пикселя
      uint32_t thisColor = getPixColor(i);

      // если 0, то пропускаем действия и переходим к следующему
      if (thisColor == 0) continue;

      // разбиваем цвет на составляющие RGB
      byte rgb[3];
      rgb[0] = (thisColor >> 16) & 0xff;
      rgb[1] = (thisColor >> 8) & 0xff;
      rgb[2] = thisColor & 0xff;

      // ищем максимум
      byte maximum = max(max(rgb[0], rgb[1]), rgb[2]);
      float coef = 0;

      // если есть возможность уменьшить
      if (maximum >= TRACK_STEP)
      // уменьшаем и находим коэффициент уменьшения
      coef = (float)(maximum - TRACK_STEP) / maximum;

      // далее все цвета умножаем на этот коэффициент
      for (int i = 0; i < 3; i++) {
      if (rgb[i] > 0) rgb[i] = (float)rgb[i] * coef;
      else rgb[i] = 0;
      }
      leds[i] = CRGB(rgb[0], rgb[1], rgb[2]);*/
  }
}
