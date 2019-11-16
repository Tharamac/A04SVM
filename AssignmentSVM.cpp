#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>



using namespace std;
using namespace cv;
using namespace cv::ml;

const int hand = 5;
const long train_item = 25010;
const long test_item = 5000;
float Round(float var)
{
	// we use array of chars to store number 
	// as a string. 
	char str[40];

	// Print in string the value of var  
	// with two decimal point 
	sprintf_s(str, "%.1f", var);

	// scan string value in var  
	sscanf_s(str, "%f", &var);

	return var;
}


void main()
{
	ifstream trainfile;
	vector<int> traindata;
	//int traindata[train_item*11];
	float avg_hand_train[train_item][2];
	array<float, 2> avg_hand_tmp = { 0 , 0 };
	int label[train_item];
	vector<string> label_txt;
	const string txt[] = { "High Card", "Pair" , "Two Pairs" , "Three of A Kind" , "Straight" ,"Flush" ,"Fullhouse" , "Four of A Kind" ,"Straight Flush" , "Royal Flush" };
	cout << "Loading : 0%...";
	
	trainfile.open("poker-hand-data\\poker-hand-training-true.data");
	string s;
	cout << "33%...";
	
	//clrscreen();
	while (getline(trainfile, s)) {
		// Output the text from the file
		
		//for(size_t j =0 ; j < train_item ; j++){
		stringstream ss(s);
			for (int i; ss >> i;) {
				traindata.push_back(i);
				if (ss.peek() == ',')
					ss.ignore();
			}
	//	}
		//cout << get
	}
	cout << "66%...";
	//cout << count;
	//clrscreen();
	for (size_t i = 0; i < traindata.size(); i++) {
		if (i % 11 == 10) {
			label[i/11] = traindata[i];
			label_txt.push_back(txt[traindata[i]]);
			avg_hand_tmp[0] /= hand;
			avg_hand_tmp[1] /= hand;
			avg_hand_train[i/11][0] = avg_hand_tmp[0];
			avg_hand_train[i/11][1] = avg_hand_tmp[1];
			avg_hand_tmp[0] = 0;
			avg_hand_tmp[1] = 0;
		}
		else if (i % 11 % 2 == 0) 
			avg_hand_tmp[0] += traindata[i];
		else if (i % 11 % 2 == 1)
			avg_hand_tmp[1] += traindata[i];
	}
//	for (size_t i = 0; i < train_item; i++)
//		cout << avg_hand_train[i][0] << " " << avg_hand_train[i][1] << " " << label_txt[i] << endl;
	cout << "Complete!" <<  endl;
	/*int mode;
	cout << "Choose Prediction Mode:" << flush << endl;
	for (int i = 0; i <= 9; i++)
		cout << i << ") " << txt[i] << endl;
	cout << "Otherwise: Exit" << endl << ": ";
	cin >> mode;
	if (!(mode >= 0 && mode <= 9))return 0;
	for (int i = 0; i < label.size(); i++) {
		label[i] = (label[i] == mode ? 1 : -1);
		label_txt[i] = (label_txt[i] == txt[mode] ? label_txt[i] : "Other Hand");
	}
	*/
	
	trainfile.close();
	cout << "Training!" << endl;
	
	
	Mat trainingDataMat(train_item, 2, CV_32F, avg_hand_train);
	Mat labelsMat(train_item, 1, CV_32SC1, label);
//	for (size_t i = 0; i < train_item; i++)
//		cout << trainingDataMat[i][0] << " " << avg_hand_train[i][1] << " " << label[i] << " " << label_txt[i] << endl;
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
	svm->train(trainingDataMat, ROW_SAMPLE, labelsMat);
	
	ifstream testfile;
	vector<int>testdata;
	float avg_hand_test[test_item][2];
	avg_hand_tmp = { 0 , 0 };
	int label_test[test_item];
	vector<string> label_test_txt;
	//const string txt[] = { "High Card", "Pair" , "Two Pairs" , "Three of A Kind" , "Straight" ,"Flush" ,"Fullhouse" , "Four of A Kind" ,"Straight Flush" , "Royal Flush" };
	cout << "Testing : 0%...";
	int count = 0;
	testfile.open("poker-hand-data\\poker-hand-testing.data");
	cout << "25%...";
	//clrscreen();
	while (getline(testfile, s) && count++ < test_item ) {
		// Output the text from the file
	//	cout << count << ".";
		//for (size_t i = 0; i < test_item; i++) {
		stringstream st(s);
		for (int i; st >> i;) {
			testdata.push_back(i);
			if (st.peek() == ',')
				st.ignore();
		}
		//}
		
		//for (size_t i = 0; i < test_item; i++)
		
	}

	
	cout << "50%...";
	//clrscreen();
	int i = 0;
	for (i = 0; i < test_item*11; i++) {
		if (i % 11 == 10) {
			label_test[i / 11] = testdata[i];
			label_test_txt.push_back(txt[testdata[i]]);
			avg_hand_tmp[0] /= hand;
			avg_hand_tmp[1] /= hand;
			avg_hand_test[i / 11][0] = avg_hand_tmp[0];
			avg_hand_test[i / 11][1] = avg_hand_tmp[1];
			avg_hand_tmp[0] = 0;
			avg_hand_tmp[1] = 0;
		}
		else if (i % 11 % 2 == 0)
			avg_hand_tmp[0] += testdata[i];
		else if (i % 11 % 2 == 1)
			avg_hand_tmp[1] += testdata[i];
	}
	//for (size_t i = 0; i < test_item; i++)
//		cout  <<  i << ". "<< avg_hand_test[i][0] << " " << avg_hand_test[i][1] << " " << label_test_txt[i] << endl;
	testfile.close();
	
	int test_response[test_item];
	size_t correct = 0;
	cout << "75%...";
	for (size_t i = 0; i < test_item; i++) {
		Mat sampleMat(1, 2, CV_32F, avg_hand_test[i]);
		test_response[i] = (int)svm->predict(sampleMat);
		cout << label_test[i] << " ? " << test_response[i] <<endl;
		if (label_test[i] == (test_response[i]) ){
			correct++;
		}
	}
	
	cout << "Test Complete!" << endl;
	cout << "All test sample: " << test_item << " ; Correct: " << correct << endl;
	cout << "Accuracy: " << (float)correct * 100 / test_item << "%" << endl;
	
	
	// Data for visual representation
	int width =520 , height = 512;
	Mat image = Mat::zeros(height, width, CV_8UC3);

	// Show the decision regions given by the SVM
	//! [show]{
	Vec3b area[10] = {
		Vec3b(125,0,25),
		Vec3b(0,75,50),
		Vec3b(0,0,75),
		Vec3b(0,60,100),
		Vec3b(0,70,120),
		Vec3b(0,80,140),
		Vec3b(0,90,160),
		Vec3b(0,100,220),
		Vec3b(0,110,240),
		Vec3b(0,120,255),
	};
	//max value sign = 4;
 	//max value 
	/*
for (int i = 5; i <= 65; i++)	
	
	for (int j = 5; j <= 20; j++)for	if(j / 16 == 0)
	
		*/
	for (int i = 0; i < image.rows; i++)
	{
		 for(int j = 0; j < image.cols; j++)
		{
			float cal[] = { Round(i / 170.6) , 1.0 + Round(j / 43.3) };
			Mat sampleMat(1, 2, CV_32F, cal);
			//Mat sampleMat = (Mat_<float>(1, 2) << j/128.0, i/40.0);
			float response = svm->predict(sampleMat);
			cout << i << ". " << 1.0 + cal[0] << " " << cal[1] <<  "==" << response << endl;
			switch ((int)response)
			{
				case 0: image.at<Vec3b>(i, j) = area[0]; break;
				case 1: image.at<Vec3b>(i, j) = area[1]; break;
				case 2: image.at<Vec3b>(i, j) = area[2]; break;
				case 3: image.at<Vec3b>(i, j) = area[3]; break;
				case 4: image.at<Vec3b>(i, j) = area[4]; break;
				case 5: image.at<Vec3b>(i, j) = area[5]; break;
				case 6: image.at<Vec3b>(i, j) = area[6]; break;
				case 7: image.at<Vec3b>(i, j) = area[7]; break;
				case 8:	image.at<Vec3b>(i, j) = area[8]; break;
				case 9:	image.at<Vec3b>(i, j) = area[9]; break;
			}
		}
	}
	//! [show]
	/*
	// Show the training data
	//! [show_data]
	int thickness = -1;
	circle(image, Point(501, 10), 5, Scalar(0, 0, 0), thickness);
	circle(image, Point(255, 10), 5, Scalar(255, 255, 255), thickness);
	circle(image, Point(501, 255), 5, Scalar(255, 255, 255), thickness);
	circle(image, Point(10, 501), 5, Scalar(255, 255, 255), thickness);
	//! [show_data]

	// Show support vectors
	//! [show_vectors]
	thickness = 2;
	Mat sv = svm->getUncompressedSupportVectors();

	for (int i = 0; i < sv.rows; i++)
	{
		const float* v = sv.ptr<float>(i);
		circle(image, Point((int)v[0], (int)v[1]), 6, Scalar(128, 128, 128), thickness);
	}
	//! [show_vectors]
	*/
	imwrite("result.png", image);        // save the image

	imshow("SVM Simple Example", image); // show it to the user
	
	waitKey();
	return;
	
}