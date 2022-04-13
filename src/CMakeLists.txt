add_compile_options(-Wall -Wextra -pedantic -Werror)

add_library (lpc32x0lib 
clkpwr.c
gpdma.c
emc.c
mlc.c
slc.c
lcd.c
ts.c
gpio.c
pinmux.c
spi.c
ssp.c
registers.c
registers.h)

target_include_directories (lpc32x0lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable (lpc32x0-offline lpc32x0-offline.c)
target_link_libraries (lpc32x0-offline LINK_PUBLIC lpc32x0lib)

add_executable (lpc32x0-dump lpc32x0-dump.c)
target_link_libraries (lpc32x0-dump LINK_PUBLIC lpc32x0lib)

add_executable (lpc32x0-write lpc32x0-write.c)
target_link_libraries (lpc32x0-write LINK_PUBLIC lpc32x0lib)

add_executable (lpc32x0-spi lpc32x0-spi.c)
target_link_libraries (lpc32x0-spi LINK_PUBLIC lpc32x0lib)

install(TARGETS lpc32x0-offline lpc32x0-dump lpc32x0-write lpc32x0-spi DESTINATION bin)
