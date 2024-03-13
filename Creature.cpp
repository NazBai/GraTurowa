#include <iostream>
#include "Element.cpp"


using namespace std;
using std::string;


class Creature {
private:
    int ID;
    int pow;
    int agl;
    int maxHp;
    int hp;
    int xp;
    int ultimateCD = 0;//czas przełdowania umiejętności CD(cooldown)
    string name;
    bool alive = true;
    Element element;

public:
    Creature(int pow, int agl, int hp, int ID, string name, Element element) {
        this->pow = pow;
        this->agl = agl;
        this->hp = hp;
        this->maxHp = hp;
        this->xp = 0;
        this->ID = ID;
        this->name = name;
        this->element = element;
    }

    Creature() {

    }

    void setPow(int pow) {
        this->pow = pow;
    }

    void setAgl(int agl) {
        this->agl = agl;
    }


    void setHp(int hp) {
        this->hp = hp;
    }

    void setXp(int xp) {
        this->xp = xp;
    }

    bool isAlive() {
        return alive;
    }

    void setIsAlive(bool alive) {
        this->alive = alive;
    }

    void setName(string name) {
        this->name = name;
    }

    int getPow() {
        return pow;
    }

    int getAgl() {
        return agl;
    }

    int getHp() {
        return hp;
    }

    int getXp() const {
        return xp;
    }

    int getID() const {
        return ID;
    }

    int getUltimateCD() const {
        return ultimateCD;
    }

    void ultimateCDDesc() {
        if (ultimateCD != 0) {
            ultimateCD--;
        }
    }

    string getName() {
        return name;
    }

    Element getElement() {
        return element;
    }

    /*!Metoda dla obliczania ostatecznej ilości obrażeń jakie zostaną zadane przeciwnikowi
     * oraz prawdopodobieństwa trafienia.
     * Także sprawdzana jest ilość pozostałych punktów życia
     *
     * @param enemyCreature wskaźnik do obecnego stwożenia przecownika
     *
     */

    void attack(Creature* enemyCreature) {

        if (isHit(enemyCreature)) {

            int amountOfDamege = this->getPow() + aditionalDamegeBasedOnElement(enemyCreature);

            enemyCreature->setHp(enemyCreature->getHp() - amountOfDamege);

            cout << enemyCreature->getName() << " dostaje " << amountOfDamege << " obrazen." << endl;

            if (enemyCreature->getHp() <= 0) {
                cout << enemyCreature->name << " traci swiadomosc" << endl;
                cout << this->getName() << " dostaje 30 punktow doswiadczenia." << endl;
                enemyCreature->setIsAlive(false);
                this->setXp(this->getXp() + 30);
            }
        }
        else {

            cout << this->getName() << " nie trafia." << endl;
        }

    }

    /*!Metoda służąca obliczania ilości obrażeń jakie zostaną zadane przeciwnikowi
     * bez zprawdzania szansy na trafienie
     *
     * @param dmg ilosc obrażeń
     * @param enemyCreature Wskaźnik do stworka przeciwnika
     */
    void attack(int dmg, Creature* enemyCreature) {
        int totalDamege = dmg + aditionalDamegeBasedOnElement(enemyCreature);
        enemyCreature->setHp(enemyCreature->getHp() - totalDamege);
        cout << enemyCreature->name << " dostaje " << totalDamege << " obrazen." << endl;
        if (enemyCreature->getHp() <= 0) {
            cout << enemyCreature->name << " traci swiadomosc" << endl;
            cout << this->getName() << " dostaje 30 punktow doswiadczenia." << endl;
            enemyCreature->setIsAlive(false);
            this->setXp(this->getXp() + 30);
        }
    }

