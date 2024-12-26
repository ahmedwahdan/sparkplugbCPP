#ifndef IMQTTCLIENT_H
#define IMQTTCLIENT_H

#include <string>

namespace sparkplugb {

/**
 * @file IMQTTClient.h
 * @brief Interface defining core MQTT client functionality
 */

/**
 * @enum QoS
 * @brief Quality of Service levels for MQTT message delivery
 */
enum class QoS {
    AtMostOnce = 0,    ///< Fire and forget - message may be lost
    AtLeastOnce = 1,   ///< Guaranteed to be delivered at least one time
    ExactlyOnce = 2    ///< Guaranteed to be delivered exactly one time
};

/**
 * @class IMQTTClient
 * @brief Interface for MQTT client implementations providing core MQTT operations
 */
class IMQTTClient {
public:
    virtual ~IMQTTClient() = default;
    
    /**
     * @brief Establishes connection with MQTT broker
     */
    virtual void connect() = 0;
    
    /**
     * @brief Subscribes to specified MQTT topic
     * @param topic Topic to subscribe to
     * @param qos Quality of Service level for subscription
     */
    virtual void subscribe(const std::string& topic, QoS qos) = 0;
    
    /**
     * @brief Publishes message to specified MQTT topic
     * @param topic Topic to publish to
     * @param message Message content to publish
     * @param qos Quality of Service level for publication
     * @param retain Whether message should be retained by broker
     */
    virtual void publish(const std::string& topic, const std::string& message, QoS qos, bool retain) = 0;
    
    /**
     * @brief Sets the Last Will and Testament message
     * @param topic Topic for the LWT message
     * @param message Content of LWT message
     * @param qos Quality of Service level for LWT
     * @param retain Whether LWT message should be retained
     */
    virtual void setWill(const std::string& topic, const std::string& message, QoS qos, bool retain) = 0;
    
    /**
     * @brief Disconnects from MQTT broker
     */
    virtual void disconnect() = 0;
};

} // namespace sparkplugb

#endif // IMQTTCLIENT_H 