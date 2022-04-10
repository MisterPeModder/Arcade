#include <ctime>

#include "Nibbler.hpp"

namespace arcade
{
    Nibbler::Nibbler() : _back(nullptr), _fruit(nullptr) {}

    void Nibbler::setup()
    {
        std::cout << "[nibbler]: setup" << std::endl;
        std::srand(std::time(nullptr));
        this->_update = 0;
        this->_scoreValue = 0;
        this->_snakeSize = 4;
        this->_snakeMaxSize = 4;
        this->_gameState = true;
        this->_direction = {0, 0};
    }

    void Nibbler::setPositions(vec2i posBack)
    {
        vec2i pos;

        // set the snake's position at the center of the ground
        pos = (static_cast<vec2i>(this->_back->getSize() / 2) + posBack);
        pos.y -= 1;
        this->_snake[0]->setPosition(pos);
        pos.y += 1;
        this->_snake[1]->setPosition(pos);
        pos.x -= 1;
        this->_snake[2]->setPosition(pos);
        pos.y -= 1;
        this->_snake[3]->setPosition(pos);

        // set the fruit's position above the snake
        pos = this->_snake.front()->getPosition();
        pos.y -= 3;
        this->_fruit->setPosition(pos);
    }

    void Nibbler::createSnakePart(IAssetManager &manager)
    {
        // create all snake part with the color Green
        this->_snakeMaxSize = (this->_back->getSize().x) * (this->_back->getSize().y);

        for (int i = 0; i < _snakeMaxSize; i++) {
            this->_snake.push_back(manager.createRectObject({1, 1}, nullptr));
            this->_snake.back()->setBackground(Color::Green);
            this->_snake.back()->setForeground(Color::Green);
        }

        // set the snake head color to Red
        this->_snake.front()->setBackground(Color::Red);
        this->_snake.front()->setForeground(Color::Red);
    }

    void Nibbler::createMap(vec2u size)
    {
        // create a map for moving the fruit
        for (int y = 0; y < static_cast<int>(size.y); y++)
            for (int x = 0; x < static_cast<int>(size.x); x++)
                this->_map.push_back({x, y});
    }

    void Nibbler::loadAssets(IAssetManager &manager, vec2u displaySize)
    {
        vec2i pos;
        vec2u size = {16, 16};

        this->_back = manager.createRectObject(size, nullptr);    // create background and fruit
        this->_fruit = manager.createRectObject({1, 1}, nullptr);

        this->_state = State::Loaded;

        this->_back->setBackground(Color(0x221111)); // set background color
        this->_back->setForeground(Color(0xccccff));

        this->_fruit->setBackground(Color::Blue); // set background color
        this->_fruit->setForeground(Color::Blue);

        pos = static_cast<vec2i>(displaySize / 2 - this->_back->getSize() / 2);
        this->_back->setPosition(pos); // set background position

        this->createMap(size); // create map for fruit

        this->createSnakePart(manager); // create snake

        this->setPositions(pos); // set snake and fruit position
    }

    void Nibbler::close()
    {
        std::cout << "[nibbler]: close" << std::endl;
        this->_back.reset();
        this->_fruit.reset();
        this->_snake.clear();
    }

    void Nibbler::setState(State state) { this->_state = state; }

    arcade::IGame::State Nibbler::getState() const { return this->_state; }

    unsigned int Nibbler::getScore() const { return this->_scoreValue; }

    void Nibbler::moveFruit()
    {
        std::vector<vec2i> pos = this->_map;
        std::vector<vec2i>::iterator it;

        if (this->_snakeSize >= this->_snakeMaxSize)
            return;

        for (int i = 0; i < this->_snakeSize; i++) { // remove the snake positions from the map
            for (int j = 0; j < static_cast<int>(pos.size()); j++) {
                if (pos[j] + this->_back->getPosition() == this->_snake[i]->getPosition()) {
                    pos[j] = pos.back();
                    pos.pop_back();
                    break;
                }
            }
        }

        // the fruit is moved randomely
        this->_fruit->setPosition(pos[rand() % pos.size()] + this->_back->getPosition());
    }

