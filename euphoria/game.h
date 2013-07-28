// Euphoria - Copyright (c) Gustav

/** @file
Game related code.
 */

#ifndef EUPHORIA_GAME_H_
#define EUPHORIA_GAME_H_

/** Contains common game data.
 */
class Game {
  public:
    /** Constructor.
     */
    Game();
    ~Game();

    /** Returns if the game should keep running.
    @returns true if it should keep running, false if not.
     */
    bool keepRunning();

    /** Quit the game.
    Sets the internal run variable to false.
     */
    void quit();
  private:
    bool shouldKeepRunning;
};

#endif  // EUPHORIA_GAME_H_
