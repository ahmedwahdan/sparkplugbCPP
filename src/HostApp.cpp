#include "HostApp.h"

#include <chrono>
#include <nlohmann/json.hpp>
#include "sparkplug_b.pb.h"

namespace sparkplugb {

HostApp::HostApp(std::shared_ptr<IMQTTClient> mqttClient, const std::string& sparkplugHostId)
    : mqttClient_(mqttClient), sparkplugHostId_(sparkplugHostId) {
    // Set up Will message for Death Certificate
    std::string deathTopic = "spBv1.0/STATE/" + sparkplugHostId_;
    auto deathStatePayload = createStatePayload(false);

    mqttClient_->setWill(deathTopic, deathStatePayload, QoS::AtLeastOnce, true);
}

long HostApp::getCurrentTimestamp() {
    return std::chrono::system_clock::now().time_since_epoch().count();
}

std::string HostApp::createStatePayload(bool online) {
    nlohmann::json state;
    state["online"] = online;
    state["timestamp"] = std::chrono::system_clock::now().time_since_epoch().count();
    return state.dump();
}

void HostApp::initialize() {
    mqttClient_->connect();
    
    // Subscribe to own Birth Certificate Topic
    std::string birthTopic = "spBv1.0/STATE/" + sparkplugHostId_;
    mqttClient_->subscribe(birthTopic, QoS::AtLeastOnce); // QoS 1
    
    // Subscribe to more generic Node Birth, Death, and Device Birth Topics
    mqttClient_->subscribe("spBv1.0/+/+/NBIRTH/#", QoS::AtLeastOnce); // Generic subscription for node birth
    mqttClient_->subscribe("spBv1.0/+/+/NDEATH/#", QoS::AtLeastOnce); // Generic subscription for node death
    mqttClient_->subscribe("spBv1.0/+/+/DBIRTH/#", QoS::AtLeastOnce); // Generic subscription for device birth
    
    // Publish Birth Certificate after subscribing
    publishState();
    
    // Additional initialization logic...
}

void HostApp::shutdown() {
    // Publish Death Certificate on clean disconnect
    std::string deathTopic = "spBv1.0/STATE/" + sparkplugHostId_;
    auto deathStatePayload = createStatePayload(false);
    mqttClient_->publish(deathTopic, deathStatePayload, QoS::AtLeastOnce, true);
    
    mqttClient_->disconnect();
}

void HostApp::publishState() {
    std::string birthTopic = "spBv1.0/STATE/" + sparkplugHostId_;
    long timestamp = getCurrentTimestamp();
    nlohmann::json birthPayload = {
        {"online", true},
        {"timestamp", timestamp}
    };
    mqttClient_->publish(birthTopic, birthPayload.dump(), QoS::AtLeastOnce, true); // QoS 1, Retain true
}

void HostApp::handleEdgeNodeBirth(const std::string& nodeId) {
    // Method removed as HostApp should not publish on behalf of nodes
}

void HostApp::handleEdgeNodeData(const std::string& nodeId, const std::string& data) {
    // Method removed as HostApp should not publish on behalf of nodes
}

void HostApp::handleEdgeNodeDeath(const std::string& nodeId) {
    // Method removed as HostApp should not publish on behalf of nodes
}

void HostApp::onMessageReceived(const std::string& topic, const std::string& payload) {
    // Parse JSON payload
    nlohmann::json message = nlohmann::json::parse(payload);
    
    // Extract node and device information from topic
    std::string nodeId = extractNodeIdFromTopic(topic);
    
    // Process the message
    if (topic.find("NBIRTH") != std::string::npos) {
        handleNodeBirth(nodeId, message);
    } else if (topic.find("NDEATH") != std::string::npos) {
        handleNodeDeath(nodeId, message);
    } else if (topic.find("DBIRTH") != std::string::npos) {
        handleDeviceBirth(nodeId, message);
    }
}

std::string HostApp::extractNodeIdFromTopic(const std::string& topic) {
    // Logic to extract node ID from topic
    // Example: spBv1.0/Sparkplug B Devices/NBIRTH/Raspberry Pi
    // Extract "Raspberry Pi" as node ID
    size_t lastSlash = topic.find_last_of('/');
    return topic.substr(lastSlash + 1);
}

void HostApp::handleNodeBirth(const std::string& nodeId, const nlohmann::json& message) {
    // Handle node birth message
    // Example: process metrics and relate devices to nodes
}

void HostApp::handleNodeDeath(const std::string& nodeId, const nlohmann::json& message) {
    // Handle node death message
    // Example: update state or clean up resources
}

void HostApp::handleDeviceBirth(const std::string& nodeId, const nlohmann::json& message) {
    // Handle device birth message
    // Example: process metrics and relate devices to nodes
}

} // namespace sparkplugb 