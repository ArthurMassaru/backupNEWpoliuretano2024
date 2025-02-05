double getBatteryAlarm() {
  battery = analogRead(BAT);
  double volt = 10 * battery / 1023.0; // 1023 of analogread * 5V / 1/2 of tension;
  return volt;
}

void BuzzerAlert (){ // Alarm of the battery
  if (getBatteryAlarm() < 7.9){ // If the voltage is below than 7.9V
    tone(BUZZER, 155, 5); //D# -2
  }
}
