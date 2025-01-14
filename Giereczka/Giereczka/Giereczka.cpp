#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>       
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

using namespace sf;
using namespace std;

//zmienne globalne klasy

int border1 = 940;
int border2 = -40;
int border3 = 60;
int border4 = 690;

int licznik = 0;
int licznikprzejscia = 250;
int licznikprzejsciastart = 0;
int licznikprzejsciastart2 = 0;

int licznikblob = 0;
int licznikblobwalka = 0;
int licznikblob2 = 0;

int animacjawalka = 0;
int offsetchodzenie = 0;
int offsetidle = 0;
int offsetmapa1 = 0;
int offsetwalka = 0;
int offsetblob = 0;
int offsetgameoverflip = 0;

int ostatninacisnietyklawisz = 3;

class Postac
{
public:
    int HP;
    int HPMax;
    Postac()
    {
        this->HPMax = 8;
        this->HP = this->HPMax;
    }
    ~Postac() {}
};

class Enemy
{
public:
    Sprite shape;
    int HP;
    int HPMax;
    int stanblob;
    int dlugoscruchu;
    int nblob;
    int offsetblobwalka;
    Enemy(Texture* texture)
    {
        this->HPMax = 1;
        this->HP = this->HPMax;
        this->shape.setTexture(*texture);
        this->shape.setTextureRect(sf::IntRect(0, 0, 150, 150));
        this->stanblob = 0;
        this->dlugoscruchu = 0;
        this->nblob = 0;
        this->offsetblobwalka = 0;
    }
    ~Enemy() {}
};

class krzak
{
public:
    Sprite krzaki;

    krzak(Texture* texture)
    {
        this->krzaki.setTexture(*texture);
        this->krzaki.setTextureRect(sf::IntRect(0, 0, 200, 200));
    }
    ~krzak() {}
};


////////////////// Funkcje pomocnicze i logika 

void Kontrolerwasd(int& keyTime1, Sprite& postacint, int& keyTime2);
void Kontrolerstrzalki(int& keyTime2, Sprite& postac, vector<Enemy>& enemies, Postac& postacliczenie, int& points);
void Draw(RenderWindow& window, Sprite& postac, const int& mapsizex, const int& mapsizey,
    vector<vector <Sprite>>& tilemap, vector<Enemy>& enemies, vector<krzak>& krzakitab,
    Text text, Text text2, Sprite& hp, Text pauza);
void Blob(std::vector<Enemy>& enemies, Texture& enemyTex, RenderWindow& window,
    Sprite& postac, int& tilemapflip, Postac& postacliczenie, int diff, int roomnumber);
void Menuz(RenderWindow& window, Sprite& Menu, Sprite& nowagrasprite,
    Sprite& kontynuujsprite, Sprite& latwysprite, Sprite& trudnysprite, Text highscoret);
void Gameover(RenderWindow& window, Text przegrana);
void krzaki(std::vector<krzak>& krzakitab, Texture& krzakiTex, int& tilemapflip);
void Startpok(RenderWindow& window, Sprite& Start, Sprite& postac);
void Mapa(const int& mapsizex, const int& mapsizey, std::vector<std::vector <sf::Sprite>>& tilemap, Sprite& mapa);

string hudtext(int roomnumber);
string hudtext2(int points);
string highscoreconvert();
void hudhp(Postac& postacliczenie, Sprite& hp);
void zapis(int diff, Postac& postacliczenie, int roomnumber, int points);
void odczyt(int& diff, Postac& postacliczenie, int& roomnumber, int& points);
void highscore(int& points);
bool przyciski(int xmax, int ymax, int xmin, int ymin, RenderWindow& window);


//       G£ÓWNA FUNKCJA GRY

