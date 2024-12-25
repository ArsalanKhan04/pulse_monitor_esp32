idf_component_register(SRCS "senddata.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES arduino esp_http_client)

project(senddata)

