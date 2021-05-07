#include "../includes/ft_traceroute.h"

BOOL    HostToIP(IcmpTargetType *targets)
{
    struct sockaddr_in *in;
    struct addrinfo hints;
    struct addrinfo *res;
    int family;

    res = NULL;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;
    ft_memset(targets->dst_addr, 0, INET_ADDRSTRLEN);
    if (getaddrinfo(targets->addr_from_arg, NULL, &hints, &res) != 0)
    {
        if (res) freeaddrinfo(res);
        return printAndExitWStr(targets->addr_from_arg, ": Name or service not known");
    }
    family = res->ai_family;
    in = (struct sockaddr_in*)res->ai_addr;
    if (!inet_ntop(family, &(in->sin_addr), targets->dst_addr, sizeof(targets->dst_addr)))
        return printAndExitWStr("Error inet_ntop() for ", targets->addr_from_arg);
    freeaddrinfo(res);
    return TRUE;
}

BOOL    init_count(uint64_t* res, int *index, int argc, char* argv[])
{
    char flag;
    char symb;

    if (sscanf(argv[*index], " -%c %ld %c ", &flag, res, &symb) == 2)
        return TRUE;
    if (ft_strlen(argv[*index]) > 2)
        return printAndExitWStr("invalid argument: ", argv[*index]);
    (*index)++;
    if (*index > argc)
        return printAndExitWStr("option requires an argument -- ", argv[(*index) - 1]);
    if (sscanf(argv[*index], " %ld %c ", res, &symb) != 1)
        return printAndExitWStr("invalid1 argument: ", argv[*index]);
    return TRUE;
}
BOOL    init_delaytime(double* res, int *index, int argc, char* argv[])
{
    char symb;

    if (count_not_of(argv[*index], ' ') > 3 &&
        sscanf(argv[*index], " -dt %lf %c ", res, &symb) == 1)
        return TRUE;
    (*index)++;
    if (*index >= argc)
        return printAndExitWStr("option requires an argument -- ", argv[(*index) - 1]);
    if (sscanf(argv[*index], " %lf %c ", res, &symb) != 1)
        return printAndExitWStr("invalid argument: ", argv[(*index)]);
    return TRUE;
}

BOOL    init_addr(IcmpTargetType *targets, const char* arg)
{
    char    symb;
    ip_addr IPcheck;

    targets->addr_from_arg = (char*)malloc(sizeof(char) * NI_MAXHOST);
    if (!targets->addr_from_arg)
        return printAndExit("Bad with allocate memory");
    targets->addr_from_arg_host = FALSE;
    ft_memset(targets->addr_from_arg, 0, NI_MAXHOST);
    if (sscanf(arg, "%hhu.%hhu.%hhu.%hhu%c",
               &IPcheck.i1, &IPcheck.i2, &IPcheck.i3, &IPcheck.i4, &symb) == 4)
    {
        ft_strcpy(targets->addr_from_arg, arg);
        ft_strcpy(targets->dst_addr, arg);
        return TRUE;
    }
    else if (sscanf(arg, " %s %c", targets->addr_from_arg, &symb) == 1)
        return (targets->addr_from_arg_host = HostToIP(targets));
    return printAndExitWStr(arg, ": Name or service not known");
}

BOOL    parseArgs(IcmpTargetType *targets, int argc, char *argv[])
{
    BOOL result = TRUE;
    int i = 0;
    while (!targets->currentFlags.print_info && result && ++i < argc)
        if (ft_strncmp(argv[i], "-h", 2) == 0 && ft_strlen(argv[i]) == 2)
            targets->currentFlags.print_info = TRUE;
        else if (ft_strncmp(argv[i], "-f", 2) == 0)
            result = init_count(&targets->currentFlags.first_ttl, &i, argc, argv);
        else if (ft_strncmp(argv[i], "-m", 2) == 0)
            result = init_count(&targets->currentFlags.max_ttl, &i, argc, argv);
        else if (ft_strncmp(argv[i], "-q", 2) == 0)
            result = init_count(&targets->currentFlags.count_queries, &i, argc, argv);
        else if (ft_strncmp(argv[i], "-z", 2) == 0)
            result = init_delaytime(&targets->currentFlags.z, &i, argc, argv);
        else
            result = printAndExitWStr("invalid argument: ", argv[i]);
    return result;
}
