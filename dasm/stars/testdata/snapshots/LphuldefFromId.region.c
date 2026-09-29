HULDEF *LphuldefFromId(int16_t id) {
    if (id < 32) {
        return &rghuldef[id];
    }
    return LphuldefSBFromId(id - 32);
}
