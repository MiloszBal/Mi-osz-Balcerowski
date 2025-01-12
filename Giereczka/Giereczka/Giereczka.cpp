#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <time.h>

using namespace sf;
using namespace std;

int border1 = 940;
int border2 = -40;
int border3 = 60;
int border4 = 690;

int licznik = 0;
int licznikprzejscia = 250;
int licznikprzejsciastart = 0;
int licznikprzejsciastart2 = 0;

int animacjawalka = 0;
int offsetchodzenie = 0;
int offsetidle = 0;
int offsetwalka = 0;
int ostatninacisnietyklawisz = 3;

class Postac {
public:
    int HP;
    int HPMax;
    Postac() {
        HPMax = 8;
        HP = HPMax;
    }
    ~Postac() {}
};

class krzak {
public:
    Sprite krzaki;
    krzak(Texture* texture) {
        krzaki.setTexture(*texture);
        krzaki.setTextureRect(IntRect(0, 0, 200, 200));
    }
    ~krzak() {}
};

void Kontrolerwasd(int& keyTime1, Sprite& postac, int& keyTime2);
void Kontrolerstrzalki(int& keyTime2, Sprite& postac, Postac& postacliczenie, int& points);
void Draw(RenderWindow& window, Sprite& postac, const int& mapsizex, const int& mapsizey,
    vector<vector<Sprite>>& tilemap, vector<krzak>& krzakitab, Text text, Text text2,
    Sprite& hp, Text pauza);
void krzaki(vector<krzak>& krzakitab, Texture& krzakiTex, int& tilemapflip);
void Menuz(RenderWindow& window, Sprite& Menu, Sprite& nowagrasprite, Sprite& kontynuujsprite,
    Sprite& latwysprite, Sprite& trudnysprite, Text highscoret);
void Gameover(RenderWindow& window, Text przegrana);
void Startpok(RenderWindow& window, Sprite& Start, Sprite& postac);
void Mapa(const int& mapsizex, const int& mapsizey, vector<vector<Sprite>>& tilemap, Sprite& mapa);

string hudtext(int roomnumber) {
    stringstream hudpunktyconv;
    hudpunktyconv << roomnumber;
    string hudpunkty;
    hudpunktyconv >> hudpunkty;
    return hudpunkty;
}

string hudtext2(int points) {
    stringstream hudpunktyconv;
    hudpunktyconv << points * 100;
    string hudpunkty;
    hudpunktyconv >> hudpunkty;
    return hudpunkty;
}

string highscoreconvert() {
    ifstream file("highscore.txt");
    int odczyt;
    file >> odczyt;
    odczyt = odczyt * 100;
    stringstream conv;
    conv << odczyt;
    string odczytstring;
    conv >> odczytstring;
    return odczytstring;
}

void hudhp(Postac& postacliczenie, Sprite& hp) {
    if (postacliczenie.HP > 5) {
        hp.setTextureRect(IntRect(0, 0, 300, 70));
    }
    if (postacliczenie.HP <= 5) {
        hp.setTextureRect(IntRect(0, 0, 230, 70));
    }
}

void zapis(int diff, Postac& postacliczenie, int roomnumber, int points) {
    ofstream file("save.txt");
    file << diff << " " << postacliczenie.HP << " " << roomnumber << " " << points;
    file.close();
}

void odczyt(int& diff, Postac& postacliczenie, int& roomnumber, int& points) {
    ifstream file("save.txt");
    int zal1, zal2, zal3, zal4;
    file >> zal1 >> zal2 >> zal3 >> zal4;
    diff = zal1;
    postacliczenie.HP = zal2;
    roomnumber = zal3;
    points = zal4;
    file.close();
}

