#ifndef HOSTAPP_H
#define HOSTAPP_H

#include <memory>
#include <string>
#include "IMQTTClient.h"
#include "IStateManager.h"
#include <nlohmann/json.hpp>

namespace sparkplugb {

/**
 * @interface INodeDeviceNotification
 * @brief Interface for handling node and device lifecycle notifications in the Sparkplug system
 * 
 * Provides callback methods for node and device birth/death events, allowing
 * implementations to respond to state changes in the Sparkplug network.
 */
class INodeDeviceNotification {
public:
    virtual ~INodeDeviceNotification() = default;
    
    /**
     * @brief Called when a node comes online and publishes its birth certificate
     * @param nodeId Unique identifier of the node
     * @param message Birth certificate data in JSON format
     */
    virtual void onNodeBirth(const std::string& nodeId, const nlohmann::json& message) = 0;
    
    /**
     * @brief Called when a node goes offline or publishes a death certificate
     * @param nodeId Unique identifier of the node
     * @param message Death certificate data in JSON format
     */
    virtual void onNodeDeath(const std::string& nodeId, const nlohmann::json& message) = 0;
    
    /**
     * @brief Called when a device connected to a node publishes its birth certificate
     * @param nodeId Unique identifier of the parent node
     * @param message Device birth certificate data in JSON format
     */
    virtual void onDeviceBirth(const std::string& nodeId, const nlohmann::json& message) = 0;
};

/**
 * @class HostApp
 * @brief Main application class for the Sparkplug host implementation
 * 
 * Manages the MQTT client connection and state management for the Sparkplug host,
 * coordinating communication between nodes and devices in the system.
 */
class HostApp {
public:
    /**
     * @brief Constructs a new HostApp instance
     * @param mqttClient MQTT client for network communication
     * @param sparkplugHostId Unique identifier for this Sparkplug host
     */
    HostApp(std::shared_ptr<IMQTTClient> mqttClient, const std::string& sparkplugHostId);

    /**
     * @brief Initializes the host application and its components
     */
    void initialize();

    /**
     * @brief Shuts down the host application and its components
     * 
     * This method publishes a death certificate message to the MQTT broker,
     * indicating that the host application is going offline, and then disconnects
     * from the MQTT broker.
     */
    void shutdown();

    /**
     * @brief Publishes the current state of the host application
     * 
     * This method constructs and sends a birth certificate message to the MQTT broker,
     * indicating that the host application is online and ready to communicate.
     */
    void publishState();

    /**
     * @brief Handles the birth of an edge node
     * @param nodeId Unique identifier of the edge node
     * 
     * This method is responsible for processing the birth certificate of an edge node,
     * updating the system state to reflect the node's presence.
     */
    void handleEdgeNodeBirth(const std::string& nodeId);

    /**
     * @brief Handles data received from an edge node
     * @param nodeId Unique identifier of the edge node
     * @param data Data payload received from the node
     * 
     * This method processes incoming data from an edge node, updating the system state
     * or triggering actions based on the data content.
     */
    void handleEdgeNodeData(const std::string& nodeId, const std::string& data);

    /**
     * @brief Handles the death of an edge node
     * @param nodeId Unique identifier of the edge node
     * 
     * This method processes the death certificate of an edge node, updating the system
     * state to reflect the node's absence.
     */
    void handleEdgeNodeDeath(const std::string& nodeId);

    /**
     * @brief Retrieves the current system timestamp
     * @return Current timestamp as a long integer
     * 
     * This method returns the current system time, which can be used for timestamping
     * messages or events within the application.
     */
    long getCurrentTimestamp();

private:
    /**
     * @brief Callback for handling received MQTT messages
     * @param topic Topic of the received message
     * @param payload Payload of the received message
     * 
     * This method is invoked when an MQTT message is received, parsing the payload
     * and triggering appropriate actions based on the message content.
     */
    void onMessageReceived(const std::string& topic, const std::string& payload);

    /**
     * @brief Extracts the node ID from an MQTT topic
     * @param topic MQTT topic string
     * @return Extracted node ID
     * 
     * This utility method parses the MQTT topic to extract the node identifier,
     * which is used for routing messages to the correct node handler.
     */
    std::string extractNodeIdFromTopic(const std::string& topic);

    /**
     * @brief Handles the birth of a node
     * @param nodeId Unique identifier of the node
     * @param message Birth certificate data in JSON format
     * 
     * This method processes the birth certificate of a node, updating the system
     * state and triggering any necessary initialization actions.
     */
    void handleNodeBirth(const std::string& nodeId, const nlohmann::json& message);

    /**
     * @brief Handles the death of a node
     * @param nodeId Unique identifier of the node
     * @param message Death certificate data in JSON format
     * 
     * This method processes the death certificate of a node, updating the system
     * state and performing any necessary cleanup actions.
     */
    void handleNodeDeath(const std::string& nodeId, const nlohmann::json& message);

    /**
     * @brief Handles the birth of a device
     * @param nodeId Unique identifier of the parent node
     * @param message Device birth certificate data in JSON format
     * 
     * This method processes the birth certificate of a device, updating the system
     * state and triggering any necessary initialization actions.
     */
    void handleDeviceBirth(const std::string& nodeId, const nlohmann::json& message);

    /**
     * @brief Creates a state payload for the host application
     * @param online Boolean indicating whether the host is online
     * @return State payload as a JSON string
     * 
     * This utility method constructs a JSON payload representing the current state
     * of the host application, including its online status and timestamp.
     */
    std::string createStatePayload(bool online);

    std::shared_ptr<IMQTTClient> mqttClient_; ///< MQTT client for network communication
    std::shared_ptr<IStateManager> stateManager_; ///< Manager for maintaining system state
    std::string sparkplugHostId_; ///< Unique identifier for this Sparkplug host
    std::shared_ptr<INodeDeviceNotification> notificationHandler_; ///< Handler for node/device notifications
};

} // namespace sparkplugb

#endif // HOSTAPP_H 