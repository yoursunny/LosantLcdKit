#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);

void
setup()
{
  Wire.pins(12, 14);

  lcd.begin(16,2);
  lcd.init();

  lcd.backlight();

  lcd.setCursor(5, 0);
  lcd.print("HELLO");

  lcd.setCursor(5, 1);      
  lcd.print("WORLD");
}

void
loop()
{
}
