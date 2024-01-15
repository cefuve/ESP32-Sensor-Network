/*
    ESP32 Transmitter
    Copyright (C) 2024 César Fuenzalida Vergara (http://www.cefuve.com/)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <esp_now.h>
#include <WiFi.h>

// ID from this board
#define nodeID 2

// Replace this MAC with the receiver MAC
uint8_t receiverMAC[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

unsigned long time_stamp = 0;

// Define an struct to save received data of sensor
typedef struct struct_message {
    int id;
    float x;
    float y;
} struct_message;

// Initialize an struct called sendData
struct_message sendData;

// Add a peerInfo to peer list.
esp_now_peer_info_t peerInfo;

// Function triggered on data sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("\r\nData struct send status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "OK" : "FAIL");
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);

    // Get this board MAC address
    Serial.print("This board MAC Address: ");
    Serial.println(WiFi.macAddress());
    
    // Initialize ESPNOW function.
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    
    // Register callback function of sending ESPNOW data.
    esp_now_register_send_cb(OnDataSent);
    
    // Configure the receiver MAC
    memcpy(peerInfo.peer_addr, receiverMAC, 6);
    // Configure the channel
    peerInfo.channel = 0;
    // Configure the encrypt mode
    peerInfo.encrypt = false;
    
    // Add the device to the paired device list
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
}


void loop() {
    // Send data each 2 seconds
    if (millis() - time_stamp > 2000){
        time_stamp = millis();

        // Assign data to the struct variables
        sendData.id = nodeID;
        sendData.x = random(1, 500) / 10.0;
        sendData.y = random(1, 500) / 10.0;

        // Print struct data
        Serial.println("--->");
        Serial.println("Data sended:");
        Serial.print("id: ");
        Serial.println(sendData.id);
        Serial.print("x: ");
        Serial.println(sendData.x);
        Serial.print("y: ");
        Serial.println(sendData.y);
        Serial.println();

        // Send data struct to the receiver board
        esp_err_t result = esp_now_send(receiverMAC, (uint8_t *) &sendData, sizeof(sendData));
        
        // Verify sending
        if (result == ESP_OK) {
            Serial.println("Sent OK");
        } else {
            Serial.println("Sent FAIL");
        }
    }
}
