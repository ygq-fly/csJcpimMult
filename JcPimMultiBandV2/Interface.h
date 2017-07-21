#ifndef _SAN_INTERFACE_H_
#define _SAN_INTERFACE_H_

#define Interface_ class
#define FInterface_ public
#define Implements_ public

template<class T> 
class Singleton {
public:
	Singleton() {}
	virtual ~Singleton() {}

	static T* Instance(){
		if (NULL == _singleton)
			_singleton = new T;

		return _singleton;
	}

	static void release() {
		if (NULL != _singleton)
			delete _singleton;

		_singleton = NULL;
	}

private:
	static T* _singleton;
};

template<class T>
T* Singleton<T>::_singleton = NULL;

#endif