# User-Space TCP/IP Stack

A minimal implementation of a TCP/IP network stack in C that runs in user-space using TAP devices.

## About This Project

I built this following Sami Niiranen's tutorial series on coding a TCP/IP stack from scratch:
- [Let's code a TCP/IP stack, 1: Ethernet & ARP](https://www.saminiir.com/lets-code-tcp-ip-stack-1-ethernet-arp/) (Part 1)
- [Let's code a TCP/IP stack, 2: IPv4 & ICMPv4](https://www.saminiir.com/lets-code-tcp-ip-stack-2-ipv4-icmpv4/) (Part 2)

Turns out reinventing the wheel is actually pretty fun when the wheel is made of packets and you get to see how the internet actually works behind all the magic.

**What works:** Ping it and it pings back 😎 It's like having a very polite digital pet chihuahua that only knows one trick.

**What doesn't work:** Pretty much everything else you'd want to do on a network. No browsing Reddit, no downloading cat videos, no online gaming. Just good old-fashioned ICMP echo replies. But hey, at least I understand why my WiFi drops out now (spoiler: it's probably not my fault).

## Features

- **TAP Device Management** - Creates and manages virtual network interfaces
- **Ethernet Frame Processing** - Parses Layer 2 frames and dispatches to protocol handlers
- **ARP Implementation** - Handles address resolution between IP and MAC addresses
- **IPv4 Support** - Basic IPv4 packet validation and routing
- **ICMP Echo** - Responds to ping requests (Echo Request/Reply)
- **Checksum Validation** - Implements Internet checksum for packet integrity

## Architecture

```
┌─────────────┐    ┌──────────────┐    ┌─────────────┐
│   Ping App  │────│ Kernel Stack │────│  TAP Device │
└─────────────┘    └──────────────┘    └─────────────┘
                                              │
                                       ┌─────────────┐
                                       │ Our Network │
                                       │    Stack    │
                                       └─────────────┘
```

## Files

- `main.c` - Entry point and main packet processing loop
- `tap_device.c` - TAP device creation and management
- `ethernet.c` - Ethernet frame parsing and dispatching
- `arp.c` - ARP request/reply handling
- `ip.c` - IPv4 packet validation and routing
- `icmp.c` - ICMP echo request/reply implementation
- `checksum.c` - Internet checksum calculation
- Header files (`.h`) - Function declarations and data structures

## Building

```bash
gcc -o mynetstack main.c tap_device.c ethernet.c arp.c ip.c icmp.c checksum.c -Wall
```

## Usage

1. **Run the network stack** (requires root for TAP device creation):
   ```bash
   sudo ./mynetstack mytap0
   ```

2. **Configure the interface** (in another terminal):
   ```bash
   sudo ip link set mytap0 nomaster
   sudo ip addr add 192.168.100.2/24 dev mytap0  
   sudo ip link set mytap0 up
   sudo ip route add 192.168.100.1/32 dev mytap0
   ```

3. **Test with ping**:
   ```bash
   ping -c 4 192.168.100.1
   ```

## Expected Output

**Network Stack Terminal:**
```
Successfully opened TAP device: mytap0
File descriptor: 3
IP address: 192.168.100.1
Read 42 bytes from TAP device
[ETH] ARP frame received
Sent ARP reply
Read 98 bytes from TAP device
[ETH] IPv4 frame received
[IP] Valid IPv4 packet for us, protocol: 1
ICMP Echo Request received with payload: abcdefghijklmnopqrsuvwabcdefghi
```

**Ping Terminal:**
```
PING 192.168.100.1 (192.168.100.1) 56(84) bytes of data.
64 bytes from 192.168.100.1: icmp_seq=1 ttl=64 time=0.123 ms
64 bytes from 192.168.100.1: icmp_seq=2 ttl=64 time=0.089 ms
```

## Requirements

- Linux system with TAP/TUN support
- Root privileges (for TAP device creation)  
- GCC compiler
- Standard C library and Linux headers

## Learning Goals

This project demonstrates:
- Network protocol implementation
- Packet parsing and construction
- System-level programming with file descriptors
- Understanding of the TCP/IP stack layers
- Binary data manipulation in C

## Future Enhancements

- [ ] TCP implementation
- [ ] UDP support  
- [ ] IP fragmentation handling
- [ ] Routing table support
- [ ] Multiple interface support