void runGame()
{
    int gamestate = 0;

    int tilemapflip = 0;

    int roomnumber = 0;
    int points = 0;
    int diff = 1;
    srand(time(NULL));

    int keyTime1 = 1;
    int keyTime2 = 28;

    RenderWindow window(VideoMode(1200, 1000), "", Style::None);//okienko i jego w³aœciwoœci
    window.setFramerateLimit(60);

    Font font;
    font.loadFromFile("Amatic-Bold.ttf");

    // mapa i jej w³aœciwoœci
    Texture textury;
    Sprite mapa;
    textury.loadFromFile("mapa.png");
    mapa.setTexture(textury);
    const int mapsizex = 6;
    const int mapsizey = 5;
    std::vector<std::vector <sf::Sprite>> tilemap(mapsizex, std::vector<sf::Sprite>(mapsizey, sf::Sprite()));/// mapa i siatka

    Texture postactextury;
    Sprite postac;// ³aduje postaæ
    postac.setPosition(500, 320);
    postactextury.loadFromFile("ORC.png");
    postac.setTexture(postactextury);// za³adowanie postaci
    Postac postacliczenie;

    Texture hudhptext;
    Sprite hp;// ³aduje postaæ
    hudhptext.loadFromFile("Hp.png");
    hp.setTexture(hudhptext);
    hp.setPosition(800.f, 915.f);

    Texture menu;
    Sprite Menu;// ³aduje game over
    menu.loadFromFile("menu.jpg");
    Menu.setTexture(menu);

    Texture nowagra;
    nowagra.loadFromFile("button.png");
    Sprite nowagrasprite;
    nowagrasprite.setTexture(nowagra);
    nowagrasprite.setPosition(230.f, 480.f);

    Texture kontynuuj;
    kontynuuj.loadFromFile("button.png");
    Sprite kontynuujsprite;
    kontynuujsprite.setTexture(kontynuuj);
    kontynuujsprite.setPosition(635.f, 480.f);

    Texture latwy;
    latwy.loadFromFile("button2.png");
    Sprite latwysprite;
    latwysprite.setTexture(latwy);
    latwysprite.setPosition(400.f, 680.f);

    Texture trudny;
    trudny.loadFromFile("button2.png");
    Sprite trudnysprite;
    trudnysprite.setTexture(trudny);
    trudnysprite.setPosition(590.f, 680.f);

    Texture enemyTex;
    enemyTex.loadFromFile("blob.png");//³aduje bloba
    std::vector<Enemy> enemies;

    Texture krzakiTex;
    krzakiTex.loadFromFile("krzaki.png");//³aduje krzaki
    std::vector<krzak> krzakitab;

    Texture start;
    Sprite Start;// ³aduje game over
    start.loadFromFile("start.png");
    Start.setTexture(start);

    Text text;
    text.setFont(font);
    text.setCharacterSize(70);
    text.setFillColor(Color::Red);
    text.setPosition(60.f, 910.f);

    Text text2;
    text2.setFont(font);
    text2.setCharacterSize(70);
    text2.setFillColor(Color::Red);
    text2.setPosition(220.f, 910.f);

    Text przegrana;
    przegrana.setFont(font);
    przegrana.setCharacterSize(200);
    przegrana.setFillColor(Color::Red);
    przegrana.setPosition(400.f, 300.f);

    Text pauza;
    pauza.setFont(font);
    pauza.setCharacterSize(200);
    pauza.setFillColor(Color::Red);
    pauza.setPosition(450.f, 300.f);
    pauza.setString("Sto[P]");

    Text highscoret;
    highscoret.setFont(font);
    highscoret.setCharacterSize(62);
    highscoret.setFillColor(Color::White);
    highscoret.setPosition(450.f, 30.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
            {
                window.close();
            }
        }

        switch (gamestate)
        {
            // ----------------------------------
        case 0: // odswiezanie
            gamestate = 1;
            break;

            // ----------------------------------
        case 1: // MENU

            if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                licznikprzejsciastart = 1;
            }
            if (przyciski(540, 600, 230, 400, window) == true)
                nowagrasprite.setColor(Color(255, 0, 0));
            else
                nowagrasprite.setColor(Color(0, 0, 255));

            if ((przyciski(540, 600, 230, 400, window) == true) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                licznikprzejsciastart = 1;

            if (licznikprzejsciastart == 1) //jesli przechodzi do gry -> efekt zanikania
            {
                if (licznikprzejscia > 0) licznikprzejscia = licznikprzejscia - 10;
                Menu.setColor(sf::Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));

                if (licznikprzejscia == 0) {
                    gamestate = 2;
                    licznikprzejsciastart = 0;
                }
            }

            if ((przyciski(500, 740, 400, 680, window) == true) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                diff = 1;
            if (Keyboard::isKeyPressed(Keyboard::Num1))
                diff = 1;
            if (przyciski(500, 740, 400, 680, window) == true)
                latwysprite.setColor(Color(255, 0, 0));
            else
                latwysprite.setColor(Color(0, 0, 255));

            if ((przyciski(780, 740, 630, 680, window) == true) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                diff = 3;
            if (Keyboard::isKeyPressed(Keyboard::Num2))
                diff = 3;
            if (przyciski(780, 740, 630, 680, window) == true)
                trudnysprite.setColor(Color(255, 0, 0));
            else
                trudnysprite.setColor(Color(0, 0, 255));

            highscoret.setString(highscoreconvert()); //wyswietlanie najwyzszego wyniku z funkcji
            Menuz(window, Menu, nowagrasprite, kontynuujsprite, latwysprite, trudnysprite, highscoret);
            Menu.setColor(sf::Color(255, 255, 255));

            if (Keyboard::isKeyPressed(Keyboard::E)) {
                postac.setPosition(500, 900);
                licznikprzejsciastart2 = 1;
                border1 = 1000;
                border2 = -50;
                border3 = 0;
                border4 = 720;
                tilemapflip = 1;
            }

            if ((przyciski(900, 600, 600, 400, window) == true) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                postac.setPosition(500, 900);
                licznikprzejsciastart2 = 1;
                border1 = 1000;
                border2 = -50;
                border3 = 0;
                border4 = 720;
                tilemapflip = 1;
            }
            if (przyciski(900, 600, 600, 400, window) == true)
                kontynuujsprite.setColor(Color(255, 0, 0));
            else
                kontynuujsprite.setColor(Color(0, 0, 255));

            if (licznikprzejsciastart2 == 1) {
                if (licznikprzejscia > 0) licznikprzejscia = licznikprzejscia - 10;
                Menu.setColor(sf::Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));

                if (licznikprzejscia == 0) {
                    gamestate = 3;
                    odczyt(diff, postacliczenie, roomnumber, points);
                    licznikprzejsciastart2 = 0;
                }
            }
            break;

            // ----------------------------------
        case 2: // EKRAN STARTOWY

            if (licznikprzejsciastart == 0) {
                if (licznikprzejscia < 250) licznikprzejscia = licznikprzejscia + 10;
                Start.setColor(sf::Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));
                postac.setColor(sf::Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));
            }

            Startpok(window, Start, postac);
            if (licznikprzejsciastart == 0) {
                Kontrolerstrzalki(keyTime2, postac, enemies, postacliczenie, points);
                Kontrolerwasd(keyTime1, postac, keyTime2);
            }
            licznik++;
            licznikblob++;
            if (licznikblob > 3) { licznikblob = 0; }
            if (licznik > 5) { licznik = 0; }

            if (Keyboard::isKeyPressed(Keyboard::W) && !Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::D)
                && postac.getPosition().x > 330 && postac.getPosition().x < 640 && postac.getPosition().y < -30)
            {
                licznikprzejsciastart = 1;
            }
            if (licznikprzejsciastart == 1) {
                if (licznikprzejscia > 0) licznikprzejscia = licznikprzejscia - 10;
                postac.setColor(sf::Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));
                Start.setColor(sf::Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));
                postac.move(0.f, -7.f);
                postac.setTextureRect(sf::IntRect(offsetchodzenie, 200, 200, 200));
            }
            if (licznikprzejscia == 0) {
                postac.setPosition(500, 900);
                licznikprzejsciastart = 0;
                border1 = 1000;
                border2 = -50;
                border3 = 0;
                border4 = 720;
                tilemapflip = 1;
                gamestate = 3;
                roomnumber++;
            }
            break;

            // ----------------------------------
        case 3: // POKÓJ 1

            krzaki(krzakitab, krzakiTex, tilemapflip);
            Draw(window, postac, mapsizex, mapsizey, tilemap, enemies, krzakitab, text, text2, hp, pauza);

            if (!Keyboard::isKeyPressed(Keyboard::P)) {
                if (licznikprzejsciastart == 1) {
                    Kontrolerwasd(keyTime1, postac, keyTime2);
                    Kontrolerstrzalki(keyTime2, postac, enemies, postacliczenie, points);
                }
                Blob(enemies, enemyTex, window, postac, tilemapflip, postacliczenie, diff, roomnumber);
            }

            if (Keyboard::isKeyPressed(Keyboard::V))
                zapis(diff, postacliczenie, roomnumber, points);

            if (tilemapflip == 1)
                Mapa(mapsizex, mapsizey, tilemap, mapa);
            //przejœcia z ciemnego na jasne
            if (licznikprzejsciastart == 0) {
                if (licznikprzejscia < 248) {
                    licznikprzejscia = licznikprzejscia + 10;
                    postac.setColor(sf::Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));
                    postac.move(0.f, -7.f);
                    postac.setTextureRect(sf::IntRect(offsetchodzenie, 200, 200, 200));
                }
                if (licznikprzejscia > 249) licznikprzejsciastart = 1;
            }

            text.setString(hudtext(roomnumber));
            text2.setString(hudtext2(points));
            hudhp(postacliczenie, hp);

            if (postacliczenie.HP < 1) {
                licznikprzejscia = 250;
                gamestate = 4;
            }

            if (Keyboard::isKeyPressed(Keyboard::W) && !Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::D)
                && postac.getPosition().x > 330 && postac.getPosition().x < 640 && postac.getPosition().y < -30)
            {
                licznikprzejscia = 1; //przechodzi do nowego pokju
            }

            tilemapflip = 0; // gracz jest w starym pokoju
            if (licznikprzejscia == 1) {
                postac.setPosition(500, 900);
                roomnumber++;
                points = points + 3;
                border1 = 1000;
                border2 = -50;
                border3 = 0;
                border4 = 720;
                tilemapflip = 1; //gracz przeszedl do nowego pokoju
                gamestate = 5;
                licznikprzejsciastart = 0;
                for (size_t i = 0; i < krzakitab.size(); i++) //usuwanie krzakow
                {
                    krzakitab[i].krzaki.setPosition(100000, 100000);
                    krzakitab.erase(krzakitab.begin() + i);
                }
                for (size_t i = 0; i < enemies.size(); i++) //usuwanie bloba
                {
                    enemies[i].shape.setPosition(100000, 100000);
                    enemies.erase(enemies.begin() + i);
                }
            }

            licznik++; //dla postaci gracza
            if (licznik > 5) { licznik = 0; }
            licznikblob++; //dla przeciwnikow
            if (licznikblob > 3) { licznikblob = 0; }
            licznikblob2++; //zmiana ruchow bloba
            if (licznikblob2 > 60) { licznikblob2 = 0; }
            licznikblobwalka++;
            if (licznikblobwalka > 4) { licznikblobwalka = 0; }

            break;

            // ----------------------------------
        case 4: // GAME OVER

            if (licznikprzejsciastart == 1) {
                przegrana.setFillColor(sf::Color(250, 250, 250));
            }
            if (licznikprzejsciastart == 0) {
                if (licznikprzejscia > 0) licznikprzejscia = licznikprzejscia - 10;
                przegrana.setFillColor(sf::Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));
            }
            if (licznikprzejscia == 0) {
                border1 = 940;
                border2 = -40;
                border3 = 60;
                border4 = 690;
                gamestate = 2;
            }
            highscore(points);
            roomnumber = 0;
            points = 0;
            Gameover(window, przegrana);
            for (size_t i = 0; i < krzakitab.size(); i++) {
                krzakitab[i].krzaki.setPosition(100000, 100000);
                krzakitab.erase(krzakitab.begin() + i);
            }
            for (size_t i = 0; i < enemies.size(); i++) {
                enemies[i].shape.setPosition(100000, 100000);
                enemies.erase(enemies.begin() + i);
            }

            if (Keyboard::isKeyPressed(Keyboard::R)) {
                postac.setPosition(500, 320);
                postacliczenie.HP = 8;
                licznikprzejsciastart = 0;
            }
            break;

            // ----------------------------------
        case 5: // POKÓJ z zapisu

            if (tilemapflip == 1)
                Mapa(mapsizex, mapsizey, tilemap, mapa);

            if (licznikprzejsciastart == 0) {
                if (licznikprzejscia < 248) {
                    licznikprzejscia = licznikprzejscia + 10;
                    postac.setColor(sf::Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));
                    postac.move(0.f, -7.f);
                    postac.setTextureRect(sf::IntRect(offsetchodzenie, 200, 200, 200));
                }
                if (licznikprzejscia > 249)
                    licznikprzejsciastart = 1;
            }
            if (Keyboard::isKeyPressed(Keyboard::V))
                zapis(diff, postacliczenie, roomnumber, points);

            krzaki(krzakitab, krzakiTex, tilemapflip);
            if (!Keyboard::isKeyPressed(Keyboard::P)) {
                if (licznikprzejsciastart == 1) {
                    Kontrolerwasd(keyTime1, postac, keyTime2);
                    Kontrolerstrzalki(keyTime2, postac, enemies, postacliczenie, points);
                }
                Blob(enemies, enemyTex, window, postac, tilemapflip, postacliczenie, diff, roomnumber);
            }
            Draw(window, postac, mapsizex, mapsizey, tilemap, enemies, krzakitab, text, text2, hp, pauza);

            text.setString(hudtext(roomnumber));
            text2.setString(hudtext2(points));
            hudhp(postacliczenie, hp);

            if (Keyboard::isKeyPressed(Keyboard::W) && !Keyboard::isKeyPressed(Keyboard::A)
                && !Keyboard::isKeyPressed(Keyboard::D)
                && postac.getPosition().x > 330 && postac.getPosition().x < 640
                && postac.getPosition().y < -30)
            {
                licznikprzejscia = 1;
            }
            tilemapflip = 0;

            if (licznikprzejscia == 1) {
                postac.setPosition(500, 900);
                roomnumber++;
                points = points + 3;
                border1 = 1000;
                border2 = -50;
                border3 = 0;
                border4 = 720;
                tilemapflip = 1;
                gamestate = 3;
                licznikprzejsciastart = 0;
                for (size_t i = 0; i < krzakitab.size(); i++) {
                    krzakitab[i].krzaki.setPosition(100000, 100000);
                    krzakitab.erase(krzakitab.begin() + i);
                }
                for (size_t i = 0; i < enemies.size(); i++) {
                    enemies[i].shape.setPosition(100000, 100000);
                    enemies.erase(enemies.begin() + i);
                }
            }
            if (postacliczenie.HP < 1) {
                licznikprzejscia = 250;
                gamestate = 4;
            }

            licznik++;
            if (licznik > 5) { licznik = 0; }
            licznikblob++;
            if (licznikblob > 3) { licznikblob = 0; }
            licznikblob2++;
            if (licznikblob2 > 60) { licznikblob2 = 0; }
            licznikblobwalka++;
            if (licznikblobwalka > 4) { licznikblobwalka = 0; }

            break;
        }

    }
}

