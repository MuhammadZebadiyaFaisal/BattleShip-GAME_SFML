#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib> 
#include <cmath>    
#include <string>

using namespace std;

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 700}), "MZF Developers : BATTLESHIP COMMAND"); 
    srand(static_cast<unsigned int>(time(0)));

    sf::Clock blinkClock, loadingClock, introClock;

    sf::Texture menuTex, gameTex, winTex, loseTex, waterTex, shipTex, hitTex, missTex, introTex;
    sf::SoundBuffer fireBuffer, hitBuffer, missBuffer, introBuffer, winBuffer, loseBuffer;
    sf::Music bgMusic;
    sf::Font font;

    if (!menuTex.loadFromFile("menu_bg.png") || !gameTex.loadFromFile("game_bg.png") || 
        !winTex.loadFromFile("win_bg.png") || !loseTex.loadFromFile("lose_bg.png") || 
        !waterTex.loadFromFile("water.png") || !shipTex.loadFromFile("ship.png") || 
        !hitTex.loadFromFile("hit.png") || !missTex.loadFromFile("miss.png") || 
        !introTex.loadFromFile("intro_bg.png") || 
        !fireBuffer.loadFromFile("fire.wav") || !hitBuffer.loadFromFile("explosion.wav") || 
        !missBuffer.loadFromFile("splash.wav") || !introBuffer.loadFromFile("mzf_intro.wav") ||
        !winBuffer.loadFromFile("win_sound.wav") || !loseBuffer.loadFromFile("lose_sound.wav") ||
        !bgMusic.openFromFile("bg_music.ogg") || !font.openFromFile("arial.ttf")) {
        return -1;
    }

    sf::Sound fireSound(fireBuffer), hitSound(hitBuffer), missSound(missBuffer), 
              introSound(introBuffer), winSound(winBuffer), loseSound(loseBuffer);
    
    bgMusic.setLooping(true);

    int board[10][10] = {0}, ammo = 30, shipsFound = 0, totalShips = 5;
    bool showIntro = true, inMenu = false, isLoading = false, showBriefing = false, gameOver = false, won = false;
    string logs = "COMMAND CENTER\n--------------\nRadar Online...";

    sf::RectangleShape playBtn(sf::Vector2f(220.f, 65.f)), exitBtn(sf::Vector2f(220.f, 65.f)), 
                       restartBtn(sf::Vector2f(220.f, 65.f)), endExitBtn(sf::Vector2f(220.f, 65.f));
    
    playBtn.setPosition(sf::Vector2f(290.f, 350.f));
    exitBtn.setPosition(sf::Vector2f(290.f, 450.f));
    restartBtn.setPosition(sf::Vector2f(290.f, 420.f)); 
    endExitBtn.setPosition(sf::Vector2f(290.f, 510.f));

    sf::Text uiText(font), introText(font), logText(font), titleText(font);
    uiText.setCharacterSize(30); uiText.setFillColor(sf::Color::White);
    
    logText.setFont(font); // Font set karna zaroori hai
    logText.setCharacterSize(18);
    logText.setPosition(sf::Vector2f(555.f, 100.f));
    logText.setFillColor(sf::Color::White);

    titleText.setFont(font);
    titleText.setString("BATTLE SHIP");
    titleText.setCharacterSize(80);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setStyle(sf::Text::Bold);
    auto titleBounds = titleText.getGlobalBounds();
    titleText.setOrigin(sf::Vector2f(titleBounds.size.x / 2.f, titleBounds.size.y / 2.f));
    titleText.setPosition(sf::Vector2f(400.f, 200.f));

    introText.setFont(font); introText.setCharacterSize(50);
    introText.setString("POWERED BY MZF DEVELOPERS");
    auto introBounds = introText.getGlobalBounds();
    introText.setOrigin(sf::Vector2f(introBounds.size.x / 2.f, introBounds.size.y / 2.f));
    introText.setPosition(sf::Vector2f(400.f, 350.f));

    auto placeShips = [&]() {
        for(int i=0; i<10; i++) for(int j=0; j<10; j++) board[i][j] = 0;
        int placed = 0;
        while (placed < totalShips) {
            int r = rand() % 10, c = rand() % 10;
            if (board[r][c] == 0) { board[r][c] = 1; placed++; }
        }
    };
    placeShips();

    introSound.play(); 

    while (window.isOpen()) {
        sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        float timeVal = blinkClock.getElapsedTime().asSeconds();
        float blinkAlpha = (sin(timeVal * 8.0f) + 1.0f) * 127.5f;

        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            if (event->is<sf::Event::MouseButtonPressed>()) {
                if (inMenu) {
                    if (playBtn.getGlobalBounds().contains(mPos)) { inMenu = false; isLoading = true; loadingClock.restart(); }
                    if (exitBtn.getGlobalBounds().contains(mPos)) window.close();
                }
                else if (showBriefing) { showBriefing = false; } 
                else if (gameOver) {
                    if (restartBtn.getGlobalBounds().contains(mPos)) {
                        ammo = 30; shipsFound = 0; gameOver = false; won = false;
                        logText.setFillColor(sf::Color::White);
                        logs = "COMMAND CENTER\n--------------\nNew Mission Started...";
                        placeShips();
                    }
                    if (endExitBtn.getGlobalBounds().contains(mPos)) window.close();
                }
                else if (!gameOver && !isLoading && !showIntro) {
                    int col = (int)(mPos.x - 50) / 50;
                    int row = (int)(mPos.y - 50) / 50;
                    if (row >= 0 && row < 10 && col >= 0 && col < 10 && board[row][col] < 2) {
                        fireSound.play(); ammo--;
                        if (board[row][col] == 1) { 
                            board[row][col] = 2; shipsFound++; hitSound.play(); 
                            logText.setFillColor(sf::Color::Green);
                            logs = "COMMAND CENTER\n--------------\n> [BOOM] HIT!\n> Ships Sunk: " + to_string(shipsFound) + "/5";
                        } 
                        else { 
                            board[row][col] = 3; missSound.play(); 
                            logText.setFillColor(sf::Color::Red);
                            logs = "COMMAND CENTER\n--------------\n> [SPLASH] MISS\n> Ammo Left: " + to_string(ammo);
                        }
                    }
                }
            }
        }

        window.clear();

        if (showIntro) {
            if (introClock.getElapsedTime().asSeconds() > 4.f) { showIntro = false; inMenu = true; bgMusic.play(); }
            sf::Sprite introSprite(introTex);
            introSprite.setScale(sf::Vector2f(800.f / introTex.getSize().x, 700.f / introTex.getSize().y));
            window.draw(introSprite);
            float flicker = (sin(timeVal * 20.0f) > 0) ? 255.f : 50.f; 
            introText.setFillColor(sf::Color(255, 0, 0, (uint8_t)flicker));
            window.draw(introText);
        }
        else if (inMenu) {
            sf::Sprite mb(menuTex); mb.setScale(sf::Vector2f(800.f/menuTex.getSize().x, 700.f/menuTex.getSize().y));
            window.draw(mb);
            window.draw(titleText);
            playBtn.setFillColor(playBtn.getGlobalBounds().contains(mPos) ? sf::Color::Yellow : sf::Color(255, 69, 0, 200));
            exitBtn.setFillColor(exitBtn.getGlobalBounds().contains(mPos) ? sf::Color::White : sf::Color(139, 0, 0, 200));
            window.draw(playBtn); window.draw(exitBtn);
            uiText.setString("PLAY"); uiText.setPosition(sf::Vector2f(360.f, 362.f)); uiText.setFillColor(sf::Color::Black); window.draw(uiText);
            uiText.setString("EXIT"); uiText.setPosition(sf::Vector2f(360.f, 462.f)); uiText.setFillColor(sf::Color::White); window.draw(uiText);
        }
        else if (isLoading) {
            sf::Sprite mb(menuTex); mb.setScale(sf::Vector2f(800.f/menuTex.getSize().x, 700.f/menuTex.getSize().y));
            window.draw(mb);
            uiText.setString("LOADING . . ."); uiText.setCharacterSize(50);
            uiText.setPosition(sf::Vector2f(250.f, 300.f)); window.draw(uiText);
            if (loadingClock.getElapsedTime().asSeconds() > 3.f) { isLoading = false; showBriefing = true; uiText.setCharacterSize(30); }
        }
        else if (showBriefing) {
            sf::Sprite bg(gameTex); bg.setScale(sf::Vector2f(800.f/gameTex.getSize().x, 700.f/gameTex.getSize().y));
            window.draw(bg);
            sf::RectangleShape overlay(sf::Vector2f(600.f, 400.f)); overlay.setPosition(sf::Vector2f(100.f, 150.f));
            overlay.setFillColor(sf::Color(0, 0, 0, 220)); overlay.setOutlineThickness(2.f); overlay.setOutlineColor(sf::Color::Green);
            window.draw(overlay);
            uiText.setFillColor(sf::Color::Green); uiText.setString("MISSION OBJECTIVE\n\n- Destroy 5 Enemy Battleships\n- Ammo: 30 Shells\n- Avoid Missing Shots\n\nCLICK ANYWHERE TO START");
            uiText.setPosition(sf::Vector2f(150.f, 200.f)); window.draw(uiText);
        }
        else if (gameOver) {
            sf::Sprite endBg(won ? winTex : loseTex);
            endBg.setScale(sf::Vector2f(800.f/endBg.getTexture().getSize().x, 700.f/endBg.getTexture().getSize().y));
            window.draw(endBg);
            uiText.setCharacterSize(60); uiText.setString(won ? "MISSION SUCCESSFUL!" : "MISSION FAILED . . .");
            auto winBounds = uiText.getGlobalBounds();
            uiText.setOrigin(sf::Vector2f(winBounds.size.x / 2.f, 0.f));
            uiText.setPosition(sf::Vector2f(400.f, 150.f)); 
            uiText.setFillColor(won ? sf::Color::Green : sf::Color::Red); window.draw(uiText);
            restartBtn.setFillColor(sf::Color(255, 69, 0, (uint8_t)blinkAlpha));
            endExitBtn.setFillColor(sf::Color(139, 0, 0, (uint8_t)blinkAlpha));
            window.draw(restartBtn); window.draw(endExitBtn);
            uiText.setCharacterSize(30); uiText.setFillColor(sf::Color::White); uiText.setOrigin(sf::Vector2f(0,0)); 
            uiText.setString("RESTART"); uiText.setPosition(sf::Vector2f(335.f, 432.f)); window.draw(uiText);
            uiText.setString("EXIT"); uiText.setPosition(sf::Vector2f(365.f, 522.f)); window.draw(uiText);
        }
        else {
            sf::Sprite bg(gameTex); bg.setScale(sf::Vector2f(800.f/gameTex.getSize().x, 700.f/gameTex.getSize().y));
            window.draw(bg);
            sf::RectangleShape logBox(sf::Vector2f(230.f, 500.f)); logBox.setPosition(sf::Vector2f(540.f, 50.f));
            logBox.setFillColor(sf::Color(0, 0, 0, 200)); logBox.setOutlineThickness(3.f);
            logBox.setOutlineColor(logText.getFillColor()); window.draw(logBox);
            logText.setString(logs); window.draw(logText);
            for (int i = 0; i < 10; i++) for (int j = 0; j < 10; j++) {
                sf::Texture* t = &waterTex;
                if (board[i][j] == 2) t = &hitTex; else if (board[i][j] == 3) t = &missTex;
                sf::Sprite s(*t); s.setPosition(sf::Vector2f(j * 50.f + 50.f, i * 50.f + 50.f));
                s.setScale(sf::Vector2f(48.f/t->getSize().x, 48.f/t->getSize().y)); window.draw(s);
            }
        }

        if (!gameOver && !showIntro && !isLoading && !inMenu && !showBriefing) {
            if (shipsFound == totalShips) { gameOver = true; won = true; winSound.play(); }
            else if (ammo <= 0) { gameOver = true; won = false; loseSound.play(); }
        }
        window.display();
    }
    return 0;
}