#include <iostream>
#include <mpi.h>
#include <vector>
#include <stdio.h>
#include "csv_reader.cpp"

#include "model_stuff.cpp"
#include <chrono>

using namespace std;

vector<int> get_slices(int model_count, int size)
{
	// 7
	// 3 4
	// 2 3 2
	int remainder = model_count%size; // 7%4 = 3 7%3 = 1 7%2 = 1 7%1 = 0 
	int split_size = model_count/size; // 7/4 = 1 7/3 = 2 7/2 = 3 7/1 = 7
	vector<int> slices;
	// assigning models for every process, 0 rank gets less work
	for (int i =0; i< size;i++){
		slices.push_back(split_size);
	}
	for (int j = 0; j < remainder;j++){
			slices[j] += 1;
	}
	return slices;
}
int are_arguments_correct(int argc, char *argv[])
{
	if (argc < 3){
		cout << "use: ./a <path_to_the_file> <number_of_columns_to_read>\n";
		throw;
	}
	int input = 0;
	// to avoid segmentation fault
	for (int i = 2; i <3; i++){
		input = strtol(argv[i],NULL,10);
		if (input < 4){
			cout << "Program need at least 4 columns to work.\n";
			throw;
		}	
	}
	return input;
}
int main(int argc, char *argv[])
{
	MPI_Init(&argc,&argv);
	int rank,size;
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	int input = are_arguments_correct(argc,argv);
	chrono::steady_clock::time_point begin;
	chrono::steady_clock::time_point begin_deep;
	chrono::steady_clock::time_point end;
	vector<int> slices;
	vector<vector<float>> assigned_models;
	
	
	vector<float> models{
		1,1,0,0,
		1,0,1,0,
		1,0,0,1,
		1,1,1,0,
		1,1,0,1,
		1,0,1,1,
		1,1,1,1,
	};
	
	int model_count = models.size()/4;
	slices = get_slices(model_count, size);
	csv_reader reader;
	model_stuff modeler;
	
	vector<vector<float>> content = reader.read_file(argv[1], input);
	vector<vector<vector<float>>> AB = reader.divide_content_to_A_B(content);
	vector<vector<float>> A_X = reader.fill_X_Matrix(AB[0]);
	vector<vector<float>> A_Y = reader.fill_Y_Matrix(AB[0]);
	vector<vector<float>> B_Y = reader.fill_Y_Matrix(AB[1]);
	
	begin_deep = std::chrono::steady_clock::now(); 
	vector<float> model;
	vector<int> index {0,2};
	if (rank == 0)
	{
		
 		// sending assigned models to another processes
		for (int i =0; i< slices.size()-1;i++){
			index[1] = slices[i];
			MPI_Send(&index[0],2,MPI_INT,i+1,0,MPI_COMM_WORLD);
			index[0] = index[0] + index[1];
		}
		// 0 gets the rest of the models
		index[1] = slices[slices.size()-1];		
	}
	else
	{	
		MPI_Recv(&index[0],2,MPI_INT,0,0,MPI_COMM_WORLD,&status);
		
	}
	
	for (int i = 0; i < index[1];i++){
			model.resize(4);
			assigned_models.push_back(model);
			for (int j = 0; j < 4; j++){
				assigned_models[i][j] = models[index[0]*4 + i*4 + j];
			}
	}
	
	vector<vector<float>> coefficients;
	vector<float> model_coefficients;
	
	for (int i =0;i<assigned_models.size();i++){
		model_coefficients=modeler.find_coefficients(assigned_models[i],A_X,A_Y);
		coefficients.push_back(model_coefficients);
	}
	
	MPI_Request request;
	if (rank == 0){
		vector<float> package;
		int models_to_get = model_count - slices[slices.size()-1];
		package.resize(4);
			
		for (int i = 0; i < models_to_get;i++){
			MPI_Recv(package.data(),4,MPI_FLOAT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
			coefficients.push_back(package);	
		}
		
		int best_model_index = 0;
		float best_model_internal_criterion;
		float min_external_criterion = ;
		float internal_criterion;
		float external_criterion;
		vector<float> externals;
		for (int i = 0; i < coefficients.size();i++){
			 internal_criterion = modeler.find_criterion(coefficients[i],AB[0],A_Y);
			 external_criterion = modeler.find_criterion(coefficients[i],AB[1],B_Y);
			 externals.push_back(external_criterion);
			 //cout << external_criterion << "\n";
			if (external_criterion < min_external_criterion){
				min_external_criterion = external_criterion;
				best_model_index = i;
				best_model_internal_criterion = internal_criterion;
			}
		}
		end = std::chrono::steady_clock::now();
		vector<int> model_raw {1,1,1,1};
		for (int i = 0; i < 4; i++){
			if (coefficients[best_model_index][i] == 0){
				model_raw[i] = 0;
			}
		}
		cout << "\nBest model found: [" << model_raw[0] << "," << model_raw[1] <<"," << model_raw[2] << "," << model_raw[3] << "]: \n";
		cout << coefficients[best_model_index][0] << " + " << coefficients[best_model_index][1] << "*x1 + "
			<< coefficients[best_model_index][2] << "*x2 + "
			<< coefficients[best_model_index][3] << "*x1^2"
			<< "\ninternal: " << best_model_internal_criterion
			<< "\nexternal criterion: " << min_external_criterion << endl;
		cout << "Time difference = " << chrono::duration_cast<std::chrono::microseconds>(end - begin_deep).count() << "[µs]" << endl;
	}
	else
	{	

		for (int i = 0; i < coefficients.size();i++){
			MPI_Isend(coefficients[i].data(),4,MPI_FLOAT,0,0,MPI_COMM_WORLD,&request);		
		}	
	}	
	MPI_Finalize();
}



