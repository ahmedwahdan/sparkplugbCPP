cmake_minimum_required(VERSION 3.10)

project(HostAppExample)

add_executable(host_app_example host_app_example.cpp MqttClientImpl.cpp)

find_package(PahoMqttCpp REQUIRED)

# Link against the SparkplugV3Cpp library and Paho MQTT C++
target_link_libraries(host_app_example
    PRIVATE
        sparkplugv3
        PahoMqttCpp::paho-mqttpp3
)