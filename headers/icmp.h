#pragma once
#ifndef ICMP_H
#define ICMP_H

#include "ip.h"
#include "checksum.h"
#include "ethernet.h"
#include <stdint.h>
#include <arpa/inet.h>

struct netdev;
struct eth_hdr;
struct iphdr;

struct icmp_v4 {
    uint8_t type;
    uint8_t code;
    uint16_t csum;
    uint8_t data[];
} __attribute__((packed));

struct icmp_v4_echo {
    uint16_t id;
    uint16_t seq;
    uint8_t data[];
} __attribute__((packed));

void handle_icmp(struct netdev *dev, struct eth_hdr *eth, struct iphdr *ip);

#endif
