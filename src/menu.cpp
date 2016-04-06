#include "menu.hpp"

Menu::Menu(Manager& manager) : View(manager){
    
    //mise en place des propriétés des textes affichés dans le menu
    choice[0].setFont(manager.getResourceManager().getFont("Raleway-Regular.ttf"));
    choice[0].setColor(sf::Color::Red);
    choice[0].setPosition(sf::Vector2f(460, 400/(NB_CHOICES + 1)));

    for(int i=1; i < NB_CHOICES; i++)
    {
        choice[i].setFont(manager.getResourceManager().getFont("Raleway-Regular.ttf"));
        choice[i].setColor(sf::Color::White);
        choice[i].setPosition(sf::Vector2f(460, 400/((NB_CHOICES + 1))*(i+1)));
    }
    choice[0].setString("Jouer");
    choice[1].setString("Regles du jeu");
    choice[2].setString("Creer un niveau");
    choice[3].setString("Quitter");

    //choix sélectionné à l'ouverture du menu
    selection = 0; 

    if (manager.isKeyPressed(sf::Keyboard::Up)) {
        MoveUp();    
    }     
    if (manager.isKeyPressed(sf::Keyboard::Down)) {
        MoveDown();
    }
}

Menu::~Menu(){
}

void Menu::frame(){
    sf::RenderWindow& window = manager.getWindow();
    window.clear(sf::Color(66, 165, 245));
    
    //dessin des textes du menu
    for(int i=0; i<NB_CHOICES; i++)
    {
        window.draw(choice[i]);
    }
    window.display();
}

void Menu::MoveUp()
{
    if(selection-1 >= 0)
    {
        choice[selection].setColor(sf::Color::White);
        selection--;
        choice[selection].setColor(sf::Color::Red);
    }
}

void Menu::MoveDown()
{
    if(selection+1 >= NB_CHOICES)
    {
        choice[selection].setColor(sf::Color::White);
        selection++;
        choice[selection].setColor(sf::Color::Red);
    }
}
