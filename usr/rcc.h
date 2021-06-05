#include "stm32_base.h"

enum class Gport : uint32_t {
	A = RCC_APB2ENR_IOPAEN, 
	B = RCC_APB2ENR_IOPBEN,
	C = RCC_APB2ENR_IOPCEN,
	D = RCC_APB2ENR_IOPDEN,
};

class T_RCC : public :: RCC_TypeDef {
private:
	T_RCC() = delete;
	~T_RCC() = delete;

public:
	template<Gport... port>
	inline void PortOn(void) {
		APB2ENR |= SetBits<(uint32_t)port...>();
	}
	
private:
	template<uint8_t... byte>
	inline constexpr uint8_t SetBits(void) {
		return(byte | ...);
	}
};