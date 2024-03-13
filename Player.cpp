#include <iostream>
#include "Creature.cpp"
#include "string"
#include <synchapi.h>
#include <fstream>

using std::string;

class Player {
private:
    Creature playersDraft[6];
    string name;
    int round = 1;
    int currentCreatureID = 0;//indeks obecnego stworka w drużynie
public:
    Player() {

    }

    Player(string name) {
        this->name = name;

    }
    void setName(string name) {
        this->name = name;
    }

    int getRound() {
        return round;
    }

    void setRound(int round) {
        this->round = round;
    }

    /**
     * Zwraca drużycie gracza bazowe parametry(uwzględniono evolujce)
     */
    void resetDraft() {
        for (int i = 0; i < 6; ++i) {
            playersDraft[i].backToDefoultValue();
        }
    }

    /**
     * Obniża CD(cooldown) całej drużyny gracza
     */
    virtual void decCD() {
        for (int i = 0; i < 6; i++) {
            playersDraft[i].ultimateCDDesc();
        }
    }

    void incRound() {
        round++;
    }

    int getcurrentCreatureId() {
        return currentCreatureID;
    }

    void setCurrentCreatureID(int newID) {
        currentCreatureID = newID;
    }

    string getName() {
        return name;
    }

    virtual Creature* getCurrentCreature() {
        return &playersDraft[currentCreatureID];
    }

    virtual Creature* getAllCreatures() {
        return &playersDraft[0];
    }

    /**
     * Drukuje imiona stworków w drużynie i ich parametry
     */
    void printACurrentDraft() {

        cout << "===================================================================================" << endl;
        for (int i = 0; i < 6; ++i) {
            cout << "Nr " << (i) << " - " << playersDraft[i].getName() << "{HP: " << playersDraft[i].getHp() << " AGL: "
                 << playersDraft[i].getAgl() << " POW: " << playersDraft[i].getPow() << "}" << "||";
            if ((i + 1) % 2 == 0) {
                cout << endl;
                cout << "===================================================================================" << endl;
            }
        }
    }

    /**Zmienia index wskazujący na obecnego stworka
     *
     * @param newCreatureID indeks nowego obecnego stworka
     */
    void swapCreature(int newCreatureID) {
        cout << "Dokonano zmiany z " << getCurrentCreature()->getName() << " na ";
        currentCreatureID = newCreatureID;
        cout << getCurrentCreature()->getName() << endl;
    }

    /**
     * Zmienia index wskazujący na obecnego stworka w przypadku ggy ten jest martwy
     */
    virtual void findNextAlive() {
        if (!getCurrentCreature()->isAlive()) {
            for (int i = 0; i < 6; i++) {
                if (playersDraft[i].isAlive()) {
                    setCurrentCreatureID(i);
                    break;
                }
            }
        }
    }

    /**
     * Zapisuje dane z klasy Player do pliku
     */
    virtual void save() {
        ofstream file;
        file.open("Player.txt");

        if (file.is_open()) {

            file << this->getName() << " ";
            for (int i = 0; i < 6; ++i) {
                file << this->getAllCreatures()[i].getPow() << " " << this->getAllCreatures()[i].getAgl() << " " << this->getAllCreatures()[i].getHp() << " " <<
                     this->getAllCreatures()[i].getID() << " " << this->getAllCreatures()[i].getName() << " " << this->getAllCreatures()[i].getNameOfElement() << std::endl;
            }
        }


        file.close();
    }

    /**
     * Wczytuje dane z Pliku
     */
    virtual void load() {
        std::ifstream file;
        string playersName = " ";
        int pow = 0;
        int agl = 0;
        int hp = 0;
        int id = 0;
        string name = " ";
        string element;
        Creature tmp;
        file.open("Player.txt");
        file >> playersName;
        this->setName(playersName);
        for (int i = 0; i < 6; ++i) {
            file >> pow >> agl >> hp >> id >> name >> element;
            cout << pow << endl;
            this->getAllCreatures()[i] = Creature(pow, agl, hp, id, name, this->getElementByName(element));
        }
        file.close();
    }

