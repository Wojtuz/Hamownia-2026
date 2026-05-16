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

| Zakres ID | Moduł |
|------------|--------|
| 0x0X | Płytka czujnikowa |
| 0x4X | Silnik hamujący |
| 0x8X | Silnik badany |
| 0xFF | Konfiguracja |

---

## Wiadomości protokołu

| ID   | Nazwa                             | Rozmiar (B) | Skala / Jednostki | Opis                                           | Kierunek |
|------|-----------------------------------|--------------|-------------------|------------------------------------------------|-----------|
| 0x01 | FEEDBACK_SENSOR_TORQUE            | 2 | 100 = 1 Nm | Moment hamowania (PID / wykresy) | ESP C -> ESP B -> (Webserver & STM32) |
| 0x02 | FEEDBACK_SENSOR_BATTERY_DATA      | 1 | 255 = 5 V | Status komunikacji / poziom baterii | ESP C -> ESP B -> Webserver |
| 0x03 | FEEDBACK_SENSOR_MOTOR_TEMPERATURE | 1 | 1 = 1 °C | Temperatura silnika (PT100) | ESP C -> ESP B -> Webserver |
| 0x04 | FEEDBACK_SENSOR_MOTOR_SPEED_DATA  | 2 | 1 = 1 rpm | Prędkość wału z enkodera | ESP C -> ESP B -> Webserver |
| 0x05 | FEEDBACK_SENSOR_MCU_TEMPERATURE   | 1 | 1 = 1 °C | Temperatura MCU płytki czujnikowej | ESP C -> ESP B -> Webserver |
| 0x41 | FEEDBACK_BRAKE_MOTOR_CURRENT      | 2 | 100 = 1 A | Prąd silnika hamującego | VESC -> STM32 -> ESP B -> Webserver |
| 0x42 | FEEDBACK_BRAKE_MOTOR_VOLTAGE      | 2 | 100 = 1 V | Napięcie silnika hamującego | VESC -> STM32 -> ESP B -> Webserver |
| 0x43 | FEEDBACK_BRAKE_MOTOR_POWER        | 2 | 10 = 1 W | Moc silnika hamującego | VESC -> STM32 -> ESP B -> Webserver |
| 0x44 | FEEDBACK_BRAKE_MOTOR_SPEED        | 2 | 1 = 1 rpm | Prędkość silnika hamującego | VESC -> STM32 -> ESP B -> Webserver |
| 0x45 | FEEDBACK_BRAKE_MOTOR_TEMPERATURE  | 1 | 1 = 1 °C | Temperatura silnika hamującego | VESC -> STM32 -> ESP B -> Webserver |
| 0x46 | SET_BRAKE_MOTOR_CURRENT           | 2 | 10 = 1 A | Ustawienie prądu hamowania | Webserver -> ESP B -> STM32 -> CAN |
| 0x47 | SET_BRAKE_MOTOR_TORQUE            | 2 | 100 = 1 Nm | Ustawienie momentu hamowania (PID) | Webserver -> ESP B -> STM32 -> CAN |
| 0x81 | FEEDBACK_TEST_MOTOR_CURRENT       | 2 | 100 = 1 A | Prąd silnika badanego | VESC -> STM32 -> ESP B -> Webserver |
| 0x82 | FEEDBACK_TEST_MOTOR_VOLTAGE       | 2 | 100 = 1 V | Napięcie silnika badanego | VESC -> STM32 -> ESP B -> Webserver |
| 0x83 | FEEDBACK_TEST_MOTOR_POWER         | 2 | 10 = 1 W | Moc silnika badanego | VESC -> STM32 -> ESP B -> Webserver |
| 0x84 | FEEDBACK_TEST_MOTOR_SPEED         | 2 | 1 = 1 rpm | Prędkość silnika badanego | VESC -> STM32 -> ESP B -> Webserver |
| 0x85 | FEEDBACK_TEST_MOTOR_TEMPERATURE   | 1 | 1 = 1 °C | Temperatura silnika badanego | VESC -> STM32 -> ESP B -> Webserver |
| 0xFF | CONFIG_TEST_MOTOR_CAN_ID          | 1 | np. 0x70 | Adres CAN silnika badanego | Webserver -> ESP B -> STM32 |


`0x0X → płytka czujnikowa`
`0x4X → silnik hamujący`
`0x8X → silnik badany`
`0xFF → konfiguracja`



