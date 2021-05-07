#include "../includes/ft_traceroute.h"

extern mypair work;

BOOL    sender(IcmpTargetType *targets)
{
    struct sockaddr_in connection;
    int         howSended;
    int         len;

    connection.sin_port = targets->_addrinfo->ai_protocol;
    connection.sin_family = targets->_addrinfo->ai_family;
    connection.sin_addr.s_addr =
            ((struct sockaddr_in*)&targets->_addrinfo->ai_addr)->sin_addr.s_addr;
    len = targets->packlen;

    createSSocket(targets);
    howSended = sendto(targets->sendsocket, targets->packToSend, len, 0,
                       (struct sockaddr*)&connection, sizeof(struct sockaddr));
    //close(targets->sendsocket); targets->sendsocket = -1;
    if (howSended == len) return TRUE;
    if (howSended == -1) return printAndExit("Connection closed");
    return printAndExit("Bad try to send packet");
}

BOOL    receiver(IcmpTargetType *target)
{
    ssize_t howReceived = -1;

    alarm(COUNT_W);
    while (work.stillwait && !work.alreadyend && howReceived == -1)
        howReceived = recvmsg(target->sendsocket, &target->msg, MSG_DONTWAIT);
    alarm(0);

    if (howReceived == -1)
    {
        ft_strcpy(target->prev_address, "*");
        printf("*  "); return TRUE;
    }
    if (!AcceptablePackage(target)) return FALSE;
    if (StopTracerouting(target)) target->CountLastTraceroute++;
    if (!initTime(&target->timeafter)) return FALSE;
    double time = (target->timeafter.tv_sec - target->timebefore.tv_sec) * 1000. +
           (double)(target->timeafter.tv_usec - target->timebefore.tv_usec) / 1000.;

    struct iphdr *ip_h = (struct iphdr*)target->packToRecv;
    struct sockaddr_in addr; addr.sin_addr.s_addr = ip_h->saddr;

    struct hostent *res = gethostbyaddr(&addr.sin_addr.s_addr, sizeof(addr.sin_addr.s_addr), AF_INET);

    if (ft_strcmp(target->prev_address, inet_ntoa(addr.sin_addr)))
    {
        ft_strcpy(target->prev_address, inet_ntoa(addr.sin_addr));
        if (res) printf("%s (%s) ", res->h_name, target->prev_address);
        else printf("%s ", target->prev_address);
    }
    printf("%.3lf ms  ", time);
    return TRUE;
}
