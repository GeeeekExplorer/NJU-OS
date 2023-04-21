#include "x86.h"
#include "device.h"

/* Press Scan Code */
#define KESC_P 0x01 // escape
#define K1_P 0x02
#define K2_P 0x03
#define K3_P 0x04
#define K4_P 0x05
#define K5_P 0x06
#define K6_P 0x07
#define K7_P 0x08
#define K8_P 0x09
#define K9_P 0x0a
#define K0_P 0x0b
#define KMIN_P 0x0c // - minus
#define KEQU_P 0x0d  // = equal
#define KBAC_P 0x0e  // backspace
#define KTAB_P 0x0f  // tabular
#define KQ_P 0x10
#define KW_P 0x11
#define KE_P 0x12
#define KR_P 0x13
#define KT_P 0x14
#define KY_P 0x15
#define KU_P 0x16
#define KI_P 0x17
#define KO_P 0x18
#define KP_P 0x19
#define KLBR_P 0x1a // [ left bracket
#define KRBR_P 0x1b // ] right bracket
#define KENT_P 0x1c
#define KLCO_P 0x1d // left ctrl
#define KA_P 0x1e
#define KS_P 0x1f
#define KD_P 0x20
#define KF_P 0x21
#define KG_P 0x22
#define KH_P 0x23
#define KJ_P 0x24
#define KK_P 0x25
#define KL_P 0x26
#define KSCO_P 0x27 // ; semicolon
#define KSQU_P 0x28 // ' single quote
#define KBTI_P 0x29 // ` back tick
#define KLSH_P 0x2a // left shift
#define KBSL_P 0x2b // \ back slash
#define KZ_P 0x2c
#define KX_P 0x2d
#define KC_P 0x2e
#define KV_P 0x2f
#define KB_P 0x30
#define KN_P 0x31
#define KM_P 0x32
#define KCOM_P 0x33 // , comma
#define KDOT_P 0x34 // . full stop
#define KFSL_P 0x35 // / forward slash
#define KRSH_P 0x36 // right shift
#define KKPS_P 0x37 // keypad *
#define KLAL_P 0x38 // left alt
#define KSPA_P 0x39 // space
#define KCAP_P 0x3a // capslock
#define KF1_P 0x3b
#define KF2_P 0x3c
#define KF3_P 0x3d
#define KF4_P 0x3e
#define KF5_P 0x3f
#define KF6_P 0x40
#define KF7_P 0x41
#define KF8_P 0x42
#define KF9_P 0x43
#define KF10_P 0x44
#define KNLO_P 0x45 // numberlock
#define KSLO_P 0x46 // scrolllock
#define KKP7_P 0x47 // keypad 7
#define KKP8_P 0x48 // keypad 8
#define KKP9_P 0x49 // keypad 9
#define KKPM_P 0x4a // keypad -
#define KKP4_P 0x4b // keypad 4
#define KKP5_P 0x4c // keypad 5
#define KKP6_P 0x4d // keypad 6
#define KKPP_P 0x4e // keypad +
#define KKP1_P 0x4f // keypad 1
#define KKP2_P 0x50 // keypad 2
#define KKP3_P 0x51 // keypad 3
#define KKP0_P 0x52 // keypad 0
#define KKPD_P 0x53 // keypad .
#define KF11_P 0x57
#define KF12_P 0x58

/* Release Scan Code = Press Scan Code + 0x80 */

#define KEYTABLE_SIZE (KF12_P+1)

uint32_t keyBuffer[MAX_KEYBUFFER_SIZE];
int bufferHead;
int bufferTail;

static char keyTableL[KEYTABLE_SIZE];
static char keyTableU[KEYTABLE_SIZE];
static char *keyTable;
static int keyboardState;

static uint32_t old_code = 0;

