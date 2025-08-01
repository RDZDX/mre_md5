#include "mre_md5.h"

VMINT response = -1;
VMINT trigeris = 0;
VMINT trigeris1 = 0;
VMINT trigeris2 = 0;
VMBOOL trigerisq = VM_FALSE;
VMINT pirmas_kartas = 0;
VMUINT8 *buffer = 0;
VMUINT8 *hcanvas = 0;

VMINT ncharacter_height = -1;
VMINT nscreen_width = -1;
VMINT nscreen_height = -1;
VMINT filledDsplByLines = 0;
VMBOOL first = VM_FALSE;
VMBOOL second = VM_FALSE;
VMBOOL third = VM_FALSE;
VMCHAR md5_string[33] = "test"; // 32 simboliai + '\0'
VMWCHAR title_name[24] = {0};

#define F(x,y,z) ((x & y) | (~x & z))
#define G(x,y,z) ((x & z) | (y & ~z))
#define H(x,y,z) (x^y^z)
#define I(x,y,z) (y ^ (x | ~z))
#define ROTATE_LEFT(x,n) ((x << n) | (x >> (32-n)))
#define FF(a,b,c,d,x,s,ac) \
	{ \
		a += F(b,c,d) + x + ac; \
		a = ROTATE_LEFT(a,s); \
		a += b; \
	}
#define GG(a,b,c,d,x,s,ac) \
	{ \
		a += G(b,c,d) + x + ac; \
		a = ROTATE_LEFT(a,s); \
		a += b; \
	}
#define HH(a,b,c,d,x,s,ac) \
	{ \
		a += H(b,c,d) + x + ac; \
		a = ROTATE_LEFT(a,s); \
		a += b; \
	}
#define II(a,b,c,d,x,s,ac) \
	{ \
		a += I(b,c,d) + x + ac; \
		a = ROTATE_LEFT(a,s); \
		a += b; \
	}

unsigned char PADDING[] = {
	0x80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

void vm_main(void) {

   layer_hdl[0] = -1;
   vm_reg_sysevt_callback(handle_sysevt);
   vm_reg_keyboard_callback(handle_keyevt);
   vm_font_set_font_size(VM_SMALL_FONT);
   ncharacter_height = vm_graphic_get_character_height();
   nscreen_width = vm_graphic_get_screen_width();
   nscreen_height = vm_graphic_get_screen_height();
   hcanvas = (VMUINT8*)vm_graphic_create_canvas(nscreen_width, nscreen_height);
   buffer = hcanvas + VM_CANVAS_DATA_OFFSET;
   vm_ascii_to_ucs2(title_name, (strlen("Input text:") + 1) * 2, "Input text:");
   vm_input_set_editor_title(title_name);
   response = vm_selector_run(0, 0, job);

}

void handle_sysevt(VMINT message, VMINT param) {
    
    switch (message) {
        case VM_MSG_CREATE:
            layer_hdl[1] = vm_graphic_create_layer(0, 0, nscreen_width, nscreen_height, -1);
            layer_hdl[0] = vm_graphic_create_layer_ex(0, 0, nscreen_width, nscreen_height, -1, VM_CREATE_CANVAS, 0);

        case VM_MSG_ACTIVE:
            buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);
            vm_graphic_set_clip(0, 0, nscreen_width, nscreen_height);
            vm_switch_power_saving_mode(turn_off_mode);
            break;

        case VM_MSG_PAINT:
            vm_switch_power_saving_mode(turn_off_mode);

            if (trigeris != 0) {vm_exit_app();}
            if (trigeris1 == 1) {trigeris2 = 1;}
            if (response == 0) {trigeris1 = 1;}
            if (trigeris2 == 1) {vm_exit_app();}
            if(second == VM_TRUE){
              second = VM_FALSE;
              display_text_line(buffer, md5_string, 0, filledDsplByLines, nscreen_width, nscreen_height, 2, 1, VM_COLOR_BLACK, VM_FALSE);
            } else {
              vm_graphic_flush_layer(layer_hdl, 1);
            }
            break;

        case VM_MSG_INACTIVE:
            vm_switch_power_saving_mode(turn_on_mode);
            break;

        case VM_MSG_QUIT:
            if (layer_hdl[0] != -1) {
                vm_graphic_delete_layer(layer_hdl[0]);
                vm_graphic_delete_layer(layer_hdl[1]);
            }
            break;
    }
}

