#include "../includes/ft_traceroute.h"

extern mypair work;

BOOL    makeDelay(double delay_sec, BOOL *is_firstIter)
{
    struct timeval  beg;
    struct timeval  curr;
    double          deltatime;

    if (*is_firstIter)
    {
        *is_firstIter = FALSE;
        return TRUE;
    }
    initTime(&beg);
    while (!work.alreadyend)
    {
        initTime(&curr);
        deltatime = (double)(curr.tv_sec - beg.tv_sec) +
                (double)(curr.tv_usec - beg.tv_usec)/1000000.;
        if (deltatime >= delay_sec)
            break;
    }
    if (work.alreadyend)
        return FALSE;
    return TRUE;
}

BOOL pingPrepare(IcmpTargetType *targets)
{
    int result;

    nullifyTargets(targets);
    targets->hints.ai_family = AF_INET;
    targets->hints.ai_socktype = SOCK_RAW;
    targets->hints.ai_protocol = IPPROTO_ICMP;
    result = getaddrinfo(targets->dst_addr, NULL, &targets->hints, &targets->_addrinfo);
    if (result != 0 || !targets->_addrinfo)
        return printAndExit("No address associated with hostname");

    if (!prepareTargets(targets))
        return FALSE;
    printHead(targets);
    return TRUE;
}

BOOL    traceroute(IcmpTargetType *targets)
{
    BOOL firstIter = TRUE;
    if (!pingPrepare(targets))
        return FALSE;

    uint64_t i;
    createRSocket(targets);
    while (!work.alreadyend && targets->currentFlags.first_ttl <= targets->currentFlags.max_ttl)
    {
        if (!makeDelay(targets->currentFlags.z, &firstIter)) continue;
        recyclePacket(targets);

        i = 0;
        printf("%3d ", targets->_ip->ttl);
        while (i++ < targets->currentFlags.count_queries)
        {
            if (!initTime(&targets->timebefore)) return FALSE;

            if (!sender(targets)) return FALSE;
            if (!receiver(targets)) return FALSE;
        }
        printf("\n");
        if (targets->CountLastTraceroute == targets->currentFlags.count_queries) break;
    }
    return TRUE;
}
