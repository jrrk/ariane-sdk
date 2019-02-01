/*

Decode an ethtool dump such as the following:

0x0000:		00 00 00 00 ff ff 00 00 00 00 00 00 ff ff 00 00 
0x0010:		00 00 00 00 ff ff 00 00 00 00 00 00 ff ff 00 00 
0x0020:		00 00 00 00 ff ff 00 00 00 00 00 00 ff ff 00 00 
0x0030:		00 00 00 00 ff ff 00 00 00 00 00 00 ff ff 00 00 
0x0040:		00 00 00 00 ff ff 00 00 00 00 00 00 ff ff 00 00 
0x0050:		00 00 00 00 ff ff 00 00 00 00 00 00 ff ff 00 00 
0x0060:		00 00 00 00 ff ff 00 00 00 00 00 00 ff ff 00 00 
0x0070:		00 00 00 00 ff ff 00 00 00 00 00 00 ff ff 00 00 
0x0080:		02 07 89 00 01 23 c0 00 46 00 46 00 12 c9 25 08 
0x0090:		0e 00 00 00 7b dd 04 c7 00 00 00 00 00 00 00 00 
0x00a0:		40 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
0x00b0:		00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
0x00c0:		02 07 89 00 01 23 c0 00 46 00 46 00 12 c9 25 08 
0x00d0:		0e 00 00 00 7b dd 04 c7 00 00 00 00 00 00 00 00 
0x00e0:		40 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
0x00f0:		00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 

*/

#include <stdio.h>
#include <stdint.h>

typedef struct {
  const char *regnam;
  int regoff;
  const char *description;
} reg_t;

const reg_t regnam[16] = {
  {"MACLO_OFFSET", 0x0800, "MAC address low 32-bits"},
  {"MACHI_OFFSET", 0x0808, "MAC address high 16-bits and MAC ctrl"},
  {"TPLR_OFFSET", 0x0810, "Tx packet length"},
  {"TFCS_OFFSET", 0x0818, "Tx frame check sequence register"},
  {"MDIOCTRL_OFFSET", 0x0820, "MDIO Control Register"},
  {"RFCS_OFFSET", 0x0828, "Rx frame check sequence register(read) and last register(write)"},
  {"RSR_OFFSET", 0x0830, "Rx status and reset register"},
  {"RBAD_OFFSET", 0x0838, "Rx bad frame and bad fcs register arrays"},
  {"RPLR_OFFSET", 0x0840, "Rx packet length register array"},
};
  
int main(int argc, char **argv)
{
  int i, j, verbose = argc > 1;
  char linbuf[256];
  uint32_t regs[64];
  while (fgets(linbuf, sizeof linbuf, stdin))
    {
      int adr, d[16];
      if (sscanf(linbuf, "0x%x: %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x",
                 &adr, d+0, d+1, d+2, d+3, d+4, d+5, d+6, d+7, d+8, d+9, d+10, d+11, d+12, d+13, d+14, d+15) == 17)
        {
          if (verbose)
            {
              printf("0x%.04x: ", adr);
              for (i = 0; i < 16; i++) printf("%.02x ", d[i]);
              putchar('\n');
            }
          for (i = 0; i < 16; i += 4)
            {
              int tot = 0;
              for (j = 0; j < 4; j++)
                {
                  tot += d[i+j] << (j*8);
                }
              regs[(adr+i)>>2] = tot;
            }
        }
    }
  for (i = 0x20; i < 0x30; i++)
    {
      static int off = 0;
      static const char *lastnam = "";
      reg_t nam = regnam[i-0x20];
      if (nam.regnam)
        lastnam = nam.regnam;
      else
        ++off;
      printf("%16s[%d]: %.08x\n", lastnam, off, regs[i]);
    }
}

