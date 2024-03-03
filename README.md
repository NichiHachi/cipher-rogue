# Cipher Rogue 

## Requirement
### Arch Linux
1. c++ compiler:

    ```bash
    sudo pacman -S gcc
    ```

2. SFML Library:

    ```bash
    sudo pacman -S sfml
    ```

### Ubuntu
1. c++ compiler:

    ```bash
    sudo apt update
    sudo apt install g++
    ```

2. SFML Library:

    ```bash
    sudo apt update
    sudo apt install libsfml-dev
    ```

### Fedora
1. c++ compiler:

    ```bash
    sudo dnf install gcc-c++
    ```

2. SFML Library:

    ```bash
    sudo dnf install SFML-devel
    ```

## Building

1. Clone the repository:

    ```bash
    git clone https://github.com/NichiHachi/cipher-rogue.git
    ```

2. Navigate to the project directory:
    
    ```bash
    cd cipher-rogue
    ```

3. Build the project using the makefile:
    
    ```bash
    make
    ```

4. Run the main file:

    ```bash
    ./bin/main
    ```
