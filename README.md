Survival-in-Polygon-World
=========================

A simple bullet hell game implemented by Qt

All game GUIs are integrated into a FSM(Finite State Machine)

some .h .cpp file descriptions:
   abstractstate.h/cpp: The abstract state class that will be used in the DSM
   audios.h/cpp: Define audio sources used in game, all of these are global phonon parameters
   boomanim.h/cpp: Define boom animation
   bullet.h/cpp: Bullet, consist of player bullet and enemy bullet
   constants.h/cpp: All game constants
   database.h: All global database parameters
   enemy.h/cpp: Enemy class
   fsmmanager.h/cpp: Define a FSM Manager
