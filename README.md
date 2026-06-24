# Hamownia 2026

## W tym repozytorium znajduje się oprogramowanie dla systemów wbudowanych hamowni. Projekt jest kompatybilny ze sterownikami implementującymi bibliotekę libVescCan.

**Projekt wykonany dla:**
Team Raptors PL

---

# System Komunikacji

## 🧭 Architektura systemu

```
        ESP-NOW                     UART
+--------+      ~~~~~~~~      +--------+      +--------+
| ESP1   | ------------->    | ESP2   | <--> | STM32  |
|        |                   |        |      |        |
+--------+                   +--------+      +--------+
```


## 📌 Opis systemu

System komunikacji składa się z trzech głównych komponentów:

* **ESP C (czujnik)** – urządzenie pomiarowe / źródło danych
* **ESP B (baza)** – gateway komunikacyjny (ESP-NOW ↔ UART), Host webservera sterującego
* **STM32** – sterownik główny (logika sterowania, regulacja)

### Kierunki komunikacji

* ESP C ⇄ ESP B (ESP-NOW)
* ESP B ⇄ STM32 (UART)



---

## 📦 Format ramki

Wszystkie dane przesyłane w systemie używają wspólnego formatu:

| Pole     | Rozmiar (B) | Opis               |
| -------- | ----------- | ------------------ |
| SOF      | 1           | Start ramki (0xAA) |
| Msg type | 1           | Typ wiadomości     |
| Payload  | N           | Dane               |

### Przykład ramki

```
AA 01 10 20
```

---

## 🔢 Typy wiadomości

# Protokół komunikacyjny

## Zakresy identyfikatorów

| Zakres ID | Moduł             |
| --------- | ----------------- |
| 0x0X      | Płytka czujnikowa |
| 0x4X      | Silnik hamujący   |
| 0x8X      | Silnik badany     |
| 0xFF      | Konfiguracja      |

---

## Wiadomości protokołu

