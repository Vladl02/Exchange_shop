# Proiect POO - Casa de Schimb Valutar

## a) Enuntul problemei
Compania ExchangePoint administreaza o casa de schimb valutar care lucreaza zilnic cu clienti fizici si cu operatii interne de gestiune. In practica, datele despre cursuri, lichiditati si tranzactii sunt dispersate, iar urmarirea profitului devine dificila atunci cand volumul de operatii creste. Pentru a standardiza modul de lucru, compania doreste o aplicatie C++ in terminal care sa simuleze activitatea reala a casei de schimb.

Casa de schimb opereaza cu o valuta principala (de exemplu RON) si cu mai multe valute secundare (EUR, USD, GBP, MDL). Pentru fiecare valuta secundara se retin, pe date calendaristice, doua valori: pretul de cumparare si pretul de vanzare. Aceste cursuri sunt folosite la schimburile valutare si influenteaza direct profitul companiei.

In aplicatie exista trei roluri: owner, cashier si client. Owner-ul poate administra taxa, poate face depuneri/retrageri si poate analiza profitul brut/net. Cashier-ul poate seta cursuri si poate efectua schimburi. Clientul poate consulta cursuri si poate initia schimburi in baza datelor disponibile in istoric. Fiecare operatie valida trebuie inregistrata ca tranzactie si pastrata pentru consultare ulterioara.

Aplicatia va ilustra urmatoarele functionalitati:
1. initializarea casei de schimb cu valuta principala, owner si cashier
2. adaugarea si actualizarea cursurilor buy/sell pe date calendaristice
3. depunerea si retragerea de lichiditate pe valute
4. efectuarea schimburilor valutare cu validari de curs si lichiditate
5. afisarea informatiilor disponibile pentru fiecare rol (owner, cashier, client)
6. consultarea preturilor de cumparare si vanzare pentru o valuta la data activa
7. calculul profitului brut si al profitului net
8. afisarea istoricului tranzactiilor in functie de rol si interval de date
9. afisarea statisticilor pe tipuri de tranzactii (exchange, deposit, withdraw)
10. persistenta configurarii si a operatiilor in fisier text
11. reincarcarea datelor la rulare si continuarea sesiunilor
12. rulare in doua moduri: interactiv si demonstrativ

---

## b) Modul de rezolvare

### 1. Organizarea codului
1. fisierele `.h` contin declaratii de clase si semnaturi de functii
2. fisierele `.cpp` contin implementarile
3. structura proiectului este impartita in `headers/` si `src/`

### 2. Ierarhii si mostenire
1. ierarhie utilizatori: `User` (baza) -> `Owner`, `Cashier`, `Client`
2. ierarhie tranzactii: `Transaction` (baza) -> `Exchange`, `Deposit`, `Withdraw`
3. clasele baza sunt proprii proiectului, nu derivate din clase predefinite ale domeniului

### 3. Polimorfism, virtuale pure, NVI
1. in `User` exista functii virtuale pure pentru afisare si filtrare specifica rolului
2. in `Transaction` exista virtual pur pentru calculul profitului si pentru `clone()`
3. in `User` este folosita interfata non-virtuala (NVI) pentru obtinerea/afisarea istoricului

### 4. Stocare polimorfica si apel prin pointer la baza
1. `ExchangeShop` stocheaza tranzactiile in `std::vector<std::shared_ptr<Transaction>>`
2. apelurile de comportament se fac prin pointer la baza (`clone`, afisare, profit)

### 5. Copiere corecta
1. `ExchangeShop` are copy constructor cu clonare polimorfica a tranzactiilor
2. operatorul de atribuire este implementat prin copy-and-swap

### 6. Downcast cu sens
1. in statistica tranzactiilor se foloseste `std::dynamic_pointer_cast`
2. downcast-ul separa tranzactiile pe tipuri concrete pentru raportare

### 7. STL modern
1. `std::string` pentru campuri text
2. `std::vector` pentru colectii
3. `std::shared_ptr` pentru gestiune de memorie in ierarhia polimorfica

### 8. Exceptii proprii si tratarea erorilor
1. ierarhie de exceptii: `ExchangeException`, `ValidationException`, `NotFoundException`, `TransactionConstraintException`
2. exceptiile sunt aruncate in validari si reguli de business
3. exceptiile sunt tratate cu `try/catch` in zonele de interactiune (`main`, meniuri, replay, demo)

### 9. Persistenta si scenarii de rulare
1. configurarea initiala si operatiile sunt salvate in fisier text
2. aplicatia poate reincarca operatiile la rulare ulterioara
3. exista mod interactiv (meniu pe roluri) si mod demonstrativ (test functional)


