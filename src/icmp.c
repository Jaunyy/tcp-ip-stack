/* Internet Control Message Protocol handler. This file helps process ICMP Echo Requests (ping packets) and generates Echo Replies. 
 * This implements the core ping functionality.
 */
#include "icmp.h"

/**
 * Processes ICMP packets, specifically Echo Requests (ping).
 * Constructs and sends Echo Reply packets back to the sender.
 */
void handle_icmp(struct netdev *dev, struct eth_hdr *eth, struct iphdr *ip) {
    struct icmp_v4 *icmp = (struct icmp_v4 *)((uint8_t *)ip + sizeof(struct iphdr));

    // Only handle Echo Request (type 8, code 0)
    if (icmp->type != 8 || icmp->code != 0) return;

    int ip_header_len = sizeof(struct iphdr);
    int icmp_len = ntohs(ip->tot_len) - ip_header_len;

    // Calculate payload info for debugging
    int icmp_data_len = icmp_len - sizeof(struct icmp_v4);
    char *data = (char *)(icmp + 1);

    printf("ICMP Echo Request received with payload: %.*s\n", icmp_data_len, data);

    int total_len = sizeof(struct eth_hdr) + sizeof(struct iphdr) + icmp_len;
    struct eth_hdr *reply_eth = malloc(total_len);
    if (!reply_eth) return;

    struct iphdr *reply_ip = (struct iphdr *)(reply_eth + 1);
    struct icmp_v4 *reply_icmp = (struct icmp_v4 *)((uint8_t *)reply_ip + sizeof(struct iphdr));

    // Fill Ethernet header directly
    memcpy(reply_eth->dmac, eth->smac, 6);
    memcpy(reply_eth->smac, dev->mac, 6);
    reply_eth->ethertype = htons(0x0800);  // IPv4

    // Fill IP header
    *reply_ip = (struct iphdr){
        .version = 4,
        .ihl = 5,
        .tos = 0,
        .tot_len = htons(sizeof(struct iphdr) + icmp_len),
        .id = 0,
        .frag_off = htons(0x4000),  // Don't Fragment flag
        .ttl = 64,
        .protocol = 1,  // ICMP
        .check = 0,
        .saddr = dev->ip_addr,
        .daddr = ip->saddr,
    };
    reply_ip->check = ip_checksum(reply_ip, sizeof(struct iphdr));

    // Fill ICMP header and payload
    memcpy(reply_icmp, icmp, icmp_len);
    reply_icmp->type = 0;  // Echo Reply
    reply_icmp->code = 0;
    reply_icmp->csum = 0;
    reply_icmp->csum = ip_checksum(reply_icmp, icmp_len);

    eth_send_frame(dev, reply_eth, total_len);
    free(reply_eth);
}