| ID   | Nazwa                            | Rozmiar (B) | Skala             | Opis                                | Kierunek                          |
| ---- | -------------------------------- | ----------- | ----------------- | ----------------------------------- | --------------------------------- |
| 0x01 | FEEDBACK_SENSOR_TORQUE           | 2           | 100 = 1 Nm        | Moment hamowania (PID / wykresy)    | ESP C -> ESP B -> (Web & STM32)   |
| 0x02 | FEEDBACK_SENSOR_BATTERY_DATA     | 1           | 255 = 5 V         | Poziom baterii                      | ESP C -> ESP B -> Web             |
| 0x05 | FEEDBACK_SENSOR_MCU_TEMPERATURE  | 1           | 1 = 1 °C          | Temperatura MCU płytki czujnikowej  | ESP C -> ESP B -> Web             |
| 0x21 | SET_SENSOR_MCU_TORQUE_RESET      | 0           | ----              | Reset odczytu momentu na czujniku   | ESP C -> ESP B -> Web             |
| 0x41 | FEEDBACK_BRAKE_MOTOR_CURRENT     | 2           | 100 = 1 A         | Prąd silnika hamującego             | VESC -> STM32 -> ESP B -> Web     |
| 0x42 | FEEDBACK_BRAKE_MOTOR_VOLTAGE     | 2           | 100 = 1 V         | Napięcie silnika hamującego         | VESC -> STM32 -> ESP B -> Web     |
| 0x43 | FEEDBACK_BRAKE_MOTOR_POWER       | 2           | 10 = 1 W          | Moc silnika hamującego              | VESC -> STM32 -> ESP B -> Web     |
| 0x44 | FEEDBACK_BRAKE_MOTOR_SPEED       | 2           | 1 = 1 rpm         | Prędkość silnika hamującego         | VESC -> STM32 -> ESP B -> Web     |
| 0x45 | FEEDBACK_BRAKE_MOTOR_TEMPERATURE | 1           | 1 = 1 °C          | Temperatura silnika hamującego      | VESC -> STM32 -> ESP B -> Web     |
| 0x48 | FEEDBACK_BRAKE_MOTOR_POSITION    | 2           | 100 = 1°          | Pozycja wału 0-360°                 | VESC -> STM32 -> ESP B -> Web     |
| 0x61 | SET_BRAKE_MOTOR_BRAKE_CURRENT    | 2           | 100 = 1 A         | Ustawienie prądu hamowania          | Web -> ESP B -> STM32 -> VESC     |
| 0x62 | SET_BRAKE_MOTOR_BRAKE_TORQUE     | 2           | 100 = 1 Nm        | Ustawienie momentu hamowania (PID)  | Web -> ESP B -> STM32 -> VESC     |
| 0x63 | SET_BRAKE_MOTOR_DRIVE_CURRENT    | 2           | 10 = 1 A          | Ustawienie prądu hamowania          | Web -> ESP B -> STM32 -> VESC     |
| 0x64 | SET_BRAKE_MOTOR_SPEED            | 2           | 1 = 1 rpm         | Ustawienie prędkości silnika        | Web -> ESP B -> STM32 -> VESC     |
| 0x65 | SET_BRAKE_MOTOR_DUTY             | 1           | 1 = 1%            | Ustawienie wypełnienia sterowania   | Web -> ESP B -> STM32 -> VESC     |
| 0x81 | FEEDBACK_TEST_MOTOR_CURRENT      | 2           | 100 = 1 A         | Prąd silnika badanego               | DRV/Base -> STM32 -> ESP B -> Web |
| 0x82 | FEEDBACK_TEST_MOTOR_VOLTAGE      | 2           | 100 = 1 V         | Napięcie silnika badanego           | DRV/Base -> STM32 -> ESP B -> Web |
| 0x83 | FEEDBACK_TEST_MOTOR_POWER        | 2           | 10 = 1 W          | Moc silnika badanego                | DRV/Base -> STM32 -> ESP B -> Web |
| 0x84 | FEEDBACK_TEST_MOTOR_SPEED        | 2           | 1 = 1 rpm         | Prędkość silnika badanego           | DRV/Base -> STM32 -> ESP B -> Web |
| 0x85 | FEEDBACK_TEST_MOTOR_TEMPERATURE  | 1           | 1 = 1 °C          | Temperatura silnika badanego        | DRV/Base -> STM32 -> ESP B -> Web |
| 0xA1 | SET_TEST_MOTOR_BRAKE_CURRENT     | 2           | 100 = 1 A         | Ustawienie prądu hamowania          | Web -> ESP B -> STM32 -> DRV      |
| 0xA2 | SET_TEST_MOTOR_BRAKE_TORQUE      | 2           | 100 = 1 Nm        | Ustawienie momentu hamowania (PID)  | Web -> ESP B -> STM32 -> DRV      |
| 0xA3 | SET_TEST_MOTOR_DRIVE_CURRENT     | 2           | 10 = 1 A          | Ustawienie prądu hamowania          | Web -> ESP B -> STM32 -> DRV      |
| 0xA4 | SET_TEST_MOTOR_SPEED             | 2           | 1 = 1 rpm         | Ustawienie prędkości silnika        | Web -> ESP B -> STM32 -> DRV      |
| 0x65 | SET_TEST_MOTOR_DUTY              | 1           | 1 = 1%            | Ustawienie wypełnienia sterowania   | Web -> ESP B -> STM32 -> DRV      |
| 0xC1 | GENERAL_WEB_STOP                 | 1           | web_estop_enum    | Zatrzymanie awaryjne przez web      | Web -> ESP B -> STM32             |
| 0xC2 | GENERAL_BASE_STOP                | 1           | base_estop_enum   | Zatrzymanie awaryjne przez baze     | Base -> ESP B -> Web              |
| 0xE0 | NEW_CAN_ID                       | 1           | np. 0x70          | Nieobsłużone CANID na szynie        | STM32 -> ESP B -> Web             |
| 0xF0 | CONFIG_TEST_MOTOR_FRAMES         | 2           | motor_frames_enum | Ramki które obsługuje silnik badany | Web -> ESP B -> STM32             |
| 0xF1 | CONFIG_TEST_MOTOR_CAN_ID         | 1           | np. 0x70          | Adres CAN silnika badanego          | Web -> ESP B -> STM32             |





