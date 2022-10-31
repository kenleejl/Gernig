#pragma once

bool asm_detection();
bool rdtsc_cpuid_check();

void mac_addr_checks();

void sidt();

void process_checks();

void sleep_check(int sleep_time);

