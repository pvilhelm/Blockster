/* This code is generated for a Blockster program */
#include <cstdint>
#include <iostream>
typedef struct constant_std_Os_constant_sources_core {
	volatile float value;		// Blockster: single
	float out_000;
} constant_std_Os_constant_sources_core_t;

typedef struct to_console_std_Is_to__console_sinks_core {
	float* const in_000;	// Constant pointer to outport
} to_console_std_Is_to__console_sinks_core_t;

constant_std_Os_constant_sources_core_t constant_0000000 =
{
	2.0f	// C++: volatile float value	Blockster: single 2
};
to_console_std_Is_to__console_sinks_core_t to_console_0000000 =
{
	&constant_0000000.out_000	// Constant pointer to outport
};
void task_0_update() {
	{
		constant_0000000.out_000 = constant_0000000.value;
	}
	{
		std::cout << *to_console_0000000.in_000 << std::endl;
	}
}
int main(int argc, char *argv[]) {
	for (;;) {
		task_0_update();
	}
	return 1;
}