void highscore(int& points) {
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

bool przyciski(int xmax, int ymax, int xmin, int ymin, RenderWindow& window) {
    Vector2f mp;
    mp.x = Mouse::getPosition(window).x;
    mp.y = Mouse::getPosition(window).y;
    if (mp.x > xmin && mp.x < xmax && mp.y > ymin && mp.y < ymax) {
        return true;
    }
    else {
        return false;
    }
}

int main() {
    int gamestate = 0;
    int tilemapflip = 0;
    int roomnumber = 0;
    int points = 0;
    int diff = 1;
    srand(time(NULL));
    int keyTime1 = 1;
    int keyTime2 = 28;

    RenderWindow window(VideoMode(1200, 1000), "", Style::None);
    window.setFramerateLimit(60);

    Font font;
    font.loadFromFile("Amatic-Bold.ttf");

    Texture textury;
    Sprite mapa;
    textury.loadFromFile("mapa.png");
    mapa.setTexture(textury);
    const int mapsizex = 6;
    const int mapsizey = 5;
    vector<vector<Sprite>> tilemap(mapsizex, vector<Sprite>(mapsizey, Sprite()));

    Texture postactextury;
    Sprite postac;
    postac.setPosition(500, 320);
    postactextury.loadFromFile("ORC.png");
    postac.setTexture(postactextury);
    Postac postacliczenie;

    Texture hudhptext;
    Sprite hp;
    hudhptext.loadFromFile("Hp.png");
    hp.setTexture(hudhptext);
    hp.setPosition(800.f, 915.f);

    Texture menu;
    Sprite Menu;
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

    Texture krzakiTex;
    krzakiTex.loadFromFile("krzaki.png");
    vector<krzak> krzakitab;

    Texture start;
    Sprite Start;
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

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                window.close();
            }
        }
        switch (gamestate) {
        case 0:
            gamestate = 1;
        case 1:
            if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                licznikprzejsciastart = 1;
            }
            if (przyciski(540, 600, 230, 400, window) && Mouse::isButtonPressed(Mouse::Left)) {
                licznikprzejsciastart = 1;
            }
            if (przyciski(540, 600, 230, 400, window)) {
                nowagrasprite.setColor(Color(255, 0, 0));
            }
            else {
                nowagrasprite.setColor(Color(0, 0, 255));
            }
            if (licznikprzejsciastart == 1) {
                if (licznikprzejscia > 0) licznikprzejscia -= 10;
                Menu.setColor(Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));
                if (licznikprzejscia == 0) {
                    gamestate = 2;
                    licznikprzejsciastart = 0;
                }
            }
            if ((przyciski(500, 740, 400, 680, window) && Mouse::isButtonPressed(Mouse::Left)) ||
                Keyboard::isKeyPressed(Keyboard::Num1)) {
                diff = 1;
            }
            if (przyciski(500, 740, 400, 680, window)) {
                latwysprite.setColor(Color(255, 0, 0));
            }
            else {
                latwysprite.setColor(Color(0, 0, 255));
            }
            if ((przyciski(780, 740, 630, 680, window) && Mouse::isButtonPressed(Mouse::Left)) ||
                Keyboard::isKeyPressed(Keyboard::Num2)) {
                diff = 3;
            }
            if (przyciski(780, 740, 630, 680, window)) {
                trudnysprite.setColor(Color(255, 0, 0));
            }
            else {
                trudnysprite.setColor(Color(0, 0, 255));
            }
            highscoret.setString(highscoreconvert());
            if ((przyciski(900, 600, 600, 400, window) && Mouse::isButtonPressed(Mouse::Left)) ||
                Keyboard::isKeyPressed(Keyboard::E)) {
                postac.setPosition(500, 900);
                licznikprzejsciastart2 = 1;
                border1 = 1000;
                border2 = -50;
                border3 = 0;
                border4 = 720;
                tilemapflip = 1;
            }
            if (przyciski(900, 600, 600, 400, window)) {
                kontynuujsprite.setColor(Color(255, 0, 0));
            }
            else {
                kontynuujsprite.setColor(Color(0, 0, 255));
            }
            if (licznikprzejsciastart2 == 1) {
                if (licznikprzejscia > 0) licznikprzejscia -= 10;
                Menu.setColor(Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));
                if (licznikprzejscia == 0) {
                    gamestate = 3;
                    odczyt(diff, postacliczenie, roomnumber, points);
                    licznikprzejsciastart2 = 0;
                }
            }
            Menuz(window, Menu, nowagrasprite, kontynuujsprite, latwysprite, trudnysprite, highscoret);
            Menu.setColor(Color(255, 255, 255));
            break;
        case 2:
            if (licznikprzejsciastart == 0) {
                if (licznikprzejscia < 250) licznikprzejscia += 10;
                Start.setColor(Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));
                postac.setColor(Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));
            }
            Startpok(window, Start, postac);
            if (licznikprzejsciastart == 0) {
                Kontrolerstrzalki(keyTime2, postac, postacliczenie, points);
                Kontrolerwasd(keyTime1, postac, keyTime2);
            }
            licznik++;
            if (licznik > 5) licznik = 0;
            if (Keyboard::isKeyPressed(Keyboard::W) &&
                postac.getPosition().x > 330 && postac.getPosition().x < 640 &&
                postac.getPosition().y < -30) {
                licznikprzejsciastart = 1;
            }
            if (licznikprzejsciastart == 1) {
                if (licznikprzejscia > 0) licznikprzejscia -= 10;
                postac.setColor(Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));
                Start.setColor(Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));
                postac.move(0.f, -7.f);
                postac.setTextureRect(IntRect(offsetchodzenie, 200, 200, 200));
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
        case 3:
            krzaki(krzakitab, krzakiTex, tilemapflip);
            Draw(window, postac, mapsizex, mapsizey, tilemap, krzakitab, text, text2, hp, pauza);
            if (!Keyboard::isKeyPressed(Keyboard::P)) {
                if (licznikprzejsciastart == 1) {
                    Kontrolerwasd(keyTime1, postac, keyTime2);
                    Kontrolerstrzalki(keyTime2, postac, postacliczenie, points);
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::V)) {
                zapis(diff, postacliczenie, roomnumber, points);
            }
            if (tilemapflip == 1) {
                Mapa(mapsizex, mapsizey, tilemap, mapa);
            }
            if (licznikprzejsciastart == 0) {
                if (licznikprzejscia < 248) {
                    licznikprzejscia += 10;
                    postac.setColor(Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));
                    postac.move(0.f, -7.f);
                    postac.setTextureRect(IntRect(offsetchodzenie, 200, 200, 200));
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
            if (Keyboard::isKeyPressed(Keyboard::W) &&
                postac.getPosition().x > 330 && postac.getPosition().x < 640 &&
                postac.getPosition().y < -30) {
                licznikprzejscia = 1;
            }
            tilemapflip = 0;
            if (licznikprzejscia == 1) {
                postac.setPosition(500, 900);
                roomnumber++;
                points += 3;
                border1 = 1000;
                border2 = -50;
                border3 = 0;
                border4 = 720;
                tilemapflip = 1;
                gamestate = 5;
                licznikprzejsciastart = 0;
                for (size_t i = 0; i < krzakitab.size(); i++) {
                    krzakitab[i].krzaki.setPosition(100000, 100000);
                }
                krzakitab.clear();
            }
            licznik++;
            if (licznik > 5) licznik = 0;
            break;
        case 4:
            if (licznikprzejsciastart == 1) {
                przegrana.setFillColor(Color(250, 250, 250));
            }
            if (licznikprzejsciastart == 0) {
                if (licznikprzejscia > 0) licznikprzejscia -= 10;
                przegrana.setFillColor(Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));
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
            }
            krzakitab.clear();
            if (Keyboard::isKeyPressed(Keyboard::R)) {
                postac.setPosition(500, 320);
                postacliczenie.HP = 8;
                licznikprzejsciastart = 0;
            }
            break;
        case 5:
            if (tilemapflip == 1) {
                Mapa(mapsizex, mapsizey, tilemap, mapa);
            }
            if (licznikprzejsciastart == 0) {
                if (licznikprzejscia < 248) {
                    licznikprzejscia += 10;
                    postac.setColor(Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));
                    postac.move(0.f, -7.f);
                    postac.setTextureRect(IntRect(offsetchodzenie, 200, 200, 200));
                }
                if (licznikprzejscia > 249) licznikprzejsciastart = 1;
            }
            if (Keyboard::isKeyPressed(Keyboard::V)) {
                zapis(diff, postacliczenie, roomnumber, points);
            }
            krzaki(krzakitab, krzakiTex, tilemapflip);
            if (!Keyboard::isKeyPressed(Keyboard::P)) {
                if (licznikprzejsciastart == 1) {
                    Kontrolerwasd(keyTime1, postac, keyTime2);
                    Kontrolerstrzalki(keyTime2, postac, postacliczenie, points);
                }
            }
            Draw(window, postac, mapsizex, mapsizey, tilemap, krzakitab, text, text2, hp, pauza);
            text.setString(hudtext(roomnumber));
            text2.setString(hudtext2(points));
            hudhp(postacliczenie, hp);
            if (Keyboard::isKeyPressed(Keyboard::W) &&
                postac.getPosition().x > 330 && postac.getPosition().x < 640 &&
                postac.getPosition().y < -30) {
                licznikprzejscia = 1;
            }
            tilemapflip = 0;
            if (licznikprzejscia == 1) {
                postac.setPosition(500, 900);
                roomnumber++;
                points += 3;
                border1 = 1000;
                border2 = -50;
                border3 = 0;
                border4 = 720;
                tilemapflip = 1;
                gamestate = 3;
                licznikprzejsciastart = 0;
                for (size_t i = 0; i < krzakitab.size(); i++) {
                    krzakitab[i].krzaki.setPosition(100000, 100000);
                }
                krzakitab.clear();
            }
            if (postacliczenie.HP < 1) {
                licznikprzejscia = 250;
                gamestate = 4;
            }
            licznik++;
            if (licznik > 5) licznik = 0;
            break;
        }
    }
    return 0;
}