    /**Metoda oblicza szanse na trafirnir w zależnosci od poziomu zręcznośći przeciwnika oraz naszej
     *
     * @param enemy wskaźnik do stworka przeciwnika
     * @return wynik operacji
     */
    bool isHit(Creature* enemy) {
        srand((unsigned)time(NULL));

        int enemyAgl = enemy->getAgl();
        int random = (rand() % 10) + 1;

        int chanceOfAHit = this->agl - enemyAgl;

        if (chanceOfAHit < 0) {
            if (random >= 5) {
                return false;
            }
            else {
                return (abs(chanceOfAHit) <= random);
            }
        }
        else if (chanceOfAHit >= 0) {

            if (random >= 5) {
                return true;
            }
            else {
                return (chanceOfAHit > random);
            }
        }
    }


    void heal(int amountofHeal) {
        setHp(this->getHp() + amountofHeal);
        cout << this->getName() << " zostaw uleczony na " << amountofHeal << " pkt" << endl;
    }

    /**Implementacja tablicy relacji elementów
     *
     * @param enemy Wskaźnik do stworka Przeciwnika
     * @return ilość dodatkowych obrażen
     */
    int aditionalDamegeBasedOnElement(Creature* enemy) {

        Element enemyElement = enemy->getElement();

        if (this->element == WODA && (enemyElement == ZIEMIA || enemyElement == OGIEN)) {
            return 2;
        }
        else if (this->element == WODA && enemyElement == WODA) {
            return -2;
        }
        else if (this->element == ZIEMIA && (enemyElement == OGIEN || enemyElement == LOD || enemyElement == STAL)) {
            return 2;
        }
        else if (this->element == ZIEMIA && enemyElement == POWIETRZE) {
            return -2;
        }
        else if (this->element == POWIETRZE && enemyElement == LOD) {
            return 2;
        }
        else if (this->element == POWIETRZE && (enemyElement == ZIEMIA || enemyElement == STAL)) {
            return -2;
        }
        else if (this->element == OGIEN && (enemyElement == LOD || enemyElement == STAL)) {
            return 2;
        }
        else if (this->element == OGIEN && (enemyElement == WODA || enemyElement == ZIEMIA)) {
            return -2;
        }
        else if (this->element == LOD && enemyElement == ZIEMIA) {
            return 2;
        }
        else if (this->element == LOD && (enemyElement == WODA || enemyElement == OGIEN || enemyElement == LOD)) {
            return -2;
        }
        else if (this->element == STAL && (enemyElement == WODA || enemyElement == POWIETRZE)) {
            return 2;
        }
        else if (this->element == STAL && (enemyElement == OGIEN || enemyElement == STAL)) {
            return -2;
        }
        else {
            return 0;
        }

    }

    void evolve() {
        string chosenStat;
        bool endOfAMove = true;

        while (endOfAMove) {

            cout << "Wybierz ktora z statystyk chcesz powiekszyc" << endl;
            cout << "Zadawane Obraenia - 1; + 2" << endl;
            cout << "Zrecznosc  - 2; + 2" << endl;
            cout << "Maksymalna ilosc punktow zycia - 3; + 3" << endl;
            cin >> chosenStat;

            if (chosenStat.compare("1") == 0) {
                cout << "Zadawane obraenia zostaly powiekszone o 2" << endl;
                this->pow = this->pow + 2;
                endOfAMove = false;
            }
            else if (chosenStat.compare("2") == 0) {
                cout << "Zrecznosc zostala powiekszona o 2" << endl;
                this->agl = this->agl + 2;
                endOfAMove = false;
            }
            else if (chosenStat.compare("3") == 0) {
                cout << "Maksymalna ilosc punktow zycia zostala powiekszona o 3" << endl;
                this->hp = this->hp + 3;
                this->maxHp = maxHp + 3;
                endOfAMove = false;
            }
            else {
                cout << "Bledne dane wejsciowe, sproboj ponownie." << endl;
            }

        }

    }

    void toString() {
        cout << this->getName() << " Element: " << this->getNameOfElement() << endl;
        cout << "Zdrowie: " << this->getHp() << " Onbrazenia : " << this->getPow() << endl;
        cout << "Zrecznosc: " << this->getAgl() << " Doswiadczenie: " << this->getXp() << endl;
        cout << "Umiejetnosci ultymatywne:" << endl;
        getUltimateDesc();
        if (ultimateCD == 0) {
            cout << "Umiejetnosc ultymatywna gotowa do wykorzystania" << endl;
        }
        else {
            cout << "Ponowne uzycie umiejetnosci ultymatywnej bedzie mozliwe za " << ultimateCD << " tury." << endl;
        }
    }

