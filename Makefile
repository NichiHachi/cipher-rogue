all: main

OBJC = Bullet.o Bombshell.o Wall.o Player.o Enemy.o EnemyCharger.o EnemyShooter.o EnemySniper.o EnemySpawner.o EnemySeeker.o EnemyTurret.o Game.o

main: main.cpp $(OBJC)
	g++ -g main.cpp $(OBJC) -o main -lsfml-graphics -lsfml-window -lsfml-system

Game.o: Game.cpp Game.h
	g++ -g -c Game.cpp

Bullet.o: Bullet.cpp Bullet.h
	g++ -g -c Bullet.cpp

Bombshell.o: Bombshell.cpp Bombshell.h
	g++ -g -c Bombshell.cpp

Wall.o: Wall.cpp Wall.h
	g++ -g -c Wall.cpp

Player.o: Player.cpp Player.h
	g++ -g -c Player.cpp

EnemyCharger.o: EnemyCharger.cpp EnemyCharger.h
	g++ -g -c EnemyCharger.cpp

EnemyShooter.o: EnemyShooter.cpp EnemyShooter.h
	g++ -g -c EnemyShooter.cpp

EnemySniper.o: EnemySniper.cpp EnemySniper.h
	g++ -g -c EnemySniper.cpp

EnemySpawner.o: EnemySpawner.cpp EnemySpawner.h
	g++ -g -c EnemySpawner.cpp

EnemyTurret.o: EnemyTurret.cpp EnemyTurret.h
	g++ -g -c EnemyTurret.cpp

EnemySeeker.o: EnemySeeker.cpp EnemySeeker.h
	g++ -g -c EnemySeeker.cpp

Enemy.o: Enemy.cpp Enemy.h
	g++ -g -c Enemy.cpp

clean:
	rm *.o
	rm main