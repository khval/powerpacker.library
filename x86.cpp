
size_t write_word(FILE* f, unsigned short v) {
    unsigned char b1 = (v >> 8) & 0xFF;
    unsigned char b2 = (v >> 0) & 0xFF;
    size_t result = fwrite(&b1, 1, sizeof(b1), f);
    return result + fwrite(&b2, 1, sizeof(b2), f);
}

size_t write_dwords(FILE* f, unsigned int* buf, int count) {
    size_t result = 0;
    
    for (int i = 0; i < count / 4; ++i) {
        result += write_word(f, (buf[i] >> 16) & 0xFFFF);
        result += write_word(f, (buf[i] >> 0) & 0xFFFF);
    }

    return result;
}

unsigned short read_word(FILE* f) {
    unsigned char b1;
    unsigned char b2;
    fread(&b1, 1, sizeof(b1), f);
    fread(&b2, 1, sizeof(b2), f);

    return (b1 << 8) | b2;
}

unsigned int read_dword(FILE* f) {
    unsigned int result = read_word(f);
    return (result << 16) | read_word(f);
}
