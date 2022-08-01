# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES )
set(COMPONENT_PRIV_REQUIRES )

set(COMPONENT_SRCS "network_time.c" "main.c" "mqtt_network.c" "wifi.c" "network_time.c")
set(COMPONENT_ADD_INCLUDEDIRS "")

register_component()
