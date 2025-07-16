#ifndef INS_H
#define INS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void update_current_opcode(void);
void return_from_subroutine(void);
void clear_scr(void);
void jump(void);
void call_subroutine(void);
void se_vx_kk(void);
void sne_vx_kk(void);
void se_vx_vy(void);
void ld_vx_imm(void);
void add_vx_imm(void);

void ld_i_addr(void);
void drw_vx_vy_n(void);
void rnd_vx_imm(void);
void add_i_vx(void);
void ld_vx_vy(void);
void or_vx_vy(void);
void skp_vx(void);
void sknp_vx(void);
void ld_vx_i(void);

#endif