void Kontrolerwasd(int& keyTime1, Sprite& postac, int& keyTime2) {
    if (keyTime1 < 4) keyTime1++;
    if (licznik == 4) {
        offsetchodzenie += 200;
        if (offsetchodzenie >= 1000) offsetchodzenie = 0;
    }
    if (licznik == 4) {
        offsetidle += 200;
        if (offsetidle >= 600) offsetidle = 0;
    }
    if (keyTime2 < 28) keyTime1 = 0;
    if (Keyboard::isKeyPressed(Keyboard::W) && Keyboard::isKeyPressed(Keyboard::D) &&
        postac.getPosition().x < border1 && postac.getPosition().y > border2 &&
        !Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::S) && keyTime1 > 0) {
        postac.move(5.f, -5.f);
        postac.setTextureRect(IntRect(offsetchodzenie, 200, 200, 200));
        ostatninacisnietyklawisz = 1;
        keyTime1 = 0;
    }
    if (Keyboard::isKeyPressed(Keyboard::W) && postac.getPosition().y > border2 &&
        !Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::D) &&
        !Keyboard::isKeyPressed(Keyboard::S) && keyTime1 > 0) {
        postac.move(0.f, -7.f);
        postac.setTextureRect(IntRect(offsetchodzenie, 200, 200, 200));
        ostatninacisnietyklawisz = 1;
        keyTime1 = 0;
    }
    if (Keyboard::isKeyPressed(Keyboard::W) && Keyboard::isKeyPressed(Keyboard::A) &&
        postac.getPosition().y > border2 && postac.getPosition().x > border3 &&
        !Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::S) &&
        keyTime1 > 0) {
        postac.move(-5.f, -5.f);
        postac.setTextureRect(IntRect(offsetchodzenie, 200, 200, 200));
        ostatninacisnietyklawisz = 1;
        keyTime1 = 0;
    }
    if (Keyboard::isKeyPressed(Keyboard::A) && postac.getPosition().x > border3 &&
        !Keyboard::isKeyPressed(Keyboard::S) && !Keyboard::isKeyPressed(Keyboard::W) &&
        !Keyboard::isKeyPressed(Keyboard::D) && keyTime1 > 0) {
        postac.move(-7.f, 0.f);
        postac.setTextureRect(IntRect(offsetchodzenie, 400, 200, 200));
        ostatninacisnietyklawisz = 2;
        keyTime1 = 0;
    }
    if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::S) &&
        postac.getPosition().y < border4 && postac.getPosition().x > border3 &&
        !Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::W) &&
        keyTime1 > 0) {
        postac.move(-5.f, 5.f);
        postac.setTextureRect(IntRect(offsetchodzenie, 0, 200, 200));
        ostatninacisnietyklawisz = 1;
        keyTime1 = 0;
    }
    if (Keyboard::isKeyPressed(Keyboard::S) && postac.getPosition().y < border4 &&
        !Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::D) &&
        !Keyboard::isKeyPressed(Keyboard::W) && keyTime1 > 0) {
        postac.move(0.f, 7.f);
        postac.setTextureRect(IntRect(offsetchodzenie, 0, 200, 200));
        ostatninacisnietyklawisz = 3;
        keyTime1 = 0;
    }
    if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::S) &&
        postac.getPosition().y < border4 && postac.getPosition().x < border1 &&
        !Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::W) &&
        keyTime1 > 0) {
        postac.move(5.f, 5.f);
        postac.setTextureRect(IntRect(offsetchodzenie, 0, 200, 200));
        ostatninacisnietyklawisz = 1;
        keyTime1 = 0;
    }
    if (Keyboard::isKeyPressed(Keyboard::D) && postac.getPosition().x < border1 &&
        !Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::S) &&
        !Keyboard::isKeyPressed(Keyboard::W) && keyTime1 > 0) {
        postac.move(7.f, 0.f);
        postac.setTextureRect(IntRect(offsetchodzenie, 600, 200, 200));
        ostatninacisnietyklawisz = 4;
        keyTime1 = 0;
    }
    if (ostatninacisnietyklawisz == 1 && !Keyboard::isKeyPressed(Keyboard::W) &&
        keyTime1 > 0) {
        postac.setTextureRect(IntRect(offsetidle, 1000, 200, 200));
    }
    if (ostatninacisnietyklawisz == 2 && !Keyboard::isKeyPressed(Keyboard::A) &&
        keyTime1 > 0) {
        postac.setTextureRect(IntRect(offsetidle, 1200, 200, 200));
    }
    if (ostatninacisnietyklawisz == 3 && !Keyboard::isKeyPressed(Keyboard::S) &&
        keyTime1 > 0) {
        postac.setTextureRect(IntRect(offsetidle, 800, 200, 200));
    }
    if (ostatninacisnietyklawisz == 4 && !Keyboard::isKeyPressed(Keyboard::D) &&
        keyTime1 > 0) {
        postac.setTextureRect(IntRect(offsetidle, 1400, 200, 200));
    }
}

