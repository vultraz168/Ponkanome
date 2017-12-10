#include <iostream>
#include <thread>

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

#include "metronome.hpp"
#include "gui.hpp"
#include "ui.hpp"

int main() { //it's all java to me
	
	//Pre-mortem:
	//What would be useful is an audio class that unifies SoundBuffer and Sound
	
	sf::Clock clock;
	
	Metronome metronome;
	Metronome metronome2;
	metronome.set_tempo(120);
	metronome2.set_tempo(100);
	
	//Rendering stuff
	sf::ContextSettings settings;
	settings.antialiasingLevel = 0;
	sf::RenderWindow window(sf::VideoMode(800, 600), "Ponkanome", sf::Style::Titlebar, settings);
	window.setFramerateLimit(60);
	
	MetronomeEntity met_ent(metronome, window);
	MetronomeEntity met_ent2(metronome2, window);
	
	MetronomeCommandHandler mch;
	mch.append(met_ent);
	mch.append(met_ent2);
	
	met_ent.set_position(200, 300);
	met_ent2.set_position(600, 300);
	
	int screen_mode = 1; //0 = metronome; 1 = sequencer
	//Eventually  figure out how to enum
	
	Sprite_Entity back("ui/base.png"); //I can't afford to hire an artist: the background
	Text_Entity title("PONKANOME");
	title.set_position(24, 24);
	
	//Q: How do we run rendering in a thread that isn't the main thread?
	std::thread metronome_thread([&](){
		//Ultra-sophisticated metronome handling
		while(window.isOpen()) {
			float cur_time = clock.getElapsedTime().asSeconds();
			met_ent.poll(cur_time);
			met_ent2.poll(cur_time);
		}
	}
	);
	
	metronome_thread.detach();
	
	while(window.isOpen())
	{
		
		//Some people think they can outsmart me. *sniff* Maybe. Maybe.
		//I have yet to meet one who can outsmart boolet.
		
		//resync command?
		
		//Ultra-sophisticated event handling
		sf::Event event;
		while (window.pollEvent(event)) {
			if(event.type == sf::Event::Closed) {
				window.close();
				std::cout << "Received close event" << std::endl;
			}
			if(screen_mode == 0) {
				mch.handle_metronome_commands(event);
			}
		}
		
		//Ultra-sophisticated render loop
		window.clear(sf::Color::Black);
		window.draw(back.get_sprite());
		window.draw(title.get_text(), title.get_transform());
		
		if(screen_mode == 0) {
			met_ent2.draw();
			met_ent.draw();
		}
		
		window.display();
	}
	return 0;
}