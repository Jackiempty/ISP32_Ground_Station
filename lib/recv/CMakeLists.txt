set(component_srcs "recv.c")
idf_component_register(SRCS "${component_srcs}" PRIV_REQUIRES driver bsp INCLUDE_DIRS ".")