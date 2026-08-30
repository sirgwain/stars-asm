void PopRandom() {
L_14a2:
    cRandStack = (cRandStack - 0x1);
    lRandSeed1 = rglRandStack[cRandStack][0x0];
    lRandSeed2 = rglRandStack[cRandStack][0x1];
    return;
}
