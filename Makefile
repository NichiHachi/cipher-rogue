cc = g++
OBJDIR = ./obj
OBJC = $(OBJDIR)/Bullet.o $(OBJDIR)/Bombshell.o $(OBJDIR)/Wall.o $(OBJDIR)/Player.o $(OBJDIR)/Enemy.o $(OBJDIR)/EnemyCharger.o $(OBJDIR)/EnemyShooter.o $(OBJDIR)/EnemySniper.o $(OBJDIR)/EnemySpawner.o $(OBJDIR)/EnemySeeker.o $(OBJDIR)/EnemyTurret.o $(OBJDIR)/Game.o
BINDIR = ./bin
SRCDIR = ./src
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

all: main

main: $(SRCDIR)/main.cpp $(OBJC)
	$(cc) -g $(SRCDIR)/main.cpp $(OBJC) -o $(BINDIR)/main $(LIBS)

$(OBJDIR)/Game.o: $(SRCDIR)/Game.cpp $(SRCDIR)/Game.h
	$(cc) -g -c $(SRCDIR)/Game.cpp -o $@
$(OBJDIR)/Bullet.o: $(SRCDIR)/Bullet.cpp $(SRCDIR)/Bullet.h
	$(cc) -g -c $(SRCDIR)/Bullet.cpp -o $@

$(OBJDIR)/Bombshell.o: $(SRCDIR)/Bombshell.cpp $(SRCDIR)/Bombshell.h
	$(cc) -g -c $(SRCDIR)/Bombshell.cpp -o $@

$(OBJDIR)/Wall.o: $(SRCDIR)/Wall.cpp $(SRCDIR)/Wall.h
	$(cc) -g -c $(SRCDIR)/Wall.cpp -o $@

$(OBJDIR)/Player.o: $(SRCDIR)/Player.cpp $(SRCDIR)/Player.h
	$(cc) -g -c $(SRCDIR)/Player.cpp -o $@

$(OBJDIR)/EnemyCharger.o: $(SRCDIR)/EnemyCharger.cpp $(SRCDIR)/EnemyCharger.h
	$(cc) -g -c $(SRCDIR)/EnemyCharger.cpp -o $@

$(OBJDIR)/EnemyShooter.o: $(SRCDIR)/EnemyShooter.cpp $(SRCDIR)/EnemyShooter.h
	$(cc) -g -c $(SRCDIR)/EnemyShooter.cpp -o $@

$(OBJDIR)/EnemySniper.o: $(SRCDIR)/EnemySniper.cpp $(SRCDIR)/EnemySniper.h
	$(cc) -g -c $(SRCDIR)/EnemySniper.cpp -o $@

$(OBJDIR)/EnemySpawner.o: $(SRCDIR)/EnemySpawner.cpp $(SRCDIR)/EnemySpawner.h
	$(cc) -g -c $(SRCDIR)/EnemySpawner.cpp -o $@

$(OBJDIR)/EnemyTurret.o: $(SRCDIR)/EnemyTurret.cpp $(SRCDIR)/EnemyTurret.h
	$(cc) -g -c $(SRCDIR)/EnemyTurret.cpp -o $@

$(OBJDIR)/EnemySeeker.o: $(SRCDIR)/EnemySeeker.cpp $(SRCDIR)/EnemySeeker.h
	$(cc) -g -c $(SRCDIR)/EnemySeeker.cpp -o $@

$(OBJDIR)/Enemy.o: $(SRCDIR)/Enemy.cpp $(SRCDIR)/Enemy.h
	$(cc) -g -c $(SRCDIR)/Enemy.cpp -o $@

clean:
	rm $(OBJDIR)/*.o
	rm $(BINDIR)/main