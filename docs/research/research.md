# 🔎 Research to market

## Introduction

Atmos transforms real-time weather data into a holographic, audio-visual installation, creating a multi-sensory experience that blends science, technology, and art.

The installation uses a frontend which is served via `nginx` and is displayed on an iPad as part of the Pepper’s Ghost illusion. With this setup, even small delays break the illusion and negatively impact the user experience, which was observed during the user testing conducted during this project.

Real-Time communication is particularly relevant for developers working with embedded devices that need to send state updates directly to another service in real time. Relying on repeated GET requests on both sites is not ideal, since it can be quite resource-intensive for the embedded devices.

**Target group:** Developers working on interactive or responsive installations, especially those using systems where another system (in this case, a weather-based holographic display) must reflect state changes instantly in sync with the embedded device.

Solving this challenge it not only improves system communications, but also opens up new possibilities for creating synchronized, immersive experiences in today’s increasingly connected world.

## Research question

**Area:** Web and Embedded

**Main question:**  
How to enable real-time communication between an embedded device and a web-based frontend, with minimal delay and resources?

**Sub-questions:**

- What protocols and technologies are used for real-time communication?
- How can an ESP32 transmit data to a web service in real time?
- What frontend techniques are required to ensure instant feedback from the embedded system?

## Research method

In order to answer the research questions, a **practice-based experimental approach** was used, supported by **technology landscape analysis**. This method helped to directly test and evaluate the communication between an embedded ESP32 device and the frontend.

During the research one ESP32 device was connected to a backend server using different approaches. The results were observed and analyzed through with the current projects infrastructure.

Response time was tested visually with the installation.

## Research Results

In the early research phase, WebSocket functionality was implemented directly on the ESP32 using the [HTTPS_Server_Generic](https://github.com/khoih-prog/HTTPS_Server_Generic) library _(Khoih-Prog, n.d.)_. This setup allowed HTTPS Communication and WebSocket handling, but it proved unreliable under when used for updating the IPad Display in real-time. The WebSocket Server only allowed a limited amound of connection, the library itself was deprecated and prompted other issues to the infrastructure and during user testing it proved that the delay was just too big for a good user experience.

```cpp
void MyWebSocketHandler::onMessage(WebsocketInputStreambuf *input) {
  // echo logic
  send(msg.c_str(), SEND_TYPE_TEXT);
}
```

Therefore it was clear, that either a hardware upgrade or an architectural change was necessary to meet requirements of the installation.

Since the existing system already uses an NGINX server, further research led to the use of `ngx_http_push_stream_module` _(Wandenberg, n.d.)_. This solution allowed the ESP32 to send data to a single HTTPS endpoint, while NGINX managed WebSocket broadcasting to one or more clients. This reduced the resources used on the ESP32 and allows scalability.

## Implementation

Explore the [Implementation](./research-implementation.md)

## Conclusions

This research explored how real-time communication between an embedded device and a web-based frontend can be achieved with minimal delay and resource usage.

**Main question:**  
How to enable real-time communication between an embedded device and a web-based frontend, with minimal delay and resources?

**Conclusion:**

- Using WebSocket directly on the ESP32 proved unstable and inefficient, especially under load.
- NGINX server using the `ngx_http_push_stream_module` offered a more reliable and scalable solution.

**Sub-questions:**

- **What protocols and technologies are used for real-time communication?**  
  WebSockets is still the standard for real-time web communication. _(WHATWG, 2024; MDN Web Docs, 2024)_. In this project, a reverse proxy with NGINX enables efficient and scalable handling of WebSocket connections.

- **How can an ESP32 transmit data to a web service in real time?**  
  The ESP32 sent HTTPS POST requests to a single endpoint on the NGINX server, which then handled broadcasting via WebSockets.

- **What frontend techniques are required to ensure instant feedback from the embedded system?**  
  A WebSocket client in the frontend subscribed to the NGINX and updates the display as soon as new index arrives.

These conclusions are supported by testing.

## Recommendations

Based on the results of this research, several recommendations can be made for future improvement and exploration:

- **Secure the communication** by restricting WebSocket access to authorized clients to ensure data integrity.
- **Add bidirectional communication** to allow the web client to control things on the embedded device and back.
- **Add scalability** by supporting multiple channels for syncing data across several displays and devices in larger installations.

## Sources and References

Khoih-Prog. (n.d.). _HTTPS_Server_Generic: HTTPS/HTTP Server Library for ESP32, WT32_ETH01, ESP32 + LwIP W5500, W6100, ENC28J60_. GitHub. [https://github.com/khoih-prog/HTTPS_Server_Generic](https://github.com/khoih-prog/HTTPS_Server_Generic)

Wandenberg. (n.d.). _nginx-push-stream-module: A pure stream HTTP push technology for your NGINX setup_. GitHub. [https://github.com/wandenberg/nginx-push-stream-module](https://github.com/wandenberg/nginx-push-stream-module)

MDN Web Docs. (2024, September 25). _WebSocket - Web APIs_. Mozilla. [https://developer.mozilla.org/en-US/docs/Web/API/WebSocket](https://developer.mozilla.org/en-US/docs/Web/API/WebSocket)

WHATWG. (2024, January 24). _WebSockets Standard_. [https://websockets.spec.whatwg.org/](https://websockets.spec.whatwg.org/)