    int Nibbler::checkSnakeMove(vec2i pos)
    {
        vec2i posSnake = this->_snake.front()->getPosition();
        vec2i posBack = this->_back->getPosition();
        vec2i sizeBack = static_cast<vec2i>(this->_back->getSize());

        // if the snake eat a fruit, his size go up by 1 and the fruit is moved
        if (this->_snakeSize >= this->_snakeMaxSize) { // if the snake do all the map, game won
            this->_gameState = false;
            return (0);
        }
        if (this->_fruit->getPosition() == pos) {
            this->_scoreValue += 1;

            this->_snakeSize += 1;
            return (2);
        }

        // check if the snake go out of the ground
        if (posSnake.x < posBack.x || posSnake.x > posBack.x + sizeBack.x - 1 || posSnake.y < posBack.y
            || posSnake.y > posBack.y + sizeBack.y - 1) {
            this->_gameState = false;
            return (0);
        }

        // check if the snake is eating himself
        for (int i = 0; i < this->_snakeSize - 1; i++) {
            if (pos == this->_snake[i]->getPosition()) {
                this->_gameState = false;
                return (0);
            }
        }
        return (1);
    }

    void Nibbler::moveSnake()
    {
        vec2i pos = this->_snake.front()->getPosition();
        vec2i pos2;
        int check = this->checkSnakeMove(pos + this->_direction);

        if (check == 0) // in case of game over of win, the snake stop moving
            return;

        this->_snake.front()->setPosition(pos + this->_direction); // move the snake
        for (int i = 1; i < this->_snakeSize; i++) {
            pos2 = this->_snake[i]->getPosition();
            this->_snake[i]->setPosition(pos);
            pos = pos2;
        }
        if (check == 2)
            moveFruit();
    }

    void Nibbler::update(float delta)
    {
        if (this->_update > 0.3) { // every 0.3 seconds, the game is updated
            this->_update = 0;
            // check if the snake is moving and if the game is win or lost
            if (this->_gameState && (this->_direction.x != 0 || this->_direction.y != 0))
                this->moveSnake();
        }
        this->_update += delta;
    }

    void Nibbler::render(IRenderer &renderer)
    {
        renderer.draw(*this->_back);
        renderer.draw(*this->_fruit);

        for (int i = 0; i < this->_snakeSize; i++)
            renderer.draw(*this->_snake[i]);
    }

    void Nibbler::resetGame()
    {
        this->_scoreValue = 0;
        this->_snakeSize = 4;
        this->_gameState = true;
        this->_direction = {0, 0};
        this->setPositions(this->_back->getPosition());
    }

    void Nibbler::handleEvent(Event &event)
    {
        if (event.type == Event::Type::KeyReleased) {
            if (event.key.code == '\x1b') {
                this->_state = State::Ended;
                this->resetGame();
            } else if (event.key.code == 'p' || event.key.code == 'P')
                this->_state = this->_state == State::Paused ? State::Running : State::Paused;
            else if (this->_gameState == false) { // in case of gameover or win, press 'r' to restart the game
                if (event.key.code == 'r' || event.key.code == 'R') {
                    this->resetGame();
                }
            } else if (event.key.code == 'z' || event.key.code == 'Z')
                this->_direction = {0, -1};
            else if (event.key.code == 'q' || event.key.code == 'Q')
                this->_direction = {-1, 0};
            else if (event.key.code == 's' || event.key.code == 'S')
                this->_direction = {0, 1};
            else if (event.key.code == 'd' || event.key.code == 'D')
                this->_direction = {1, 0};
        }

        // Don't update anything else if the game is paused
        if (this->_state == State::Paused)
            return;

        if (event.type == Event::Type::Resized) { // move the background to the center of the window
            vec2u size(this->_back->getSize());
            vec2i newPosBack = static_cast<vec2i>(event.size.newSize / 2 - size / 2);
            vec2i move = newPosBack - this->_back->getPosition();

            this->_back->setPosition(newPosBack);

            // move all the gameobjects on the moved background
            this->_fruit->setPosition(this->_fruit->getPosition() + move);
            for (int i = 0; i < this->_snakeSize; i++)
                this->_snake[i]->setPosition(this->_snake[i]->getPosition() + move);
        }
    }
} // namespace arcade