///////////////////////////////////////////////////////////////
int main()
{
    runGame();
    return 0;
}

////////////////////////////////////////////////////////////////
//FUNKCJE LOGIKI

void Kontrolerwasd(int& keyTime1, Sprite& postac, int& keyTime2)
{
    if (keyTime1 < 4) keyTime1++;
    if (licznik == 4) {
        offsetchodzenie = offsetchodzenie + 200;
        if (offsetchodzenie >= 1000) { offsetchodzenie = 0; }
    }
    if (licznik == 4) {
        offsetidle = offsetidle + 200;
        if (offsetidle >= 600) { offsetidle = 0; }
    }
    if (keyTime2 < 28)
        keyTime1 = 0;

    if (Keyboard::isKeyPressed(Keyboard::W) && Keyboard::isKeyPressed(Keyboard::D)
        && postac.getPosition().x<border1 && postac.getPosition().y >border2
        && !Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::S) && keyTime1 > 0)
    {
        postac.move(5.f, -5.f);
        postac.setTextureRect(sf::IntRect(offsetchodzenie, 200, 200, 200));
        ostatninacisnietyklawisz = 1;
        keyTime1 = 0;
    }
    if (Keyboard::isKeyPressed(Keyboard::W) && postac.getPosition().y > border2
        && !Keyboard::isKeyPressed(Keyboard::A)
        && !Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::S) && keyTime1 > 0)
    {
        postac.move(0.f, -7.f);
        postac.setTextureRect(sf::IntRect(offsetchodzenie, 200, 200, 200));
        ostatninacisnietyklawisz = 1;
        keyTime1 = 0;
    }
    if (Keyboard::isKeyPressed(Keyboard::W) && Keyboard::isKeyPressed(Keyboard::A)
        && postac.getPosition().y > border2 && postac.getPosition().x > border3
        && !Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::S) && keyTime1 > 0)
    {
        postac.move(-5.f, -5.f);
        postac.setTextureRect(sf::IntRect(offsetchodzenie, 200, 200, 200));
        ostatninacisnietyklawisz = 1;
        keyTime1 = 0;
    }

    if (Keyboard::isKeyPressed(Keyboard::A) && postac.getPosition().x > border3
        && !Keyboard::isKeyPressed(Keyboard::S)
        && !Keyboard::isKeyPressed(Keyboard::W) && !Keyboard::isKeyPressed(Keyboard::D) && keyTime1 > 0)
    {
        postac.move(-7.f, 0.f);
        postac.setTextureRect(sf::IntRect(offsetchodzenie, 400, 200, 200));
        ostatninacisnietyklawisz = 2;
        keyTime1 = 0;
    }
    if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::S)
        && postac.getPosition().y < border4 && postac.getPosition().x >border3
        && !Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::W) && keyTime1 > 0)
    {
        postac.move(-5.f, 5.f);
        postac.setTextureRect(sf::IntRect(offsetchodzenie, 0, 200, 200));
        ostatninacisnietyklawisz = 1;
        keyTime1 = 0;
    }
    if (Keyboard::isKeyPressed(Keyboard::S) && postac.getPosition().y < border4
        && !Keyboard::isKeyPressed(Keyboard::A)
        && !Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::W) && keyTime1>0)
    {
        postac.move(0.f, 7.f);
        postac.setTextureRect(sf::IntRect(offsetchodzenie, 0, 200, 200));
        ostatninacisnietyklawisz = 3;
        keyTime1 = 0;
    }
    if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::S)
        && postac.getPosition().y < border4 && postac.getPosition().x < border1
        && !Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::W) && keyTime1>0)
    {
        postac.move(5.f, 5.f);
        postac.setTextureRect(sf::IntRect(offsetchodzenie, 0, 200, 200));
        ostatninacisnietyklawisz = 1;
        keyTime1 = 0;
    }
    if (Keyboard::isKeyPressed(Keyboard::D) && postac.getPosition().x < border1
        && !Keyboard::isKeyPressed(Keyboard::A) &&
        !Keyboard::isKeyPressed(Keyboard::S) && !Keyboard::isKeyPressed(Keyboard::W) && keyTime1>0)
    {
        postac.move(7.f, 0.f);
        postac.setTextureRect(sf::IntRect(offsetchodzenie, 600, 200, 200));
        ostatninacisnietyklawisz = 4;
        keyTime1 = 0;
    }
    if (ostatninacisnietyklawisz == 1 && !Keyboard::isKeyPressed(Keyboard::W)
        && !Keyboard::isKeyPressed(Keyboard::Down) && keyTime1 > 0) {
        postac.setTextureRect(sf::IntRect(offsetidle, 1000, 200, 200));
    }
    if (ostatninacisnietyklawisz == 2 && !Keyboard::isKeyPressed(Keyboard::A)
        && !Keyboard::isKeyPressed(Keyboard::Down) && keyTime1 > 0) {
        postac.setTextureRect(sf::IntRect(offsetidle, 1200, 200, 200));
    }
    if (ostatninacisnietyklawisz == 3 && !Keyboard::isKeyPressed(Keyboard::S)
        && !Keyboard::isKeyPressed(Keyboard::Left) && keyTime1 > 0) {
        postac.setTextureRect(sf::IntRect(offsetidle, 800, 200, 200));
    }
    if (ostatninacisnietyklawisz == 4 && !Keyboard::isKeyPressed(Keyboard::D)
        && !Keyboard::isKeyPressed(Keyboard::Right) && keyTime1 > 0) {
        postac.setTextureRect(sf::IntRect(offsetidle, 1400, 200, 200));
    }
}

