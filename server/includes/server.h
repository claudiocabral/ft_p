#ifndef SERVER_H
# define SERVER_H
# include <sys/select.h>
# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include <netinet/in.h>

# define ERROR -1

typedef void    (*t_func)(int fd);

typedef uint8_t t_buffer[1024];

typedef struct   s_connection
{
    bool        expects_read;
    bool        expects_write;
    t_buffer    read_buffer;
    t_buffer    write_buffer;
}               t_connection;

typedef struct  s_server
{
    int                 socket;
    int                 nfds;
    t_connection        *connections[FD_SETSIZE + 1];
    struct sockaddr_in  addr;
    socklen_t           addrlen;
}               t_server;

void    start_server(int port);

#endif
