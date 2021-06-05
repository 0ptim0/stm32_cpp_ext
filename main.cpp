#include "stm32_conf.h"
#include "blah.h"

const ram_class b;
const ram_class *ptr;
int i; 

int main(void) {
    ptr = &b;
    T_RCC & RCC1 = *static_cast<T_RCC *>RCC;
    RCC1.PortOn<Gport::A, Gport::B, Gport::C>();
    b.foo();
    while(1){
    }
}
