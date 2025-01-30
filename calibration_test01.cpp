#include <iostream>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


int main(int argc, char** argv) {

	(void)argc;
	(void)argv;

	std::vector<cv::String> fileNames;
	cv::glob("Vision_project\\Cpluplus\\Dependencies\\checkerboard\\frames*.jpg", fileNames, false);
	cv::Size patternsize(9 - 1, 6 - 1);
	std::vector<std::vector<cv::Point2f>> q(fileNames.size());


	std::vector<std::vector<cv::Point3f>> Q;


	int cheakboard[2] = {9, 6}; //9*6 cm
	int fieldsize = 30; //30*30 mm

	std::vector<cv::Point3f> objp;

	for (int i = 1; i < cheakboard[1]; i++){
		for (int j = 0; j < cheakboard[0]; j++) {
			objp.push_back(cv::Point3f(j * fieldsize, i * fieldsize, 0));
		}
	}



	std::vector<cv::Point2f> imgPoint;
	std::size_t i = 0;
	for (auto const &f: fileNames)	{

		std::cout << std::string(f) << std::endl;


		cv::Mat img = cv::imread(fileNames[i]);
		cv::Mat gray;

		cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

		bool patternfound = cv::findChessboardCorners(gray, patternsize, q[i], cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK);

		if (patternfound) {

			cv::cornerSubPix(gray, q[i], cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.1));
			Q.push_back(objp);
			
		}




		//Display
		cv::drawChessboardCorners(img, patternsize, q[i], patternfound);
		cv::imshow("Checkboard Detection", img);
		cv::waitKey(0);

		i++;

	}




	cv::Matx33f K(cv::Matx33f::eye()); //Intrinsic parameters
	cv::Vec<float, 5> k(0,0,0,0,0);	  // Distorion Coefficients

	std::vector<cv::Mat> rvecs, tvecs;
	std::vector<double> stdIntrincs, stdExtrincs, perViewErrors;

	int flags = cv::CALIB_FIX_ASPECT_RATIO + cv::CALIB_FIX_K3 + cv::CALIB_ZERO_TANGENT_DIST + cv::CALIB_FIX_PRINCIPAL_POINT;
	cv::Size frameSize(1920, 1080);

	std::cout << "Calibrating ... " << std::endl;

	float error = cv::calibrateCamera(Q, q, frameSize, K, k, rvecs, tvecs, flags);
	std::cout << "Re-projection Error= " << error << "\nK =\n"
		<< K << "\nk =\n" << k << std::endl;



	//Pre-compute lens correction Interpolation
	cv::Mat mapX, mapY;
	cv::initUndistortRectifyMap(K, k, cv::Matx33f::eye(), K, frameSize, CV_32FC1, mapX, mapY);


	//Show lens corrections images
	for (auto const &f:fileNames){
	
		std::cout << std::string(f) << std::endl;
		cv::Mat img = cv::imread(f, cv::IMREAD_COLOR);
	
		cv::Mat imgUndistorted;


		//5. Remap the image using the precomputed interpolation maps
		cv::remap(img, imgUndistorted, mapX, mapY, cv::INTER_LINEAR);


		//Display
		cv::imshow("Undistorted image", imgUndistorted);
		cv::waitKey(0);

	}


	return 0;
}