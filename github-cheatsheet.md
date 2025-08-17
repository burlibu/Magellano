# GitHub & Git Workflow Cheatsheet

## 1. Creazione Repository

- **Browser (GitHub):**
  1. Vai su github.com
  2. Clicca "New repository"
  3. Dai nome, scegli visibilità, crea
  4. Clona in locale: `git clone https://github.com/tuoutente/nomerepo.git`
- **Terminale:**
  1. `git init`
  2. Crea repo su GitHub da browser
  3. Collega: `git remote add origin https://github.com/tuoutente/nomerepo.git`
  4. `git add .`
  5. `git commit -m "Primo commit"`
  6. `git push -u origin master`

## 2. Sincronizzazione

- **Push:** `git push origin branch`
- **Pull:** `git pull origin branch`
- **Fetch:** `git fetch origin`

## 3. Modifica e Commit

- Modifica file
- `git status`
- `git add file`
- `git commit -m "messaggio"`
- `git push`

## 4. Branch Workflow

- Crea branch: `git checkout -b nuovo-branch`
- Switch branch: `git switch branch`
- Elenca branch: `git branch`

## 5. Fork e Collaborazione

- Fai fork da GitHub (browser)
- Clona fork: `git clone https://github.com/tuoutente/nomerepo.git`
- Modifica, commit, push
- Apri pull request dal browser

## 6. Gestione conflitti

- Se durante `git pull` compaiono conflitti, modifica i file segnalati, poi:
  1. `git add file`
  2. `git commit -m "risolvo conflitto"`
  3. `git push`

## 7. Glossario

- **Commit**: Salvataggio modifiche
- **Branch**: “Ramo” di lavoro separato
- **Fork**: Copia indipendente di una repo
- **Push**: Invio modifiche al remoto
- **Pull**: Ricevi modifiche dal remoto
- **Merge**: Unisci modifiche tra branch

## 8. Esempio collaborazione

1. Mario crea repo → Anna fa fork → Anna clona fork
2. Anna modifica, push → Anna apre pull request verso repo di Mario
3. Mario revisiona, fa merge

---

**Diagramma del workflow (testuale):**

```
[Crea repo] → [Clona] → [Modifica] → [Commit] → [Push] → [Pull] → [Fork] → [Pull Request] → [Merge]
```
