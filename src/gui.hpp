#pragma once
#include <string>
#include <vector>

#include "SFML/Graphics.hpp"
#include "metronome.hpp"

class Sprite_Entity { //Single sprite, single-texture gui entity. Sufficient for this application.
	public:
		Sprite_Entity(std::string texture_path);
		~Sprite_Entity();
		sf::Sprite& get_sprite();
	private:
		sf::Texture texture;
		sf::Sprite sprite;
};

class Text_Entity {
	public:
		Text_Entity();
		Text_Entity(std::string string);
		~Text_Entity();
		sf::Text& get_text(); //extremely creative function names
		void set_text(std::string string);
		void set_position(int x, int y);
		sf::Transform& get_transform();
	private:
		sf::Transform transform;
		sf::Font font;
		sf::Text text;
};

class ClockShape { //hollow regular n-gon
//you could actually make it an elliptical thingy if you cared enough
	public:
		ClockShape(int reso, int th, int rad, float rg);
		~ClockShape();
		sf::VertexArray& get_shape();
		sf::Transform& get_transform();
		void calculate(int reso, int th, int rad); //Calculates the initial shape (which is triangles)
		//reso = resolution (number of triangles)
		//th = thickness in px
		//rad = radius in px
		void display(float curValue); //Modifies color value curValue/range way around
		void set_position(int x, int y);
	private:
		sf::VertexArray hollow_arc;
		sf::Transform transform;
		int resolution;
		int thickness;
		int radius;
		float range;
		const double PI = 3.14159265358973;
};

class SequencerShape { // A long strip of quads. Everything should have the same xmin/xmax
	public: 
		SequencerShape();
		~SequencerShape();
		sf::VertexArray& get_shape();
		sf::Transform& get_transform();
		void calculate(int xmin, int xmax, int nx, int ny);
	private:
		sf::VertexArray quadstrip;
		sf::Transform transform;
		float scroll; //0-1
		int heighth; //I have no idea
		//what a fuck is this really?
};

class MetronomeEntity {
	public:
		MetronomeEntity(Metronome& met, sf::RenderWindow& win);
		~MetronomeEntity();
		void poll(float time);
		void draw();
		void set_position(int x, int y);
		Metronome& get_metronome();
		float* get_button_position();
	private:
		Metronome& metronome;
		sf::RenderWindow& window;
		
		Text_Entity tempo;
		Text_Entity time_sig;
		Text_Entity beat_count;
		Text_Entity beat_count_index;
		
		float small_progress;
		float large_progress;
		float button_position[2]; //setting this will NOT change the position. Use set_position.
		
		int num;
		sf::Transform transform;
		ClockShape c1; //Total progress
		ClockShape c2; //Fractional progress
		ClockShape c3; //Per-beat progress
};

class SequencerEntity {
	public:
		SequencerEntity(Metronome& met, sf::RenderWindow& win);
		~SequencerEntity();
		void poll(float time);
		void draw();
};
