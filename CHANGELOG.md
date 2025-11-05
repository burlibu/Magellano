# CHANGELOG

## Version 0.0.1

#### 12/7/2025

// TODO creare un sistema di salvataggio configurazioni e dati magari con json. C' è il video skyes molto utile
// TODO spostare le variabili del progetto dal main a un file apposito

---

## Version 0.0.2

#### 13/7/2025

- Implementate funzioni di supporto, creati file di environment e settings, corretti bug minori.
- Dato una struttura più robusta al progetto
- Avviata documentazione funzioni con Doxygen e creato Doxygen docs in docs/

---

## Version 0.0.3

#### 16/7/2025

- Ristrutturato tutto il codice attraverso classi
- TODO: Creare un metodo per calcolare quali pixel sono occupati da delle finestre in runtime in modo da
  riuscire a creare finestre dinamicamente
- TODO: All' avvio mostrare solo il login e mostrare le altre finestre dopo il login.

---

## Version 0.0.4

### 5/8/2025

- Sistemato l' intero schermo coprendolo con delle finestre

## Version 0.0.5

### 3/9/2025

- Definito lo scope dell' applicazione: hacking and cybersecurity
- Aggiunta la gestione degli attacchi nel GUI
- Aggiunta la visualizzazione della cronologia degli attacchi tramite un sistema di scrittura e lettura di un file json
- Aggiunta dipendenza nlohmann/json per la gestione dei file json
- Aggiunti errori per ip e port
- Fixed Help tab
- Cambiato nome del progetto in Magellano
- todo da sistemare il titolo in alto della finestra principale in magellano

## Version 0.0.6

### 11/5/2025

- Risistemata directory
- Sistemato il Makefile
- Sistemato bug di non-resize delle finestre. Bisognava settare imguiCond_Always
- Non Testata profondamente
- Implementato colore frame rosso -> TODO creare sistema di temi personalizzati
- TODO risolvere bug che quando clicchi su save settings porta a 1920x1080 anche quando non lo hai selezionato
