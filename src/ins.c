#include "ins.h"
#include "chip8.h"
#include "graphics.h"
#include "debug.h"

void _step(void) {
  chip8->pc_reg+=2;
  update_current_opcode();
  vlog("[v] stepped, current pc: %04X\n", chip8->pc_reg);
}

uint8_t _get_x(void) {
  return (chip8->cur_opcode & 0x0F00);
}

uint8_t _get_y(void) {
  return (chip8->cur_opcode & 0x00F0);
}

uint8_t _get_n(void) {
  return (chip8->cur_opcode & 0x000F);
}

uint8_t _get_kk(void) {
  return (chip8->cur_opcode & 0x00FF);
}

uint16_t _get_nnn(void) {
  return (chip8->cur_opcode & 0x0FFF);
}

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
void call_subroutine(void) {
  return;
}

// 3000
void se_vx_kk(void) {
  return;
}

// 4000
void sne_vx_kk(void) {
  return;
}

// 5000
void se_vx_vy(void) {
  return;
}

// 6000
void ld_vx_imm(void) {

  return;
}

// 7000
void add_vx_imm(void) {
  return;
}


