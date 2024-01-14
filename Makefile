all: main

main: main.cpp Bullet.o Player.o EnemyCharger.o EnemyShooter.o EnemySniper.o EnemySpawner.o EnemySeeker.o EnemyTurret.o Wall.o
	g++ main.cpp Bullet.o Player.o EnemyCharger.o EnemyShooter.o EnemySniper.o EnemySpawner.o EnemySeeker.o EnemyTurret.o Wall.o -o main -lsfml-graphics -lsfml-window -lsfml-system

Bullet.o: Bullet.cpp Bullet.h
	g++ -c Bullet.cpp

Wall.o: Wall.cpp Wall.h
	g++ -c Wall.cpp

Player.o: Player.cpp Player.h
	g++ -c Player.cpp

EnemyCharger.o: EnemyCharger.cpp EnemyCharger.h
	g++ -c EnemyCharger.cpp

EnemyShooter.o: EnemyShooter.cpp EnemyShooter.h
	g++ -c EnemyShooter.cpp

EnemySniper.o: EnemySniper.cpp EnemySniper.h
	g++ -c EnemySniper.cpp

EnemySpawner.o: EnemySpawner.cpp EnemySpawner.h
	g++ -c EnemySpawner.cpp

EnemyTurret.o: EnemyTurret.cpp EnemyTurret.h
	g++ -c EnemyTurret.cpp

EnemySeeker.o: EnemySeeker.cpp EnemySeeker.h
	g++ -c EnemySeeker.cpp

clean:
	rm *.o
	rm main