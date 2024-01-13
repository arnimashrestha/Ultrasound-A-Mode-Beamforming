#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>

using namespace std; 
//Part 1  Creating Data Arrays to Load Data From Text Files [6 marks]
//1) For createDataMatrix, create a double pointer of float type named RFData
float ** createDataMatrix(int numElement, int numSample){
    //2) Allocate a float pointer with numElement elements to RFData using the new keyword, and for each
    //of the float ointer
    float **RFData = new float* [numElement];
    //allocate a new float array of length numSample; you should have a numElement x numSample matrix after this step
    for (int i= 0; (i<numElement); i++){
        RFData[i]= new float[numSample];
    }
    ///3) Return RFData
    return RFData;
}
//4) For loadRFData, open the file with the file name stored in filename using ifstream
int loadRFData(float **RFData, const char *fileName, int numElement, int numSample){
    ifstream infile(fileName);
    
 //Return -1 if the function failed to open the file specified in filename
    if (infile.fail()) {
        return -1;
        }   

    

// Read the data from the text file and populate RFData matrix; the data are stored in the following format:
// the first numSample floating-point values are for element 1, the second numSample floating-point
// values are for element 2, so on and so forth; 
    char line[20]; 
    for (int i=0; i<numElement; i++){
        for (int j= 0; j<numSample; j++){
            // infile.getline(line, 20);
            infile.getline(line,20,'\n');
            RFData[i][j]= atof(line);
        } 
    } 
    return 0;
}     
//Part 2 
float *genScanlineLocation(int &numPixel){
    //Asking User How much depth they want in scanline
    float scanline; 
    float scanline_increment;
    float* ScanlineLocation = new float[numPixel]; // dynamically allocate array after initalization
 
    cout<<"How deep would the scanline be?"<<endl; 
    cin >>scanline; 
    cout<<"How many numbers of pixels should the scanline have?"<<endl; 
    cin >> numPixel;

    // add spacing
    float spacing = scanline / (numPixel-1); // scanline and numPixels are related
    //  Create an array scanlineLocation, whose first element is the starting depth (at 0m), last
    // element is the imaging depth, and the number of array elements is equal to number of pixels
    for (int i=0; i<numPixel; i++){
        ScanlineLocation[i]= scanline_increment;
        scanline_increment += spacing;
    }
    return ScanlineLocation; 
    
}
//Create an array eleLocation with 128 elements
float *genElementLocation(int numElement, float PITCH){
    
    float *eleLocation = new float[numElement];

    //using formula from assigment

    for(int n=0; n<numElement; n++) {
        eleLocation[n]= ((n- ((numElement-1.0)/2.0))*PITCH);
    }
    return eleLocation; 
}

//Part 3
//For createScanine, allocate a float array called scanline to store the results from beamform
//a. Return the float array scanline
float *createScanline(int numPixel) {
    float *scanline = new float[numPixel]; 
    return scanline;   
}

void beamform(float *scanline, float ** realRFData, float ** imagRFData, float *scanlinePosition, float *elementPosition, int numElement, int numSample, int numPixel, float FS, float SOS){
    //he ultrasound wave propagates down toward the imaging space of the ith position in scanlineLocation at the speed of sound
    //(SoS), so the forward time of flight tforward for the ith position can be calculated using the following equation
    for (int i= 0; i<numPixel; i++){
        //initializing Preal and PImage for later 
        float Preal= 0.0; 
        float Pimag= 0.0; 
        float tforward= (scanlinePosition[i]/SOS); 

        for (int k=0; k<numElement; k++){
            float tbackward= sqrt(pow(scanlinePosition[i],2)+ pow(elementPosition[k],2))/SOS; 
            float ttotal= tforward+tbackward;
            int s = floor(ttotal*FS); //FS is given to us already
            // if (s<numSample){
            Preal+= realRFData[k][s]; //we have to do += because in the instructions it tells us a formula of series, so all the terms have to be added
            Pimag+= imagRFData[k][s];
        }
        float mag= sqrt(pow(Preal,2) + pow(Pimag,2));
        scanline[i] = mag;
        // cout <<Preal<<endl; 
        // cout <<Pimag<<endl; 
    }
    
}
///Create and open an output file with the file name specified in filename using ofstream
int outputScanline(const char * fileName, float *scanlinePosition, float *scanline, int numPixel){
    ofstream outfile(fileName); 
    // outfile.open(fileName);
    if (outfile.fail()) {
        return -1;
    }   
    //terate through each of the scanline locations and scanline elements and store them into the output file.
    //Make sure to separate array and location values with a comma and use endl before outputting the next
    //array elements.
    for (int i=0; i <numPixel; i++){
        outfile << scanlinePosition[i] << "," << scanline[i] << endl;
    }
    return 0;
}
void destroyAllArrays(float *scanline, float **realRFData, float **imagRFData, float *scanlinePosition, float *elementPosition, int numElement, int numSample, int numPixel) {
   //deleting 1D array
   //1D array only has one memory space
    delete scanline;

    //deleting 2D array
    //2D array has series of memory, hence we need to use a for loop 
    //(Note to self: 3D arrays would have rows and columns of memorys - needs a nested loop)
    for (int i=0; i<numElement; i++){
        delete realRFData[i];
    }
    delete realRFData;
    //delete ImagRFdata
    for (int i=0; i <numElement; i++){
        delete imagRFData[i]; 
    }
    delete imagRFData;
    //delete scanline Position
    delete scanlinePosition;
    //delete element position
    delete elementPosition;
}
    
