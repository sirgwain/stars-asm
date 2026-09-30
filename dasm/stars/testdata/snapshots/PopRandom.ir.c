void PopRandom() {
L_14a2:
    cRandStack--;
    lRandSeed1 = rglRandStack[cRandStack][0];
    lRandSeed2 = rglRandStack[cRandStack][1];
    return;
}
