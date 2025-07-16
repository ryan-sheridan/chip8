#include "ins.h"
#include "chip8.h"
#include "debug.h"
#include "graphics.h"

void _step(void) {
  chip8->pc_reg += 2;
  update_current_opcode();
  vlog("[v] stepped, current pc: %04X\n", chip8->pc_reg);
}

uint8_t _get_x(void) { return (chip8->cur_opcode & 0x0F00) >> 8; }

uint8_t _get_y(void) { return (chip8->cur_opcode & 0x00F0) >> 4; }

uint8_t _get_n(void) { return (chip8->cur_opcode & 0x000F); }

uint8_t _get_kk(void) { return (chip8->cur_opcode & 0x00FF); }

uint16_t _get_nnn(void) { return (chip8->cur_opcode & 0x0FFF); }

void update_current_opcode(void) {
  // explaination of the following (imagine cur_opcode 00E0)
  // - cur_opcodes are 16 bits (2 bytes)
  // - we grab the first byte by using memory[PC] (00)
  // - we shift it to the left by 8 bits (0000)
  // - we grab the second byte (EO) and | it with the first byte leaving us with
  // (00EO) as a uint16_t
  chip8->cur_opcode =
      chip8->memory[chip8->pc_reg] << 8 | chip8->memory[chip8->pc_reg + 1];
}

// 00E0
void clear_scr(void) {
  clear_framebuffer();
  chip8->draw_flag = true;
  _step();
  vlog("[0] cleared screen\n");
}

// 00EE
void return_from_subroutine(void) {
  chip8->sp_reg--;
  chip8->pc_reg = chip8->stack[chip8->sp_reg];
  _step();
  vlog("[<-] returned from subroutine\n");
}

// 1000
void jump(void) {
  chip8->pc_reg = _get_nnn();
  vlog("[->] jumped to addr, %04X\n", chip8->pc_reg);
}

// 2000
void call_subroutine(void) {
  chip8->stack[chip8->sp_reg] = chip8->pc_reg;
  chip8->sp_reg++;
  chip8->pc_reg = _get_nnn();
}

// 3000
void se_vx_kk(void) {
  if(chip8->V[_get_x()] == _get_kk()) {
    chip8->pc_reg += 4;
  } else {
    chip8->pc_reg += 2;
  }
}

// 4000
void sne_vx_kk(void) {
  if(chip8->V[_get_x()] != _get_kk()) {
    chip8->pc_reg += 4;
  } else {
    chip8->pc_reg += 2;
  }
}

// 5000
void se_vx_vy(void) {
  if(chip8->V[_get_x()] == chip8->V[_get_y()]) {
    chip8->pc_reg += 4;
  } else {
    chip8->pc_reg += 2;
  }
}

// 6XKK
void ld_vx_imm(void) {
  chip8->V[_get_x()] = _get_kk();
  _step();
}

// 7000
void add_vx_imm(void) {
  chip8->V[_get_x()] = chip8->V[_get_x()] + _get_kk();
  _step();
}

// ANNN
void ld_i_addr(void) {
  chip8->i_reg = _get_nnn();
  _step();
}

// DXYN
void drw_vx_vy_n(void) {
  // draw an n byte sprite at memory location I, at vx, vy
  // interpreter reads n bytes from memory, at the addr stored in I,
  // the bytes are displayed at vx, vy, sprites are xor'd onto the screen
  // if any pixels get erased, vf is set to 1, otherwise, its set to 0
  // sprites should wrap around to the other side of the screen
  uint8_t x_loc = chip8->V[_get_x()];
  uint8_t y_loc = chip8->V[_get_y()];
  uint8_t sprite_height = _get_n();
  uint8_t pixel;
  uint8_t uh;

  chip8->V[0xF] = FALSE;
  for (int y = 0; y < sprite_height; y++) {
    pixel = chip8->memory[chip8->i_reg + y];
    for (int x = 0; x < 8; x++) {
      // TODO: figure out what the following truly does, because im not fully
      // sure
      if ((pixel & (0x80 >> x)) != 0) {
        uint8_t pixel_x = (x_loc + x) % 64;
        uint8_t pixel_y = (y_loc + y) % 32;
        uh = get_pixel(pixel_x, pixel_y);
        if (uh == 1) {
          chip8->V[0xF] = TRUE;
        }
        set_pixel(pixel_x, pixel_y, uh ^= 1);
      }
    }
  }
  _step();
}

// CXKK
void rnd_vx_imm(void) {
  srand ((unsigned int) time (NULL));
  chip8->V[_get_x()] = rand() & _get_kk();
  vlog("[*] rand and %x\n", chip8->V[_get_x()]);
  _step();
}

// FX1E
void add_i_vx(void) {
  chip8->i_reg = chip8->i_reg + chip8->V[_get_x()];
  _step();
}

// 8000
void ld_vx_vy(void) {
  chip8->V[_get_x()] = chip8->V[_get_y()];
  _step();
}

// 8001
void or_vx_vy(void) {
  chip8->V[_get_x()] = chip8->V[_get_y()] | chip8->V[_get_x()];
  _step();
}

// EX9E
void skp_vx(void) {
  printf("key to press: %x\n", chip8->keyboard[_get_x()]);
  if(chip8->keyboard[_get_x()]) {
    printf("skipped because pressed\n");
    chip8->was_key_pressed = false;
    chip8->keyboard[_get_x()] = 0;
    chip8->pc_reg += 4;
  } else {
    _step();
  }
}

// EXA1
void sknp_vx(void) {
  printf("key to press: %x\n", chip8->keyboard[_get_x()]);
  if(!chip8->keyboard[_get_x()]) {
    printf("skipped because not pressed\n");
    chip8->pc_reg += 4;
  } else {
    _step();
  }
}

// FX65
void ld_vx_i(void) {
  for(int i=0;i<=_get_x();i++) {
    chip8->V[i] = chip8->memory[chip8->i_reg + i];
  }

  // doing this because someone elses code has it
  chip8->i_reg += (_get_x() + 1);
  _step();
}

// FX15
void ld_dt_vx(void) {
  chip8->delay_timer = _get_x();
  _step();
}

// FX07
void ld_vx_dt(void) {
  chip8->V[_get_x()] = chip8->delay_timer;
  _step();
}
