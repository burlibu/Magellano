#pragma once

#include <string>
#include <vector>

namespace CryptoUtils {
    /**
     * @brief Funzione per criptare una stringa con AES
     * @param plaintext Il testo da criptare
     * @param key La chiave di crittografia (deve essere di 16, 24 o 32 byte per AES-128, AES-192 o AES-256)
     * @return La stringa criptata in formato esadecimale
     */
    std::string encrypt_string(const std::string& plaintext, const std::string& key);
    
    /**
     * @brief Funzione per decriptare una stringa con AES
     * @param ciphertext Il testo criptato in formato esadecimale
     * @param key La chiave di crittografia (deve essere uguale a quella usata per criptare)
     * @return La stringa decriptata originale
     */
    std::string decrypt_string(const std::string& ciphertext, const std::string& key);
    
    /**
     * @brief Funzione per generare hash di una password (utile per il login)
     * @param password La password da trasformare in hash
     * @param salt Un valore casuale unico per utente per evitare attacchi rainbow table
     * @return L'hash della password in formato esadecimale
     */
    std::string hash_password(const std::string& password, const std::string& salt);
    
    /**
     * @brief Funzione per verificare una password con il suo hash
     * @param password La password inserita dall'utente
     * @param hash L'hash salvato nel database
     * @param salt Il salt specifico dell'utente
     * @return true se la password è corretta, false altrimenti
     */
    bool verify_password(const std::string& password, const std::string& hash, const std::string& salt);
    
    /**
     * @brief Genera un salt casuale per la sicurezza delle password
     * @param length Lunghezza del salt (default 16 byte)
     * @return Il salt generato in formato esadecimale
     */
    std::string generate_random_salt(size_t length = 16);
}
