#define SELECT_EL1_SP() __asm__ volatile (  \
    "mov x0, #0\n"                         \
    "msr spsel, x0\n"                      \
    ::: "x0"                               \
)                            
#define SELECT_EL0_SP() __asm__ volatile (  \
    "mov x0, #1\n"                         \
    "msr spsel, x0\n"                      \
    ::: "x0"                               \
)
