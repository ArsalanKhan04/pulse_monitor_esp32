idf_component_register(SRCS "oled.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES arduino Adafruit_SSD1306
                    )

project(oled)
