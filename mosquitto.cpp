#include <mosquitto.h>

void message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);
void connect_callback(struct mosquitto *mosq, void *userdata, int result);
void subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos);
void log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str);

calss MQTTServer
{
public:	
	
	MQTTServer(char* host, int port, char* user, char* password)
	{
		m_host           	= host;
		m_port                  = port;
		m_user 	                 = user;
		m_password        = password;
		m_clean_session = true;
	}

	int InitServer()
	{
		mosquitto_lib_init();
		m_mosq = mosquitto_new(NULL, m_clean_session, NULL);
		if(!m_mosq)
		{
			fprintf(stderr, "Error: Out of memory.\n");
			return 1;
		}
		mosquitto_log_callback_set(m_mosq, log_callback);
		mosquitto_connect_callback_set(m_mosq, connect_callback);
		mosquitto_message_callback_set(m_mosq, message_callback);
		mosquitto_subscribe_callback_set(m_mosq, subscribe_callback);
		mosquitto_username_pw_set(m_mosq, m_user, m_password);
		return 0;
	}

	int connect()
	{
		if(MOSQ_ERR_SUCCESS != mosquitto_connect(m_mosq, host, port, keepalive))
		{
			fprintf(stderr, "Unable to connect.\n");
			return 1;
		}
	}
private:
	char *	m_host;
	int	m_port;
	int	m_keepalive;
	char*	m_user;
	char*	m_password;
	bool	m_clean_session;
	struct	mosquitto *m_mosq;
};