    /**
     * Wypisuje na konsoli infornacje o Zdolności ultymatywnej stworka w zależności od ID
     */
    void getUltimateDesc() {

        if (getID() == 0) {

            cout << "Tsunami: Zadaje 2 obrazenia wszystkim przeciwnika" << endl;
            cout << "Cicha woda: leczy 10 pkt zycia" << endl;
        }
        else if (getID() == 1) {

            cout << "Rzut trojzabem: Zadaje 10 obrazen" << endl;
            cout << "Bog: leczy 12 pkt zycia" << endl;
        }
        else if (getID() == 2) {

            cout << "Fontana: 3 obrazenia wszystkim przeciwnika" << endl;
            cout << "Gruba skora: leczy na 12 pkt zycia" << endl;
        }
        else if (getID() == 3) {

            cout << "Trzesienie ziemi: 1pkt obrazenia wszystkim przeciwnika" << endl;
            cout << "Hard rock: leczy na 10 pktn zycia" << endl;
        }
        else if (getID() == 4) {

            cout << "Rzut kamieniem: 10 obrazen stworku przeciwnika" << endl;
            cout << "Kamienna Zbroja: leczy na 10 pktn zycia" << endl;
        }
        else if (getID() == 5) {

            cout << "Plusk ziemi: 2 obrazenia wszystkim przeciwnika" << endl;
            cout << "Hartowanie: leczy na 12 pkt zycia" << endl;
        }
        else if (getID() == 6) {

            cout << "Rzeski wiatr: 1 obrazenia wszystkim przeciwnika" << endl;
            cout << "Mirarz: leczy na 10 pktn zycia" << endl;
        }
        else if (getID() == 7) {

            cout << "Tornado: 3 obrazenia przeciwniku i stworzeniu zlewa od niego" << endl;
            cout << "Cieply wiatr: leczy na 10 pkt zycia" << endl;
        }
        else if (getID() == 8) {

            cout << "Miecz sprawiedliwosci: 12 obrazenia przeciwniku" << endl;
            cout << "Swiety duch: leczy na 13 pktn zycia" << endl;
        }
        else if (getID() == 9) {

            cout << "Sloneczny promien: 2 obrazenia wszystkim przeciwnika" << endl;
            cout << "Swiete leczenie: leczy na 10 pktn zycia" << endl;
        }
        else if (getID() == 10) {

            cout << "Podpal: Zadaj 12 obrazen przeciwniku" << endl;
            cout << "Zar: leczy na 10 pkt zycia" << endl;
        }
        else if (getID() == 11) {

            cout << "Swieta kara: Zadaj sobie i przeciwniku obrazenia rowne plowie twoich punktow zycia" << endl;
            cout << "Swiety: leczy na 12 pkt zycia" << endl;
        }
        else if (getID() == 12) {

            cout << "Rzut toporem: Zadaj 10 obrazen przeciwniku" << endl;
            cout << "Zimna krew: leczy na 10 pkt zycia" << endl;
        }
        else if (getID() == 13) {

            cout << "Mrozacy wiatr: Zadaj 2 obrazen wszystkim przeciwnika" << endl;
            cout << "Mrozna zbroja: leczy na 10 pkt zycia" << endl;
        }
        else if (getID() == 14) {

            cout << "Snierzyca: Zadaj 2 obrazen wszystkim przeciwnika" << endl;
            cout << "Dzika natura: ulecz sie na 10 pkt zycia" << endl;
        }
        else if (getID() == 15) {

            cout << "Ostry miecz: Zadaj 10 obrazen" << endl;
            cout << "Zbroja: leczy na 10 pkt zycia" << endl;
        }
        else if (getID() == 16) {

            cout << "Zelazny deszcz: Zadaj 2 obrazen wszystkim przeciwnika" << endl;
            cout << "Chromowana powieszcnia: leczy na 10 pkt zycia" << endl;
        }
        else if (getID() == 17) {

            cout << "Ciekly metal: Zadaj 10 obrazen" << endl;
            cout << "Magnetyzm: leczy na 10 pkt zycia" << endl;
        }
    }

