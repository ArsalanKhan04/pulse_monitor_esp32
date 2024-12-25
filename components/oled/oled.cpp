#include "oled.h"
#include <stdio.h>
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
void oled_setup() {
  // initialize the OLED object
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    printf("SSD1306 allocation failed\n");
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer.
  display.clearDisplay();
}

void oled_display(void *pvParameters) {
  oled_data *data = (oled_data *)pvParameters;
  while(1) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(4,2);
    display.write(3);
    display.print(" BPM:");
    display.println(data->bpm);
    if (data->bpm > 100) {
      display.write(1);
      display.setTextSize(1);
      display.println(" High BPM!");
    }
    if (data->bpm < 60) {
      display.write(1);
      display.setTextSize(1);
      display.println(" Low BPM!");
    }
    if (data->bpm >= 60 && data->bpm <= 100) {
      display.write(2);
      display.setTextSize(1);
      display.println(" Normal BPM!");
    }
    if (data->bpm == 0) {
      display.write(0x0F);
      display.println(" DEAD");
    }
    display.display();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void oled_check() {

  printf("Now displaying text. \n");
  // Display Text
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,28);
  display.println("Hello world!");
  display.display();
  delay(2000);
  display.clearDisplay();

  printf("Now displaying inverted text.\n");
  // Display Inverted Text
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.setCursor(0,28);
  display.println("Hello world!");
  display.display();
  delay(2000);
  display.clearDisplay();

  printf("Now displaying numbers.\n");
  // Changing Font Size
  display.setTextColor(WHITE);
  display.setCursor(0,24);
  display.setTextSize(2);
  display.println("Hello!");
  display.display();
  delay(2000);
  display.clearDisplay();

  printf("Now displaying numbers.\n");
  // Display Numbers
  display.setTextSize(1);
  display.setCursor(0,28);
  display.println(123456789);
  display.display();
  delay(2000);
  display.clearDisplay();

  printf("Now displaying a floating point number.\n");
  // Specifying Base For Numbers
  display.setCursor(0,28);
  display.print("0x"); display.print(0xFF, HEX); 
  display.print("(HEX) = ");
  display.print(0xFF, DEC);
  display.println("(DEC)"); 
  display.display();
  delay(2000);
  display.clearDisplay();

  printf("Now displaying a bitmap.\n");
  // Display ASCII Characters
  display.setCursor(0,24);
  display.setTextSize(2);
  display.write(3);
  display.display();
  delay(2000);
  display.clearDisplay();

  printf("Now scrolling full screen.\n");
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println("Full");
  display.println("screen");
  display.println("scrolling!");
  display.display();
  display.startscrollright(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);    
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  display.clearDisplay();

  printf("Now scrolling part of the screen.\n");
  // Scroll part of the screen
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println("Scroll");
  display.println("some part");
  display.println("of the screen.");
  display.display();
  display.startscrollright(0x00, 0x00);
}
