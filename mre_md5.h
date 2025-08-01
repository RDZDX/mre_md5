#ifndef _VRE_APP_WIZARDTEMPLATE_
#define	_VRE_APP_WIZARDTEMPLATE_

#include "vmsys.h"
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmsms.h"
#include "string.h"
#include "stdint.h"
#include "vmstdlib.h"

VMINT		layer_hdl[2];

typedef struct
{
	unsigned int count[2];
	unsigned int state[4];
	unsigned char buffer[64];
}MD5_CTX;

void handle_sysevt(VMINT message, VMINT param);
void handle_keyevt(VMINT event, VMINT keycode);
static VMINT job(VMWCHAR *filepath, VMINT wlen);
void MD5Init(MD5_CTX *context);
void MD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen);
void MD5Final(MD5_CTX *context,unsigned char digest[16]);
void MD5Encode(unsigned char *output,unsigned int *input,unsigned int len);
void MD5Decode(unsigned int *output,unsigned char *input,unsigned int len);
void MD5Transform(unsigned int state[4],unsigned char block[64]);
static void fill_white(void);
VMINT string_width(VMWCHAR *whead, VMWCHAR *wtail);
void display_text_line(VMUINT8 *disp_buf, VMSTR str, VMINT x, VMINT y, VMINT width, VMINT height, VMINT betlines, VMINT startLine, VMINT color, VMBOOL fix_pos);
void save_text(VMINT state, VMWSTR text);

#endif