    /**
     * Umożliwia dokonanie wyboru 6-ciu stworków z 18 możliwych
     */
    virtual void makeADraft() {

        string wybranyStworek = " ";
        for (size_t i = 0; i < 6; i++) {
            cout << "Wybierz nastepnego stworka wpisujac jego numer" << endl;
            printStatsOfCreatures();
            cout << "Dla wyswietlenia informacji o wszystkich stworkach wpisz \"-h\"" << endl;
            cin >> wybranyStworek;
            if (wybranyStworek == "-h") {
                printADraftInfo();
            }
            else {
                playersDraft[i] = draftedCreature(wybranyStworek);
                if (playersDraft[i].getName() == "ERROR") {
                    cout << "Blad, sprobuj ponownie";
                    i--;
                }
                else {
                    cout << playersDraft[i].getName() << " zostaw dodany do twojej druzyny." << endl;
                }
            }


        }
    }

    /**
     * Drukuje infirmacje o wszystkich stworkach
     */
    void printStatsOfCreatures() {

        cout << "===================================================================================" << endl;
        for (int i = 1; i <= 18; ++i) {
            Creature tmp = draftedCreature(to_string(i));
            cout << "Nr " << i << " - " << tmp.getName() << "{HP: " << tmp.getHp() << " AGL: " << tmp.getAgl() <<
                 " POW: " << tmp.getPow() << "}" << "||";
            if (i % 2 == 0) {
                cout << endl;
                cout << "===================================================================================" << endl;
            }
        }
    }

    /**
     * Drukuje dokadną informację o wszystkich stworkach
     */
    void printADraftInfo() {
        for (int i = 1; i <= 18; ++i) {
            Creature tmp = draftedCreature(to_string(i));
            cout << "Stworek Nr " << i << " ===========================================" << endl;
            tmp.toString();
        }

    }

    /**Zwraca kopię żądanego stworka
     *
     * @param wybranyStworek identyfikator wybranego stworka
     * @return kopia zfabrykowanego stworka
     */
    Creature draftedCreature(string wybranyStworek) {
        if (wybranyStworek.compare("1") == 0) {
            Creature fisherTiger = Creature(5, 22, 15, 0, "FisherTiger", WODA);
            return fisherTiger;
        }
        else if (wybranyStworek.compare("2") == 0) {
            Creature neptune = Creature(6, 21, 16, 1, "Neptune", WODA);
            return neptune;
        }
        else if (wybranyStworek.compare("3") == 0) {
            Creature whaleSally = Creature(5, 22, 16, 2, "WhaleSally", WODA);
            return whaleSally;
        }
        else if (wybranyStworek.compare("4") == 0) {
            Creature obsi = Creature(7, 20, 17, 3, "Obsi", ZIEMIA);
            return obsi;
        }
        else if (wybranyStworek.compare("5") == 0) {
            Creature yamato = Creature(8, 20, 17, 4, "Yamato", ZIEMIA);
            return yamato;
        }
        else if (wybranyStworek.compare("6") == 0) {
            Creature brick = Creature(7, 23, 16, 5, "Brick", ZIEMIA);
            return brick;
        }
        else if (wybranyStworek.compare("7") == 0) {
            Creature musson = Creature(5, 22, 16, 6, "Musson", POWIETRZE);
            return musson;
        }
        else if (wybranyStworek.compare("8") == 0) {
            Creature kirichin = Creature(6, 23, 16, 7, "Kirichin", POWIETRZE);
            return kirichin;
        }
        else if (wybranyStworek.compare("9") == 0) {
            Creature valkiry = Creature(5, 22, 15, 8, "Valkiry", POWIETRZE);
            return valkiry;
        }
        else if (wybranyStworek.compare("10") == 0) {
            Creature phoenix = Creature(6, 22, 15, 9, "Phoenix", OGIEN);
            return phoenix;
        }
        else if (wybranyStworek.compare("11") == 0) {
            Creature spark = Creature(7, 22, 14, 10, "Spark", OGIEN);
            return spark;
        }
        else if (wybranyStworek.compare("12") == 0) {
            Creature michael = Creature(6, 20, 15, 11, "Michael", OGIEN);
            return michael;
        }
        else if (wybranyStworek.compare("13") == 0) {
            Creature jotun = Creature(7, 19, 16, 12, "Jotun", LOD);
            return jotun;
        }
        else if (wybranyStworek.compare("14") == 0) {
            Creature bars = Creature(5, 20, 18, 13, "Bars", LOD);
            return bars;
        }
        else if (wybranyStworek.compare("15") == 0) {
            Creature serpent = Creature(5, 20, 16, 14, "Serpent", LOD);
            return serpent;
        }
        else if (wybranyStworek.compare("16") == 0) {
            Creature ferum = Creature(6, 22, 15, 15, "Ferum", STAL);
            return ferum;
        }
        else if (wybranyStworek.compare("17") == 0) {
            Creature chrom = Creature(6, 20, 15, 16, "Chrom", STAL);
            return chrom;
        }
        else if (wybranyStworek.compare("18") == 0) {
            Creature nikel = Creature(5, 22, 15, 17, "Nikel", STAL);
            return nikel;
        }
        else {
            Creature ErrorFlagCreature = Creature(1, 1, 1, 1, "ERROR", STAL);
            return ErrorFlagCreature;
        }
    }



