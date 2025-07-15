#include "ins.h"
#include "chip8.h"
#include "graphics.h"

void step(void) {
  chip8->pc_reg+=2;
}

// 00E0
void clear_scr(void) {
  printf("cleared screen\n");
  clear_framebuffer();
  step();
}

// 00EE
void return_from_subroutine(void) {
  printf("returned from subroutine\n");
  chip8->sp_reg--;
  chip8->pc_reg = chip8->stack[chip8->sp_reg];
  step();
}

// 1000
void jump(void) {
  return;
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


