#include<iostream>
#include<cstdlib>
#include<ctime>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<cctype>

#include "chrome.h"

using namespace std;

/*We want to find the largest clique and output it,
 * -lets use a function to create a matrix from the csv? Works
 * -create random population.
 * -evaluate fitness.
 * -sort fitness
 *
 *  I don't think i'll use labels for the arrays, so A=0, B=1, ..., T=19
 * */


#define population_set 500


vector<vector<int>> create_matrix(string file_name);
int fitness_test(vector<int> person, vector<vector<int>> sample);
Chromosome breed(Chromosome c1, Chromosome c2);

int main(){
	Population village;
	Population village_2;// have seperate everything for this so they can be run simultaniosly

	vector<vector<int>> sample_five;
	vector<vector<int>> sample_six;
	sample_five = create_matrix("input_clique_5_new-1.csv");
	sample_six = create_matrix("input_clique_6_new-1.csv");
	
	srand((unsigned int)time(0));
	
	/*
	//Printing out the sample matrix
	cout << "\n";
	cout << "Sample 'five' Matrix" << endl;
	for(int i=0; i<20; i++){
		for(int j=0; j<20; j++){
			cout << sample_five[i][j] << " ";
		}
		cout << endl;
	}
	*/
	

	//creating the first town
	for(int i=0; i<population_set; i++){
		Chromosome chromosome;
		for(int j=0; j<20; j++){//this is the randomized chromosome
			//find num between 0 and 100, if under 20 put in a 1
			if(rand()%100 < 20){
				chromosome.add_gene(1);
			
			}else{
				chromosome.add_gene(0);
				
			}
		}
		village.add_chromosome(chromosome);
	}
	
	//creating the second town
	for(int i=0; i<population_set; i++){
		Chromosome chromosome;
		for(int j=0; j<20; j++){//this is the randomized chromosome
			//find num between 0 and 100, if under 20 put in a 1
			if(rand()%100 < 20){
				chromosome.add_gene(1);
			
			}else{
				chromosome.add_gene(0);
				
			}
		}
		village_2.add_chromosome(chromosome);
	}

	//village.print_population();

	/*FITNESS GAUNTLET!!!!!!!*/
	
	//find which vertex has highest fitness and put them at the top of the village, after it is swapped and in order breed from there.
	//Could essentially wipe out the whole village and replace with children? maybe. (only half)
	
	//assigning fitness for first set
	for(int i=0; i<population_set; i++){
		village.population[i].fitness = fitness_test(village.population[i].chromosome, sample_five);
	}
	//assign fitness for second set
	for(int i=0; i<population_set; i++){
		village_2.population[i].fitness = fitness_test(village_2.population[i].chromosome, sample_six);
	}
	
	//village.print_population_fitness(); // pre sort printing
	
	//we want to do a sort before the first gauntlet
	village.sort_population();
	village_2.sort_population();

	/*gonna want to do this section till I'm done*/
	int gen=1;
	while(village.population[0].fitness != 5){//this number is how big of clique i want to find
		village.gauntlet();
		for(int i=0; i<population_set; i++){
			if(village.population[i].is_invalid == false){
				village.population[i].fitness = fitness_test(village.population[i].chromosome, sample_five);
			}
		}
		village.sort_population();
		gen++;
	}

	//selecting and breeding second village
	int gen_2=1;
	while(village_2.population[0].fitness != 6){//find clique of 6
		village_2.gauntlet();
		for(int i=0; i<population_set; i++){
			if(village_2.population[i].is_invalid == false){
				village_2.population[i].fitness = fitness_test(village_2.population[i].chromosome, sample_six);
			}
		}
		village_2.sort_population();
		gen_2++;
	}
	
	/*
	//this is for printing out the top 10% (for testing)
	village_2.print_top_population_fitness();
	cout << "Generation: " << gen_2 << endl;
	for(int i=0; i<population_set; i++){
		if(village_2.population[i].fitness == 6){
			village_2.population[i].print_chromosome();
		}
	}
	*/

	//writing to file
	ofstream fp1;
	fp1.open("clique_solution.txt");

	if(!fp1){
		cout << "\n\nFailed to open file\n\n";
		exit(-1);
	}
	

	//printing out label for clique of five
	fp1 << "Largest clique in input_clique_5_new-1.csv: {";
	for(int i=0; i<20; i++){
		if(village.population[0].chromosome[i] == 1){
			fp1 << char(i+65) << ", "; 
		}
	}

	fp1 << "}" << "\n\n";
	
	//printing out label for clique of six
	fp1 << "Largest clique in input_clique_6_new-1.csv: {";
	for(int i=0; i<20; i++){
		if(village_2.population[0].chromosome[i] == 1){
			fp1 << char(i+65) << ", "; //converts int (ascii vlaue) to char
		}
	}
	fp1 << "}" << endl;

		
	

}

//func for file reading and turning into 2d array
vector<vector<int>> create_matrix(string file_name){
	vector<vector<int>> matrix(20, vector<int>(20));

	
	ifstream fp;
	fp.open(file_name);

	if(!fp){
		cout << "\n\nFailed to open file\n\n";
		exit(-1);
	}
	string buffer;
	string char_buffer;
	char *pointer_buffer;
	int int_buffer;

	
	getline(fp, buffer);
	int i;
	for(int j=0; j<20; j++){//horizontal node
		i = 0;
		//grab whole horizontal line
		getline(fp, buffer);
		stringstream s(buffer); //split it apart
		while(getline(s, char_buffer, ',')){//one character seperated by ','
			if(isdigit(char_buffer[0]) == 0){ //isdigit requires a char
				continue;
			}else{
				pointer_buffer = &char_buffer[0];
				int_buffer = atoi(pointer_buffer); //turning the char into an int using char*
				matrix[i][j] = int_buffer;
			}
			i++;

		}
			
			
	}
	return matrix;
}

//input is the chromosome we want to test as well as the sample
int fitness_test(vector<int> person, vector<vector<int>> sample){//single chromosome, whole matrix
	vector<int> potential_clique; //may need to empty after

	int fitness = 0;
	bool is_clique = true;
	for(int i=0; i<20; i++){
		for(int j=i+1; j<20; j++){
			if(person[i] == 1 && person[j] == 1){
				if(sample[i][j]){
				}
				else{
					is_clique = false;
					break;
				}
			}
		}
	}
	

	//this finds the actual fitness after the vector is approved
	if(is_clique){
		for(int i=0; i<20; i++){
			if(person[i] == 1){
				fitness++;
			}
		}
		return fitness;
	}else{
		return 0;	
	}
}





Chromosome breed(Chromosome c1, Chromosome c2){
	Chromosome child;
	//create new out of breeding
	for(int i=0; i<20; i++){
		if(rand()%100 < 50){//50/50 if either parent
			if(rand()%100 > 0){//this is for mutation
				child.add_gene(c1.chromosome[i]);
			}else{
				if(c1.chromosome[i] == 0){// 1% chance
					child.add_gene(1);
				}else{
					child.add_gene(0);
				}
			}
		}else{
			if(rand()%100 > 0){
				child.add_gene(c2.chromosome[i]);
			}else{
				if(c2.chromosome[i] == 0){// this is for doing the opposite
					child.add_gene(1);
				}else{
					child.add_gene(0);
				}
			}
		}
	}
	return child;
}

