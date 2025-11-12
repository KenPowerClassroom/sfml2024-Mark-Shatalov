#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

int arkanoid()
{
    srand(static_cast<unsigned>(time(0)));

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
    paddleSprite.setPosition(PADDLE_START_POSITION_X,PADDLE_START_POSITION_Y);

    Sprite blockSprite[MAX_BLOCKS];

	// create block grid
    int blockCount = 0;
    for (int row = 1; row <= BLOCK_ROWS; row++)
    {
        for (int column = 1; column <= BLOCK_COLUMNS; column++)
        {
            blockSprite[blockCount].setTexture(blockTexture);
            blockSprite[blockCount].setPosition(row * BLOCK_OFFSET_X, column * BLOCK_OFFSET_Y);
            blockCount++;
        }
    }
    
	// ball variables
    float ballVelocityX = 6, ballVelocityY = 5;
    float ballPositionX = BALL_POSITION_X, ballPositionY = BALL_POSITION_Y;

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
        ballPositionX += ballVelocityX;
        for (int i = 0; i < blockCount; i++)
        {
            if (FloatRect(ballPositionX + BALL_COLLISION_OFFSET, ballPositionY + BALL_COLLISION_OFFSET, 
                BALL_COLLISION_SIZE, BALL_COLLISION_SIZE).intersects(blockSprite[i].getGlobalBounds()))
            {
               blockSprite[i].setPosition(OFFSCREEN_POS_X, OFFSCREEN_POS_Y); ballVelocityX = -ballVelocityX;
            }
        }
        
		// update ball position
        ballPositionY += ballVelocityY;
        for (int i = 0; i < blockCount; i++)
        {
            if (FloatRect(ballPositionX + BALL_COLLISION_OFFSET, ballPositionY + BALL_COLLISION_OFFSET,
                BALL_COLLISION_SIZE, BALL_COLLISION_SIZE).intersects(blockSprite[i].getGlobalBounds()))
            {
                blockSprite[i].setPosition(OFFSCREEN_POS_X, OFFSCREEN_POS_Y);
                ballVelocityY = -ballVelocityY;
            }
        }
       
		// boundary cheking 
        if (ballPositionX < 0 || ballPositionX > WINDOW_WIDTH)
        {
            ballVelocityX = -ballVelocityX;
        }
        if (ballPositionY < 0 || ballPositionY > WINDOW_HEIGHT)
        {
            ballVelocityY = -ballVelocityY;
        }

		// paddle movement
        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            paddleSprite.move(6, 0);
        }
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            paddleSprite.move(-6, 0);
        }

		// ball collision with paddle
        if (FloatRect(ballPositionX, ballPositionY, BALL_SIZE, BALL_SIZE).intersects(paddleSprite.getGlobalBounds()))
        {
            ballVelocityY = -(rand() % 5 + 2);
        }

		// update ball sprite position
        ballSprite.setPosition(ballPositionX, ballPositionY);

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