    /**Pobiera nazwę elementu i zwraca odpowiednią wartość enum
     *
     * @param element nazwa elementu
     * @return wartość enum
     */
    Element getElementByName(const string& element) {
        Element res;
        if (element == "WODA") {
            res = WODA;
        }
        else if (element == "OGIEN") {
            res = OGIEN;
        }
        else if (element == "POWIETRZE") {
            res = POWIETRZE;
        }
        else if (element == "LOD") {
            res = LOD;
        }
        else if (element == "STAL") {
            res = STAL;
        }
        else if (element == "ZIEMIA") {
            res = ZIEMIA;
        }

        return res;
    }

    /**Sprawdza drużyne pod kątem pozostałych czlonków
     *
     * @return flaga tego że wszystkie stworzenia są martwe
     */
    virtual bool allAreDead(){
        for (int i = 0; i < 6; ++i) {
            if(getAllCreatures()[i].isAlive()){
                return false;
            }
        }

        return true;
    }

    /**
    * czysta funkcja virtualna
    * @return rozmiar drużyny
    */
    virtual size_t getSize() {
        return 0;
    };
    /**Czysta funkcja virtualna
     *
     * @param player wskaźnik do objectu gracza
     */
    virtual void enemyTurn(Player* player) {
    }

};

class Enemy : public Player {
private:
    Creature lvl1[4];
    Creature lvl2[6];
    Creature lvl3[8];
    Creature lvl4[10];
    int lvl1Size = 4;
    int lvl2Size = 6;
    int lvl3Size = 8;
    int lvl4Size = 10;

public:
    Enemy() {

    }

    /**
     * Tworzy 4 drużyny przeciwnika(po 1 na każdą runde)
     */
    virtual void makeADraft() {
        int draftedCreature = 0;
        srand((unsigned)time(NULL));


        for (size_t i = 0; i < 10; i++) {
            if (i < 4) {
                draftedCreature = 1 + (rand() % 18);
                lvl1[i] = this->draftedCreature(std::to_string(draftedCreature));
            }

            if (i < 6) {
                draftedCreature = 1 + (rand() % 18);
                lvl2[i] = this->draftedCreature(std::to_string(draftedCreature));
            }

            if (i < 8) {
                draftedCreature = 1 + (rand() % 18);
                lvl3[i] = this->draftedCreature(std::to_string(draftedCreature));
            }

            if (i < 10) {
                draftedCreature = 1 + (rand() % 18);
                lvl4[i] = this->draftedCreature(std::to_string(draftedCreature));
            }

        }
    }


/**Zwraca rozmiar drużyny w zależności od rundy
 *
 * @return rozmiar drużyny
 */
    virtual size_t getSize() {
        if (getRound() == 1) {
            return lvl1Size;
        }
        else if (getRound() == 2) {
            return lvl2Size;
        }
        else if (getRound() == 3) {
            return lvl3Size;
        }
        else if (getRound() == 4) {
            return lvl4Size;
        }
    }

