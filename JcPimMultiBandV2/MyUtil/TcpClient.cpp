//
// Created by San on 15/12/27.
//

#ifdef _WIN32
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#endif
#include "TcpClient.h"

TcpClient::TcpClient()
    : m_isConnected(false)
{
#ifdef _WIN32
	WORD wVersionRequested = 0;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(2, 2);
	int err = WSAStartup(wVersionRequested, &wsaData);
#endif
}

TcpClient::~TcpClient() {
#ifdef _WIN32
	WSACleanup();
#endif
}

int TcpClient::init_client(std::string strAddr, int port) {
    m_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_fd == -1)
        return false;

    m_address = strAddr;
    m_port = port;
    sockaddr_in addr_server;
    memset(&addr_server, 0, sizeof(sockaddr_in));
    addr_server.sin_family = AF_INET;
    addr_server.sin_port = htons(m_port);
    addr_server.sin_addr.s_addr = inet_addr(m_address.c_str());
    int ret = connect(m_fd, (sockaddr*)&addr_server, sizeof(addr_server));
    if (ret < 0) {
        m_isConnected = false;
        return -1;
    }

    m_isConnected = true;
    return 0;
}

int TcpClient::close_client() {
    if(m_isConnected)
#ifdef _WIN32
		return closesocket(m_fd);
#else
		return close(m_fd);
#endif  
    else
        return 0;
}

int TcpClient::write_data(const char *buf, int count) {
    int ret = -1;
    if(count == -1)
        count = std::strlen(buf);
    if(m_isConnected)
        ret = send(m_fd, buf, count, 0);

    return ret;
}

int TcpClient::read_data(char *buf, int size) {
    int ret = -1;
    if(m_isConnected)
        ret = recv(m_fd, buf, size, 0);
    return ret;
}

bool TcpClient::state() const {
    return m_isConnected;
}


bool TcpClient::wait_for_ready_read(int time) {
    u_long reach_count = 0;
    for (int i = 0; i <= (time / 100); ++i) {
#ifdef _WIN32
		Sleep(100);
		int ret = ioctlsocket(m_fd, FIONREAD, &reach_count);
#else
        struct timespec ts = { 0, 100 * 1000 * 1000 };
        nanosleep(&ts, NULL);
		int ret = ioctl(m_fd, FIONREAD, &reach_count);
#endif
        if (ret == -1)
            break;
        else if(reach_count > 0)
            return true;
    }
    return false;
}

bool TcpClient::write_and_read(const char* wBuf,int wCount, char* rBuf, int rCount) {
    write_data(wBuf,wCount);

    bool ret = wait_for_ready_read(5000);
    if(ret) {
        read_data(rBuf, rCount);
    }
    return ret;
}
