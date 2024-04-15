cc = g++
OBJDIR = ./obj
OBJC = $(OBJDIR)/Bullet.o $(OBJDIR)/Bombshell.o $(OBJDIR)/Wall.o $(OBJDIR)/Player.o $(OBJDIR)/Enemy.o $(OBJDIR)/EnemyCharger.o $(OBJDIR)/EnemyShooter.o $(OBJDIR)/EnemySniper.o $(OBJDIR)/EnemySpawner.o $(OBJDIR)/EnemySeeker.o $(OBJDIR)/EnemyTurret.o $(OBJDIR)/Game.o $(OBJDIR)/Homepage.o
BINDIR = ./bin
SRCDIR = ./src
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

all: directories main test

directories: ${OBJDIR} ${BINDIR}

${OBJDIR}:
	mkdir -p ${OBJDIR}

${BINDIR}:
	mkdir -p ${BINDIR}

main: $(SRCDIR)/main.cpp $(OBJC)
	$(cc) -g $(SRCDIR)/main.cpp $(OBJC) -o $(BINDIR)/main $(LIBS)

test: $(SRCDIR)/test.cpp $(OBJC)
	$(cc) -g $(SRCDIR)/test.cpp $(OBJC) -o $(BINDIR)/test $(LIBS)

$(OBJDIR)/Game.o: $(SRCDIR)/Game.cpp $(SRCDIR)/Game.h
	$(cc) -g -c $(SRCDIR)/Game.cpp -o $@

$(OBJDIR)/Bullet.o: $(SRCDIR)/Projectile/Bullet.cpp $(SRCDIR)/Projectile/Bullet.h
	$(cc) -g -c $(SRCDIR)/Projectile/Bullet.cpp -o $@

$(OBJDIR)/Bombshell.o: $(SRCDIR)/Projectile/Bombshell.cpp $(SRCDIR)/Projectile/Bombshell.h
	$(cc) -g -c $(SRCDIR)/Projectile/Bombshell.cpp -o $@

$(OBJDIR)/Wall.o: $(SRCDIR)/Wall.cpp $(SRCDIR)/Wall.h
	$(cc) -g -c $(SRCDIR)/Wall.cpp -o $@

$(OBJDIR)/Player.o: $(SRCDIR)/Player/Player.cpp $(SRCDIR)/Player/Player.h
	$(cc) -g -c $(SRCDIR)/Player/Player.cpp -o $@

$(OBJDIR)/EnemyCharger.o: $(SRCDIR)/Enemy/EnemyCharger.cpp $(SRCDIR)/Enemy/EnemyCharger.h
	$(cc) -g -c $(SRCDIR)/Enemy/EnemyCharger.cpp -o $@

$(OBJDIR)/EnemyShooter.o: $(SRCDIR)/Enemy/EnemyShooter.cpp $(SRCDIR)/Enemy/EnemyShooter.h
	$(cc) -g -c $(SRCDIR)/Enemy/EnemyShooter.cpp -o $@

$(OBJDIR)/EnemySniper.o: $(SRCDIR)/Enemy/EnemySniper.cpp $(SRCDIR)/Enemy/EnemySniper.h
	$(cc) -g -c $(SRCDIR)/Enemy/EnemySniper.cpp -o $@

$(OBJDIR)/EnemySpawner.o: $(SRCDIR)/Enemy/EnemySpawner.cpp $(SRCDIR)/Enemy/EnemySpawner.h
	$(cc) -g -c $(SRCDIR)/Enemy/EnemySpawner.cpp -o $@

$(OBJDIR)/EnemyTurret.o: $(SRCDIR)/Enemy/EnemyTurret.cpp $(SRCDIR)/Enemy/EnemyTurret.h
	$(cc) -g -c $(SRCDIR)/Enemy/EnemyTurret.cpp -o $@

$(OBJDIR)/EnemySeeker.o: $(SRCDIR)/Enemy/EnemySeeker.cpp $(SRCDIR)/Enemy/EnemySeeker.h
	$(cc) -g -c $(SRCDIR)/Enemy/EnemySeeker.cpp -o $@

$(OBJDIR)/Enemy.o: $(SRCDIR)/Enemy/Enemy.cpp $(SRCDIR)/Enemy/Enemy.h
	$(cc) -g -c $(SRCDIR)/Enemy/Enemy.cpp -o $@

$(OBJDIR)/Homepage.o: $(SRCDIR)/Homepage.cpp $(SRCDIR)/Homepage.h
	$(cc) -g -c $(SRCDIR)/Homepage.cpp -o $@

documentation:
	doxygen doc/Doxyfile
	mv latex doc/
	mv html doc/

clean:
	rm $(OBJDIR)/*.o
	rm $(BINDIR)/main