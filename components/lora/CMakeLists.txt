set(srcs "ra01s.c")
idf_component_register(SRCS "${srcs}" PRIV_REQUIRES driver bsp INCLUDE_DIRS ".")