all:
	gcc tcp_server_v1.c -o tcp_server_v1
	gcc tcp_server_select.c -o tcp_server_select
	gcc tcp_server_epoll.c -o tcp_server_epoll
	gcc tcp_server_epoll_timerfd.c -o tcp_server_epoll_timerfd
	gcc tcp_server_libuv.c -o tcp_server_libuv -luv
	gcc tcp_client.c -o tcp_client
	gcc tls_server.c -o tls_server -lssl -lcrypto
	gcc tls_client.c -o tls_client -lssl -lcrypto
	gcc udp_server.c -o udp_server
	gcc udp_client.c -o udp_client
	gcc udp_client_connect.c -o udp_client_connect
	gcc tun_exp.c -o tun_exp
	
	gcc thread_example.c -o thread_example -lpthread
clean:
	rm tcp_server_v1 tcp_client udp_server udp_client
	rm tcp_server_select
	rm tcp_server_epoll
	rm tcp_server_epoll_timerfd
	rm tls_server tls_client
	rm thread_example
	rm tcp_server_libuv
	rm udp_client_connect
	rm tun_exp
	rm *~