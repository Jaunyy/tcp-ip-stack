/* IPv4 packet validaotr and router. Verifies IP header checksums, cvalidates packet integrity, and forwards
 * valid packets detined for the lcoal IP address to higher-layer protocals like ICMP. So far, this only handles ICMP.
 */
#include "ip.h"


/* This handle IP function takes in the netdev struct and an eth_hdr and processes IPv4 packets. It validates
 * the IP headers, verifies checksums, and routes packets to appropriate protocal handlers. dev hodls the 
 * IP/MAC info, and points to the received frame.
 */
void handle_ip(struct netdev *dev, struct eth_hdr *eth) {
    struct iphdr *ip = (struct iphdr *) eth->payload;
    // check IP version (should be 4 for IPv4)
    if (ip->version != 4) {
        printf("[IP] Not IPv4 packet, version: %d\n", ip->version);
        return;
    }
    
    // verify IP header length
    if (ip->ihl < 5) {
        printf("[IP] Invalid IHL: %d\n", ip->ihl);
        return;
    }
    
    // double verify with checksum
    uint16_t received_checksum = ip->check;
    ip->check = 0; 
    uint16_t calculated_checksum = ip_checksum(ip, ip->ihl * 4);
    ip->check = received_checksum;  // Restoring
    
    if (received_checksum != calculated_checksum) {
        printf("[IP] Checksum mismatch: received 0x%04x, calculated 0x%04x\n", 
               received_checksum, calculated_checksum);
        return;
    }
    
    // if packet was meant for us.
    if (ip->daddr != dev->ip_addr) {
        printf("[IP] Packet not for us: dest=%08x, our=%08x\n", 
               ip->daddr, dev->ip_addr);
        return;
    }
    
    printf("[IP] Valid IPv4 packet for us, protocol: %d\n", ip->protocol);
    
    switch (ip->protocol) {
        case 1:  // ICMP
            handle_icmp(dev, eth, ip);
            break;
        default:
            printf("[IP] Unsupported protocol: %d\n", ip->protocol);
            break;
    }
}