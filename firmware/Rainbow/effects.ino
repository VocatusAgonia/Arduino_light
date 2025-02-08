void rainbow() {
  hue += 2;
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CHSV((byte)(hue + i * float(255 / NUM_LEDS)), 255, 255);
};
/*
1. hue += 2;:

Переменная hue отвечает за цвет в цветовой модели HSV (Hue, Saturation, Value). Увеличивая hue на 2, мы изменяем цвет, создавая эффект движения радуги. Значение hue обычно варьируется от 0 до 255, и при превышении 255 оно может "обернуться" обратно к 0, что обеспечивает бесконечный цикл цветов.
2. Цикл for (int i = 0; i < NUM_LEDS; i++):

Этот цикл проходит по каждому светодиоду в массиве leds, который содержит значения цвета для каждого светодиода.
3. leds[i] = CHSV((byte)(hue + i * float(255 / NUM_LEDS)), 255, 255);:

Здесь для каждого светодиода устанавливается цвет, используя функцию CHSV(), которая принимает три параметра:
Hue: Цвет, который мы рассчитываем как hue + i * float(255 / NUM_LEDS). Это позволяет каждому светодиоду иметь немного другой цвет, создавая эффект градиента. Чем дальше светодиод от начала массива, тем больше значение hue, что приводит к различным цветам.
Saturation: Установлено на 255, что означает максимальную насыщенность цвета.
Value: Также установлено на 255, что означает максимальную яркость.
*/
