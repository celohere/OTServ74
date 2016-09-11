#ifndef OTSERV_PROTOCOL_H_
#define OTSERV_PROTOCOL_H_

#include <cstdio>
#include <cstring>

#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include "definitions.h"

class NetworkMessage;
class OutputMessage;
class Connection;

typedef boost::shared_ptr<OutputMessage> OutputMessage_ptr;

#if defined(__PROTOCOL_76__)
#define CLIENT_VERSION_MIN 760
#define CLIENT_VERSION_MAX 760
#define STRING_CLIENT_VERSION "This server requires client version 7.6."
#else
#define CLIENT_VERSION_MIN 740
#define CLIENT_VERSION_MAX 740
#define STRING_CLIENT_VERSION "This server requires client version 7.4."
#endif

class Protocol : boost::noncopyable
{
public:
	Protocol(Connection* connection)
	{
		m_connection = connection;
		m_rawMessages = false;
	}

	virtual ~Protocol()
	{
	}

	virtual void parsePacket(NetworkMessage& msg){};

	void onSendMessage(OutputMessage_ptr msg);
	void onRecvMessage(NetworkMessage& msg);
	virtual void onRecvFirstMessage(NetworkMessage& msg) = 0;

	Connection* getConnection()
	{
		return m_connection;
	}
	const Connection* getConnection() const
	{
		return m_connection;
	}
	void setConnection(Connection* connection)
	{
		m_connection = connection;
	}

	uint32_t getIP() const;
	int32_t addRef()
	{
		return ++m_refCount;
	}
	int32_t unRef()
	{
		return --m_refCount;
	}

protected:
	// Use this function for autosend messages only
	OutputMessage_ptr getOutputBuffer();

	void setRawMessages(bool value)
	{
		m_rawMessages = value;
	}

	virtual void releaseProtocol();
	virtual void deleteProtocolTask();
	friend class Connection;

private:
	OutputMessage_ptr m_outputBuffer;
	Connection* m_connection;
	bool m_rawMessages;
	uint32_t m_refCount;
};

#endif
