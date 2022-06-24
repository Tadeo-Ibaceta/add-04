#include <stdio.h>
#include "pico/stdlib.h"

/* Elijo un pin de Trigger y Echo */
const uint TRIGGER = 27;
const uint ECHO = 26;

float ultrasonic_get_distance_cm(void){
  /* Escribo un 1 en el Trigger */
  gpio_put(TRIGGER, true);
  /* Espero 10 us con el pulso encendido */
  sleep_us(10);
  /* Escribo un 0 en el Trigger */
  gpio_put(TRIGGER, false);
  /* Espero a que el pulso llegue al Echo */
  while(!gpio_get(ECHO));
  /* Mido el tiempo de cuando llega el pulso */
  absolute_time_t from = get_absolute_time();
  /* Espero a que el pulso del Echo baje a cero */
  while(gpio_get(ECHO));
  /* Mido el tiempo cuando termina el pulso */
  absolute_time_t to = get_absolute_time();
  /* Saco la diferencia de tiempo (el ancho del pulso) en us */
  int64_t pulse_width = absolute_time_diff_us(from, to);
  /* Calculo la distancia */
  float distance = pulse_width / 59.0;
  return distance;
} 

int main() {
  void ultrasonic_init(uint8_t trigger, uint8_t echo){
    gpio_init(trigger);
    gpio_set_dir(27, true);

    gpio_init(echo);
    gpio_set_dir(26, true);
    }
  /* Habilito el USB */
  stdio_init_all();
  sleep_ms(1000);
  printf("Listo!\r\n");
  
  gpio_init(22);
  gpio_set_dir(22, true);

  gpio_init(21);  
  gpio_set_dir(21, true);
  
  gpio_init(20);  
  gpio_set_dir(20, true);
  
  gpio_init(TRIGGER);
  gpio_put(TRIGGER, false);
  /* Habilito el pin de Echo */
  gpio_init(ECHO);
  /* Configuro el Echo como entrada */
  gpio_set_dir(ECHO, false);

  while (true){

      float distance = ultrasonic_get_distance_cm();

      printf("Distance: %.2f\r\n", distance);

      if (distance < 10.0){
        gpio_put(22, true);
        gpio_put(21, false);
        gpio_put(20, false);
      }
      else if (distance > 10.0 && distance <50.0){
        gpio_put(21, true);
        gpio_put(22, false);
        gpio_put(20, false);
      }
      else if (distance > 50.0 && distance < 100.0){
        gpio_put(20, true);
        gpio_put(22, false);
        gpio_put(21, false);
      }
      sleep_ms(500);
  }
}