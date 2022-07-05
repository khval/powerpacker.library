
size_t write_word(BPTR f, unsigned short v);
size_t write_dwords(BPTR f, unsigned int* buf, int count);
unsigned short read_word(BPTR f);
unsigned int read_dword(BPTR f);

