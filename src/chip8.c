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
  update_current_opcode();
  // print_debug_info();
  vlog_pc_opcode();

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
        default:
          printf("cur_opcode %04X not implemented\n", chip8->cur_opcode);
          start_debug_shell();
          exit(EXIT_FAILURE);
      }
      break;
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
    case 0xA000:
      // load nnn into i
      ld_i_addr();
      break;
    case 0xD000:
      // draws sprite at vx vy, with a width of 8 pixels and a height of N pixels
      drw_vx_vy_n();
      break;
    default:
      printf("cur_opcode %04X not implemented\n", chip8->cur_opcode);
      exit(EXIT_FAILURE);
      break;
  }

  if(chip8->draw_flag) {
    render_framebuffer();
    chip8->draw_flag = false;
  }

  return 0;
}
