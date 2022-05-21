static unsigned long const PowersOf10[] = 
{1, 10, 100, 1000, 10000, 100000,
    1000000, 10000000, 100000000, 1000000000}; // etc

unsigned log10(unsigned long v) {
    auto log2 = (64 - __builtin_clzl(v)) + 1;
    auto toLog10 = log2 * 1233 >> 12;
    return toLog10 - (v < PowersOf10[toLog10]) + 1;
}
