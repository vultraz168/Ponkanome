#include <cmath>
#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "metronome.hpp"
#include "gui.hpp"
#include "ui.hpp"

MetronomeCommandHandler::MetronomeCommandHandler() {

}

MetronomeCommandHandler::~MetronomeCommandHandler() {
	
}

void MetronomeCommandHandler::append(MetronomeEntity& me) {
	std::shared_ptr<MetronomeEntity> metronome_entity_ptr(&me);
	metronome_entity_ptr_vector.push_back(metronome_entity_ptr);
}

void MetronomeCommandHandler::handle_metronome_commands(sf::Event event) {
	switch(event.type) {
		case sf::Event::MouseButtonPressed: {
			GetActiveCommand gac; //GRAB SOME GAK AND SQUISH IT FLAT
			gac.execute(event.mouseButton.x, event.mouseButton.y, metronome_entity_ptr_vector, active_metronome_entity_ptr);
		break;}
		case sf::Event::KeyPressed: {
			if(active_metronome_entity_ptr) {
				int old_tempo = active_metronome_entity_ptr->get_metronome().get_tempo();
				int tempo_adj_val = 1;
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
					tempo_adj_val *= 10;
				} //I'm not gonna implement a way to undo and redo your tempo changes you lazy shit.
				switch(event.key.code) {
					case sf::Keyboard::Key::Up : {
						active_metronome_entity_ptr->get_metronome().set_tempo(old_tempo + tempo_adj_val);
						break;
					}
					case sf::Keyboard::Key::Down : {
						active_metronome_entity_ptr->get_metronome().set_tempo(old_tempo - tempo_adj_val);
						break;
					}
				}
			}
		break;}
	}
	//try-catch everything
}

Command::Command() {
	
}

Command::~Command() {
	
}

void Command::execute() {
}

void GetActiveCommand::execute(int mouseX, int mouseY, std::vector<std::shared_ptr<MetronomeEntity>>& meptrv, std::shared_ptr<MetronomeEntity>& amep) {
	float min_dist = 1e10;
	int id = 0;
	std::for_each(meptrv.begin(), meptrv.end(), [&](std::shared_ptr<MetronomeEntity>& meptr){
		float cur_dist = std::hypot(mouseX - meptr->get_button_position()[0], mouseY - meptr->get_button_position()[1]);
		if(cur_dist < min_dist) {
			min_dist = cur_dist;
			id = meptr->get_metronome().get_id();
			amep = meptr; //not entirely sure how this works
		}
	});
	std::cout << "ID of closest metronome = " << id << std::endl;
}
