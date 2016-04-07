#include "menu.hpp"

Menu::Menu(Manager& manager) : View(manager){
    
    manager.getResourceManager().setMusic("menu.wav");
    manager.getResourceManager().playMusic();
    
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
}

Menu::~Menu(){
}


void Menu::MoveUp()
{   
    //change la couleur du choix sélectionné
    if(selection-1 >= 0)
    {
        choice[selection].setColor(sf::Color::White);
        selection--;
        choice[selection].setColor(sf::Color::Red);
    }
}

void Menu::MoveDown()
{
    //change la couleur du choix sélectionné
    if(selection+1 < NB_CHOICES)
    {
        choice[selection].setColor(sf::Color::White);
        selection++;
        choice[selection].setColor(sf::Color::Red);
    }
}

void Menu::frame(){
    sf::RenderWindow& window = manager.getWindow();
    window.clear(sf::Color(66, 40, 245));

    const std::vector<sf::Event>& events = manager.getEvents();
        
    for (unsigned int i = 0; i < events.size(); i++) {
        const sf::Event& event = events[i];
    
        // gestion des touches
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) {
                MoveUp();
            }
            if (event.key.code == sf::Keyboard::Down) {
                MoveDown();
            }
            if (event.key.code == sf::Keyboard::Return) {
                //si on choisit "jouer", la vue se met sur Lvl_menu
                if(selection==0){
                    std::shared_ptr<View> lvl_menu = std::shared_ptr<View>(new Lvl_menu(manager));
                    manager.setView(lvl_menu);
                }
                if(selection==1){
                    
                }
                //si on choisit "créer un niveau", la vue se met sur Editor
                if(selection==2){
                    std::shared_ptr<View> editor = std::shared_ptr<View>(new Editor(manager));
                    manager.setView(editor);   
                }
            }
        }
    }

    for(int i=0; i<NB_CHOICES; i++)
    {
        window.draw(choice[i]);
    }
    window.display();

}