void handle_keyevt(VMINT event, VMINT keycode) {

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_RIGHT_SOFTKEY) {
        if (layer_hdl[0] != -1) {
            vm_graphic_delete_layer(layer_hdl[0]);
            layer_hdl[0] = -1;
        }
        vm_exit_app();
    }
    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_LEFT_SOFTKEY) {
        second = VM_FALSE;
        vm_selector_run(0, 0, job);
    }

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_NUM1) {
        second = VM_FALSE;
        vm_input_text3(NULL, 100, 32, save_text);
    }

}

static VMINT job(VMWCHAR *filepath, VMINT wlen) {

    VMUINT nread;
    unsigned char decrypt[16];
    MD5_CTX md5;
    VMFILE fp;
    unsigned char buffer_read[1024];
    int i;

    response = 1;
    //trigeris = 1;
    trigeris1 = 0;
    trigeris2 = 0;

    fp = vm_file_open(filepath, MODE_READ, TRUE);
    if (fp < 0) {
        return -1;
    }

    MD5Init(&md5);

    while (!vm_file_is_eof(fp)) {
        if (vm_file_read(fp, buffer_read, sizeof(buffer_read), &nread) > 0 && nread > 0) {
            MD5Update(&md5, buffer_read, nread);
        } else {
            break;
        }
    }

    vm_file_close(fp);

    MD5Final(&md5, decrypt);

    for (i = 0; i < 16; i++) {
        sprintf(md5_string + (i * 2), "%02x", decrypt[i]);
    }

    md5_string[32] = '\0';
    second = VM_TRUE;

    return 0;

}


void MD5Init(MD5_CTX *context) {

	context->count[0] = 0;
	context->count[1] = 0;
	context->state[0] = 0x67452301;
	context->state[1] = 0xEFCDAB89;
	context->state[2] = 0x98BADCFE;
	context->state[3] = 0x10325476;
}

void MD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen) {

	unsigned int i = 0,index = 0,partlen = 0;
	index = (context->count[0] >> 3) & 0x3F;
	partlen = 64 - index;
	context->count[0] += inputlen << 3;
	if(context->count[0] < (inputlen << 3)) {
		context->count[1]++;
	}
	context->count[1] += inputlen >> 29;

	if(inputlen >= partlen) {
		memcpy(&context->buffer[index],input,partlen);
		MD5Transform(context->state,context->buffer);
		for(i = partlen;i+64 <= inputlen;i+=64) {
			MD5Transform(context->state,&input[i]);
		}
		index = 0;
	} else {
		i = 0;
	}
	
	memcpy(&context->buffer[index],&input[i],inputlen-i);
}

void MD5Final(MD5_CTX *context,unsigned char digest[16]) {

	unsigned int index = 0,padlen = 0;
	unsigned char bits[8];
	index = (context->count[0] >> 3) & 0x3F;
	padlen = (index < 56)?(56-index):(120-index);
	MD5Encode(bits,context->count,8);
	MD5Update(context,PADDING,padlen);
	MD5Update(context,bits,8);
	MD5Encode(digest,context->state,16);
}

void MD5Encode(unsigned char *output,unsigned int *input,unsigned int len) {

	unsigned int i = 0,j = 0;
	while(j < len) {
		output[j] = input[i] & 0xFF;
		output[j+1] = (input[i] >> 8) & 0xFF;
		output[j+2] = (input[i] >> 16) & 0xFF;
		output[j+3] = (input[i] >> 24) & 0xFF;
		i++;
		j+=4;
	}
}

void MD5Decode(unsigned int *output,unsigned char *input,unsigned int len) {

	unsigned int i = 0,j = 0;
	while(j < len) {
		output[i] = (input[j]) | (input[j+1] << 8) | (input[j+2] << 16) | (input[j+3] << 24);
		i++;
		j+=4;
	}
}

