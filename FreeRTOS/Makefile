# Nome do executável
TARGET = freertos_project

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -I./Source/include \
         -I./include \
         -I./portable/ThirdParty/GCC/Posix \
         -I./portable/ThirdParty/GCC/Posix/utils \
         -Wall -Wextra -std=c99

# Fontes do FreeRTOS
FREERTOS_SRC = \
    Source/tasks.c \
    Source/queue.c \
    Source/timers.c \
    Source/event_groups.c \
    Source/list.c \
    portable/ThirdParty/GCC/Posix/port.c \
    portable/ThirdParty/GCC/Posix/utils/wait_for_event.c \
    portable/MemMang/heap_3.c

# Arquivo principal
SRC = main.c $(FREERTOS_SRC)

# Regras de build
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) -lpthread

clean:
	rm -f $(TARGET)
