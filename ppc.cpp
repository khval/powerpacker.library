size_t write_word(FILE* f, unsigned short v) {
    return fwrite(&v, 1, sizeof(v), f);
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
    unsigned short w;
    fread(&w, 1, sizeof(w), f);
    return w;
}

unsigned int read_dword(FILE* f) {
    unsigned int dw;
    fread(&dw, 1, sizeof(dw), f);
    return dw;
}