    virtual Creature* getCurrentCreature() {
        return &getAllCreatures()[getcurrentCreatureId()];
    }

    /**Zwraca wskaźnik do drużyny w zależności od rundy
     *
     * @return wskaźnik do drużyny
     */
    virtual Creature* getAllCreatures() {
        if (getRound() == 1) {
            return &lvl1[0];
        }
        else if (getRound() == 2) {
            return &lvl2[0];
        }
        else if (getRound() == 3) {
            return &lvl3[0];
        }
        else if (getRound() == 4) {
            return &lvl4[0];
        }
    }

    /**
     * zmienia ideks obecnego stworka o ilie ten jest martwy
     */
    virtual void findNextAlive() {

        if (!getCurrentCreature()->isAlive()) {
            for (int i = 0; i < getSize(); i++) {
                if (getAllCreatures()[i].isAlive()) {
                    setCurrentCreatureID(i);
                    break;
                }
            }
        }
    }

    /**Odpowiada za podjęcie decyzji przez przeciwnika o tym w jaki sposób dokonać zamiany
     *
     * @return nowy indeks obecnego stworka
     */
    int makeASwapDecision() {

        for (int i = 0; i < getSize(); i++) {
            if (this->getAllCreatures()[i].getHp() > 10) {
                return i;
            }
        }

        for (int i = 0; i < 4; i++) {
            if (getAllCreatures()[i].isAlive()) {
                return i;
            }
        }

    }

    /**Odpowiada za podjęcie decyzji co do następnego ruchu(uwzględniono skalowanie poziomu trudności w zależności od rundy)
     *
     * @return flaga podjętej decyzji
     */
    int makeADecision() {
        srand((unsigned)time(NULL));

        if (getRound() == 1) {
            if (getCurrentCreature()->getUltimateCD() == 0) {
                return (1 + (rand() % 3));
            }
            else {
                return (1 + (rand() % 2));
            }
        }
        else if (getRound() == 2) {
            if (this->getCurrentCreature()->getXp() >= 100) {
                return 4;
            }
            else {
                if (getCurrentCreature()->getUltimateCD() == 0) {
                    return (1 + (rand() % 3));
                }
                else {
                    return (1 + (rand() % 2));
                }
            }
        }
        else if (getRound() == 3) {
            if (getCurrentCreature()->getHp() <= 10 && makeASwapDecision() != -1) {
                return 2;
            }
            else if (this->getCurrentCreature()->getXp() >= 100) {
                return 4;
            }
            else {
                if (getCurrentCreature()->getUltimateCD() == 0) {
                    return (1 + (rand() % 3));
                }
                else {
                    return (1 + (rand() % 2));
                }
            }
        }
        else if (getRound() == 4) {
            if (getCurrentCreature()->getHp() <= 10 && makeASwapDecision() != -1) {
                return 2;
            }
            else if (this->getCurrentCreature()->getXp() >= 100) {
                return 4;
            }
            else {

                if (getCurrentCreature()->getUltimateCD() == 0) {
                    return (1 + (rand() % 3));
                }
                else {
                    return (1 + (rand() % 2));
                }
            }
        }
    }

