#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Inisialisasi LCD dengan alamat I2C 0x27

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'*', '2', '3', '1'},
  {'/', '6', '5', '4'},
  {'+', '9', '8', '7'},
  {'-', 'C', '0', '='}
};

byte rowPins[ROWS] = {2, 3, 4, 5}; // Pin baris keypad
byte colPins[COLS] = {6, 7, 8, 9}; // Pin kolom keypad

Keypad keyp = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

long num1 = 0, num2 = 0;
char operation = '\0'; // Operator yang dipilih
bool isSecondNumber = false; // Apakah sedang memasukkan angka kedua?

void setup() {
  lcd.init();         // Inisialisasi LCD
  lcd.backlight();    // Aktifkan lampu latar LCD
  lcd.print("Calculator");
  delay(1000);
  lcd.clear();
}

void loop() {
  char key = keyp.getKey(); // Baca tombol yang ditekan

  if (key != NO_KEY) {
    handleKeyPress(key);
  }
}

void handleKeyPress(char key) {
  if (key >= '0' && key <= '9') { // Jika tombol angka ditekan
    if (isSecondNumber) {
      num2 = num2 * 10 + (key - '0'); // Menambahkan digit ke angka kedua
    } else {
      num1 = num1 * 10 + (key - '0'); // Menambahkan digit ke angka pertama
    }
    updateDisplay();
  } else if (key == '+' || key == '-' || key == '*' || key == '/') { // Jika tombol operator ditekan
    if (!isSecondNumber) {
      operation = key;    // Simpan operator
      isSecondNumber = true; // Beralih ke angka kedua
      updateDisplay();
    }
  } else if (key == '=') { // Jika tombol '=' ditekan
    long result = calculate();
    displayResult(result); // Tampilkan hasil
    resetCalculator();     // Reset kalkulator
  } else if (key == 'C') { // Jika tombol 'C' ditekan
    resetCalculator();
    lcd.clear();
    lcd.print("Cleared");
    delay(1000);
    lcd.clear();
  }
}

long calculate() {
  switch (operation) {
    case '+': return num1 + num2;
    case '-': return num1 - num2;
    case '*': return num1 * num2;
    case '/': return (num2 != 0) ? num1 / num2 : 0; // Hindari pembagian dengan nol
    default: return 0;
  }
}

void updateDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(num1);
  if (operation != '\0') {
    lcd.print(" ");
    lcd.print(operation);
    lcd.print(" ");
    lcd.print(num2);
  }
}

void displayResult(long result) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Result:");
  lcd.setCursor(0, 1);
  lcd.print(result);
  delay(3000);
  lcd.clear();
}

void resetCalculator() {
  num1 = 0;
  num2 = 0;
  operation = '\0';
  isSecondNumber = false;
}