void Kontrolerstrzalki(int& keyTime2, Sprite& postac, vector<Enemy>& enemies, Postac& postacliczenie, int& points)
{
    for (size_t i = 0; i < enemies.size(); i++)
    {
        if (((enemies[i].shape.getPosition().y - 75 > postac.getPosition().y - 100 &&
            enemies[i].shape.getPosition().y - 75 < postac.getPosition().y + 50 &&
            enemies[i].shape.getPosition().x + 75 > postac.getPosition().x + 25 &&
            enemies[i].shape.getPosition().x + 75 < postac.getPosition().x + 175))
            && ((Keyboard::isKeyPressed(Keyboard::Up) && postac.getPosition().y > border2 && keyTime2 > 27) ||
                ((Keyboard::isKeyPressed(Keyboard::Down) && postac.getPosition().y > border2 && keyTime2 > 27)) ||
                (Keyboard::isKeyPressed(Keyboard::Left) && postac.getPosition().y > border2 && keyTime2 > 27) ||
                ((Keyboard::isKeyPressed(Keyboard::Right) && postac.getPosition().y > border2 && keyTime2 > 27))))
        {
            enemies[i].HP = enemies[i].HP - 1;
            if (enemies[i].HP < 1)
            {
                enemies[i].shape.setPosition(100000, 100000);
                points++;
            }
        }
    }

    if (licznikblob == 2) {
        offsetwalka = offsetwalka + 200;
        if (offsetwalka >= 1200) { offsetwalka = 0; }
    }
    if (keyTime2 < 28)
        keyTime2++;
    if (Keyboard::isKeyPressed(Keyboard::Up) && postac.getPosition().y > border2 && keyTime2 > 27)
    {
        offsetwalka = 0;
        postac.move(0.f, -40.f);
        keyTime2 = 0;
        animacjawalka = 1;
    }
    if (Keyboard::isKeyPressed(Keyboard::Left) && postac.getPosition().x > border3 && keyTime2 > 27)
    {
        offsetwalka = 0;
        postac.move(-40.f, 0.f);
        keyTime2 = 0;
        animacjawalka = 2;
    }
    if (Keyboard::isKeyPressed(Keyboard::Down) && postac.getPosition().y < border4 && keyTime2 > 27)
    {
        offsetwalka = 0;
        postac.move(0.f, 40.f);
        keyTime2 = 0;
        animacjawalka = 3;
    }
    if (Keyboard::isKeyPressed(Keyboard::Right) && postac.getPosition().x < border1 && keyTime2 > 27)
    {
        offsetwalka = 0;
        postac.move(40.f, 0.f);
        keyTime2 = 0;
        animacjawalka = 4;
    }
    if (animacjawalka == 1) {
        postac.setTextureRect(sf::IntRect(offsetwalka, 1800, 200, 200));
    }
    if (animacjawalka == 2) {
        postac.setTextureRect(sf::IntRect(offsetwalka, 2000, 200, 200));
    }
    if (animacjawalka == 3) {
        postac.setTextureRect(sf::IntRect(offsetwalka, 1600, 200, 200));
    }
    if (animacjawalka == 4) {
        postac.setTextureRect(sf::IntRect(offsetwalka, 2200, 200, 200));
    }
    if (keyTime2 > 27)
    {
        animacjawalka = 0;
    }
}

