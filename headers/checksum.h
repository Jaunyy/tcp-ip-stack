#pragma once
#ifndef CHECKSUM_H
#define CHECKSUM_H
#include <stdint.h>

uint16_t ip_checksum(void *buf, int len);

#endif