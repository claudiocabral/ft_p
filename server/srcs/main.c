#include <arpa/inet.h>
#include <stdio.h>
#include <server.h>

int ft_atoi(const char *str)
{
    long        result;
    int         digit;
    const char  *head;

    if (!str)
        return (0);
    head = str;
    while(*str && *str >= '0' && *str <= '9')
        ++str;
    digit = 1;
    result = 0;
    --str;
    while (str >= head)
    {
        if (*str < '0' || *str > '9')
            return (result);
        result += (*str - '0') * digit;
        digit *= 10;
        --str;
    }
    return (result);

}

int ft_main(int argc, char **argv)
{
    int port;

    if (argc != 2)
    {
        printf("usage: ./server port\n");
        return (1);
    }
    port = ft_atoi(argv[1]);
    if (port <= 0 || port > 65000)
        return (1);
    start_server(port);
    return (0);
}

int main(int argc, char **argv)
{
    return (ft_main(argc, argv));
}