void Blob(std::vector<Enemy>& enemies, Texture& enemyTex, RenderWindow& window,
    Sprite& postac, int& tilemapflip, Postac& postacliczenie, int diff, int roomnumber)
{
    if (licznikblob == 2) {
        offsetblob += 150;
        if (offsetblob >= 1500) { offsetblob = 0; }
    }
    for (size_t i = 0; i < enemies.size(); i++) {
        if (licznikblobwalka == 4 && enemies[i].stanblob == 5) {
            enemies[i].offsetblobwalka += 150;
            if (enemies[i].offsetblobwalka >= 1500) {
                enemies[i].offsetblobwalka = 0;
                enemies[i].stanblob = rand() % 4 + 1;
            }
        }
    }

    int difftemp = diff * 6;
    if (tilemapflip == 1) {
        for (size_t i = 0; i < (size_t)(rand() % difftemp + roomnumber); i++) {
            enemies.push_back(Enemy(&enemyTex)); // dodawanie blobow do gry
        }
        for (size_t i = 0; i < enemies.size(); i++) {
            enemies[i].shape.setPosition((rand() % 9 + 1) * 100, (rand() % 7 + 1) * 100);//losowa pozycja bloba
        }
    }

    //RUCH BLOBA
    for (size_t i = 0; i < enemies.size(); i++) //dla kazdego bloba
    {
        if (enemies[i].nblob == 0 && enemies[i].stanblob < 5)
            enemies[i].stanblob = rand() % 4 + 1; //losowanie kierunku bloba
        if (enemies[i].nblob == 0 && enemies[i].stanblob < 5)
            enemies[i].dlugoscruchu = rand() % 60 + 10; //losowanie dlugosci ruchu w klatkach

        if (enemies[i].shape.getPosition().y - 75 > postac.getPosition().y - 100
            && enemies[i].shape.getPosition().y - 75 < postac.getPosition().y + 50
            && enemies[i].shape.getPosition().x + 75 > postac.getPosition().x + 25
            && enemies[i].shape.getPosition().x + 75 < postac.getPosition().x + 175)
        {
            enemies[i].stanblob = 5;
            for (size_t j = 0; j < enemies.size(); j++) //dla kazdego bloba
            {
                if (enemies[j].offsetblobwalka > 1349 && enemies[j].offsetblobwalka < 1351)
                {
                    postacliczenie.HP = postacliczenie.HP - 1;
                    postac.setColor(sf::Color(255, 50, 50, 200));
                }
            }
        }

        //ruch w góre i sprawdzenie czy blob nie jest w ataku
        if (enemies[i].stanblob == 1 && enemies[i].shape.getPosition().y > -50
            && enemies[i].offsetblobwalka > -1 && enemies[i].offsetblobwalka < 1)
        {
            if (enemies[i].nblob < enemies[i].dlugoscruchu) //sprawdzamy ilosc klatek od dlugosci ruchu
            {
                enemies[i].shape.move(0.f, -6.f);
                enemies[i].stanblob = 1;
                if (enemies[i].shape.getPosition().y <= -50) //jak wychodzi za obszar zmiana pozycji
                    enemies[i].stanblob = 3;
            }
            if (licznikblob == 2) {
                enemies[i].shape.setTextureRect(sf::IntRect(offsetblob, 0, 150, 150));
            }
        }

        if (enemies[i].stanblob == 2 && enemies[i].shape.getPosition().x > 0
            && enemies[i].offsetblobwalka > -1 && enemies[i].offsetblobwalka < 1)
        {
            if (enemies[i].nblob < enemies[i].dlugoscruchu) {
                enemies[i].shape.move(-6.f, 0.f);
                enemies[i].stanblob = 2;
                if (enemies[i].shape.getPosition().x <= 0)
                    enemies[i].stanblob = 4;
            }
            if (licznikblob == 2) {
                enemies[i].shape.setTextureRect(sf::IntRect(offsetblob, 0, 150, 150));
            }
        }
        if (enemies[i].stanblob == 3 && enemies[i].shape.getPosition().y < 720
            && enemies[i].offsetblobwalka > -1 && enemies[i].offsetblobwalka < 1)
        {
            if (enemies[i].nblob < enemies[i].dlugoscruchu) {
                enemies[i].shape.move(0.f, 6.f);
                enemies[i].stanblob = 3;
                if (enemies[i].shape.getPosition().y >= 720)
                    enemies[i].stanblob = 1;
            }
            if (licznikblob == 2) {
                enemies[i].shape.setTextureRect(sf::IntRect(offsetblob, 0, 150, 150));
            }
        }
        if (enemies[i].stanblob == 4 && enemies[i].shape.getPosition().x < 1000
            && enemies[i].offsetblobwalka > -1 && enemies[i].offsetblobwalka < 1)
        {
            if (enemies[i].nblob < enemies[i].dlugoscruchu) {
                enemies[i].shape.move(6.f, 0.f);
                enemies[i].stanblob = 4;
                if (enemies[i].shape.getPosition().x >= 1000)
                    enemies[i].stanblob = 2;
            }
            if (licznikblob == 2) {
                enemies[i].shape.setTextureRect(sf::IntRect(offsetblob, 0, 150, 150));
            }
        }
        if (enemies[i].stanblob == 5)
        {
            if (enemies[i].nblob < enemies[i].dlugoscruchu) {
                if (licznikblobwalka == 4) {
                    enemies[i].shape.setTextureRect(sf::IntRect(enemies[i].offsetblobwalka, 150, 150, 150));
                }
            }
        }

        enemies[i].nblob++;
        if (enemies[i].nblob == enemies[i].dlugoscruchu)
        {
            enemies[i].nblob = 0;
        }
    }
}