    /**W zależności od ID stworka definiuje jakiej umiejętności użyć
     *
     * @param enemyDraft wskaźnik do tablicy którą jest drużyną przeciwnika
     * @param size rozmiar drużyny
     * @param curentCreaature index wskazujący na obecne stwożenie przeciwnika w tablicy
     * @param wybor definijuje wybór rodzaju umiejętności (defensywna/ofensywna)
     */
    void useUltimage(Creature* enemyDraft, size_t size, int curentCreaature, string wybor) {
        if (to_string(getID()) == "0") {
            if (wybor == "1") {
                for (size_t i = 0; i < size; i++) {
                    this->attack(2, &enemyDraft[i]);
                }
            }
            else if (wybor == "2") {
                heal(10);

            }

            this->ultimateCD = 4;
        }
        else if (to_string(getID()) == "1") {
            if (wybor == "1") {
                this->attack(10, &enemyDraft[curentCreaature]);
            }
            else if (wybor == "2") {
                heal(10);
            }
            this->ultimateCD = 4;
        }
        else if (to_string(getID()) == "2") {
            if (wybor == "1") {
                for (size_t i = 0; i < size; i++) {
                    this->attack(3, &enemyDraft[i]);
                }
            }
            else if (wybor == "2") {
                heal(12);
            }

            ultimateCD = 6;

        }
        else if (to_string(getID()) == "3") {
            if (wybor == "1") {
                for (size_t i = 0; i < size; i++) {
                    this->attack(1, &enemyDraft[i]);
                }
            }
            else if (wybor == "2") {
                heal(10);
            }
            ultimateCD = 6;
        }
        else if (to_string(getID()) == "4") {
            if (wybor == "1") {
                this->attack(10, &enemyDraft[curentCreaature]);
            }
            else if (wybor == "2") {
                heal(10);
            }

            ultimateCD = 5;
        }
        else if (to_string(getID()) == "5") {
            if (wybor == "1") {
                for (size_t i = 0; i < size; i++) {
                    this->attack(2, &enemyDraft[i]);
                }
            }
            else if (wybor == "2") {
                heal(12);
            }
            ultimateCD = 6;
        }
        else if (to_string(getID()) == "6") {
            if (wybor == "1") {
                for (size_t i = 0; i < size; i++) {
                    this->attack(1, &enemyDraft[i]);
                }
            }
            else if (wybor == "2") {
                heal(10);
            }
            ultimateCD = 4;
        }
        else if (to_string(getID()) == "7") {
            if (wybor == "1") {
                this->attack(3, &enemyDraft[curentCreaature]);
                if (curentCreaature == size) {
                    this->attack(3, &enemyDraft[0]);
                }
                else {
                    this->attack(3, &enemyDraft[curentCreaature + 1]);
                }
            }
            else if (wybor == "2") {
                heal(10);
            }
            ultimateCD = 6;
        }
        else if (to_string(getID()) == "8") {
            if (wybor == "1") {
                this->attack(12, &enemyDraft[curentCreaature]);
            }
            else if (wybor == "2") {
                heal(13);
            }
            ultimateCD = 6;
        }
        else if (to_string(getID()) == "9") {
            if (wybor == "1") {
                for (size_t i = 0; i < size; i++) {
                    this->attack(2, &enemyDraft[i]);
                }
            }
            else if (wybor == "2") {
                heal(10);
            }
            ultimateCD = 5;
        }
        else if (to_string(getID()) == "10") {
            if (wybor == "1") {
                attack(12, &enemyDraft[curentCreaature]);
            }
            else if (wybor == "2") {
                heal(10);
            }
            ultimateCD = 6;
        }
        else if (to_string(getID()) == "11") {
            if (wybor == "1") {
                int tmp = this->getHp() / 2;
                this->setHp(tmp);
                attack(tmp, &enemyDraft[curentCreaature]);

            }
            else if (wybor == "2") {
                heal(12);
            }
            ultimateCD = 6;
        }
        else if (to_string(getID()) == "12") {
            if (wybor == "1") {
                attack(10, &enemyDraft[curentCreaature]);

            }
            else if (wybor == "2") {
                heal(10);
            }
            ultimateCD = 5;
        }
        else if (to_string(getID()) == "13") {
            if (wybor == "1") {
                for (size_t i = 0; i < size; i++) {
                    attack(2, &enemyDraft[i]);
                }

            }
            else if (wybor == "2") {
                heal(10);
            }
            ultimateCD = 6;
        }
        else if (to_string(getID()) == "14") {
            if (wybor == "1") {
                for (size_t i = 0; i < size; i++) {
                    attack(2, &enemyDraft[i]);
                }
            }
            else if (wybor == "2") {
                heal(10);
            }
            ultimateCD = 5;
        }
        else if (to_string(getID()) == "15") {
            if (wybor == "1") {
                attack(12, &enemyDraft[curentCreaature]);
            }
            else if (wybor == "2") {
                heal(10);
            }
            ultimateCD = 4;
        }
        else if (to_string(getID()) == "16") {
            if (wybor == "1") {
                for (size_t i = 0; i < size; i++) {
                    attack(2, &enemyDraft[i]);
                }
            }
            else if (wybor == "2") {
                heal(10);
            }
        }
        else if (to_string(getID()) == "17") {
            if (wybor == "1") {
                attack(10, &enemyDraft[curentCreaature]);

            }
            else if (wybor == "2") {
                heal(10);
            }
        }
    }

