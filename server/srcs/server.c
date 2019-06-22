#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <server.h>
#include <stdio.h>
#include <stdlib.h>

void    fd_apply(int *fds, size_t size, t_func f) {
    (void)fds;
    (void)size;
    (void)f;
}

struct sockaddr_in  make_sockaddr(int port)
{
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    return (addr);
}

int     socket_open(int port)
{
    int sock;
    struct sockaddr_in addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return (ERROR);
    addr = make_sockaddr(port);
    if ((bind(sock, (struct sockaddr *)&addr, sizeof(addr))) < 0)
    {
        close(sock);
        return (ERROR);
    }
    if ((listen(sock, FD_SETSIZE)) < 0)
    {
        close(sock);
        return (ERROR);
    }
    return (sock);
}

void    server_connect(t_server *server)
{
    int client;

    if ((client = accept(server->socket,
                    (struct sockaddr *)&server->addr, &server->addrlen)) < 0)
        return ;
    if (server->connections[client])
        free(server->connections[client]);
    if (!(server->connections[client]
                = (t_connection *)malloc(sizeof(t_connection))))
        return ;
    server->connections[client]->expects_read = true;
    server->connections[client]->expects_write = true;
    server->nfds = server->nfds <= client
        ? client + 1
        : server->nfds;
}

void    connection_read(t_server *server, int index)
{
    recv(index, server->connections[index]->read_buffer, 256, 0);
    server->connections[index]->expects_read = false;
    server->connections[index]->expects_write = true;
}

void    connection_write(t_server *server, int index)
{
    send(index, "got", 3, 0);
    server->connections[index]->expects_write = false;
}

void    handle_fds(t_server *server, fd_set *readfds, fd_set *writefds)
{
    int i;

    if (FD_ISSET(server->socket, readfds))
        server_connect(server);
    i = server->socket + 1;
    printf("Handling fds\n");
    while (i < server->nfds)
    {
        if (FD_ISSET(i, readfds))
            connection_read(server, i);
        if (FD_ISSET(i, writefds))
            connection_write(server, i);
        ++i;
    }
    printf("Handled fds\n");

}

void    set_fds(t_server *server, fd_set *readfds, fd_set *writefds)
{
    int i;

    FD_ZERO(readfds);
    FD_ZERO(writefds);
    FD_SET(server->socket, readfds);
    i = server->socket + 1;
    while (i < server->nfds)
    {
        if (server->connections[i] && server->connections[i]->expects_read)
            FD_SET(i, readfds);
        if (server->connections[i] && server->connections[i]->expects_write)
            FD_SET(i, writefds);
        ++i;
    }

}

void    server_loop(t_server *server)
{
    fd_set  readfds;
    fd_set  writefds;

    set_fds(server, &readfds, &writefds);
    if ((select(server->nfds, &readfds, &writefds, NULL, NULL)) < 0)
        return ;
    printf("fds available\n");
    handle_fds(server, &readfds, &writefds);
}

void    start_server(int port)
{
    t_server    server;

    if ((server.socket = socket_open(port)) < 0)
        return ;
    server.addr = make_sockaddr(port);
    server.addrlen = sizeof(server.addr);
    server.nfds = server.socket + 1;
    int i = 0;
    while (i < FD_SETSIZE + 1)
    {
        server.connections[i] = NULL;
        ++i;
    }
    printf("Server started on port %d\n", port);
    while (1)
    {
      server_loop(&server);
    }
}
