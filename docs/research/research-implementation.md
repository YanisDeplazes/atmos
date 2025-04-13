# 🔎 Research to Market: Implementation

The NGINX server was configured with the following logic:

```
nginx
location = /api/installation/ws {
    push_stream_subscriber websocket;
    push_stream_channels_path "installation";
    push_stream_websocket_allow_publish off;
    push_stream_ping_message_interval 30s;
    push_stream_message_template "~text~";
}

location = /api/installation/publish {
    push_stream_publisher admin;
    push_stream_channels_path "installation";
}
```

The ESP32 acts as a secure HTTPS client and publishes the `currentIndex` to the `/api/installation/publish` endpoint when changed.

```cpp
void sendCurrentIndexUpdate(int currentIndex) {
  String payload = "{\"index\": " + String(currentIndex) + "}";
  String response = httpPOST("/api/installation/publish", payload, "application/json");
}
```

The frontend connects to the `/api/installation/ws` endpoint and updates the carousel when receiving the new `index`.

```js
static async connect() {
  this.socket = new WebSocket(this.URL);
  this.socket.addEventListener("message", (event) => {
    const data = JSON.parse(event.data);
    if (data?.index !== undefined) {
      this.carousel.setActiveIndex(data.index);
    }
  });
}
```

A short demo video demonstrates how the frontend instantly reflects index changes sent by the ESP32, confirming the low-latency performance of the system.

<video src="https://atmos.yanis.io/assets/research/proof.mp4" controls width="600">
  Your browser does not support the video tag.
</video>
