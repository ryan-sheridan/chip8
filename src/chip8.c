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
    case 0x8000:
      switch(chip8->cur_opcode & 0x000F) {
        case 0x0000:
          ld_vx_vy();
          break;
        case 0x0001:
          or_vx_vy();
          break;
        case 0x0002:
          break;
        case 0x0003:
          break;
        case 0x0004:
          break;
        case 0x0005:
          break;
        case 0x0006:
          break;
        case 0x0007:
          break;
        case 0x000E:
          break;
        default:
          vlog("8000 sub-opcode %04X not implemented\n", chip8->cur_opcode);
          exit(EXIT_FAILURE);
      }
      break;
    case 0xA000:
      // load nnn into i
      ld_i_addr();
      break;
    case 0xB000:
      // TODO:
      break;
    case 0xC000:
      rnd_vx_imm();
      break;
    case 0xD000:
      // draws sprite at vx vy, with a width of 8 pixels and a height of N pixels
      drw_vx_vy_n();
      chip8->draw_flag = true;
      break;
    case 0xE000:
      switch(chip8->cur_opcode & 0x00FF) {
        case 0x009E:
          skp_vx();
          break;
        case 0x00A1:
          sknp_vx();
          break;
        default:
          vlog("E000 sub-opcode %04X not implemented\n", chip8->cur_opcode);
          exit(EXIT_FAILURE);
      }
      break;

    case 0xF000:
      switch(chip8->cur_opcode & 0x0FF) {
        case 0x0007:
          ld_vx_dt();
          break;
        case 0x000A:
          break;
        case 0x0015:
          ld_dt_vx();
          break;
        case 0x0018:
          break;
        case 0x001E:
          add_i_vx();
          break;
        case 0x0029:
          break;
        case 0x0033:
          break;
        case 0x0055:
          break;
        case 0x0065:
          ld_vx_i();
          break;
        default:
          vlog("F000 sub-opcode %04X not implemented\n", chip8->cur_opcode);
          exit(EXIT_FAILURE);
      }
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
