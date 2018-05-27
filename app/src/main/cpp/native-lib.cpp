#include "b_big_myeyetraccker_OpencvClass.h"
#include <jni.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>


using namespace cv;
using namespace std;



JNIEXPORT void JNICALL
Java_b_big_myeyetraccker_OpencvClass_faceDetection(JNIEnv *, jclass, jlong addrRgba) {

    Mat& frame = *(Mat*)addrRgba;




    detect(frame);

}



void detect(Mat& frame) {

    cv::String face_cascade_name = "/storage/emulated/0/lbpcascade_frontalface.xml";
    cv::String eyes_cascade_name = "/storage/emulated/0/haarcascade_eye.xml";
    CascadeClassifier face_cascade;
    CascadeClassifier eyes_cascade;




    if( !face_cascade.load(face_cascade_name) ){
        printf("--(!)Error loading face cascade\n");
        return ;
    };
    if( !eyes_cascade.load( eyes_cascade_name ) ){
        printf("--(!)Error loading eyes cascade\n");
        return ;
    };

        std::vector<Rect> faces;
        Mat frame_gray, resized_frame;
        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
        const int scale = 2;

        cv::resize(frame, resized_frame, cv::Size(), 0.5, 0.5);
        //-- Detect faces
        face_cascade.detectMultiScale(resized_frame, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE,
                                      Size(30, 30));
        for (size_t i = 0; i < faces.size(); i++) {
            Point center(faces[i].x*2 + faces[i].width , faces[i].y*2 + faces[i].height );
           ellipse(frame, center, Size(faces[i].width , faces[i].height), 0, 0, 360,
                    Scalar(255, 0, 255), 2, 8, 0);
            Mat faceROI = resized_frame(faces[i]);
            std::vector<Rect> eyes;
            //-- In each face, detect eyes
            eyes_cascade.detectMultiScale(faceROI, eyes, 1.2, 2, 0 | CASCADE_SCALE_IMAGE,
                                          Size(5, 5));
            for (size_t j = 0; j < eyes.size(); j++) {
                Point eye_center((faces[i].x + eyes[j].x+ eyes[j].width/2)*2,
                                 (faces[i].y + eyes[j].y+ eyes[j].height/2)*2 );
                int radius = cvRound((eyes[j].width + eyes[j].height) * 0.10);
                circle(frame, eye_center, radius, Scalar(255, 0, 0), 2, 8, 0);
            }
        }
    }

