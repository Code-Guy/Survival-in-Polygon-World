# Survival-in-Polygon-World
A simple bullet hell game implemented with Qt.

![main](https://github.com/Code-Guy/Survival-in-Polygon-World/blob/master/snapshots/QQ%E6%88%AA%E5%9B%BE20170113190429.png?raw=true)

###Introduction
Welcome to the mysterious polygon world! All you need to do is survive!

It's a world full of polygon, polygon, and polygon! Your spaceship accidentally rushed into this weird polygon world, surrounded by  amounts of primitive enemies. Now, load your bullets and booms, it's your time to start the magical adventure!

###How to use
Use W A S D to move the spaceship. The spaceship is auto-firing.

There are 10 kinds of polygonal enemies: "Triangle", "Square", "Pentagon", "Hexagon", "Heptagon", "Octagon", "Nonagon", "Decagon", "Hendecagon", "Dodecagon". Each polygonal enemy will shot bullets from it's edges with certain speed and bullet form.

There are 4 kinds of enemy bullets: "Common", "Bounce", "Split", "Bounce&Split".

There are 3 kinds of players: "Jet", "Boomer", "Glider". All kinds of player is same except for shape.

There are 7 kinds of items: "First Aid", "Boom", "Speed Up(Bullet)", "Shield", "Add Columns(Bullet)", "Clean Enemy Bullets", "Wave".

###Feature
All game GUIs are integrated into a FSM(Finite State Machine)

some .h .cpp file descriptions:

 - audios.h/cpp: Define audio sources used in game, all of these are  - global phonon parameters 
 - audios.h/cpp: Define audio sources used in game, all of these are  - global phonon parameters 
 - boomanim.h/cpp: Define boom animation 
 - bullet.h/cpp: Bullet, consist of player bullet and enemy bullet
 - constants.h/cpp: All game constants 
 - database.h: All global database parameters 
 - enemy.h/cpp: Enemy class 
 - fsmmanager.h/cpp: Define a FSM Manager
 
###Framework
 Qt 4.8.2
 
###More Examples
![overlook](https://github.com/Code-Guy/Survival-in-Polygon-World/blob/master/snapshots/QQ%E6%88%AA%E5%9B%BE20170113190520.png?raw=true)

![overlook](https://github.com/Code-Guy/Survival-in-Polygon-World/blob/master/snapshots/QQ%E6%88%AA%E5%9B%BE20170113190531.png?raw=true)

![overlook](https://github.com/Code-Guy/Survival-in-Polygon-World/blob/master/snapshots/QQ%E6%88%AA%E5%9B%BE20170113190249.png?raw=true)
