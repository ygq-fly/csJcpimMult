#include "EventDispatch.h"

EventDispatch* EventDispatch::pEventDispatch = NULL;

EventDispatch::EventDispatch()
{
	RemoveAllEvent();
}

EventDispatch::~EventDispatch()
{
	RemoveAllEvent();
}

void EventDispatch::StartEvent() {
	fd_set read_set, write_set, excep_set;

	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 100 * 1000;

	while (1) {

		if (!m_read_set.fd_count && !m_write_set.fd_count && !m_excep_set.fd_count) {
			Sleep(100);
			continue;
		}

		m_mutex.lock();
		memcpy(&read_set, &m_read_set, sizeof(fd_set));
		memcpy(&write_set, &m_write_set, sizeof(fd_set));
		memcpy(&excep_set, &m_excep_set, sizeof(fd_set));
		m_mutex.unlock();

		int status = select(0, &read_set, &write_set, &excep_set, &timeout);
		if (status < 0) {
			printf("select error!");
			continue;
		}

		if (0 == status) {
			continue;
		}

		for (size_t i = 0; i < read_set.fd_count; ++i) {

		}

		for (size_t i = 0; i < write_set.fd_count; ++i) {

		}

	}
}

void EventDispatch::AddEvent(SOCKET socket_fd, uint8_t event_index) {
	std::unique_lock<std::mutex> lock(m_mutex);

	if (0 != (event_index & 0x01)) {
		FD_SET(socket_fd, &m_read_set);
	}

	if (0 != (event_index & 0x02)) {
		FD_SET(socket_fd, &m_write_set);
	}

	if (0 != (event_index & 0x04)) {
		FD_SET(socket_fd, &m_excep_set);
	}
}

void EventDispatch::RemoveEvent(SOCKET socket_fd, uint8_t event_index) {
	std::unique_lock<std::mutex> lock(m_mutex);

	if (0 != (event_index & 0x01)) {
		FD_CLR(socket_fd, &m_read_set);
	}

	if (0 != (event_index & 0x02)) {
		FD_CLR(socket_fd, &m_write_set);
	}

	if (0 != (event_index & 0x04)) {
		FD_CLR(socket_fd, &m_excep_set);
	}
}

void EventDispatch::RemoveAllEvent() {
	FD_ZERO(&m_read_set);
	FD_ZERO(&m_write_set);
	FD_ZERO(&m_excep_set);
}


