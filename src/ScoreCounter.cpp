#include <ScoreCounter.hpp>

bol::ScoreCounter::ScoreCounter() {
    for (int i = 0; i < 10; i++) {
        std::string string_representation = std::to_string(i);

        sf::Texture texture;

        std::string path = "assets/score-digits/" + string_representation + ".png";
    
        if (!texture.loadFromFile(path))
            throw std::exception("Couldn\'t load digit texture!");

        digitTextures.insert({string_representation, texture});
    }

    digitSprites = {sf::Sprite(digitTextures["0"])};
}

sf::Vector2f bol::ScoreCounter::getCenter() const {
    return center;
}

sf::Vector2f bol::ScoreCounter::getRealSize() const {
    return {size.x * scale.x, size.y * scale.y};
}

void bol::ScoreCounter::setScore(float newScore) {
    int castedScore = (int)score;
    int castedNewScore = (int)(newScore);

    score = newScore;

    if (castedScore == castedNewScore)
        return;

    scoreText = std::to_string(castedNewScore);

    digitSprites = {};

    size.x = scoreText.length() * textureSize.x;

    sf::Vector2f realSize = getRealSize();

    float xPos = center.x - realSize.x / 2.f;
    float yPos = center.y - realSize.y / 2.f;

    for (int i = 0; i < scoreText.size(); i++) {
        std::string character = std::to_string(scoreText[i] - 48);

        sf::Sprite digitSprite {digitTextures[character]};

        digitSprite.setScale(scale);

        digitSprite.setPosition({xPos, yPos});

        digitSprite.setColor({255u, 255u, 255u, opacity});

        xPos += textureSize.x * scale.x;

        digitSprites.push_back(digitSprite);
    }
}

void bol::ScoreCounter::setCenter(const sf::Vector2f &newCenter) {
    sf::Vector2f distance = newCenter - center;

    center = newCenter;

    for (sf::Sprite &sprite : digitSprites) {
        sprite.move(distance);
    }
}

void bol::ScoreCounter::move(const sf::Vector2f &offset) {
    setCenter(center + offset);
}

sf::Vector2f bol::ScoreCounter::getDigitTextureSize() {
    return textureSize;
}

void bol::ScoreCounter::setScale(const sf::Vector2f &newScale) {
    scale = newScale;

    sf::Vector2f realSize = getRealSize();

    float xPos = center.x - realSize.x / 2.f;
    float yPos = center.y - realSize.y / 2.f;

    for (sf::Sprite &sprite : digitSprites) {
        sprite.setPosition({xPos, yPos});
        sprite.setScale(scale);

        xPos += textureSize.x * scale.x;
    }
}

int bol::ScoreCounter::getAmountOfDigits() {
    return digitSprites.size();
}

void bol::ScoreCounter::incrementScore(float increment) {
    setScore(score + increment);
}

void bol::ScoreCounter::setOpacity(unsigned char newOpacity) {
    opacity = newOpacity;

    for (sf::Sprite &sprite : digitSprites)
        sprite.setColor({255u, 255u, 255u, opacity});
}

void bol::ScoreCounter::draw(sf::RenderWindow &window) {
    for (sf::Sprite &sprite : digitSprites)
        window.draw(sprite);
}