void krzaki(std::vector<krzak>& krzakitab, Texture& krzakiTex, int& tilemapflip)
{
    if (tilemapflip == 1) {
        for (size_t i = 0; i < (size_t)(rand() % 5 + 1); i++)
            krzakitab.push_back(krzak(&krzakiTex));

        for (size_t i = 0; i < krzakitab.size(); i++) {
            krzakitab[i].krzaki.setPosition((rand() % 9 + 1) * 100, (rand() % 7 + 1) * 100);
        }
        for (size_t i = 0; i < krzakitab.size(); i++)
        {
            krzakitab[i].krzaki.setTextureRect(sf::IntRect((rand() % 2) * 200, 0, 200, 200));
        }
    }
}

void Draw(RenderWindow& window, Sprite& postac, const int& mapsizex, const int& mapsizey,
    std::vector<std::vector <sf::Sprite>>& tilemap, std::vector<Enemy>& enemies,
    std::vector<krzak>& krzakitab, Text text, Text text2, Sprite& hp, Text pauza)
{
    for (int x = 0; x < mapsizex; x++)
    {
        for (int y = 0; y < mapsizey; y++)
        {
            tilemap[x][y].setColor(sf::Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));
        }
    }
    for (size_t i = 0; i < enemies.size(); i++)
    {
        enemies[i].shape.setColor(sf::Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));
    }
    for (size_t i = 0; i < krzakitab.size(); i++)
    {
        krzakitab[i].krzaki.setColor(sf::Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));
    }
    window.clear();

    for (int x = 0; x < mapsizex; x++)
    {
        for (int y = 0; y < mapsizey; y++)
        {
            window.draw(tilemap[x][y]);
        }
    }
    for (size_t i = 0; i < enemies.size(); i++)
    {
        if (postac.getPosition().y >= enemies[i].shape.getPosition().y - 40)
            window.draw(enemies[i].shape);
    }
    for (size_t i = 0; i < krzakitab.size(); i++)
    {
        if (postac.getPosition().y >= krzakitab[i].krzaki.getPosition().y - 40)
            window.draw(krzakitab[i].krzaki);
    }
    window.draw(postac);

    for (size_t i = 0; i < enemies.size(); i++)
    {
        if (postac.getPosition().y <= enemies[i].shape.getPosition().y - 40)
            window.draw(enemies[i].shape);
    }
    for (size_t i = 0; i < krzakitab.size(); i++)
    {
        if (postac.getPosition().y <= krzakitab[i].krzaki.getPosition().y - 40)
            window.draw(krzakitab[i].krzaki);
    }

    window.draw(text);
    window.draw(text2);
    window.draw(hp);
    if (Keyboard::isKeyPressed(Keyboard::P))
    {
        window.draw(pauza);
    }
    window.display();
}

