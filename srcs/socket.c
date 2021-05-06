#include "../includes/ft_traceroute.h"

BOOL    createSSocket(IcmpTargetType *target)
{
    int yes = 1;
    target->sendsocket = socket(target->_addrinfo->ai_family,
                              target->_addrinfo->ai_socktype,
                              target->_addrinfo->ai_protocol);
    target->hints.ai_protocol = IPPROTO_ICMP;
    if (target->sendsocket < 0)
        return printAndExit("Can't create socket for send");
    if (setsockopt(target->sendsocket, IPPROTO_IP, IP_HDRINCL, &yes, sizeof(int)) < 0)
        return printAndExit("Bad with setting socket options");
    return TRUE;
}

BOOL    createRSocket(IcmpTargetType *target)
{
    target->recvsocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (target->recvsocket < 0)
        return printAndExit("Can't create socket for receive");
    return TRUE;
}
