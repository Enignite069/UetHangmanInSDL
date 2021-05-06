#include "GameObject.h"

#include <ctime>
#include <tr1/unordered_map>

#include "SDLspace.h"
#include "optimize.h"

using namespace std::tr1;

GameObject::GameObject(SDLspace* SDLspace, int time) : SDL(SDLspace), playTime(time) {
    playing = true;
    win = 0;
    loss = 0;
}

void GameObject::startGame() {
    quit = false;
    system("cls");
    chooseCategory();
    chooseDifficulty();
    initWord();
    guessedWord = string(word.length(), '-');
    guessChar = ' ';
    badGuessCount = 0;
    maxSuggest = 10;
    badGuess = "";
    suggested = 0;
    guessedStr = "";
    animatedTime = 0;
    time(&startTime);
    for (unsigned int i = 0; i < word.length(); i++)
        if (word[i] == ' ')
            guessedWord[i] = ' ';
    updateSuggest();
}

void GameObject::chooseCategory() {
    category = "";
    while (category == "" && playing && !quit) {
        renderCategory();
        chooseCategoryEvent();
    }
}

void GameObject::chooseCategoryEvent() {
    SDL_Event event;
    if (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            playing = false;
            quit = true;
        } else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE) {
            playing = false;
        } else if (event.type == SDL_KEYUP) {
            string keyName = SDL_GetKeyName(event.key.keysym.sym);
            if (keyName.length() == 1 && keyName[0] >= '1' && keyName[0] <= '5')
                switch (keyName[0]) {
                    case '1':
                        category = "animal.txt";
                        catName = "Animal";
                        break;
                    case '2':
                        category = "astronomy.txt";
                        catName = "Space & Astronomy";
                        break;
                    case '3':
                        category = "transportation.txt";
                        catName = "Traffic";
                        break;
                    case '4':
                        category = "ielts.txt";
                        catName = "Ielts word list";
                        break;
                    case '5':
                        category = "PotterHead.txt";
                        catName = "Gift for Potterhead";
                        break;
                }
        }
    }
}

void GameObject::renderCategory() {
    SDL->createImageBackground("background1.jpg");
    SDL->createTextTexture("HANGMAN", 400, 50);
    SDL->createTextTexture("Made by:", 100, 170);
    SDL->createTextTexture("Duong Hung Anh", 150, 200);
    SDL->createTextTexture("Choose word category:", 100, 350);
    SDL->createTextTexture("1. Animal", 150, 400);
    SDL->createTextTexture("2. Space & Astronomy", 150, 450);
    SDL->createTextTexture("3. Traffic", 150, 500);
    SDL->createTextTexture("4. Ielts word list", 150, 550);
    SDL->createTextTexture("5. Gift for Potterhead", 150, 600);
    SDL->updateScreen();
}

void GameObject::chooseDifficulty() {
    difficult = -1;
    while (difficult == -1 && playing && !quit) {
        renderDifficulty();
        chooseDifficultyEvent();
    }
}

void GameObject::chooseDifficultyEvent() {
    SDL_Event event;
    if (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            playing = false;
            quit = true;
        } else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE) {
            playing = false;
        } else if (event.type == SDL_KEYUP) {
            string keyName = SDL_GetKeyName(event.key.keysym.sym);
            if (keyName.length() == 1 && keyName[0] >= '1' && keyName[0] <= '5')
                switch (keyName[0]) {
                    case '1':
                        difficult = 0;
                        break;
                    case '2':
                        difficult = 1;
                        break;
                }
        }
    }
}

void GameObject::renderDifficulty() {
    SDL->createImageBackground("background1.jpg");
    SDL->createTextTexture("HANGMAN", 400, 50);
    SDL->createTextTexture("Category: " + catName, 100, 250);
    SDL->createTextTexture("Choose word difficulty:", 100, 300);
    SDL->createTextTexture("1. Easy", 150, 350);
    SDL->createTextTexture("2. Hard", 150, 400);
    SDL->updateScreen();
}

void GameObject::renderWordMove(char guessedChar, int num) {
    time_t startT, endT;
    time(&startT);
    int i = -300;
    bool skip = false;
    while (i < 1000 && !skip) {
        SDL_Event event;
        wordMoveEvent(event, skip);
        SDL->createImageBackground("background2.jpg");
        SDL->createTextTexture(string("There ") + (num == 1 ? "is " : "are ") + to_string(num) + " of " + guessedChar, i + 165, 215);
        SDL->createTextTexture("Press 'Space' to skip", 300, 750);
        SDL->updateScreen();
        i += 5;
    }
    time(&endT);
    animatedTime += difftime(endT, startT);
}

void GameObject::wordMoveEvent(SDL_Event e, bool& skip) {
    if (SDL_PollEvent(&e)) {
        if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_SPACE)
            skip = true;
        if (e.type == SDL_QUIT) {
            playing = false;
            quit = true;
            skip = true;
        }
    }
}

