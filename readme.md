# Send all Serial.print() on ESP32 self hosted webview

<code>git clone https://github.com/GreenPonik/esp32-print-serial-to-webview.git</code>

## #1/ add your ssid and password
![network_setting](/doc/images/network_setting.jpg)

## #2/ run : <code>pio run -t upload</code>

## #3/ then run : <code>pio device monitor</code> to get the esp32's ip address
![get_ip_address_image](/doc/images/get_ip_address.jpg)

## #4/ next open browser paste esp32's ip address like <code>192.168.0.12</code>
![get_ip_address_image](/doc/images/index.jpg)

## #5/ enjoy debug
![get_ip_address_image](/doc/images/get_logs_on_webview.jpg)

#### lib dependencies :
    -ESPAsyncWebServer
    -WiFi
    -SPIFFS

#ESP32remotedebugger #platformio #gpk