void Kontrolerstrzalki(int& keyTime2, Sprite& postac, Postac& postacliczenie, int& points) {
    if (keyTime2 < 28) keyTime2++;
    if (Keyboard::isKeyPressed(Keyboard::Up) && postac.getPosition().y > border2 && keyTime2 > 27) {
        offsetwalka = 0;
        postac.move(0.f, -40.f);
        keyTime2 = 0;
        animacjawalka = 1;
    }
    if (Keyboard::isKeyPressed(Keyboard::Left) && postac.getPosition().x > border3 && keyTime2 > 27) {
        offsetwalka = 0;
        postac.move(-40.f, 0.f);
        keyTime2 = 0;
        animacjawalka = 2;
    }
    if (Keyboard::isKeyPressed(Keyboard::Down) && postac.getPosition().y < border4 && keyTime2 > 27) {
        offsetwalka = 0;
        postac.move(0.f, 40.f);
        keyTime2 = 0;
        animacjawalka = 3;
    }
    if (Keyboard::isKeyPressed(Keyboard::Right) && postac.getPosition().x < border1 && keyTime2 > 27) {
        offsetwalka = 0;
        postac.move(40.f, 0.f);
        keyTime2 = 0;
        animacjawalka = 4;
    }
    if (animacjawalka == 1) {
        postac.setTextureRect(IntRect(offsetwalka, 1800, 200, 200));
    }
    if (animacjawalka == 2) {
        postac.setTextureRect(IntRect(offsetwalka, 2000, 200, 200));
    }
    if (animacjawalka == 3) {
        postac.setTextureRect(IntRect(offsetwalka, 1600, 200, 200));
    }
    if (animacjawalka == 4) {
        postac.setTextureRect(IntRect(offsetwalka, 2200, 200, 200));
    }
    if (keyTime2 > 27) {
        animacjawalka = 0;
    }
}

