#include "stm32_conf.h"

int main(void) {
    T_RCC & RCC1 = *static_cast<T_RCC *>RCC;
    RCC1.PortOn<Gport::A, Gport::B, Gport::C>();
    while(1){  
    }
}
