#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>       
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

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

// Animacje i stany
int offsetchodzenie = 0;
int offsetidle = 0;
int ostatninacisnietyklawisz = 3;

// ----------------------------------------------------
// PROTOTYPY FUNKCJI
// ----------------------------------------------------
void Kontrolerwasd(int& keyTime1, Sprite& postac);
void Menuz(RenderWindow& window, Sprite& Menu, Sprite& nowagrasprite);
void Startpok(RenderWindow& window, Sprite& Start, Sprite& postac);

// ----------------------------------------------------
// FUNKCJA SPRAWDZAJ�CA, CZY KURSOR JEST W OBR�BIE PRZYCISKU
// ----------------------------------------------------
bool przyciski(int xmax, int ymax, int xmin, int ymin, RenderWindow& window)
{
    Vector2f mp;
    mp.x = Mouse::getPosition(window).x;
    mp.y = Mouse::getPosition(window).y;
    if (mp.x > xmin && mp.x < xmax && mp.y > ymin && mp.y < ymax)
        return true;
    return false;
}

// ----------------------------------------------------
// FUNKCJA main()
// ----------------------------------------------------
int main()
{
    // Stan gry: 0 -> inicjalizacja, 1 -> menu, 2 -> pierwszy etap
    int gamestate = 0;

    // Kluczowe zmienne do animacji ruchu postaci
    int keyTime1 = 1;

    // Tworzymy okno
    RenderWindow window(VideoMode(1200, 1000), "", Style::None);
    window.setFramerateLimit(60);

    // �adujemy czcionk� (je�li chcesz co� wypisa� na ekranie)
    Font font;
    font.loadFromFile("Amatic-Bold.ttf");

    // Tekstura i sprite dla menu
    Texture menu;
    menu.loadFromFile("menu.jpg");
    Sprite Menu;
    Menu.setTexture(menu);

    // Przycisk "Nowa gra"
    Texture nowagra;
    nowagra.loadFromFile("button.png");
    Sprite nowagrasprite;
    nowagrasprite.setTexture(nowagra);
    nowagrasprite.setPosition(230.f, 480.f);

    // Tekstura i sprite "Start" (ekran z postaci�)
    Texture start;
    start.loadFromFile("start.png");
    Sprite Start;
    Start.setTexture(start);

    // Tekstura i sprite dla naszej postaci
    Texture postactextury;
    postactextury.loadFromFile("orc3_walk_full.png");
    Sprite postac;
    postac.setPosition(500, 320);
    postac.setTexture(postactextury);

    // ------------------------------------------------
    // P�tla g��wna
    // ------------------------------------------------
    while (window.isOpen())
    {
        // Obs�uga event�w okna
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                window.close();
        }

        switch (gamestate)
        {
        case 0: // Inicjalizacja stanu gry, przej�cie do menu
            gamestate = 1;
            break;

        case 1: // MENU
        {
            // Pod�wietlanie przycisku "Nowa gra"
            if (przyciski(540, 600, 230, 400, window))
                nowagrasprite.setColor(Color(255, 0, 0));
            else
                nowagrasprite.setColor(Color(0, 0, 255));

            // Obs�uga klikni�cia mysz� w "Nowa gra"
            if (przyciski(540, 600, 230, 400, window) && Mouse::isButtonPressed(Mouse::Left))
            {
                licznikprzejsciastart = 1;
            }

            // Animacja wygaszenia menu i przej�cie do stanu 2
            if (licznikprzejsciastart == 1)
            {
                if (licznikprzejscia > 0)
                    licznikprzejscia -= 10;

                Menu.setColor(sf::Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));

                if (licznikprzejscia == 0)
                {
                    gamestate = 2;
                    licznikprzejsciastart = 0;
                    licznikprzejscia = 250; // ewentualnie przywracamy do stanu wyj�ciowego
                }
            }

            // Rysujemy menu na ekranie
            Menuz(window, Menu, nowagrasprite);
        }
        break;

        case 2: // PIERWSZY ETAP GRY
        {
            // Prosta animacja "pojawiania si�" ekranu startowego i postaci
            if (licznikprzejsciastart == 0)
            {
                if (licznikprzejscia < 250)
                    licznikprzejscia += 10;

                Start.setColor(Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));
                postac.setColor(Color(licznikprzejscia, licznikprzejscia, licznikprzejscia));
            }

            // Sterowanie WASD postaci�
            Kontrolerwasd(keyTime1, postac);

            // Rysowanie
            window.clear();
            window.draw(Start);
            window.draw(postac);
            window.display();
        }
        break;
        } // switch
    } // while (window.isOpen())

    return 0;
}

