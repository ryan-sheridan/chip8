#include "chip8.h"
#include "debug.h"
#include "graphics.h"
#include "ins.h"
#include <stdlib.h>

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
  // explaination of the following (imagine cur_opcode 00E0)
  // - cur_opcodes are 16 bits (2 bytes)
  // - we grab the first byte by using memory[PC] (00)
  // - we shift it to the left by 8 bits (0000)
  // - we grab the second byte (EO) and | it with the first byte leaving us with
  // (00EO) as a uint16_t
  chip8->cur_opcode =
      chip8->memory[chip8->pc_reg] << 8 | chip8->memory[chip8->pc_reg + 1];

  print_debug_info();

  switch (chip8->cur_opcode & 0xF000) {
    case 0x0000:
      switch (chip8->cur_opcode & 0x00FF) {
        case 0x00E0:
          // clear screen
          clear_scr();
          break;
        case 0x00EE:
          return_from_subroutine();
          break;
        case 0x1000:
          break;
        default:
          printf("cur_opcode %04X not implemented\n", chip8->cur_opcode);
          start_debug_shell();
          exit(EXIT_FAILURE);
          break;
      }
    case 0x1000:
      // jump
      jump();
      break;
    case 0x2000:
      // call subroutine at addr
      call_subroutine();
      break;
    case 0x3000:
      // skip if vx = kk
      se_vx_kk();
      break;
    case 0x4000:
      // skip if vx != kk NOTE: (pc += 4 i think)
      sne_vx_kk();
      break;
    case 0x5000:
      // skip if vx == vy
      se_vx_vy();
      break;
    case 0x6000:
      // load immediate 8 bit const into vx
      ld_vx_imm();
      break;
    case 0x7000:
      // add vx and imm and store in vx
      add_vx_imm();
      break;
    default:
      printf("cur_opcode %04X not implemented\n", chip8->cur_opcode);
      exit(EXIT_FAILURE);
      break;
  }

  return 0;
}
