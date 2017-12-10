#include <memory>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <array>

#include "metronome.hpp"

Soundbank::Soundbank() {
	buffer_bank.resize(1,sf::SoundBuffer());
	buffer_bank.at(0).loadFromFile("snd/kick.wav");
	
	//pitch 0
	sound_bank.resize(1, sf::Sound());
	sound_bank.at(0).setBuffer(buffer_bank.at(0));
}

Soundbank::~Soundbank() {}

void Soundbank::play(int pitch, int velocity) {
	if((pitch >= 0) && (sound_bank.size() >= pitch)) {
		sound_bank.at(pitch).play();
	}
}

Beat::Beat() : pitch(0), velocity(128) {
}

Beat::~Beat() {
	//std::cout << "Deleted a beat" << std::endl;
}

std::vector<Beat>& Measure::current_beat() {
	//std::cout << "Current beat: " << beat_index << std::endl;
	return beat_vec.at(beat_index);
}

void Measure::change_time_signature(int numerator, int denominator) {
	//Undo/redo?
	beat_vec.resize(numerator, std::vector<Beat>(channel_count));
	time_numerator = numerator;
	time_denominator = denominator;
}

int Measure::current_index() {
	//std::cout << "Mesaure current beat" << std::endl;
	return beat_index;
}

int Measure::next() {
	if(beat_index >= time_numerator - 1) {
		std::cout << "Looping" << std::endl;
		beat_index = 0;
		return 1; //Loop
	}
	else {
		std::cout << "Incrementing beat index" << std::endl;
		beat_index++;
		return 0;
	}
}

Measure::Measure() : time_numerator(4), time_denominator(4), beat_index(0), channel_count(1) {
	//std::cout << "Created a measure with numerator " << time_numerator << " and denominator " << time_denominator << std::endl;
	change_time_signature(time_numerator, time_denominator);
}

int Measure::get_time_denominator() {
	return time_denominator;
}

int Measure::get_time_numerator() {
	return time_numerator;
}

Measure::~Measure() {
	std::cout << "Deleted a measure" << std::endl;
	beat_vec.clear();
	beat_vec.shrink_to_fit();
}

/* //DEPRECATED?
Measure& Metronome::operator[](int index) {
	return measure_vector[index];
}
*/

int Metronome::idcount = 0;

Measure& Metronome::current_measure() {
	return measure_vector.at(measure_index);
}

void Metronome::update_spb() {
	//std::cout << "Updating spb" << std::endl;
	//std::cout << measure_index << std::endl;
	seconds_per_beat = 240.0/(_bpm*measure_vector.at(measure_index).get_time_denominator()); //Problem goes here
	std::cout << "Updated spb: " << seconds_per_beat << std::endl;
	latest_integer = static_cast<int>(std::floor(last_time/seconds_per_beat));
	//target = seconds_per_beat;
}

void Metronome::play(std::vector<Beat>& beat_c) {
	std::cout << "Playing a beat: Measure " << measure_index << ", Beat " << measure_vector.at(measure_index).current_index() << "\n";
	++beat_count; //Just for the display.
	
	std::for_each(beat_c.begin(), beat_c.end(), [&](Beat& beat){
		soundbank.play(beat.pitch, beat.velocity); //The actual important part
	}); //lol javascript
}

void Metronome::next() {
	//std::cout << "Metronome next" << std::endl;
	if (measure_index >= measure_vector.size() - 1) {
		measure_index = 0;
	}
	else {
		measure_index++;
	}
}

//Cases we want to account for:
	//1. fmod(time, seconds_per_beat) is less than .001. This is the leading case. In this case, the number of beats we have actually played is EQUAL to the number of beats that should have been played. Or, clock_integer == latest_integer.
	//2. fmod(time, seconds_per_beat) is over seconds_per_beat - .003. This is the lagging case. In this case, the number of beats we have actually played is LESS than the number of beats we SHOULD have played. Or, clock_integer > latest_integer.
	//well, that's just, like, your opinion bro
	//If I have ADHD how can I into C++ anyways

//Can I be inspired to make a application that doesn't require sub-millisecond precision please?

float Metronome::unipoll(float time) { //1 millisecond is the timing precision of a very good drummer; 100 milliseconds is my timing precision (as a "drummer")

	last_time = time;
	int clock_integer = static_cast<int>(std::floor(time/seconds_per_beat)); //How many beats have theoretically played
	//Latest integer is how many beats we have actually played
	if((clock_integer != latest_integer) && (fmod(time, seconds_per_beat) <= .003)) {
		++latest_integer;
		play(current_measure().current_beat());
		if(current_measure().next()) { //increment beat and check if we can go to next measure
			next(); //go to next measure (loops around automatically)
			update_spb(); //time sig change
		}
	}
	return fmod(time, seconds_per_beat)/seconds_per_beat;
}

float Metronome::large_progress() {
	return (static_cast<float>(current_measure().current_index())/(current_measure().get_time_numerator()));
}

float Metronome::set_tempo(int bpm) {
	_bpm = bpm;
	update_spb();
	return seconds_per_beat;
}

int Metronome::get_tempo() {
	return _bpm;
}

int Metronome::get_id() {
	return id;
}

unsigned long Metronome::get_beat_count() {
	return beat_count;
}

void Metronome::add_measure() {
	//std::cout << "Added a measure" << std::endl;
	measure_vector.push_back(Measure());
	update_spb();
}

Metronome::Metronome() : measure_index(0), _bpm(120), seconds_per_beat(0.5), latest_integer(-1), beat_count(0), id(idcount) {
	std::cout << "Made new metronome with id " << id << std::endl;
	add_measure();
	update_spb();
	++idcount;
}

Metronome::~Metronome() {
	//std::cout << "Deleted a metronome" << std::endl;
	measure_vector.clear();
	measure_vector.shrink_to_fit();
}