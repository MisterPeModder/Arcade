#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>

#include <arcade/Color.hpp>
#include <arcade/Event.hpp>
#include <arcade/IAsset.hpp>
#include <arcade/IAssetManager.hpp>
#include <arcade/IGame.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/IRenderer.hpp>
#include <arcade/types.hpp>

namespace arcade
{
    class Nibbler : public IGame {
      public:
        Nibbler() : _back(nullptr), _fruit(nullptr)
        {
            this->_update = 0;
            this->_scoreValue = 0;
            this->_snakeSize = 4;
            this->_snakeMaxSize = 4;
            this->_gameState = true;
            this->_direction = {0, 0};
        }

        void setup() override final { std::srand(std::time(nullptr)); }

        void setPositions(vec2i posBack)
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

        void createSnakePart(IAssetManager &manager)
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

        void createMap(vec2u size)
        {
            // create a map for moving the fruit
            for (int y = 0; y < static_cast<int>(size.y); y++)
                for (int x = 0; x < static_cast<int>(size.x); x++)
                    this->_map.push_back({x, y});
        }

        void loadAssets(IAssetManager &manager, vec2u displaySize) override final
        {
            vec2i pos;
            vec2u size = {16, 16};

            // this->_font = manager.loadAsset("./assets/fonts/roboto_regular.ttf", IAsset::Type::Font);
            // this->_score = manager.createTextObject("This is a test", this->_font.get());

            this->_back = manager.createRectObject(size, nullptr); // create background and fruit
            this->_fruit = manager.createRectObject({1, 1}, nullptr);

            this->_state = State::Loaded;

            this->_back->setBackground(Color(0x221111)); // set background color
            this->_back->setForeground(Color(0xccccff));

            pos = static_cast<vec2i>(displaySize / 2 - this->_back->getSize() / 2);
            this->_back->setPosition(pos); // set background position

            this->_fruit->setBackground(Color::Blue); // set fruit color
            this->_fruit->setForeground(Color::Blue);

            // this->_score->setForeground(Color::Black);
            // this->_score->setBackground(Color::White);

            this->createSnakePart(manager); // create snake

            this->setPositions(pos); // set snake and fruit position

            this->createMap(size); // create map for fruit
        }

        void close() override final
        {
            // this->_font.reset();
            // this->_score.reset();

            this->_back.reset();
            this->_fruit.reset();

            for (size_t i = 0; i < this->_snake.size(); i++)
                this->_snake[i].reset();
        }

        void setState(State state) override final { this->_state = state; }

        State getState() const override final { return this->_state; }

        unsigned int getScore() const override final { return this->_scoreValue; }

        void moveFruit()
        {
            std::vector<vec2i> pos = this->_map;

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

        int checkSnakeMove(vec2i pos)
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

        void moveSnake()
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

        // void testGame() // work on pair map side length
        // {
        //     vec2i posSnake = _snake.front()->getPosition();
        //     vec2i posBack = _back->getPosition();
        //     vec2u sizeBack = _back->getSize();

        //     if (posSnake.x == posBack.x + static_cast<int>(sizeBack.x) - 1 && posSnake.y == posBack.y)
        //         _direction = {0, 1};
        //     else if ((posSnake.x == posBack.x + static_cast<int>(sizeBack.x) - 1
        //                  && posSnake.y == posBack.y + static_cast<int>(sizeBack.y) - 1)
        //         || (posSnake.x == posBack.x + static_cast<int>(sizeBack.x) - 2 && _direction.y == -1))
        //         _direction = {-1, 0};
        //     else if ((posSnake.x == posBack.x && _direction.x == -1)
        //         || (posSnake.x == posBack.x + static_cast<int>(sizeBack.x) - 2 && _direction.x == 1
        //             && posSnake.y > posBack.y))
        //         _direction = {0, -1};
        //     else if ((posSnake.x == posBack.x && _direction.y == -1))
        //         _direction = {1, 0};
        // }

        void update(float delta) override final
        {
            if (this->_update > 0.3) { // every 0.3 seconds, the game is updated
                this->_update = 0;
                // check if the snake is moving and if the game is win or lost
                if (this->_gameState && (this->_direction.x != 0 || this->_direction.y != 0))
                    this->moveSnake();
            }
            this->_update += delta;
        }

        void render(IRenderer &renderer) override final
        {
            renderer.draw(*this->_back);
            renderer.draw(*this->_fruit);
            // renderer.draw(*this->_score);

            for (int i = 0; i < this->_snakeSize; i++)
                renderer.draw(*this->_snake[i]);
        }

        void handleEvent(Event &event) override final
        {
            if (event.type == Event::Type::KeyReleased) {
                if (event.key.code == '\x1b')
                    this->_state = State::Ended;
                else if (event.key.code == 'p')
                    this->_state = this->_state == State::Paused ? State::Running : State::Paused;
                else if (this->_gameState == false) { // in case of gameover or win, press 'r' to restart the game
                    if (event.key.code == 'r') {
                        this->_scoreValue = 0;
                        this->_snakeSize = 4;
                        this->_gameState = true;
                        this->_direction = {0, 0};
                        this->setPositions(this->_back->getPosition());
                    }
                } else if (event.key.code == 'z')
                    this->_direction = {0, -1};
                else if (event.key.code == 'q')
                    this->_direction = {-1, 0};
                else if (event.key.code == 's')
                    this->_direction = {0, 1};
                else if (event.key.code == 'd')
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

                this->_fruit->setPosition(
                    this->_fruit->getPosition() + move); // move all the gameobjects on the moved background
                for (int i = 0; i < this->_snakeSize; i++)
                    this->_snake[i]->setPosition(this->_snake[i]->getPosition() + move);
            }
        }

      private:
        unsigned int _scoreValue;
        int _snakeSize;
        int _snakeMaxSize;
        float _update;

        bool _gameState;

        State _state;
        vec2i _direction;
        std::vector<vec2i> _map;

        // std::unique_ptr<IAsset> _font;
        // std::unique_ptr<IGameObject> _score;

        std::unique_ptr<IGameObject> _back;
        std::unique_ptr<IGameObject> _fruit;
        std::vector<std::unique_ptr<IGameObject>> _snake;
    };

    static arcade::IGame *GAME_INSTANCE = nullptr;

    ARCADE_GAME_ENTRY_POINT
    {
        std::cout << "[nibbler]: called entry point" << std::endl;
        return GAME_INSTANCE;
    }

    [[gnu::constructor]] void onConstruct()
    {
        GAME_INSTANCE = new Nibbler();
        std::cout << "[nibbler]: constructed" << std::endl;
    }

    [[gnu::destructor]] void onDestroy()
    {
        delete GAME_INSTANCE;
        GAME_INSTANCE = nullptr;
        std::cout << "[nibbler]: destroyed" << std::endl;
    }
} // namespace arcade
