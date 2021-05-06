#include "../includes/ft_traceroute.h"

extern mypair   work;

void    recyclePacket(IcmpTargetType *target)
{
    target->CountLastTraceroute = 0;
    target->_ip->ttl = target->currentFlags.first_ttl;
    ++target->currentFlags.first_ttl;
    target->_icmp->un.echo.sequence++;
    target->_icmp->checksum = 0;
    target->_icmp->checksum = ~GetCSum((uint8_t*)target->_icmp,
                                       sizeof(struct icmphdr) + PACKSIZE, 0);
    ft_memset(target->packToRecv, 0, target->packlen);
    target->iov.iov_base = target->packToRecv;
    target->iov.iov_len = target->packlen;
    target->msg.msg_iov = &target->iov;
    work.stillwait = TRUE;
}

void    clearPacket(IcmpTargetType *target)
{
    if (target->addr_from_arg)
        free(target->addr_from_arg);
    if (target->_addrinfo)
        freeaddrinfo(target->_addrinfo);
    if (target->packToSend)
        free(target->packToSend);
    if (target->packToRecv)
        free(target->packToRecv);
    if (INITED(target->recvsocket))
        close(target->recvsocket);
}

BOOL    bindingPacket(IcmpTargetType *targets)
{
    targets->packlen = sizeof(struct iphdr) +
            sizeof(struct icmphdr) + PACKSIZE;
    if (!(targets->packToSend = (char*)malloc(sizeof(char) * targets->packlen)))
        return printAndExit("System error. Bad allocate memory");
    if (!(targets->packToRecv = (char*)malloc(sizeof(char) * targets->packlen)))
        return printAndExit("System error. Bad allocate memory");
    targets->_ip = (struct iphdr*)targets->packToSend;
    targets->_icmp = (struct icmphdr*)(targets->packToSend + sizeof(struct iphdr));
    return TRUE;
}
