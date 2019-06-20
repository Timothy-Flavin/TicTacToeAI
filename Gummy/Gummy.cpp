#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<cmath>
#include"Matrix.h"
#include"DenseNet.h"
#include"CSV.h"

csv* readCSV(const char* fileName);
void csvToDouble(csv* file);
DenseNet* init(DenseNet* net, int* numIterations, double* stepSize, csv* file, char* name);
void train(DenseNet* net, int numIterations, double* stepSize, csv* file);
void saveNet(DenseNet* net);

int main() {
	DenseNet * userNet = NULL;
	int numIterations = 0;
	double* stepSize=new double;
	csv* newcsv = new csv;
	char* name;
	userNet = init(userNet, &numIterations, stepSize, newcsv, name);
	userNet->print();
	bool stillGoing = true;
	while(stillGoing){
		stillGoing=false;
		train(userNet, numIterations, stepSize, newcsv);
		std::cout<<"would you like to train more? y/n"<<std::endl;
		char ans = 'a';
		std::cin>>ans;
		if(ans == 'y'){
			stillGoing = true;
			std::cout<<"How many iterations? ";
			std::cin>>numIterations;
			std::cout<<"step size: ";
			std::cin>>*stepSize;
		}
	}
	saveNet(userNet);
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout<<"done and authored by Timothy-Flavin"<<std::endl;
	std::cin.get();
	return 0;
}

csv* readCSV(const char* fileName) {
	std::ifstream infile;
	std::cout<<"file being read "<<fileName<<std::endl;
	infile.open(fileName);
	std::cout<<"opened file"<<std::endl;
	int curLine = 0;
	std::string dead = "";
	while (std::getline(infile,dead)) {
		curLine++;
		std::cout<<"why not working"<<std::endl;
		//infile>>dead;
		std::cout<<infile.good()<<", got line "<<curLine<<", "<<dead<<std::endl;
		/*if(infile.good()==0){
			std::cout<<"done"<<std::endl;
			break;
		}*/
	}
	std::cout << "line num: "<<curLine<<std::endl; //<< curLine;
	infile.clear();
	std::cout<<"cleared"<<std::endl;
	infile.seekg(0, std::ios::beg);
	std::cout<<"seeked 0"<<std::endl;
	csv* file = new csv;
	file->numLines = curLine;
	std::cout<<"made num lines"<<std::endl;
	file->data = new std::string[curLine];
	curLine = 0;
	std::cout<<"made empty file"<<std::endl;
	while (std::getline(infile, file->data[curLine])) {
		std::cout << file->data[curLine] << std::endl;
		curLine++;
	}
	std::cout<<"filled file"<<std::endl;
	int numVals = 1;
	for (int i = 0; i < file->data[0].size(); i++) {
		if (file->data[0].at(i) == ',') {
			numVals++;
		}
	}
	std::cout<<"got num vals"<<std::endl;
	std::cout << "\nnumVals: " << numVals<<std::endl;
	file->numVals = numVals;
	return file;
}

void csvToDouble(csv* file) {
	std::cout << "------------------------CSV TO DOUBLE---------------\n";
	file->numData = new double*[file->numLines];
	for (int i = 0; i < file->numLines; i++) {
		//std::cout << "starting thing\n";
		int valCount = 1;
		for (int j = 0; j < file->data[i].size(); j++) {
			if(file->data[i].at(j) == ','){
				valCount++;
			}
		}
		file->numData[i] = new double[valCount];
		//std::cout << "after error thing\n";
		int commaPos = 0;
		int vals = 0;
		for (int j = 0; j < file->data[i].size(); j++) {
			if (file->data[i].at(j) == ',') {
				//std::cout << "\nfirst if";
				file->numData[i][vals] = std::stod(file->data[i].substr(commaPos, j - commaPos));
				std::cout<<file->numData[i][vals]<<',';
				commaPos = j+1;
				vals++;
			}
			else if (j == file->data[i].size() - 1) {
				//std::cout << "\nsecond if";
				file->numData[i][vals] = std::stod(file->data[i].substr(commaPos));
				std::cout<<file->numData[i][vals]<<',';
				commaPos = j;
				vals++;
			}
		}
		std::cout<<std::endl;
	}
}

