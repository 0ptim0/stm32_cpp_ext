#include "stm32_conf.h"
#include "blah.h"

const rom_class b;
int const rom_glob_var = 1;
int ram_glob_var = 5;

const rom_class *ptr1;
const int *ptr2;
int *ptr3;

int main(void) {
    int local1;
    int local2 = 5;
    static int local3;
    static int local4 = 1;
    static const int local5 = 1;
    int const *ptr[5] = {&local1, &local2, &local3, &local4, &local5};
    ptr1 = &b;
    ptr2 = &rom_glob_var;
    ptr3 = &ram_glob_var;

    T_RCC & RCC1 = *static_cast<T_RCC *>RCC;
    RCC1.PortOn<Gport::A, Gport::B, Gport::C>();
    b.foo();
    while(1){
    }
}
