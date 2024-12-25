idf_component_register(SRCS "gps.cpp"
                      INCLUDE_DIRS "."
                      REQUIRES arduino TinyGPSPlus)

project(gps)
