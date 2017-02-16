# Compiler Skizzle sur Linux

## Debian, Ubuntu et dérivés

### Installation des outils de compilation

Cette commande installe les outils nécessaires au développement sur les systèmes Debian (probablement déjà installés), ainsi que CMake.

```bash
sudo apt-get install build-essential cmake
```

### Installation de SFML

La librairie SFML est déjà enpaquetée pour la plupart des systèmes dérivés de Debian. Si la version est trop vieille (< 2.3.2), vous devrez [compiler la librairie par vous-même.](http://www.sfml-dev.org/tutorials/2.4/compile-with-cmake-fr.php)

```bash
sudo apt-get install libsfml-dev
```

### Installation de Boost

La librairie Boost est déjà enpaquetée pour la plupart des systèmes dérivés de Debian. Si la version est trop vieille (< 1.58.0), vous devrez [compiler la librairie par vous-même.](http://www.boost.org/doc/libs/1_46_1/more/getting_started/unix-variants.html)

```bash
sudo apt-get install libboost-dev libboost-system-dev libboost-filesystem-dev
```

### Installation de SFGUI

Il n'existe pas de paquet déjà préparé pour la librairie SFGUI. Il faut donc télécharger les sources et compiler la librairie manuellement. *Conseil : pour éviter de fausses manipulations, placez-vous dans un dossier ne contenant pas de données importantes (le dossier de téléchargements peut convenir).*

```bash
wget -O SFGUI.tar.gz $(curl -s https://api.github.com/repos/TankOs/SFGUI/releases | grep tarball_url | head -n 1 | cut -d '"' -f 4)
mkdir SFGUI
tar -zxf SFGUI.tar.gz -C SFGUI --strip-components=1
cd SFGUI
mkdir build
cd build
cmake -DSFGUI_BUILD_SHARED_LIBS=NO -DSFGUI_BUILD_EXAMPLES=NO ..
make
sudo make install
```

Ceci a pour effet de compiler SFGUI en version statique et d'installer ses librairies, ses en-têtes et sa documentation  dans `/usr/local`. Vous pouvez maintenant supprimer l'archive `SFGUI.tar.gz` et le dossier `SFGUI` si vous n'en avez plus besoin.

### Compilation du jeu

Toutes les dépendances nécessaires étant installées, vous
pouvez maintenant télécharger le code source du jeu, le compiler et le lancer.

```bash
wget -O skizzle.tar.gz https://github.com/matteodelabre/projet-cmi/archive/master.tar.gz
mkdir skizzle
tar -zxf skizzle.tar.gz -C skizzle --strip-components=1
cd skizzle
mkdir build
cd build
cmake ..
make
./skizzle
```

Et voilà ! :smiley:
