
/* The main file creates the TAP device, initalizes the netwrok with IP/MAC addresses
 * and continously reads packets from the TAP device to process them. Soon,
 * I plan on expanding this to fully implement the TCP process.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/if.h>
#include <arpa/inet.h>
#include "tap_device.h"
#include "ethernet.h"
#include "netdev.h"

#define BUFLEN 2000

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <tap-device-name>\n", argv[0]);
        return 1;
    }

    char tap_name[IFNAMSIZ];
    strncpy(tap_name, argv[1], IFNAMSIZ - 1);
    tap_name[IFNAMSIZ - 1] = '\0';

    // initialize netdev struct
    struct netdev netdev = {
        .name = "mytap0",
        .mac = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff},
         // set a proper (interchangeable) IP address
        .ip_addr = inet_addr("192.168.100.1"),
        .fd = -1
    };

    // creating the TAP network.
    netdev.fd = tun_alloc(tap_name);
    if (netdev.fd < 0) {
        fprintf(stderr, "Error creating TAP device: %s\n", tap_name);
        return 1;
    }

    printf("Successfully opened TAP device: %s\n", tap_name);
    printf("File descriptor: %d\n", netdev.fd);
    printf("IP address: %s\n", inet_ntoa((struct in_addr){.s_addr = netdev.ip_addr}));

    // Main loop: read from tap, handle frames
    uint8_t buf[1600];
    while (1) {
        ssize_t nread = read(netdev.fd, buf, sizeof(buf));
        if (nread < 0) {
            perror("read from tap failed");
            break;
        }

        printf("Read %ld bytes from TAP device\n", nread);
        handle_frame(&netdev, buf, (size_t)nread);
    }

    close(netdev.fd);
    return 0;
}