void krzaki(vector<krzak>& krzakitab, Texture& krzakiTex, int& tilemapflip) {
    if (tilemapflip == 1) {
        for (size_t i = 0; i < rand() % 5 + 1; i++)
            krzakitab.push_back(krzak(&krzakiTex));
        for (size_t i = 0; i < krzakitab.size(); i++) {
            krzakitab[i].krzaki.setPosition((rand() % 9 + 1) * 100, (rand() % 7 + 1) * 100);
            krzakitab[i].krzaki.setTextureRect(IntRect((rand() % 2) * 200, 0, 200, 200));
        }
    }
}

void Draw(RenderWindow& window, Sprite& postac, const int& mapsizex, const int& mapsizey,
    vector<vector<Sprite>>& tilemap, vector<krzak>& krzakitab, Text text, Text text2,
    Sprite& hp, Text pauza) {
    for (int x = 0; x < mapsizex; x++) {
        for (int y = 0; y < mapsizey; y++) {
            tilemap[x][y].setColor(Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));
        }
    }
    for (size_t i = 0; i < krzakitab.size(); i++) {
        krzakitab[i].krzaki.setColor(Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));
    }
    window.clear();
    for (int x = 0; x < mapsizex; x++) {
        for (int y = 0; y < mapsizey; y++) {
            window.draw(tilemap[x][y]);
        }
    }
    for (size_t i = 0; i < krzakitab.size(); i++) {
        if (postac.getPosition().y >= krzakitab[i].krzaki.getPosition().y - 40)
            window.draw(krzakitab[i].krzaki);
    }
    window.draw(postac);
    for (size_t i = 0; i < krzakitab.size(); i++) {
        if (postac.getPosition().y <= krzakitab[i].krzaki.getPosition().y - 40)
            window.draw(krzakitab[i].krzaki);
    }
    window.draw(text);
    window.draw(text2);
    window.draw(hp);
    if (Keyboard::isKeyPressed(Keyboard::P)) {
        window.draw(pauza);
    }
    window.display();
}

