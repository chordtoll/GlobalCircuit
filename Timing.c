
unsigned long long get_timer() {
    unsigned long long acc1;
    unsigned int tim;
    unsigned long long acc2;
    acc1=timer_accum;
    tim=ReadCoreTimer();
    acc2=timer_accum;
    if (acc2!=acc1) {
        tim=ReadCoreTimer();
    }
    return acc2+tim;
}

void wait_ticks(long n) {
    long long donetime=get_timer()+n;
    while (get_timer()<donetime);
}

void wait_us(int n) {
    long long donetime=get_timer()+(n*tps)/1000000;
    while (get_timer()<donetime);
}
void wait_ms(int n) {
    long long donetime=get_timer()+(n*tps)/1000;
    while (get_timer()<donetime);
}
void wait_s(int n) {
    long long donetime=get_timer()+n*tps;
    while (get_timer()<donetime);
}
