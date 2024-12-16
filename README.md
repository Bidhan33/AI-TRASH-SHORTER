# AI Waste Sorter 🚮✨


## ⚠️The full code for this project is available exclusively upon request via email.⚠️

## What an Amazing Way to End the Semester! 🌟

After four months of hard work at **Haaga-Helia University of Applied Sciences**, we are proud to present **AI Waste Sorter**, an innovative solution for smart and efficient waste management. 🌍✨

### Overview
The **AI Waste Sorter** combines advanced **AI technology**, the **ESP32 microcontroller**, and the **GPT API** to classify waste materials and recommend the appropriate recycling bin. 🧠📷 With a press of a button:

1. The **ESP32** captures an image. 📸
2. The image is sent to our custom-built server via an API. 🔗
3. Within 5-6 seconds, the classification result is displayed on the onboard screen. ⏱️📟

This project showcases our ability to merge hardware, software, and AI seamlessly to tackle real-world problems. 💡💪

---

## Key Features 🚀

- **Custom APIs**: Built from scratch to handle image data and ensure efficient communication between hardware and the AI system. ⚙️📡
- **Hardware Optimization**: Used **Charlieplexing** to control 7 LED indicators with only 5 GPIO ports. 💡🔌
- **AI Integration**: Utilized the **GPT API** to analyze captured images and classify waste accurately. 🧠📊
- **User-Friendly Design**: Clear and concise display of recycling instructions on a screen for ease of use. ✅🖥️

---

## Learning and Achievements 📚✨

### 1. Connecting ESP32 to Wi-Fi 📶
We established a reliable Wi-Fi connection for the ESP32 microcontroller, enabling seamless communication with external APIs. Initially, a mobile hotspot was used for server connectivity. 🌐📡

### 2. Camera Integration 📸
We connected a camera module to the ESP32 and programmed it to capture images upon button press. This involved:
- Configuring I/O pins. 🔧
- Implementing debouncing techniques for the physical button to prevent accidental triggers. 🔘

### 3. Connecting to the GPT API 🧠🔗
To classify the waste:
- We integrated the **OpenAI GPT API** for image analysis.
- Sent image data from the ESP32 to the API.
- Parsed and interpreted the API's responses. 🔍📩

### 4. Displaying Results 📟✨
The classification results from the GPT API were displayed on a screen connected to the ESP32. This included:
- Choosing and linking a suitable display module. 📺
- Formatting the output for clarity and user-friendliness. 📝✅

### 5. Implementing Charlieplexing 💡🔌
Due to GPIO limitations on the ESP32 WROVER module, we implemented **Charlieplexing** to control 7 LEDs using only 5 pins, optimizing our design without additional hardware. 🤓✨

---

## Challenges and Solutions 🛠️🌟
- **GPIO Limitations**: Solved by implementing Charlieplexing for LED control. 💡
- **Wi-Fi Connectivity Issues**: Ensured stable network communication through troubleshooting and optimization. 📶
- **API Integration**: Learned how to process and utilize responses from the GPT API effectively. 🧠📩

---

## Acknowledgments 🙌❤️
This project would not have been possible without the guidance and support of:

- **Heikki Hietala, FM, AmO** 🧑‍🏫✨
- **Mikko Uusitalo** 👨‍🏫✨

A huge thank you to our incredible teammates:

- **Youp Verkooijen** 👨‍💻
- **Biselx Thomas** 👨‍💻
- **Liarte Serra Laura** 👩‍💻

Finally, we extend our gratitude to **Haaga-Helia University of Applied Sciences** for providing the platform and resources to make this project a reality. 🎓🌍

---

## How It Works 🛠️🔍
1. **Capture Image**: The camera module captures an image when the button is pressed. 📸🔘
2. **Send Image to API**: The ESP32 sends the image data to the server via custom-built APIs. 🔗📡
3. **Classify Waste**: The GPT API analyzes the image and classifies the waste. 🧠🔍
4. **Display Results**: The classification result is displayed on the onboard screen, and the corresponding LED indicator lights up. 📟💡

---

## Getting Started 🚀

### Prerequisites 📋
- **ESP32 WROVER module**
- **Camera module** 📸
- **Display module** 📟
- **LEDs for bin indicators** 💡
- **Access to OpenAI GPT API** 🧠

### Installation 🛠️
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/ai-waste-sorter.git
   ```
2. Configure the Wi-Fi and API credentials in the ESP32 firmware. 📄🔐
3. Install the necessary dependencies for the server. 📦

### Usage 🎮
1. Power up the ESP32 and connect to Wi-Fi. 📶
2. Press the button to capture an image. 🔘📸
3. View the classification result on the display and follow the recycling instructions. 📟✅

---

## Future Improvements 🌟
- Enhance image classification accuracy with more advanced AI models. 🧠📈
- Expand hardware support for additional GPIO ports. 🔧🔌
- Develop a mobile app for real-time waste classification updates. 📱✨

---

## Feedback 💬
We’d love to hear your thoughts and suggestions about our prototype! Together, let’s contribute to a cleaner and greener future. 🌍♻️




## Pictures 📷


![Media (5)](https://github.com/user-attachments/assets/d26ac52b-afe6-488b-a42b-387ad2a64f08)

![Media (6)](https://github.com/user-attachments/assets/3d279cfe-6b23-4cc6-83d8-65a1e11dc1a8)

![Media (7)](https://github.com/user-attachments/assets/1e565dfc-b7c4-4b7b-a5bf-c824a5e0d303)

![Media (8)](https://github.com/user-attachments/assets/d5e4330b-1456-42c0-90c6-3f0b12328c96)

![Media (9)](https://github.com/user-attachments/assets/60f0199b-9044-4a3a-955f-b06e27243ff1)

![Media (11)](https://github.com/user-attachments/assets/c00571d3-a576-4811-9ba5-8a8dbf36c106)

![Media (10)](https://github.com/user-attachments/assets/08c5ee1a-8b83-493d-8588-57f4aab5fd76)
![Media (12)](https://github.com/user-attachments/assets/afc10cf2-a514-46f4-9f6a-3631cf6093c3)

