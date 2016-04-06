#include "lvl_menu.hpp"

Lvl_menu::Lvl_menu(Manager& manager) : View(manager){
    
    //mise en place des propriétés des textes affichés dans le menu
    choice[0].setFont(manager.getResourceManager().getFont("Raleway-Regular.ttf"));
    choice[0].setColor(sf::Color::Red);
    choice[0].setPosition(sf::Vector2f(300, 400/(NB_LVL_CHOICES + 1)));

    for(int i=1; i < NB_LVL_CHOICES; i++)
    {
        choice[i].setFont(manager.getResourceManager().getFont("Raleway-Regular.ttf"));
        choice[i].setColor(sf::Color::White);
        choice[i].setPosition(sf::Vector2f(300, 400/((NB_LVL_CHOICES + 1))*(i+1)));
    }
    choice[0].setString("Tutoriel");
    choice[1].setString("Niveau 1");
    choice[2].setString("Niveau 2");

    //choix sélectionné à l'ouverture du menu
    selection = 0; 
}

Lvl_menu::~Lvl_menu(){
}


void Lvl_menu::MoveUp()
{   
    //change la couleur du choix sélectionné
    if(selection-1 >= 0)
    {
        choice[selection].setColor(sf::Color::White);
        selection--;
        choice[selection].setColor(sf::Color::Red);
    }
}

void Lvl_menu::MoveDown()
{
    //change la couleur du choix sélectionné
    if(selection+1 < NB_LVL_CHOICES)
    {
        choice[selection].setColor(sf::Color::White);
        selection++;
        choice[selection].setColor(sf::Color::Red);
    }
}

void Lvl_menu::frame(){
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
                //si on choisit "tutoriel", on charge le niveau
                //tutoriel et la vue se met sur Game
                if(selection==0){
                    std::shared_ptr<Editor> editor = std::shared_ptr<Editor>(new Editor(manager));
                    try {
                        // ouverture du niveau
                        std::ifstream file;
                        file.open("./levels/level1.dat", std::ios::binary | std::ios::in);
                        editor->load(file);
                        file.close();

                    } catch (const std::exception& exception) {
                        std::cerr << "Le programme a quitté après une erreur d'exécution." << std::endl;
                        std::cerr << exception.what() << std::endl;
                    }
                    std::shared_ptr<View> game = std::shared_ptr<View>(new Game(manager));
                    manager.setView(game);  
                }
            }
        }
    }

    for(int i=0; i<NB_LVL_CHOICES; i++)
    {
        window.draw(choice[i]);
    }
    window.display();

}

