/* 因为一个 Arduino mini 模拟引脚不够用，此代码用于辅助 sketch_air-condition 读取温控器的 LCD 信息 */
#define IN_PIN (A0)
#define OUT_PIN (2)

/* 模拟输入只有最低和最高有效，其他无效，所以他有 0(低), 1(高), 2(无效) 3个状态，可以用 7 个数字引脚输出给主 Arduino mini。(3 ^ 4 = 81, 2 ^ 7 = 128) */
#define IN_NUM (4)
#define OUT_NUM (7)

void setup() {
  Serial.begin(115200);
  Serial.println("setup...");

  for (int i = 0; i < IN_NUM; i++) {
    pinMode(IN_PIN + i, INPUT);
  }

#if 0
  for (int i = 0; i < OUT_NUM; i++) {
    pinMode(OUT_PIN + i, OUTPUT);
    digitalWrite(OUT_PIN + i, LOW);
  }
#endif

  DDRD |= B11111100;
  DDRB |= B00000001;
}

void loop() {
  int i = 0;
  int v = 0;
  int f = 1;
  uint8_t data = 0;
  uint8_t u = 0;

  for (i = 0; i < IN_NUM; i++) {
    v = analogRead(IN_PIN + i);
    if (v < 32) {
      v = 0;
    } else if (v > 992) {
      v = 1;
    } else {
      v = 2;
    }

    data += (v * f);
    f *= 3;
  }

//  Serial.println("seg_s:");
//  Serial.println(data);

  u = ((data << 2) & B11111100) | (PORTD & B00000011);
  PORTD = u;

  u = ((data >> 6) & B00000001) | (PORTB & B11111110);
  PORTB = u;
}
