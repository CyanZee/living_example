#include <iostream>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

using namespace std;

int main(int argc, char const *const *argv)
{
	char const *host_name;
	int port;
	char const *queue_name;
	char const *message;

	if (argc < 5){
		fprintf(stderr, "000000 Usage: sender host_name port queue_name message\n");
		return 1;
	}
	
	host_name = argv[1];
	port = atoi(argv[2]);
	queue_name = argv[3];
	message = argv[4];

	AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Create(host_name, 5672, "admin", "admin");
	channel->DeclareQueue(queue_name,false, true, false, false);

	channel->BasicPublish("", queue_name, AmqpClient::BasicMessage::Create(message));
	cout<< "+++ send the message body is: " << message << endl;
	return 0;
}
