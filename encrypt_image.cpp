//Usage ./app imagePath
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <sys/stat.h>
using namespace cv;
using namespace std;
int h = 20;
int w = 20;
int white = 0;
int black = 0;
vector<string> paths_n;
vector<string> paths_p;
void get_intensity(Mat im)
{
  for (int j = 0; j < im.rows; j++)
  {
    for (int i = 0; i < im.cols; i++)
    {
      int intensity = im.at<uchar>(j, i);
      if (intensity == 0)
      {
        black++;
      }
      else
      {
        white ++;
      }
    }
  }
}

void showimage(String image,Mat img,int wait=1)
{
  namedWindow(image, WINDOW_AUTOSIZE);
  imshow(image, img);
  if (wait == 0)
  {
  waitKey(0);
  }
}
void create_alphabet(string text)
{
    for(int i=0; i<text.length();i++)
    {
      Mat black_background(h,w,CV_8UC1,Scalar(0,0,0));
      Mat white_background(h,w,CV_8UC1,Scalar(255,255,255));
      Size textSize = getTextSize(text, FONT_HERSHEY_PLAIN,1,1,0);
      int flag = 0;
      int thicknessText = 1;
      while(flag != 1)
      {
          Mat dst = white_background.clone();
          putText(dst, string(1,text[i]),Point(0.1*w,0.75*h),FONT_HERSHEY_PLAIN, 1,  Scalar(0,0,0),thicknessText);
          get_intensity(dst);
          if( abs(white - black) < 0.8*white)
          {
            flag = 1;
          }
          else
          {
            thicknessText++;
          }
          black = 0;
          white = 0;
      }
      putText(white_background, string(1,text[i]),Point(0.2*w,0.75*h),FONT_HERSHEY_PLAIN, 1,  Scalar(0,0,0),thicknessText);
      putText(black_background, string(1,text[i]),Point(0.2*w,0.75*h),FONT_HERSHEY_PLAIN, 1,  Scalar(255,255,255),thicknessText);
      string path_w = string("./negative/img") + to_string(i) + ".jpg";
      string path_b = string("./positive/img") +  to_string(i) +".jpg";
      paths_n.push_back(path_w);
      paths_p.push_back(path_b);
      imwrite(path_b,black_background);
      imwrite(path_w,white_background);
      flag = 0;
      thicknessText =  0;
    }
}
int main(int argc, char** argv)
{
  if (argc != 2)
  {
    cout <<"Usage: ./app imagePath"<<endl;
  }
  else
  {
    mkdir("positive",0777);
    mkdir("negative",0777);
    system("exec rm ./negative/*");
    system("exec rm ./positive/*");
    string alphabets;
    cout <<"\nEnter 4 alphabets Example: abcd"<<endl;
    getline(cin,alphabets);
    char char_to_remove = ' ';
    alphabets.erase(remove(alphabets.begin(), alphabets.end(), char_to_remove), alphabets.end());
    alphabets = boost::to_upper_copy<string>(alphabets);
    cout<<"The alphabets are: "<<alphabets<<endl;
    create_alphabet(alphabets);
    Mat img = imread(argv[1],0);
    resize(img,img,Size(200 ,200), 0, 0, CV_INTER_LINEAR);
    Mat share1(img.rows,img.cols,CV_8UC1,Scalar(255,255,255));
    Mat share2(img.rows,img.cols,CV_8UC1,Scalar(255,255,255));
    resize(img,img,Size(img.cols * 5,img.rows * 5), 0, 0, CV_INTER_LINEAR);
    resize(share1,share1,Size(img.cols,img.rows), 0, 0, CV_INTER_LINEAR);
    resize(share2,share2,Size(img.cols,img.rows), 0, 0, CV_INTER_LINEAR);
    vector<cv::String> n_alph;
    vector<cv::String> p_alph;
    glob("./negative/*.jpg",n_alph);
    glob("./positive/*.jpg",p_alph);
    vector<Mat> n_images;
    vector<Mat> p_images;
    size_t count = paths_n.size();
    for(size_t i=0; i<count; i++)
    {
      Mat n_resize = imread(paths_n[i],0);
      Mat p_resize = imread(paths_p[i],0);
      n_images.push_back(n_resize);
      p_images.push_back(p_resize);
    }
    int AI = 1;
    int cnt = 0;
    for (int j = 0; j < img.rows; j+=n_images[0].rows)
    {
      for (int i = 0; i < img.cols; i+=n_images[0].cols)
      {
        AI = rand()%2;
        int intensity = img.at<uchar>(j, i);
        Mat key1 = n_images[cnt];
        Mat key2 = p_images[cnt];
        if (intensity > 100)
           {
             if (AI == 0)
             {
               key1.copyTo(share1(Rect(i,j,key1.cols,key1.rows)));
               key1.copyTo(share2(Rect(i,j,key1.cols,key1.rows)));
             }
             else
             {
               key2.copyTo(share1(Rect(i,j,key1.cols,key1.rows)));
               key2.copyTo(share2(Rect(i,j,key1.cols,key1.rows)));
             }
              cnt += 1;
            }
        else
           {
             if (AI == 1)
             {
               key1.copyTo(share1(Rect(i,j,key1.cols,key1.rows)));
               key2.copyTo(share2(Rect(i,j,key2.cols,key2.rows)));
             }
             else
             {
               key2.copyTo(share1(Rect(i,j,key1.cols,key1.rows)));
               key1.copyTo(share2(Rect(i,j,key2.cols,key2.rows)));
             }
             cnt += 1;
            }
            if (cnt == alphabets.length())
            {
              cnt = 0;
            }
          }
        }
    imwrite("imgShare1.png", share1);
		imwrite("imgShare2.png", share2);
    showimage("Encrypted Image 1",share1);
    showimage("Encrypted Image 2",share2);
    Mat finale(share1.rows,share1.cols,CV_8UC1,Scalar(255,255,255));
    for (int j = 0; j < share1.rows; j++)
    {
      for (int i = 0; i < share1.cols; i++)
      {
              int intensity_share1 = share1.at<uchar>(j, i);
              int intensity_share2 = share2.at<uchar>(j, i);
              finale.at<uchar>(j, i) = min(intensity_share1,intensity_share2);
      }
    }
    imwrite("reconstructed.png", finale);
    showimage("Reconstructed image",finale,0);
    return 0;
   }
}
