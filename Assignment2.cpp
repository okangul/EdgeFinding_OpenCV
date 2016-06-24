#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>



// For using my code you need to change the output source with name like f1 E1 E1_1 Es1 for different results
//Thanks Okan Gul

using namespace std;
using namespace cv;
int xGradient(Mat image, int x, int y)
{
    return image.at<uchar>(y-1, x-1) + 2*image.at<uchar>(y, x-1) +image.at<uchar>(y+1, x-1) -image.at<uchar>(y-1, x+1) -2*image.at<uchar>(y, x+1) - image.at<uchar>(y+1, x+1);
}
 
// Computes the y component of the gradient vector
// at a given point in a image
// returns gradient in the y direction
 
int yGradient(Mat image, int x, int y)
{
    return image.at<uchar>(y-1, x-1) + 2*image.at<uchar>(y-1, x) + image.at<uchar>(y-1, x+1) - image.at<uchar>(y+1, x-1) - 2*image.at<uchar>(y+1, x) - image.at<uchar>(y+1, x+1);
}





int main(int argc, char **argv) {
  


  
  if(argc != 2) {
    cout << "USAGE: skeleton <input file path>" << endl;
    return -1;
  }
  
  //Load two copies of the image. One to leave as the original, and one to be modified.
  //Done for display purposes only
  //Use CV_LOAD_IMAGE_GRAYSCALE for greyscale images
  Mat f1= imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  Mat f2= imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  Mat E= imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  Mat Es1= imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  Mat E1= imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  Mat E1_1= imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
 
  //Create a pointer so that we can quickly toggle between which image is being displayed
  Mat *image = &f1;
 
  //Check that the images loaded
  if(!f1.data || !E.data) {
    cout << "ERROR: Could not load image data." << endl;
    return -1;
  }

  

  double kernel1[3][3]={{0.0625,0.125,0.0625},{0.125,0.25,0.125},{0.0625,0.125,0.0625}};
  
  for(int a=2;a < f1.rows; a++){
    for(int b=2;b < f1.cols; b++){
      E.at<uchar>(a,b)=f1.at<uchar>(a-1,b-1)*kernel1[0][0]+f1.at<uchar>(a-1,b)*kernel1[0][1]+f1.at<uchar>(a-1,b+1)*kernel1[0][2]+f1.at<uchar>(a,b-1)*kernel1[1][0]+f1.at<uchar>(a,b)*kernel1[1][1]+ f1.at<uchar>(a,b+1)*kernel1[1][2]+ f1.at<uchar>(a+1,b-1)*kernel1[2][0]+ f1.at<uchar>(a+1,b)*kernel1[2][1]+f1.at<uchar>(a+1,b+1)*kernel1[2][2];
      
    }

  }
  double k=0.6;
  for (int i=0; i < image->rows*image->step; i++){
    E1= f1 * (1+k) - E * k;
  }


  int gx, gy, sum;
  for(int y = 0; y < f1.rows; y++)
            for(int x = 0; x < f1.cols; x++)
                Es1.at<uchar>(y,x) = 0.0;
 
  for(int y = 1; y < f1.rows - 1; y++){
      for(int x = 1; x < f1.cols - 1; x++){
        gx = xGradient(f1, x, y);

        gy = yGradient(f1, x, y);
        
        
        sum = abs(gx) + abs(gy);
        if(sum>255)
          sum=255;
        else if(sum<0)
          sum=0;
        Es1.at<uchar>(y,x) = sum;
      }
  }
  int N = 7;
  int min = (int)((double)-N / (double)2);
  int max = (int)((double)N/ (double)2);
  float MASK [N][N];
  float sigma = 1.4f;
  float s2 = sigma * sigma;
  float s4 = sigma * sigma * sigma * sigma;
   
  
  int xcount=0;

  for(int x = min; x <= max; ++x)
  {
    
    int ycount=0;
    for(int y = min; y <= max; ++y)
    {
      float val = (float)(-1.0f / (3.14 * (s4))) * (1.0f - ((x * x + y * y) / (2.0f * (s2)))) * (float)exp((-(x * x + y * y) / (2.0f * (s2))));
      MASK[xcount][ycount]=(int)(483*val);
      
      ycount++;
         
        
    }
    xcount++;  
     
  }

  int res=0;
  for(int i=min; i<f1.rows+min;i++){
    for(int j=min;j<f1.cols+min;j++){
      for(int x = min; x <= max; ++x){
        for(int y = min; y <= max; ++y){
          res=res+f2.at<uchar>(x+i,y+j)*MASK[x+max][y+max];

        }

      }
      if(res<0)
        res=0;
      E1_1.at<uchar>(i+max,j+max)=res;

    }
  }

  
  

  for(int x = 0; x <= 6; ++x)
  {
     cout<<endl;
    
    for(int y = 0; y <= 6; ++y)
    {
      
      cout<<MASK[x][y]<<" ";  
    }  


  }
  

 
 
 
  
 //Create the display window
  namedWindow("Unix Sample Skeleton");
  //Display loop
  bool loop = true;
  while(loop) {
    imshow("Unix Sample Skeleton", *image);
    
    switch(cvWaitKey(15)) {
      case 27:  //Exit display loop if ESC is pressed
        loop = false;
        break;
      case 32:  //Swap image pointer if space is pressed
        if(image == &f1) image = &E1_1;
        else image = &f1;
        break;
      default:
        break;
    }
  }
}


