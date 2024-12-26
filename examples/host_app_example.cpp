
#include "HostApp.h"
#include "MQTTClient.h" // Assuming this is the interface for the MQTT client

#include <memory>
#include <iostream>
#include <mqtt/async_client.h>

namespace examples {

class MQTTClient : public sparkplugb::IMQTTClient {
public:
    MQTTClient() : client_("tcp://localhost:1883", "exampleClientId") {
        mqtt::connect_options connOpts;
        mqtt::message willMessage("", "", 0, false);
        connOpts.set_will(willMessage);
        connOpts_ = connOpts;
    }

    void connect() override {
        client_.connect(connOpts_)->wait();
        std::cout << "MQTTClient: Connected to MQTT broker." << std::endl;
    }

    void disconnect() override {
        client_.disconnect()->wait();
        std::cout << "MQTTClient: Disconnected from MQTT broker." << std::endl;
    }

    void setWill(const std::string& topic, const std::string& payload, sparkplugb::QoS qos, bool retain) override {
        mqtt::message willMessage(topic, payload, static_cast<int>(qos), retain);
        connOpts_.set_will(willMessage);
        std::cout << "MQTTClient: Will set for topic " << topic << std::endl;
    }

    void subscribe(const std::string& topic, sparkplugb::QoS qos) override {
        client_.subscribe(topic, static_cast<int>(qos))->wait();
        std::cout << "MQTTClient: Subscribed to topic " << topic << std::endl;
    }

    void publish(const std::string& topic, const std::string& payload, sparkplugb::QoS qos, bool retain) override {
        mqtt::message_ptr pubmsg = mqtt::make_message(topic, payload);
        pubmsg->set_qos(static_cast<int>(qos));
        pubmsg->set_retained(retain);
        client_.publish(pubmsg)->wait();
        std::cout << "MQTTClient: Published to topic " << topic << " with payload: " << payload << std::endl;
    }

private:
    mqtt::async_client client_;
    mqtt::connect_options connOpts_;
};

} // namespace examples

int main() {
    // Create instances of MQTT client and state manager
    auto mqttClient = std::make_shared<examples::MQTTClient>();

    // Create HostApp instance
    sparkplugb::HostApp hostApp(mqttClient, "exampleHostId");

    // Initialize the HostApp
    hostApp.initialize();

    // Simulate some operations
    std::cout << "HostApp initialized and running..." << std::endl;

    // Shutdown the HostApp
    hostApp.shutdown();

    std::cout << "HostApp shutdown completed." << std::endl;

    return 0;
} 