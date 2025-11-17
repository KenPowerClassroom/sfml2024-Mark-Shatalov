#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

// ball class 
class Ball
{
public:
	// position and velocity of the ball
    float x, y;
    float velocityX, velocityY;

    Ball(float startX, float startY, float speedX, float speedY)
        : x(startX), y(startY), velocityX(speedX), velocityY(speedY) 
    { }

	// move the ball
    void move()
    {
        x += velocityX;
        y += velocityY;
    }

	// revert velocity on x or y axis
    void bounceX() 
    {
         velocityX = -velocityX;
    }
    void bounceY() 
    {
         velocityY = -velocityY;
    }

	// boundary checking
    void checkBounds(int width, int height)
    {
        if (x < 0 || x > width)
        {
            bounceX();
        }
        if (y < 0 || y > height)
        {
            bounceY();
        }
    }
};

// paddle class 
class Paddle
{
public:
	// position and speed of the paddle
    float x, y;
    float speed;

    Paddle(float startX, float startY)
        : x(startX), y(startY), speed(6.0f) 
    { }

	// move the paddle
    void moveLeft() { x -= speed; }
    void moveRight() { x += speed; }
};

// block class 
class Block
{
public:
	// position and alive status of the block
    float x, y;
    bool alive;

    Block() : x(0), y(0), alive(false) {}

    Block(float startX, float startY)
        : x(startX), y(startY), alive(true) 
    { }

	// destroy the block
    void destroy() { alive = false; }
};

// window size const
const int WINDOW_WIDTH = 520;
const int WINDOW_HEIGHT = 450;

// block grid const
const int BLOCK_ROWS = 10;
const int BLOCK_COLUMNS = 10;
const int BLOCK_OFFSET_X = 43;
const int BLOCK_OFFSET_Y = 20;
const int MAX_BLOCKS = 1000;

// ball const
const float BALL_POSITION_X = 300.0f;
const float BALL_POSITION_Y = 300.0f;
const float BALL_SIZE = 12.0f;
const int BALL_COLLISION_OFFSET = 3;
const int BALL_COLLISION_SIZE = 6;

// paddle const
const float PADDLE_START_POSITION_X = 300.0f;
const float PADDLE_START_POSITION_Y = 440.0f;

// oof-screen pos for destroyed blocks
const int OFFSCREEN_POS_X = -100;
const int OFFSCREEN_POS_Y = 0;

int arkanoid()
{
    srand(static_cast<unsigned>(time(0)));

    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Arkanoid!");
    window.setFramerateLimit(60);

    // set up textures
    Texture blockTexture, backgroundTexture, ballTexture, paddleTexture;
    blockTexture.loadFromFile("images/arkanoid/block01.png");
    backgroundTexture.loadFromFile("images/arkanoid/background.jpg");
    ballTexture.loadFromFile("images/arkanoid/ball.png");
    paddleTexture.loadFromFile("images/arkanoid/paddle.png");

    // set up sprites
    Sprite backgroundSprite(backgroundTexture), ballSprite(ballTexture), paddleSprite(paddleTexture);

    // Create game objects
    Ball ball(BALL_POSITION_X, BALL_POSITION_Y, 6, 5);
    Paddle paddle(PADDLE_START_POSITION_X, PADDLE_START_POSITION_Y);

    Sprite blockSprite[MAX_BLOCKS];
    Block blocks[MAX_BLOCKS];

    // create block grid
    int blockCount = 0;
    for (int row = 1; row <= BLOCK_ROWS; row++)
    {
        for (int column = 1; column <= BLOCK_COLUMNS; column++)
        {
            blocks[blockCount] = Block(row * BLOCK_OFFSET_X, column * BLOCK_OFFSET_Y);
			// set up block sprite
            blockSprite[blockCount].setTexture(blockTexture);
			// set block sprite position
            blockSprite[blockCount].setPosition(blocks[blockCount].x, blocks[blockCount].y);
            blockCount++;
        }
    }

    // game loop
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        // update ball position
        ball.x += ball.velocityX;
        for (int i = 0; i < blockCount; i++)
        {
			// if the block is destroyed skip it
            if (!blocks[i].alive) 
            {
                 continue;
            }

            // ball collision box
            float ballColisionX = ball.x + BALL_COLLISION_OFFSET;
            float ballColisionY = ball.y + BALL_COLLISION_OFFSET;
            // create collision box
            FloatRect ballCollisionBox(ballColisionX, ballColisionY, BALL_COLLISION_SIZE, BALL_COLLISION_SIZE);
            // get block bounds
            FloatRect blockBounds = blockSprite[i].getGlobalBounds();
            // check if ball intersects block
            bool ballHitBlock = ballCollisionBox.intersects(blockBounds);

            // if collision move block out of screen
            if (ballHitBlock)
            {
                blocks[i].destroy();
                blockSprite[i].setPosition(OFFSCREEN_POS_X, OFFSCREEN_POS_Y);
                // reverse ball x velocity
                ball.bounceX();
            }
        }

        // update ball position 
        ball.y += ball.velocityY;
        for (int i = 0; i < blockCount; i++)
        {
            // if the block is destroyed skip it
            if (!blocks[i].alive) 
            {
                continue;
            }

            // ball collision box
            float ballCollisionX = ball.x + BALL_COLLISION_OFFSET;
            float ballCollisionY = ball.y + BALL_COLLISION_OFFSET;
            // create collision box
            FloatRect ballCollisionBox(ballCollisionX, ballCollisionY, BALL_COLLISION_SIZE, BALL_COLLISION_SIZE);
            // get block bounds
            FloatRect blockBounds = blockSprite[i].getGlobalBounds();
            // check if ball intersects block
            bool ballHitBlock = ballCollisionBox.intersects(blockBounds);

            // if collision move block out of screen
            if (ballHitBlock)
            {
                blocks[i].destroy();
                blockSprite[i].setPosition(OFFSCREEN_POS_X, OFFSCREEN_POS_Y);
                // reverse ball y velocity
                ball.bounceY();
            }
        }

        // boundary cheking 
        ball.checkBounds(WINDOW_WIDTH, WINDOW_HEIGHT);

        // paddle movement
        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            paddle.moveRight();
        }
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            paddle.moveLeft();
        }

        // ball collision box
        FloatRect ballBounds(ball.x, ball.y, BALL_SIZE, BALL_SIZE);
        // paddle bounds
        paddleSprite.setPosition(paddle.x, paddle.y);
        FloatRect paddleBounds = paddleSprite.getGlobalBounds();
        // check if ball intersects paddle
        bool ballHitPaddle = ballBounds.intersects(paddleBounds);

        // if collision, reverse ball y velocity
        if (ballHitPaddle)
        {
            ball.velocityY = -(rand() % 5 + 2);
        }

        // update ball sprite position
        ballSprite.setPosition(ball.x, ball.y);

        // render
        // draw background, ball and  paddle
        window.clear();
        window.draw(backgroundSprite);
        window.draw(ballSprite);
        window.draw(paddleSprite);

        // draw blocks
        for (int i = 0; i < blockCount; i++)
        {
            window.draw(blockSprite[i]);
        }

        window.display();
    }

    return 0;
}
