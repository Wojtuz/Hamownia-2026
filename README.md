# Hamownia 2026

## W tym repozytorium znajduje się oprogramowanie dla systemów wbudowanych hamowni. Projekt jest kompatybilny ze sterownikami implementującymi bibliotekę libVescCan.

---

**Projekt wykonany dla:**
[Team Raptors PL]


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

| ID   | Nazwa                             | Rozmiar (B) | Skala/Jednostki | Opis                                   | Kierunek                              |
| ---- | --------------------------------- | ----------- | --------------- | -------------------------------------- | ------------------------------------- |
| 0x01 | FEEDBACK_SENSOR_TORQUE            | 2           | 100 = 1Nm       | Moment hamowania (Do wykresów i PID)   | ESP C -> ESP B -> (Webserver & STM32) |
| 0x02 | FEEDBACK_SENSOR_BATTERY_DATA      | 1           | 255 = 5V        | Status komunikacji                     | ESP C -> ESP B -> Webserver           |
| 0x03 | FEEDBACK_SENSOR_MOTOR_TEMPERATURE | 1           | 1 = 1 st C      | Temaratura silnika (PT100 na czujniku) | ESP C -> ESP B -> Webserver           |
| 0x04 | FEEDBACK_SENSOR_MOTOR_SPEED_DATA  | 2           | 1 = 1 rpm       | Predkosc wału (enkoder)                | ESP C -> ESP B -> Webserver           |
| 0x05 | FEEDBACK_SENSOR_MCU_TEMPERATURE   | 1           | 1 = 1 st C      | Temaratura nadajnika                   | ESP C -> ESP B -> Webserver           |
| 0x11 | FEEDBACK_BRAKE_MOTOR_CURRENT      | 2           | 100 = 1A        | Prąd silnika hamującego (CAN)          | VESC -> STM32 -> ESP B -> Webserver   |
| 0x12 | FEEDBACK_BRAKE_MOTOR_VOLTAGE      | 2           | 100 = 1V        | Napięcie silnika hamującego (CAN)      | VESC -> STM32 -> ESP B -> Webserver   |
| 0x13 | FEEDBACK_BRAKE_MOTOR_POWER        | 2           | 10 = 1W         | Moc silnika hamującego (CAN)           | VESC -> STM32 -> ESP B -> Webserver   |
| 0x14 | FEEDBACK_BRAKE_MOTOR_SPEED        | 2           | 1 = 1 rpm       | Predkość silnika hamującego (CAN)      | VESC -> STM32 -> ESP B -> Webserver   |
| 0x15 | SET_BRAKE_MOTOR_CURRENT           | 2           | 10 = 1A         | Ustawianie prądu hamowania             | Webserver -> ESP B -> STM32 -> CAN    |
| 0x16 | SET_BRAKE_MOTOR_TORQUE            | 2           | 100 = 1Nm       | Ustawienie momentu hamowania (PID)     | Webserver -> ESP B -> STM32 -> CAN    |
| 0x21 | FEEDBACK_TEST_MOTOR_CURRENT       | 2           | 100 = 1A        | Prąd silnika badanego (CAN)            | VESC -> STM32 -> ESP B -> Webserver   |
| 0x22 | FEEDBACK_TEST_MOTOR_VOLTAGE       | 2           | 100 = 1V        | Napięcie silnika badanego (CAN)        | VESC -> STM32 -> ESP B -> Webserver   |
| 0x23 | FEEDBACK_TEST_MOTOR_POWER         | 2           | 10 = 1W         | Moc silnika badanego (CAN)             | VESC -> STM32 -> ESP B -> Webserver   |
| 0x24 | FEEDBACK_TEST_MOTOR_SPEED         | 2           | 1 = 1 rpm       | Predkość silnika badanego (CAN)        | VESC -> STM32 -> ESP B -> Webserver   |
| 0x25 | CONFIG_TEST_MOTOR_CAN_ID          | 1           | 0x70 = 0x70     | Adres CAN silnika badanego             | Webserver -> ESP B -> STM32           |


`0x0X - Płytka czujnikowa`
`0x1X - Silnik hamujący`
`0x2X - Silnik badany`



