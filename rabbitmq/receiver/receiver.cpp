#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <iostream>

int main(int argc, char const *const *argv) {
    //std::string queue_name = "hello";
	char const *host_name;
	int port;
	char const *queue_name;
	std::string buffer;
	if (argc < 4){
		fprintf(stderr, "000000 Usage: sender host_name port queue_name message\n");
		return 1;
	}

	host_name = argv[1];
	port = atoi(argv[2]);
	queue_name = argv[3];
	
	AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Create(host_name, 5672, "admin", "admin");
   // AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Create("localhost");

    channel->DeclareQueue(queue_name, false, true, false, false);

    std::string consumer_tag = channel->BasicConsume(queue_name, "");

    while (1) {
        std::cout << "[y] wait for the message" << std::endl;

        AmqpClient::Envelope::ptr_t envelope = channel->BasicConsumeMessage(consumer_tag);

        std::string buffer = envelope->Message()->Body();

        std::cout << "[y] receve " << buffer << std::endl;
    }

    channel->BasicCancel(consumer_tag);
}
