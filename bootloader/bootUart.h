void BLuart_init2();
void BLuart_send2(char c);
void BLuart_puts(char *s);
void BLuart_sendString(char* string,int size);
void BLkprint(char* str,unsigned int size);
void BLuart_print_hex(unsigned long value);
void BLuart_print_address(void* addr);