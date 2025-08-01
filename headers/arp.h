#pragma once
#ifndef ARP_H
#define ARP_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>


struct netdev;
struct eth_hdr;

void handle_arp(struct netdev *dev, struct eth_hdr *eth, const uint8_t *packet, size_t len);

#endif