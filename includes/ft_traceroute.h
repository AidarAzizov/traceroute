#pragma once

#ifndef FT_PING_H
# define FT_PING_H

#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <netdb.h>
#include <signal.h>
#include <ifaddrs.h>

#define SUCCESS_CODE 0
#define ERROR_CODE 1

#define PACKSIZE 32
#define COUNT_W 5.0

#define LOCALHOST "0.0.0.0"

#define BOOL int
#define TRUE 1
#define FALSE 0

#define ROOT_UID 0
#define SOCKET int
#define INITED(x) (x != -1)


typedef struct
{
    uint8_t i1;
    uint8_t i2;
    uint8_t i3;
    uint8_t i4;
} ip_addr;

typedef struct
{
    uint64_t    first_ttl; //-f
    uint64_t    max_ttl; //-m
    uint64_t    count_queries; //number of probe -q
    double      z; //delay between tracerouting
    BOOL        print_info; // help
} flags;

typedef struct
{
    BOOL stillwait;
    BOOL alreadyend;
} mypair;

typedef struct
{
    flags           currentFlags;
    struct iphdr    *_ip;

    struct icmphdr  *_icmp;
    struct addrinfo *_addrinfo;
    struct addrinfo hints;
    struct iovec    iov;
    struct msghdr   msg;
    struct timeval  timebefore;
    struct timeval  timeafter;
    SOCKET          sendsocket;
    SOCKET          recvsocket;
    struct timeval  timebegin;
    struct timeval  timeend;
    __uid_t         uid;

    char            prev_address[NI_MAXHOST];
    uint8_t         CountLastTraceroute;
    BOOL            addr_from_arg_host;
    char            *addr_from_arg;
    char            dst_addr[INET_ADDRSTRLEN];
    char            src_addr[INET_ADDRSTRLEN];
    char            *packToRecv;
    char            *packToSend;
    uint16_t        packlen;
} IcmpTargetType;


BOOL        StopTracerouting(const IcmpTargetType *target);
BOOL        AcceptablePackage(IcmpTargetType *target);
void        printHead(IcmpTargetType *targets);

BOOL        createSSocket(IcmpTargetType *target);
BOOL        createRSocket(IcmpTargetType *target);

BOOL        init_addr(IcmpTargetType *targets, const char* arg);
BOOL        parseArgs(IcmpTargetType *targets, int argc, char *argv[]);

int         count_not_of(const char *str, char symb);
BOOL        initTime(struct timeval* timeout);
BOOL        printAndExitWStr(const char* message, const char *second_message);
int         printAndExitWInt(const char *message, int code);
BOOL        printAndExit(const char *message);
uint16_t	GetCSum(const uint8_t *Buf, uint32_t Len, uint16_t StartVal);

BOOL        prepareTargets(IcmpTargetType *target);
void        nullifyTargets(IcmpTargetType *target);

void        recyclePacket(IcmpTargetType *target);
void        clearPacket(IcmpTargetType *target);
BOOL        bindingPacket(IcmpTargetType *targets);

BOOL        sender(IcmpTargetType *targets);
BOOL        receiver(IcmpTargetType *target);

BOOL        traceroute(IcmpTargetType *targets);

size_t      ft_strlen(char const *s);
int         ft_strncmp(const char *s1, const char *s2, unsigned int n);
void        *ft_memset(void *destination, int c, size_t n);
char        *ft_strcpy(char *s1, const char *s2);
int         ft_strcmp(const char *s1, const char *s2);

# endif
