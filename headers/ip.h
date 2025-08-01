#pragma once
#ifndef IP_H
#define IP_H

#include <stdint.h>
#include "checksum.h"
#include "icmp.h"
#include "ethernet.h"

struct iphdr {
    uint8_t ihl:4;
    uint8_t version:4;
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check;
    uint32_t saddr;
    uint32_t daddr;
} __attribute__((packed));


#define IP_IHL(hdr)      ((hdr)->ver_ihl & 0x0F)
#define IP_FLAGS(hdr)    (((ntohs((hdr)->frag_off)) & 0xE000) >> 13)
#define IP_FRAG_OFF(hdr) ((ntohs((hdr)->frag_off)) & 0x1FFF)

void handle_ip(struct netdev *dev, struct eth_hdr *eth);

#endif
