#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

//Definição dos dispositivos.
#define SERVOMOTOR 22 //servomotor.
#define RGB_LED_PIN 12 //LED rgb.
#define PWM_FREQ 50 //Frequencia do PWM (50 Hz).
#define PERIODO 20000 //Período do PWM em microssegundos (20ms).

//Configuração do PWM.
void setup_pwm(uint gpio){
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    uint chan = pwm_gpio_to_channel(gpio);
    
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125.0f); // Define o divisor de clock
    pwm_config_set_wrap(&config, 20000); // Define o período em ciclos
        
    pwm_init(slice_num, &config, true);
    pwm_set_enabled(slice_num, true);
}

// Função para definir o ângulo do servo
void set_servo_angle(uint gpio, uint duty_us) {
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_gpio_level(gpio, duty_us);
}

int main()
{
    stdio_init_all();

    //Inicializando os dispositivos.
    setup_pwm(SERVOMOTOR);
    gpio_init(RGB_LED_PIN);
    gpio_set_dir(RGB_LED_PIN, GPIO_OUT);

    while (true) {
        // Define posição 180° (2400µs)
        set_servo_angle(SERVOMOTOR, 2400);
        gpio_put(RGB_LED_PIN, 1); // Acende LED
        sleep_ms(5000);
        
        // Define posição 90° (1470µs)
        set_servo_angle(SERVOMOTOR, 1470);
        gpio_put(RGB_LED_PIN, 0); // Apaga LED
        sleep_ms(5000);
        
        // Define posição 0° (500µs)
        set_servo_angle(SERVOMOTOR, 500);
        gpio_put(RGB_LED_PIN, 1); // Acende LED
        sleep_ms(5000);
        
        // Movimentação suave entre 0° e 180°
        for (uint duty = 500; duty <= 2400; duty += 5) {
            set_servo_angle(SERVOMOTOR, duty);
            sleep_ms(10);
        }
        for (uint duty = 2400; duty >= 500; duty -= 5) {
            set_servo_angle(SERVOMOTOR, duty);
            sleep_ms(10);
        }
        sleep_ms(1000);
    }
}
