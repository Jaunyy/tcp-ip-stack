/* This file is the Ethernet frame parser and dispatcher. 
 * Examines incoming frames, identifies the protocol type (ARP, IPv4, IPv6),
 * and routes them to the appropriate protocol handlers.
 */

#include "ethernet.h"
#include "arp.h"
#include "ip.h"


/* Initializes the ethernet header.
 */
struct eth_hdr *init_eth_hdr(char *buf) {
    return (struct eth_hdr *)buf;
}

/* Sends an Ethernet frame out through the TAP device.
 * This function writes the complete frame (including Ethernet header)
 * to the TAP device file descriptor, making it appear on the network.
 */
void eth_send_frame(struct netdev *dev, const void *frame, size_t len) {
    ssize_t written = write(dev->fd, frame, len);
    if (written < 0) {
        perror("write to tap failed");
    } else if ((size_t)written != len) {
        fprintf(stderr, "partial frame write: %ld / %ld bytes\n", written, len);
    }
}

/**
 * Handles incoming Ethernet frames from the TAP device.
 * Parses the Ethernet header and dispatches to protocol handlers
 * based on EtherType (ARP, IPv4, etc.)
 */
void handle_frame(struct netdev *dev, uint8_t *buf, size_t len) {
    if (len < sizeof(struct eth_hdr)) {
        fprintf(stderr, "[ETH] Frame too short\n");
        return;
    }

    struct eth_hdr *eth = (struct eth_hdr *)buf;
    uint16_t type = ntohs(eth->ethertype);

    switch (type) {
        case ETH_P_ARP:
            printf("[ETH] ARP frame received\n");
            handle_arp(dev, eth, eth->payload, len - sizeof(struct eth_hdr));
            break;

        case ETH_P_IP:
            printf("[ETH] IPv4 frame received\n");
            handle_ip(dev, eth);
            break;

        default:
            printf("[ETH] Unknown ethertype 0x%04x\n", type);
            break;
    }
}
