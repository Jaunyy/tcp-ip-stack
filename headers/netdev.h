#pragma once
#ifndef NETDEV_H
#define NETDEV_H

#include <stdint.h>
#include <stddef.h>

struct netdev {
    char name[16];
    uint8_t mac[6];
    uint32_t ip_addr;
    int fd;
};

#endif

