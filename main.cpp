#include <iostream>
#include "Player.cpp"
#include "string"

using std::string;
using namespace std;

void printABattleScreen(Player* player, Player* enemy) {
    cout << "Stworek Gracza: " << endl;
    cout << "=========================================================" << endl;
    player->getCurrentCreature()->toString();

    cout << "=========================================================" << endl;
    cout << "Stworek Przeciwnika : " << endl;
    cout << "=========================================================" << endl;
    enemy->getCurrentCreature()->toString();
    cout << "Wybierz swoj nastepny ruch:" << endl;
    cout << "1 - Ataka | 2 - Zmiana Stworka | 3 - Umiejetnosc specialna | 4 - Evolucja " << endl;
    cout << "\"-h\" - pomoc" << endl;
}

template <typename FUN>
void welcomeScreen(Player* player, Player* enemy, FUN fun) {
    string wybor = " ";
    bool flag = true;

    cout << "Witaj w grze!" << endl;
    cout << "Jesli chcesz wczytac zapisana gre wprowadz 1, jesli chcesz zaczac od nowa wprowadz 2" << endl;
    cout << "Dla Wyswietlenia informacji o grze wprowadz \"-h\"" << endl;
    while (flag) {
        cin >> wybor;
        if (wybor == "1") {
            player->load();
            enemy->load();
            flag = false;
        }
        else if (wybor == "2") {
            cout << "Wpisz swoje imie" << endl;
            cin >> wybor;
            player->setName(wybor);
            player->makeADraft();
            enemy->makeADraft();
            flag = false;
        }
        else if (wybor == "-h") {
            fun();
        }
        else {
            cout << "Blad, sprobobuj ponownie." << endl;
        }
    };
}

static void infoScreen() {
    cout << "Gra polega na wyeliminowaniu stworkow przeciwnika" << endl;
    cout << "Gracze, kazdy w swojej turze, moga wykonanc 1 z 4 dostepnych dzialan:" << endl;
    cout << "1 - Atak(Nanosi stworku przeciwnika obrazenia rowne ilosci pkt sily(POW) twojego stworka)" << endl;
    cout << "2 - Zamiana(Mozesz zamienic obecnego stworka na innego z wczesniej wybranych)" << endl;
    cout << "3 - Uzycie umiejetnosci ultymatywnej(Kazdy stworek ma 1 umiejetnosc atakujaca i defensywna, mozesz wybrac jaka chcesz teraz uzyc)" << endl;
    cout << "4 - Ewolucja(Za zabijanie stworkow przeciwnika sa przyznawane pkt doswiadczenia uzbieraj 100" << endl;
    cout << "i bedziesz mogl do konca gry powiekszyc 1 z statystyk swojego stworka)" << endl << endl;
    cout << "Statystyki: " << endl;
    cout << "Sila (POW): (odpowiada za pkt obrazenia pod czas ataku)" << endl;
    cout << "Zrecznisc (AGL): (odpowiada za szansy nie trafienia przeciwnika po twoim stworku i podwyzsza twoja szanse na trafienie)" << endl;
    cout << "Punkty doswiadczenia(EX): (sa przyznawane za zabijanie stwrkow przeciwnika)" << endl;
    cout << "Element: (zwieksza odpornosc przeciwko obrazen przeciwnika o okreslonych elementach " << endl;
    cout << "ale zwieksza obrazenia od przeciwnikow o innych elementach zgodnie z infirmacja ponizej:)" << endl;
    cout << "===========================================================" << endl;
    cout << "||Silniejszy od:  ||Element:  || Slabszy od:             ||" << endl;
    cout << "||Ziemia, Ogien   || Woda     || Woda                    ||" << endl;
    cout << "||Lod, Stal, Ogien|| Ziemia   || Powietrze               ||" << endl;
    cout << "||Lod             || Powietrze|| Powietrze|| Ziemia, Stal||" << endl;
    cout << "||Lod, Stal       || Ogien    || Woda, Ziemia            ||" << endl;
    cout << "||Lod, Stal, Ogien|| Ziemia   || Powietrze               ||" << endl;
    cout << "||Ziemia          || Lod      || Woda, Ogien, Lod        ||" << endl;
    cout << "||Woda, Powietrze || Stal     || Ogien, Stal             ||" << endl;
    cout << "===========================================================" << endl;
    cout << "Po ukonczeniu twojego ruchu nastepuje ruch przeciwnika, i tak do tego czasu az nie zostanie tylko najsilniejsy" << endl;
    cout << "Pokonanie 4 przeciwnikow konczy gre." << endl;

}

