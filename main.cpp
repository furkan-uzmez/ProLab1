#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>  // Rastgele sayı üretimi için
#include <ctime>    // Rastgele sayı için zaman tabanlı seed
#include <string.h>
#include "grafik.h" // 230202073_230202103 fonksiyonları için başlık dosyası

const int GRID_SIZE = 10;          // 10x10 ızgara
const float WINDOW_SIZE = 800.0f;  // Pencere boyutu
const float SPACING = 4.0f;        // Kareler arası boşluk

float calculateHexSize() {
    float totalWidth = WINDOW_SIZE - (GRID_SIZE - 1) * SPACING;
    return totalWidth / GRID_SIZE;
}

sf::Vector2f hexPosition(int row, int col, float hexSize) {
    float x = col * (hexSize + SPACING);
    float y = row * (hexSize + SPACING);
    return sf::Vector2f(x, y);
}

void DrawGraph(int initialValues[20]) {
    int healthValues[8];
    for(int j=0,i=12;i<20;i++) {
        healthValues[j++] = initialValues[i];
    }
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Hex Grid");

    // Font yükle
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return;
    }

    // Arka plan resmi
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("arkaplan.jpeg")) {
        return; // Resim yüklenemezse çık
    }
    sf::Sprite backgroundSprite(backgroundTexture);

    // Yeni PNG sayıları
    std::vector<sf::Texture> greenTextures(4);  // Yeşil birimler
    std::vector<sf::Texture> brownTextures(4);  // Kahverengi birimler

    // Yeşil PNG'lerini yükle
    if (!greenTextures[0].loadFromFile("okcu.png") ||
        !greenTextures[1].loadFromFile("kusatmamakinesi.png") ||
        !greenTextures[2].loadFromFile("suvari.png") ||
        !greenTextures[3].loadFromFile("piyade.png")) {
        return;
    }

    // Kahverengi PNG'lerini yükle
    if (!brownTextures[0].loadFromFile("orkdovuscusu.png") ||
        !brownTextures[1].loadFromFile("varg.png") ||
        !brownTextures[2].loadFromFile("troll.png") ||
        !brownTextures[3].loadFromFile("mizrakcilar.png")) {
        return;
    }

    // İnsancanavar ve Orkcanavar PNG'leri
    sf::Texture humanMonsterTexture;
    if (!humanMonsterTexture.loadFromFile("insancanavar.png")) {
        return;
    }
    sf::Texture orcMonsterTexture;
    if (!orcMonsterTexture.loadFromFile("orkcanavar.png")) {
        return;
    }

    // Yeni kahraman PNG'lerini yükle
    sf::Texture humanHeroTexture;
    if (!humanHeroTexture.loadFromFile("insankahraman.png")) {
        return;
    }
    sf::Texture orcHeroTexture;
    if (!orcHeroTexture.loadFromFile("orkkahraman.png")) {
        return;
    }

    float hexSize = calculateHexSize();
    window.clear(); // Pencereyi temizle

    // Grid dizisini sıfırla
    int grid[GRID_SIZE][GRID_SIZE] = {0};

    // Başlangıç değerlerini grid'e dağıt
    for (int i = 1; i < 9; ++i) {
        int unitsToPlace = initialValues[i-1] / 100; // 100 birimlik bölme
        int remainder = initialValues[i-1] % 100;    // Kalan birimler

        // Satıra yerleştir
        for (int j = 0; j < unitsToPlace; ++j) {
            grid[i][j] = 100; // 100 birim
        }
        // Kalan birimleri bir sonraki hücreye yerleştir
        if (unitsToPlace < GRID_SIZE) {
            grid[i][unitsToPlace] = remainder; // Kalan birimleri yerleştir
        }
    }

    // Arka planı çizeceğiz
    window.draw(backgroundSprite);
    int humanHeroes = initialValues[8];  // İnsan kahraman sayısı
    int orcHeroes = initialValues[10];    // Ork kahraman sayısı
    int humanMonsters = initialValues[9]; // İnsan canavar sayısı
    int orcMonsters = initialValues[11];   // Ork canavar sayısı

    // Birim sayıları
    int humanUnits[4] = {initialValues[0], initialValues[1], initialValues[2], initialValues[3]}; // İnsan birimleri
    int orcUnits[4] = {initialValues[4], initialValues[5], initialValues[6], initialValues[7]};   // Ork birimleri

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            sf::RectangleShape hex(sf::Vector2f(hexSize, hexSize));
            hex.setOutlineThickness(1);
            hex.setOutlineColor(sf::Color::Black);
            hex.setPosition(hexPosition(i, j, hexSize));

            // Renkleri ayarla
            if (grid[i][j] >= 0 && grid[i][j] < 50) {
                hex.setFillColor(sf::Color(144, 238, 144, 51)); // Pastel yeşil (%20 opaklık)
            } else if (grid[i][j] >= 50 && grid[i][j] < 100) {
                hex.setFillColor(sf::Color(210, 180, 140, 51)); // Pastel kahverengi (%20 opaklık)
            } else if (grid[i][j] >= 100 && grid[i][j] < 200) {
                hex.setFillColor(sf::Color(210, 180, 140, 51)); // Pastel kahverengi (%20 opaklık)
            } else {
                hex.setFillColor(sf::Color::Transparent); // Diğer durumlar için şeffaf
            }

            window.draw(hex);

            sf::Sprite hexSprite;
            bool hasUnit = false;  // Birim olup olmadığını takip edelim

            // Kahramanlar ve canavarlar
            if (i == 0 && j == 0 && humanHeroes > 0) { // 1. İnsan kahraman [0,0]
                hexSprite.setTexture(humanHeroTexture);
                humanHeroes--;
                hasUnit = true;
            } else if (i == 0 && j == 1 && humanHeroes > 0) { // 2. İnsan kahraman [0,1]
                hexSprite.setTexture(humanHeroTexture);
                humanHeroes--;
                hasUnit = true;
            } else if (i == 0 && j == 2 && humanMonsters > 0) { // İnsan canavarı [0,2]
                hexSprite.setTexture(humanMonsterTexture);
                humanMonsters--;
                hasUnit = true;
            } else if (i == 9 && j == 0 && orcHeroes > 0) { // 1. Ork kahraman [9,0]
                hexSprite.setTexture(orcHeroTexture);
                orcHeroes--;
                hasUnit = true;
            } else if (i == 9 && j == 1 && orcHeroes > 0) { // 2. Ork kahraman [9,1]
                hexSprite.setTexture(orcHeroTexture);
                orcHeroes--;
                hasUnit = true;
            } else if (i == 9 && j == 2 && orcMonsters > 0) { // Ork canavarı [9,2]
                hexSprite.setTexture(orcMonsterTexture);
                orcMonsters--;
                hasUnit = true;
            }

            // Diğer insan birimleri
            if (!hasUnit && i > 0 && i < 5 && humanUnits[i - 1] > 0) { // İlk 4 satırda insan birimleri
                hexSprite.setTexture(greenTextures[i - 1]);
                humanUnits[i - 1] -= 100;
                hasUnit = true;
            }

            // Diğer ork birimleri
            if (!hasUnit && i > 4 && i < 9 && orcUnits[i - 5] > 0) { // Son 4 satırda ork birimleri
                hexSprite.setTexture(brownTextures[i - 5]);
                orcUnits[i - 5] -= 100;
                hasUnit = true;
            }

            // Birim varsa görüntüle
            if (hasUnit) {
                // Resmin ölçeklendirilmesi
                float scaleX = hexSize * 0.9f / hexSprite.getGlobalBounds().width;
                float scaleY = hexSize * 0.9f / hexSprite.getGlobalBounds().height;
                hexSprite.setScale(scaleX, scaleY);
                hexSprite.setPosition(
                    hex.getPosition().x + hexSize / 2 - hexSprite.getGlobalBounds().width / 2,
                    hex.getPosition().y + hexSize / 2 - hexSprite.getGlobalBounds().height / 2);

                window.draw(hexSprite);
            }
            if (i > 0 && i < 9 && grid[i][j] > 0) { // Sayıları göster
                sf::Text unitCountText;
                unitCountText.setStyle(sf::Text::Bold); // Yazıyı kalın yaparak daha "koyu" görünmesini sağla
                unitCountText.setFont(font);
                unitCountText.setCharacterSize(20);
                unitCountText.setFillColor(sf::Color(0, 0, 0));
                unitCountText.setString(std::to_string(grid[i][j]));
                unitCountText.setPosition(hex.getPosition().x, hex.getPosition().y);
                window.draw(unitCountText);
            }
            // Sağlık çubuğu çizimi

            if(hasUnit && i>0 && i<9) {
                int health = healthValues[i-1]; // Sağlık değeri
                sf::RectangleShape healthBar(sf::Vector2f(hexSize * 0.8f, 10)); // Sağlık çubuğu boyutu
                healthBar.setPosition(hex.getPosition().x + hexSize * 0.1f, hex.getPosition().y + hexSize - 15);

                // Sağlık durumuna göre renk ayarlama
                if (health <= 0) {
                    // Sağlık 0 ise hiçbir şey çizme
                } else if (health <= 30) {
                    healthBar.setFillColor(sf::Color::Red); // Kırmızı
                } else if (health <= 70) {
                    healthBar.setFillColor(sf::Color::Yellow); // Sarı
                } else {
                    healthBar.setFillColor(sf::Color::Green); // Yeşil
                }
                if (health > 0) {
                    window.draw(healthBar);
                }
            }
        }
    }
    window.display(); // Ekranı güncelle

    // Pencere açık kaldığı sürece olay döngüsü
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
}
