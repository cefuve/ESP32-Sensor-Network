/*
    ESP32 Receiver
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

// Define an struct to save received data of sensor
typedef struct struct_message {
    int id;
    float x;
    float y;
} struct_message;

// Initialize an struct called getData
struct_message getData;

// Add a peerInfo to peer list.
esp_now_peer_info_t peerInfo;

// Function triggered on data received and print the data
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    memcpy(&getData, incomingData, sizeof(getData));
    Serial.println("<---");
    Serial.println("Data received:");
    Serial.print("id: ");
    Serial.println(getData.id);
    Serial.print("x: ");
    Serial.println(getData.x);
    Serial.print("y: ");
    Serial.println(getData.y);
    Serial.println();
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
    
    // Register callback function of receiving ESPNOW data.
    esp_now_register_recv_cb(OnDataRecv);
}


void loop() {
    // put your main code here, to run repeatedly:
}
