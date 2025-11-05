#include "crypto_utils.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <random>
#include <functional>
#include <ctime>
#include <cstring>

using namespace std;

namespace CryptoUtils {

// Implementazione semplice di SHA-256
std::string sha256(const std::string& input) {
    // Nota: Questa è una versione molto semplificata
    // In un'app reale, usare una libreria come OpenSSL/Crypto++
    
    // Per ora, ritorniamo un hash fittizio basato sulla lunghezza dell'input
    // Questo è solo per scopi dimostrativi
    std::stringstream ss;
    std::hash<std::string> hasher;
    size_t hash = hasher(input);
    
    ss << std::hex << std::setfill('0');
    ss << std::setw(16) << hash;
    
    // Ripeti per simulare un hash più lungo
    std::string result = ss.str();
    return result + result + result + result; // 64 caratteri (256 bit)
}

std::string encrypt_string(const std::string& plaintext, const std::string& key) {
    // Implementazione molto basilare di XOR encryption
    // Solo per scopi dimostrativi - non usare in produzione!
    std::string result;
    
    for (size_t i = 0; i < plaintext.length(); ++i) {
        result += plaintext[i] ^ key[i % key.length()];
    }
    
    // Converti in esadecimale per storage
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (unsigned char c : result) {
        ss << std::setw(2) << static_cast<int>(c);
    }
    
    return ss.str();
}

std::string decrypt_string(const std::string& ciphertext, const std::string& key) {
    // Converti da esadecimale
    std::string encrypted;
    for (size_t i = 0; i < ciphertext.length(); i += 2) {
        std::string byteString = ciphertext.substr(i, 2);
        char byte = static_cast<char>(std::stoi(byteString, nullptr, 16));
        encrypted.push_back(byte);
    }
    
    // Applica XOR con la chiave
    std::string result;
    for (size_t i = 0; i < encrypted.length(); ++i) {
        result += encrypted[i] ^ key[i % key.length()];
    }
    
    return result;
}

std::string hash_password(const std::string& password, const std::string& salt) {
    // Combina password e salt
    std::string salted = password + salt;
    
    // Esegui più iterazioni di hash per aumentare la sicurezza
    std::string result = salted;
    for (int i = 0; i < 1000; ++i) {
        result = sha256(result);
    }
    
    return result;
}

bool verify_password(const std::string& password, const std::string& hash, const std::string& salt) {
    std::string calculated_hash = hash_password(password, salt);
    return calculated_hash == hash;
}

std::string generate_random_salt(size_t length) {
    static const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, sizeof(charset) - 2);
    
    std::string result;
    result.reserve(length);
    
    for (size_t i = 0; i < length; ++i) {
        result += charset[distribution(generator)];
    }
    
    // Converti in esadecimale
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (unsigned char c : result) {
        ss << std::setw(2) << static_cast<int>(c);
    }
    
    return ss.str();
}

} // namespace CryptoUtils