void initKeyTable() {
	int i;

	bufferHead = 0;
	bufferTail = 0;

	keyboardState = 0; // 0: normal state; 1: capslock pressed; 2: capslock pressed again;
	keyTable = keyTableL;

	for (i = 0; i < KEYTABLE_SIZE; i++) {
		keyTableL[i] = 0;
		keyTableU[i] = 0;
	}

	keyTableL[K1_P]='1';    keyTableU[K1_P]='!';
	keyTableL[K2_P]='2';    keyTableU[K2_P]='@';
	keyTableL[K3_P]='3';    keyTableU[K3_P]='#';
	keyTableL[K4_P]='4';    keyTableU[K4_P]='$';
	keyTableL[K5_P]='5';    keyTableU[K5_P]='%';
	keyTableL[K6_P]='6';    keyTableU[K6_P]='^';
	keyTableL[K7_P]='7';    keyTableU[K7_P]='&';
	keyTableL[K8_P]='8';    keyTableU[K8_P]='*';
	keyTableL[K9_P]='9';    keyTableU[K9_P]='(';
	keyTableL[K0_P]='0';    keyTableU[K0_P]=')';
                                                    
	keyTableL[KA_P]='a';    keyTableU[KA_P]='A';
	keyTableL[KB_P]='b';    keyTableU[KB_P]='B';
	keyTableL[KC_P]='c';    keyTableU[KC_P]='C';
	keyTableL[KD_P]='d';    keyTableU[KD_P]='D';
	keyTableL[KE_P]='e';    keyTableU[KE_P]='E';
	keyTableL[KF_P]='f';    keyTableU[KF_P]='F';
	keyTableL[KG_P]='g';    keyTableU[KG_P]='G';
	keyTableL[KH_P]='h';    keyTableU[KH_P]='H';
	keyTableL[KI_P]='i';    keyTableU[KI_P]='I';
	keyTableL[KJ_P]='j';    keyTableU[KJ_P]='J';
	keyTableL[KK_P]='k';    keyTableU[KK_P]='K';
	keyTableL[KL_P]='l';    keyTableU[KL_P]='L';
	keyTableL[KM_P]='m';    keyTableU[KM_P]='M';
	keyTableL[KN_P]='n';    keyTableU[KN_P]='N';
	keyTableL[KO_P]='o';    keyTableU[KO_P]='O';
	keyTableL[KP_P]='p';    keyTableU[KP_P]='P';
	keyTableL[KQ_P]='q';    keyTableU[KQ_P]='Q';
	keyTableL[KR_P]='r';    keyTableU[KR_P]='R';
	keyTableL[KS_P]='s';    keyTableU[KS_P]='S';
	keyTableL[KT_P]='t';    keyTableU[KT_P]='T';
	keyTableL[KU_P]='u';    keyTableU[KU_P]='U';
	keyTableL[KV_P]='v';    keyTableU[KV_P]='V';
	keyTableL[KW_P]='w';    keyTableU[KW_P]='W';
	keyTableL[KX_P]='x';    keyTableU[KX_P]='X';
	keyTableL[KY_P]='y';    keyTableU[KY_P]='Y';
	keyTableL[KZ_P]='z';    keyTableU[KZ_P]='Z';
	
	keyTableL[KMIN_P]='-';  keyTableU[KMIN_P]='_'; 
	keyTableL[KEQU_P]='=';  keyTableU[KEQU_P]='+'; 
	keyTableL[KLBR_P]='[';  keyTableU[KLBR_P]='{'; 
	keyTableL[KRBR_P]=']';  keyTableU[KRBR_P]='}'; 
	keyTableL[KENT_P]='\n'; keyTableU[KENT_P]='\n';
	keyTableL[KSCO_P]=';';  keyTableU[KSCO_P]=':'; 
	keyTableL[KSQU_P]='\'';  keyTableU[KSQU_P]='\"'; 
	keyTableL[KBTI_P]='`';  keyTableU[KBTI_P]='~'; 
	keyTableL[KBSL_P]='\\'; keyTableU[KBSL_P]='|';
	keyTableL[KCOM_P]=',';  keyTableU[KCOM_P]='<'; 
	keyTableL[KDOT_P]='.';  keyTableU[KDOT_P]='>'; 
	keyTableL[KFSL_P]='/';  keyTableU[KFSL_P]='?'; 
	keyTableL[KSPA_P]=' ';  keyTableU[KSPA_P]=' '; 
}

uint32_t getKeyCode() {
	uint32_t code = inByte(0x60);
	uint32_t val = inByte(0x61);
	outByte(0x61, val | 0x80);
	outByte(0x61, val);
	if (code <= (KF12_P+0x80) && code != old_code) { // all 1 byte scan code, press or release
		old_code = code;
        	return code;
	}
	else
		return 0;
}

static inline void switchKeyboard() {
	if (keyTable == keyTableL)
		keyTable = keyTableU;
	else
		keyTable = keyTableL;
}

char getChar(uint32_t code) {
	switch(code) {
		case KLSH_P: // press left shift
		case (KLSH_P+0x80): // release left shift
		case KRSH_P: // press right shift
		case (KRSH_P+0x80): // release right shift
			switchKeyboard();
			break;
		case KCAP_P: // press capslock
			if (keyboardState == 0) {
				keyboardState = 1;
				switchKeyboard();
			}
			else if (keyboardState == 1) {
				keyboardState = 2;
			}
			break;
		case (KCAP_P+0x80): // release capslock
			if (keyboardState == 1) {
				keyboardState = 1;
			}
			else if (keyboardState == 2) {
				keyboardState = 0;
				switchKeyboard();
			}
			break;
		default:
			break;
	}
	if (code >= KF12_P)
		return 0;
	else
		return keyTable[code];
}
