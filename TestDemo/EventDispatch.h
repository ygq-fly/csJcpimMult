#pragma once
#include "stdafx.h"

class EventDispatch
{
public:
	EventDispatch();
	~EventDispatch();

public:
	void AddEvent(SOCKET socketfd, uint8_t event_index);
	void RemoveEvent(SOCKET socketfd, uint8_t event_index);
	void RemoveAllEvent();
	void StartEvent();

	static EventDispatch* Instance() {
		if (NULL == pEventDispatch) {
			pEventDispatch = new EventDispatch;
		}

		return pEventDispatch;
	}

	static void ReleaseInstance() {
		if (NULL != pEventDispatch) {
			delete pEventDispatch;
			pEventDispatch = NULL;
		}
	}

private:
	static EventDispatch* pEventDispatch;

	fd_set m_read_set;
	fd_set m_write_set;
	fd_set m_excep_set;

	std::mutex m_mutex;
};