void Menuz(RenderWindow& window, Sprite& Menu, Sprite& nowagrasprite, Sprite& kontynuujsprite,
    Sprite& latwysprite, Sprite& trudnysprite, Text highscoret) {
    window.clear();
    Menu.setTextureRect(IntRect(0, 0, 1200, 1000));
    window.draw(Menu);
    window.draw(nowagrasprite);
    window.draw(kontynuujsprite);
    window.draw(latwysprite);
    window.draw(trudnysprite);
    window.draw(highscoret);
    window.display();
}

void Gameover(RenderWindow& window, Text przegrana) {
    window.clear();
    przegrana.setString("[R]estart");
    window.draw(przegrana);
    window.display();
}

void Startpok(RenderWindow& window, Sprite& Start, Sprite& postac) {
    window.clear();
    window.draw(Start);
    window.draw(postac);
    window.display();
}

void Mapa(const int& mapsizex, const int& mapsizey, vector<vector<Sprite>>& tilemap, Sprite& mapa) {
    float gridSizeF = 200.f;
    for (int x = 0; x < mapsizex; x++) {
        for (int y = 0; y < mapsizey; y++) {
            tilemap[0][0] = Sprite(mapa);
            tilemap[0][0].setTextureRect(IntRect(0, 0, 200, 200));
            tilemap[0][0].setPosition(0 * gridSizeF, 0 * gridSizeF);

            tilemap[0][1] = Sprite(mapa);
            tilemap[0][1].setTextureRect(IntRect(0, 1200, 200, 200));
            tilemap[0][1].setPosition(0 * gridSizeF, 1 * gridSizeF);

            tilemap[0][2] = Sprite(mapa);
            tilemap[0][2].setTextureRect(IntRect(0, 2400, 200, 200));
            tilemap[0][2].setPosition(0 * gridSizeF, 2 * gridSizeF);

            tilemap[0][3] = Sprite(mapa);
            tilemap[0][3].setTextureRect(IntRect(0, 3600, 200, 200));
            tilemap[0][3].setPosition(0 * gridSizeF, 3 * gridSizeF);

            tilemap[0][4] = Sprite(mapa);
            tilemap[0][4].setTextureRect(IntRect(0, 4800, 200, 200));
            tilemap[0][4].setPosition(0 * gridSizeF, 4 * gridSizeF);

            tilemap[1][0] = Sprite(mapa);
            tilemap[1][0].setTextureRect(IntRect(0, 200, 200, 200));
            tilemap[1][0].setPosition(1 * gridSizeF, 0 * gridSizeF);

            tilemap[1][1] = Sprite(mapa);
            tilemap[1][1].setTextureRect(IntRect(rand() % 9 * 200, 1400, 200, 200));
            tilemap[1][1].setPosition(1 * gridSizeF, 1 * gridSizeF);

            tilemap[1][2] = Sprite(mapa);
            tilemap[1][2].setTextureRect(IntRect(rand() % 9 * 200, 2600, 200, 200));
            tilemap[1][2].setPosition(1 * gridSizeF, 2 * gridSizeF);

            tilemap[1][3] = Sprite(mapa);
            tilemap[1][3].setTextureRect(IntRect(rand() % 9 * 200, 3800, 200, 200));
            tilemap[1][3].setPosition(1 * gridSizeF, 3 * gridSizeF);

            tilemap[1][4] = Sprite(mapa);
            tilemap[1][4].setTextureRect(IntRect(0, 5000, 200, 200));
            tilemap[1][4].setPosition(1 * gridSizeF, 4 * gridSizeF);

            tilemap[2][0] = Sprite(mapa);
            tilemap[2][0].setTextureRect(IntRect(0, 400, 200, 200));
            tilemap[2][0].setPosition(2 * gridSizeF, 0 * gridSizeF);

            tilemap[2][1] = Sprite(mapa);
            tilemap[2][1].setTextureRect(IntRect(rand() % 9 * 200, 1600, 200, 200));
            tilemap[2][1].setPosition(2 * gridSizeF, 1 * gridSizeF);

            tilemap[2][2] = Sprite(mapa);
            tilemap[2][2].setTextureRect(IntRect(rand() % 9 * 200, 2800, 200, 200));
            tilemap[2][2].setPosition(2 * gridSizeF, 2 * gridSizeF);

            tilemap[2][3] = Sprite(mapa);
            tilemap[2][3].setTextureRect(IntRect(rand() % 9 * 200, 4000, 200, 200));
            tilemap[2][3].setPosition(2 * gridSizeF, 3 * gridSizeF);

            tilemap[2][4] = Sprite(mapa);
            tilemap[2][4].setTextureRect(IntRect(0, 5200, 200, 200));
            tilemap[2][4].setPosition(2 * gridSizeF, 4 * gridSizeF);

            tilemap[3][0] = Sprite(mapa);
            tilemap[3][0].setTextureRect(IntRect(0, 600, 200, 200));
            tilemap[3][0].setPosition(3 * gridSizeF, 0 * gridSizeF);

            tilemap[3][1] = Sprite(mapa);
            tilemap[3][1].setTextureRect(IntRect(rand() % 9 * 200, 1800, 200, 200));
            tilemap[3][1].setPosition(3 * gridSizeF, 1 * gridSizeF);

            tilemap[3][2] = Sprite(mapa);
            tilemap[3][2].setTextureRect(IntRect(rand() % 9 * 200, 3000, 200, 200));
            tilemap[3][2].setPosition(3 * gridSizeF, 2 * gridSizeF);

            tilemap[3][3] = Sprite(mapa);
            tilemap[3][3].setTextureRect(IntRect(rand() % 9 * 200, 4200, 200, 200));
            tilemap[3][3].setPosition(3 * gridSizeF, 3 * gridSizeF);

            tilemap[3][4] = Sprite(mapa);
            tilemap[3][4].setTextureRect(IntRect(0, 5400, 200, 200));
            tilemap[3][4].setPosition(3 * gridSizeF, 4 * gridSizeF);

            tilemap[4][0] = Sprite(mapa);
            tilemap[4][0].setTextureRect(IntRect(0, 800, 200, 200));
            tilemap[4][0].setPosition(4 * gridSizeF, 0 * gridSizeF);

            tilemap[4][1] = Sprite(mapa);
            tilemap[4][1].setTextureRect(IntRect(rand() % 9 * 200, 2000, 200, 200));
            tilemap[4][1].setPosition(4 * gridSizeF, 1 * gridSizeF);

            tilemap[4][2] = Sprite(mapa);
            tilemap[4][2].setTextureRect(IntRect(rand() % 9 * 200, 3200, 200, 200));
            tilemap[4][2].setPosition(4 * gridSizeF, 2 * gridSizeF);

            tilemap[4][3] = Sprite(mapa);
            tilemap[4][3].setTextureRect(IntRect(rand() % 9 * 200, 4400, 200, 200));
            tilemap[4][3].setPosition(4 * gridSizeF, 3 * gridSizeF);

            tilemap[4][4] = Sprite(mapa);
            tilemap[4][4].setTextureRect(IntRect(0, 5600, 200, 200));
            tilemap[4][4].setPosition(4 * gridSizeF, 4 * gridSizeF);

            tilemap[5][0] = Sprite(mapa);
            tilemap[5][0].setTextureRect(IntRect(0, 1000, 200, 200));
            tilemap[5][0].setPosition(5 * gridSizeF, 0 * gridSizeF);

            tilemap[5][1] = Sprite(mapa);
            tilemap[5][1].setTextureRect(IntRect(0, 2200, 200, 200));
            tilemap[5][1].setPosition(5 * gridSizeF, 1 * gridSizeF);

            tilemap[5][2] = Sprite(mapa);
            tilemap[5][2].setTextureRect(IntRect(0, 3400, 200, 200));
            tilemap[5][2].setPosition(5 * gridSizeF, 2 * gridSizeF);

            tilemap[5][3] = Sprite(mapa);
            tilemap[5][3].setTextureRect(IntRect(0, 4600, 200, 200));
            tilemap[5][3].setPosition(5 * gridSizeF, 3 * gridSizeF);

            tilemap[5][4] = Sprite(mapa);
            tilemap[5][4].setTextureRect(IntRect(0, 5800, 200, 200));
            tilemap[5][4].setPosition(5 * gridSizeF, 4 * gridSizeF);
        }
    }
}
