#include "fastpbkdf2.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

typedef void (*pbkdf2_fn)(const uint8_t *pw, size_t npw,
                          const uint8_t *salt, size_t nsalt,
                          uint32_t iterations,
                          uint8_t *out, size_t nout);

static void check(pbkdf2_fn fn,
                  const void *pw, size_t npw,
                  const void *salt, size_t nsalt,
                  unsigned iterations,
                  const void *expect, size_t nexpect)
{
  uint8_t out[128];
  assert(nexpect < sizeof(out));

  fn(pw, npw,
     salt, nsalt,
     iterations,
     out, nexpect);

  assert(memcmp(expect, out, nexpect) == 0);
  printf("- test passed\n");
}

int main(void)
{
  /* SHA1 tests from RFC6070 */
  printf("sha1:\n");
  check(fastpbkdf2_hmac_sha1,
        "password", 8,
        "salt", 4,
        1,
        "\x0c\x60\xc8\x0f\x96\x1f\x0e\x71\xf3\xa9\xb5\x24\xaf\x60\x12\x06\x2f\xe0\x37\xa6", 20);

  check(fastpbkdf2_hmac_sha1,
        "password", 8,
        "salt", 4,
        2,
        "\xea\x6c\x01\x4d\xc7\x2d\x6f\x8c\xcd\x1e\xd9\x2a\xce\x1d\x41\xf0\xd8\xde\x89\x57", 20);

  check(fastpbkdf2_hmac_sha1,
        "password", 8,
        "salt", 4,
        4096,
        "\x4b\x00\x79\x01\xb7\x65\x48\x9a\xbe\xad\x49\xd9\x26\xf7\x21\xd0\x65\xa4\x29\xc1", 20);

  check(fastpbkdf2_hmac_sha1,
        "password", 8,
        "salt", 4,
        16777216,
        "\xee\xfe\x3d\x61\xcd\x4d\xa4\xe4\xe9\x94\x5b\x3d\x6b\xa2\x15\x8c\x26\x34\xe9\x84", 20);

  check(fastpbkdf2_hmac_sha1,
        "passwordPASSWORDpassword", 24,
        "saltSALTsaltSALTsaltSALTsaltSALTsalt", 36,
        4096,
        "\x3d\x2e\xec\x4f\xe4\x1c\x84\x9b\x80\xc8\xd8\x36\x62\xc0\xe4\x4a\x8b\x29\x1a\x96\x4c\xf2\xf0\x70\x38", 25);

  check(fastpbkdf2_hmac_sha1,
        "\x70\x61\x73\x73\x00\x77\x6f\x72\x64", 9,
        "\x73\x61\x00\x6c\x74", 5,
        4096,
        "\x56\xfa\x6a\xa7\x55\x48\x09\x9d\xcc\x37\xd7\xf0\x34\x25\xe0\xc3", 16);

  /* SHA256 tests from assorted places:
   * https://tools.ietf.org/html/draft-josefsson-scrypt-kdf-00#page-10
   * http://stackoverflow.com/questions/5130513/pbkdf2-hmac-sha2-test-vectors/
   */
  printf("sha256:\n");
  check(fastpbkdf2_hmac_sha256,
        "passwd", 6,
        "salt", 4,
        1,
        "\x55\xac\x04\x6e\x56\xe3\x08\x9f\xec\x16\x91\xc2\x25\x44\xb6\x05\xf9\x41\x85\x21\x6d\xde\x04\x65\xe6\x8b\x9d\x57\xc2\x0d\xac\xbc\x49\xca\x9c\xcc\xf1\x79\xb6\x45\x99\x16\x64\xb3\x9d\x77\xef\x31\x7c\x71\xb8\x45\xb1\xe3\x0b\xd5\x09\x11\x20\x41\xd3\xa1\x97\x83", 64);

  check(fastpbkdf2_hmac_sha256,
        "Password", 8,
        "NaCl", 4,
        80000,
        "\x4d\xdc\xd8\xf6\x0b\x98\xbe\x21\x83\x0c\xee\x5e\xf2\x27\x01\xf9\x64\x1a\x44\x18\xd0\x4c\x04\x14\xae\xff\x08\x87\x6b\x34\xab\x56\xa1\xd4\x25\xa1\x22\x58\x33\x54\x9a\xdb\x84\x1b\x51\xc9\xb3\x17\x6a\x27\x2b\xde\xbb\xa1\xd0\x78\x47\x8f\x62\xb3\x97\xf3\x3c\x8d", 64);

  check(fastpbkdf2_hmac_sha256,
        "password", 8,
        "salt", 4,
        1,
        "\x12\x0f\xb6\xcf\xfc\xf8\xb3\x2c\x43\xe7\x22\x52\x56\xc4\xf8\x37\xa8\x65\x48\xc9\x2c\xcc\x35\x48\x08\x05\x98\x7c\xb7\x0b\xe1\x7b", 32);

  check(fastpbkdf2_hmac_sha256,
        "password", 8,
        "salt", 4,
        2,
        "\xae\x4d\x0c\x95\xaf\x6b\x46\xd3\x2d\x0a\xdf\xf9\x28\xf0\x6d\xd0\x2a\x30\x3f\x8e\xf3\xc2\x51\xdf\xd6\xe2\xd8\x5a\x95\x47\x4c\x43", 32);

  check(fastpbkdf2_hmac_sha256,
        "password", 8,
        "salt", 4,
        4096,
        "\xc5\xe4\x78\xd5\x92\x88\xc8\x41\xaa\x53\x0d\xb6\x84\x5c\x4c\x8d\x96\x28\x93\xa0\x01\xce\x4e\x11\xa4\x96\x38\x73\xaa\x98\x13\x4a", 32);

  check(fastpbkdf2_hmac_sha256,
        "passwordPASSWORDpassword", 24,
        "saltSALTsaltSALTsaltSALTsaltSALTsalt", 36,
        4096,
        "\x34\x8c\x89\xdb\xcb\xd3\x2b\x2f\x32\xd8\x14\xb8\x11\x6e\x84\xcf\x2b\x17\x34\x7e\xbc\x18\x00\x18\x1c\x4e\x2a\x1f\xb8\xdd\x53\xe1\xc6\x35\x51\x8c\x7d\xac\x47\xe9", 40);

  check(fastpbkdf2_hmac_sha256,
        "", 0,
        "salt", 4,
        1024,
        "\x9e\x83\xf2\x79\xc0\x40\xf2\xa1\x1a\xa4\xa0\x2b\x24\xc4\x18\xf2\xd3\xcb\x39\x56\x0c\x96\x27\xfa\x4f\x47\xe3\xbc\xc2\x89\x7c\x3d", 32);

  check(fastpbkdf2_hmac_sha256,
        "password", 8,
        "", 0,
        1024,
        "\xea\x58\x08\x41\x1e\xb0\xc7\xe8\x30\xde\xab\x55\x09\x6c\xee\x58\x27\x61\xe2\x2a\x9b\xc0\x34\xe3\xec\xe9\x25\x22\x5b\x07\xbf\x46", 32);

  check(fastpbkdf2_hmac_sha256,
        "\x70\x61\x73\x73\x00\x77\x6f\x72\x64", 9,
        "\x73\x61\x00\x6c\x74", 5,
        4096,
        "\x89\xb6\x9d\x05\x16\xf8\x29\x89\x3c\x69\x62\x26\x65\x0a\x86\x87", 16);

  /* SHA512 */
  printf("sha512:\n");
  check(fastpbkdf2_hmac_sha512,
        "password", 8,
        "salt", 4,
        1,
        "\x86\x7f\x70\xcf\x1a\xde\x02\xcf\xf3\x75\x25\x99\xa3\xa5\x3d\xc4\xaf\x34\xc7\xa6\x69\x81\x5a\xe5\xd5\x13\x55\x4e\x1c\x8c\xf2\x52", 32);

  check(fastpbkdf2_hmac_sha512,
        "password", 8,
        "salt", 4,
        2,
        "\xe1\xd9\xc1\x6a\xa6\x81\x70\x8a\x45\xf5\xc7\xc4\xe2\x15\xce\xb6\x6e\x01\x1a\x2e\x9f\x00\x40\x71\x3f\x18\xae\xfd\xb8\x66\xd5\x3c", 32);

  check(fastpbkdf2_hmac_sha512,
        "password", 8,
        "salt", 4,
        4096,
        "\xd1\x97\xb1\xb3\x3d\xb0\x14\x3e\x01\x8b\x12\xf3\xd1\xd1\x47\x9e\x6c\xde\xbd\xcc\x97\xc5\xc0\xf8\x7f\x69\x02\xe0\x72\xf4\x57\xb5", 32);

  check(fastpbkdf2_hmac_sha512,
        "passwordPASSWORDpassword", 24,
        "saltSALTsaltSALTsaltSALTsaltSALTsalt", 36,
        1,
        "\x6e\x23\xf2\x76\x38\x08\x4b\x0f\x7e\xa1\x73\x4e\x0d\x98\x41\xf5\x5d\xd2\x9e\xa6\x0a\x83\x44\x66\xf3\x39\x6b\xac\x80\x1f\xac\x1e\xeb\x63\x80\x2f\x03\xa0\xb4\xac\xd7\x60\x3e\x36\x99\xc8\xb7\x44\x37\xbe\x83\xff\x01\xad\x7f\x55\xda\xc1\xef\x60\xf4\xd5\x64\x80\xc3\x5e\xe6\x8f\xd5\x2c\x69\x36", 72);

  printf("all tests passed\n");

  return 0;
}
