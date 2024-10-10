#ifndef CHROME_H
#define CHROME_H

#include<iostream>
#include<cstdlib>
#include<ctime>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<cctype>

#define population_set 500

using namespace std; //should probably replace

vector<vector<int>> create_matrix(string file_name);
int fitness_test(vector<int> person, vector<vector<int>> sample);

class Chromosome{
	public:
		int fitness;
		bool is_invalid = true;
		vector<int> chromosome;

		
		void add_gene(int value){
			chromosome.push_back(value);
		}

		void replace_gene(int idx, int value){
			chromosome[idx] = value;
		}
		
		int get_fitness(int idx){
			return fitness;
		}

		void print_chromosome(){
			for(int i=0; i<20; i++){
				cout << chromosome[i] << " ";
			}
			cout << endl;
		}

		void update_fitness(vector<vector<int>> sample_file){
			fitness = fitness_test(chromosome, sample_file);
		}



		
};

Chromosome breed(Chromosome c1, Chromosome c2);

class Population{
	public:
		vector<Chromosome> population;
		
		void add_chromosome(Chromosome chromosome){
			population.push_back(chromosome);
		}
		
		void print_population(){
			cout << "\n\nPopulation:\n";
			for(int i=0; i<population_set; i++){
				population[i].print_chromosome();
			}
		}
		void print_population_fitness(){
			cout << "\n\nPopulation:\n";
			for(int i=0; i<population_set; i++){
				cout << "Fitness: " << population[i].fitness;
				if(population[i].fitness > 0){
					cout << "<";
				}
				if(population[i].fitness > 1){
					cout << "< ";
				}
				cout << endl;
			}
		}
		//prints the first 10% (top 10% after sort)
		void print_top_population_fitness(){
			cout << "\n\nPopulation:\n";
			for(int i=0; i<population_set/10; i++){
				cout << "Fitness: " << population[i].fitness;
				if(population[i].fitness > 0){
					cout << "<";
				}
				if(population[i].fitness > 1){
					cout << "< ";
				}
				cout << endl;
			}
		}

		void sort_population(){
			//sorting the village best to worst
			for(int j=0; j<population_set; j++){//do it a few times
				//this should be enough iterations for complete sorting
				for(int i=0; i<population_set-1; i++){
					if(population[i+1].fitness > population[i].fitness){//check if next one is bigger
						swap(population[i+1], population[i]); //bubble sort
					}
				}
			}	
		}

		
		void gauntlet(){
			//erase last half (lowest half) of population
			population.erase(population.begin()+population_set/2, population.end());
			//cout << "\n" << population.size() << endl;
			//create new children
			int parents = population_set/40;//5% of total pop, 2 parents each
			//TODO integer division
			//breed from the top 5% to replace 50% (20 per parent couple)
			for(int i=0; i<parents; i+=2){//each iteration should be all the breeding between parents
				for(int j=0; j<40; j++){//40 is for how many children each parents have
					Chromosome child;
					child = breed(population[i], population[i+1]);
					child.is_invalid = false;
					population.push_back(child);
				
				}
			}
			
			//cout << "\n" << population.size() << endl;
		}
};

#endif
