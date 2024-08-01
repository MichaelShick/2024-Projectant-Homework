#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <GLFW/glfw3.h>

#define MEMORY_SIZE 4096
#define REGISTER_COUNT 16
#define STACK_SIZE 16
#define KEY_COUNT 16
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

typedef struct {
    uint8_t memory[MEMORY_SIZE];
    uint8_t V[REGISTER_COUNT];
    uint16_t I;
    uint16_t pc;
    uint8_t sp;
    uint16_t stack[STACK_SIZE];
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t keypad[KEY_COUNT];
    uint32_t display[DISPLAY_WIDTH * DISPLAY_HEIGHT];
} Chip8;

Chip8 chip8;
GLFWwindow* window;

void initialize_chip8() {
    memset(&chip8, 0, sizeof(Chip8));
    chip8.pc = 0x200;  // Program counter starts at 0x200
}

void load_rom(const char* filename) {
    FILE* rom = fopen(filename, "rb");
    if (!rom) {
        fprintf(stderr, "Failed to open ROM file: %s\n", filename);
        exit(1);
    }
    fread(&chip8.memory[0x200], 1, MEMORY_SIZE - 0x200, rom);
    fclose(rom);
}

void emulate_cycle() {
    uint16_t opcode = (chip8.memory[chip8.pc] << 8) | chip8.memory[chip8.pc + 1];
    chip8.pc += 2;

    switch(opcode & 0xF000) {
        case 0x0000:
            if (opcode == 0x00E0) {
                // Clear the display
                memset(chip8.display, 0, sizeof(chip8.display));
            }
            break;
        case 0x1000:
            // Jump to address NNN
            chip8.pc = opcode & 0x0FFF;
            break;
        case 0x6000:
            // Set VX to NN
            chip8.V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            break;
        case 0x7000:
            // Add NN to VX
            chip8.V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            break;
        case 0xA000:
            // Set I to the address NNN
            chip8.I = opcode & 0x0FFF;
            break;
        case 0xD000: {
            uint8_t x = chip8.V[(opcode & 0x0F00) >> 8];
            uint8_t y = chip8.V[(opcode & 0x00F0) >> 4];
            uint8_t height = opcode & 0x000F;
            uint8_t pixel;

            chip8.V[0xF] = 0;
            for (int yline = 0; yline < height; yline++) {
                pixel = chip8.memory[chip8.I + yline];
                for(int xline = 0; xline < 8; xline++) {
                    if((pixel & (0x80 >> xline)) != 0) {
                        if(chip8.display[(y + yline) * DISPLAY_WIDTH + (x + xline)] == 1)
                            chip8.V[0xF] = 1;
                        chip8.display[(y + yline) * DISPLAY_WIDTH + (x + xline)] ^= 1;
                    }
                }
            }
            break;
        }
        default:
            printf("Unknown opcode: 0x%X\n", opcode);
    }
}

void setup_graphics() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(1);
    }

    window = glfwCreateWindow(DISPLAY_WIDTH * 10, DISPLAY_HEIGHT * 10, "CHIP-8 Emulator", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
}

void draw_graphics() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            if (chip8.display[y * DISPLAY_WIDTH + x]) {
                glColor3f(1.0f, 1.0f, 1.0f);
            } else {
                glColor3f(0.0f, 0.0f, 0.0f);
            }

            float pixel_width = 2.0f / DISPLAY_WIDTH;
            float pixel_height = 2.0f / DISPLAY_HEIGHT;
            float x1 = x * pixel_width - 1.0f;
            float y1 = 1.0f - y * pixel_height;

            glBegin(GL_QUADS);
            glVertex2f(x1, y1);
            glVertex2f(x1 + pixel_width, y1);
            glVertex2f(x1 + pixel_width, y1 - pixel_height);
            glVertex2f(x1, y1 - pixel_height);
            glEnd();
        }
    }

    glfwSwapBuffers(window);
}

int main() {
    const char* rom_filename = "IBM Logo.ch8";  // Hardcoded ROM filename

    initialize_chip8();
    load_rom(rom_filename);
    setup_graphics();

    while (!glfwWindowShouldClose(window)) {
        emulate_cycle();
        draw_graphics();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