int main() {
    string wybor = " ";//Zmienna dla zapisu danych wejsciowech od gracza
    string winner = " ";
    bool endOfATurn = true;//flaga wskazująca na poprawność danych wejsciowech pod czas tury gracza

    Player* player = new Player();
    Enemy cpu = Enemy();
    Player* enemy = &cpu;


    welcomeScreen(player, enemy, infoScreen);

    //Pętla reprezentująca pozrywkę turową
    while (winner == " ") {

        //Tura gracza
        while (endOfATurn) {

            printABattleScreen(player, enemy);

            cin >> wybor;
            if (wybor == "1") {
                player->getCurrentCreature()->attack(enemy->getCurrentCreature());
                endOfATurn = false;
            }
            else if (wybor == "2") {

                cout << "Wybierz na kogo chcesz zamienic swojego stwora " << endl;
                player->printACurrentDraft();
                int indexNowegoStwora;
                cin >> indexNowegoStwora;
                if (player->getAllCreatures()[indexNowegoStwora].isAlive()) {
                    player->swapCreature(indexNowegoStwora);
                    endOfATurn = false;
                }
                else {
                    cout << "Nie mozesz tego zrobic, wybrany stworek nie zyje. Sprobuj jeszcze raz." << endl;
                }

            }
            else if (wybor == "3") {
                if (player->getCurrentCreature()->getUltimateCD() == 0) {
                    player->getCurrentCreature()->ultimate(enemy->getAllCreatures(), enemy->getSize(), enemy->getcurrentCreatureId());
                    endOfATurn = false;
                }
                else {
                    cout << "Nie mozesz uzyc umiejetnosci ultymatywnej, mozna bedzie z niej skoystac za: " << player->getCurrentCreature()->getUltimateCD() << " rund." << endl;
                }

            }
            else if (wybor == "4") {
                if (player->getCurrentCreature()->getXp() >= 100) {
                    player->getCurrentCreature()->setXp(player->getCurrentCreature()->getXp() - 100);
                    player->getCurrentCreature()->evolve();
                    endOfATurn = false;
                }
                else {
                    cout << "Niewystarczjaca ilosc punktow dozwiadczenia dla ewolucii" << endl;
                }
            }else if (wybor == "-h") {
                infoScreen();

            }
            else{
                cout << "Bledne dane wejsciowe, sprobuj ponownie."<< endl;
            }


        }
        enemy->save();//Zapis danych przeciwnika
        player->decCD();//Obniżenia licznika przeładowania umiejętności gracza

        //Sprawdzanie czy nie wygrał gracz
        if (enemy->allAreDead() && enemy->getRound() == 4) {
            winner = player->getName();
            break;
        }
        else if (enemy->allAreDead()) {
            cout << "Pokonales przeciwnika w " << enemy->getRound() << " rundzie." << endl;
            enemy->incRound();
            player->resetDraft();
        }
        else {
            endOfATurn = true;

            cout << "Tura przeciwnika " << endl;
            Sleep(1000);
            enemy->enemyTurn(player);// Tura przeciwnika
        }
        enemy->decCD();//Obniżenia licznika przeładowania umiejętności przeciwnika

        //Sprawdzenie czy nie wygrał przeciwnik
        if (player->allAreDead()) {
            winner = "CPU";
        }

        enemy->findNextAlive();

        player->save();//Zapis danych gracza

    }

    cout << "Koniec gry! Zwyciezca : " << winner << "." << endl;
}
