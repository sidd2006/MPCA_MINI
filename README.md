# MPCA_MINI

The Assistive Vest for the Visually Impaired is a wearable device built around an Arduino Uno that helps visually impaired users navigate their surroundings safely. It uses multiple sensors working together to detect environmental hazards and alert the user through audio and visual feedback.
The MPU6050 accelerometer detects falls by monitoring sudden changes in acceleration. When a fall is detected, the RGB LED flashes red and yellow alternately as a distress signal. The RGB LED also provides combined visual status across all sensors simultaneously: red for IR, green for ultrasonic, and blue for temperature alerts.
The HC-SR04 ultrasonic sensor continuously measures distance to obstacles ahead in three stages: a slow beep at 100cm, a medium beep at 50cm, and a continuous high-pitched tone at 20cm. The IR sensor detects surface-level changes like steps or edges and triggers a rapid beep. 
The DHT11 temperature sensor alerts the user with a distinct tone when temperature exceeds 30 degrees Celsius.
