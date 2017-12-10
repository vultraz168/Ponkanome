#pragma once
#include <vector>
#include <array>

#include "SFML/System.hpp"
#include "SFML/Audio.hpp"

class Soundbank { //monophonic sound player oh shiet nieger this is a problem.
	public: //pitch does not actually determine pitch in this case but instead the sound played by the metronome
		void play(int pitch, int velocity);
		Soundbank();
		~Soundbank();
	private:
		std::vector<sf::Sound> sound_bank;
		std::vector<sf::SoundBuffer> buffer_bank;
};

class Beat { //How about a pitch bank? We polyphonic now
	public:
		Beat();
		~Beat();
		
		int pitch; //Not actually MIDI. It's internal because fuck you and if you ever plan to use this in a DAW we have to feed your MIDI through a mapping first.
		int velocity; //0-256
};

//You wouldn't change time signatures in the middle of a measure, would you?
//Yes but what if you want to jump into the middle of a sequence?


class Measure { 
	public:
		Measure();
		~Measure();
		void change_time_signature(int numerator, int denominator);
		int get_time_numerator();
		int get_time_denominator();
		std::vector<Beat>& current_beat();
		int current_index();
		int next(); //returns 1 when restarting
		//Beat& operator[](int index);
		
		
	private:
		std::vector <std::vector<Beat>> beat_vec; //TODO: make this 2D.
		int channel_count;

		int beat_index;
		int time_numerator; //Really a redundant variable since it's the length of the beat vector but helpful nonetheless
		int time_denominator; //No we're not playing in 2/pi time.
};

class Metronome { //Specialized timer
	public:
	
		Metronome();
		~Metronome();
		
		void add_measure();
		void remove_measure(int index);
		//void poll(); //DEPRECATED
		float unipoll(float time);
		
		float large_progress();
		
		//Measure& operator[](int index); //Returns measure whose index is in current brackets
		void next();
		
		float set_tempo(int bpm);
		int get_tempo();
		
		unsigned long get_beat_count();
		void play(std::vector<Beat>& beat_c);
		Measure& current_measure();
		
		int get_id();
		
	private:
		static int idcount;
		int id; //super fancy!
		
		int measure_index;
		void update_spb();
		std::vector<Measure> measure_vector;
		
		int _bpm;
		float seconds_per_beat; //Referenced using x/4 time: The seconds per beat is 240/(bpm*time_denominator).
		float last_time;
		
		unsigned long beat_count; //This metronome has worked x beats without an accident
		int latest_integer; //Stores the integer quotient from fmod in unipoll every time we play a beat
		
		//float target;
		//sf::Clock& uniclock;
		Soundbank soundbank;
};