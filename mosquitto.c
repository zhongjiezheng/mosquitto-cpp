#include <stdio.h>
#include <mosquitto.h>

void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
	if(message->payloadlen){
		printf("%s %s\n", message->topic, (const char*)message->payload);
	}else{
		printf("%s (null)\n", message->topic);
	}
	fflush(stdout);
}

void my_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
	int i;
	if(!result){
		/* Subscribe to broker information topics on successful connect. */
		mosquitto_subscribe(mosq, NULL, "$SYS/#", 2);
	}else{
		fprintf(stderr, "Connect failed\n");
	}
}

void my_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos)
{
	int i;

	printf("Subscribed (mid: %d): %d", mid, granted_qos[0]);
	for(i=1; i<qos_count; i++){
		printf(", %d", granted_qos[i]);
	}
	printf("\n");
}

void my_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str)
{
	/* Pring all log messages regardless of level. */
	printf("%s\n", str);
}

int main(int argc, char *argv[])
{
	int i;
	char *host = "mqtt.dms.aodianyun.com";
	//char *host = "localhost";
	int port = 1883;
	int keepalive = 20;
	char *user = "pub_39347cd29e6ed4503b0e09017d259ae3";
	char *password = "sub_4685229aeb38e8b9e5bb92f9a994ba90";
	bool clean_session = true;
	struct mosquitto *mosq = NULL;

	mosquitto_lib_init();
	mosq = mosquitto_new(NULL, clean_session, NULL);
	if(!mosq){
		fprintf(stderr, "Error: Out of memory.\n");
		return 1;
	}
	mosquitto_log_callback_set(mosq, my_log_callback);
	mosquitto_connect_callback_set(mosq, my_connect_callback);
	mosquitto_message_callback_set(mosq, my_message_callback);
	mosquitto_subscribe_callback_set(mosq, my_subscribe_callback);
	mosquitto_username_pw_set(mosq, user, password);
	if(MOSQ_ERR_SUCCESS != mosquitto_connect(mosq, host, port, keepalive)){
		fprintf(stderr, "Unable to connect.\n");
		return 1;
	}
	int mid;
	mosquitto_subscribe(mosq, &mid, "KTV", 1);
	while(!mosquitto_loop(mosq, -1, 1)){
	}
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
	return 0;
}