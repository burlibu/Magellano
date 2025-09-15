# Come Usare le Funzionalità di Crittografia

## Instalazione Crypto++

Assicurati di aver installato Crypto++ con vcpkg:

```
vcpkg install cryptopp
```

## Funzionalità Disponibili

La libreria Crypto++ è stata integrata nel progetto Magellano con le seguenti funzionalità:

### 1. Crittografia e Decrittografia AES

```cpp
// Crittografia
std::string encrypted = CryptoUtils::encrypt_string("testo da criptare", "chiave_segreta");

// Decrittografia
std::string decrypted = CryptoUtils::decrypt_string(encrypted, "chiave_segreta");
```

### 2. Hash di Password (PBKDF2 con SHA-256)

```cpp
// Generare un salt casuale
std::string salt = CryptoUtils::generate_random_salt();

// Creare hash di una password
std::string hashed_password = CryptoUtils::hash_password("password_utente", salt);

// Verificare una password
bool is_valid = CryptoUtils::verify_password("password_da_verificare", hashed_password, salt);
```

## Implementazione Login Sicuro

Il sistema di login è stato migliorato per utilizzare hash delle password invece di memorizzare password in chiaro:

1. **Registrazione utente**:
   - Generare un salt casuale per ogni utente
   - Calcolare l'hash della password con il salt
   - Memorizzare username, salt e hash

2. **Verifica login**:
   - Recuperare salt e hash associati all'username
   - Calcolare l'hash della password fornita con lo stesso salt
   - Confrontare l'hash calcolato con quello memorizzato

## Esempio di Crittografia File

Per criptare/decriptare un file JSON con dati sensibili:

```cpp
// Leggi il contenuto del file
std::ifstream file("data.json");
std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
file.close();

// Cripta
std::string encrypted = CryptoUtils::encrypt_string(content, "chiave_segreta");

// Salva il contenuto criptato
std::ofstream encrypted_file("data.json.enc");
encrypted_file << encrypted;
encrypted_file.close();

// Per decriptare
std::ifstream enc_file("data.json.enc");
std::string encrypted_content((std::istreambuf_iterator<char>(enc_file)), std::istreambuf_iterator<char>());
enc_file.close();

std::string decrypted = CryptoUtils::decrypt_string(encrypted_content, "chiave_segreta");
```

## Considerazioni sulla Sicurezza

- La chiave di crittografia dovrebbe essere protetta e non hard-coded
- Considera l'uso di un key management system per progetti di produzione
- Ogni utente dovrebbe avere un salt unico per l'hash della password
- Usa sempre connessioni sicure (HTTPS) quando trasmetti dati sensibili