    /**Implementacja ruchu Przeciwnika
     *
     * @param player wskaźnik do Gracza
     */
    virtual void enemyTurn(Player* player) {
        srand((unsigned)time(NULL));

        int nextMove = makeADecision();
        Sleep(1000);

        if (nextMove == 1) {
            getCurrentCreature()->attack(player->getCurrentCreature());
        }
        else if (nextMove == 2) {

            swapCreature(makeASwapDecision());
        }
        else if (nextMove == 3) {
            cout << "Przeciwnik uzywa zdolnosci ultymatywnej." << endl;
            int ultimateChoice = 1 + (rand() % 2);
            getCurrentCreature()->ultimate(player->getAllCreatures(), getSize(), getcurrentCreatureId(), ultimateChoice);
        }
        else if (nextMove == 4) {
            getCurrentCreature()->evolve();
        }
        Sleep(1000);
    };

    /**
     * Zmniejsza CD(cooldown) stworków przeciwnika
     */
    virtual void decCD() {
        for (int i = 0; i < getSize(); i++) {
            getAllCreatures()[i].ultimateCDDesc();
        }
    }

    /**
     * Zapisywanie danych przeciwnika do pliku
     */
    virtual void save() {
        std::ofstream file;

        file.open("EnemyCreatures.txt");
        file << this->getRound() << " ";
        for (int i = 0; i < 10; ++i) {
            if (i < 4) {
                file << this->lvl1[i].getPow() << " " << this->lvl1[i].getAgl() << " " << this->lvl1[i].getHp() << " " <<
                     this->lvl1[i].getID() << " " << this->lvl1[i].getName() << " " << this->lvl1[i].getNameOfElement() << std::endl;
            }

            if (i < 6) {
                file << this->lvl2[i].getPow() << " " << this->lvl2[i].getAgl() << " " << this->lvl2[i].getHp() << " " <<
                     this->lvl2[i].getID() << " " << this->lvl2[i].getName() << " " << this->lvl2[i].getNameOfElement() << std::endl;
            }

            if (i < 8) {
                file << this->lvl3[i].getPow() << " " << this->lvl3[i].getAgl() << " " << this->lvl3[i].getHp() << " " <<
                     this->lvl3[i].getID() << " " << this->lvl3[i].getName() << " " << this->lvl3[i].getNameOfElement() << std::endl;
            }

            if (i < 10) {
                file << this->lvl4[i].getPow() << " " << this->lvl4[i].getAgl() << " " << this->lvl4[i].getHp() << " " <<
                     this->lvl4[i].getID() << " " << this->lvl4[i].getName() << " " << this->lvl4[i].getNameOfElement() << std::endl;
            }

        }
        cout << "Stan gry został zapisany."<<endl;
        file.close();
    }

    /**
     * Wczytywanie danych o przeciwnika z pliku
     */
    virtual void load() {
        std::ifstream file;
        int round = 0;
        int pow = 0;
        int agl = 0;
        int hp = 0;
        int id = 0;
        string name = " ";
        string element;
        Creature tmp;
        file.open("EnemyCreatures.txt");
        file >> round;
        this->setRound(round);
        for (int i = 0; i < 10; ++i) {
            if (i < 4) {
                file >> pow >> agl >> hp >> id >> name >> element;
                this->lvl1[i] = Creature(pow, agl, hp, id, name, this->getElementByName(element));
            }

            if (i < 6) {
                file >> pow >> agl >> hp >> id >> name >> element;
                this->lvl2[i] = Creature(pow, agl, hp, id, name, this->getElementByName(element));
            }

            if (i < 8) {
                file >> pow >> agl >> hp >> id >> name >> element;
                this->lvl3[i] = Creature(pow, agl, hp, id, name, this->getElementByName(element));
            }

            if (i < 10) {
                file >> pow >> agl >> hp >> id >> name >> element;
                this->lvl4[i] = Creature(pow, agl, hp, id, name, this->getElementByName(element));
            }

        }
        file.close();
    }

    /**Sprawdza drużyne pod kątem pozostałych czlonków
     *
     * @return flaga tego że wszystkie stworzenia są martwe
     */
    virtual bool allAreDead(){
        for (int i = 0; i < getSize(); ++i) {
            if(getAllCreatures()[i].isAlive()){
                return false;
            }
        }

        return true;
    }


};
