#include <LiquidCrystal.h>
#include <PID_v2.h>
#include <HX711_ADC.h>

// Khai báo chân cho encoder
const int encoderPinA = 2; // Chân A của encoder
const int encoderPinB = 3; // Chân B của encoder

// Khai báo chân điều khiển động cơ
const int motorPin1 = 4; // Chân IN1 trên module L298N
const int motorPin2 = 5; // Chân IN2 trên module L298N
const int enablePin = 9; // Chân ENA trên module L298N

// Khai báo chân cho LCD 1602
LiquidCrystal lcd(7, 8, 10, 11, 12, 13);

// Khai báo chân cho nút bấm
const int buttonPin = 6;

// Khai báo biến PID
double setpoint = 0; // Giá trị mục tiêu tốc độ
double input, output;
double Kp = 2; // Hệ số P (Proportional)
double Ki = 5; // Hệ số I (Integral)
double Kd = 1; // Hệ số D (Derivative)
PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

// Khai báo biến cho encoder
volatile long encoderCount = 0;
long lastEncoderCount = 0;

// Khai báo biến cho trạng thái nút bấm
int buttonState = 0;
int lastButtonState = 0;
int speedIndex = 0; // Chỉ số tốc độ hiện tại

// Khai báo biến cho Loadcell HX711
//HX711_ADC Loadcell;
HX711_ADC Loadcell(2, 3); // Chân DOUT được kết nối vào chân 2, chân SCK được kết nối vào chân 3

void setup() {
  // Khai báo chế độ INPUT cho chân encoder
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), updateEncoder, CHANGE);
  
  // Khai báo chế độ OUTPUT cho các chân điều khiển động cơ
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  
  // Khai báo chế độ INPUT_PULLUP cho chân nút bấm
  pinMode(buttonPin, INPUT_PULLUP);
  
  // Khởi tạo PID
  myPID.SetMode(AUTOMATIC);
  
  // Thiết lập mục tiêu ban đầu
  setpoint = 100; // Điều khiển tốc độ để đạt được giá trị mục tiêu 100 (có thể điều chỉnh)
  
  // Khai báo kích thước của màn hình LCD (16x2)
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Toc do:");
  
  // Khởi tạo Loadcell
  Loadcell.begin();
  Loadcell.start(1000); // Đợi 1 giây để khởi động
}

void loop() {
  // Đọc giá trị đầu vào từ encoder
  noInterrupts();
  input = encoderCount - lastEncoderCount;
  lastEncoderCount = encoderCount;
  interrupts();

  // Tính toán điều khiển PID
  myPID.Compute();

  // Điều khiển động cơ bằng giá trị đầu ra từ PID
  if (output > 0) {
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
  } else {
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
  }
  
  // Điều khiển tốc độ bằng PWM
  int speed = abs(output); // Điều khiển tốc độ dương
  analogWrite(enablePin, speed);

  // Đọc trạng thái nút bấm và điều chỉnh tốc độ
  buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState) {
    if (buttonState == LOW) {
      speedIndex = (speedIndex + 1) % 5; // Chuyển đổi giữa 5 tốc độ
      switch (speedIndex) {
        case 0:
          setpoint = 100;
          break;
        case 1:
          setpoint = 200;
          break;
        case 2:
          setpoint = 300;
          break;
        case 3:
          setpoint = 400;
          break;
        case 4:
          setpoint = 500;
          break;
      }
    }
    lastButtonState = buttonState;
  }

  // Hiển thị tốc độ lên LCD
  lcd.setCursor(8, 0);
  lcd.print("   "); // Xóa dòng cũ
  lcd.setCursor(8, 0);
  lcd.print(input);

  // Đo và hiển thị khối lượng sản phẩm
  if (Loadcell.update()) {
    lcd.setCursor(0, 1);
    lcd.print("Khoi luong: ");
    lcd.print(Loadcell.getData(), 2); // Hiển thị khối lượng với 2 chữ số sau dấu thập phân
    lcd.print(" kg");
  }

  // Đợi một khoảng thời gian trước khi lặp lại
  delay(100);
}

void updateEncoder() {
  int encoderAState = digitalRead(encoderPinA);
  int encoderBState = digitalRead(encoderPinB);
  
  if ((encoderAState == HIGH && encoderBState == LOW) ||
      (encoderAState == LOW && encoderBState == HIGH)) {
    encoderCount++;
  } else {
    encoderCount--;
  }
}