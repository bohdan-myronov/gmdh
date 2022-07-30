
#include <vector>
#include <iostream>
#include "matrix_stuff.cpp"
#include <math.h>
using namespace std;

class model_stuff{
public:
	vector<float> find_coefficients (vector<float>model,vector<vector<float>>dataX,vector<vector<float>>dataY)
	{
		matrix_stuff matrices;

		vector<int> zero_coeff_index;
		zero_coeff_index.clear();
		for (int i = model.size()-1;i > 0; i--){
			if (model[i] == 0){
				zero_coeff_index.push_back(i);
				for (int j =0; j < dataX.size();j++){
					dataX[j].erase(dataX[j].begin() + i);
				}
			}
		}
			
		vector<vector<float>> result;
		vector<vector<float>> transposed = matrices.transpose(dataX);
		result = matrices.multiply(transposed,dataX);
		result = matrices.invert(result);
		result = matrices.multiply(result, transposed);
		result = matrices.multiply(result,dataY);
		vector<float> result_shaped;
		for (int i = 0; i < result.size();i++){
			result_shaped.push_back(result[i][0]);
		}
		for (int i = zero_coeff_index.size()-1; i >= 0 ; i--){
			result_shaped.emplace(result_shaped.begin()+ zero_coeff_index[i],0);
			}

		return result_shaped;
	}
	float calculate_equation(vector<float> equation, float x1, float x2){
		float sum = 0;
		sum  = equation[0] + equation[1] * x1  + equation[2] * x2 + equation[3]  *x1 *x1;
		return sum;
	}
	float find_criterion(vector<float> model, vector<vector<float>> source, vector<vector<float>>Y)
	{
		float criterion = 0;
		vector<float> Yreg ;
		Yreg.clear();
		for (int i = 0; i < source[0].size();i++){
			float y = calculate_equation(model,source[0][i],source[1][i]);
			Yreg.push_back(y);
		}
		//cout << "yregsum: " << Yregsum <<"\n";
		vector<float> Y_Yreg2;
		Y_Yreg2.clear();
		float y_yreg2 = 0;
		for (int i = 0; i < Y.size();i++){
			y_yreg2 = powl(Y[i][0] - Yreg[i],2);
			Y_Yreg2.push_back(y_yreg2);
		}
		//cout << "y_yreg2sum: " << y_yreg2sum << "\n";
		for (int i = 0; i < Y_Yreg2.size();i++){
			criterion += Y_Yreg2[i];
		}

		float Y_sum = 0;
			for (int i =0; i < Y.size(); i++){
			Y_sum += Y[i][0];
		}
		//cout << "Ysum: " <<Y_sum << "\n";
		criterion = criterion/Y_sum;
		return criterion;
		
	}
	
};
