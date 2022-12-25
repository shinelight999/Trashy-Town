#include<iostream>
#include<string>
#include<random>
#include<fstream>
#include<SFML/Graphics.hpp>
#include<SFML/Graphics/Rect.hpp>
#include<SFML/System/Clock.hpp>
#include<SFML/Audio.hpp>
#include"Player.h"
#include"Obstacle.h"

using namespace std;

void saveLeaderBoard(int firstScore, int secondScore, int thirdScore, int fourthScore, int fifthScore);
enum states { menu, play, pause };

void loadLeaderBoard(int &firstScore, int &secondScore, int &thirdScore, int &fourthScore, int &fifthScore)
{
	fstream file;
	string score;
	file.open("LeaderBoard.txt", ios::in);

	getline(file, score);
	firstScore = stoi(score);
	getline(file, score);
	secondScore = stoi(score);
	getline(file, score);
	thirdScore = stoi(score);
	getline(file, score);
	fourthScore = stoi(score);
	getline(file, score);
	fifthScore = stoi(score);

	file.close();

}

int randomizeYPosInit()
{
	int randomY;
	int thePosition = 0;
	randomY = rand()%2;
	if (randomY == 0)
	{
		thePosition = 400;
	}
	else if (randomY == 1)
	{
		thePosition = 300;
	}
	return thePosition;
}