DenseNet* init(DenseNet* nete, int* numIterations, double* stepSize, csv* file, char* name) {
	srand(time(NULL));
	int netType = 0, numLayers = 0;
	int*layerSizes=NULL;
	char*fileName = new char[20];
	bool sigmoidOutput = true;
	std::cout<< "\nEnter the file name to save your net: ";
	std::cin.clear();
	//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.getline(name, 20);
	std::cout<<"\nname: "<<name;
	std::string path = "";
	std::cout << "\nWhat type of Neural net do you want?\n1: Dense, 2: Recurrent(NA), 3: TBD\n";
	std::cin >> netType;
	std::cout << "\nHow many layers do you want?\n";
	std::cin >> numLayers;
	layerSizes = new int[numLayers];
	std::cout << "number of nodes in layer 0 and last layer must \nadd up to number of values in csv file\n";
	for (int i = 0; i < numLayers; i++) {
		std::cout << "how many nodes in layer " << i << std::endl;
		std::cin >> layerSizes[i];
	}
	std::cout << "\nChoose output mode: 1: sigmoid, 0: raw\n";
	std::cin >> sigmoidOutput;
	std::cout << "\nChoose number of iterations to train: ";
	std::cin >> *numIterations;
	std::cout << "\nChoose step size: ";
	std::cin >> *stepSize;
	std::cout<< "\nput file into gummy.cpp folder and enter file name: ";
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.getline(fileName, 20);
	std::cout<<"FINISHED GATHERING DATA\n"<<fileName<<std::endl;
	DenseNet* net = new DenseNet(numLayers, layerSizes, sigmoidOutput, name);
	//net->print();
	std::cout<<"reading file"<<std::endl;
	csv* temp = readCSV(fileName);
	file->data=temp->data;
	file->numLines=temp->numLines;
	file->numVals=temp->numVals;
	csvToDouble(file);
	std::cout<<"FINISHED MAKING DENSE NET\n";
	return net;
}

void train(DenseNet* net, int numIterations, double* stepSize, csv* file) {
	std::cout<<"training net"<<std::endl;
	int percentNum = 1;
	if (numIterations >= 100) {
		percentNum = numIterations / 100;
	}
	std::cout<<"assigned percent "<<numIterations<<std::endl;
	net->print();
	int rowNum = 0;
	int numIn = net->getNumInputs();
	int numOut = net->getNumOutputs();
	std::cout<<"got in and outputs"<<std::endl;
	Matrix* inputMatrix = new Matrix(numIn,1);
	Matrix* outputMatrix = new Matrix(numOut,1);
	Matrix* netOutput = new Matrix(numOut, 1);
	std::cout<<"made training matrices"<<std::endl;
	for (int i = 0; i < numIterations; i++) {
		rowNum = rand() % file->numLines;
		//std::cout<<"picked a row"<<std::endl;
		for(int j = 0; j < numIn; j++){
			inputMatrix->set(j,0,file->numData[rowNum][j]);
		}
		for(int j = 0; j < numOut; j++){
			outputMatrix->set(j,0,file->numData[rowNum][j+numIn]);
		}
		//std::cout<<"set input and outputs"<<std::endl;
		
		net->feedForward(inputMatrix);
		//std::cout<<"fed forward"<<std::endl;
		net->backProp(outputMatrix, *stepSize);
		//std::cout<<"back propped"<<std::endl;
		if (i%percentNum == 0) {
			std::cout << "percent done: " << i / percentNum<<"%\n";
			double error = 0;
			for(int rowNum = 0; rowNum < file->numLines; rowNum++){
				for(int j = 0; j < numIn; j++){
					inputMatrix->set(j,0,file->numData[rowNum][j]);
				}
				for(int j = 0; j < numOut; j++){
					outputMatrix->set(j,0,file->numData[rowNum][j+numIn]);
				}
				net->feedForward(inputMatrix);
				error+=net->calcError(outputMatrix);
			}
			std::cout<<"error: "<<error<<std::endl;
		}
	}
	for(int i = 0; i < 10; i++){
		rowNum = rand() % file->numLines;
		std::cout<<"input: ";
		for(int j = 0; j < numIn; j++){
			inputMatrix->set(j,0,file->numData[rowNum][j]);
			std::cout<<inputMatrix->get(j,0)<<", ";
		}
		std::cout<<std::endl;
		std::cout<<"desired output: ";
		for(int j = 0; j < numOut; j++){
			outputMatrix->set(j,0,file->numData[rowNum][j+numIn]);
			std::cout<<outputMatrix->get(j,0)<<", ";
		}
		std::cout<<std::endl;
		std::cout<<"actual output: ";
		netOutput = net->feedForward(inputMatrix);
		for(int j = 0; j < netOutput->getM(); j++){
			std::cout<<netOutput->get(j,0)<<", ";
		}
		std::cout<<std::endl;
	}
	
}

void saveNet(DenseNet* net){
	net->save();
}

DenseNet* loadNet(char* fileName){
	csv* loadnet = readCSV(fileName);
	csvToDouble(loadnet);
}