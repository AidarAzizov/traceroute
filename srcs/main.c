#include "../includes/ft_traceroute.h"

mypair  work;

void    setSignal(int sig)
{
    if (sig == SIGALRM) work.stillwait = FALSE;
    else if (sig == SIGINT) work.alreadyend = TRUE;
}

void    setBaseFlags(flags *fl)
{
    fl->first_ttl = 1;
    fl->max_ttl = 30;
    fl->count_queries = 3;
    fl->z = 0.0;
    fl->print_info = FALSE;
}

int     printInfo()
{
    printf("\nUsage\n\tping [options] <destination>\n");
    printf("\nOptions:\n  <destination>\t\tdns name or ip address");
    printf("\n  -dt <delay>\t\tinstall <delay>(may"
           " be less than 1.0) time for delay between pings");
    printf("\n  -c <count>\t\tstop after <count> replies");
    printf("\n  -h\t\t\tprint help and exit");
    printf("\n  -s <size>\t\tuse <size> as number of data bytes to be sent");
    printf("\n  -w <deadline>\t\treply wait <deadline> in seconds");
    printf("\n");
    return SUCCESS_CODE;
}

BOOL    checkArgs(flags *fl)
{
    if (fl->max_ttl > 255)
        return printAndExit("max hops cannot be more than 255");
    if (fl->first_ttl > fl->max_ttl)
        return printAndExit("first hop out of range");
    if (fl->count_queries > 10)
        return printAndExit("no more than 10 probes per hop");
    if (fl->z < 0)
        return printAndExit(" out of range argument for -z flag");
    if (fl->z > 1000.0) fl->z = 1000.0;
    if (fl->z > 10.0) fl->z /= 1000.0;

    return TRUE;
}

BOOL    baseInit(IcmpTargetType  *targets, int argc, char *argv[])
{
    targets->uid = getuid();
    targets->addr_from_arg = NULL;
    targets->_addrinfo = NULL;
    targets->packToSend = NULL;
    targets->packToRecv = NULL;
    targets->recvsocket = -1;
    if (argc == 1)
        return printAndExit("usage error:"
                            "Destination address required");
    if (targets->uid != ROOT_UID)
        return printAndExit("Launch this program "
                            "from root for create RAW socket\n");
    setBaseFlags(&targets->currentFlags);
    if (!init_addr(targets, argv[1]))
        return FALSE;
    return TRUE;
}

int     main(int argc, char *argv[])
{
    IcmpTargetType  targets;
    BOOL            allIsGood = TRUE;

    signal(SIGALRM, setSignal);
    signal(SIGINT, setSignal);
    while (TRUE)
    {
        if (!baseInit(&targets, argc, argv))
            allIsGood = FALSE;
        if (allIsGood && !parseArgs(&targets, argc - 1, argv + 1))
            allIsGood = FALSE;
        if (allIsGood && targets.currentFlags.print_info)
            return printInfo();
        if (allIsGood && !checkArgs(&targets.currentFlags))
            allIsGood = FALSE;
        if (allIsGood && !bindingPacket(&targets))
            allIsGood = FALSE;
        if (allIsGood && !traceroute(&targets))
            allIsGood = FALSE;
        break;
    }
    clearPacket(&targets);

    if (!allIsGood) return ERROR_CODE;
    return SUCCESS_CODE;
}
