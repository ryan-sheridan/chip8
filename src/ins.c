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
void call_subroutine(void) { return; }

// 3000
void se_vx_kk(void) { return; }

// 4000
void sne_vx_kk(void) { return; }

// 5000
void se_vx_vy(void) { return; }

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
        vlog("x: %d, y: %d\n", pixel_x, pixel_y);
        set_pixel(pixel_x, pixel_y, uh ^= 1);
      }
    }
  }
  _step();
  printf("Sprite data at I=0x%03X: %02X %02X %02X %02X %02X\n", chip8->i_reg,
         chip8->memory[chip8->i_reg], chip8->memory[chip8->i_reg + 1],
         chip8->memory[chip8->i_reg + 2], chip8->memory[chip8->i_reg + 3],
         chip8->memory[chip8->i_reg + 4]);
}