// ----------------------------------------------------
// Definicje funkcji
// ----------------------------------------------------

// WY�WIETLANIE MENU
void Menuz(RenderWindow& window, Sprite& Menu, Sprite& nowagrasprite)
{
    window.clear();
    Menu.setTextureRect(sf::IntRect(0, 0, 1200, 1000));
    window.draw(Menu);
    window.draw(nowagrasprite);
    window.display();
}

// EKRAN STARTOWY (t�o + posta�); tutaj minimalny przyk�ad
void Startpok(RenderWindow& window, Sprite& Start, Sprite& postac)
{
    window.clear();
    window.draw(Start);
    window.draw(postac);
    window.display();
}

// OBS�UGA RUCHU (WASD) � usuni�to zb�dne rzeczy zwi�zane z walk� i kolejnymi planszami
void Kontrolerwasd(int& keyTime1, Sprite& postac)
{
    // Prosta animacja (co pewien czas przesuni�cie klatki)
    if (keyTime1 < 4)
        keyTime1++;

    if (licznik == 4)
    {
        offsetchodzenie += 200;
        if (offsetchodzenie >= 1400)
            offsetchodzenie = 0;
    }

    if (licznik == 4)
    {
        offsetidle += 200;
        if (offsetidle >= 2400)
            offsetidle = 0;
    }

    // Co klatk� zwi�kszamy licznik (aby by�a ma�a �p�ynna� animacja)
    licznik++;
    if (licznik > 5)
        licznik = 0;

    // Ruch postaci (z ograniczeniami kraw�dzi)
    // G�ra
    if (Keyboard::isKeyPressed(Keyboard::W) && postac.getPosition().y > border2)
    {
        postac.move(0.f, -7.f);
        postac.setTextureRect(IntRect(offsetchodzenie, 200, 200, 200));
        ostatninacisnietyklawisz = 1;
    }
    // Lewo
    else if (Keyboard::isKeyPressed(Keyboard::A) && postac.getPosition().x > border3)
    {
        postac.move(-7.f, 0.f);
        postac.setTextureRect(IntRect(offsetchodzenie, 600, 200, 200));
        ostatninacisnietyklawisz = 2;
    }
    // D�
    else if (Keyboard::isKeyPressed(Keyboard::S) && postac.getPosition().y < border4)
    {
        postac.move(0.f, 7.f);
        postac.setTextureRect(IntRect(offsetchodzenie, 0, 200, 200));
        ostatninacisnietyklawisz = 3;
    }
    // Prawo
    else if (Keyboard::isKeyPressed(Keyboard::D) && postac.getPosition().x < border1)
    {
        postac.move(7.f, 0.f);
        postac.setTextureRect(IntRect(offsetchodzenie, 400, 200, 200));
        ostatninacisnietyklawisz = 4;
    }
    else
    {
        // Je�li nie wciskamy klawisza ruchu, poka� animacj� "idle" w zale�no�ci od ostatniego kierunku
        if (ostatninacisnietyklawisz == 1)
            postac.setTextureRect(IntRect(offsetidle, 800, 200, 200));
        else if (ostatninacisnietyklawisz == 2)
            postac.setTextureRect(IntRect(offsetidle, 1200, 200, 200));
        else if (ostatninacisnietyklawisz == 3)
            postac.setTextureRect(IntRect(offsetidle, 1000, 200, 200));
        else if (ostatninacisnietyklawisz == 4)
            postac.setTextureRect(IntRect(offsetidle, 1400, 200, 200));
    }
}
