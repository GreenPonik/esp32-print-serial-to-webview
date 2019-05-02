# Send all Serial.print() on ESP32 self web view

`git clone https://github.com/GreenPonik/esp32-print-serial-to-webview.git`

### add your ssid and password
![network_setting](/doc/images/network_setting.jpg)

### run : <code>pio run -t upload</code>
### then run : <code>pio device monitor</code> to get the esp32's ip address
![get_ip_address_image](/doc/images/get_ip_address.jpg)
## next open browser paste esp32's ip address like <code>192.168.0.12</code>
![get_ip_address_image](/doc/images/index.jpg)

## enjoy debug
![get_ip_address_image](/doc/images/get_logs_on_webview.jpg)
### lib dependencies :
    -ESPAsyncWebServer
    -WiFi
    -SPIFFS

#ESP32remotedebugger #platformio