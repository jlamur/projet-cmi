# Compiler le projet sur Linux

## Debian, Ubuntu et dérivés

### Installation des outils de compilation

Cette commande installe les outils nécessaires au développement sur les
systèmes Debian. Il est probable que ces outils soient déjà installés.
Si cette commande renvoie une erreur, renseignez-vous sur les paquets
de développement pour votre système.

```sh
$ sudo apt-get install build-essential cmake
```

### Installation de SFML

La librairie SFML est déjà packagée pour la plupart des systèmes dérivés
de Debian, notamment Ubuntu. Si la version est trop vieille (< \ 2.3.2), vous
devrez la compiler vous mêmes.

```sh
$ sudo apt-get install libsfml-dev
```

### Installation de SFGUI

Il n'existe pas de paquet SFGUI déjà préparé, il faut donc télécharger
les sources et les compiler. *Conseil :* pour éviter de fausses manipulations,
placez vous dans un dossier ne contenant pas de données importantes
comme votre dossier de téléchargements pour effectuer ces commandes.

```sh
$ wget -O SFGUI.tar.gz $(curl -s https://api.github.com/repos/TankOs/SFGUI/releases | grep tarball_url | head -n 1 | cut -d '"' -f 4)
$ mkdir SFGUI
$ tar -zxf SFGUI.tar.gz -C SFGUI --strip-components=1
$ cd SFGUI
$ cmake .
$ make
$ sudo make install
```

Vous pouvez supprimer l'archive `SFGUI.tar.gz` et le dossier `SFGUI`
si vous n'en avez plus besoin.

```sh
$ rm -r SFGUI.tar.gz SFGUI
```

### Installation de Boost

Comme nous n'avons besoin que d'une petite partie de Boost, et vu
la taille de la librairie Boost entière, il est plus sage de
l'installer manuellement pour pouvoir spécifier les parties qui
nous sont nécessaires. Si vous avez déjà installé les librairies
`system` et `filesystem` de Boost sur votre machine, vous pouvez
sauter cette étape. *Conseil :* pour éviter de fausses manipulations,
placez vous dans un dossier ne contenant pas de données importantes
comme votre dossier de téléchargements pour effectuer ces commandes.

```sh
$ wget -O boost.tar.bz2 https://sourceforge.net/projects/boost/files/latest/download
$ mkdir boost
$ tar -jxf boost.tar.bz2 -C boost --strip-components=1
$ cd boost
$ ./bootstrap.sh --with-libraries=system,filesystem
$ sudo ./b2 install
```

Vous pouvez supprimer l'archive `boost.tar.bz2` et le dossier `boost`
si vous n'en avez plus besoin.

```sh
$ rm -r boost.tar.bz2 boost
```

### Compilation du projet

Toutes les dépendances nécessaires étant installées, vous
pouvez maintenant télécharger le code source du projet et
le compiler.

```sh
$ wget -O skizzle.tar.gz https://github.com/matteodelabre/projet-cmi/archive/master.tar.gz
$ mkdir skizzle
$ tar -zxf skizzle.tar.gz -C skizzle --strip-components=1
$ cd skizzle
$ cmake .
$ make
$ ./skizzle
```

Et voilà ! :smiley:

