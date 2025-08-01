/* Handles Address Resolution Protocol (ARP) requests.
 * The ARP implementation ensures that the MAC address of the device
 * with the same IP as a request is sent back to the requester.
 * When another device wants to communicate with our IP address, it sends
 * an ARP request asking "who has IP X.X.X.X?". This function responds
 * with "I have that IP, and my MAC address is Y:Y:Y:Y:Y:Y".
 */
#include "arp.h"
#include "netdev.h"
#include "ethernet.h"

struct arp_hdr {
    uint16_t hwtype;
    uint16_t protype;
    unsigned char hwsize;
    unsigned char prosize;
    uint16_t opcode;
    unsigned char data[];
} __attribute__((packed));

struct arp_ipv4 {
    unsigned char smac[6];
    uint32_t sip;
    unsigned char dmac[6];
    uint32_t dip;
} __attribute__((packed));

/**
 * Handles ARP (Address Resolution Protocol) requests.
 * Responds with our MAC address when someone asks for our IP.
 */
void handle_arp(struct netdev *dev, struct eth_hdr *eth, const uint8_t *packet, size_t len) {
    struct arp_hdr *newPack = (struct arp_hdr *)packet;
    struct arp_ipv4 *arp_data = (struct arp_ipv4 *)newPack->data;

    if (ntohs(newPack->opcode) == 1 && arp_data->dip == dev->ip_addr) {
        uint8_t reply[sizeof(struct eth_hdr) + sizeof(struct arp_hdr) + sizeof(struct arp_ipv4)];
        // create the response
        struct eth_hdr *eth_reply = (struct eth_hdr *)reply;
        struct arp_hdr *arp_reply = (struct arp_hdr *) eth_reply->payload;
        struct arp_ipv4 *arp_ipv4_reply = (struct arp_ipv4 *)arp_reply->data;

        memcpy(eth_reply->dmac, arp_data->smac, 6);
        memcpy(eth_reply->smac, dev->mac, 6);
        eth_reply->ethertype = htons(0x0806);

        arp_reply->hwtype = htons(1);
        arp_reply->protype = htons(0x0800);
        arp_reply->hwsize = 6;
        arp_reply->prosize = 4;
        arp_reply->opcode = htons(2);

        memcpy(arp_ipv4_reply->smac, dev->mac, 6);
        arp_ipv4_reply->sip = dev->ip_addr;
        memcpy(arp_ipv4_reply->dmac, arp_data->smac, 6);
        arp_ipv4_reply->dip = arp_data->sip;

        // Use eth_send_frame instead of direct write
        eth_send_frame(dev, reply, sizeof(reply));
        printf("Sent ARP reply\n");
    }
}