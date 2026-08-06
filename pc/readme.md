# Software components on the PC


## Serial hub

Der Serial Hub verbindet den PC über einen ESP32 mit dem ESP-NOW basierten Netzwerk des Roboters.
In der finalen Ausbaustufe soll es möglich sein, neue Sensoren und Aktuatoren zu registrieren. 

## Sound Server

Der Sound Server erzeugt die nicht-sprachlichen Geräusche des Systems. Aktuell als python system umgesetzt kann in einer späteren Stufe der Sound auch über einen esp32 generiert werden.

## USBCAM VIEW

Hier erfolgt die Verarbeitung der Bilder von den USB cams. Insbesondere soll über yolo11n eine Personendetection umgesetzt werden. Gleichzeitig sollen die Bilder bei Bedarf auf dem Bilder-Backend gespeichert werden. 

