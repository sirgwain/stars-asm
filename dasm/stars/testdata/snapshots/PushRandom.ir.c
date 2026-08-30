void PushRandom(int32_t lNew1, int32_t lNew2) {
L_1440:
    rglRandStack[cRandStack][0x0] = lRandSeed1;
    rglRandStack[cRandStack][0x1] = lRandSeed2;
    cRandStack = (cRandStack + 0x1);
    lRandSeed1 = lNew1;
    lRandSeed2 = lNew2;
    return;
}
