/**
 *
 *
 *
 *  test_convex_utils_hex_to_bytes
 *
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "convex_utils.h"

const char *DATA_HEX = "5288fec4153b702430771dfac8aed0b21cafca4344dae0d47b97f0bf532b3306";
const unsigned char DATA_BYTES[] = {
    0x52, 0x88, 0xfe, 0xc4, 0x15, 0x3b, 0x70, 0x24, 0x30, 0x77, 0x1d, 0xfa, 0xc8, 0xae, 0xd0, 0xb2,
    0x1c, 0xaf, 0xca, 0x43, 0x44, 0xda, 0xe0, 0xd4, 0x7b, 0x97, 0xf0, 0xbf, 0x53, 0x2b, 0x33, 0x06,
};

int main() {
    char data_buffer[32];
    int data_buffer_length = 32;
    int result;

    result = convex_utils_hex_to_bytes(DATA_HEX, data_buffer, &data_buffer_length);
    assert(result == CONVEX_OK);

    assert(memcmp(data_buffer, DATA_BYTES, sizeof(DATA_BYTES)) == 0);

    return 0;
}
