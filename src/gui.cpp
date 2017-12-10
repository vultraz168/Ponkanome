#include <string>
#include <iostream>
#include <cmath>

#include "SFML/Graphics.hpp"
#include "gui.hpp"
#include "metronome.hpp"

Sprite_Entity::Sprite_Entity(std::string texture_path) {
	if(!texture.loadFromFile(texture_path)) {
		std::cout << "Texture failed to load." << std::endl;
	}
	sprite.setTexture(texture);
}

Sprite_Entity::~Sprite_Entity() {

}

sf::Sprite& Sprite_Entity::get_sprite() {
	return sprite;
}

Text_Entity::Text_Entity() {
	font.loadFromFile("ui/abel.ttf"); //and cain
	text.setFont(font);
	text.setString("0");
	text.setCharacterSize(48);
	text.setFillColor(sf::Color::White);
}

Text_Entity::Text_Entity(std::string string) {
	font.loadFromFile("ui/abel.ttf"); //and cain
	text.setFont(font);
	text.setString(string);
	text.setCharacterSize(48);
	text.setFillColor(sf::Color::White);
}

Text_Entity::~Text_Entity() {
}

void Text_Entity::set_text(std::string string) {
	text.setString(string);
}

void Text_Entity::set_position(int x, int y) {
	transform.translate(x, y);
}

sf::Transform& Text_Entity::get_transform() {
	return transform;
}

sf::Text& Text_Entity::get_text() {
	return text;
}

ClockShape::ClockShape(int reso, int th, int rad, float rg) : resolution(reso), thickness(th), radius(rad), range(rg) { //just draws a hollow n-gon.
	hollow_arc.setPrimitiveType(sf::Triangles);
	calculate(reso, th, rad);
}

ClockShape::~ClockShape() {
	
}

void ClockShape::calculate(int reso, int th, int rad) {
	hollow_arc.resize(resolution*2 + 2); //need to overlap a bit to get the full circle
	for (int i = 0; i <= resolution*2; i += 2) {
		hollow_arc[i].position = sf::Vector2f(radius*std::sin(2*PI*i/(resolution*2)), -radius*std::cos(2*PI*i/(resolution*2))); //sin = x and cos = -y because I am a genius
		hollow_arc[i+1].position = sf::Vector2f((radius+thickness)*std::sin(2*PI*i/(resolution*2)), -(radius+thickness)*std::cos(2*PI*i/(resolution*2)));
		hollow_arc[i].color = sf::Color::White;
		hollow_arc[i+1].color = sf::Color::White;
	}
}

void ClockShape::display(float curValue) {
	//Get the points curValue/range the way around
	for (int i = 0; i <= resolution*2; i += 2) {
		if (i <= resolution*2*curValue/range) { //within the desired region
			hollow_arc[i].color = sf::Color(255, 255, 255, 255);
			hollow_arc[i+1].color = sf::Color(255, 255, 255, 255);
		} else { //not within the desired region
			hollow_arc[i].color = sf::Color(255, 255, 255, 15);
			hollow_arc[i+1].color = sf::Color(255, 255, 255, 15);
		}
	}
}

//2px of space between rectangles

sf::VertexArray& ClockShape::get_shape() {
	return hollow_arc;
}

sf::Transform& ClockShape::get_transform() {
	return transform;
}

void ClockShape::set_position(int x, int y) {
	transform.translate(x, y);
}

SequencerShape::SequencerShape() : heighth(20), scroll(0) {
	quadstrip.setPrimitiveType(sf::Quads);
}

SequencerShape::~SequencerShape() {
}

sf::VertexArray& SequencerShape::get_shape() {
	return quadstrip;
}

sf::Transform& SequencerShape::get_transform() {
}

void SequencerShape::calculate(int xmin, int xmax, int nx, int ny) {
	quadstrip.resize(nx*ny*4);
	float rect_width = xmax/nx - 2; //(xmax - 2nx)/nx
	for (int i = 0; i < ny; i++) {
		for (int j = 0; j < nx; j++) {
			
		}
	}
}

MetronomeEntity::MetronomeEntity(Metronome& met, sf::RenderWindow& win) : metronome(met), window(win), c1(200, 5, 175, 1), c2(200, 5, 168, 1), c3(200, 5, 161, 1), small_progress(0.0), large_progress(0.0), num(0) { //lol initializer lists
beat_count.get_text().setCharacterSize(12);
}

MetronomeEntity::~MetronomeEntity() {
}

void MetronomeEntity::poll(float time) {
	small_progress = metronome.unipoll(time);
	large_progress = metronome.large_progress();
	num = metronome.current_measure().get_time_numerator();
}

void MetronomeEntity::draw() {
	tempo.set_text(std::to_string(metronome.get_tempo()) + " BPM");
	time_sig.set_text(std::to_string(metronome.current_measure().get_time_numerator()) + "/" + std::to_string(metronome.current_measure().get_time_denominator()));
	beat_count.set_text("This metronome has worked " + std::to_string(metronome.get_beat_count()) + " beats without an accident.");
	beat_count_index.set_text(std::to_string(metronome.current_measure().current_index() + 1));
	c1.display(large_progress + small_progress/num);
	c2.display(large_progress + 1.0f/num);
	c3.display(small_progress);
	window.draw(c1.get_shape(), transform);
	window.draw(c2.get_shape(), transform);
	window.draw(c3.get_shape(), transform);
	window.draw(tempo.get_text(), transform * tempo.get_transform());
	window.draw(time_sig.get_text(), transform * time_sig.get_transform());
	window.draw(beat_count.get_text(), transform * beat_count.get_transform());
	window.draw(beat_count_index.get_text(), transform * beat_count_index.get_transform());
}

void MetronomeEntity::set_position(int x, int y) {
	transform.translate(x, y);
	button_position[0] = static_cast<float>(x);
	button_position[1] = static_cast<float>(y);
	tempo.set_position(-80, -100);
	time_sig.set_position(-30, -50);
	beat_count.set_position(-140, 40);
	beat_count_index.set_position(-10, 60);
}

Metronome& MetronomeEntity::get_metronome() {
	return metronome;
}

float* MetronomeEntity::get_button_position() {
	return button_position;
}