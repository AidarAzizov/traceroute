#include "../includes/ft_traceroute.h"

extern mypair work;

BOOL    ip_prepare(IcmpTargetType *target)
{
    target->_ip->tot_len = htons(sizeof(struct iphdr) +
                                 sizeof(struct icmphdr) + PACKSIZE);
    target->_ip->protocol = target->_addrinfo->ai_protocol;
    target->_ip->ihl = sizeof(*(target->_ip)) >> 2;
    target->_ip->frag_off = 0;
    target->_ip->version = 4;
    target->_ip->check = 0;
    target->_ip->tos = 0;
    target->_ip->ttl = 1;

    if (inet_pton(target->_addrinfo->ai_family,
                  target->src_addr, &(target->_ip->saddr)) != 1)
        return printAndExit("Error inet_pton1");
    if (inet_pton(target->_addrinfo->ai_family,
                  target->dst_addr, &(target->_ip->daddr)) != 1)
        return printAndExit("Error inet_pton2");
    return TRUE;
}

void    msg_prepare(IcmpTargetType *target)
{
    ft_memset(target->packToRecv, 0, target->packlen);
    ft_memset(&target->msg, 0, sizeof(struct msghdr));
    target->msg.msg_iovlen = 1;
    target->msg.msg_name = target->_addrinfo->ai_addr;
    target->msg.msg_namelen = target->_addrinfo->ai_addrlen;
    target->msg.msg_flags = 0;
    target->msg.msg_control = NULL;
    target->msg.msg_controllen = 0;
}

void    icmp_prepare(IcmpTargetType *target)
{
    target->_icmp->un.echo.sequence = 0;
    target->_icmp->type = ICMP_ECHO;
    target->_icmp->un.echo.id = 1;
    target->_icmp->checksum = 0;
    target->_icmp->code = 0;
}

BOOL    prepareTargets(IcmpTargetType *target)
{
    uint16_t    i;
    size_t      offset;

    ft_strcpy(target->src_addr, LOCALHOST);
    if (!ip_prepare(target))
        return FALSE;
    icmp_prepare(target);
    msg_prepare(target);
    offset = sizeof(struct iphdr) + sizeof(struct icmphdr);
    i = 0;
    while (++i < PACKSIZE)
        target->packToSend[offset + i] = (char)i;
    return TRUE;
}

void    nullifyTargets(IcmpTargetType *target)
{
    ft_memset(&target->iov, 0, sizeof(target->iov));
    ft_memset(&target->_ip->saddr, 0, sizeof(in_addr_t));
    ft_memset(&target->_ip->daddr, 0, sizeof(in_addr_t));
    ft_memset(target->packToSend, 0, target->packlen);
    ft_memset(&target->hints, 0, sizeof(struct addrinfo));
    target->_addrinfo = NULL;
    target->sendsocket = -1;
    target->recvsocket = -1;


    work.alreadyend = FALSE;
}