void Menuz(RenderWindow& window, Sprite& Menu, Sprite& nowagrasprite,
    Sprite& kontynuujsprite, Sprite& latwysprite, Sprite& trudnysprite, Text highscoret)
{
    window.clear();
    Menu.setTextureRect(sf::IntRect(0, 0, 1200, 1000));
    window.draw(Menu);
    window.draw(nowagrasprite);
    window.draw(kontynuujsprite);
    window.draw(latwysprite);
    window.draw(trudnysprite);
    window.draw(highscoret);
    window.display();
}

void Gameover(RenderWindow& window, Text przegrana)
{
    window.clear();
    przegrana.setString("[R]estart");
    window.draw(przegrana);
    window.display();
}

void Startpok(RenderWindow& window, Sprite& Start, Sprite& postac)
{
    window.clear();
    window.draw(Start);
    window.draw(postac);
    window.display();
}

void Mapa(const int& mapsizex, const int& mapsizey, std::vector<std::vector<sf::Sprite>>& tilemap, Sprite& mapa)
{
    float gridSizeF = 200.f;
    tilemap.resize(mapsizex, std::vector<Sprite>(mapsizey));

    for (int x = 0; x < mapsizex; x++)
    {
        for (int y = 0; y < mapsizey; y++)
        {
            tilemap[x][y] = sf::Sprite(mapa);
            tilemap[x][y].setPosition(x * gridSizeF, y * gridSizeF);
            //GORA
            if (y == 0)
            {
                tilemap[x][y].setTextureRect(IntRect(0, x * 200, 200, 200));
            }
            //DOL
            else if (y == mapsizey - 1)
            {
                tilemap[x][y].setTextureRect(IntRect(0, 4800 + x * 200, 200, 200));
            }
            //LEWO
            else if (x == 0)
            {
                int kafelekY = y * 1200;
                tilemap[x][y].setTextureRect(IntRect(0, kafelekY, 200, 200));
            }
            //PRAWO
            else if (x == mapsizex - 1)
            {
                int kafelekY = y * 1200 + 1000;
                tilemap[x][y].setTextureRect(IntRect(0, kafelekY, 200, 200));
            }
            else if (x > 0 && x < mapsizex - 1 && y > 0 && y < mapsizey - 1)
            {
                int losowyX = (rand() % 8) * 200;
                int losowyY = 1400 + (rand() % 4) * 200;
                tilemap[x][y].setTextureRect(IntRect(losowyX, losowyY, 200, 200));
                tilemap[x][y].setPosition(x * gridSizeF, y * gridSizeF);
            }
        }
    }
}
///////////////////
   //funkcje poboczne
