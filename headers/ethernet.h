#pragma once
#ifndef ETHERNET_H
#define ETHERNET_H
#include "netdev.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h> // for write()
#include <linux/if_ether.h>

struct eth_hdr {
    unsigned char dmac[6];
    unsigned char smac[6];
    uint16_t ethertype;
    unsigned char payload[];
} __attribute__((packed));

struct eth_hdr *init_eth_hdr(char *buf);
void handle_frame(struct netdev *dev, uint8_t *buf, size_t len);
void eth_send_frame(struct netdev *dev, const void *frame, size_t len);

#endif