//
// Created by San on 15/12/27.
//

#ifndef _SAN_TCPCLIENT_H
#define _SAN_TCPCLIENT_H

#include "../stdafx.h"

class TcpClient
{
public:
    TcpClient();
    virtual ~TcpClient();

public:
    int init_client(std::string strAddr, int port);
    int close_client();
    int write_data(const char *buf, int count);
    int read_data(char *buf, int size);
    bool write_and_read(const char* wBuf,int wCount, char* rBuf, int rCount);
    bool state() const;

private:
    bool wait_for_ready_read(int time);

private:
    std::string m_address;
    int m_port;
    int m_fd;
    bool m_isConnected;
};


#endif //X_PARAM_VISACLIENT_H
