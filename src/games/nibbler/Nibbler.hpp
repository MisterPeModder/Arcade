#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

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
        Nibbler();
        void setPositions(vec2i posBack);
        void createSnakePart(IAssetManager &manager);
        void createMap(vec2u size);

        int checkSnakeMove(vec2i pos);
        void moveFruit();
        void moveSnake();

        void resetGame();

        void setup() override final;
        void loadAssets(IAssetManager &manager, vec2u displaySize) override final;
        void close() override final;
        void setState(State state) override final;
        State getState() const override final;
        unsigned int getScore() const override final;
        void update(float delta) override final;
        void render(IRenderer &renderer) override final;
        void handleEvent(Event &event) override final;

      private:
        unsigned int _scoreValue;
        int _snakeSize;
        int _snakeMaxSize;
        float _update;

        bool _gameState;

        State _state;
        vec2i _direction;
        std::vector<vec2i> _map;

        std::unique_ptr<IGameObject> _back;
        std::unique_ptr<IGameObject> _fruit;
        std::vector<std::unique_ptr<IGameObject>> _snake;
    };
} // namespace arcade