void GameObject::initWord() {
    word = chooseWord(category, difficult);
    if (word.empty()) {
        cout << "No word available in " << category << endl;
        playing = false;
        quit = true;
    }
}

void GameObject::guessEvent() {
    SDL_Event event;
    guessChar = ' ';
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            playing = false;
            quit = true;
        } else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE) {
            playing = false;
        } else if (event.type == SDL_KEYUP) {
            string keyName = SDL_GetKeyName(event.key.keysym.sym);
            if (keyName == "Escape")
                playing = false;
            else if (keyName == "Space")
                guessChar = '$';
            else if (keyName.length() == 1 && keyName[0] >= 'A' && keyName[0] <= 'Z')
                guessChar = keyName[0];
        }
    }
}

void GameObject::handleGuess() {
    if (guessChar == ' ') return;
    if (guessChar == '$')
        getSuggest();
    else if (contains(guessedStr, guessChar))
        return;
    else if (contains(word, guessChar)) {
        updateGuessedWord();
        updateSuggest();
    } else if (!contains(badGuess, guessChar)) {
        badGuessed();
        renderWordMove(guessChar, 0);
    }
}

void GameObject::badGuessed() {
    badGuessCount++;
    badGuess += guessChar;
    guessedStr += guessChar;
}

bool GameObject::guessing() {
    return badGuessCount < MAX_BAD_GUESS && guessedWord != word && timeLeft > 0 && playing;
}

void GameObject::updateTime() {
    time_t now;
    time(&now);
    timeLeft = playTime - difftime(now, startTime) + animatedTime;
}

void GameObject::gameOver() {
    if (guessedWord != word)
        loss++;
    else
        win++;
    createGameOverSDL();
}

void GameObject::updateGuessedWord() {
    int n = guessedWord.length(), numOfChar = 0;
    for (int i = 0; i < n; i++) {
        if (word[i] == guessChar) {
            guessedWord[i] = guessChar;
            numOfChar++;
        }
    }
    guessedStr += guessChar;
    renderWordMove(guessChar, numOfChar);
}

void GameObject::updateSuggest() {
    if (suggested < maxSuggest) {
        int suggest = 0, n = guessedWord.length();
        unordered_map<char, int> m;
        for (int i = 0; i < n; i++)
            if (guessedWord[i] == '-')
                m[word[i]]++;
        suggest = m.size();
        maxSuggest = suggest / 2;
    }
    if (suggested > maxSuggest) maxSuggest = suggested;
}

void GameObject::getSuggest() {
    if (suggested < maxSuggest) {
        suggested++;
        while (true) {
            int i = rand() % guessedWord.length();
            if (guessedWord[i] == '-') {
                guessChar = word[i];
                updateGuessedWord();
                break;
            }
        }
    }
}

void GameObject::renderGameSDL() {
    SDL->createImageBackground("hang" + to_string(badGuessCount) + ".png");
    SDL->createTextTexture("Time: " + to_string(timeLeft), 750, 5);
    SDL->createTextTexture("Win : " + to_string(win), 750, 45);
    SDL->createTextTexture("Loss: " + to_string(loss), 750, 85);
    SDL->createTextTexture("Current Guess    :     " + guessedWord, 100, 650);
    SDL->createTextTexture("Bad Guesses      :     " + badGuess, 100, 700);
    SDL->createTextTexture("Used suggestions :     " + to_string(suggested) + "/" + to_string(maxSuggest) + "   (Press 'Space')", 100, 750);
    SDL->updateScreen();
    SDL_Delay(10);
}

void GameObject::renderGameOverSDL(int imageIndex) {
    string status = (guessedWord == word ? "free" : "hanged");
    SDL->createImageBackground(status + to_string(imageIndex) + ".png");
    if (timeLeft <= 0)
        SDL->createTextTexture("Time Up!!!", 750, 5);
    SDL->createTextTexture("Win : " + to_string(win), 750, 45);
    SDL->createTextTexture("Loss: " + to_string(loss), 750, 85);
    if (guessedWord == word)
        SDL->createTextTexture("Alright, you are free.", 100, 650);
    else
        SDL->createTextTexture("Joke's Over, You are DEAD!!", 100, 650);
    SDL->createTextTexture("Correct word: " + word, 100, 700);
    SDL->createTextTexture("Press 'Enter' to keep playing, 'ESC' to exit.", 100, 750);
    SDL->updateScreen();
}

void GameObject::createGameOverSDL() {
    int imageIndex = 0;
    while (!quit) {
        SDL_Event e;
        checkContinue(e);
        renderGameOverSDL(imageIndex);
        SDL_Delay(200);
        ++imageIndex %= 4;
    }
}

void GameObject::checkContinue(SDL_Event e) {
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT || (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)) {
            playing = false;
            quit = true;
        } else if (e.type == SDL_KEYUP &&
                   (e.key.keysym.sym == SDLK_RETURN ||
                    e.key.keysym.sym == SDLK_RETURN2 ||
                    e.key.keysym.sym == SDLK_KP_ENTER)) {
            playing = true;
            quit = true;
        }
    }
}
