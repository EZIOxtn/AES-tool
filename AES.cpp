#include <iostream>
#include <openssl/aes.h>
#include <openssl/md5.h>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <stdexcept>

std::string base64_encode(const std::string &in) {
    std::string out;
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    BIO_push(b64, bio);
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(b64, in.data(), in.size());
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bufferPtr);
    out.assign(bufferPtr->data, bufferPtr->length);
    BIO_free_all(b64);

    return out;
}

std::vector<unsigned char> create_key(const std::string &pass) {
    unsigned char hash[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(pass.c_str()), pass.size(), hash);
    std::vector<unsigned char> key(16);
    std::memcpy(key.data(), hash, 16);
    return key;
}

std::string aes_encrypt(const std::string &input, const std::string &pass) {
    AES_KEY encryptKey;
    std::vector<unsigned char> key = create_key(pass);
    AES_set_encrypt_key(key.data(), 128, &encryptKey);

    std::string padded_input = input;
    int padding_length = AES_BLOCK_SIZE - (input.size() % AES_BLOCK_SIZE);
    padded_input.append(padding_length, padding_length);

    std::vector<unsigned char> encrypted(padded_input.size());
    AES_encrypt(reinterpret_cast<const unsigned char*>(padded_input.c_str()), encrypted.data(), &encryptKey);

    return base64_encode(std::string(encrypted.begin(), encrypted.end()));
}

std::string aes_decrypt(const std::string &input, const std::string &pass) {
    AES_KEY decryptKey;
    std::vector<unsigned char> key = create_key(pass);
    AES_set_decrypt_key(key.data(), 128, &decryptKey);

    std::string decoded = input; 
    std::vector<unsigned char> decrypted(decoded.size());
    AES_decrypt(reinterpret_cast<const unsigned char*>(decoded.c_str()), decrypted.data(), &decryptKey);

    return std::string(decrypted.begin(), decrypted.end());
}

int main() {
    std::string text, password;
    std::cout << "Enter text to encrypt: ";
    std::getline(std::cin, text);
    std::cout << "Enter password: ";
    std::getline(std::cin, password);

    if (text.empty() || password.empty()) {
        std::cout << "Text or password cannot be empty." << std::endl;
    } else {
        std::string encrypted_text = aes_encrypt(text, password);
        std::cout << "Encrypted text: " << encrypted_text << std::endl;

        std::string decrypted_text = aes_decrypt(encrypted_text, password);
        std::cout << "Decrypted text: " << decrypted_text << std::endl;
    }

    return 0;
}
