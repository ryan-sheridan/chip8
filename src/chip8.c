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

  fread(memory + 0x200, sizeof(uint16_t), size, fp);
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
  opcode = memory[PC] << 8 | memory[PC + 1];
  printf("opcode: %04X\n", opcode);

  switch (opcode & 0xF000) {
  case 0x000:
    switch (opcode & 0x00FF) {
    case 0x00E0:
      // clear screen
      clear_framebuffer();
    default:
      printf("opcode %04X not implemented\n", opcode);
    }
  default:
    printf("opcode %04X not implemented\n", opcode);
  }

  // yo
  PC+=2;
  return 0;
}
