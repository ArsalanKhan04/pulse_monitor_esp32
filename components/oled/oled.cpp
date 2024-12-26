#include "oled.h"
#include <stdio.h>
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
void oled_setup() {
  // initialize the OLED object
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    printf("SSD1306 allocation failed\n");
    for(;;); // Don't proceed, loop forever
  }

}

void oled_display(void *pvParameters) {
  oled_data *data = (oled_data *)pvParameters;
  while(1) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(4,2);
    display.write(3);
    display.print(" ");
    display.println(data->bpm);
    if (data->bpm > 100) {
      display.write(1);
      display.setTextSize(1);
      display.println(" High BPM!");
    }
    if (data->bpm == 0) {
      display.write(0x0F);
      display.println(" DEAD");
    } else if (data->bpm < 60) {
      display.write(1);
      display.setTextSize(1);
      display.println(" Low BPM!");
    }
    if (data->bpm >= 60 && data->bpm <= 100) {
      display.write(2);
      display.setTextSize(1);
      display.println(" Normal BPM!");
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

// Version 1: All information on one screen
void displayTask(void *pvParameters) {
  oled_data *data = (oled_data *)pvParameters;
  
  while(1) {
    display.clearDisplay();
    display.setTextColor(WHITE);
    
    // Top section - BPM
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.write(3);  // Heart symbol
    display.print(" ");
    display.print(data->bpm);
    
    // Status icon and message
    display.setCursor(90, 0);
    if (data->bpm > 100) {
      display.write(1);  // Sad face
      display.setTextSize(1);
      display.setCursor(0, 18);
      display.println("High BPM!");
    } else if (data->bpm == 0) {
      display.write(0x0F);
      display.setCursor(0, 18);
      display.println("DEAD");
    } else if (data->bpm < 60) {
      display.write(1);  // Sad face
      display.setTextSize(1);
      display.setCursor(0, 18);
      display.println("Low BPM!");
    } else {
      display.write(2);  // Smiley face
      display.setTextSize(1);
      display.setCursor(0, 18);
      display.println("Normal BPM");
    }
    
    // Bottom section - SpO2
    display.drawLine(0, 32, 128, 32, WHITE);  // Separator line
    display.setTextSize(2);
    display.setCursor(0, 40);
    display.print("O2: ");
    display.print(data->oxy);
    display.print("%");
    
    // SpO2 status
    display.setTextSize(1);
    display.setCursor(0, 56);
    if (data->oxy >= 95) {
      display.println("Normal O2 Level");
    } else if (data->oxy >= 90) {
      display.println("Low O2 - Check!");
    } else if (data->oxy > 0) {
      display.println("Critical O2!");
    }
    
    display.display();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Version 2: Slideshow version
void displaySlideshow(void *pvParameters) {
  oled_data *data = (oled_data *)pvParameters;
  uint8_t screen = 0;  // Track which screen to show
  
  while(1) {
    display.clearDisplay();
    display.setTextColor(WHITE);
    
    if (screen == 0) {  // BPM Screen
      // Large BPM display
      display.setTextSize(3);
      display.setCursor(20, 5);
      display.write(3);  // Heart symbol
      display.print(" ");
      display.print(data->bpm);
      
      // Status message
      display.setTextSize(1);
      display.setCursor(0, 48);
      if (data->bpm > 100) {
        display.write(1);
        display.println(" High BPM!");
      } else if (data->bpm == 0) {
        display.write(0x0F);
        display.println(" DEAD");
      } else if (data->bpm < 60) {
        display.write(1);
        display.println(" Low BPM!");
      } else {
        display.write(2);
        display.println(" Normal BPM");
      }
    } else {  // SpO2 Screen
      // Large SpO2 display
      display.setTextSize(3);
      display.setCursor(15, 5);
      display.setTextSize(2);
      display.print("O2  ");
      display.setTextSize(3);
      display.print(data->oxy);
      display.print("%");
      
      // Status message
      display.setTextSize(1);
      display.setCursor(25, 48);
      if (data->oxy >= 95) {
        display.write(2);
        display.println(" Normal O2 Level");
      } else if (data->oxy >= 90) {
        display.write(1);
        display.println(" Low O2 - Check!");
      } else if (data->oxy > 0) {
        display.write(1);
        display.println(" Critical O2!");
      }
    }
    
    display.display();
    screen = (screen + 1) % 2;  // Toggle between screens
    vTaskDelay(2000 / portTICK_PERIOD_MS);  // Show each screen for 2 seconds
  }
}
