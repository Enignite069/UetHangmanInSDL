#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED


#include <ctime>
#include <string>

#include "SDLspace.h"
using namespace std;

class GameObject {
    const int MAX_BAD_GUESS = 7;
    string category;
    string catName;
    int difficult = -1;
    SDLspace* SDL;
    int playTime;
    int win;
    int loss;
    bool quit;
    string word;
    char guessChar;
    string guessedWord;
    string guessedStr;
    int badGuessCount;
    int suggested;
    int maxSuggest;
    int timeLeft;
    string badGuess;
    time_t startTime;
    int animatedTime;

   public:
    bool playing;
    GameObject(SDLspace*, int);
    void startGame();
    void chooseCategory();
    void renderCategory();
    void chooseCategoryEvent();
    void chooseDifficulty();
    void renderDifficulty();
    void chooseDifficultyEvent();
    void initWord();
    void getSuggest();
    void updateGuessedWord();
    void updateSuggest();
    bool guessing();
    void guessEvent();
    void handleGuess();
    void updateTime();
    void badGuessed();
    void gameOver();
    void checkContinue(SDL_Event);
    void renderGameSDL();
    void renderGameOverSDL(int);
    void createGameOverSDL();
    void renderWordMove(char, int);
    void wordMoveEvent(SDL_Event, bool&);
};


#endif // GAMEOBJECT_H_INCLUDED
