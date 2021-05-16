#include <array>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>

#include <openssl/evp.h>
#include <openssl/md5.h>

using namespace std;

struct Deleter {
  void operator()(EVP_MD_CTX* ctx) {
    if (ctx == nullptr) return;
    EVP_MD_CTX_free(ctx);
  }
};

int main(int argc, char** argv) {
  int ret = 1;

  unique_ptr<EVP_MD_CTX, Deleter> ctx(EVP_MD_CTX_new());

  ret = EVP_DigestInit_ex(ctx.get(), EVP_md5(), NULL);
  if (ret != 1) return -1;

  ret = EVP_DigestUpdate(ctx.get(), "hello world", 11);
  if (ret != 1) return -1;

  std::array<unsigned char, MD5_DIGEST_LENGTH> md5;
  unsigned int len;
  ret = EVP_DigestFinal_ex(ctx.get(), md5.data(), &len);
  if (ret != 1) return -1;

  assert(len == MD5_DIGEST_LENGTH);
  stringstream str;
  for (auto c : md5) str << hex << setw(2) << setfill('0') << (int)c;
  assert("5eb63bbbe01eeed093cb22bb8f5acdc3" == str.str());

  return 0;
}