int main()
{
	states machine = menu;

	int leaderBoard[5] = { 0, 0, 0, 0, 0 };
	int score = 0;
	int newScore = 0;
	int firstScore = 0;
	int secondScore = 0;
	int thirdScore = 0;
	int fourthScore = 0;
	int fifthScore = 0;

	sf::RenderWindow window(sf::VideoMode(852, 480), "Running!");

	sf::Clock clock;

	player thePlayer;
	thePlayer.playerTexture.loadFromFile("Textures/Car.png");
	thePlayer.myPlayer.setTexture(thePlayer.playerTexture);
	thePlayer.myPlayer.setPosition(0, 400);
	
	sf::Texture background;
	sf::Sprite bgSprite;
	background.loadFromFile("Textures/PresentDay.png");
	bgSprite.setTexture(background);
	bgSprite.setOrigin(0, 0);
	window.setFramerateLimit(40);

	sf::Texture obstacleTexture;
	obstacleTexture.loadFromFile("Textures/Trashbag.png");
	obstacle trash(sf::Vector2f(852, randomizeYPosInit()), &obstacleTexture);

	sf::Music menuMusic;
	if (!menuMusic.openFromFile("Audio/Theme6.ogg"))
	{
	}
	menuMusic.setPosition(0, 1, 10);
	menuMusic.setPitch(1);
	menuMusic.setVolume(50);
	menuMusic.setLoop(true);
	
	sf::Music gameMusic;
	if (!gameMusic.openFromFile("Audio/Battle1.ogg"))
	{
	}
	gameMusic.setPosition(0, 1, 10);
	gameMusic.setPitch(1);
	gameMusic.setVolume(50);
	gameMusic.setLoop(true);

	sf::Music gameOverMusic;
	if (!gameOverMusic.openFromFile("Audio/Gameover2.ogg"))
	{
	}
	gameOverMusic.setPosition(0, 1, 10);
	gameOverMusic.setPitch(1);
	gameOverMusic.setVolume(50);

	sf::Font gameFont;
	gameFont.loadFromFile("Fonts/Candal.ttf");

	sf::Text titleText;
	titleText.setFont(gameFont);
	titleText.setString("Trashy Town");
	titleText.setCharacterSize(75);
	titleText.setFillColor(sf::Color::Black);
	titleText.setPosition(150, 200);

	sf::Text instructions;
	instructions.setFont(gameFont);
	instructions.setString("Instructions\nGoal: Avoid the trash\n'M' - Menu\n'P' - Play Game / Pause Game\n'I' - Instructions\n'W' - Left Lane\n'S' - Right Lane\n'R' - Restart\n'G' - Game Over");
	instructions.setCharacterSize(30);
	instructions.setFillColor(sf::Color::Black);

	sf::Text instructionText;
	instructionText.setFont(gameFont);
	instructionText.setString("W-Left Lane\nS-Right Lane");
	instructionText.setCharacterSize(15);
	instructionText.setFillColor(sf::Color::Black);
	instructionText.setPosition(730, 0);

	sf::Text goalText;
	goalText.setFont(gameFont);
	goalText.setString("Avoid the trash");
	goalText.setCharacterSize(25);
	goalText.setFillColor(sf::Color::Black);
	goalText.setPosition(300, 0);

	sf::Text menuText;
	menuText.setFont(gameFont);
	menuText.setString("'P' - Play\n'I' - Instructions");
	menuText.setCharacterSize(30);
	menuText.setFillColor(sf::Color::Black);

	sf::Text scoreText;
	scoreText.setFont(gameFont);
	scoreText.setCharacterSize(24);
	scoreText.setString("Score: " + std::to_string(newScore));
	scoreText.setFillColor(sf::Color::Black);

	sf::Text gameOverText;
	gameOverText.setFont(gameFont);
	gameOverText.setString("Game Over");
	gameOverText.setFillColor(sf::Color::White);
	gameOverText.setCharacterSize(24);
	gameOverText.setPosition(335, 0);

	sf::Text restartText;
	restartText.setFont(gameFont);
	restartText.setString("Press 'R' to restart");
	restartText.setFillColor(sf::Color::White);
	restartText.setCharacterSize(24);
	restartText.setPosition(280, 400);

	sf::Text returnMenu;
	returnMenu.setFont(gameFont);
	returnMenu.setString("Press 'M' to return to menu");
	returnMenu.setFillColor(sf::Color::White);
	returnMenu.setCharacterSize(24);
	returnMenu.setPosition(222, 430);

	sf::Text leaderBoardText;
	leaderBoardText.setFont(gameFont);
	leaderBoardText.setCharacterSize(24);
	leaderBoardText.setPosition(20, 100);
	leaderBoardText.setFillColor(sf::Color::White);

	sf::Text autoText;
	autoText.setFont(gameFont);
	autoText.setString("Auto play - on");
	autoText.setCharacterSize(15);
	autoText.setPosition(343, 30);
	autoText.setFillColor(sf::Color::Black);

	sf::Text pauseText;
	pauseText.setFont(gameFont);
	pauseText.setString("Paused");
	pauseText.setCharacterSize(45);
	pauseText.setPosition(320, 200);
	pauseText.setFillColor(sf::Color::Black);

	bool moveUp = false;
	bool moveDown = false;
	bool autoPlay = false;

	bool updateScore = true;
	bool gameOver = false;
	bool instruction = false;
	
	bool startGameMusic = true;
	bool startGameOverMusic = true;
	bool startMenuMusic = true;

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::P)
				{
					if (!gameOver)
					{
						updateScore = true;
					}
					if (machine == menu)
					{
						machine = play;
						clock.restart();
					}
					else if (gameOver == true)
					{
					}
					else if (machine == pause)
					{
						machine = play;
					}
					else if (machine == play)
					{
						machine = pause;
					}
				}
				if (event.key.code == sf::Keyboard::M)
				{
					autoPlay = false;
					instruction = false;
					if (machine == play)
					{
						machine = menu;
					}
					newScore = 0;
					trash.speed = -5;
					
					for (int i = 0; i < 4; i++)
					{
						trash.reposition();
					}
					gameOver = false;
					updateScore = false;
				}
				if (event.key.code == sf::Keyboard::I)
				{
					machine = menu;
					gameOver = false;
					instruction = true;
					updateScore = false;
					autoPlay = false;
				}
				if (event.key.code == sf::Keyboard::G)
				{
					machine = play;
					gameOver = true; 
					autoPlay = false;
					if (!gameOver)
					{
						updateScore = true;
					}
				}
				if (event.key.code == sf::Keyboard::A)
				{
					autoPlay = true;
					moveUp = false;
					moveDown = false;
					if (!gameOver)
					{
						updateScore = true;
					}
				}
				if (event.key.code == sf::Keyboard::W)
				{
					moveUp = true;
					moveDown = false;
					autoPlay = false;
				}
				if (event.key.code == sf::Keyboard::S)
				{
					moveDown = true;
					moveUp = false;
					autoPlay = false;
				}
				if (event.key.code == sf::Keyboard::R)
				{
					if (machine == play)
					{
						autoPlay = false;
						for (int i = 0; i < 4; i++)
						{
							trash.reposition();
						}
						gameOver = false;
						updateScore = true;
						newScore = 0;
						trash.speed = -5;
					}
				}
			}
		}
		if (moveUp)
		{
			thePlayer.myPlayer.setPosition(0, 312);
		}
		if (moveDown)
		{
			thePlayer.myPlayer.setPosition(0, 395);
		}
		if (autoPlay)
		{
			if (trash.obstacleSprite.getPosition().y == 392)
			{
				thePlayer.myPlayer.setPosition(0, 312);
			}
			else if (trash.obstacleSprite.getPosition().y == 300)
			{
				thePlayer.myPlayer.setPosition(0, 395);
			}
		}

		window.clear();

		switch (machine)
		{
		case menu:
			gameMusic.stop();
			gameOverMusic.stop();
			window.draw(bgSprite);

			if (startMenuMusic == true)
			{
				menuMusic.play();
				startMenuMusic = false;
				startGameMusic = true;
			}
			if (!instruction)
			{
				window.draw(menuText);
				window.draw(titleText);
			}
			else
			{
				window.draw(instructions);
			}
			break;
		case play:

			if (!gameOver)
			{
				menuMusic.stop();
				gameOverMusic.stop();
			
				if (startGameMusic == true)
				{
					gameMusic.play();
					startGameMusic = false;
					startGameOverMusic = true;
					startMenuMusic = true;
				}
				
				window.draw(bgSprite);
				window.draw(goalText);
				window.draw(instructionText);
				window.draw(thePlayer.myPlayer);
				window.draw(trash.obstacleSprite);
				
				if (autoPlay == true)
				{
					window.draw(autoText);
				}

				if (clock.getElapsedTime().asSeconds() > 1)
				{
					newScore ++;
					scoreText.setString("Score: " + std::to_string(newScore));
					clock.restart();
					trash.speed -= 0.2;
				}
				trash.update();

				if (thePlayer.myPlayer.getGlobalBounds().intersects(trash.obstacleSprite.getGlobalBounds()))
				{
					trash.reposition();
					gameOver = true;
				}
				window.draw(scoreText);
			}
			else
			{
				menuMusic.stop();
				gameMusic.stop();

				if (startGameOverMusic == true)
				{
					gameOverMusic.play();
					startGameOverMusic = false;
					startGameMusic = true;
					startMenuMusic = true;
				}
				
				if (updateScore == true) 
				{
					loadLeaderBoard( firstScore,  secondScore,  thirdScore,  fourthScore,  fifthScore);
					if (newScore > firstScore) {
						fifthScore = fourthScore;
						fourthScore = thirdScore;
						thirdScore = secondScore;
						secondScore = firstScore;
						firstScore = newScore;
					}
					else if (newScore > secondScore) 
					{
						fifthScore = fourthScore;
						fourthScore = thirdScore;
						thirdScore = secondScore;
						secondScore = newScore;
					}
					else if (newScore > thirdScore) 
					{
						fifthScore = fourthScore;
						fourthScore = thirdScore;
						thirdScore = newScore;
					}
					else if (newScore > fourthScore) 
					{
						fifthScore = fourthScore;
						fourthScore = newScore;
					}
					else if (newScore > fifthScore) 
					{
						fifthScore = newScore;
					}
				}
				updateScore = false;

				leaderBoardText.setString("Your Score: " + to_string(newScore) + "\n \n" + "Leader Board: \n" + to_string(firstScore) + "\n" + to_string(secondScore) + "\n" + to_string(thirdScore) + "\n" + to_string(fourthScore) + "\n" + to_string(fifthScore));
				window.draw(leaderBoardText);
				window.draw(gameOverText);
				window.draw(restartText);
				window.draw(returnMenu);
				saveLeaderBoard(firstScore, secondScore, thirdScore, fourthScore, fifthScore);
			}
		break;
		case pause:
			gameMusic.pause();
			startGameMusic = true;

			window.draw(bgSprite);
			window.draw(pauseText);
		break;
		}
		window.display();
	}
return 0;
}
void saveLeaderBoard(int firstScore, int secondScore, int thirdScore, int fourthScore, int fifthScore)
{
	fstream file;
	string filename = "LeaderBoard.txt";

	file.open(filename, ios::out);

	file << firstScore << endl;
	file << secondScore << endl;
	file << thirdScore << endl;
	file << fourthScore << endl;
	file << fifthScore << endl;

	file.close();
}



