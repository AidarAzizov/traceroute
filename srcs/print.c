#include "../includes/ft_traceroute.h"

BOOL StopTracerouting(const IcmpTargetType *target)
{
    struct iphdr   *ip_h;
    struct icmphdr *icmp_h;

    icmp_h = (struct icmphdr*)(target->packToRecv + sizeof(struct iphdr));
    ip_h = (struct iphdr*)target->packToRecv;
    if (ip_h->saddr == target->_ip->daddr &&
        ip_h->protocol == target->_ip->protocol &&
        ip_h->tot_len == target->_ip->tot_len &&
        icmp_h->un.echo.id == target->_icmp->un.echo.id &&
        icmp_h->un.echo.sequence == target->_icmp->un.echo.sequence &&
        icmp_h->type == ICMP_ECHOREPLY)
        return TRUE;
    return FALSE;
}

BOOL    AcceptablePackage(IcmpTargetType *target)
{
    struct icmphdr     *icmp_h;
    uint16_t    temp;

    icmp_h = (struct icmphdr*)(target->packToRecv + sizeof(struct iphdr));
    temp = icmp_h->checksum;
    icmp_h->checksum = 0;
    uint16_t res = ~GetCSum((uint8_t*)icmp_h,
                            sizeof(struct icmphdr) + PACKSIZE, 0);
    if (!(icmp_h->type == ICMP_ECHOREPLY || icmp_h->type == ICMP_TIME_EXCEEDED))
    {
        printf("\nError code: %d\n", icmp_h->type);
        return FALSE;
    }
    if (res != temp)
        return printAndExit("In received header checksum is not true");
    return TRUE;
}

void    printHead(IcmpTargetType *targets)
{
    int len = PACKSIZE + sizeof(struct iphdr) + sizeof(struct icmphdr);
    printf("traceroute to %s (%s), %d hops max, %d byte packets.\n",
           targets->addr_from_arg, targets->dst_addr, (int)targets->currentFlags.max_ttl, len);
}
