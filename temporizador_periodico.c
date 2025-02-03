#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

// Definição dos pinos GPIO
#define LED_VERMELHO 11
#define LED_AMARELO 12
#define LED_VERDE 13

// Estados do semáforo
typedef enum {
    VERMELHO,
    AMARELO,
    VERDE
} estado_semaforo_t;

// Variável para controlar o estado atual do semáforo
estado_semaforo_t estado_atual = VERMELHO;

// Função para atualizar os LEDs de acordo com o estado do semáforo
void atualizar_leds() {
    gpio_put(LED_VERMELHO, estado_atual == VERMELHO);
    gpio_put(LED_AMARELO, estado_atual == AMARELO);
    gpio_put(LED_VERDE, estado_atual == VERDE);
}

// Função de call-back do temporizador
void repeating_timer_callback(struct repeating_timer *t) {
    switch (estado_atual) {
        case VERMELHO:
            estado_atual = AMARELO;
            break;
        case AMARELO:
            estado_atual = VERDE;
            break;
        case VERDE:
            estado_atual = VERMELHO;
            break;
    }

    atualizar_leds();
}

int main() {
    stdio_init_all();

    // Inicialização dos pinos GPIO
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_init(LED_AMARELO);
    gpio_set_dir(LED_AMARELO, GPIO_OUT);
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);

    // Inicializa os LEDs com o estado inicial (vermelho)
    atualizar_leds();

    // Criação do temporizador periódico
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

    // Loop principal
    while (true) {
        // Imprime informações a cada segundo
        sleep_ms(1000);
        printf("Informação: Tempo = %d segundos\n", time_us_64() / 1000000);
    }

    return 0;
}