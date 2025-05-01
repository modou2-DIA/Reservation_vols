 
 
# ✈️ Gestion de Réservations de Vol – Projet en C

Ce projet consiste à développer une application en langage C permettant à des agences de voyage de se connecter à un serveur central pour effectuer des **réservations** ou **annulations** de billets d’avion. Le système repose sur une architecture **client-serveur** utilisant les sockets TCP.

---

## 📁 Structure du projet

 ```bash
gestion-reservations-vols/
│
├── src/              # Fichiers source (.c)
│   ├── client.c
│   ├── serveur.c
│   ├── utils.c
│
├── include/          # Fichiers d'en-tête (.h)
│   ├── client.h
│   ├── serveur.h
│   ├── utils.h
│
├── data/             # Fichiers de données
│   ├── vols.txt          # Liste des vols disponibles
│   ├── histo.txt         # Historique des transactions
│   ├── facture.txt       # Facture générée par l’agence
│
├── doc/              # Documentation et spécifications
│   ├── rapport.pdf
│   ├── diagrammes/
│
├── Makefile          # Compilation automatisée
├── README.md         # Ce fichier
└── .gitignore
```

 

## 🚀 Fonctionnalités

- 📂 Chargement des données à partir d’un fichier `vols.txt`
- 🤝 Connexion client-serveur via des sockets TCP
- 📝 Enregistrement des réservations/annulations dans `histo.txt`
- 🧾 Génération automatique de facture (`facture.txt`)
- 🔁 Mise à jour en temps réel du nombre de places disponibles

---

## ⚙️ Installation & Compilation

### Prérequis :
- Système Unix/Linux
- Un compilateur C (ex : `gcc`)

### Compilation :

```bash
make all
```

### Exécution :

Dans un terminal (serveur) :
```bash
./serveur
```

Dans un autre terminal (client/agence) :
```bash
./client
```

---

## 📌 Exemples de fichier `vols.txt`

```txt
Vol1|Tunis-Paris|2024-06-10|12:00|150|50
Vol2|Tunis-Marseille|2024-06-12|09:30|120|70
```

Format :
```
<IdentifiantVol>|<Trajet>|<Date>|<Heure>|<PlacesTotales>|<PlacesDisponibles>
```

 
 

 

 
