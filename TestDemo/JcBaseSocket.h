#pragma once
#include "stdafx.h"

class JcBaseSocket
{
public:
	JcBaseSocket();
	~JcBaseSocket();

public:
	int OnListen(const char* server_ip, uint16_t port);
	int OnWrite();
	int OnRead();

public:
	int getsocket();

private:
	SOCKET m_socket_fd;
	sockaddr_in m_local_ip;

};

typedef std::shared_ptr<JcBaseSocket> PtrSocket;
typedef std::map<SOCKET, PtrSocket> SocketMap;
SocketMap g_socket_map;

void AddSocketMap(PtrSocket pSocket) {
	g_socket_map.insert(std::make_pair(pSocket->getsocket(), pSocket));
}

void RemoveSocketMap(PtrSocket pSocket) {
	g_socket_map.erase(pSocket->getsocket());
}

PtrSocket FindSocketMap(int fd) {
	SocketMap::iterator iter = g_socket_map.find(fd);
	if (iter != g_socket_map.end())
		return iter->second;
	else
		return NULL;
}