void MD5Transform(unsigned int state[4],unsigned char block[64]) {

	unsigned int a = state[0];
	unsigned int b = state[1];
	unsigned int c = state[2];
	unsigned int d = state[3];
	unsigned int x[64];

	MD5Decode(x,block,64);

	FF(a, b, c, d, x[ 0], 7, 0xd76aa478);
	FF(d, a, b, c, x[ 1], 12, 0xe8c7b756);
	FF(c, d, a, b, x[ 2], 17, 0x242070db);
	FF(b, c, d, a, x[ 3], 22, 0xc1bdceee);
	FF(a, b, c, d, x[ 4], 7, 0xf57c0faf);
	FF(d, a, b, c, x[ 5], 12, 0x4787c62a);
	FF(c, d, a, b, x[ 6], 17, 0xa8304613);
	FF(b, c, d, a, x[ 7], 22, 0xfd469501);
	FF(a, b, c, d, x[ 8], 7, 0x698098d8);
	FF(d, a, b, c, x[ 9], 12, 0x8b44f7af);
	FF(c, d, a, b, x[10], 17, 0xffff5bb1);
	FF(b, c, d, a, x[11], 22, 0x895cd7be);
	FF(a, b, c, d, x[12], 7, 0x6b901122);
	FF(d, a, b, c, x[13], 12, 0xfd987193);
	FF(c, d, a, b, x[14], 17, 0xa679438e);
	FF(b, c, d, a, x[15], 22, 0x49b40821);

	GG(a, b, c, d, x[ 1], 5, 0xf61e2562);
	GG(d, a, b, c, x[ 6], 9, 0xc040b340);
	GG(c, d, a, b, x[11], 14, 0x265e5a51);
	GG(b, c, d, a, x[ 0], 20, 0xe9b6c7aa);
	GG(a, b, c, d, x[ 5], 5, 0xd62f105d);
	GG(d, a, b, c, x[10], 9,  0x2441453);
	GG(c, d, a, b, x[15], 14, 0xd8a1e681);
	GG(b, c, d, a, x[ 4], 20, 0xe7d3fbc8);
	GG(a, b, c, d, x[ 9], 5, 0x21e1cde6);
	GG(d, a, b, c, x[14], 9, 0xc33707d6);
	GG(c, d, a, b, x[ 3], 14, 0xf4d50d87);
	GG(b, c, d, a, x[ 8], 20, 0x455a14ed);
	GG(a, b, c, d, x[13], 5, 0xa9e3e905);
	GG(d, a, b, c, x[ 2], 9, 0xfcefa3f8);
	GG(c, d, a, b, x[ 7], 14, 0x676f02d9);
	GG(b, c, d, a, x[12], 20, 0x8d2a4c8a);


	HH(a, b, c, d, x[ 5], 4, 0xfffa3942);
	HH(d, a, b, c, x[ 8], 11, 0x8771f681);
	HH(c, d, a, b, x[11], 16, 0x6d9d6122);
	HH(b, c, d, a, x[14], 23, 0xfde5380c);
	HH(a, b, c, d, x[ 1], 4, 0xa4beea44);
	HH(d, a, b, c, x[ 4], 11, 0x4bdecfa9);
	HH(c, d, a, b, x[ 7], 16, 0xf6bb4b60);
	HH(b, c, d, a, x[10], 23, 0xbebfbc70);
	HH(a, b, c, d, x[13], 4, 0x289b7ec6);
	HH(d, a, b, c, x[ 0], 11, 0xeaa127fa);
	HH(c, d, a, b, x[ 3], 16, 0xd4ef3085);
	HH(b, c, d, a, x[ 6], 23,  0x4881d05);
	HH(a, b, c, d, x[ 9], 4, 0xd9d4d039);
	HH(d, a, b, c, x[12], 11, 0xe6db99e5);
	HH(c, d, a, b, x[15], 16, 0x1fa27cf8);
	HH(b, c, d, a, x[ 2], 23, 0xc4ac5665);


	II(a, b, c, d, x[ 0], 6, 0xf4292244);
	II(d, a, b, c, x[ 7], 10, 0x432aff97);
	II(c, d, a, b, x[14], 15, 0xab9423a7);
	II(b, c, d, a, x[ 5], 21, 0xfc93a039);
	II(a, b, c, d, x[12], 6, 0x655b59c3);
	II(d, a, b, c, x[ 3], 10, 0x8f0ccc92);
	II(c, d, a, b, x[10], 15, 0xffeff47d);
	II(b, c, d, a, x[ 1], 21, 0x85845dd1);
	II(a, b, c, d, x[ 8], 6, 0x6fa87e4f);
	II(d, a, b, c, x[15], 10, 0xfe2ce6e0);
	II(c, d, a, b, x[ 6], 15, 0xa3014314);
	II(b, c, d, a, x[13], 21, 0x4e0811a1);
	II(a, b, c, d, x[ 4], 6, 0xf7537e82);
	II(d, a, b, c, x[11], 10, 0xbd3af235);
	II(c, d, a, b, x[ 2], 15, 0x2ad7d2bb);
	II(b, c, d, a, x[ 9], 21, 0xeb86d391);
	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
}

