#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>
using namespace cv;
using namespace std;
int h = 20;
int w = 50;
int showimage(String image,Mat img,int wait=1)
{
  namedWindow(image, WINDOW_AUTOSIZE);
  imshow(image, img);
  if (wait == 0)
  {
  waitKey(0);
  }
}
void create_alphabet(String text)
{
    int count = 0;
    Mat black_background(h,w,CV_8UC3,Scalar(0,0,0));
    Mat white_background(h,w,CV_8UC3,Scalar(255,255,255));
    putText(white_background,text,Point(0,h/2),FONT_HERSHEY_PLAIN, 1,  Scalar(0,0,0),2);
    putText(black_background,text,Point(0,h/2),FONT_HERSHEY_PLAIN, 1,  Scalar(255,255,255),2);
    string path_b = string("/mnt/d/DeepLearning/lile/") + "text_p" +".jpg";
    string path_w = string("/mnt/d/DeepLearning/lile/") + "text_n" +".jpg";
    imwrite(path_b,black_background);
    imwrite(path_w,white_background);
}
int main(int argc, char** argv)
{
  string alphabets;
  cout <<"Enter 4 alphabets Example: abcd"<<endl;
  cin>>alphabets;
  cout<<"The alphabets are: "<<alphabets<<endl;
  create_alphabet(alphabets);
  Mat img = imread("0791.PNG",0);
  Mat key1 = imread("text_p.JPG",0);
  Mat key2 = imread("text_n.JPG",0);
  Mat share1(img.rows,img.cols,CV_8UC1,Scalar(255,255,255));
  Mat share2(img.rows,img.cols,CV_8UC1,Scalar(255,255,255));
  resize(img,img,Size(img.cols * 5,img.rows * 5), 0, 0, CV_INTER_LINEAR);
  resize(share1,share1,Size(img.cols,img.rows), 0, 0, CV_INTER_LINEAR);
  resize(share2,share2,Size(img.cols,img.rows), 0, 0, CV_INTER_LINEAR);
  resize(key1,key1,Size(key1.cols * 0.5,key1.rows*0.5), 0, 0, CV_INTER_LINEAR);
  resize(key2,key2,Size(key2.cols * 0.5,key2.rows*0.5), 0, 0, CV_INTER_LINEAR);
  int AI = 1;
  for (int j = 0; j < img.rows - 20; j += 0.5*key1.rows)
  {
    for (int i = 0; i < img.cols - 20; i+=0.5*key1.cols)
    {
      AI = rand()%2;
      cout<<AI<<endl;
      int intensity = img.at<uchar>(j, i);
      if (intensity > 100)
         {
           if (AI == 0)
           {
             key1.copyTo(img(Rect(i,j,key1.cols,key1.rows)));
             key1.copyTo(share1(Rect(i,j,key1.cols,key1.rows)));
             key1.copyTo(share2(Rect(i,j,key1.cols,key1.rows)));
             i = i+0.75*key1.cols;
           }
           else
           {
             key2.copyTo(share1(Rect(i,j,key1.cols,key1.rows)));
             key2.copyTo(share2(Rect(i,j,key1.cols,key1.rows)));
             i = i+0.75*key1.cols;
           }

          }
      else
         {
           if (AI == 1)
           {
             //key2.copyTo(img(Rect(i,j,key2.cols,key2.rows)));
             key1.copyTo(share1(Rect(i,j,key1.cols,key1.rows)));
             key2.copyTo(share2(Rect(i,j,key2.cols,key2.rows)));
             i = i+0.75*key2.cols;
           }
           else
           {
             key2.copyTo(share1(Rect(i,j,key1.cols,key1.rows)));
             key1.copyTo(share2(Rect(i,j,key2.cols,key2.rows)));
             i = i+0.75*key2.cols;
           }

          }
        }

        j = j+key1.rows;
      }
  showimage("Share1",share1);
  showimage("Share2",share2);
  showimage("Final",share1+share2);
  showimage("Processed Image",img,0);
}