    /**Udostepnia możliwość podjęcia decyzji przez Gracz o rodzaju umiejętności
     *
     * @param enemyDraft wskaźnik do tablicy którą jest drużyną przeciwnika
     * @param size rozmiar drużyny
     * @param curentCreaature index wskazujący na obecne stwożenie przeciwnika w tablicy
     */
    void ultimate(Creature* enemyDraft, size_t size, int curentCreaature) {
        bool fin = true;
        string wybor;

        while (fin) {
            cout << "Jezeli chcesz uzyc umiejentnosci ofensywnej wybierz 1" << endl;
            cout << "Dla uzycia umiejentnosci defensywnej wybierz 2" << endl;
            cin >> wybor;
            if (wybor == "1" || wybor == "2") {

                useUltimage(enemyDraft, size, curentCreaature, wybor);

                fin = false;

            }
            else {
                cout << "Bledny wpis, sprobuj ponownie" << endl;
            }
        }

    }

    /**Przciążona, uproszczona metoda umożliwiająca użycie Zdolności ultymatywnej przez przeciwnika nie wypisując komunikaty
     *
     * @param enemyDraft wskaźnik do tablicy którą jest drużyną przeciwnika
     * @param size rozmiar drużyny
     * @param curentCreaature index wskazujący na obecne stwożenie przeciwnika w tablicy
     * @param enemyChoice decyzja komputera o rodzaju użytej umiejętności
     */
    void ultimate(Creature* enemyDraft, size_t size, int curentCreaature, int enemyChoice) {

        string wybor = to_string(enemyChoice);
        useUltimage(enemyDraft, size, curentCreaature, wybor);

    }

    /**
     *Zwraca utracone punkty życia
     */
    void backToDefoultValue() {
        setHp(maxHp);
        alive = true;
    }

    /**Zwraca tekstową interpretacje elsementu
     *
     * @return tekstowa interetacje elementu
     */
    string getNameOfElement() {
        string res = "";
        if (this->element == WODA) {
            res = "WODA";
        }
        else if (this->element == OGIEN) {
            res = "OGIEN";
        }
        else if (this->element == POWIETRZE) {
            res = "POWIETRZE";
        }
        else if (this->element == LOD) {
            res = "LOD";
        }
        else if (this->element == STAL) {
            res = "STAL";
        }
        else if (this->element == ZIEMIA) {
            res = "ZIEMIA";
        }

        return res;
    }

};