static void fill_white(void) {

    vm_graphic_color color;
    color.vm_color_565 = VM_COLOR_WHITE;
    vm_graphic_setcolor(&color);
    vm_graphic_fill_rect_ex(layer_hdl[0], 0, 0, nscreen_width, nscreen_height);
    vm_graphic_flush_layer(layer_hdl, 1);
    filledDsplByLines = 0;
}

VMINT string_width(VMWCHAR *whead, VMWCHAR *wtail) {

    VMWCHAR *wtemp = NULL;
    VMINT width = 0;
    if (whead == NULL || wtail == NULL) return 0;
    wtemp = (VMWCHAR *)vm_malloc((wtail - whead) * 2 + 2);
    if (wtemp == NULL) return 0;
    memset(wtemp, 0, (wtail - whead) * 2 + 2);
    memcpy(wtemp, whead, (wtail - whead) * 2);
    width = vm_graphic_get_string_width(wtemp);
    vm_free(wtemp);
    return width;
}

void display_text_line(VMUINT8 *disp_buf, VMSTR str, VMINT x, VMINT y, VMINT width, VMINT height, VMINT betlines, VMINT startLine, VMINT color, VMBOOL fix_pos) {

    VMWCHAR *ucstr;
    VMWCHAR *ucshead;
    VMWCHAR *ucstail;
    VMINT is_end = FALSE;
    VMINT nheight = y;
    VMINT nline_height;
    VMINT nlines = 0;

    if (y == 0) {
       fill_white();
       nheight = nheight + 1;
    }
    if (str == NULL || disp_buf == NULL || betlines < 0) return;

    nline_height = vm_graphic_get_character_height() + betlines;

    if (third == VM_TRUE && fix_pos == VM_FALSE) {
        nheight += nline_height;
        third = VM_FALSE;
    }

    if (fix_pos == VM_TRUE) {
        vm_graphic_fill_rect(disp_buf, 0, filledDsplByLines, nscreen_width, nline_height, VM_COLOR_WHITE, VM_COLOR_WHITE);
        third = VM_TRUE;
    }

    ucstr = (VMWCHAR*)vm_malloc(2 * (strlen(str) + 1));
    if (ucstr == NULL) return;
    if (0 != vm_ascii_to_ucs2(ucstr, 2 * (strlen(str) + 1), str)) {
        vm_free(ucstr);
        return;
    }

    ucshead = ucstr;
    ucstail = ucshead + 1;

    while (!is_end) {
        //if (nheight > y + height) {
        //if (nheight + y > height) {
        if (nheight > height) {
            fill_white();
            nheight = 1;
        }

        while (1) {
            if (string_width(ucshead, ucstail) <= width) {
                ucstail++;
            } else {
                nlines++;
                ucstail--;
                break;
            }
            if (0 == vm_wstrlen(ucstail)) {
                is_end = TRUE;
                nlines++;
                break;
            }
        }

        if (nlines >= startLine) {
            vm_graphic_textout(disp_buf, x, nheight, ucshead, (ucstail - ucshead), (VMUINT16)(color));
            vm_graphic_flush_layer(layer_hdl, 1);
            if (!fix_pos) nheight += nline_height;
            filledDsplByLines = nheight;
        }

        ucshead = ucstail;
        ucstail++;
    }
    vm_free(ucstr);
}

void save_text(VMINT state, VMWSTR text) {

    VMCHAR tmp[100] = {0};
    VMINT lenght = vm_wstrlen(text);

    unsigned char decrypt[16];
    MD5_CTX md5;
    unsigned char buffer_read[1024];
    int i;


    //if (state == VM_INPUT_OK && lenght > 0) {
    if (state == VM_INPUT_OK) {
        vm_chset_convert(VM_CHSET_UCS2, VM_CHSET_UTF8, (VMSTR)text, tmp, (lenght + 1) * 2);
        MD5Init(&md5);
        MD5Update(&md5, tmp, strlen(tmp));
        MD5Final(&md5, decrypt);

    for (i = 0; i < 16; i++) {
        sprintf(md5_string + (i * 2), "%02x", decrypt[i]);
    }

    md5_string[32] = '\0';

    display_text_line(buffer, md5_string, 0, filledDsplByLines, nscreen_width, nscreen_height, 2, 1, VM_COLOR_BLACK, VM_FALSE);

    } else {
        //vm_exit_app();
    }
}