//////////////////////////
string hudtext(int roomnumber)
{
    stringstream hudpunktyconv; //tworzy strumien tekstowy
    hudpunktyconv << roomnumber; //zapisuje do strumienia
    string hudpunkty;
    hudpunktyconv >> hudpunkty; //odczytuje strumien jako ciag znakow string
    return hudpunkty;
}

string hudtext2(int points)
{
    stringstream hudpunktyconv;
    hudpunktyconv << points * 100;
    string hudpunkty;
    hudpunktyconv >> hudpunkty;
    return hudpunkty;
}

string highscoreconvert()
{
    ifstream file("highscore.txt"); //otwarcie pliku
    int odczyt;
    file >> odczyt; //wczytuje liczbe z pliku
    odczyt = odczyt * 100;
    stringstream conv;
    conv << odczyt;
    string odczytstring;
    conv >> odczytstring;
    return odczytstring;
}

void hudhp(Postac& postacliczenie, Sprite& hp)
{
    if (postacliczenie.HP > 5)
    {
        hp.setTextureRect(sf::IntRect(0, 0, 300, 70));
    }
    if (postacliczenie.HP <= 5)
    {
        hp.setTextureRect(sf::IntRect(0, 0, 230, 70));
    }
}

void zapis(int diff, Postac& postacliczenie, int roomnumber, int points)
{
    ofstream file("save.txt");
    file << diff << " " << postacliczenie.HP << " " << roomnumber << " " << points;
    file.close();
}

void odczyt(int& diff, Postac& postacliczenie, int& roomnumber, int& points)
{
    ifstream file("save.txt");
    int zal1, zal2, zal3, zal4;
    file >> zal1 >> zal2 >> zal3 >> zal4;
    diff = zal1;
    postacliczenie.HP = zal2;
    roomnumber = zal3;
    points = zal4;
    file.close();
}

void highscore(int& points)
{
    int scorefromfile;
    ifstream file("highscore.txt");
    file >> scorefromfile;
    file.close();
    if (points > scorefromfile) {
        ofstream file("highscore.txt");
        file << points;
        file.close();
    }
}

bool przyciski(int xmax, int ymax, int xmin, int ymin, RenderWindow& window)
{
    Vector2f mp;
    mp.x = Mouse::getPosition(window).x;
    mp.y = Mouse::getPosition(window).y;
    if (mp.x > xmin && mp.x < xmax && mp.y > ymin && mp.y < ymax) {
        return true;
    }
    else
        return false;
}
