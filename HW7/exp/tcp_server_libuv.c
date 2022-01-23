#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <uv.h>

uv_loop_t *loop;
struct sockaddr_in addr;

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) 
{
    buf->base = (char*)malloc(suggested_size);
    buf->len = suggested_size;
}

void echo_write(uv_write_t *req, int status) 
{
    if (status) 
    {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
    free(req);
}

void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) 
{
    if (nread < 0) 
    {
        if (nread != UV_EOF) 
        {
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
            uv_close((uv_handle_t*) client, NULL);
        }
    } 
    else if (nread > 0) 
    {
        uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));
        uv_buf_t wrbuf = uv_buf_init(buf->base, nread);
        uv_write(req, client, &wrbuf, 1, echo_write);
    }

    if (buf->base) 
    {
        free(buf->base);
    }
}

void on_new_connection(uv_stream_t *server, int status) 
{
    if (status < 0) 
    {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        return;
    }

    uv_tcp_t *client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, client);
    if (uv_accept(server, (uv_stream_t*) client) == 0) 
    {
        struct sockaddr client_addr;
        int addrlen = sizeof( struct sockaddr );
        
        int r = uv_tcp_getpeername( client, &client_addr, &addrlen );
        printf( "Connection from %s:%d\n",
                inet_ntoa( ( ( struct sockaddr_in * )&client_addr ) -> sin_addr ), 
                htons( ( ( struct sockaddr_in * )&client_addr ) -> sin_port ) );

        uv_read_start((uv_stream_t*)client, alloc_buffer, echo_read);
    } 
    else 
    {
        uv_close((uv_handle_t*) client, NULL);
    }
}

void timer_cb_once( uv_timer_t *handler )
{
    printf( "Timer 01 expiration!!\n" );
    return;
}

void timer_cb_period( uv_timer_t *handler )
{
    printf( "Timer 02 expiration!!\n" );
    return;
}

int main() 
{
    loop = uv_default_loop();

    uv_tcp_t server;
    uv_tcp_init(loop, &server);

    uv_ip4_addr("0.0.0.0", 8000, &addr);

    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
    int r = uv_listen((uv_stream_t*)&server, 128, on_new_connection);
    if (r) 
    {
        fprintf(stderr, "Listen error %s\n", uv_strerror(r));
        return 1;
    }
    
    uv_timer_t timer_req_once;
    uv_timer_t timer_req_period;

    uv_timer_init( loop, &timer_req_once );
    uv_timer_start( &timer_req_once, timer_cb_once, 5000, 0 );
    uv_timer_init( loop, &timer_req_period );
    uv_timer_start( &timer_req_period, timer_cb_period, 2000, 2000 );
    
    return uv_run(loop, UV_RUN_DEFAULT);
}
