#pragma once
#include <vector>
#include <memory>
#include "SFML/System.hpp"
#include "gui.hpp"

struct MetronomeCommandHandler {
	MetronomeCommandHandler();
	~MetronomeCommandHandler();
	
	void append(MetronomeEntity& me);
	
	std::vector<std::shared_ptr<MetronomeEntity>> metronome_entity_ptr_vector; //aw fug we used a ptr.
	std::shared_ptr<MetronomeEntity> active_metronome_entity_ptr;
	bool clicked;
	
	void handle_metronome_commands(sf::Event event);
};

class Command {
	public: 
		Command();
		~Command();
		virtual void execute();
};

class GetActiveCommand : public Command {
	public:
		void execute(int mouseX, int mouseY, std::vector<std::shared_ptr<MetronomeEntity>>& meptrv, std::shared_ptr<MetronomeEntity>& amep);
};

