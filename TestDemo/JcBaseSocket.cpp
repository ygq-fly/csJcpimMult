#include "JcBaseSocket.h"
#include "EventDispatch.h"

JcBaseSocket::JcBaseSocket()
{
}


JcBaseSocket::~JcBaseSocket()
{
}

int JcBaseSocket::getsocket() {
	return m_socket_fd;
}

int JcBaseSocket::OnListen(const char* server_ip, uint16_t server_port){
	m_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == m_socket_fd) {
		printf("socket Error: %d", WSAGetLastError());
		return -1;
	}

	memset(&m_local_ip, 0, sizeof(m_local_ip));
	m_local_ip.sin_family = AF_INET;
	m_local_ip.sin_addr.s_addr = inet_addr(server_ip);
	m_local_ip.sin_port = htons(server_port);


	//ÉèÖÃ·Ç×èÈû
	u_long nonblock = 1;
	int ret = ioctlsocket(m_socket_fd, FIONBIO, &nonblock);
	if (-1 == ret) {
		printf("set nonblock Error: %d", WSAGetLastError());
		return -1;
	}

	BOOL nodelay = TRUE;
	ret = setsockopt(m_socket_fd, IPPROTO_TCP, TCP_NODELAY, (char*)&nodelay, sizeof(nodelay));
	if (-1 == ret) {
		printf("set nodelay Error: %d", WSAGetLastError());
		return -1;
	}

	ret = bind(m_socket_fd, (sockaddr *)&m_local_ip, sizeof(m_local_ip));
	if (-1 == ret) {
		printf("bind Error: %d", WSAGetLastError());
		return -1;
	}

	ret = listen(m_socket_fd, 16);
	if (-1 == ret) {
		printf("bind Error: %d", WSAGetLastError());
		return -1;
	}

	EventDispatch::Instance()->AddEvent(m_socket_fd, 0x01 | 0x04);

	return 0;
}

int JcBaseSocket::OnWrite() {

	return 0;
}

int JcBaseSocket::OnRead() {

	return 0;
}


