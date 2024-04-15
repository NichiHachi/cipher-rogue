# Cipher Rogue 

Projet réalisé dans le cadre de L'UE LIFAP4 à l'université Claude Bernard Lyon 1, dans le cadre de notre deuxième année de cursus préparatoire aux écoles d'ingénieur Polytech (Peip). Les cours magistraux de cette UE ont été assurés par Alexandre Meyer et Nicolas Pronost, tous deux chercheurs à LIRIS, un laboratoire du CNRS. 

## Requirement
### Arch Linux
1. C++ compiler:

    ```bash
    sudo pacman -S gcc
    ```

2. SFML Library:

    ```bash
    sudo pacman -S sfml
    ```

3. Doxygen:

    ```bash
    sudo pacman -S doxygen
    ```


### Ubuntu
1. C++ compiler:

    ```bash
    sudo apt update
    sudo apt install g++
    ```

2. SFML Library:

    ```bash
    sudo apt update
    sudo apt install libsfml-dev
    ```

3. Doxygen:

    ```bash
    sudo apt update
    sudo apt install doxygen
    ```

### Fedora
1. C++ compiler:

    ```bash
    sudo dnf install gcc-c++
    ```

2. SFML Library:

    ```bash
    sudo dnf install SFML-devel
    ```

3. Doxygen:

    ```bash
    sudo dnf install doxygen
    ```

## Building

1. Build the project using the makefile:
    
    ```bash
    make
    ```

2. Run the main file:

    ```bash
    ./bin/main
    ```

3. Build the documentation with Doxygen :
    

    ```bash
    make documentation
    ```

## Organisation de l'archive

1. src: Contient les fichiers sources du projet.

2. doc: Contient la documentation du projet, le diagramme des classes, ma présantation orale, ainsi que le diagramme de Gantt.

3. bin: Contient les exécutables du projet, le dossier et son contenu sont générés lors de la compilation.

4. obj: Contient les fichiers objets du projet, le dossier et son contenu sont générés lors de la compilation.

5. Enemy: Contient les classes des ennemis
    - Enemy (Classe parent)
    - EnemyCharger
    - EnemySeeker
    - EnemyShooter
    - EnemySniper
    - EnemySpawner
    - EnemyTurret

6. Player: Contient la classe du joueur

7. Projectile: Contient la classe des projectiles
    - Bullet
    - Bombshell

8. makefile: Fichier permettant de compiler le projet sous Linux.

9. README.md: Fichier contenant les informations relatives au projet.
