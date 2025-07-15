#include "chip8.h"
#include "graphics.h"

int load_rom(char *rom_path) {
  FILE *fp = fopen(rom_path, "r");
  if (!fp) {
    fprintf(stderr, "failed to open rom %s\n", rom_path);
    return 1;
  }

  fseek(fp, 0, SEEK_END);
  int size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  fread(chip8->memory + 0x200, sizeof(uint16_t), size, fp);
  fclose(fp);

  return 0;
}

int execute(void) {
  // explaination of the following (imagine opcode 00E0)
  // - opcodes are 16 bits (2 bytes)
  // - we grab the first byte by using memory[PC] (00)
  // - we shift it to the left by 8 bits (0000)
  // - we grab the second byte (EO) and | it with the first byte leaving us with
  // (00EO) as a uint16_t
  chip8->opcode = chip8->memory[chip8->PC] << 8 | chip8->memory[chip8->PC + 1];
  printf("opcode: %04X\n", chip8->opcode);

  switch (chip8->opcode & 0xF000) {
  case 0x000:
    switch (chip8->opcode & 0x00FF) {
    case 0x00E0:
      // clear screen
      clear_framebuffer();
      break;
    case 0x00EE:
      break;
    default:
      printf("opcode %04X not implemented\n", chip8->opcode);
    }
  default:
    printf("opcode %04X not implemented\n", chip8->opcode);
  }

  // yo
  chip8->PC += 2;
  return 0;
}
