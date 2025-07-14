#include "chip8.h"

int load_rom(char *rom_path) {
  FILE *fp = fopen(rom_path, "r");
  if(!fp) {
    fprintf(stderr, "failed to open rom %s\n", rom_path);
    return 1;
  }

  fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0 ,SEEK_SET);

	fread(memory+0x200,sizeof(uint16_t),size,fp);
  fclose(fp);

  return 0;
}
